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

  // Init wifi, bme & mqtt device
  setupWifi();
  delay(2000);
  setupBME280();
  delay(1000);
  setupMqttClient();
}

void loop() {
  mqttClient.poll();

  delay(10000);

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

  readBME280(temp, pressure, altitude, humidity);

  if (temp > 120 || temp < -10 || isnan(temp)) {
    reinitBME280();
    return;
  }

  // Format as CSV string - Avoid using String(..) + "," + ... - this is way cleaner memory wise.
  char data[100];  // make sure the buffer is large enough
  snprintf(data, sizeof(data), "%.2f,%.2f,%.2f,%.2f", temp, pressure, altitude, humidity);

  mqttClient.beginMessage(topic);
  mqttClient.print(data);
  mqttClient.endMessage();
}

void setupWifi() {
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
}

void setupBME280() {
  if (!bme.begin(BME280_ADDRESS)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    delay(2000);
    reinitBME280();
  }

  Serial.println("BME280 initialized!");
}

void setupMqttClient() {
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

// Attempt to reinit the sensor if the device suddenly fails (cant gaurantee a success)
void reinitBME280() {
  delay(500);
  bme = Adafruit_BME280();
  if (!bme.begin(BME280_ADDRESS)) {
    Serial.println("BME280 reinit failed, retrying.");
  } else {
    Serial.println("BME280 reinitialized successfully.");
  }

  delay(2000);

  float temp = bme.readTemperature();
  if (temp > 120 || temp < -10 || isnan(temp)) {
    Serial.println("Power cycle bme280 & reset arduino board");
    Wire.beginTransmission(0x76);
    Wire.write(0xE0);              // Reset register
    Wire.write(0xB6);              // Reset command
    Wire.endTransmission();

    delay(500);
    Serial.println("");
    NVIC_SystemReset();
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
