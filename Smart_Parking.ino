#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// WiFi
#define WIFI_SSID "92" //thay đổi thành tên wifi của bạn
#define WIFI_PASSWORD "1234567891011" // thay đổi thành mk wifi của bạn

// Firebase
#define FIREBASE_HOST "https://parking-management-3feda-default-rtdb.firebaseio.com/" // thay đổi thành link realtime database của bạn
#define FIREBASE_AUTH "q1CpEWruzY8w97rKNrokwB2eObqJqyBk3fiKTXaw" // thanh đổi thành địa chỉ firebase của bạn

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// SRF05
#define TRIG_PIN 5
#define ECHO_PIN 18

// Servo, Buzzer, LED
#define SERVO_PIN 23
#define BUZZER_PIN 26
#define LED_PIN 25

// IR sensor
#define IR_A1 32
#define IR_A2 33
#define IR_A3 27

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C 16x2
Servo myServo;

// Trạng thái
bool vehicleDetected = false;
unsigned long detectTime = 0;
bool servoReturned = false;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Pin Mode
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(IR_A1, INPUT);
  pinMode(IR_A2, INPUT);
  pinMode(IR_A3, INPUT);

  // Servo
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400);
  myServo.write(90);  // ban đầu đóng

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Đã kết nối WiFi");

  // Firebase
  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Hiển thị LCD mặc định
  lcd.setCursor(0, 0);
  lcd.print("A1:E A2:E A3:E");
  lcd.setCursor(0, 1);
  lcd.print("Wellcome !");
}

void loop() {
  // Đọc IR sensor
  bool a1Full = digitalRead(IR_A1) == LOW;
  bool a2Full = digitalRead(IR_A2) == LOW;
  bool a3Full = digitalRead(IR_A3) == LOW;

  // Cập nhật LCD dòng đầu
  lcd.setCursor(0, 0);
  lcd.print("A1:");
  lcd.print(a1Full ? "F" : "E");
  lcd.print(" A2:");
  lcd.print(a2Full ? "F" : "E");
  lcd.print(" A3:");
  lcd.print(a3Full ? "F" : "E");

  // Gửi trạng thái lên Firebase
  Firebase.setString(fbdo, "/parking/A1", a1Full ? "Full" : "Empty");
  Firebase.setString(fbdo, "/parking/A2", a2Full ? "Full" : "Empty");
  Firebase.setString(fbdo, "/parking/A3", a3Full ? "Full" : "Empty");

  // Kiểm tra điều khiển thủ công từ Firebase
  if (Firebase.getString(fbdo, "/parking/servo")) {
    String servoState = fbdo.stringData();
    if (servoState == "ON") {
      myServo.write(0); // mở
    } else if (servoState == "OFF") {
      myServo.write(90); // đóng
    }
  }

  float distance = getDistance();

  // Phát hiện xe vào
  if (distance > 0 && distance < 7) {
    if (!vehicleDetected) {
      vehicleDetected = true;
      servoReturned = false;
      detectTime = millis();

      myServo.write(0);
      lcd.setCursor(0, 1);
      lcd.print("Please Enter     ");
      Firebase.setString(fbdo, "/parking/message", "Please Enter 🚗");

      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, HIGH);
    }
  }

  // Sau 2s đóng barrie lại
  if (vehicleDetected && !servoReturned && millis() - detectTime >= 2000) {
    myServo.write(90);
    servoReturned = true;
  }

  // Sau 2s → tắt LED, hiển thị lại
  if (vehicleDetected && millis() - detectTime >= 2000) {
    digitalWrite(LED_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Wellcome !     ");
    Firebase.setString(fbdo, "/parking/message", "Wellcome !");
    vehicleDetected = false;
  }

  delay(200);
}

// Đo khoảng cách từ SRF05
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
  if (duration == 0) return -1;

  float distanceCm = duration * 0.034 / 2.0;
  return distanceCm;
}
