# BÀI TẬP LỚN: MẬT MÃ VÀ AN NINH MẠNG (HK251 - CO2069)

Trường Đại học Bách khoa - ĐHQG TP.HCM
Khoa Khoa học & Kỹ thuật Máy tính

## Đề tài 7: Bảo mật Internet vạn vật (IoT)

Repo này chứa mã nguồn cho bài tập lớn của môn Mật mã và An ninh mạng.

---

### 1. Thông tin nhóm

| STT | Họ và Tên       | MSSV    | Email                           |
| :-- | :-------------- | :------ | :------------------------------ |
| 1   | Trương Anh Khôi | 2211707 | khoi.truongtak0601@hcmut.edu.vn |
| 2   | Phạm Gia Lương  | 2211960 | luong.pham2211960@hcmut.edu.vn  |

### 2. Mô tả đề tài

Mục tiêu của đề tài này là chỉ ra các lỗ hổng bảo mật trong một hệ thống IoT cơ bản và sau đó triển khai các biện pháp để khắc phục.

### 3. Các nội dung cần thực hiện

Các nhiệm vụ chính của dự án này bao gồm:

- **Tìm hiểu:** Nghiên cứu về bảo mật Internet vạn vật, tầm quan trọng và các khác biệt so với bảo mật thông thường.
- **Nghiên cứu tấn công:** Tìm hiểu các phương thức đối phó với những loại hình tấn công phổ biến trên nền tảng IoT.
- **Triển khai:** Lựa chọn và triển khai một số biện pháp đối phó (dựa trên mã nguồn mở) để bảo mật hệ thống IoT.
- **Đánh giá:** Thử nghiệm và đánh giá hiệu quả của các giải pháp đã triển khai.

---

### 4. Hướng dẫn sử dụng (Mô phỏng)

Phần này mô tả cách chạy mã nguồn mô phỏng (trong thư mục `sres/`).

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
    - **Mật khẩu:** `mmanm251`
6.  Kết nối vào mạng AP này và truy cập IP `192.168.4.1` để cấu hình Wi-Fi cho thiết bị.
7.  Sau khi kết nối Wi-Fi thành công, thiết bị sẽ tự động kết nối đến MQTT Broker và bắt đầu gửi dữ liệu.
8.  Sử dụng MQTT Client để theo dõi topic: `hcmut/co3069/iot_security/data` và xem dữ liệu.

---

### 5. Cấu trúc thư mục

```

.
├── README.md           \# Hướng dẫn sử dụng (tệp này)
├── platformio.ini      \# Cấu hình dự án và các thư viện cho PlatformIO
├── include/
├── lib/
├── src/
└── report.pdf          \

```
