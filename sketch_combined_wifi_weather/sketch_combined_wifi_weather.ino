#include <WiFiS3.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ArduinoMqttClient.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_ADDRESS 0x76  // or 0x77 depending on your wiring

const char* wifiSSID = "h4prog";
const char* wifiPass = "1234567890";

const char* mqttBroker = "192.168.102.254";
const char* topic = "weather";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // change default clock speed - this should make the 180c temp error less commpon
  Wire.setClock(100000);

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  while (WiFi.begin(wifiSSID, wifiPass) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  while (WiFi.localIP() == INADDR_NONE) {
    delay(100);
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  delay(2000);

  if (!bme.begin(BME280_ADDRESS)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("BME280 initialized!");

  // Retry connecting to MQTT broker until success
  Serial.println("Connecting to MQTT broker...");
  while (!mqttClient.connect(mqttBroker)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    Serial.println("Retrying in 5 seconds...");
    delay(5000);
  }
  Serial.println("Connected to MQTT broker!");
}

void loop() {
  mqttClient.poll();

  delay(2000);

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  if (!mqttClient.connected()) {
    Serial.println("MQTT disconnected, trying to reconnect...");
    while (!mqttClient.connect(mqttBroker)) {
      Serial.print("MQTT reconnect failed! Error code = ");
      Serial.println(mqttClient.connectError());
      delay(5000);
    }
    Serial.println("MQTT reconnected!");
  }

  float temp     = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humidity = bme.readHumidity();

  if (temp > 120) {
    reinitBME280();
    return;
  }

  // Format as CSV string
  String data = String(temp, 2) + "," +
                String(pressure, 2) + "," +
                String(altitude, 2) + "," +
                String(humidity, 2);

  mqttClient.beginMessage(topic);
  mqttClient.print(data);
  mqttClient.endMessage();

  readBME280(temp, pressure, altitude, humidity);
}

void reinitBME280() {
  bme = Adafruit_BME280();
  if (!bme.begin(BME280_ADDRESS)) {
    Serial.println("BME280 reinit failed, retrying.");
  } else {
    Serial.println("BME280 reinitialized successfully.");
  }
}

void readBME280(float temp, float pressure, float altitude, float humidity) {
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println("*C");

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println("hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println("m");

  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.println();
}
