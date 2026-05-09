# MagicMirror_DoNhatThanh ( Một dự án do Nhật Thành Thực hiện)
-Sử dụng Source Code của https://github.com/MagicMirrorOrg/MagicMirror.git để làm nền tảng cho Magic_Mirror.
-Download module git clone https://github.com/Coernel82/MMM-Pir.git để sử dụng đc cảm biến PIR phát hiện người ra vào tại folder module của Magic_Mirror download ở bên trên.
-Download module git clone https://github.com/timdows/MMM-JsonTable.git cũng tại folder module của Magic_Mirror, module này phục vụ nhận dữ liệu dnagj JSON file của ESP32 cung cấp thông tin về nhiệt độ, độ ẩm(AHT10) và Điện năng tiêu thụ (PZEM-004T).
-Ngoài ra có thể tích hợp thêm nhiều module khác như: module tương tác bằng âm thanh, moduel cảm ứng nếu dùng màn hình cảm ứng,...
-Sau đó vào config và chỉnh sửa file config.js để tương tác đc với các moduel vừa tải về , rồi chạy npm run start là chạy đc chương trình.(file config.js bên trên đã đc sửa và có thể chạy).
-Code chạy ESP32 cũng kèm theo và chỉ cần nạp là chạy đc( lưu ý esp32 và raspberry pi4 phải bắt cùng 1 mạng wifi để có cùng ip và nếu cần hãy vào lại file config.js ở thư mục config để chỉnh lại ip thì raspberry4 mới lấy đc dữ liệu của espe32).
-Ảnh sản phẩm kèm thèo.
<img width="2048" height="1536" alt="MagicMirror3" src="https://github.com/user-attachments/assets/8017fe41-005b-46ab-b566-7daeea17ec8e" />
<img width="2048" height="1536" alt="MagicMirror2" src="https://github.com/user-attachments/assets/ee1958fa-986b-4067-8d08-973fadb50c1a" />
<img width="2048" height="1536" alt="MagicMirror1" src="https://github.com/user-attachments/assets/bb1a82b7-3670-4fe9-b3cb-d88a5f71ff35" />


