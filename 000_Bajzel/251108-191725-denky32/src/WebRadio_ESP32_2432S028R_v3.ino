//========================================================================
// WebRadio ESP32_2432S028R_v3 (CYD2USB) : org : 2024.04.23 : macsbug
//   Sketch uses 1525537 bytes (72%) of program storage space.   
// https://macsbug.wordpress.com/2022/08/20/web-radio-esp32-2432s028-i2s/
//========================================================================
// Hard : ESP32-2432S028R V3 (CYD2USB): A type with two USB ports.
//        2.8" 240x320 SPI ST7789 LCD with XPT2046 Touch
//        https://www.aliexpress.com/item/1005004502250619.html
// Hard : MAX98357A I2S 3W Class D Amplifier Board
// Dev environment   : Arduino IDE 1.8.19
//  Board Manager    : arduino-esp32 2.0.11
//  Board            : "ESP32 Dev Modlue"
//  Upload Speed     : "921600"
//  CPU Frequency    : "240MHz (WiFi/BT)" or "160MHz (WiFi/BT)"
//  Flash Frequency  : "80MHz"
//  Flash Mode       : "DIO"
//  Flash Size       : "4MB (32Mb)"
//  Partition Scheme : "No OTA (2MB APP/2MB SPIFSS)"
//  Core Degug Level : "None " or "Verbose"
//  PSRAM            : "Disabled"
//  Arduino Runs On  : "Core 1"
//  Events Run On    : "Core 1"
//  Pord             : "dev/cu.wchusbserial14240"
// Library : schreibfaul1 / ESP32-audioI2S 2.0.4, 2.0.5(Sep 05,2022)
//         : https://github.com/schreibfaul1/ESP32-audioI2S
// Library : lovyanGFX : https://github.com/lovyan03/LovyanGFX
// BLOG    : ESP32_2432S028 
//         : https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/
//========================================================================
// Display Line
// Y 0 -  30 :  1 : Title
//  30 -  60 :  2 : infomation
//  60 -  90 :  3 : Station No.
//  90 - 120 :  4 : Playing : music name
// 120 - 150 :  5 : Playing : music name
// 150 - 180 :  6 : Playing : music name
// 180 - 210 :  7 : Volume, VolumeSlider
// 210 - 240 :  8 : Button : Vol-, << , <ch, ch>, >>, Vol+
//=====================================================================

#pragma GCC optimize ("Ofast")
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "lgfx_ESP32_2432S028R_ST7789.h" // ESP32_2432S028R_CYD2USB
#include "station.h"
#include "frame.h"
#include "apple20.c"
#include <Arduino.h>
#include <WiFi.h>
#include <Audio.h>
Audio audio;
#include <Preferences.h>
Preferences pref;
//　Your settings　*****************************************************
#define BCLK 22 // are using Set to I2S pin
#define LRC  27 // are using Set to I2S pin
#define DIN  17 // are using Set to I2S pin
char ssid[] = "YOUR SSID";// your network SSID (name)
char pass[] = "YOUR PASS";// your network password
//*********************************************************************
int sn = 1, vol = 5, bl = 2, wave = 0, rx = 0, rs = 0;
int backlight[5] = {10,100,160,200,255};// Blightness
const int LED_R=4,LED_G=16,LED_B=17;// GPIO 4(RED),16(GREEN),17(BLUE)
boolean pl = false, led = false, inv = false;
char URL[100], sta[50];
uint16_t tX, tY, bc = 0; // touch x y , bgcolor
String Bn;
int b[6][12]={ // Button
  //    frame     ,     button     ,   nomen
  // 0,  1, 2, 3,4,   5,  6, 7, 8,9   10, 11
  //x1, y1,x2,y2,r,  x3, y3,x4,y4,r,  x5, y5
  {  0,210,60,30,5,   1,211,58,28,5,   3,215}, // Bd 0 vol-
  { 65,210,30,30,5,  66,211,28,28,5,  66,215}, // Bd 1 <<
  { 99,210,60,30,5, 100,211,58,28,5, 106,215}, // Bd 2 <ch
  {162,210,60,30,5, 163,211,58,28,5, 173,215}, // Bd 3 ch>
  {225,210,30,30,5, 226,211,28,28,5, 226,215}, // Bd 4 >>
  {260,210,60,30,5, 261,211,58,28,5, 263,215}  // Bd 5 vol+
};

//===========================================================
void setup() {
  adc_power_acquire(); // Bug fixes for GPIO39 and GPIO36
  pinMode(LED_R,OUTPUT);digitalWrite(LED_R,HIGH);//RED   LED off
  pinMode(LED_G,OUTPUT);digitalWrite(LED_G,HIGH);//GREEN LED off
  pinMode(LED_B,OUTPUT);digitalWrite(LED_B,HIGH);//BLUE  LED off
  Serial.begin(115200);                    //
  tft.init();                              //
  tft.setRotation(0);                      // USB is on the right
  tft.setBrightness(200);                  //
  tft.setSwapBytes(true);                  //
  tft.fillScreen  (0x0000);                // BLACK
  tft.setFont(&fonts::FreeMonoBold12pt7b); //
  MP(25,1,"Web Radio ESP32");              // x,yLine,WHITE
  bool touched = tft.getTouch( &tX, &tY);  // Touch X,Y
  if(touched){sn=0;MP(0,3,"Station 0");}   // Reset sn, 0
  // WiFi Connection ----------------------------------------
  WiFi.disconnect();                       //
  WiFi.mode(WIFI_STA);                     // tts
  WiFi.begin(ssid, pass);                  // Set ssid,pass
  WiFi.setSleep(false);                    // Prevent GPIO39 Pulse
  tft.setTextColor(0xFFFF);tft.setCursor(0,30);
  while(WiFi.status() != WL_CONNECTED){delay(500);tft.print(".");}
  MP(0,2,"Connected to Wifi");             // Infomation
  // I2S DAC Initialize -------------------------------------
  audio.setPinout(BCLK, LRC, DIN);         // Setup I2S Pin
  audio.setVolume(vol);                    // Set Vol 0...21
  speech("こんにちは","ja");                // Speech
  speech("Web Radio of M5Stack","en");     // Speech
  // Button Initialize --------------------------------------
  Btn("all","off");                        // button
  tft.pushImage(0,0,18,20,(uint16_t *)apple20);//to apple fan
  // NVM sta No. --------------------------------------------
  pref.begin("my-app",false);              // Namspace,RW mode
  if (sn==0){pref.putInt("counter",0);}    // sta=0, Save NVM
  sn = pref.getInt("counter",0);           // Read NVM Memory
  // --------------------------------------------------------
  tft.startWrite();                        // SPI Bus
  audio.connecttospeech("hello", "en");    // port 443
}
 //==========================================================
void loop() {                             // loop time 25msec
  audio.loop();//Executes an internal loop of the audio library
  touch();     //vol-,<<,<ch,ch>,>>Vol+,bl-,bl+,inv
  // not playing ********************************************
  if ( !pl ) { pl= !pl;                   // 
    tft.fillRect(268,30,47,60,bc);        // Wave off
    // sta up -----------------------------------------------
    MP(0, 2, "Wait...");                  // Infomation
    url_sta(sn);                          // station,url
    speech(sta,"en");                     // speech
    audio.connecttohost(URL);             // URL connection *
    Btn(Bn,"off");                        // Button off
    delay(999);pref.putInt("counter",sn); // Save no to NVM
    // Display update ---------------------------------------
    MP(0,3,"station:"+String(sn)          // staion NO.
                 +" / "+String(un));      // Total station
    MP(0,4,"Playing:"+String(sta));       // station name
    bar();                                // Volume
    MP(0,2,"Playing");                    // Infomation
  }
  // END of not playing **************************************

  // are playing *********************************************
  // Wave ----------------------------------------------------
  tft.pushImage(268,30,47,60,frame[int(wave)]);  // Wave on
  wave=wave+1; if(int(wave)==frames){wave=0;}    // reset
  // Play LED ------------------------------------------------
  led = !led;                                    // on/off
  if ( led){tft.fillCircle(310,8,5,0x7FF0);}     // Led on
  if (!led){tft.fillCircle(310,8,5,0x0000);}     // Led off
  // reception check -----------------------------------------
  rx=audio.getAudioCurrentTime();if(rx==0){rs++;}// reception
  if (rx==0){tft.fillCircle(310,8,5,0xFD00);}    // Led on
  if (rx>0 && !led){tft.fillCircle(310,8,5,0x0000);}//Led off
  if (rs>999){ rs=0; sn=sn+1; pl = false;        // next sta
    if(sn>un){sn=0;}                             // sn reset
    Serial.print("error sta = ");Serial.println(String(sta));
    tft.fillRect(268,30,47,60,bc);               // Wave off
    MP(0,2,"do not receive");delay(2000);        // Infomation
    MP(0,2,"Next station"  );delay(2000);        // Infomation
    pref.putUInt("counter",sn);//Save no to NVM,Reboot measures
  }
  // End of playing ******************************************

} // End of Loop

//==============================================================
void MP(int xp,int yl,String str){               // 1Line=30
  int ln = 1; if ( yl == 4){ ln = 3;}            //
  tft.fillRect (xp,30*(yl-1),320,30*ln,bc);      // clear BG
  tft.setCursor(xp+1,30*(yl-1));                 // yLine:1-8
  tft.setTextColor(0xFFFF); tft.println(str);    // Display
}
//==============================================================
void bar() {
  audio.setVolume(vol);                        // Set Volume
  MP(0,7,"Volume:");MP(100,7,String(int(vol*5)));
  tft.drawRect(150,182,162  ,16,0xFFFF);       // WHITE
  tft.fillRect(151,183,159  ,14,0x0000);       // BLACK
  tft.fillRect(151,183,vol*8,14,0x3B94);       // Bar color
  Btn("vol-","off");Btn("vol+","off");         // vol Button off
}
//==============================================================
void Btn(String b,String s){                  // Button display
  long c=0xA554;if(s.equals("on")){c=0x8FEE;} // Gray.Green
  if(b.equals("vol-") || b.equals("all")){Bd(0,c,"vol-");}
  if(b.equals("<<"  ) || b.equals("all")){Bd(1,c,"<<"  );}
  if(b.equals("<ch" ) || b.equals("all")){Bd(2,c,"<ch" );}
  if(b.equals("ch>" ) || b.equals("all")){Bd(3,c,"ch>" );}
  if(b.equals(">>"  ) || b.equals("all")){Bd(4,c,">>"  );}
  if(b.equals("vol+") || b.equals("all")){Bd(5,c,"vol+");}
}
//==============================================================
void url_sta(int c) { // Separate into URL, sta
  String u=String(url[c]).substring(0,String(url[c]).indexOf("*")+1);
  u.toCharArray(URL,u.length());           // URL
  String s=String(url[c]).substring(String(url[c]).indexOf("//")+2);
  String n=s.substring(s.indexOf("*")+1)+1;
  n.toCharArray(sta,n.length());           // station name
}
//==============================================================
void speech(char* speech, char* Lang){ // Increase volume by +5
  audio.setVolume(vol+5);audio.connecttospeech(speech,Lang);
  audio.setVolume(vol);
}
//==============================================================
void mon( uint16_t color){tft.fillCircle(290,8,5,color);}
//==============================================================
void ind( String C, uint16_t color){            // R,G,B, 0x0000
  tft.fillCircle(310,8,5,color);                   // Led on/off
  if(!C.equals("G")){digitalWrite(LED_G,HIGH);}    // LED   off
  if( C.equals("G")){digitalWrite(LED_G,LOW );}    // GREEN on
}
//==============================================================
void Bd(uint8_t y, long color, String nomen){
  tft.setTextColor (0x0000); 
  tft.fillRoundRect(b[y][0],b[y][1],b[y][2],b[y][3],b[y][4],0xFFFF);
  tft.fillRoundRect(b[y][5],b[y][6],b[y][7],b[y][8],b[y][9],color);
  tft.setCursor    (b[y][10],b[y][11]);tft.println(nomen);
}
//==============================================================
