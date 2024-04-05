#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ThingSpeak.h>

// WiFi parameters
char ssid[] = "mimi";  // Enter your Wifi Username
char pass[] = "123123!45";  // Enter your Wifi password


// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883

unsigned long myChannelNumber = ********; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "*********************"; //Your Write API Key

//username and key from the Adafruit IO 
#define IO_USERNAME  "**********"
#define IO_KEY       "**************************"

WiFiClient client;
// Define MQTT topics
#define LIGHT_SENSOR_FEED   IO_USERNAME "/feeds/light_intensity"
#define LED_CONTROL_FEED    IO_USERNAME "/feeds/bulb_control"

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);
// Setup feeds for light e & ledcontrol
Adafruit_MQTT_Publish lightSensor = Adafruit_MQTT_Publish(&mqtt, LIGHT_SENSOR_FEED);
Adafruit_MQTT_Subscribe ledControl = Adafruit_MQTT_Subscribe(&mqtt, LED_CONTROL_FEED);

#define LIGHT_SENSOR_PIN A0
#define LED_PIN  D1
 
const int rs = D2, en = D3, d4 = D4, d5 = D5, d6 = D6, d7 = D7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// connect to adafruit io via MQTT
void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }
 
    if(ret >= 0)
      mqtt.disconnect();
 
    Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.begin(16, 2);
  //pinMode(led,OUTPUT);
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  mqtt.subscribe(&ledControl);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // connecting to adafruit io
  connect();

}

void loop() {
    delay(2000);

    // light sensor value
    int lightIntensity = analogRead(LIGHT_SENSOR_PIN);

    // Publish light intensity value to Adafruit IO
    lightSensor.publish(lightIntensity);

    // Display light intensity on LCD
    lcd.clear();  // Clear the LCD screen
    lcd.setCursor(0, 0);
    lcd.print("Light Intensity:");
    lcd.setCursor(0, 1);
    lcd.print(lightIntensity);

    // Check for LED control message from Adafruit IO
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(5000))) {
        if (subscription == &ledControl) {
            // Turn LED on or off based on received message
            int ledState = atoi((char *)ledControl.lastread);
            digitalWrite(LED_PIN, ledState);
            Serial.print("LED control message received: ");
            Serial.println(ledState ? "ON" : "OFF");
        }
    }

    // Check if the LCD prints "LED ON" and then turn on the LED
    if (strcmp((char *)ledControl.lastread, "ON") == 0) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW); // Ensure LED is off if "LED ON" is not printed
    }

    delay(1000);  // Wait for 1 second
}


