#include <ESP8266WiFi.h> // optional in the simulator
#include <wire.h> // optional in the simulator


const char* wifiSSID = "Simulator Wifi";              // Wi-Fi network SSID
const char* wifiPassword = "";      // Wi-Fi network password
const char* host = "api.thingspeak.com";            // ThingSpeak API server
const int httpPort = 80;                            // HTTP port for communication
const char* apiEndpoint = "/update?api_key=RZVOOQ9M4FFAGTZA&field1=";  // ThingSpeak API endpoint

int setupESP8266() {
  Serial.begin(115200);                             // Starting the serial communication
  Serial.println("AT");                              // Sending AT command to ESP8266
  delay(10);
  if (!Serial.find("OK")) return 1;                  // Checking if "OK" response is received

  Serial.println("AT+CWJAP=\"" + String(wifiSSID) + "\",\"" + String(wifiPassword) + "\"");   // Connect to Wi-Fi network
  delay(10);
  if (!Serial.find("OK")) return 2;                  // Checking if "OK" response is received

  Serial.println("AT+CIPSTART=\"TCP\",\"" + String(host) + "\"," + String(httpPort));       // Establish TCP connection with ThingSpeak server
  delay(50);
  if (!Serial.find("OK")) return 3;                  // Checking if "OK" response is received

  return 0;                                         // Return success
}

void sendDataToThingSpeak() {
  int temperature = map(analogRead(A0), 20, 358, -40, 125);   // Reading temperature from analog pin and maping the values

  String httpRequest = "GET " + String(apiEndpoint) + String(temperature) + " HTTP/1.1\r\nHost: " + String(host) + "\r\n\r\n";   // making an HTTP request
  int length = httpRequest.length();

  Serial.print("AT+CIPSEND=");                       // Sending the length of the HTTP request to ESP8266
  Serial.println(length);
  delay(10);
  if (!Serial.find(">")) return;                      // Checking if ">" response is received

  Serial.print(httpRequest);                          // Sending HTTP request to ThingSpeak server
  delay(10);
  if (!Serial.find("SEND OK\r\n")) return;            // Checking if "SEND OK" response is received
}

void setup() {
  setupESP8266();                                    // Initializing ESP8266
}

void loop() {
  sendDataToThingSpeak();                            // Sending data to ThingSpeak
  delay(10000);                                      // Delaying for 10 seconds before looping again
}

