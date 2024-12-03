#ifndef SUBCRIBE
#define SUBCRIBE
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "setup_WiFi_and_connect_mqtt.h"

// Define variable to subscribe MQTT
String messages[10];
int floorrs[10];
int zones[10];
int levels[10];
int indexx = 0;
int mdnh[3][6] = {0};
int level[3][6] = {0};
int timer = 0;

//Define function
void callback(char* topic, byte* payload, unsigned int length);
void mdnh_func(int floorrs[10], int zones[10], String messages[10], int indexx);
void level_func(int floorrs[10], int zones[10], int levels[10], int indexx);
void fire_alarm(int level[3][6]);


void callback(char* topic, byte* payload, unsigned int length) { // Hàm xử lý khi nhận được dữ liệu từ MQTT
// Hàm sẽ lưu đc 10 mes, 10 floorr, 10 zone hiện tại
  int floorr = topic[4] - '0';
  int zone = topic[6] - '0';
  // Giải quyết chuỗi
  char message[length+1];
  memcpy(message, payload, length);
  message[length] = '\0';
  Serial.printf("Message arrived [%s]: %s\n", topic, message);  

  if (indexx <10) {
    messages[indexx] = String(message);
    Serial.printf("Message[1]: %c, Message[2]: %c\n", message[1], message[2]);
    if((String(message)[1]!='2') && (String(message)[2]!='2')) {
      levels[indexx] = 1;
    }
    else if ((String(message)[1]!='2') && (String(message)[2]=='2')) {
      levels[indexx] = 2;
    }
    else if ((String(message)[1]=='2') && (String(message)[2]!='2')) {
      levels[indexx] = 3;
    }
    else if ((String(message)[1]=='2') && (String(message)[2]=='2')) {
      levels[indexx] = 4;
    }
    floorrs[indexx] = floorr;
    zones[indexx] = zone;
    indexx++;
  }
  else {
    for (int i = 0; i< 9; i++) {
      messages[i] = messages[i+1];
      levels[i] = levels[i+1];
      floorrs[i] = floorrs[i+1];
      zones[i] = zones[i+1];
    }
    messages[9] = String(message);
    if((String(message)[1]!='2') && (String(message)[2]!='2')) {
      levels[9] = 1;
    }
    else if ((String(message)[1]!='2') && (String(message)[2]=='2')) {
      levels[9] = 2;
    }
    else if ((String(message)[1]=='2') && (String(message)[2]!='2')) {
      levels[9] = 3;
    }
    else if ((String(message)[1]=='2') && (String(message)[2]=='2')) {
      levels[9] = 4;
    }
    floorrs[9] = floorr;
    zones[9] = zone;
  }

  mdnh_func(floorrs, zones, messages, indexx);
  level_func(floorrs, zones, levels, indexx);
  if (millis()-timer>=5000) {
    fire_alarm(level); // se co delay tong cho ham nay nhe
    timer = millis();
  }

}

void mdnh_func(int floorrs[10], int zones[10], String messages[10], int indexx) {
  for (int i = 0; i< indexx; i++) {
    int value = atoi(messages[i].c_str());  // Chuyển String thành số
    mdnh[floorrs[i]-1][zones[i]-1] = value;
  }
  
  Serial.println("Updated mdnh array:");
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) {
      Serial.print(mdnh[i][j]);
      Serial.print("\t");
    }
    Serial.println();
  }
}

void level_func(int floorrs[10], int zones[10], int levels[10], int indexx) {
  for (int i = 0; i< indexx; i++) {
    level[floorrs[i]-1][zones[i]-1] = levels[i];
  }
  
  Serial.println("Updated level array:");
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) {
      Serial.print(level[i][j]);
      Serial.print("\t");
    }
    Serial.println();
  }
}


void fire_alarm(int level[3][6]) {
  const char* thongbao[5][5] = {
  { "Tang 1 chay", //0
    "Tang 2 chay",
    "Tang 3 chay" },
  { "Khu vuc 1 chay", //1
    "Khu vuc 2 chay",
    "Khu vuc 3 chay",
    "Khu vuc 4 chay",
    "Xung quanh nen can than, can..." },
  { "Hai loi thoat deu chay ", //2
    "Loi thoat 1 chay ",
    "Loi thoat 2 chay " },
  { "voi muc do 1, can...", //3
    "voi muc do 2, can...",
    "voi muc do 3, can...",
    "voi muc do 4, can..." },
  { "Chuong reo" } 
  };
  int stair[2] = {-1};
  bool flagg = 0;
  for (int i=0; i<3; i++){ //i: floorr - 1 
    int countstair = 0;
    bool flag = 0;
    for (int j=0; j<6; j++) { //j: zone - 1
      if (level[i][j] != 0) {
        if (flagg == 0){
          Serial.println(thongbao[4][0]); //4 //sau moi lan phat loa phai delay 1 xiu (dung millis)
          flagg = 1;
        }  
        Serial.println(thongbao[0][i]); //0    

        if (j<4) {
          Serial.println(thongbao[1][j]);
        }
        else if (j == 4) {
          flag = 1;
          stair[countstair] = 0;
          countstair++;
          if (level[i][j+1]!=0) {
            stair[countstair] = 1;
            countstair++;
          }
        }
        else if (flag == 0) {
          stair[countstair] = 1;
          countstair++;
        }
        
        if (countstair == 1) {
          Serial.print(thongbao[2][stair[0]+1]);
          Serial.println(thongbao[3][level[i][stair[0]+4] - 1]);
          if (flag == 1) {
            flag = 0;
            break;
          }
        }
        
        if (countstair == 2) {
          if (level[i][4] == level[i][5]){
            Serial.print(thongbao[2][0]);
            Serial.println(thongbao[3][level[i][4] - 1]);
          }
          else if (level[i][4] > level[i][5]) {
            Serial.print(thongbao[2][1]);
            Serial.println(thongbao[3][level[i][4] - 1]);
            Serial.print(thongbao[2][2]);
            Serial.println(thongbao[3][level[i][5] - 1]);
          }
          else {
            Serial.print(thongbao[2][2]);
            Serial.println(thongbao[3][level[i][5] - 1]);
            Serial.print(thongbao[2][1]);
            Serial.println(thongbao[3][level[i][4] - 1]);
          }
          break;
        }
      }
    }
  }
}
#endif