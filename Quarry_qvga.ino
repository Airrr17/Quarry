// Version jun.2020
#include <TFT_eSPI.h>  ///// MOD User_Setup.h: TFT_CS - PIN_D1; TFT_DC - PIN_D4; TFT_RST - -1; #define SUPPORT_TRANSACTIONS !!!!!!!!!!!; NODEmcu 1.0 (ESP12E)
#include <SPI.h>
#include "SdFat.h"
#include "tiles.h"
TFT_eSPI tft = TFT_eSPI();
SdFat sd;
File file;

unsigned int level[30][40];
int x, y, player_x, player_y, lev = 0, total_crys = 0, got_crys = 0, player_present = 0, exit_present = 0;
int analog, ax, ay;
char levelName[8];
bool padenie = true, gad = false, sdini = false;
long key_to;

#define sprite_crystal    crys3        // 0-4;3 choose your tiles from tiles.h
#define sprite_empty      empty
#define sprite_wall       wall4        // 0-4;4
#define sprite_rock       rock1        // 0-4;1 or whatever you add
#define sprite_vata       vata1        // 0-4;1
#define sprite_critter    critter5     // 0-7;0 player vrode
#define sprite_exit       exit1        // 0-2;1
#define sprite_gad        critter3     // 0-7;3 gad

#define delay_button      100          // set your speeds
#define delay_loop        35           // fall speed
#define delay_scan        5            // microsec.

#define UPmin             141          // Control pad analogin A0 requires five buttons and six resistors serial circuit.
#define UPmax             144          // Adjust those analogRead values:
#define DOWNmin           14
#define DOWNmax           16
#define LEFTmin           267          // right+left - 326-327, up+down - 157-158, up+fire - 170, down+fire - 49, right+fire - 126, left+fire - 289
#define LEFTmax           270
#define RIGHTmin          96
#define RIGHTmax          98
#define FIREmin           31
#define FIREmax           32


void setup() {
  delay(500);
  tft.init();
  tft.setTextSize(1);
  tft.setRotation(3);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  Serial.begin(115200);
  Serial.println(F("(c)Airrr(r) presents"));
  while (!sdini) {
    if (!sd.begin(D2)) {
      tft.setCursor(37, 15), tft.println(F("SDcard init. failed! (Re)insert card now.")), delay (333);
    }
    else {
      sdini = 1;
    }
  }
  tft.fillScreen(TFT_BLACK);
  startMenu();
  openLevel();
}


void loop() {           //////////////////////////////////////////////////////////////////  LOOP  ///////////////////////////////////////////////////////////////////////
  analog = analogRead(A0);
  if ((analog != 0) and ((millis() - key_to) > delay_button)) {

    if (analog >= UPmin and analog <= UPmax) { ////////// UP start
      if ((level[player_x - 1][player_y] == 3) or (level[player_x - 1][player_y] == 8)) {
        level[player_x][player_y] = 8, level[player_x - 1][player_y] = 5, tft.pushImage(player_y * 8, player_x * 8, 8, 8, sprite_empty), tft.pushImage(player_y * 8, (player_x - 1) * 8, 8, 8, sprite_critter), player_x--;
        key_to = millis();
        goto padenie;
      }
      if (level[player_x - 1][player_y] == 4) {
        level[player_x][player_y] = 8, level[player_x - 1][player_y] = 5, tft.pushImage(player_y * 8, player_x * 8, 8, 8, sprite_empty), tft.pushImage(player_y * 8, (player_x - 1) * 8, 8, 8, sprite_critter), player_x--, got_crys++;
        key_to = millis();
        goto padenie;
      }

      if ((level[player_x - 1][player_y] == 6) and (got_crys == total_crys)) lev++, openLevel();

    }//////////////////////////////////////////////// UP end

    if (analog >= DOWNmin and analog <= DOWNmax) {   ////////// DOWN start
      if ((level[player_x + 1][player_y] == 3) or (level[player_x + 1][player_y] == 8)) {
        level[player_x][player_y] = 8, level[player_x + 1][player_y] = 5, tft.pushImage(player_y * 8, player_x * 8, 8, 8, sprite_empty), tft.pushImage(player_y * 8, (player_x + 1) * 8, 8, 8, sprite_critter), player_x++;
        key_to = millis();
        goto padenie;
      }
      if (level[player_x + 1][player_y] == 4) {
        level[player_x][player_y] = 8, level[player_x + 1][player_y] = 5, tft.pushImage(player_y * 8, player_x * 8, 8, 8, sprite_empty), tft.pushImage(player_y * 8, (player_x + 1) * 8, 8, 8, sprite_critter), player_x++, got_crys++;
        key_to = millis();
        goto padenie;
      }
      if ((level[player_x + 1][player_y] == 6) and (got_crys == total_crys)) lev++, openLevel();

    }//////////////////////////////////////////////// DOWN end

    if (analog >= LEFTmin and analog <= LEFTmax) { ////////// LEFT start
      if ((level[player_x][player_y - 1] == 3) or (level[player_x][player_y - 1] == 8)) {
        level[player_x][player_y] = 8, level[player_x][player_y - 1] = 5, tft.pushImage(player_y * 8, player_x * 8, 8, 8, sprite_empty), tft.pushImage((player_y - 1) * 8, player_x * 8, 8, 8, sprite_critter), player_y--;
        key_to = millis();
        goto padenie;
      }
      if (level[player_x][player_y - 1] == 4) {
        level[player_x][player_y] = 8, level[player_x][player_y - 1] = 5, tft.pushImage(player_y * 8, player_x * 8, 8, 8, sprite_empty), tft.pushImage((player_y - 1) * 8, (player_x) * 8, 8, 8, sprite_critter), player_y--, got_crys++;
        key_to = millis();
        goto padenie;
      }
      if ((level[player_x][player_y - 1] == 6) and (got_crys == total_crys)) lev++, openLevel();

      if ((level[player_x][player_y - 1] == 2) and (level[player_x][player_y - 2] == 8)) {   ////////////////// dvigaem rock left //////////////////
        level[player_x][player_y] = 8, level[player_x][player_y - 1] = 5, level[player_x][player_y - 2] = 2, tft.pushImage(player_y * 8, player_x * 8, 8, 8, sprite_empty), tft.pushImage((player_y - 1) * 8, player_x * 8, 8, 8, sprite_critter), tft.pushImage((player_y - 2) * 8, player_x * 8, 8, 8, sprite_rock), player_y--;
        key_to = millis();
        goto padenie;
      }

    }//////////////////////////////////////////////// LEFT end

    if (analog >= RIGHTmin and analog <= RIGHTmax) {   ////////// RIGHT start
      if ((level[player_x][player_y + 1] == 3) or (level[player_x][player_y + 1] == 8)) {
        level[player_x][player_y] = 8, level[player_x][player_y + 1] = 5, tft.pushImage(player_y * 8, player_x * 8, 8, 8, sprite_empty), tft.pushImage((player_y + 1) * 8, player_x * 8, 8, 8, sprite_critter), player_y++;
        key_to = millis();
        goto padenie;
      }
      if (level[player_x][player_y + 1] == 4) {
        level[player_x][player_y] = 8, level[player_x][player_y + 1] = 5, tft.pushImage(player_y * 8, player_x * 8, 8, 8, sprite_empty), tft.pushImage((player_y + 1) * 8, (player_x) * 8, 8, 8, sprite_critter), player_y++, got_crys++;
        key_to = millis();
        goto padenie;
      }
      if ((level[player_x][player_y + 1] == 6) and (got_crys == total_crys)) lev++, openLevel();

      if ((level[player_x][player_y + 1] == 2) and (level[player_x][player_y + 2] == 8)) {   ////////////////// dvigaem rock right //////////////////
        level[player_x][player_y] = 8, level[player_x][player_y + 1] = 5, level[player_x][player_y + 2] = 2, tft.pushImage(player_y * 8, player_x * 8, 8, 8, sprite_empty), tft.pushImage((player_y + 1) * 8, player_x * 8, 8, 8, sprite_critter), tft.pushImage((player_y + 2) * 8, player_x * 8, 8, 8, sprite_rock), player_y++;
        key_to = millis();
        goto padenie;
      }

    }//////////////////////////////////////////////// RIGHT end

    if (analog >= FIREmin and analog <= FIREmax) {   ////////// Fire
      mainMenu();
    }
  }

  for (x = 30; x > 0; x--) {
    for (y = 42; y > 0; y--) {                                                    //////////////// miganie kristalov
      if (level[x][y] == 4) {
        if (random(10) == 1) tft.setSwapBytes(false), tft.pushImage(y * 8, x * 8, 8, 8, sprite_crystal), tft.setSwapBytes(true);   // chance to blink
        if (random(10) > 7) tft.setSwapBytes(true), tft.pushImage(y * 8, x * 8, 8, 8, sprite_crystal);                             // chance to back normal
      }
    }
  }

padenie:
  ////////////////////////////////// padenie //////////////////////////////////
  padenie = true;
  for (x = 30; x > 0; x--) {
    for (y = 42; y > 0; y--) {                                                    // 42 - two more bytes for 10 and 13 (cr lf) of each lineend

      delayMicroseconds(delay_scan + x);

      ///// snachala tupo POD:
      if ((level[x][y] == 2) and (level[x + 1][y] == 8)) level[x][y] = 8, level[x + 1][y] = 2, tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty), tft.pushImage(y * 8, (x + 1) * 8, 8, 8, sprite_rock);      //if found rock and empty beneath. from bottom left
      if ((level[x][y] == 4) and (level[x + 1][y] == 8)) level[x][y] = 8, level[x + 1][y] = 4, tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty), tft.pushImage(y * 8, (x + 1) * 8, 8, 8, sprite_crystal);   //if found crystal and empty beneath. from bottom left


      if (random(2) == 1) goto dalee;     //------------------///////// smotrya na kakuu storonu padat' snachala

      if ((level[x][y] == 2) and (level[x + 1][y + 1] == 8) and (level[x][y + 1] == 8) and ((level[x + 1][y] == 2) or (level[x + 1][y] == 4))) {
        level[x][y] = 8, level[x][y + 1] = 2, tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty), tft.pushImage((y + 1) * 8, x * 8, 8, 8, sprite_rock);   //if found rock and fall right
        goto vseSovsem;
      }
      if ((level[x][y] == 4) and (level[x + 1][y + 1] == 8) and (level[x][y + 1] == 8) and ((level[x + 1][y] == 2) or (level[x + 1][y] == 4))) {
        level[x][y] = 8, level[x][y + 1] = 4, tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty), tft.pushImage((y + 1) * 8, x * 8, 8, 8, sprite_crystal); //if found crystal and fall right
        goto vseSovsem;
      }
      if ((level[x][y] == 2) and (level[x + 1][y - 1] == 8) and (level[x][y - 1] == 8) and ((level[x + 1][y] == 2) or (level[x + 1][y] == 4))) {
        level[x][y] = 8, level[x][y - 1] = 2, tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty), tft.pushImage((y - 1) * 8, x * 8, 8, 8, sprite_rock);    //if found rock and fall left
        goto vseSovsem;
      }
      if ((level[x][y] == 4) and (level[x + 1][y - 1] == 8) and (level[x][y - 1] == 8) and ((level[x + 1][y] == 2) or (level[x + 1][y] == 4))) {
        level[x][y] = 8, level[x][y - 1] = 4, tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty), tft.pushImage((y - 1) * 8, x * 8, 8, 8, sprite_crystal); //if found crystal and fall left
        goto vseSovsem;
      }
dalee:
      if ((level[x][y] == 2) and (level[x + 1][y - 1] == 8) and (level[x][y - 1] == 8) and ((level[x + 1][y] == 2) or (level[x + 1][y] == 4))) {
        level[x][y] = 8, level[x][y - 1] = 2, tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty), tft.pushImage((y - 1) * 8, x * 8, 8, 8, sprite_rock);    //if found rock and fall left
        goto vseSovsem;
      }
      if ((level[x][y] == 4) and (level[x + 1][y - 1] == 8) and (level[x][y - 1] == 8) and ((level[x + 1][y] == 2) or (level[x + 1][y] == 4))) {
        level[x][y] = 8, level[x][y - 1] = 4, tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty), tft.pushImage((y - 1) * 8, x * 8, 8, 8, sprite_crystal); //if found crystal and fall left
        goto vseSovsem;
      }
      if ((level[x][y] == 2) and (level[x + 1][y + 1] == 8) and (level[x][y + 1] == 8) and ((level[x + 1][y] == 2) or (level[x + 1][y] == 4))) {
        level[x][y] = 8, level[x][y + 1] = 2, tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty), tft.pushImage((y + 1) * 8, x * 8, 8, 8, sprite_rock);   //if found rock and fall right
        goto vseSovsem;
      }
      if ((level[x][y] == 4) and (level[x + 1][y + 1] == 8) and (level[x][y + 1] == 8) and ((level[x + 1][y] == 2) or (level[x + 1][y] == 4))) {
        level[x][y] = 8, level[x][y + 1] = 4, tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty), tft.pushImage((y + 1) * 8, x * 8, 8, 8, sprite_crystal); //if found crystal and fall right
        goto vseSovsem;
      }


    }
    padenie = false;
  }

  ////////////////////////////////// padenie //////////////////////////////////

vse:

  delay(delay_loop);

vseSovsem:
  if ((random(2) == 1) and (gad == true)) moveGad();
}                      //////////////////////////////////////////////////////////////////  LOOP  ///////////////////////////////////////////////////////////////////////


void openLevel() {
  if (lev > 99) lev = 0;
  if (lev < 0) lev = 0;
  snprintf (levelName, sizeof(levelName), "level%02d", lev);
  gad = false;

  if (!sd.exists(levelName)) tft.fillScreen(TFT_BLACK), tft.setCursor(90, 15), tft.print(levelName), tft.println(F(" file not found")), delay(1500), lev++, openLevel();
  file = sd.open(levelName, O_READ);

  if (file) {   //file
    if (file.fileSize() != 1260) tft.fillScreen(TFT_BLACK), tft.setCursor(85, 15), tft.print(levelName), tft.println(F(" filesize (1260b) mismatch")), delay(10000), lev++, openLevel();
    total_crys = 0, got_crys = 0, exit_present = 0, player_present = 0;
    for (x = 0; x < 30; x++)  {
      for (y = 0; y < 42; y++) {                                                    // 42 - two more bytes for 10 and 13 (cr lf) of each lineend
        int readd = file.read();
        if (readd == 56) level[x][y] = 8;
        if (readd == 49) level[x][y] = 1;
        if (readd == 50) level[x][y] = 2;
        if (readd == 51) level[x][y] = 3;
        if (readd == 52) level[x][y] = 4, total_crys++;
        if (readd == 53) level[x][y] = 5, player_x = x, player_y = y, player_present++;
        if (readd == 54) level[x][y] = 6, exit_present++;
      }
    }
    file.close();
  }

  if (player_present != 1) tft.fillScreen(TFT_BLACK), tft.setCursor(50, 15), tft.print(F("Player start position error in ")), tft.print(levelName), delay(10000), lev++, openLevel();
  if (exit_present != 1) tft.fillScreen(TFT_BLACK), tft.setCursor(50, 15), tft.print(F("Error exit door from ")), tft.print(levelName), delay(10000), lev++, openLevel();

  for (int po = 0; po < 25; po++) {                                                 // popitok postavit' gada
    ax = random (0, 29), ay = random (0, 39);

    if (level[ax][ay] == 8) {                                                       // norm mesto spawna
      level[ax][ay] = 7, gad = true;
      goto vse;
    }
  }
vse:

  printLevel();
}

void printLevel() {
  for (x = 0; x < 30; x++)  {
    for (y = 0; y < 42; y++) {                                                    // 42 - two more bytes for 10 and 13 (cr lf) of each lineend
      if (level[x][y] == 8) tft.pushImage(y * 8, x * 8, 8, 8, sprite_empty);
      if (level[x][y] == 1) tft.pushImage(y * 8, x * 8, 8, 8, sprite_wall);
      if (level[x][y] == 2) tft.pushImage(y * 8, x * 8, 8, 8, sprite_rock);
      if (level[x][y] == 3) tft.pushImage(y * 8, x * 8, 8, 8, sprite_vata);
      if (level[x][y] == 4) tft.pushImage(y * 8, x * 8, 8, 8, sprite_crystal);
      if (level[x][y] == 5) tft.pushImage(y * 8, x * 8, 8, 8, sprite_critter);
      if (level[x][y] == 6) tft.pushImage(y * 8, x * 8, 8, 8, sprite_exit);
      if (level[x][y] == 7) tft.pushImage(y * 8, x * 8, 8, 8, sprite_gad);

    }
  }
}


void mainMenu() {
  analog = 0;
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(95, 4), tft.println(F("(c)Airrr(r) presents"));
  tft.setCursor(274, 4), tft.println(F("27jun20"));
  tft.drawLine (2, 13, 317, 13, TFT_YELLOW);
  tft.setCursor(3, 16), tft.println(F("Main menu:"));
  tft.setCursor(2, 32), tft.println(F("Info:"));
  tft.setCursor(13, 42), tft.print(F("Level #: ")), tft.printf ("%03d", (lev + 1));                                                                       // +1 dlya krasoti tolko   !!!!
  tft.pushImage(13, 52, 8, 8, sprite_crystal), tft.setCursor(23, 52), tft.print(F(" - Crystals left: ")), tft.printf("%04d", (total_crys - got_crys));
  if ((total_crys - got_crys) <= 0) tft.setCursor(155, 52), tft.print(F(", you'r done, proceed exit!"));
  tft.pushImage(13, 62, 8, 8, sprite_gad), tft.setCursor(23, 62), tft.print(F(" - will try to block you "));

  tft.setCursor(3, 82), tft.println(F("*debug:"));
  tft.setCursor(13, 92), tft.println(F("UP - load next level"));
  tft.setCursor(13, 102), tft.println(F("DOWN for next level"));
  tft.setCursor(13, 112), tft.println(F("LEFT - restart level"));
  tft.setCursor(13, 122), tft.println(F("RIGHT - first level"));
  tft.setCursor(13, 132), tft.println(F("FIRE - back to level"));

  tft.setCursor(3, 208), tft.println(F("Get all crystals without being blocked by rocks"));
  tft.setCursor(3, 218), tft.println(F("                             and enter the door."));
  tft.setCursor(3, 228), tft.println(F("You can move a single rock aside if nothing behind."));

  delay (250);

  while (analog == 0) {
    analog = analogRead(A0);
    delay(75);
    tft.drawRect(0, 0, 319, 239, (random(0, 10)) * 6000), tft.drawRect(1, 1, 317, 237, (random(0, 10)) * 6000);
  }
  if (analog >= UPmin and analog <= UPmax) lev++, openLevel();                 /////////////////////////////////////UP to reload level, bebug only
  if (analog >= DOWNmin and analog <= DOWNmax) lev--, openLevel();                   /////////////////////////////////////DOWN to load next level, bebug only
  if (analog >= LEFTmin and analog <= LEFTmax) openLevel();                        /////////////////////////////////////LEFT
  if (analog >= RIGHTmin and analog <= RIGHTmax) lev = 0, openLevel();                 /////////////////////////////////////RIGHT

  printLevel();
}


void startMenu () {
  int xm = 100, ym = 40, nm = 0, xm2 = 17, ym2 = 17;                            // start puti
  bool lm2 = true, im2 = true;

  do {
    if (nm <= 100) xm++;
    if ((nm > 100) and (nm <= 150)) ym++;
    if ((nm > 150) and (nm <= 250)) xm--;
    if ((nm > 250) and (nm <= 300)) ym--;

    if (xm2 > 312) lm2 = false;
    if (xm2 < 1 ) lm2 = true;
    if ((xm2 > 20) and (xm2 < 292) and (random(333) == 0)) lm2 = !lm2;
    if (ym2 > 232) im2 = false;
    if (ym2 < 1) im2 = true;
    if ((ym2 > 20) and (ym2 < 212) and (random(333) == 0)) im2 = !im2;
    if (lm2 == true) xm2++;
    if (lm2 == false) xm2--;
    if (im2 == true) ym2++;
    if (im2 == false) ym2--;

    tft.pushImage(xm, ym, 8, 8, sprite_gad);
    tft.pushImage(xm2, ym2, 8, 8, sprite_critter);
    tft.setCursor(137, 65), tft.println(F("QUARRY"));

    if (nm >= 300) xm = 100, ym = 40, nm = 0;
    delay (10);

    tft.pushImage(xm, ym, 8, 8, sprite_empty);
    tft.pushImage(xm2, ym2, 8, 8, sprite_empty);

    //tft.drawRect(0, 0, 319, 239, (random(0, 10)) * 6000), tft.drawRect(1, 1, 317, 237, (random(0, 10)) * 6000);

    nm++;
    tft.pushImage(15, 145, 8, 8, exit0), tft.pushImage(30, 145, 8, 8, exit1), tft.pushImage(45, 145, 8, 8, exit2);
    tft.pushImage(15, 160, 8, 8, vata0), tft.pushImage(30, 160, 8, 8, vata1), tft.pushImage(45, 160, 8, 8, vata2), tft.pushImage(60, 160, 8, 8, vata3), tft.pushImage(75, 160, 8, 8, vata4);
    tft.pushImage(15, 175, 8, 8, rock0), tft.pushImage(30, 175, 8, 8, rock1), tft.pushImage(45, 175, 8, 8, rock2), tft.pushImage(60, 175, 8, 8, rock3), tft.pushImage(75, 175, 8, 8, rock4);
    tft.pushImage(15, 190, 8, 8, crys0), tft.pushImage(30, 190, 8, 8, crys1), tft.pushImage(45, 190, 8, 8, crys2), tft.pushImage(60, 190, 8, 8, crys3), tft.pushImage(75, 190, 8, 8, crys4);
    tft.pushImage(15, 205, 8, 8, wall0), tft.pushImage(30, 205, 8, 8, wall1), tft.pushImage(45, 205, 8, 8, wall2), tft.pushImage(60, 205, 8, 8, wall3), tft.pushImage(75, 205, 8, 8, wall4);
    tft.pushImage(15, 220, 8, 8, critter0), tft.pushImage(30, 220, 8, 8, critter1), tft.pushImage(45, 220, 8, 8, critter2), tft.pushImage(60, 220, 8, 8, critter3), tft.pushImage(75, 220, 8, 8, critter4), tft.pushImage(90, 220, 8, 8, critter5), tft.pushImage(105, 220, 8, 8, critter6), tft.pushImage(120, 220, 8, 8, critter7);
  }
  while (analogRead(A0) == 0);
  delay(50);
}

void moveGad() {
  int prob = random(9);
  if (abs(ax - player_x) + abs(ay - player_y) == 1) prob = random(75);                       // if blocked became lazy

  if ((prob == 5) and (level[ax + 1][ay] == 8) and ax < player_x) tft.pushImage(ay * 8, ax * 8, 8, 8, sprite_empty), tft.pushImage(ay * 8, (ax + 1) * 8, 8, 8, sprite_gad), level[ax][ay] = 8, level[ax + 1][ay] = 7, ax++;
  if ((prob == 6) and (level[ax - 1][ay] == 8) and ax > player_x) tft.pushImage(ay * 8, ax * 8, 8, 8, sprite_empty), tft.pushImage(ay * 8, (ax - 1) * 8, 8, 8, sprite_gad), level[ax][ay] = 8, level[ax - 1][ay] = 7, ax--;
  if ((prob == 7) and (level[ax][ay + 1] == 8) and ay < player_y) tft.pushImage(ay * 8, ax * 8, 8, 8, sprite_empty), tft.pushImage((ay + 1) * 8, ax * 8, 8, 8, sprite_gad), level[ax][ay] = 8, level[ax][ay + 1] = 7, ay++;
  if ((prob == 8) and (level[ax][ay - 1] == 8) and ay > player_y) tft.pushImage(ay * 8, ax * 8, 8, 8, sprite_empty), tft.pushImage((ay - 1) * 8, ax * 8, 8, 8, sprite_gad), level[ax][ay] = 8, level[ax][ay - 1] = 7, ay--;

  if ((prob == 1) and (level[ax + 1][ay] == 8)) tft.pushImage(ay * 8, ax * 8, 8, 8, sprite_empty), tft.pushImage(ay * 8, (ax + 1) * 8, 8, 8, sprite_gad), level[ax][ay] = 8, level[ax + 1][ay] = 7, ax++;
  if ((prob == 2) and (level[ax - 1][ay] == 8)) tft.pushImage(ay * 8, ax * 8, 8, 8, sprite_empty), tft.pushImage(ay * 8, (ax - 1) * 8, 8, 8, sprite_gad), level[ax][ay] = 8, level[ax - 1][ay] = 7, ax--;
  if ((prob == 3) and (level[ax][ay + 1] == 8)) tft.pushImage(ay * 8, ax * 8, 8, 8, sprite_empty), tft.pushImage((ay + 1) * 8, ax * 8, 8, 8, sprite_gad), level[ax][ay] = 8, level[ax][ay + 1] = 7, ay++;
  if ((prob == 4) and (level[ax][ay - 1] == 8)) tft.pushImage(ay * 8, ax * 8, 8, 8, sprite_empty), tft.pushImage((ay - 1) * 8, ax * 8, 8, 8, sprite_gad), level[ax][ay] = 8, level[ax][ay - 1] = 7, ay--;

}
