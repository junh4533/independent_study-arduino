# Entry 3: Planning my project

Like I said in my previous entry, a well thought out 
plan is fundamental to the success of a project.
For the first part of my smart home, I am thinking about using 
a servo motor to unlock the door when a valid rfid card is detected.

## Components
This is a list of the things I'll need to make the door lock.

Electrical components:
1. 1x Arduino Uno board
2. 1x Servo motor
3. 1x RFID sensor
4. 1x Power adapter 
5. Hall (Magnetic) sensor
 
Additional components:
1. Solder kit (for soldering pins onto RFID sensor)
2. Hot glue gun
3. Pieces of wood (to secure the parts onto the door)

Optional parts:
1. LCD screen 
2. 2x LEDs (1x red and 1x green)
3. Piezo buzzer (beeps when the door is unlocked)

*1x Transceivers (per Arduino board) are necessary for wireless capability

#### Servo motor
A wheel will be attached to the servo motor. When the motor turns, 
the wheel will turn the lock 45 degrees (clockwise to lock, counterclockwise to open). 

#### Power adapter
Since the arduino won't be connected to the computer via USB once it is installed on the door,
it will require another power source. A recommended power input for the Arduino Uno is 9v 1A.

#### Hall sensor
This will be placed on the door. On the wall adjacent to the door, there will be a piece of magnet. 
When the door closes, the magnet will be in range for the hall sensor to detect. 
This will notify the software that the door has been closed and the lock switch should be turned. 

#### Arduino board(s)
The original plan was to attach the arduino board and the rfid sensor to the back of my door.
When I place the rfid card on my door, the sensor will detect it and activate the motor.
In my previous entry, I mentioned that the door lock will have to be wireless. 
I recently discovered that I can route cables through the hole that holds my lock.
For the purpose of making the rfid lock, it is no longer necessary to have wireless features. 
Somewhere along the line, there might be a use for wireless capability.

#### Liquid crystal display (LCD) screen
There are 16 pins in total and 8 of them are data pins. The pins D4 to D7 are only necessary for 8-bit mode, 
which is more efficient than 4-bit. For general purpose, 4-bit mode is sufficient. 
<img src="../images/lcd.png"/>

** Pins 8-14 are data pins **

1. Ground for power
2. 5v for power
3. Display contrast control (regulated through voltage) **
4. Register select: controls where the data is being sent to (connect to PWM pin 13)
5. Read/write: (connect to ground)
6. Enable: controls where in the LCD to write data to (connect to PWM pin 12)
7. Only for 8-bit mode
8. Only for 8-bit mode
9. Only for 8-bit mode
10. Only for 8-bit mode
11. Connect to PWM pin 11
12. Connect to PWM pin 10
13. Connect to PWM pin 9
14. Connect to PWM pin 8
15. 5v for backlighting
16. Ground for backlighting

** This pin is usually connected to a potentiometer for voltage regulation.
The higher the voltage, the higher the contrast. 

#### Coding for the LCD

##### Initialization
First you must include the library with this line of code: `#include <LiquidCrystal.h>`
<br/>
<br/>
After the library is included, it has to be initialized.
As you have noticed, 6 of the 16 pins require a digital output. 
These are the pins that transfer data between the arduino board and the LCD.
In order to begin transferring data, you must first identify the pins used to connect to the LCD
using `LiquidCrystal lcd(pin4, pin6, pin11, pin12, pin13, pin14)`
Note that these pins numbers are referring the ones on the LCD screen. 
The number that actually goes into the function are the digital pins that are connected to those pins on the LCD.
The pins on the LCD can be connected to other PWM pins, but it must be connected in a descending order.
For example, if the pins 4 to 9 from the arduino board are used, then the code has to be like this: 
`LiquidCrystal lcd(pin9, pin8, pin7, pin6, pin5, pin4)`. 
<br/>
<br/>
Inside `void setup()`, define the size of the LCD using `lcd.begin(length, width)`.

##### Printing the message

The LCD is organized in a grid by using rows and columns.
Similar to arrays, both the rows and columns starts counting at 0.
By using `lcd.setCursor(column, row)`, 
you can set the starting position of the desired output.
In order to actually print the output, 
you need this line of code `lcd.print("message")`.

#### Identifying resistors
Every resistor has 4 colored stripes. I noticed that they all share the same colors, 
but the stripes has a different order. The gold strip, however, is always the first stripe. 
Additionally, the stripe that follows the gold stripe differs between each resistor. 
Below is a list of the stripes that **follow the gold stripe on each resistor**.

1. 200 ohm: Dark red
2. 1k ohm: Black
3. 10k: Orange
4. 1m: Green

## Takeaway

#### Always read the comments, especially when using examples
When I was testing a different LCD screen, I realized that it came with a I2C module.
This module allows the LCD screen to function with only 4 pins rather than 12 pins. 
However, this requires a different library. So I downloaded the library and tested out the example. 
I double checked the tutorial and nothing was working. 
It took me almost an hour to realize that the code was only compatible with Arduino IDE version 1.0.1. 
I could've saved myself that hour if I read the **first line of the comment**. 


 
 

