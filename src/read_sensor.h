#include <Arduino.h>
#include <max6675.h>

#define thermoCLK 13
#define thermoSO 12
int ctrpin[3] = {14, 27, 26};
MAX6675 thermocouple(thermoCLK, -1, thermoSO);
int analogsmoke[3] = {34, 35, 36};  //Add them sau
//INPUT only: 34 35 36 39

//Variables
int count = 0; 
//count 0,1 là floor 1 zone 1, floor 2 zone 1
//count 2->7 là floor 3 zone 1 -> 6:
int floorr = 0; // tầng sẽ publish lên mqtt
int zone = 0;

int read_temp(int count);
int logic_temp(int value);
int read_smoke(int count, int analogsmoke[]);
int logic_smoke(int value);

int read_temp(int count) {
  digitalWrite(ctrpin[0], count & 0x01);
  digitalWrite(ctrpin[1], count >> 1 & 0x01);
  digitalWrite(ctrpin[2], count >> 2 & 0x01);
  return thermocouple.readCelsius();
}

int logic_temp(int value) {
  if (value < 45) {
    return 0;
  } else if (value < 300) {
    return 1;
  }
  return 2;
}

int read_smoke(int count, int analogsmoke[]) {
  int value = analogRead(analogsmoke[count]) * 1000 / 4095;
  return value;
}
int logic_smoke(int value) {
  if (value < 100) {
    return 0;
  } else if (value < 1000) {
    return 1;
  } else {
    return 2;
  }
}