#include <DS3231.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Timer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2  // Arduino數位腳位2接到1-Wire裝置 (啟用溫度感測器)
SoftwareSerial ArduinoUno(4,3);
OneWire oneWire(ONE_WIRE_BUS); // 運用程式庫建立物件
DallasTemperature sensors(&oneWire);

int time_for_pump = 300; //打水多久
Timer t1;  //制訂計時器1
Timer tcb; //制訂計時器2
int v1 = 10; //pump
int v2 = 11; //deep pump
int v3 = 12; // 加熱器
int v4 = 9; // LED燈號控制
int s1 = 8;  //水位sensor
int L1 = 0; //水位狀況

int lower_Temperature = 18; //溫度管制下限
int higher_Temperature = 25; //溫度管制上限
int break_sensor = -30 ;//偵測溫度計斷線 (溫度小於-30判斷為斷線)
int T1 = 0; //溫度感測器1
int T2 = 0; //溫度感測器2
int LTS = 0; //加熱器運作燈號
int BTS = 0; //溫度計斷線閃爍燈號

int pump_action = 0; //pump實際運轉時間
int deep_pump_states = 0; //沉水馬達狀態
//時鐘初始參數設定//
DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
byte year, month, date, DoW, hour, minute, second;
//時鐘初始參數設定//
void setup() {
  Wire.begin();
  Serial.begin(9600); //設定串流頻率9600
  pinMode(v1,OUTPUT); //設定10號位為v1(pump)，模式為輸出
  pinMode(v2,OUTPUT); //設定11號位為v2(deep pump)，模式為輸出
  pinMode(v3,OUTPUT); //設定11號位為v3(heater)，模式為輸出
  pinMode(v4,OUTPUT); //設定9號位為v4(LED)，模式為輸出
  pinMode(s1,INPUT_PULLUP); //設定8號位為s1(水位感測器)，模式為輸入
  //初始化，確認pumpdeep pump作動　
  digitalWrite(v1,HIGH);
  delay(5000);
  digitalWrite(v1,LOW);
  digitalWrite(v2,HIGH);
  delay(5000);
  digitalWrite(v2,LOW);
  //初始化結束
  tcb.every(1000,writeToSerial);//1秒update一次
  t1.oscillate(v4,500,HIGH); //控制燈號閃爍
  int pump_action = 0;
}

void loop() {
  tcb.update(); //開機後持續執行tcb內命令

if (LTS == 1){
 digitalWrite(v4,HIGH); //開始加熱，開燈
}
if(LTS == 0){
 digitalWrite(v4,LOW);
 Serial.println("LED close"); //結束加熱，關燈
}
if(BTS == 1){
  t1.update();
  Serial.println("LED bright"); //溫度計斷線，燈號閃爍
}
if(BTS == 0){
}//溫度計復線，回復燈號
}

///執行function內容如下///

void writeToSerial(){
  L1 = digitalRead(s1);
  ReadDS3231(); //導入時鐘function
  hour=Clock.getHour(h12, PM); //獲取時鐘模組內資料(小時)
  sensors.requestTemperatures();// 要求匯流排上的所有感測器進行溫度轉換
  T1 = sensors.getTempCByIndex(0);
  T2 = sensors.getTempCByIndex(1);
//每天時間7點、18點進行pump打水，維持time_for_pump所設定時間 (目前為600s)
if ((hour == 7)||(hour == 18)){
 if (pump_action < time_for_pump){
  if (L1 == 1){ //偵測水位，低於感測器位置即強制停止
  pump_action++; 
  digitalWrite(v1,HIGH); 
  Serial.println("open pump");
  }
  else{
   digitalWrite(v1,LOW); 
   Serial.println("low Level"); //低於水位，強制停止
  } 
  }
  if (pump_action >= time_for_pump ){
   digitalWrite(v1,LOW); 
   Serial.println("close");
  } 
}
else{
  pump_action = 0; //循環結束後reset pump運行時間
}

//每天10點、14點、19點進行deep pump運行，持續兩小時(指定時間12點、16點、21點關閉)
if ((hour == 10)||(hour == 14)||(hour == 19)){
 deep_pump_states = 1;//沉水馬達開啟
  }
if ((hour == 12)||(hour == 16)||(hour == 7)){
 deep_pump_states = 0;//沉水馬達關閉
}
 if (deep_pump_states == 1){
  if (L1 == 1){ 
  digitalWrite(v2,HIGH); 
  Serial.println("open deep pump");
  }
  else{
   digitalWrite(v2,LOW);
   Serial.println("low Level"); //低於水位，強制停止
  } 
  }
  if(deep_pump_states == 0){
  digitalWrite(v2,LOW);
  Serial.println("close deep pump"); 
  }
///溫度監控開啟加熱器 & LED燈號顯示
  if( T1 < lower_Temperature & T1 >= break_sensor &  T2 < higher_Temperature & T2 >= break_sensor){
    digitalWrite(v3,HIGH);
    Serial.println("heater open");
    LTS = 1;BTS = 0;//燈號控制    
  }
  if ( T1 < lower_Temperature & T2 >= higher_Temperature){
  digitalWrite(v3,LOW);
    Serial.println("heater close");
    LTS = 0;BTS = 0;//燈號控制 
  }
 
  if(T1 <= break_sensor & T2 <= break_sensor){
      digitalWrite(v3,LOW);
      Serial.println("sensor broken,close heater");
      LTS = 2;BTS = 1;//燈號控制 
  }
  if(T1 >= lower_Temperature){
digitalWrite(v3,LOW);
    Serial.println("heater close");
    LTS = 0;BTS = 0;//燈號控制
  }
}

//時鐘function//
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

//  Serial.print("20");
//  Serial.print(year,DEC);
//  Serial.print('-');
//  Serial.print(month,DEC);
//  Serial.print('-');
//  Serial.print(date,DEC);
//  Serial.print(' ');
//  Serial.print(hour,DEC);
//  Serial.print(':');
//  Serial.print(minute,DEC);
//  Serial.print(':');
//  Serial.print(second,DEC);
//  Serial.print('\n');
//  Serial.print("Temperature=");
//  Serial.print(temperature); 
//  Serial.print('\n');
}
//時鐘function//
