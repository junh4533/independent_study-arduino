#include <SPI.h> //initialize the SPI library
#include <MFRC522.h> //initialize the RFID library
#include "RF24.h"
#include <Servo.h> //initialize the servo library

#define green_LED 2
#define yellow_LED 3
#define red_LED 4
#define buzzer 5

#define motion 8
#define button 22
#define hall 23
#define motor 40

#define ce 44 //orange wire
#define cs 45 //yellow wire

#define reset 48
#define sda 53 //data line

int mag;
int unlock = 35; //35,115
int lock = 115;
int pid;
int button_status;
int door_status = 1; //1 = locked, 2 = unlocked
byte uid[4]; //initialize uid variable with 4 elements

MFRC522 mfrc522(sda, reset);  //create MFRC522 instance with pin 48 as reset and pin 53 as SDA
Servo servo; //create Servo instance
RF24 door_radio (ce, cs); // "door_radio" is the identifier you will use in following methods

byte addresses[][6] = {"pipe1"}; // Create address for 1 pipe.

void setup() {
  pinMode(green_LED, OUTPUT);
  pinMode(red_LED, OUTPUT);
  pinMode(yellow_LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT);
  pinMode(hall, INPUT);
  pinMode(motion, INPUT);

  Serial.begin(115200); //start serial communications at 115200 baud
  SPI.begin(); //start the SPI bus
  mfrc522.PCD_Init(); //start the MFRC522 rfid sensor
  servo.attach(motor);
  servo.write(lock); //reset door to LOCKED
  delay(1000); //give the servo 1 second to lock the door
  servo.detach(); //allow lock to be opened by key
  Serial.println("RFID ready. Door is locked.");

  door_radio.begin();  // Start up the physical nRF24L01 Radio
  door_radio.setChannel(108);  // Above most Wifi Channels
  door_radio.setPALevel(RF24_PA_MIN);
  door_radio.openWritingPipe(addresses[0]); // Use the first entry in array 'addresses' (Only 1 right now)
  door_radio.write( &door_status, sizeof(door_status) );
  Serial.println("Transmitter ready.");

}

void loop() {
  /***************BUTTON***************/
  button_status = digitalRead(button);

  if (button_status == 1) {
    door_status = 2;
    door_radio.write( &door_status, sizeof(door_status) );
    Serial.print("Door status: "); Serial.println(door_status);
    Serial.print("Button status: "); Serial.println(button_status);
    servo.attach(motor);
    servo.write(unlock);
    digitalWrite(green_LED, HIGH);
    tone(buzzer, 2000, 300);
    delay(5000);
    servo.detach();
    digitalWrite(green_LED, LOW);
  }

  //***************MOTION***************/
  //pid = digitalRead(motion);  //0 = no motion, 1 = motion detected
  //if (pid == 1 && door_status == 0) { //
  //    if (pid == 1) {
  //    Serial.println("Motion detected! Unlock the door!");
  //    servo.attach(motor);
  //    servo.write(unlock);
  //    digitalWrite(yellow_LED, HIGH);
  //    tone(buzzer,2000,300);
  //    door_status = 1;
  //    delay(5000); //5 seconds to open the door after someone is detected near the door
  //    digitalWrite(yellow_LED, LOW);
  //    servo.detach();
  //    }
  //}

  /***************HALL***************/
  mag = digitalRead(hall); //0 = magnet detected, 1 = magnet not detected

  if (mag == 0 && door_status == 2) { //if the magnet is detected and the door is unlocked
    digitalWrite(yellow_LED, HIGH);
    Serial.println("Magnet detected. Door is locked!");
    tone(buzzer, 500, 300);
    servo.attach(motor); //send a PWM signal to the motor
    servo.write(lock); //turn clockwise 80 degrees (LOCK the door)

    door_status = 1;
    door_radio.write( &door_status, sizeof(door_status) );
    Serial.print("Door status: "); Serial.println(door_status);

    delay(1000);
    digitalWrite(yellow_LED, LOW);
    servo.detach();
  }

  /***************RFID***************/
  if (mfrc522.PICC_IsNewCardPresent()) {
    mfrc522.PICC_ReadCardSerial();

    //store each byte (4 bytes in total) of scanned key into the array
    for (byte i = 0; i < 4; i++) {
      uid[i] = mfrc522.uid.uidByte[i];
      Serial.print(uid[i]); //print in decimal
      //Serial.print(String(uid[i], HEX)); //print in hex
      Serial.print(" ");
    }
    Serial.println();

    //keychain A1 id = 48 196 199 126
    //keychain A1 hex id = 30 c4 c7 7e

    //key matches
    if (uid[0] == 48 && uid[1] == 196 && uid[2] == 199 && uid[3] == 126) {
      Serial.println("Match. Unlock the door!");
      servo.attach(motor);
      servo.write(unlock); //turn counterclockwise 100 degrees (UNLOCK the door)
      digitalWrite(green_LED, HIGH);
      tone(buzzer, 2000, 300);
      door_status = 2;
      door_radio.write( &door_status, sizeof(door_status) );
      Serial.print("Door status: "); Serial.println(door_status);

      delay(1000);
      servo.detach();
      digitalWrite(green_LED, LOW);
      delay(7000); //8 seconds to open the door after an authorized key is presented
    }
    //key doesn't match
    else {
      Serial.println("No match!");
      digitalWrite(red_LED, HIGH);
      tone(buzzer, 500, 300);
      delay(1000);
      digitalWrite(red_LED, LOW);
    }
  }

  /***************END***************/
}

