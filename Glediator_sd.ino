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
int zeile = 0;
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
    string = btSerial.readString();
    if (string.toInt() == 0){
      fxdata = SD.open(string);
      Serial.println(string);
      spiele();
      while (wiederholen){
        fxdata = SD.open(string);
        spiele();
      }
    } else {
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
      zeile = i/12;
      if (zeile % 2 != 1){
        fxdata.readBytes(out, 3);
        leds[i][0] = out[0];
        leds[i][1] = out[1];
        leds[i][2] = out[2];
      } 
      if (zeile % 2 == 1){
        fxdata.readBytes(out, 3);
        leds[zeile*12+11-(i-(zeile*12))][0] = out[0];
        leds[zeile*12+11-(i-(zeile*12))][1] = out[1];
        leds[zeile*12+11-(i-(zeile*12))][2] = out[2];
      }
      } 
    FastLED.show();
    delay(Speed - 25);
  }
  
  // close the file in order to prevent hanging IO or similar throughout time
  fxdata.close();
}
