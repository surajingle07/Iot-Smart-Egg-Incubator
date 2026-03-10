#define BLYNK_TEMPLATE_ID "TMPL3MaH0E_j3"
#define BLYNK_TEMPLATE_NAME "Iot Based Smart Egg incubator"
#define BLYNK_AUTH_TOKEN "xoB99ANSg1MizcIPlsGsmOeLAlhoZiU4"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Initialize LCD and DHT sensor
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(D3, DHT11); // DHT connected to pin D3

// WiFi credentials
char auth[] = "xoB99ANSg1MizcIPlsGsmOeLAlhoZiU4";
char ssid[] = "12123";
char pass[] = "123456789";

// Blynk Timer
BlynkTimer timer;

// Relay pin (Only Fan)
#define RELAY_FAN D5

// Function to read DHT11 sensor and update Blynk and LCD
void sendSensorData() {

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        lcd.setCursor(0, 0);
        lcd.print("Sensor Error!   ");
        return;
    }

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C   ");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%   ");

    Blynk.virtualWrite(V0, temperature);
    Blynk.virtualWrite(V1, humidity);
}

// Fan Control Only
BLYNK_WRITE(V2) {
    int relayState = param.asInt();

    if (relayState == 1) {
        digitalWrite(RELAY_FAN, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("Fan: ON    ");
    } else {
        digitalWrite(RELAY_FAN, LOW);
        lcd.setCursor(0, 0);
        lcd.print("Fan: OFF   ");
    }
}

void setup() {

    Serial.begin(9600);

    Wire.begin(D2, D1); // SDA = D2, SCL = D1
    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Connecting...");

    Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

    dht.begin();

    pinMode(RELAY_FAN, OUTPUT);
    digitalWrite(RELAY_FAN, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready!");
    delay(2000);
    lcd.clear();

    timer.setInterval(1000L, sendSensorData);
}

void loop() {
    Blynk.run();
    timer.run();
}