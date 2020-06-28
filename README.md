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
  