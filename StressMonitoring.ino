#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFiS3.h>
#include <WiFiSSLClient.h>
#include <UniversalTelegramBot.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// GSR reading and percentage
int GSR;
int GSR_percentage;
// WiFi credentials
#define WIFI_SSID "LAVKUSH_CHAUDHARI "
#define WIFI_PASSWORD "8108491665"
// Telegram bot token
#define BOT_API_TOKEN "7534847312:AAEmhLTw7KuXPPPt9pWi00ePsqpilst1zdo"
// BME280 sensor configuration
Adafruit_BME280 bme;
// MAX30105 sensor configuration
MAX30105 particleSensor;
// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const byte RATE_SIZE = 4; // Increase for more averaging
byte rates[RATE_SIZE]; // Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; // Time of the last beat
float beatsPerMinute;
int beatAvg;
String chat_id = "";
int message_id = -1;
unsigned long last_call = 0;
const unsigned long interval = 1000;
// Object for secure WiFi client management
WiFiSSLClient securedClient;
// Object for managing the Telegram bot
UniversalTelegramBot bot(BOT_API_TOKEN, securedClient);
// Function to handle incoming messages from Telegram
void handleMessages(int num_new_messages) {
  for (int i = 0; i < num_new_messages; i++) {
    message_id = bot.messages[i].message_id;
    chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    // Command handling
    if (text == "/GSR") {
      // Send the GSR value and mapped percentage to the chat
      bot.sendMessage(chat_id, "Mapped GSR: " + String(GSR_percentage) + "%", "Markdown");
    } else if (text == "/TEMP") {
      float temperature = bme.readTemperature(); // Read temperature
      bot.sendMessage(chat_id, "Temperature: " + String(temperature, 2) + " °C", "Markdown");
      Serial.print("Temperature: ");
      Serial.print(temperature, 2);
      Serial.println(" °C");
    } else if (text == "/BPM") {
      bot.sendMessage(chat_id, "Average BPM: " + String(beatAvg), "Markdown");
    } else if (text == "/start") {
      bot.sendMessage(chat_id, "Use /GSR Value to get the GSR value, /TEMP to get the temperature, and /BPM to get the average heart rate.", "Markdown");
    }
  }
}
// Function to connect to WiFi network
void connectToWiFi() {
  Serial.print("Connecting to WiFi network ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.print("WiFi connected. IP Address: ");
  Serial.println(WiFi.localIP());
}
// Initialization function
void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  randomSeed(analogRead(A1));
  connectToWiFi();
  // Initialize BME280 sensor
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  // Initialize MAX30105 sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) { // Use default I2C port, 400kHz speed
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
  bot.sendMessage(chat_id, "BOT Started", "Markdown");
  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.display();
  delay(2000);
}
// Main loop
void loop() {
  GSR = analogRead(A0);
  // Map the GSR value from 700-300 to 0-100%
  GSR_percentage = map(GSR, 300, 700, 0, 100);
  if (GSR_percentage < 0) GSR_percentage = 0;
  if (GSR_percentage > 100) GSR_percentage = 100;
  // Check for new messages from Telegram
  if (millis() - last_call > interval) {
    int num_new_messages = bot.getUpdates(bot.last_message_received + 1);
    while (num_new_messages) {
      handleMessages(num_new_messages);
      num_new_messages = bot.getUpdates(bot.last_message_received + 1);
    }
    last_call = millis();
  }
  // Update the OLED display with sensor data
  display.clearDisplay();
  // Display GSR data
  display.setCursor(0, 0);
  display.print("GSR: ");
  display.print(GSR_percentage);
  display.print("%");
  // Display temperature data
  float temperature = bme.readTemperature();
  display.setCursor(0, 24);
  display.print("Temp: ");
  display.print(int(temperature));
  display.print("C");
  // Check heart rate data
  long irValue = particleSensor.getIR();
  if (irValue > 50000) {
    if (checkForBeat(irValue) == true) {
      long delta = millis() - lastBeat;
      lastBeat = millis();
      beatsPerMinute = 60 / (delta / 1000.0);
      if (beatsPerMinute < 255 && beatsPerMinute > 20) {
        rates[rateSpot++] = (byte)beatsPerMinute;
        rateSpot %= RATE_SIZE;
        // Take average of readings
        beatAvg = 0;
        for (byte x = 0; x < RATE_SIZE; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
      // Display the heart rate (BPM) on OLED
      display.setCursor(0, 48);
      display.print("BPM: ");
      display.print(beatAvg);
    }
  }
  // Check if all conditions are met: GSR > 400, Temp > 39°C, BPM > 90
  if (GSR_percentage > 40 && temperature > 39.0 && beatAvg > 90) {
    bot.sendMessage(chat_id, "Try to relax, you are stressed!", "Markdown");
  }
  // Show the display contents
  display.display();
  // Print to serial for debugging
  Serial.print("GSR: ");
  Serial.print(GSR);
  Serial.print(" | Mapped GSR: ");
  Serial.print(GSR_percentage);
  Serial.print(" | Temp: ");
  Serial.print(temperature);
  Serial.print(" | BPM: ");
  Serial.println(beatAvg);
}
