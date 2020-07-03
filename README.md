##     Quarry  
Logic puzzle game for arduino.  
Get all crystals and exit the door to next level.  
Wrong steps can lead to rockfall and blocking player unable to move.  
  
Setup:  
1. Edit "User_Setup.h" in TFT_eSPI library:  
   TFT_CS - PIN_D1; TFT_DC - PIN_D4; TFT_RST - -1;  
   define SUPPORT_TRANSACTIONS !!!!!!!!!!!  
2. Copy all files "levelXX" to the root of SD.  
   You can add or edit those levels.  
  
Hardware connections:  
ESP8266 and ili9341  
  
TFT and SD SPI:  
SCK        - D5  
MISO       - D6  
MOSI       - D7  
  
TFT reset  - RST  
TFT LED    - VCC  
TFT SS     - D1  
SD SS      - D2  
TFT DC     - D4  
  
Keypad     - A0  
  
  
![alt text](https://github.com/Airrr17/Quarry/blob/master/controller.png)  
  
Adjust your analog values at lines 30+  
  
  
## ToDo:  
  
- Load\Save  
- Sound  
  
![alt text](https://github.com/Airrr17/Quarry/blob/master/quarry01.jpg)  
  
  
First level is really easiest. Second is for fall demonstration.  
From forth and further didn't much tried :)  
  
But you can always edit those levels and add yours.  
With any text editor where:  
8 is empty space  
1 is wall  
2 is rock  
3 is send you can dig  
4 is crystal  
5 where you start, should be only one  
6 exit door, should be only one  
  
Just name it "levelXX" 01 to 99 save on SD and reset.  
*Open existing level files for understanding :)  
*You can't "die" in this game, but can be blocked with rocks leading restart.  
*Use "fire" button for menu like restart or next and prev. level.  
**YOU NEED to adjust analog buttons values in the code as all resistors are different.  
  
  video:  
  https://www.youtube.com/watch?v=wPVtNHVkfvM    
    