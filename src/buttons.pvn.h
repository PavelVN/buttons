#ifndef BUTTONS_PVN_H
#define BUTTONS_PVN_H

#include <Arduino.h>

class buttons_pvn
{
private:
  uint8_t *PIN;
  uint8_t buttonsCount;
  bool *state;
  bool *onState;
  bool *longPress;
  bool *shortPress;
  uint16_t *time;
  uint32_t READ_DELAY;
  uint16_t longPressCount;
  uint32_t lastRunMillis;

public:
  buttons_pvn();
  ~buttons_pvn();
  void init(const uint8_t buttonsCount, const uint8_t *PIN);
  int loop(const uint32_t millisLoop);
  bool isShortPress(uint8_t buttonNum);
  bool isLongPress(uint8_t buttonNum);
  uint8_t reset();
};

#endif