# ESP32 Temperature Logger

## Overview

This project is a temperature logging system using the ESP32 microcontroller and DS18B20 temperature sensors. It reads temperatures from multiple sensors and sends the data to a specified server API. The project includes deep sleep functionality for energy efficiency.

### Features

Read temperature data from multiple DS18B20 sensors.
Connect to WiFi and send temperature data to a web server via HTTP POST (using multipart/form-data).
Enter deep sleep mode to save energy and wake up periodically to read and send data.
Modular code structure for easy expansion and modification.

### Hardware Requirements

- ESP32 microcontroller.
- One or more DS18B20 temperature sensors.
- General electronics: resistors, wires, breadboard, etc.

### Software Requirements

Arduino IDE or PlatformIO for compiling and uploading the code to the ESP32.
Libraries: WiFi.h, HTTPClient.h, ArduinoJson.h, OneWire.h, DallasTemperature.h.

## Setup Instructions

### Hardware Setup:
Connect the DS18B20 sensors to the ESP32 as per the DS18B20 datasheet.
Ensure a 4.7kΩ resistor is used as a pull-up on the data line.

### Software Setup:
Install the necessary libraries through the Arduino IDE or PlatformIO.
Configure your WiFi SSID, password, server URL, and server CA certificate in `Program/Credentials.h`:
```
#define WIFI_SSID "YourSSID"
#define WIFI_PASSWORD "YourPassword"
#define SERVER_CA_PEM_FILE "YourServerCAPem"
#define SERVER_URL "https://yourserver/apiendpoint"
```
### Configuration:
Set the WiFi credentials and server details as environment variables or in a separate header file excluded from version control for security.
Modify the ONE_WIRE_BUS definition in the code to match the GPIO pin used on the ESP32.
Upload and Monitor:
Compile and upload the code to the ESP32.
Monitor the serial output to verify the connectivity and sensor readings.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.

## Contribution

Contributions to this project are welcome. Please adhere to standard coding practices and submit pull requests for any proposed changes.

## Important Note

Sensitive data like WiFi credentials and server information should not be hardcoded. These "secrets" should be defined through environment variables or external configuration files for security purposes.
