#include <SPI.h>
#include "RF24.h"
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <stdint.h>
#include "TouchScreen.h"

#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_RESET A4 // Reset goes to Analog 4

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define motor 40 //orange wire
#define ce 44 //orange wire
#define cs 45 //yellow wire
#define photo A11 // Photoresistor at Arduino analog pin A0

int photo_value;
int packet; //1 = off, 2 = on
//int packet2; //1 = locked, 2 = open
int light_switch = 1;  //1 = off, 2 = on
int door_status = 1; //1 = locked, 2 = open
int off = 0;
int on = 90;

//graphics
int switch_w = 80;
int switch_l = 50;
int switch_x = 160;
int switch_y = 180;
int center_x = switch_x - (switch_w / 2);
int center_y = switch_y - (switch_l / 2);

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET); //ILI9341 240x320 2.8 inch
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
RF24 light_radio (ce, cs);
Servo servo; //create Servo instance

byte addresses[][6] = {"pipe1"}; // Create address for 1 pipe.

void setup() {
  pinMode(photo, INPUT);
  Serial.begin(115200);
  delay(1000);
  light_radio.begin();  // Start up the physical nRF24L01 Radio
  light_radio.setChannel(108);  // Above most Wifi Channels
  light_radio.setPALevel(RF24_PA_MIN);
  light_radio.openReadingPipe(1, addresses[0]); // pipe1
  light_radio.startListening(); //begin receiving data
  Serial.println("Receiver ready");

  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  Serial.println("TFT ready");

  servo.attach(motor);
  servo.write(off);
  Serial.println("Servo ready");
}

void loop() {
  /******************TFT***************************/
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  tft.fillRect(center_x, center_y, switch_w, switch_l, RED); //x,y,w,l light switch

  //change touch screen coordinates into drawing coordinates
  p.x = map(p.x, 120, 950, 0, 320);
  p.y = map(p.y, 110, 920, 0, 240);
  tft.drawRect(center_x, center_y, switch_w, switch_l, RED);

  //  if (p.z > 0) { //pressure is applied
  //    Serial.print("X = "); Serial.print(p.x);
  //    Serial.print("  Y = "); Serial.print(p.y);
  //    Serial.print("\tPressure = "); Serial.println(p.z);
  //    Serial.println();
  //  }

  if (p.z > 0 && p.x >= center_x && p.y >= center_y && p.x <= center_x + switch_w && p.y <= center_y + switch_l) { //button is pressed
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("  Y = "); Serial.print(p.y);
    Serial.println();
    Serial.println("button pressed!");
    tft.drawRect(center_x, center_y, switch_w, switch_l, WHITE); //x,y,w,l
    tft.fillScreen(BLACK);
    //switch the light
    if (light_switch == 1) {
      light_switch = 2;
    }
    else {
      light_switch = 1;
    }
  }

  //light status
  tft.setCursor(0, 20);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("Light is: ");
  tft.setCursor(170, 20);
  if (light_switch == 2) {
    tft.setTextColor(GREEN);
    tft.print("ON");
    servo.attach(motor);
    servo.write(on);
  }
  else {
    tft.setTextColor(RED);
    tft.print("OFF");
    servo.attach(motor);
    servo.write(off);
  }

  //door status
  tft.setCursor(0, 100);
  tft.setTextColor(WHITE);
  tft.print("Door is: ");
  tft.setCursor(150, 100);
  if (door_status == 2) {
    tft.setTextColor(GREEN);
    tft.print("OPEN");
  }
  else {
    tft.setTextColor(RED);
    tft.print("LOCKED");
  }

  /******************NRF2401+***************************/

  photo_value = analogRead(photo);

  light_radio.read( &packet, sizeof(packet) ); //retrieve the data

  if (packet == 1) {
    tft.fillScreen(BLACK);
    door_status = 1;
    Serial.println("door status: locked");
  }
  else if (packet == 2) {
    tft.fillScreen(BLACK);
    door_status = 2;
    Serial.println("door status: open");
    Serial.print("Photoresistor: ");
    Serial.println(photo_value);
  }

  if (packet == 2 && photo_value >= 700) { //if the door is opened and it is dark
    light_switch = 2;
    tft.fillScreen(BLACK);
    Serial.println("turn on the lights");
    Serial.print("Light switch: ");
    Serial.println(light_switch);
    Serial.print("Photoresistor: ");
    Serial.println(photo_value);
  }

  /***************END***************/
}


