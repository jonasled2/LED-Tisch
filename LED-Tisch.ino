// Glediator Arduino UNO sketch by Jens Andrée
// 500k bauds with 80 pixels no problem
// sdcard stream for stand-alone operation.

#include <SoftwareSerial.h>
#include <FastLED.h>
#include <SPI.h>
#include <SD.h>

#define NUM_LEDS 144
#define DATA_PIN 3
#define CMD_NEW_DATA 1

File fxdata;
CRGB leds[NUM_LEDS];
SoftwareSerial btSerial(5, 6);

String string = "";
String file ="";
boolean BTconnected = false;
boolean gesendet = false;
boolean wiederholen = false;
byte out[3];
int temp = 0;
int Speed = 50;

void setup()
{
  pinMode(8, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); //se doc for different LED strips
  Serial.begin(115200);
  btSerial.begin(9600);
  
  for(int y = 0 ; y < NUM_LEDS ; y++) 
  {
    leds[y] = CRGB::Black; // set all leds to black during setup
  }
  FastLED.show();

  pinMode(10, OUTPUT); // CS/SS pin as output for SD library to work.
  digitalWrite(10, HIGH); // workaround for sdcard failed error...

  if (!SD.begin(4))
  {
    Serial.println("sdcard initialization failed!");
    return;
  }
  Serial.println("sdcard initialization done.");
  fxdata = SD.open("Boot.dat");   
  spiele();
  
}

void loop()
{
  while (!BTconnected)
  {
    if ( digitalRead(7)==HIGH)  {
      Serial.println("BT verbunden");
      BTconnected = true;
      gesendet = true;
      }
  }
  
  if (BTconnected and digitalRead(7)==LOW){
    Serial.println("BT getrennt");
    BTconnected = false;
    }
    
  if(gesendet){
  File dataFile = SD.open("list.txt");
  if (dataFile) {
    while (dataFile.available()) {
     btSerial.write(dataFile.read());
    }
    // close the file:
    dataFile.close();
  }
  gesendet = false;
  }
  if(digitalRead(8)==LOW && !wiederholen){
    wiederholen = true;
    digitalWrite(9, HIGH);
    while(digitalRead(8)==LOW){
      delay(10);
    }
  }else{
    if(digitalRead(8)==LOW && wiederholen){
      wiederholen = false;
      digitalWrite(9, LOW);
      while(digitalRead(8)==LOW){
        delay(10);
      }
    }
  }
  if(btSerial.available() > 0) {
    delay(100);
    string = btSerial.readString();
    Serial.println(string);
    if (string.toInt() == 0){
      if (string == "loop"){
        if (!wiederholen){
          wiederholen = true;
          digitalWrite(9, HIGH);
        } else {
          wiederholen = false;
          digitalWrite(9, LOW);
        }
      } else if(string == "wuerfel"){
        wuerfel();
      } else if(string == "zufaellig"){
        zufall();
      } else {
        fxdata = SD.open(string);
        spiele();
        while (wiederholen){
          fxdata = SD.open(string);
          spiele();
        }
        btSerial.println("OK");
      } 
    }else {
      Serial.println(Speed);
      Speed = 1000 / string.toInt();
    }
  }
}

void spiele()
{
  if (fxdata)
    {
      Serial.println("file open ok");      
    }
  while (fxdata.available()) 
  {
      if(digitalRead(8)==LOW && wiederholen){
        wiederholen = false;
        digitalWrite(9, LOW);
        while(digitalRead(8)==LOW){
        }
      }
     for (int i=0; i <= 143; i++){ //Spalten
      temp = i/12;
      if (temp % 2 != 1){
        fxdata.readBytes(out, 3);
        leds[i][0] = out[0];
        leds[i][1] = out[1];
        leds[i][2] = out[2];
      } 
      if (temp % 2 == 1){
        fxdata.readBytes(out, 3);
        leds[temp*12+11-(i-(temp*12))][0] = out[0];
        leds[temp*12+11-(i-(temp*12))][1] = out[1];
        leds[temp*12+11-(i-(temp*12))][2] = out[2];
      }
      } 
    FastLED.show();
    delay(Speed - 25);
    if(btSerial.available() > 0) {
      wiederholen = false;
      break;
    }
  }
  
  // close the file in order to prevent hanging IO or similar throughout time
  fxdata.close();
}

void wuerfel(){
  while(true){
    temp = millis();
    while(digitalRead(8) == HIGH){    
      if (digitalRead(9) == HIGH){
        digitalWrite(9, LOW);
      } else{
        digitalWrite(9,HIGH);
      }
      delay(500);
      temp = millis();
    }
    delay(50);
    temp = random(1,7);
    while(digitalRead(8) == LOW){
      for (int i=0; i <= 143; i++){
        leds[i] = CRGB::Black;
      }
      if (temp == 1 || temp == 3 || temp == 5){
        leds[65] = CRGB::White;
        leds[66] = CRGB::White;
        leds[77] = CRGB::White;
        leds[78] = CRGB::White;
      }
      if (temp > 1){
        leds[120] = CRGB::White;
        leds[143] = CRGB::White;
        leds[121] = CRGB::White;
        leds[142] = CRGB::White;
        leds[10] = CRGB::White;
        leds[11] = CRGB::White;
        leds[12] = CRGB::White;
        leds[13] = CRGB::White;
      }
        if (temp > 3){
        leds[0] = CRGB::White;
        leds[1] = CRGB::White;
        leds[23] = CRGB::White;
        leds[22] = CRGB::White;
        leds[132] = CRGB::White;
        leds[133] = CRGB::White;
        leds[131] = CRGB::White;
        leds[130] = CRGB::White;
      }
        if (temp == 6){
        leds[70] = CRGB::White;
        leds[71] = CRGB::White;
        leds[72] = CRGB::White;
        leds[73] = CRGB::White;
        leds[61] = CRGB::White;
        leds[60] = CRGB::White;
        leds[82] = CRGB::White;
        leds[83] = CRGB::White;
      }
      FastLED.show();
      delay(50);
      if(temp >= 6){
        temp = 1;
      } else {
      temp++;
      }
      }
      if(btSerial.available() > 0) {
        Serial.println("break");
        break;
      }
  }
  if(wiederholen){
    digitalWrite(9, HIGH);
  } else {
    digitalWrite(9, LOW);
}
  }
void zufall(){
  temp = 1;
  while(temp){
    leds[random(0, 144)].setRGB(random(0, 255), random(0, 255), random(0, 255));
    FastLED.show();
    if(btSerial.available() > 0) {
      delay(100);
      loop();
    }
  }
}

