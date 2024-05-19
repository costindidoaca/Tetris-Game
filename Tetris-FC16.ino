#include <MD_MAX72xx.h>
#include "Common.h"
#include "Tetrimino.h"
#include "DotMatrixBoard.h"
#include "KeypadReader.h"

// TODO Possible game improvements:
// - Reduce tick and factor for faster play at higher levels
// - Display score and Game Over message to user
// - Add sounds

unsigned long lastTs = 0;
unsigned long counter = 0;
int tick = 100;
int factor = 5;
DotMatrixBoard* dm;
KeypadReader kpr;

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));
  kpr.setPins(3, 4, 5, 6);
}

void loop()
{
  bool toPrint = false;

  unsigned long nowTs = millis();
  if (nowTs - lastTs < tick) return;
  lastTs = nowTs;
  counter++;
  #ifdef DEBUG
  Serial.print("Counter: "); Serial.println(counter);
  #endif

  if (dm == NULL) {
    dm = new DotMatrixBoard(32, 8);
    dm->setPins(11, 10, 13);
    #ifdef DEBUG
    dm->testPrint(true);
    delay(1000);
    dm->testPrint(false);
    #endif
  }

  String cmd = kpr.read();
  if (cmd.length() > 0) {
    #ifdef DEBUG
    Serial.print("Cmd: "); Serial.println(cmd);
    #endif
    dm->processCmd(cmd);
    toPrint = true;
  }

  if (counter % factor == 0) {
    #ifdef DEBUG
    Serial.println("Fall");
    #endif
    dm->updateFall();
    toPrint = true;
  }

  if (toPrint) dm->print();

  if (dm->isGameOver()) {
    Serial.println("Game Over!\n");
    delete dm;
    dm = NULL;
    while (true) { }
  }
}
