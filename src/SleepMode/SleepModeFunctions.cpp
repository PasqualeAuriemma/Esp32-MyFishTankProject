/*
  Aquarium Project Pasquale
*/

#include "SleepModeFunctions.h"

/*
If the ESP32 is asleep and you touch T3, the ESP will wake up
– the callback() function won’t be executed if you just press and release the touch pin;
If the ESP32 is awake and you touch T3, the callback function will be executed.
So, if you want to execute the callback() function when you wake up the ESP32,
you need to hold the touch on that pin for a while, until the function is executed.
In this case the callback() function is empty.
*/
void callback()
{
  // placeholder callback function
}

SleepLogic::SleepLogic(int timeToSleep, int threshold)
{
  setFactor(1000000);
  setTimeToSleep(timeToSleep); // 1800;
  setThreshold(threshold);     // 40
  // Setup interrupt on Touch Pad 3 (GPIO15)
  touchAttachInterrupt(T3, callback, Threshold);
  // Connfigure Touchpad as wakeup source
  esp_sleep_enable_touchpad_wakeup();
}

/*
  Going to sleep when the wake up time is expired, the counter will be reset each time a key is pressed.
  The function returns the seconds left to go to sleep. If the value is -1, it means that it isn't the time to go to sleep.
*/
int SleepLogic::sleepCountDown(int keyPad, byte noTouch, int sleepDuration, int frp, int sec, int secondToSleep, int minuteToSleep)
{
  static unsigned long timePointSleep = millis();
  // Setup the count down
  if (keyPad != noTouch)
  {
    timePointSleep = millis();
  }
  else if (millis() - timePointSleep > (sec * 1000U))
  {
    int sleepSeconds;
    if (minuteToSleep < 30)
    {
      sleepSeconds = ((sleepDuration - ((minuteToSleep % sleepDuration) + 1)) * 60) + (60 - secondToSleep);
    }
    else
    {
      sleepSeconds = (((sleepDuration * 2) - ((minuteToSleep % (sleepDuration * 2)) + 1)) * 60) + (60 - secondToSleep);
    }
    return sleepSeconds;
  }
  return -1;
}

/*
  Setting the sleeping configuration and holding the pins state during the sleep mode.
*/
void SleepLogic::goToSleep(int sleepSeconds)
{
  // Configure Timer as wakeup source
  esp_sleep_enable_timer_wakeup(sleepSeconds * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(sleepSeconds) + " Seconds");
  if (ESP_OK == gpio_hold_en(GPIO_NUM_14))
  {
    Serial.print("HOLD 14 - ");
  }
  else
  {
    Serial.print("NO HOLD 14 - ");
  }

  if (ESP_OK == gpio_hold_en(GPIO_NUM_27))
  {
    Serial.print("HOLD 27 - ");
  }
  else
  {
    Serial.print("NO HOLD 27 - ");
  }

  if (ESP_OK == gpio_hold_en(GPIO_NUM_26))
  {
    Serial.print("HOLD 26 - ");
  }
  else
  {
    Serial.print("NO HOLD 26 - ");
  }

  if (ESP_OK == gpio_hold_en(GPIO_NUM_25))
  {
    Serial.print("HOLD 25 - ");
  }
  else
  {
    Serial.print("NO HOLD 25 - ");
  }

  if (ESP_OK == gpio_hold_en(GPIO_NUM_33))
  {
    Serial.print("HOLD 33 ");
  }
  else
  {
    Serial.println("NO HOLD 33 ");
  }
  gpio_deep_sleep_hold_en();
  esp_deep_sleep_start();
}

/*
  Printing the reason by which ESP32 has been awaken from sleep and disabling gpio pad hold function.
*/
bool SleepLogic::printWakeupReason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    return false;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    return false;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    if (ESP_OK == gpio_hold_dis(GPIO_NUM_14))
    {
      Serial.print("DISABLE HOLD 14 - ");
    }
    else
    {
      Serial.print("AGAIN HOLD 14 - ");
    }

    if (ESP_OK == gpio_hold_dis(GPIO_NUM_27))
    {
      Serial.print("DISABLE HOLD 27 - ");
    }
    else
    {
      Serial.print("AGAIN HOLD 27 - ");
    }

    if (ESP_OK == gpio_hold_dis(GPIO_NUM_26))
    {
      Serial.print("DISABLE HOLD 26 - ");
    }
    else
    {
      Serial.print("AGAIN HOLD 26 - ");
    }

    if (ESP_OK == gpio_hold_dis(GPIO_NUM_25))
    {
      Serial.print("DISABLE HOLD 25 - ");
    }
    else
    {
      Serial.print("AGAIN HOLD 25 - ");
    }

    if (ESP_OK == gpio_hold_dis(GPIO_NUM_33))
    {
      Serial.print("DISABLE HOLD 33 - ");
    }
    else
    {
      Serial.print("AGAIN HOLD 33 ");
    }
    gpio_deep_sleep_hold_dis();
    return false;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    if (ESP_OK == gpio_hold_dis(GPIO_NUM_14))
    {
      Serial.print("DISABLE HOLD 14 - ");
    }
    else
    {
      Serial.print("AGAIN HOLD 14 - ");
    }

    if (ESP_OK == gpio_hold_dis(GPIO_NUM_27))
    {
      Serial.print("DISABLE HOLD 27 - ");
    }
    else
    {
      Serial.print("AGAIN HOLD 27 - ");
    }

    if (ESP_OK == gpio_hold_dis(GPIO_NUM_26))
    {
      Serial.print("DISABLE HOLD 26 - ");
    }
    else
    {
      Serial.print("AGAIN HOLD 26 - ");
    }

    if (ESP_OK == gpio_hold_dis(GPIO_NUM_25))
    {
      Serial.print("DISABLE HOLD 25 - ");
    }
    else
    {
      Serial.print("AGAIN HOLD 25 - ");
    }

    if (ESP_OK == gpio_hold_dis(GPIO_NUM_33))
    {
      Serial.print("DISABLE HOLD 33 ");
    }
    else
    {
      Serial.print("AGAIN HOLD 33 ");
    }
    gpio_deep_sleep_hold_dis();
    return false;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    return false;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    return true;
  }
}

/*
  Pinting the touchpad by which ESP32 has been awaken from sleep
*/
void SleepLogic::printWakeupTouchpad()
{

  touch_pad_t touchPin;
  touchPin = esp_sleep_get_touchpad_wakeup_status();

  switch (touchPin)
  {
  case 0:
    Serial.println("Touch detected on GPIO 4");
    break;
  case 1:
    Serial.println("Touch detected on GPIO 0");
    break;
  case 2:
    Serial.println("Touch detected on GPIO 2");
    break;
  case 3:
    Serial.println("Touch detected on GPIO 15");
    break;
  case 4:
    Serial.println("Touch detected on GPIO 13");
    break;
  case 5:
    Serial.println("Touch detected on GPIO 12");
    break;
  case 6:
    Serial.println("Touch detected on GPIO 14");
    break;
  case 7:
    Serial.println("Touch detected on GPIO 27");
    break;
  case 8:
    Serial.println("Touch detected on GPIO 33");
    break;
  case 9:
    Serial.println("Touch detected on GPIO 32");
    break;
  default:
    Serial.println("Wakeup not by touchpad");
    break;
  }
}
