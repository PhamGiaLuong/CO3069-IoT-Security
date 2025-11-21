import paho.mqtt.client as mqtt
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
import base64
import json
import sys

# --- CẤU HÌNH (Phải khớp tuyệt đối với code trên ESP32) ---
MQTT_BROKER = "broker.hivemq.com"
MQTT_PORT = 1883
MQTT_TOPIC = "hcmut/co3069/iot_security/data"

# KHÓA BÍ MẬT & IV (Phải giống hệt trong file crypto_helper.h của bạn)
# Nếu bạn đổi trong code ESP32, hãy đổi lại ở đây.
AES_KEY = b"1234567890123456"  # 16 bytes
AES_IV  = b"abcdefghijklmnop"  # 16 bytes

def decrypt_message(encrypted_base64):
    try:
        # 1. Base64 Decode: Chuyển chuỗi ký tự về lại binary mã hóa
        encrypted_bytes = base64.b64decode(encrypted_base64)
        
        # 2. Khởi tạo bộ giải mã AES-CBC
        cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
        
        # 3. Giải mã
        decrypted_padded = cipher.decrypt(encrypted_bytes)
        
        # 4. Unpadding (Cắt bỏ các byte đệm PKCS7)
        decrypted_bytes = unpad(decrypted_padded, AES.block_size)
        
        # 5. Chuyển về String UTF-8
        return decrypted_bytes.decode('utf-8')
    except Exception as e:
        return None

# Callback khi kết nối thành công
def on_connect(client, userdata, flags, rc):
    print(f"Connected to Broker with result code {rc}")
    client.subscribe(MQTT_TOPIC)
    print(f"Subscribed to topic: {MQTT_TOPIC}")
    print("Waiting for encrypted messages...\n")

# Callback khi nhận tin nhắn
def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    print("-" * 50)
    print(f"Received Encrypted (Ciphertext): {payload}")
    
    # Thực hiện giải mã
    decrypted_json = decrypt_message(payload)
    
    if decrypted_json:
        print(f"\n>> DECRYPTED SUCCESS (Plaintext): {decrypted_json}")
        
        # Thử phân tích JSON để hiển thị đẹp hơn
        try:
            data = json.loads(decrypted_json)
            print(f">> Sensor Data:")
            print(f"   - Device ID: {data.get('device_id')}")
            print(f"   - Temp:      {data.get('temperature')} °C")
            print(f"   - Humidity:  {data.get('humidity')} %")
            print(f"   - Status:    {data.get('status')}")
        except json.JSONDecodeError:
            print("   (Content is not valid JSON)")
    else:
        print("\n>> DECRYPTION FAILED! (Check Key/IV or Padding)")
    print("-" * 50 + "\n")

# Thiết lập Client
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

print(f"Connecting to {MQTT_BROKER}...")
try:
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_forever()
except KeyboardInterrupt:
    print("\nStopped by user.")
    sys.exit(0)
except Exception as e:
    print(f"Connection failed: {e}")