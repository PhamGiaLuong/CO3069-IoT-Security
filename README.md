# BÀI TẬP LỚN: MẬT MÃ VÀ AN NINH MẠNG (HK251 - CO3069)

Khoa Khoa học & Kỹ thuật Máy tính, Trường Đại học Bách khoa - ĐHQG TP.HCM

## Đề tài 7: Bảo mật Internet vạn vật (IoT)

Repo này chứa mã nguồn cho bài tập lớn của môn Mật mã và An ninh mạng.

---

### 1. Thông tin nhóm

| STT | Họ và Tên       | MSSV    | Email                           |
| :-- | :-------------- | :------ | :------------------------------ |
| 1   | Trương Anh Khôi | 2211707 | khoi.truongtak0601@hcmut.edu.vn |
| 2   | Phạm Gia Lương  | 2211960 | luong.pham2211960@hcmut.edu.vn  |

### 2. Mô tả đề tài

Đề tài tập trung vào việc giải quyết vấn đề bảo mật dữ liệu (Confidentiality) trong giao tiếp giữa thiết bị IoT và máy chủ thông qua giao thức MQTT. Trong các hệ thống IoT cơ bản, dữ liệu thường được gửi dưới dạng văn bản thuần (plaintext), khiến chúng dễ bị nghe lén và đánh cắp thông tin.

Dự án này xây dựng một hệ thống IoT minh họa gồm:

- **Thiết bị IoT (ESP32):** Đóng vai trò thu thập dữ liệu môi trường (Nhiệt độ, Độ ẩm). Thiết bị có khả năng hoạt động ở chế độ Access Point (AP) để cấu hình WiFi ban đầu.
- **Cơ chế bảo mật:** Triển khai mã hóa đầu cuối (End-to-End Encryption) ngay trên vi điều khiển. Dữ liệu cảm biến được đóng gói thành JSON, sau đó mã hóa bằng thuật toán **AES-128 (Chế độ CBC)** và mã hóa **Base64** trước khi gửi đi.
- **Giao thức truyền thông:** Sử dụng MQTT để gửi dữ liệu đã mã hóa lên Broker công cộng (`broker.hivemq.com`).
- **Client (Python):** Một ứng dụng trên máy tính đóng vai trò nhận dữ liệu từ MQTT Broker, thực hiện giải mã AES để khôi phục và hiển thị thông tin gốc.

### 3. Các nội dung cần thực hiện

Các nhiệm vụ chính của dự án này bao gồm:

- **Tìm hiểu:** Nghiên cứu về bảo mật Internet vạn vật, tầm quan trọng và các khác biệt so với bảo mật thông thường.
- **Nghiên cứu tấn công:** Tìm hiểu các phương thức đối phó với những loại hình tấn công phổ biến trên nền tảng IoT.
- **Triển khai:** Lựa chọn và triển khai một số biện pháp đối phó (dựa trên mã nguồn mở) để bảo mật hệ thống IoT.
- **Đánh giá:** Thử nghiệm và đánh giá hiệu quả của các giải pháp đã triển khai.

---

### 4. Hướng dẫn sử dụng (Mô phỏng)

Phần này mô tả cách chạy mã nguồn mô phỏng (trong thư mục `srcs/`).

#### Yêu cầu

- Phần cứng: 01x ESP32
- Phần mềm:
  - Visual Studio Code
  - PlatformIO IDE
  - Một MQTT Client (ví dụ: [HiveMQ Web Client](https://www.hivemq.com/demos/websocket-client/) hoặc MQTTX)

#### Cài đặt và chạy

1.  Clone repository này về máy.
2.  Mở dự án bằng VS Code và PlatformIO.
3.  Biên dịch (Build) và Nạp (Upload) mã nguồn vào ESP32.
4.  Mở Serial Monitor (Baud rate: `115200`) để theo dõi log.
5.  Thiết bị sẽ khởi động ở chế độ Access Point (AP).
    - **Tên (SSID):** `IoT-Security-AP`
    - **Mật khẩu:** `IoTs-{MAC}` (xem trong console)
6.  Kết nối vào mạng AP này và truy cập IP `192.168.4.1` để cấu hình Wi-Fi cho thiết bị.
7.  Sau khi kết nối Wi-Fi thành công, thiết bị sẽ tự động kết nối đến MQTT Broker và bắt đầu gửi dữ liệu.
8.  Sử dụng MQTT Client để theo dõi topic: `hcmut/co3069/iot_security/data` và xem dữ liệu.

---

### 5. Cấu trúc thư mục

```
.
├── client/
│   └── mqtt_receiver.py    # Script Python nhận, giải mã và hiển thị dữ liệu từ MQTT
├── include/                # Các file header (.h) khai báo hàm và biến toàn cục
│   ├── accessPoint.h       # Header cho module WiFi AP và Web Server
│   ├── crypto_helper.h     # Header chứa khóa bí mật (Key, IV) và khai báo hàm mã hóa
│   ├── global.h            # Các biến toàn cục và thư viện dùng chung
│   ├── mqttClient.h        # Header cho module xử lý MQTT
│   └── sensorMonitor.h     # Header cho module đọc cảm biến và xử lý dữ liệu
├── lib/                    # Các thư viện phụ thuộc (ArduinoJson, DHT, PubSubClient,...)
├── src/                    # Mã nguồn chính cho ESP32 (.cpp)
│   ├── accessPoint.cpp     # Xử lý chế độ AP, Web Server cấu hình WiFi (Captive Portal)
│   ├── crypto_helper.cpp   # Thực thi mã hóa AES-128-CBC và Base64
│   ├── global.cpp          # Định nghĩa các biến toàn cục và hàm tiện ích (log, timestamp)
│   ├── main.cpp            # Điểm bắt đầu chương trình, khởi tạo các FreeRTOS Task
│   ├── mqttClient.cpp      # Quản lý kết nối MQTT, kết nối lại và gửi tin nhắn
│   └── sensorMonitor.cpp   # Task đọc cảm biến (hoặc giả lập dữ liệu), đóng gói JSON và gọi hàm mã hóa
├── platformio.ini          # File cấu hình dự án PlatformIO (board, baud rate, thư viện)
└── README.md               # Hướng dẫn sử dụng và thông tin dự án
```
