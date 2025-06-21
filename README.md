# 🌟 Chào mừng bạn đến với thiên đường ESP 🌟

Đây tiếp tục là một dự án nho nhỏ ứng dụng **ESP32** trong việc **quản lý bãi đậu xe thông minh**, sử dụng các **cảm biến siêu âm SRF05**, **cảm biến hồng ngoại**, **Servo motor**, **LCD**, **Firebase** và **Dashboard giao diện web**.

---

## 🎯 Nguyên lý hoạt động

- Khi có xe đi vào:
  - Cảm biến **siêu âm SRF05** phát hiện và gửi tín hiệu đến ESP32.
  - ESP32 điều khiển **Servo** mở thanh chắn.
  - **Buzzer** phát âm báo, **LED** sáng trong 2 giây.
  - **LCD** và **Dashboard** hiển thị thông báo "Mời xe đi qua".

- Khi xe đến khu vực đậu:
  - **Cảm biến hồng ngoại** phát hiện vật cản tại chỗ đậu.
  - **LCD** và **Dashboard** cập nhật trạng thái: còn trống hoặc đã có xe.

- Ngoài ra, trên Dashboard còn có một công tắc để **điều khiển thủ công** thanh chắn khi cần thiết.

---

## 📦 Danh sách linh kiện

Xem chi tiết trong file: [`Linh_kiện.pdf`](./Linh_kiện.pdf)

---

## 🚀 Các bước triển khai hệ thống Smart Parking

### 🔧 Bước 1: Chuẩn bị giao diện Dashboard

1. Tải 2 file sau:
   - `index.html`
   - `style.css`
2. Đặt chung trong một thư mục.
3. Mở bằng **Visual Studio Code** (hoặc trình soạn thảo khác).
4. Chạy `index.html` để xem giao diện Dashboard.

---

### ☁️ Bước 2: Cấu hình Firebase

1. Truy cập [Firebase Console](https://console.firebase.google.com/) và tạo một **Realtime Database** mới.
   Ví dụ:

    ![image](https://github.com/user-attachments/assets/1710f44d-6f3c-4b13-b9db-5013550604b2)
3. Mở tab `Authentication` → nhấn **Start** → chọn **Anonymous** → **Enable**.
4. Lấy đường dẫn **Firebase Database URL** và dán vào file `index.html`.

---

### 🔌 Bước 3: Nạp code vào ESP32

1. Mở file `Smart_Parking.ino` bằng **Arduino IDE**.
2. Chỉnh sửa các thông tin:
   - WiFi SSID và Password.
   - Địa chỉ Firebase Database của bạn.
3. Kết nối ESP32 với máy tính và **nạp code**.

📌 *Mình đã để hướng dẫn cụ thể trong phần chú thích của code.*

---

### ✅ Bước 4: Tận hưởng thành quả

Chúc mừng bạn đã triển khai xong hệ thống **Smart Parking** với:
- Giao tiếp **Realtime** với Firebase.
- Tự động hóa thanh chắn khi có xe ra vào.
- Theo dõi và hiển thị **tình trạng bãi đậu xe**.

Link dashboard của mình: https://thien2411-vn.github.io/SmartParking_ESP32/

---

## 🎥 Video Demo

[![image](https://github.com/user-attachments/assets/6b6a0c7f-b6cf-4569-b616-cd5dc3180d6f)](https://www.youtube.com/watch?v=PYl_N5S4V6w)

---

Nếu bạn thấy dự án hữu ích, hãy ⭐ star hoặc chia sẻ để nhiều người biết đến nhé!

