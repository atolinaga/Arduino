#include <SoftwareSerial.h>
SoftwareSerial BT(10,11);
int v1 = 2;
int v3 = 4;
int check = 0;
int scan = 0;
#define fsr_pin A0
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BT.begin(9600);
  pinMode(v1,OUTPUT);
  pinMode(v3,OUTPUT);
  digitalWrite(v1,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
int P_value = analogRead(fsr_pin);
int val = BT.read();
if (P_value > 50){
scan = 1;
  }
if(scan == 1){
  if(check == 1){
  check = 0;
  }
  else{
    check = 1;
  }
  }
if (check == 1){
  //digitalWrite(v1,HIGH);
  digitalWrite(v3,LOW);
    scan = 0;
}
else{
  //digitalWrite(v1,LOW);
  digitalWrite(v3,HIGH);
    scan = 0;
}
if(val == -1){
}
else{
  scan = 1;
  Serial.println("a"); 
}
delay(400);
}
