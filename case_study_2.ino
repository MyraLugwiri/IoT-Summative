#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <LiquidCrystal.h> // Include the standard LiquidCrystal library
#include "DHT.h"


#define DHTPIN 2         // Pin which is connected to the DHT sensor
#define DHTTYPE DHT22    // DHT 22 (AM2302)


const char *ssid = "mimi";      // Enter your WiFi SSID
const char *password = "*******";  // Enter your WiFi password
const unsigned long channelID = *******; // Enter your ThingSpeak Channel ID
const char *writeAPIKey = "**********";  // Enter your ThingSpeak Write API Key


WiFiClient client;


DHT dht(DHTPIN, DHTTYPE);


// Define the LCD pins
const int rs = D6, en = D5, d4 = D1, d5 = D2, d6 = D3, d7 = D4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Create an instance of the LiquidCrystal class
int Soil = A0;
int MoistureValue;


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows
  dht.begin();
  pinMode(Soil, INPUT);
  ThingSpeak.begin(client);
  connectWiFi();
}


void loop() {
  float humidity = dht.readHumidity();        // Read humidity
  float temperature = dht.readTemperature();  // Read temperature in Celsius
  MoistureValue = analogRead(Soil);


  if (isnan(humidity) || isnan(temperature) || isnan(MoistureValue)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  Serial.print(MoistureValue);


  // Display temperature and humidity on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print(MoistureValue);


  // Update ThingSpeak channel fields
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, MoistureValue);
  ThingSpeak.writeFields(channelID, writeAPIKey);


  delay(2000); // Delay between updates
}


void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
}
