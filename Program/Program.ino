#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Credentials.h>


//Sensor-Identification
const char* sensorId = "Wohnzimmer";

#define ONE_WIRE_BUS 4  // The pin the DS18B20s are connected on
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


// WiFi and API configuration
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* serverName = SERVER_URL;
const char* serverCertificate = SERVER_CA_PEM_FILE;

// Deep sleep interval (e.g., 10 minutes)
const uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
const uint64_t TIME_TO_SLEEP = 10;  // Time ESP32 will go to sleep (in seconds)

void setup() {
  Serial.begin(115200);
  
  // Warm up Sensors
  sensors.begin();

  //connect to WiFi
  connectToWiFi();

  // Read data and send to API
  sensors.requestTemperatures(); // Send command to get temperatures

  for (int i = 0; i < sensors.getDeviceCount(); i++) 
  {
    DeviceAddress sensorAddress;
    float temperature = readTemperature(i);
    sendDataToAPI(temperature, sensorAddress);
  }
  

  // Enter deep sleep
  goToDeepSleep();
}

void loop() {
  // Empty, everything is managed in setup
}


float readTemperature(int i) {
  return sensors.getTempCByIndex(i);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

String getAddressString(DeviceAddress deviceAddress) {
  String addressStr = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) addressStr += "0";
    addressStr += String(deviceAddress[i], HEX);
  }
  return addressStr;
}

void sendDataToAPI(float temperature, DeviceAddress deviceAddress) {
    if(WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverName, serverCertificate);

        String boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
        String contentType = "multipart/form-data; boundary=" + boundary;
        http.addHeader("Content-Type", contentType);

        String httpRequestData = "--" + boundary + "\r\n";
        httpRequestData += "Content-Disposition: form-data; name=\"sensorName\"\r\n\r\n";
        httpRequestData += String(sensorId) + "_" + getAddressString(deviceAddress) + "\r\n";
        httpRequestData += "--" + boundary + "\r\n";
        httpRequestData += "Content-Disposition: form-data; name=\"temperature\"\r\n\r\n";
        httpRequestData += String(temperature) + "\r\n";
        httpRequestData += "--" + boundary + "--\r\n";

        Serial.println(httpRequestData); // for debugging
        int httpResponseCode = http.POST(httpRequestData);
        Serial.println(httpResponseCode); // for debugging
        http.end();
    }
}


void goToDeepSleep() {
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
