#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Timer.h>
#define ONE_WIRE_BUS 2  // Arduino數位腳位2接到1-Wire裝置
SoftwareSerial ArduinoUno(4,3);

Timer t1; //制訂計時器
Timer tcb;
OneWire oneWire(ONE_WIRE_BUS); // 運用程式庫建立物件
DallasTemperature sensors(&oneWire);
int v1 = 10; //pump
//int v2 = 11; // 加熱器
int v3 = 12; // LED燈號
//int v4 = 13; // 電磁閥
//int S_T = 18; //合適溫度，低於此溫度開始加溫
//int T_T = 25; //加熱瓶內最高溫度，避免過熱
int B_T = -30 ;//溫度計斷線溫度
int T1 = 0; //溫度感測器1
int T2 = 0; //溫度感測器2
//int LTS = 0; //低溫
//int BTS = 0; //溫度計斷線
int count = 0;
int a = 0; //pump運轉時間
int b = 1; //澆菜計數器
int c = 0; //傳送數據計時器
int i = 0; //loop計算
int check_R = 0; //數據回傳確認開關
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ArduinoUno.begin(4800);
//  Serial.println("Temperature Sensor");
  pinMode(v1,OUTPUT);
//  pinMode(v2,OUTPUT);
//  pinMode(v3,OUTPUT);
//  pinMode(v4,OUTPUT);
//  pinMode(4,INPUT);
 // pinMode(3,OUTPUT);
 // digitalWrite(v1,LOW);
//  digitalWrite(v2,LOW);
//  digitalWrite(v3,LOW);
//  digitalWrite(v4,LOW);
  digitalWrite(v1,HIGH);
//  digitalWrite(v4,HIGH);
//  delay(15000);
//  digitalWrite(v4,LOW);
  delay(30000);
  digitalWrite(v1,LOW);
  // 初始化 
  sensors.begin();
  t1.oscillate(v3,500,HIGH);
  tcb.every(1000,writeToSerial);
  int a = 0;
}

void loop() {
tcb.update();
//if (LTS == 1){
// digitalWrite(v3,HIGH);
//}
//if(LTS == 0){
// digitalWrite(v3,LOW);
// Serial.println("LED close");
//}
//if(BTS == 1){
// t1.update();
//  Serial.println("LED bright");
//}
//if(BTS == 0){
//}
}
void writeToSerial(){
  a++; 
  //c++;
  // put your main code here, to run repeatedly:
  // 要求匯流排上的所有感測器進行溫度轉換 
  sensors.requestTemperatures();
  // 取得溫度讀數（攝氏）並輸出，
  // 參數0代表匯流排上第0個1-Wire裝置
//  T1 = sensors.getTempCByIndex(0);
 // Serial.print(T1);
 // Serial.print("/");
 // T2 = sensors.getTempCByIndex(1);
// Serial.println(T2);
  Serial.println(a);
 // Serial.println(b);
  if (a> 7200){
  digitalWrite(v1,HIGH); 
  Serial.println("open pump");
//  if (b > 3){
//  digitalWrite(v4,LOW);
 // Serial.println("close valve");
//  } 
//  else
 // {
 //   digitalWrite(v4,HIGH);
 // Serial.println("open valve");
 // }
  }
  if (a > 7320 ){
   digitalWrite(v1,LOW);
   Serial.println("close");
//   digitalWrite(v4,LOW);
//  Serial.println("close pump & valve");
//  if (b > 3){
//  Serial.println("reset b"); 
//  b=0;
  a = 120;
  Serial.println("reset");
  }

  
//  b++;
  }
//  if (c >= 5){
//    c=0;
//      for (i=0;i<=3;i++){
//ArduinoUno.print(i);
//ArduinoUno.println("\n");
//if (i == 0){             //第一組數據傳輸 -時間
//   while(ArduinoUno.available()>0){
//  int check_R = ArduinoUno.parseInt();
//  if(ArduinoUno.read()=='\n'){
//    if (check_R == 1){
//      check_R = 0;
//      ArduinoUno.print(1800-a);
//      ArduinoUno.println("\n");
//      Serial.println("send time");
//    }
//    }
//   }
//    }
////第一組數據傳輸  
//if (i == 1){             //第二組數據傳輸-溫度 
//   while(ArduinoUno.available()>0){
//  int check_R = ArduinoUno.parseInt();
//  if(ArduinoUno.read()=='\n'){
//    if (check_R == 1){
//      check_R = 0;
//      ArduinoUno.print(T1);
//      ArduinoUno.println("\n");
//      Serial.println("send temperature 1");
//    }
//    }
//   }
//    }//第二組數據傳輸
//    if (i == 2){             //第三組數據傳輸 -溫度
//   while(ArduinoUno.available()>0){
//  int check_R = ArduinoUno.parseInt();
//  if(ArduinoUno.read()=='\n'){
//    if (check_R == 1){
//      check_R = 0;
//      ArduinoUno.print(T2);
//      ArduinoUno.println("\n");
//      Serial.println("send temperature 2");
//    }
//    }
//   }
//    }//第三組數據傳輸    
//    }
//  }
//  }
//  if( T1 < S_T & T1 >= B_T &  T2 < T_T & T2 >= B_T){
//    digitalWrite(v2,HIGH);
//    Serial.println("heater open");
//    LTS = 1;BTS = 0;//燈號控制    
//  }
//  if ( T1 < S_T & T2 >= T_T){
//  digitalWrite(v2,LOW);
//    Serial.println("heater close");
//    LTS = 0;BTS = 0;//燈號控制 
//  }
 
//  if(T1 <= B_T & T2 <= B_T){
//      digitalWrite(v2,LOW);
//      Serial.println("sensor broken,close heater");
//      LTS = 2;BTS = 1;//燈號控制 
//  }
//  if(T1 >= S_T){
//digitalWrite(v2,LOW);
//    Serial.println("heater close");
//    LTS = 0;BTS = 0;//燈號控制
//  }
