//Coding ini dibuat oleh Dani_Rajacell untuk produk Digital Clock Module 6-Digit TM1637
//https://tokopedia.com/rajacell/module-7-segment-jam-digital-clock-module-6-digit-tm1637-dengan-push-button
//https://tokopedia.com/rajacell/ds3231-i2c-rtc-module-for-arduino-real-time-clock-iic-module
//https://www.facebook.com/dani.ardianto.rajacell

#include <Arduino.h>
#include <TM1637TinyDisplay6.h> //Library TM1637 6-digit RJC
#include <DS3231.h> //Library RTC3231 
#include <Wire.h>  //Library RTC3231 

// Module Seven Segment 6digits TM1637 RJC ----------------------
#define CLK 4
#define DIO 5
TM1637TinyDisplay6 display(CLK, DIO); 

// Init RTC DS3231 ----------------------------------------------
DS3231  rtc(SDA, SCL);
Time  t; //pencacah string time()

/*
//Mengambil waktu jam dari waktu Compile/Upload di system Windows
uint32_t targetTime = 0;
uint8_t conv2d(const char* p) {
uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
   v = *p - '0';
  return 10 * v + *++p - '0';}
*/

uint8_t hh = 0, mm = 0, ss = 0, dd = 0, bb = 0, DAY = 1, ssLast = 0;
int yy = 0;
char* Day ;
float suhu = 0;
uint8_t alarmHH = 9, alarmMM = 30;
int alarmLONG = 5;
uint8_t setMode = 0, setting=0, setAlarm = 0, alarmMode=0;
bool boot = 0;

//Tombol TM1637 ------------------------------------------------------
//SET     0x1
//UP      0x2
//DOWN    0x3
//ALR     0x4
//RELEASE 0xFF
#include <ErriezTM1637.h> //Library TM1637 Scan Keys
TM1637 tm1637(CLK, DIO);
static uint8_t keysLast = 0;
uint8_t keys;
//--------------------------------------------------------------------
unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 500;    // waktu bouncing tombol menu
int delayShow = 1500;


void setup() {
//Serial.begin(115200);
display.begin();

// Test Vertical Level Meter
  for (int count = 0; count < 3; count++) {
    for (int x = 0; x <= 100; x = x + 10) {
      display.showLevel(x, false);
      delay(10);
    }
    for (int x = 100; x > 0; x = x - 10) {
      display.showLevel(x, false);
      delay(10);
    }
  }
display.showString("RAJACELL BEKASI"); 
delay(500);
display.showString("DIGITAL CLOCK"); 
delay(1000);

rtc.begin();
//rtc.setTime (17, 40, 00); 
//rtc.setDate(21, 10, 2023); 
//rtc.setDOW(6); //minggu=0,senin=1,dst.

}

void loop() {

t = rtc.getTime();
if (setMode == 0){
hh = t.hour,DEC;
mm = t.min,DEC;
ss = t.sec,DEC;
dd = t.date,DEC;
bb = t.mon,DEC;
yy = t.year,DEC;

if(boot==0){
showClock(); delay(delayShow);
showDay(); delay(delayShow);
showDate(); delay(delayShow);
showTemp(); delay(delayShow);  
boot=1;
}
if(ssLast != ss) {ssLast = ss; //showClock();
if(ss<50)showClock();
if(ss>=50 && ss<54)showDay();
if(ss>=54 && ss<57)showDate();
if(ss>=57)showTemp();
}
  
else {
tm1637.begin();
keys = tm1637.getKeys();
if (keysLast != keys) { keysLast = keys;}
   if (keys == 0x0) {display.showString("-SET"); setMode=1; delay(1000); display.clear();}
   if (keys == 0x1) {showDay(); delay(delayShow);}
   if (keys == 0x2) {showDate(); delay(delayShow);}
   if (keys == 0x3) {showTemp(); delay(delayShow);}
  }
}

if (setMode == 1){
 tm1637.begin();
keys = tm1637.getKeys();
if (keysLast != keys) { keysLast = keys;}
  
   if(setting==0){ //Seting JAM
      display.showString("jj",2,0,1); display.showNumber(hh,1,2,2);
      if (keys == 0x0 && (millis() - lastDebounceTime) > debounceDelay) { lastDebounceTime = millis(); setting++;}
      if (keys == 0x1 && hh<23) {hh++; display.showNumber(hh,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x2 && hh>0) {hh--;  display.showNumber(hh,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x3) {setMode=0; setting=0; display.showString("CANCEL",6,0,1); delay(500);} 
  }

     if(setting==1){ //Seting Menit
      display.showString("nn",2,0,1); display.showNumber(mm,1,2,2);
      if (keys == 0x0 && (millis() - lastDebounceTime) > debounceDelay) { lastDebounceTime = millis(); setting++;}
      if (keys == 0x1 && mm<59) {mm++; display.showNumber(mm,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x2 && mm>0) {mm--;  display.showNumber(mm,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x3) {setMode=0; setting=0; display.showString("CANCEL",6,0,1); delay(500);} 
  }

     if(setting==2){ //Seting Tanggal
      display.showString("tt",2,0,1); display.showNumber(dd,1,2,2);
      if (keys == 0x0 && (millis() - lastDebounceTime) > debounceDelay) { lastDebounceTime = millis(); setting++;}
      if (keys == 0x1 && dd<31) {dd++; display.showNumber(dd,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x2 && dd>0) {dd--;  display.showNumber(dd,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x3) {setMode=0; setting=0; display.showString("CANCEL",6,0,1); delay(500);} 
  }

      if(setting==3){ //Seting Bulan
      display.showString("bL",2,0,1); display.showNumber(bb,1,2,2);
      if (keys == 0x0 && (millis() - lastDebounceTime) > debounceDelay) { lastDebounceTime = millis(); setting++;}
      if (keys == 0x1 && bb<12) {bb++; display.showNumber(bb,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x2 && bb>0) {bb--;  display.showNumber(bb,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x3) {setMode=0; setting=0; display.showString("CANCEL",6,0,1); delay(500);} 
  }

      if(setting==4){ //Seting Tahun
      display.showString("Th",2,0,1); display.showNumber(yy,1,2,2);
      if (keys == 0x0 && (millis() - lastDebounceTime) > debounceDelay) { lastDebounceTime = millis(); setting++;}
      if (keys == 0x1 && yy<2099) {yy++; display.showNumber(yy,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x2 && yy>2020) {yy--; display.showNumber(yy,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x3) {setMode=0; setting=0; display.showString("CANCEL",6,0,1); delay(500);}
  }

      if(setting==5){ //Seting Hari
      display.showString("Hr",2,0,1); display.showNumber(DAY,1,2,2);
      if (keys == 0x0 && (millis() - lastDebounceTime) > debounceDelay) { lastDebounceTime = millis(); 
          setRtc(hh,mm,dd,bb,yy,DAY); setMode=0; setting=0; display.showString("done",4,0,1); delay(1000);}
      if (keys == 0x1 && DAY<7) {DAY++; display.showNumber(DAY,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x2 && DAY>1) {DAY--; display.showNumber(DAY,1,2,2); delay(200);}  //num, zero, lenght, pos
      if (keys == 0x3) {setMode=0; setting=0; display.showString("CANCEL",6,0,1); delay(500);}
  }
  
} //end set mode
//Looping Debug here
} //end loop

void showClock (){
  if(ss%2==0){
if(hh<10){display.showNumberDec(hh, 0x40, true, 2, 0);} // (num,dots, zero, lenght, pos)
else{display.showNumberDec(hh, 0x40, false, 2, 0);}
}
else {
if(hh<10){display.showNumber(hh,1,2,0);}
else{display.showNumber(hh,0,2,0);}  
}

if(mm<10){display.showNumber(mm,1,2,2);}
else{display.showNumber(mm,0,2,2);}

if(ss<10){display.showNumber(ss,1,2,4);}
else{display.showNumber(ss,1,2,4);}
}

void showDate (){
if(dd<10){display.showNumber(dd,1,2,0);}
else{display.showNumber(dd,0,2,0);}  
if(bb<10){display.showNumber(bb,1,2,2);}
else{display.showNumber(bb,0,2,2);}
if(yy<10){display.showNumber(yy,1,2,4);}
else{display.showNumber(yy,1,2,4);} 
}

char degree[] = "\xB0";
void showTemp (){
  suhu = rtc.getTemp();
  display.showNumber(suhu,2,4,0);
  display.showString(degree, 1, 4);
  display.showString("C", 1, 5);
}

void showDay (){
Day = rtc.getDOWStr(0);
display.showString(Day,6,0,0);
}

// Seting RTC ( jam, menit, tanggal, bulan, tahun, hari )
void setRtc (uint8_t HH, uint8_t MM, uint8_t DD, uint8_t BB, int YY, uint8_t DAY ){
rtc.setTime (HH, MM, 00); //JAM, MENIT, DETIK
rtc.setDate(DD, BB, YY); //TGL, BLN, THN
rtc.setDOW(DAY); //HARI senin=1

}
