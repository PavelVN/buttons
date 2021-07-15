#include <buttons.pvn.h>

buttons_pvn::buttons_pvn()
{
}
#ifdef ESP32
void buttons_pvn::init(const uint8_t buttonsCount, const uint8_t *PIN, uint8_t esp32touch)
{
  init(buttonsCount, PIN);
  for (uint8_t i = 0; i < buttonsCount; i++)
  {
    this->esp32touch[i] = esp32touch;
  }
}
#endif
void buttons_pvn::init(const uint8_t buttonsCount, const uint8_t *PIN)
{
  
  this->buttonsCount = buttonsCount;
  this->PIN = (uint8_t *)malloc(buttonsCount);
  state = (bool *)malloc(buttonsCount);
  onState = (bool *)malloc(buttonsCount);
  time = (uint16_t *)malloc(2 * buttonsCount);
  shortPress = (bool *)malloc(buttonsCount);
  longPress = (bool *)malloc(buttonsCount);
#ifdef ESP32
  esp32touch = (uint8_t *)malloc(buttonsCount);
#endif
  if (PIN == nullptr || state == nullptr || onState == nullptr || time == nullptr || shortPress == nullptr || longPress == nullptr)
  {
    Serial.println("Error memory");
    while (1)
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
#ifdef ESP32
    esp32touch[i] = 0;
#endif
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

int buttons_pvn::loop(const uint32_t millisLoop)
{
  if (millisLoop - lastRunMillis > READ_DELAY)
  {
    lastRunMillis = millisLoop;
    int ans{0};
    for (uint8_t i = 0; i < buttonsCount; i++)
    {
#ifdef ESP32
      if (esp32touch[i])
      {
        if (touchRead(PIN[i]) < esp32touch[i])
        {
          state[i] = 1;
        }
        else
        {
          state[i] = 0;
        }
      }
      else
      {
        state[i] = digitalRead(PIN[i]);
      }
#else
      state[i] = digitalRead(PIN[i]);
#endif
    }
    for (uint8_t i = 0; i < buttonsCount; i++)
    {
      if (state[i] == onState[i])
      {
        time[i]++;
        if (time[i] == longPressCount)
        {
          longPress[i] = true;
          ++ans;
        }
      }
      else if (time[i] > 0)
      {
        if (time[i] > 1 && time[i] < longPressCount)
        {
          shortPress[i] = true;
          ++ans;
        }
        time[i] = 0;
      }
    }
    return ans;
  }
  return 0;
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
