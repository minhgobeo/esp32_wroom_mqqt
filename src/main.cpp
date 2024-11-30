#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
// Include thư viện chứa thông tin mạng WiFi & HiveMQ broker
#include "setup_WiFi_and_connect_mqtt.h" 
// Thông tin mạng WiFi & HiveMQ broker

#include "read_sensor.h"

char topic_read_sensor[10];
char mes[15];

//Setup
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; i++) {  //chinh len 8 sau
    pinMode(analogsmoke[i], INPUT);
  }
  for (int i = 0; i < 3; i++) {
    pinMode(ctrpin[i], OUTPUT);
    digitalWrite(ctrpin[i], LOW);
  }
  setup_wifi();
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
}

//Loop
void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();  // Duy trì kết nối với MQTT (~ blynk.run();)*/
  if (count > 7) {
    count = 0;
    delay(8000);
  }

  if (count<2) {
    floorr = count + 1;
    zone = 1;
  }
  else {
    floorr = 3;
    zone = count - 1;
  }

  if (zone == 1) {
    Serial.printf("\nFloor %d:\n",floorr);
  }
  
  Serial.printf("Zone %d: ", zone);
  Serial.printf("%d %d, %d %d\n", read_temp(count), logic_temp(read_temp(count)), read_smoke(count, analogsmoke), logic_smoke(read_smoke(count, analogsmoke)));

  // xong roi gui data len hivemq (chia topic sao ta)
  snprintf(topic_read_sensor, 17, "esp/%d/%d", floorr, zone);
  snprintf(mes, 11, "%d%d", logic_temp(read_temp(count)), logic_smoke(read_smoke(count, analogsmoke)));
  client.publish(topic_read_sensor, mes);

  count++;
  delay(50);

}


