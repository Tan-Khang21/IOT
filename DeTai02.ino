#define BLYNK_AUTH_TOKEN "mu7MieemKvOaf-V7BKl_CbbPoEXp1N_s"
#define BLYNK_TEMPLATE_ID "TMPL6JWidIiTE"
#define BLYNK_TEMPLATE_NAME "Dk Led"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- Cấu hình phần cứng ----------------
#define LED1 2
#define LED2 16
#define LED3 17
#define BUZZER 12
#define DHTPIN 14
#define DHTTYPE DHT11
#define BUTTON1 23
#define BUTTON2 25
#define BUTTON3 26

// ---------------- Cấu hình Blynk & WiFi ----------------
#define WIFI_SSID  "Galaxy s23"
#define WIFI_PASS  "12345678"

// ---------------- Biến & đối tượng ----------------
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
BlynkTimer timer;

bool led1State = false;
bool led2State = false;
bool led3State = false;

// ---------------- Cập nhật LED trên LCD ----------------
void updateLedStatusLCD() {
  lcd.setCursor(0, 2);
  lcd.print("LED1:");
  lcd.print(led1State ? "ON " : "OFF");

  lcd.print(" LED2:");
  lcd.print(led2State ? "ON " : "OFF");

  lcd.setCursor(0, 3);
  lcd.print("LED3:");
  lcd.print(led3State ? "ON " : "OFF  ");
}

// ---------------- Cập nhật LED hiển thị trên Blynk ----------------
void updateLedStatusBlynk() {
  Blynk.virtualWrite(V3, led1State); // LED trạng thái 1
  Blynk.virtualWrite(V4, led2State); // LED trạng thái 2
  Blynk.virtualWrite(V5, led3State); // LED trạng thái 3
}

// ---------------- Gửi dữ liệu cảm biến ----------------
void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

 
  // Hiển thị LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nhiet do: ");
  lcd.print(t);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Do am: ");
  lcd.print(h);
  lcd.print("%");

  // Hiển thị trạng thái 3 LED
  updateLedStatusLCD();

  // Gửi dữ liệu lên Blynk
  Blynk.virtualWrite(V8, t);
  Blynk.virtualWrite(V9, h);

  // Cảnh báo nhiệt độ cao
  if (t > 35) digitalWrite(BUZZER, HIGH);
  else digitalWrite(BUZZER, LOW);
}

// ---------------- Xử lý Switch trên Blynk ----------------
BLYNK_WRITE(V0) { // LED1
  led1State = param.asInt();
  digitalWrite(LED1, led1State);
  updateLedStatusLCD();
  updateLedStatusBlynk();
}

BLYNK_WRITE(V1) { // LED2
  led2State = param.asInt();
  digitalWrite(LED2, led2State);
  updateLedStatusLCD();
  updateLedStatusBlynk();
}

BLYNK_WRITE(V2) { // LED3
  led3State = param.asInt();
  digitalWrite(LED3, led3State);
  updateLedStatusLCD();
  updateLedStatusBlynk();
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.begin();
  lcd.backlight();

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(DHTPIN, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Dang ket noi WiFi...");
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  lcd.clear();
  lcd.print("Da ket noi Blynk!");

  timer.setInterval(2000L, sendSensorData);
  delay(1000);
  lcd.clear();


  updateLedStatusBlynk(); // Gửi trạng thái ban đầu
}

// ---------------- Loop ----------------
void loop() {
  Blynk.run();
  timer.run();

  static bool lastButton1 = HIGH, lastButton2 = HIGH, lastButton3 = HIGH;
  bool current1 = digitalRead(BUTTON1);
  bool current2 = digitalRead(BUTTON2);
  bool current3 = digitalRead(BUTTON3);

  // --- Nút 1 ---
  if (current1 == LOW && lastButton1 == HIGH) {
    led1State = !led1State;
    digitalWrite(LED1, led1State);
    Blynk.virtualWrite(V0, led1State);
    updateLedStatusLCD();
    updateLedStatusBlynk();
    delay(200);
  }

  // --- Nút 2 ---
  if (current2 == LOW && lastButton2 == HIGH) {
    led2State = !led2State;
    digitalWrite(LED2, led2State);
    Blynk.virtualWrite(V1, led2State);
    updateLedStatusLCD();
    updateLedStatusBlynk();
    delay(200);
  }

  // --- Nút 3 ---
  if (current3 == LOW && lastButton3 == HIGH) {
    led3State = !led3State;
    digitalWrite(LED3, led3State);
    Blynk.virtualWrite(V2, led3State);
    updateLedStatusLCD();
    updateLedStatusBlynk();
    delay(200);
  }

  lastButton1 = current1;
  lastButton2 = current2;
  lastButton3 = current3;

  lcd.setCursor(11, 3);
  lcd.print("N.Sinh");
}
