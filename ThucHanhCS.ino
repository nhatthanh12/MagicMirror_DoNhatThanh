#include <WiFi.h>
#include <WebServer.h>
#include <PZEM004Tv30.h>
#include <Adafruit_AHTX0.h>
#include <ArduinoJson.h>
#include <WiFiManager.h> 
#include <ESPmDNS.h> // 1. Thêm thư viện mDNS

WebServer server(80);
PZEM004Tv30 pzem(Serial2, 16, 17); 
Adafruit_AHTX0 aht;

// --- GIỮ NGUYÊN HÀM TÍNH TIỀN ĐIỆN ---
float calculateEVN(float kwh) {
  float money = 0;
  if (kwh <= 50) money = kwh * 1806;
  else if (kwh <= 100) money = 50 * 1806 + (kwh - 50) * 1866;
  else if (kwh <= 200) money = 50 * 1806 + 50 * 1866 + (kwh - 100) * 2167;
  else if (kwh <= 300) money = 50 * 1806 + 50 * 1866 + 100 * 2167 + (kwh - 200) * 2729;
  else if (kwh <= 400) money = 50 * 1806 + 50 * 1866 + 100 * 2167 + 100 * 2729 + (kwh - 300) * 3050;
  else money = 50 * 1806 + 50 * 1866 + 100 * 2167 + 100 * 2729 + 100 * 3050 + (kwh - 400) * 3151;
  return money;
}

// --- GIỮ NGUYÊN GIAO DIỆN HTML NHẬT THÀNH ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="utf-8">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css">
  <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700&family=Inter:wght@300;500;700&display=swap" rel="stylesheet">
  <style>
    :root { --primary: #00f2ff; --secondary: #7000ff; --bg: #050505; --glass: rgba(255, 255, 255, 0.05); --gold: #ffd700; }
    html { font-family: 'Inter', sans-serif; background: var(--bg); color: white; margin: 0; overflow-x: hidden; }
    body { min-height: 100vh; display: flex; flex-direction: column; align-items: center; background: radial-gradient(circle at 50% -20%, #1a1a2e, #050505); padding: 40px 20px; }
    .author-box { background: linear-gradient(90deg, var(--secondary), var(--primary)); padding: 2px; border-radius: 50px; margin-bottom: 40px; box-shadow: 0 0 20px rgba(0, 242, 255, 0.2); }
    .author-inner { background: var(--bg); padding: 10px 30px; border-radius: 50px; display: flex; align-items: center; gap: 15px; }
    .author-inner i { color: var(--primary); }
    .author-name { font-family: 'Orbitron', sans-serif; font-size: 0.9rem; letter-spacing: 2px; text-transform: uppercase; font-weight: 700; }
    h2 { font-family: 'Orbitron', sans-serif; font-size: 2.2rem; margin-bottom: 40px; background: linear-gradient(to right, #fff, var(--primary)); -webkit-background-clip: text; -webkit-text-fill-color: transparent; text-align:center; }
    .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 25px; width: 100%; max-width: 1100px; }
    .card { background: var(--glass); backdrop-filter: blur(15px); padding: 30px; border-radius: 30px; border: 1px solid rgba(255,255,255,0.1); position: relative; overflow: hidden; transition: all 0.4s cubic-bezier(0.175, 0.885, 0.32, 1.275); }
    .card::before { content: ''; position: absolute; top: 0; left: 0; width: 100%; height: 5px; background: linear-gradient(90deg, transparent, var(--primary), transparent); }
    .card.money-card::before { background: linear-gradient(90deg, transparent, var(--gold), transparent); }
    .card:hover { transform: translateY(-10px); background: rgba(255,255,255,0.08); border-color: var(--primary); }
    .card-content { display: flex; justify-content: space-between; align-items: center; }
    .icon-circle { width: 60px; height: 60px; border-radius: 50%; background: rgba(0, 242, 255, 0.1); display: flex; align-items: center; justify-content: center; font-size: 1.5rem; color: var(--primary); }
    .money-card .icon-circle { color: var(--gold); background: rgba(255, 215, 0, 0.1); }
    .label { font-size: 0.75rem; text-transform: uppercase; color: #888; letter-spacing: 1px; margin-bottom: 8px; }
    .value { font-family: 'Orbitron', sans-serif; font-size: 1.8rem; font-weight: 700; color: #fff; }
    .unit { font-size: 0.9rem; color: var(--primary); margin-left: 5px; opacity: 0.8; }
    .unit-money { color: var(--gold); }
  </style>
</head>
<body>
  <div class="author-box"><div class="author-inner"><i class="fas fa-code"></i><div class="author-name">Designed by Nhật Thành</div></div></div>
  <h2>SMART ENERGY MONITOR</h2>
  <div class="grid">
    <div class="card"><div class="card-content"><div><div class="label">Voltage</div><span id="v" class="value">0</span><span class="unit">V</span></div><div class="icon-circle"><i class="fas fa-bolt"></i></div></div></div>
    <div class="card"><div class="card-content"><div><div class="label">Current</div><span id="i" class="value">0</span><span class="unit">A</span></div><div class="icon-circle"><i class="fas fa-microchip"></i></div></div></div>
    <div class="card"><div class="card-content"><div><div class="label">Power</div><span id="p" class="value">0</span><span class="unit">W</span></div><div class="icon-circle"><i class="fas fa-plug"></i></div></div></div>
    <div class="card"><div class="card-content"><div><div class="label">Energy</div><span id="e" class="value">0</span><span class="unit">kWh</span></div><div class="icon-circle"><i class="fas fa-history"></i></div></div></div>
    <div class="card money-card"><div class="card-content"><div><div class="label">Cost (EVN)</div><span id="m" class="value">0</span><span class="unit unit-money">VNĐ</span></div><div class="icon-circle"><i class="fas fa-hand-holding-usd"></i></div></div></div>
    <div class="card"><div class="card-content"><div><div class="label">Temp/Humi</div><span id="t" class="value">0</span><span class="unit">°C</span> | <span id="h" class="value">0</span><span class="unit">%</span></div><div class="icon-circle"><i class="fas fa-cloud-sun"></i></div></div></div>
  </div>
  <script>
    function update() {
      fetch('/data').then(r => r.json()).then(jsonResponse => {
        const d = jsonResponse[0];
        document.getElementById("v").innerText = d.Voltage.toFixed(1);
        document.getElementById("i").innerText = d.I.toFixed(2);
        document.getElementById("p").innerText = d.P.toFixed(1);
        document.getElementById("e").innerText = d.E.toFixed(3);
        document.getElementById("m").innerText = d.Money.toLocaleString('vi-VN');
        document.getElementById("t").innerText = d.temp.toFixed(1);
        document.getElementById("h").innerText = d.humi.toFixed(1);
      }).catch(e => console.log("Error"));
    }
    setInterval(update, 2000); update();
  </script>
</body>
</html>)rawliteral";

void handleData() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  float v = pzem.voltage(), i = pzem.current(), p = pzem.power(), e = pzem.energy();
  float cost = calculateEVN(isnan(e) ? 0 : e);
  StaticJsonDocument<512> doc;
  doc["Voltage"] = isnan(v) ? 0 : v;
  doc["I"] = isnan(i) ? 0 : i;
  doc["P"] = isnan(p) ? 0 : p;
  doc["E"] = isnan(e) ? 0 : e;
  doc["Money"] = (int)cost; 
  doc["temp"] = isnan(temp.temperature) ? 0 : temp.temperature;
  doc["humi"] = isnan(humidity.relative_humidity) ? 0 : humidity.relative_humidity;
  String res; 
  serializeJson(doc, res);
  String arrayRes = "[" + res + "]"; 
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", arrayRes);
}

void setup() {
  Serial.begin(115200);
  aht.begin();
  
  WiFiManager wm;
  bool res = wm.autoConnect("Energy_Monitor_AP");

  if(!res) {
    Serial.println("Kết nối thất bại, đang khởi động lại...");
    ESP.restart();
  } 

  Serial.println("\nĐã kết nối! IP: " + WiFi.localIP().toString()); 

  // 2. KHỞI TẠO MDNS VỚI TÊN MIỀN "nhatthanh"
  if (!MDNS.begin("nhatthanh")) { 
    Serial.println("Lỗi thiết lập mDNS!");
  } else {
    Serial.println("mDNS đã chạy! Truy cập tại: http://nhatthanh.local");
  }

  server.on("/", [](){ server.send(200, "text/html", index_html); });
  server.on("/data", handleData);
  server.begin();
}

void loop() { server.handleClient(); }

//http://nhatthanh.local