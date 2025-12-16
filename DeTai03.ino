/***************** BLYNK *****************/
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "Smart Light"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>


/***************** LCD *****************/
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

/***************** DHT *****************/
#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/***************** PIN *****************/
#define LED_PIN     2
#define BUTTON_PIN  23

/***************** VARIABLE *****************/
bool ledState = false;
bool lastButtonState = HIGH;

/***************** BLYNK BUTTON (V0) *****************/
BLYNK_WRITE(V0) {
  ledState = param.asInt();
  digitalWrite(LED_PIN, ledState);
}

/***************** SETUP *****************/
void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("SMART LIGHT IOT");
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.clear();
}

/***************** LOOP *****************/
void loop() {
  Blynk.run();

  // Đọc nút nhấn
  bool buttonState = digitalRead(BUTTON_PIN);
  if (lastButtonState == HIGH && buttonState == LOW) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    Blynk.virtualWrite(V0, ledState);
    delay(300); // chống dội nút
  }
  lastButtonState = buttonState;

  // Đọc DHT11
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  // Hiển thị LCD
  lcd.setCursor(0, 0);
  lcd.print("Den: ");
  lcd.print(ledState ? "ON " : "OFF");

  lcd.setCursor(0, 1);
  lcd.print("Nhiet do: ");
  lcd.print(temp);
  lcd.print(" C  ");

  lcd.setCursor(0, 2);
  lcd.print("Do am: ");
  lcd.print(hum);
  lcd.print(" %   ");

  delay(1000);
}
