#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Thông tin mạng WiFi & HiveMQ broker
const char* mqtt_server = "4838f20abef342ccba6a129cecb3ebe8.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "testaccount";  //User
const char* mqtt_password = "Test12345";    //Password
// Tạo đối tượng WiFi và MQTT client
WiFiClientSecure espClient;
PubSubClient client(espClient);

int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}