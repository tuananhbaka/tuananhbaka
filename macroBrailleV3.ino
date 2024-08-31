#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Keyboard.h>
#include <Mouse.h>

SoftwareSerial mySoftwareSerial(10, 16); // TX-2; RX-3
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

const byte pinButton = 5;
byte Button[pinButton] = {2,3,4,5,6};
byte ValueButton[pinButton] = {0};
byte ValuePButton[pinButton] = {1, 1, 1, 1, 1};
unsigned int lastTime[pinButton] = {0};
unsigned int Time[pinButton] = {0};
const byte pinButtonB = 6;
byte ButtonB[pinButtonB] = {14,15,A0,A1,A2,A3};
byte ValueButtonB[pinButtonB] = {0};
byte ValuePButtonB[pinButtonB] = {1, 1, 1, 1, 1, 1};
unsigned int lastTimeB[pinButtonB] = {0};
unsigned int TimeB[pinButtonB] = {0};

const byte pinButtonC = 3;
byte ButtonC[pinButtonC] = {7,8,9};
byte ValueButtonC[pinButtonC] = {0};
byte ValuePButtonC[pinButtonC] = {1, 1, 1};
unsigned int lastTimeC[pinButtonC] = {0};
unsigned int TimeC[pinButtonC] = {0};

byte Songlistchars[] = {
   9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
  19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
  31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
  43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 
  55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 
  67, 68, 69, 70
};

byte Value=0;
byte ValueA;
byte Check[]={0,0,0,0,0,
              0,0,0,0,0,
              0,0,0,0,0,
              0,0,0,0,0,
              0,0,0,0,0,
              0,0,0,0,0,};
String XauKtr="";
String Ktr="52";
const byte ValueString = 37;              // Kích thước của mảng
const char* ListString[ValueString] = {    // Mảng chứa các chuỗi
  "311721","111694222","2917354527","21920","1320",
  "273617","1136","2716344232","19942","29942",
  "32222827281013","30222512","291741","122845","273521",
  "201345","103641","112221","16229422115","2728344121",
  "92116","271628344221","20172116","11162241","20134222",
  "11941","21344128","113920","16224511","27254039422115",
  "21152832354421","16283545","271721","27404111","27163541",
  "1517394117","211540394217"
};
byte SonglistString[]={
  84,85,86,87,88,
  89,90,91,92,93,
  94,95,96,97,98,
  99,100,101,102,103,
  104,105,106,107,108,
  109,110,111,112,113,
  114,115,116,117,118,
  119,120
};
bool FoundStringinlist=false;
byte So_dem=0;
void setup() {
  for (int i = 0; i < pinButton; i++) {
    pinMode(Button[i], INPUT_PULLUP);
  }
  for (int i = 0; i < pinButtonB; i++) {
    pinMode(ButtonB[i], INPUT_PULLUP);
  }
  for (int i = 0; i < pinButtonC; i++) {
    pinMode(ButtonC[i], INPUT_PULLUP);
  }
  Serial.begin(115200);
  Keyboard.begin();
  Mouse.begin();
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) 
  {  
      while(true)
      {
        delay(0);
      }
  }
  myDFPlayer.volume(30);  //cài đặt volume từ 0 đến 30
  myDFPlayer.play(1);
}

void loop() {
  buttons();
  XLBraille();
}

void buttons() {
  for (int i = 0; i < pinButton; i++) {
    ValueButton[i] = digitalRead(Button[i]);
    Time[i] = millis() - lastTime[i];
    if (Time[i] > 10 && ValueButton[i] != ValuePButton[i]) {
      lastTime[i] = millis();
      if (ValueButton[i] == LOW) {
        handleButtonPress(i);
      } else {
        handleButtonRelease(i);
      }
      delay(100); // Giữ phím trong 100ms
      Keyboard.releaseAll(); // Nhả tất cả các phím
      ValuePButton[i] = ValueButton[i];
    }
  }
}

void handleButtonPress(int i) {
  if (i == 0) {
    if(Value>0){
      if(ValueA!=1){
        myDFPlayer.play(2);
        delay(600);
        Value-=1;
        myDFPlayer.play(Check[Value]);
        delay(100);
      }
      else{
        myDFPlayer.play(83);
        delay(600);
        Value=0;
      }
      Keyboard.press(KEY_BACKSPACE);
      ValueA=0;
    }
    else if (Value==0){
      if(ValueA!=1){
        myDFPlayer.play(82);
        delay(100);
      }
      else{
        myDFPlayer.play(83);
        delay(100);
        Keyboard.press(KEY_BACKSPACE);
        Value=0;
        ValueA=0;
      }
    }
  } else if (i == 1) {
    myDFPlayer.play(3);
    delay(100);
  } else if (i == 3) {
    myDFPlayer.play(5);
    delay(100);
    Keyboard.press(KEY_RETURN);
    Value=0;
  } else if (i == 4) {
    if(ValueButtonB[0]==1 && ValueButtonB[1]==1 && ValueButtonB[2]==1 && 
       ValueButtonB[3]==1 && ValueButtonB[4]==1 && ValueButtonB[5]==1){
      myDFPlayer.play(79);
      Value=0;
      XauKtr="";
    }
    else{
      //Kiểu tra và phát các ký tự
      if(Value!=0){
        for (int j=0;j<Value;j++){
          XauKtr+=String(Check[j]);
          if(XauKtr.endsWith("52")){
            XauKtr=XauKtr.substring(0, XauKtr.length() - Ktr.length() );
            Serial.println(XauKtr);
            for (int i = 0; i < ValueString; i++) {
              if (XauKtr.equalsIgnoreCase(ListString[i])) { 
                myDFPlayer.play(SonglistString[i]);
                delay(1000);
                XauKtr="";
                So_dem=j+1;               
                break;                    
              }
              else{
                FoundStringinlist = true; 
              }
            }
            if(FoundStringinlist==true){
              XauKtr="";
              for (So_dem;So_dem<j;So_dem++){
                myDFPlayer.play(Check[So_dem]);
                delay(800);
              }
              So_dem=j+1;
              FoundStringinlist = false;
            }
          }
        }
        So_dem=0;
      }
      else{
        myDFPlayer.play(81);
      }
    }
  }
}

void handleButtonRelease(int i) {
  if (i == 2) {
    myDFPlayer.play(4);
    delay(100);
    Keyboard.press(KEY_LEFT_GUI); // Nhấn phím Windows
    Keyboard.press('s');          // Nhấn phím S
  }
}

void XLBraille() {
  for (int i = 0; i < pinButtonB; i++) {
    ValueButtonB[i] = digitalRead(ButtonB[i]);
    TimeB[i] = millis() - lastTimeB[i];
    if (TimeB[i] > 10 && ValueButtonB[i] != ValuePButtonB[i]) {
      lastTimeB[i] = millis();
      ValuePButtonB[i] = ValueButtonB[i];
    }
  }
  for (int i = 0; i < pinButtonC; i++) {
    ValueButtonC[i] = digitalRead(ButtonC[i]);
    TimeC[i] = millis() - lastTimeC[i];
    if (TimeC[i] > 10 && ValueButtonC[i] != ValuePButtonC[i]) {
      lastTimeC[i] = millis();
      if (ValueButtonC[i] == LOW) {
        handleBrailleConditions(i, ValueButtonB);
      } 
      ValuePButtonC[i] = ValueButtonC[i];
    }
  }
}

bool matchPattern(byte pattern[6], byte *valueButtonB) {
  for (int k = 0; k < 6; k++) {
    if (pattern[k] != valueButtonB[k]) {
      return false;
    }
  }
  return true;
}

void processPattern(const char *pattern, int i, long int j, bool& patternMatched) {
  if((i==0) || (i==1)){
    myDFPlayer.play(Songlistchars[j]);
    if(ValueButton[1]!=LOW){
      Check[Value]=Songlistchars[j];
      Value+=1;
    }
  }
  else if (i==2){
    myDFPlayer.play(Songlistchars[j+44]);
    if((ValueButton[1]!=LOW) && (strcmp(pattern, "/") != 0) && ((strcmp(pattern, "EC") != 0))){
      Check[Value]=Songlistchars[j+44];
      Value+=1;
    }
  }
  if(Check[0]==52){
          for (int i=0;i<Value;i++){
            Check[i]=Check[i+1];
          }
          Value-=1;
        }
  delay(500);
  	if (ValueButton[1]==LOW) {
        if (strcmp(pattern, "c") == 0 || strcmp(pattern, "C") == 0 ||
            strcmp(pattern, "l") == 0 || strcmp(pattern, "L") == 0 ||
            strcmp(pattern, "p") == 0 || strcmp(pattern, "P") == 0 ||
            strcmp(pattern, "s") == 0 || strcmp(pattern, "S") == 0 ||
            strcmp(pattern, "v") == 0 || strcmp(pattern, "V") == 0 ||
            strcmp(pattern, "a") == 0 || strcmp(pattern, "A") == 0 ) {
              if(strcmp(pattern, "c") == 0 || strcmp(pattern, "C") == 0){
                myDFPlayer.play(72);
                ValueA=0;
              }
              else if(strcmp(pattern, "l") == 0 || strcmp(pattern, "L") == 0){
                myDFPlayer.play(73);
                ValueA=0;
              }
              else if(strcmp(pattern, "p") == 0 || strcmp(pattern, "P") == 0){
                myDFPlayer.play(74);
                ValueA=0;
              }
              else if(strcmp(pattern, "s") == 0 || strcmp(pattern, "S") == 0){
                myDFPlayer.play(75);
                ValueA=0;
              }
              else if(strcmp(pattern, "v") == 0 || strcmp(pattern, "V") == 0){
                myDFPlayer.play(76);
                ValueA=0;
              }
              else if(strcmp(pattern, "a") == 0 || strcmp(pattern, "A") == 0){
                myDFPlayer.play(77);
                ValueA=1;
              }
              delay(100);
             Keyboard.press(KEY_LEFT_CTRL);
        }
        else {
          patternMatched = false;
          return;
        }
    }
    if(pattern=="EC"){
      myDFPlayer.play(71);
      delay(100);
      for (int i=0;i<=20;i++){
        Mouse.move(100000,1000,0);
      }
      Mouse.move(600,630,600);
      Mouse.click(MOUSE_LEFT);
    }
    else{
    Keyboard.print(pattern);
    delay(100);
    Keyboard.releaseAll();
    }
    patternMatched = true;
}

void handleBraillePatterns(int i, byte *ValueButtonB, byte patterns[][6],
                           char **chars, char **charsUpper, char **charsValue,
                           int size) {
  bool patternMatched = false; // Đánh dấu khi đã tìm thấy mẫu khớp
  for (int j = 0; j < size; j++) {
    if (matchPattern(patterns[j], ValueButtonB)) {
      if(i==0 || i==1){
        if (j<32){
          switch(i){
            case 0:
            if(ValueButton[1]!=LOW){
              myDFPlayer.play(7);
              delay(500);
            }
            processPattern(chars[j], i, j, patternMatched);
            break;
            case 1:
            if(ValueButton[1]!=LOW){
              myDFPlayer.play(8);
              delay(500);
            }
            processPattern(charsUpper[j], i, j, patternMatched);
            break;
          }
        }
        else{
          processPattern(chars[j], i, j, patternMatched);
        }
      }
      else if (i==2){
        processPattern(charsValue[j], i, j, patternMatched);
      }
      if (patternMatched) {
        break;
      }
    }
  }
  if (patternMatched != true){
      myDFPlayer.play(78);
  }
}

void handleBrailleConditions(int i, byte *ValueButtonB) {
  byte braillePatterns[44][6] = {
      {1, 0, 0, 0, 0, 0},  // a 1
      {1, 0, 1, 0, 0, 0},  // b 1
      {1, 1, 0, 0, 0, 0},  // c 1
      {1, 1, 0, 1, 0, 0},  // d 1
      {1, 0, 0, 1, 0, 0},  // e 1
      {1, 1, 1, 0, 0, 0},  // f 1
      {1, 1, 1, 1, 0, 0},  // g 1
      {1, 0, 1, 1, 0, 0},  // h 1
      {0, 1, 1, 0, 0, 0},  // i 1
      {1, 0, 0, 0, 1, 0},  // k 1
      {1, 0, 1, 0, 1, 0},  // l 1
      {1, 1, 0, 0, 1, 0},  // m 1
      {1, 1, 0, 1, 1, 0},  // n 1
      {1, 0, 0, 1, 1, 0},  // o 1
      {1, 1, 1, 0, 1, 0},  // p 1
      {1, 1, 1, 1, 1, 0},  // q 1
      {1, 0, 1, 1, 1, 0},  // r 1
      {0, 1, 1, 0, 1, 0},  // s 1
      {0, 1, 1, 1, 1, 0},  // t 1
      {1, 0, 0, 0, 1, 1},  // u 1
      {1, 0, 1, 0, 1, 1},  // v 1
      {0, 1, 1, 1, 0, 1},  // w 1
      {1, 1, 0, 0, 1, 1},  // x 1
      {1, 1, 0, 1, 1, 1},  // y 1
      {1, 0, 0, 1, 1, 1},  // z 1
      {1, 0, 0, 0, 0, 1},  // â 1
      {1, 0, 1, 0, 0, 1},  // ê 1
      {1, 1, 0, 1, 0, 1},  // ô 1
      {0, 1, 0, 1, 1, 0},  // ă 1
      {0, 1, 1, 0, 1, 1},  // đ 1
      {0, 1, 1, 0, 0, 1},  // ơ 1
      {1, 0, 1, 1, 0, 1},  // ư//33
      {0, 0, 0, 1, 1, 0},  // sac//34
      {0, 0, 0, 1, 0, 1},  // huynh//35
      {0, 0, 1, 0, 0, 1},  // hoi//36
      {0, 0, 0, 0, 1, 1},  // nga//37
      {0, 0, 0, 0, 0, 1},  // nang//38
      {0, 0, 1, 1, 0, 0},  // :cham//39
      {0, 0, 1, 0, 0, 0},  // ,//40
      {0, 0, 1, 0, 1, 0},  // ;//41
      {0, 0, 1, 1, 0, 1},  // .//42
      {0, 0, 1, 1, 1, 0},  // !//43
      {0, 0, 1, 0, 0, 1},  // ?/44
      {0, 0, 0, 0, 0, 0},  //    //17
  };

  byte braillePatternsValue[19][6] = {
      {1, 1, 1, 0, 0, 0},  // +
      {0, 0, 0, 0, 1, 1},  // -
      {0, 0, 1, 0, 1, 1},  // *
      {0, 0, 1, 1, 0, 1},  // :
      {0, 0, 1, 1, 1, 1},  // =
      {0, 1, 1, 0, 0, 1},  // <
      {1, 0, 0, 1, 1, 0},  // >
      {0, 1, 1, 1, 0, 0},  // 0
      {1, 0, 0, 0, 0, 0},  // 1
      {1, 0, 1, 0, 0, 0},  // 2//10
      {1, 1, 0, 0, 0, 0},  // 3//11
      {1, 1, 0, 1, 0, 0},  // 4//12
      {1, 0, 0, 1, 0, 0},  // 5//13
      {1, 1, 1, 0, 0, 0},  // 6//14
      {1, 1, 1, 1, 0, 0},  // 7//15
      {1, 0, 1, 1, 0, 0},  // 8//16
      {0, 1, 1, 0, 0, 0},   // 9//17
      {0, 1, 1, 1, 1, 0}, 
      {0, 0, 1, 0, 0, 0}
  };

  char *brailleChars[44] = {
      "a",    "b",     "c",  "d",  "e",  "f",   "g",     "h",    "i",
      "k",    "l",     "m",  "n",  "o",  "p",   "q",     "r",    "s",
      "t",    "u",     "v",  "ww",  "x",  "y",   "z",     "aa",   "ee",
      "oo",   "aw",    "dd", "ow", "uw", "s", "f", "r",  "x",
      "j", ":", ",",  ";",  ".",  "!",   "?"," "};

  char *brailleCharsUpper[44] = {
      "A",    "B",     "C",  "D",  "E",  "F",   "G",     "H",    "I",
      "K",    "L",     "M",  "N",  "O",  "P",   "Q",     "R",    "S",
      "T",    "U",     "V",  "Ww",  "X",  "Y",   "Z",     "Aa",   "Ee",
      "Oo",   "Aw",    "Dd", "Ow", "Uw", "s", "f", "r",  "x",
      "j", ":", ",",  ";",  ".",  "!",   "?"," "};

  char *brailleCharsValue[19] = {"+", "-", "*", ":", "=", "<", ">", "0", "1",
                                "2", "3", "4", "5", "6", "7", "8", "9", "/", "EC"};
  if (i == 0 || i == 1) {
    handleBraillePatterns(i, ValueButtonB, braillePatterns, brailleChars,
brailleCharsUpper, nullptr, 44);
  } else if (i == 2) {
    handleBraillePatterns(i, ValueButtonB, braillePatternsValue, nullptr,
                          nullptr, brailleCharsValue, 19);
  }
}
