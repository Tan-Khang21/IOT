/*************************
 *  IOT GIAM SAT KHONG KHI
 *  ESP32 + MQ + DHT + LCD + BLYNK
 *************************/

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>



/* ===== PIN ===== */
#define DHTPIN 4
#define DHTTYPE DHT11        // Hoặc DHT22
#define MQ_PIN 34
#define BUZZER 18
#define LED 19

/* ===== OBJECT ===== */
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
BlynkTimer timer;

/* ===== NGUONG CANH BAO ===== */
int gasThreshold = 1800;   // Ngưỡng khí gas
float tempThreshold = 35; // Nhiệt độ cao

/* ===== HAM DOC CAM BIEN ===== */
void readSensors() {
  int gasValue = analogRead(MQ_PIN);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Loi doc DHT!");
    return;
  }

  /* ===== LCD ===== */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nhiet do: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Do am: ");
  lcd.print(humidity);
  lcd.print(" %");

  lcd.setCursor(0, 2);
  lcd.print("Khi gas: ");
  lcd.print(gasValue);

  /* ===== BLYNK ===== */
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, gasValue);

  /* ===== CANH BAO ===== */
  if (gasValue > gasThreshold || temperature > tempThreshold) {
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    lcd.setCursor(0, 3);
    lcd.print("CANH BAO NGUY HIEM");
    Blynk.logEvent("canhbao", "Moi truong khong an toan!");
  } else {
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
    lcd.setCursor(0, 3);
    lcd.print("Moi truong an toan");
  }

  /* ===== SERIAL ===== */
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" | Hum: ");
  Serial.print(humidity);
  Serial.print(" | Gas: ");
  Serial.println(gasValue);
}

/* ===== SETUP ===== */
void setup() {
  Serial.begin(9600);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  lcd.init();
  lcd.backlight();

  dht.begin();

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(2000L, readSensors);

  lcd.setCursor(0, 0);
  lcd.print("He thong IoT");
  lcd.setCursor(0, 1);
  lcd.print("Giam sat khong khi");
  delay(2000);
  lcd.clear();
}

/* ===== LOOP ===== */
void loop() {
  Blynk.run();
  timer.run();
}
