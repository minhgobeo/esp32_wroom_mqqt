#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "credential.h"

const char* mqtt_server = "4838f20abef342ccba6a129cecb3ebe8.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "testaccount";  //User
const char* mqtt_password = "Test12345";    //Password
// Tạo đối tượng WiFi và MQTT client
WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("connected.");
    } else {
      Serial.printf("failed, rc=%d\n", client.state());
      Serial.println("Try again in 5 seconds.");
      delay(5000);
    }
  }
}

void reconnect_wifi_and_mqtt()
{
    setup_wifi();
    reconnect_mqtt();
}