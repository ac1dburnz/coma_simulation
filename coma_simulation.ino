#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9
#define OLED_RESET 13


int x=0;
int lastx=0;
int lasty=0;
int LastTime=0;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;

#define UpperThreshold 550
#define LowerThreshold 500
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);





void setup()   
{                
  Serial.begin(9600);
  pinMode(6, INPUT); // Setup for leads off detection LO +
  pinMode(5, INPUT); // Setup for leads off detection LO -
  display.begin(SSD1306_SWITCHCAPVCC);  // Switch OLED
  display.clearDisplay();  // Clear OLED
}

void loop() 
{
  if(x>127)  
  {
    display.clearDisplay();
    x=0;
    lastx=x;
  }

  int value=analogRead(0);
  display.setTextColor(WHITE);
  int y=60-(value/16);
  display.writeLine(lastx,lasty,x,y,WHITE);
  lasty=y;
  lastx=x;
  // calc bpm

  if(value>UpperThreshold)
  {
    if(BeatComplete)
    {
      BPM=millis()-LastTime;
      BPM=int(60/(float(BPM)/1000));
      BPMTiming=false;
      BeatComplete=false;
      tone(3,1000,250);
    }
    if(BPMTiming==false)
    {
      LastTime=millis();
      BPMTiming=true;
    }
  }
  if((value<LowerThreshold)&(BPMTiming))
    BeatComplete=true;
    // display bpm
display.writeFillRect(0,50,128,16,BLACK);
  display.setCursor(0,50);
  display.print(BPM);
  display.print(" BPM");
  display.display();
  x++;
}
