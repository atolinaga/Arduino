#include <SoftwareSerial.h>   // 引用程式庫
#include <ld3320.h>            // 引用程式庫
#include <IRremote.h>          // 引用紅外線命令程式庫
#include <DFPlayer_Mini_Mp3.h> // 引用播放音檔程式庫
#include <avr/pgmspace.h>      // 引用程式庫
#include <avr/wdt.h>           // 引用看門狗程式庫
#include <EEPROM.h>           // 引用arduino記憶體程式庫
byte times = 0;
byte reset_times = 0;
int mode = 0;
int eeAddress = 0;
int TV_mode = 0;　//轉輸入源，確認電視狀態
int TV_mode_now = 0;　//電視目前狀態
SoftwareSerial BT(7, 8);     // 接收腳, 傳送腳
VoiceRecognition Voice;        //定義聲控函數
IRsend irsend;                //定義紅外線函數
int LED1 = 5;
//讓arduino記憶紅外線命令
const unsigned int TV[99] PROGMEM = {3400,1750,400,450,400,1300,450,450,400,450,400,450,450,400,450,450,400,450,400,450,400,450,450,450,350,500,400,450,400,1350,400,450,400,450,400,450,450,450,400,450,400,450,400,450,450,400,400,500,400,1300,400,450,450,450,400,450,400,450,400,450,450,450,400,450,400,450,400,1350,400,450,400,1300,450,1300,400,1300,450,1300,400,450,400,450,450,1300,400,450,400,1300,450,1300,400,1300,450,1300,400,450,400,1350,400};
const unsigned int ChCl[99] PROGMEM = {3400,1750,450,400,450,1300,400,450,400,450,450,450,400,450,400,450,400,450,450,400,450,450,400,450,400,450,450,400,450,1300,400,450,450,400,450,450,400,450,400,450,450,400,450,450,450,400,400,450,400,1350,400,450,400,450,400,450,450,400,450,450,400,450,400,450,450,400,450,1300,400,450,450,1300,400,450,400,450,450,400,450,450,400,450,400,1300,450,450,400,1300,450,400,450,450,400,450,400,450,400,1350,400};
const unsigned int AirOP[83] PROGMEM = {8900,4400,600,550,600,550,600,500,600,1650,600,550,550,550,600,550,550,600,600,550,550,1650,600,550,550,1650,600,1600,600,550,600,550,600,550,550,1650,600,1650,550,550,600,550,600,550,600,500,600,550,600,550,600,550,550,550,600,550,550,600,600,1600,600,550,600,550,600,500,600,550,550,600,600,1600,600,1650,600,550,550,550,600,550,600,550,600}; 
const unsigned int AirCL[83] PROGMEM = {8900,4400,600,550,600,550,600,550,600,500,600,550,600,550,600,550,600,500,600,550,600,1650,550,550,600,1650,600,1650,550,550,600,550,600,500,600,1650,600,1600,600,550,600,550,550,550,600,550,600,550,600,550,600,500,600,550,550,600,550,600,550,1650,600,550,550,550,600,550,600,550,600,550,600,1600,600,1650,600,500,600,550,600,550,600,550,550};
const unsigned int AirDR[83] PROGMEM = {8900,4450,550,1700,500,650,500,650,500,1700,550,600,500,650,500,600,500,650,500,650,500,1700,500,650,550,1700,500,1700,500,600,550,600,550,600,500,1750,500,1700,550,600,500,650,500,650,450,650,500,650,500,650,500,650,550,550,500,650,550,600,500,1700,500,650,500,600,550,650,500,600,500,650,500,1700,550,1700,500,650,500,600,550,600,500,650,500};
const unsigned int AirTIMER[83] PROGMEM = {8950,4400,600,1650,600,500,600,550,600,1650,550,550,600,550,600,550,600,550,550,550,600,1650,600,550,600,1650,550,1650,600,500,600,550,600,550,600,1600,600,1650,550,600,550,550,600,1650,550,600,550,550,600,550,600,550,550,600,600,500,600,550,600,1650,550,550,600,1650,600,550,550,550,600,550,600,1650,600,1600,600,550,600,550,550,600,550,550,600};
unsigned int Order[100];
//定義紅外線命令
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // 與電腦序列埠連線
  mp3_set_serial (Serial); //連結播放模組
  BT.begin(9600);   //預設藍芽連線速率
  wdt_enable(WDTO_4S);   //4秒後無餵狗即重置，防止待機時當機
  pinMode(LED1,OUTPUT);　//接收到聲控指示燈
  digitalWrite(LED1,LOW);
  Voice.init();  //建立聲控命令標籤
  Voice.addCommand("la bi",0);   //啟動
  Voice.addCommand("kai deng",1);  //開燈
  Voice.addCommand("guan deng",2);  //關燈
  Voice.addCommand("jiang wen",3);  //開冷氣
  Voice.addCommand("chang wen",4);  //開除濕
  Voice.addCommand("dian shr",5);  //電視
  Voice.addCommand("sha dang",6);  //關冷氣
  Voice.addCommand("wan an",7);  //晚安
  Voice.addCommand("huan tai",8);  //轉台
  Voice.addCommand("you shi",9);  //電視轉輸入源到PS4
  Voice.addCommand("kai",200);   //添加垃圾詞彙，增加辨識準確度 
  Voice.addCommand("wu wu wu wu",200);  //添加垃圾詞彙，增加辨識準確度
  Voice.addCommand("wen tai",200);  //添加垃圾詞彙，增加辨識準確度
  Voice.addCommand("juan an",200);  //添加垃圾詞彙，增加辨識準確度
  Voice.addCommand("ha ha ha ha",200);     //添加垃圾詞彙，增加辨識準確度  
  Voice.start();  //開始聲控判讀
}
void loop() {
  reset_times ++;　//看門狗倒數
  delay(100);
  if(reset_times < 3000){
  wdt_reset();　//5分鐘後重新開機
  }
 switch(Voice.read())
 {
  case 0:
  digitalWrite(LED1,HIGH);　//燈號亮起，開始判斷遙控指令
  wdt_reset();
  times = 0;
      while(times <18){  //等待命令1.8秒
      times ++;
      delay(100);
      switch(Voice.read()){
    case 1:
    mp3_set_volume (25);播放音量 25
    mp3_play (2); //播放音檔 0001.mp3
    delay (500);
    BT.print(1); //傳送藍芽命令 "1" -- 房間電燈開關
    break;
    case 2:
    mp3_set_volume (25);
    mp3_play (2); //播放音檔 0001.mp3
    delay (500);
    BT.print(1);  //傳送藍芽命令 "1" -- 房間電燈開關
    break;
    case 3:
    mp3_set_volume (25);
    mp3_play (2); //play 0001.mp3
    delay (500);
    memcpy_P(Order,AirOP,166); //讀取Arduino內紅外線命令 (冷氣開啟)
    irsend.sendRaw(Order,83,38); //送出命令
    break;
    case 4:
    mp3_set_volume (25);
    mp3_play (2); //play 0001.mp3
    delay (500);
     memcpy_P(Order,AirDR,166); //讀取Arduino內紅外線命令 (開啟冷氣除溼功能)
    irsend.sendRaw(Order,83,38); //送出命令
    break;
    case 5:
    mp3_set_volume (25);
    mp3_play (2); //play 0001.mp3
    delay(500);
    TV_mode_now = EEPROM.get(eeAddress,TV_mode); //擷取目前電視狀態
    if(TV_mode_now == 0) //如果電視關機
    {
    memcpy_P(Order,TV,198); //打開電源
    irsend.sendRaw(Order,99,38);
    TV_mode = 1;
    TV_mode_now = 1;
    EEPROM.put(eeAddress,TV_mode);
    }
    else if(TV_mode_now == 1) //如果電視開機
    {
    memcpy_P(Order,TV,198); //關閉電源
    irsend.sendRaw(Order,99,38);
    TV_mode = 0;
    TV_mode_now = 0;
    EEPROM.put(eeAddress,TV_mode);
    }
    else if(TV_mode_now == 2) //如果現在電視在輸入源2
    {
      memcpy_P(Order,ChCl,198); //讀取轉輸入源命令，轉回輸入源1
      irsend.sendRaw(Order,99,38);
      delay(500);
      irsend.sendRaw(Order,99,38);
      delay(500);
      irsend.sendRaw(Order,99,38);
      delay(500);
      irsend.sendRaw(Order,99,38);
      delay(500);
      irsend.sendRaw(Order,99,38);
      delay(500);
      memcpy_P(Order,TV,198); //讀取電視電源命令
      irsend.sendRaw(Order,99,38); //關機
      TV_mode = 0;
      TV_mode_now = 0;
      EEPROM.put(eeAddress,TV_mode);
    }
    else if(TV_mode_now == 3)
    {
     memcpy_P(Order,ChCl,198);
     irsend.sendRaw(Order,99,38);
      delay(500);
     irsend.sendRaw(Order,99,38);
      delay(500);
     irsend.sendRaw(Order,99,38);
      delay(500);
      irsend.sendRaw(Order,99,38);
      delay(500); 
    memcpy_P(Order,TV,198);
    irsend.sendRaw(Order,99,38);
    TV_mode = 0;
    TV_mode_now = 0;
    EEPROM.put(eeAddress,TV_mode);
    }
    break;
    case 6:
    mp3_set_volume (25);
    mp3_play (2); //play 0001.mp3
    delay (500);
    memcpy_P(Order,AirCL,166); //讀取冷氣關閉命令
    irsend.sendRaw(Order,83,38); //傳送紅外線命令
    break;
    
    case 8:
    mp3_set_volume (25);
    mp3_play (2); //play 0001.mp3
    delay(500);
    TV_mode_now = EEPROM.get(eeAddress,TV_mode);
    memcpy_P(Order,ChCl,198);  //讀取轉換輸入源命令
    if(TV_mode_now == 1) //如果電視輸入源為1
    {
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    TV_mode = 3;
    TV_mode_now = 3; //轉成輸入源3，並記錄
    EEPROM.put(eeAddress,TV_mode);
    }
    else if(TV_mode_now == 2) //如果電視輸入源為2
    {
     irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
     irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    TV_mode = 1;
    TV_mode_now = 1; //轉成輸入源1，並記錄
    EEPROM.put(eeAddress,TV_mode);  
    }
    else if(TV_mode == 3) //如果電視輸入源為3
    {
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    TV_mode = 1; 
    TV_mode_now = 1; //轉成輸入源3，並記錄
    EEPROM.put(eeAddress,TV_mode);
    }
    break;
    case 9:
    mp3_set_volume (25);
    mp3_play (2); //play 0001.mp3
    TV_mode_now = EEPROM.get(eeAddress,TV_mode);
    memcpy_P(Order,ChCl,198);
    if (TV_mode_now == 0){
    delay (500);
    }
    else{
    delay (500);
    if(TV_mode_now == 1)
    {
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    TV_mode = 2;
    TV_mode_now = 2;
    EEPROM.put(eeAddress,TV_mode);
    }
    else if(TV_mode == 3)
    {
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500);
    irsend.sendRaw(Order,99,38);
    delay (500); 
    TV_mode = 2;
    TV_mode_now = 2;
    EEPROM.put(eeAddress,TV_mode);
    }
    }
    break;
    default:
    break;
  }
      }
     digitalWrite(LED1,LOW); //接受完畢命令，關閉
      break;
    default:
  break;
 }
}
