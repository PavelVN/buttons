#ifndef BUTTONS_PVN_H
#define BUTTONS_PVN_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

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
  bool loop(const uint32_t millisLoop);
  bool isShortPress(uint8_t buttonNum);
  bool isLongPress(uint8_t buttonNum);
  uint8_t reset();
};

buttons_pvn::buttons_pvn()
{
}

void buttons_pvn::init(const uint8_t buttonsCount, const uint8_t *PIN)
{
  this->buttonsCount = buttonsCount;
  this->PIN = (uint8_t *)malloc(buttonsCount);
  state = (bool *)malloc(buttonsCount);
  onState = (bool *)malloc(buttonsCount);
  time = (uint16_t *)malloc(2 * buttonsCount);
  shortPress = (bool *)malloc(buttonsCount);
  longPress = (bool *)malloc(buttonsCount);
  if (PIN == nullptr || state == nullptr || onState == nullptr || time == nullptr || shortPress == nullptr || longPress == nullptr)
  {
    Serial.println("Error memory");
    while(1)
    {
      delay(1000);
      Serial.print(".");
    }

  }
  
  for (uint8_t i = 0; i < buttonsCount; i++)
  {
    this->PIN[i] = PIN[i];
    pinMode(PIN[i], INPUT);
    state[i] = 0;
    onState[i] = 1;
    time[i] = 0;
    longPress[i] = 0;
    shortPress[i] = 0;
  }
  READ_DELAY = 50;
  longPressCount = 15;
}

buttons_pvn::~buttons_pvn()
{
  free(state);
  free(onState);
  free(time);
  free(PIN);
  free(longPress);
  free(shortPress);
  buttonsCount = 0;
}

bool buttons_pvn::loop(const uint32_t millisLoop)
{
  if (millisLoop - lastRunMillis > READ_DELAY)
  {
    lastRunMillis = millisLoop;
    bool ans;
    ans = false;
    for (uint8_t i = 0; i < buttonsCount; i++)
    {
      state[i] = digitalRead(PIN[i]);
    }
    for (uint8_t i = 0; i < buttonsCount; i++)
    {
      if (state[i] == onState[i])
      {
        time[i]++;
        if (time[i] == longPressCount)
        {
          longPress[i] = true;
          ans = true;
        }
      }
      else if (time[i] > 0)
      {
        if (time[i] > 1  && time[i] < longPressCount)
        {
          shortPress[i] = true;
          ans = true;
        }
        time[i] = 0;
      }
    }
    return ans;
  }
  return false;
}

bool buttons_pvn::isShortPress(uint8_t buttonNum)
{
  bool ans{false};
  if (buttonNum < buttonsCount)
  {
    ans = shortPress[buttonNum];
    shortPress[buttonNum] = 0;
  }
  return ans;
}

bool buttons_pvn::isLongPress(uint8_t buttonNum)
{
  bool ans{false};
  ans = false;
  if (buttonNum < buttonsCount)
  {
    ans = longPress[buttonNum];
    longPress[buttonNum] = 0;
  }
  return ans;
}

uint8_t buttons_pvn::reset()
{
  uint8_t ans;
  ans = 0;
  Serial.println("Short: " + String(shortPress[0]) + String(shortPress[1]) + String(shortPress[2]) + String(shortPress[3]));
  Serial.println("Long: " + String(longPress[0]) + String(longPress[1]) + String(longPress[2]) + String(longPress[3]));
  for (uint8_t i = 0; i < buttonsCount; i++)
  {
    if (shortPress[i])
    {
      shortPress[i] = 0;
      ans++;
    }
    if (longPress[i])
    {
      longPress[i] = 0;
      ans++;
    }
  }
  return ans;
}

#endif