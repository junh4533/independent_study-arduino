# Entry 7: Connecting to the wifi

## Esp8266
The esp8266 wifi module is a low cost device that enable internet capability.
For Arduino, it offers a wide range of wireless projects. It can get 
information from websites through apis, send information to a server, and connect to other devices wirelessly. Unlike other gadgets, it uses AT(tention) commands sent through serial communication. 

### Pin layout
There are a total of 8 pins on the esp8266. For my project, I only need to use 6 pins.
<img src="../images/esp8266_pin_layout.png">

1. RX > Arduino RX
2. TX > Arduino TX
3. VCC > 3.3v
4. GND > GND
5. CH_PD > 3.3v
6. GPIO > GND (only for flashing)

### AT commands
Here are some of the basic AT commands that I will use for my project.
```cpp
AT //check if the module is functional
AT+GMR //view firmware version
AT+RST //reset
AT+CWMODE=MODE //wifi mode
AT+CWLAP //list all the wifis
AT+CWJAP="Wifi SSID","password" //join the wifi
AT+CIFSR //return ip address
```
To ensure that the module is functional, type `AT` into the console. If nothing is returned, 
try a different baud rate. Each esp8266 module might run on different baud rates. For mine,
it runs on 115200 baud (bits per second).

### Flashing the esp8266

Before connecting to the wifi, it is recommended to have the latest firmware. Check the version
using `AT+GMR`. Then, download the firmware flasher (https://drive.google.com/file/d/0B3dUKfqzZnlwVGc1YnFyUjgxelE/view) along with the latest **.bin** file.
Right before flashing, connect the GPIO pin on the esp8266 to ground so that it knows it is in flashing mode.
Once the flashing is complete, remove the ground wire connected to GPIO. The esp8266 will now **operate at 9600 baud instead of 115200**.

### Connecting to the wifi

Another optional step is `AT+RST`, but it's best to run it to erase any existing code flashed to the module.

Before searching for nearby wifis, the mode must be configured. Setting `AT+CWMODE=3` enables
both the router and access point modes. Basically, the module can receive and send information.

Upon running `AT+CWLAP`, which lists all the nearby wifis, something like this should be returned.

```cpp
AT+CWLAP

+CWLAP:(4,"belkin.5a4",-71,"08:86:3b:a0:d5:a4",1,3,0)
+CWLAP:(4,"u15814",-66,"50:c7:bf:53:28:a0",1,16,0)
+CWLAP:(3,"DIRECT-2B-HP Officejet 5740",-92,"50:65:f3:6f:47:2d",1,6,0)
+CWLAP:(3,"J&T",-91,"14:cf:e2:a8:1d:50",1,-9,0)
+CWLAP:(3,"TP-LINK_A8B5F4",-84,"64:70:02:a8:b5:f4",6,20,0)
+CWLAP:(3,"DG1670A92",-84,"00:ac:e0:a1:34:90",6,-36,0)
+CWLAP:(4,"linksys",-85,"00:25:9c:b7:04:62",6,11,0)
+CWLAP:(3,"TFTH9",-71,"f8:e4:fb:50:af:20",6,-7,0)
+CWLAP:(3,"tptom",-77,"50:c7:bf:1d:c9:1e",3,11,0)
+CWLAP:(4,"NETGEAR22",-46,"00:22:3f:9d:38:0e",6,-31,0)
+CWLAP:(3,"G8R52",-82,"18:1b:eb:01:63:89",6,20,0)
+CWLAP:(4,"Tenda_578E50",-78,"c8:3a:35:57:8e:51",7,6,0)
+CWLAP:(4,"Xu",-81,"78:54:2e:a2:1e:34",8,-2,0)
+CWLAP:(4,"SELINA",-84,"34:08:04:13:23:da",6,-21,0)
+CWLAP:(3,"CellSpot_2.4GHz_BE60",-42,"1c:b7:2c:c5:be:60",10,-7,0)
+CWLAP:(3,"DIRECT-lXMFC-L5850DW_BRcd54",-68,"4a:e2:44:45:cd:54",10,21,0)
+CWLAP:(3,"NETGEAR-D1-2G",-65,"6c:b0:ce:a0:08:12",10,25,0)
+CWLAP:(4,"belkin.e20",-79,"08:86:3b:b0:1e:20",11,6,0)
+CWLAP:(4,"JW",-85,"c0:c1:c0:d3:0d:6d",11,11,0)
+CWLAP:(0,"JW-guest",-85,"c0:c1:c0:d3:0d:6e",11,11,0)
+CWLAP:(0,"SETUP",-89,"40:b8:9a:3a:e2:a7",11,16,0)
+CWLAP:(3,"Cat",-88,"00:14:6c:79:61:34",11,20,0)
```

Once the wifi is listed, you can join the network using `AT+CWJAP="Wifi SSID","password"`.
Since my home wifi is **NETGEAR22**, I will type `AT+CWJAP="NETGEAR22","password"` into 
the console. 

To make sure that it successfully connected, type `AT+CIFSR` to see the ip address of the connection.
```
AT+CIFSR

+CIFSR:APIP,"192.168.4.1"
+CIFSR:APMAC,"5e:cf:7f:f6:93:47"
+CIFSR:STAIP,"10.0.0.91"
+CIFSR:STAMAC,"5c:cf:7f:f6:93:47"
```
Once the code is flashed into the module, it will automatically connect to the wifi the next time it is turned on. This is a huge discovery for me because that means I don't have to send commands to the console each time i upload a sketch. 
## Takeaways

#### Use your time more efficiently
This might not apply for everyone, but I do not have access to a lot of the Arduino features at school.
For the esp8266 and the wireless transceiver, I needed wifi access, separate Arduino instances, and certain libraries installed through the boards manager. I can only do them at home, so I decided to spend class time blogging instead of coding. 

#### Every guide is useful in its own way
I followed multiple guides to connect to the wifi. The first guide I looked at wasn't even for Arduino. 
I did learn, however, learn that the esp8266 operates at 3.3v, but the Arduino pins functions at 5v. From another guide, I learned that the TX and RX pins are 5v tolerant, meaning that I can connect them directly to the Arduino TX/RX pins. Combining both pieces of knowledge, I was able to hook up the wifi module. 

## Sources
http://www.instructables.com/id/ESP8266-Wi-fi-module-explain-and-connection/

https://forum.arduino.cc/index.php?topic=283043.0

http://www.instructables.com/id/Internet-of-Things-Arduino/

http://iot-playground.com/blog/2-uncategorised/17-esp8266-wifi-module-and-5v-arduino-connection

https://learn.sparkfun.com/tutorials/voltage-dividers

http://www.elec-cafe.com/esp8266-esp-01-firmware-update/

https://drive.google.com/file/d/0B3dUKfqzZnlwVGc1YnFyUjgxelE/view

