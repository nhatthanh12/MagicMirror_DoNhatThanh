/* MagicMirror² Config - Nhật Thành Final (Clean Energy Monitor) */

let config = {
	address: "localhost",	
	port: 8080,
	basePath: "/",	
	ipWhitelist: ["127.0.0.1", "::ffff:127.0.0.1", "::1"],	

	language: "vi",           
	locale: "vi-VN",
	timeFormat: 24,           
	units: "metric",          

	modules: [
		{
			module: "alert",
		},
		{
			module: "updatenotification",
			position: "top_bar"
		},
		/* --- COT BEN TRAI: DONG HO & PIR --- */
		{   
			module: "clock",
			position: "top_left",
			config: {
				displayType: "both" 
			}
		},
		{
			module: 'MMM-Pir',
			position: 'top_left', 
			config: {
				// Đưa các thông số hiển thị vào nhóm Display
				Display: {
					mode: 4,              // vcgencmd cho Pi 4
					timeout: 30000,       // Thời gian chờ 30s
					counter: true,
					style: 1
				},
				// Đưa thông số cảm biến vào nhóm Pir
				Pir: {
					gpio: 17,             // Chân GPIO 17 (Vật lý 11)
					reverseValue: true    // Để true vì máy bạn đang bị ngược logic
				}
			}
		},
		{
			module: "calendar",
			header: "LICH NGHI LE",
			position: "top_left",
			config: {
				calendars: [
					{
						symbol: "calendar-check",
						url: "https://www.google.com/calendar/ical/vi.vietnamese%23holiday%40group.v.calendar.google.com/public/basic.ics"
					}
				]
			}
		},

		/* --- COT BEN PHAI: THOI TIET & NANG LUONG --- */
		{
			module: "weather",
			position: "top_right",
			header: "THOI TIET",
			config: {
				weatherProvider: "openmeteo",
				type: "current",
				lat: 21.0285,   
				lon: 105.8542
			}
		},
		{
			module: "MMM-JsonTable",
			position: "top_right",
			header: "GIAM SAT NANG LUONG",
			config: {
				url: "http://192.168.1.9/data", 
				updateInterval: 2000,
				descriptiveKeys: {
					"P": "CONG SUAT (W):",
					"E": "TONG DIEN (kWh):",
					"Money": "GIA TIEN (VND):",
					"temp": "NHIET DO (C):",
					"humi": "DO AM (%):"
				}
			}
		},

		/* --- PHAN DUOI: LOI CHAO & TIN TUC --- */
		{
			module: "compliments",
			position: "lower_third",
			config: {
				compliments: {
					anytime: ["CHAO NHAT THANH!", "HE THONG CHAY TOT!"],
					morning: ["CHAO BUOI SANG!", "NGAY MOI NANG LUONG!"],
					evening: ["NGHI NGOI THOI THANH OI!"]
				}
			}
		},
		{
			module: "newsfeed",
			position: "bottom_bar",
			config: {
				feeds: [
					{
						title: "VnExpress",
						url: "https://vnexpress.net/rss/tin-moi-nhat.rss"
					}
				],
				showSourceTitle: true,
				showPublishDate: true
			}
		},
	]
};

/*************** DO NOT EDIT THE LINE BELOW ***************/
if (typeof module !== "undefined") { module.exports = config; }