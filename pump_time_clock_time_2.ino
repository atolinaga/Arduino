#include <DS3231.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Timer.h>
int time_for_pump = 600; //打水多久
Timer t1; //制訂計時器
Timer tcb;
byte k = 0;
int test_time = 20;
int v1 = 8; //pump
int s1 = 12;  //水位sensor
int L1 = 0; //水位狀況
int a = 0; //pump運轉時間
//時鐘設定
DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
byte year, month, date, DoW, hour, minute, second;
//時鐘設定
void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(v1,OUTPUT);
  pinMode(s1,INPUT_PULLUP);
  digitalWrite(v1,LOW);
  delay(3000);
  digitalWrite(v1,HIGH);
  // 初始化 
  tcb.every(1000,writeToSerial);
  int a = 0;
}

void loop() {
  tcb.update();
}
void writeToSerial(){
  ReadDS3231();
//  L1 = digitalRead(s1);
  Serial.println(a);
  hour=Clock.getHour(h12, PM);
  minute=Clock.getMinute();
if ((hour == 8)||(hour == 10)||(hour == 12)||(hour == 14)||(hour == 16)||(hour == 18)||(hour == 20)||(hour == 21)||(hour == 22)||(hour == 23)||(hour == 24)||(hour == 1)||(hour == 2)||(hour == 3)||(hour == 4)||(hour == 5)||(hour == 6)||(hour == 7)){
 if(minute < 9){
 if (a < time_for_pump){
  a++; 
  digitalWrite(v1,LOW); 
  Serial.println("open pump");
  }
  if (a >= time_for_pump ){
   digitalWrite(v1,HIGH);
   a = 0;
   Serial.println("close");
  }
 } 
}
else{
}
}
void ReadDS3231()
{
  int second,minute,hour,date,month,year,temperature; 
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();  
  temperature=Clock.getTemperature();

  Serial.print("20");
  Serial.print(year,DEC);
  Serial.print('-');
  Serial.print(month,DEC);
  Serial.print('-');
  Serial.print(date,DEC);
  Serial.print(' ');
  Serial.print(hour,DEC);
  Serial.print(':');
  Serial.print(minute,DEC);
  Serial.print(':');
  Serial.print(second,DEC);
  Serial.print('\n');
  Serial.print("Temperature=");
  Serial.print(temperature); 
  Serial.print('\n');
}
