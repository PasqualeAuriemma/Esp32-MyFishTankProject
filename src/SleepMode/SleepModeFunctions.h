/* Aquarium Project Pasquale */

#ifndef SleepModeFunctions_H_
#define SleepModeFunctions_H_
#include <Arduino.h>

/*
  *** Touch sleep mode ***
  Threshold 40  Greater the value, more the sensitivity
  - Setup interrupt on Touch Pad 3 (GPIO15) with: 
                    touchAttachInterrupt(T3, callback, Threshold);
    When touch is detected on a specified GPIO, a callback function is executed, but
    it will only be executed if the ESP32 is awake.

  - Configure Touchpad as wakeup source:
                    esp_sleep_enable_touchpad_wakeup();
  
  *** Timer sleep mode ***
  uS_TO_S_FACTOR 1000000     Conversion factor for micro seconds to seconds
  TIME_TO_SLEEP  100         Time ESP32 will go to sleep (in seconds) 
  - Configure Timer as wakeup source and set our ESP32 to wake up every 5 seconds:
                    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  
  Next we decide what all peripherals to shut down/keep on
  By default, ESP32 will automatically power down the peripherals
  not needed by the wakeup source, but if you want to be a poweruser
  this is for you. Read in detail at the API docs
  http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
  
  The example below configure peripherals and turns off all RTC peripherals in deep sleep.

  esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  Serial.println("Configured all RTC Peripherals to be powered down in sleep");

  Now that we have setup a wake cause and if needed setup the
  peripherals state in deep sleep, we can now start going to
  deep sleep.
  In the case that no wake up sources were provided but deep
  sleep was started, it will sleep forever unless hardware
  reset occurs.
  
  - Start the sleep mode: 
  esp_deep_sleep_start();

*/

class SleepLogic{
private:
  long uS_TO_S_FACTOR;    /* Conversion factor for micro seconds to seconds */
  int TIME_TO_SLEEP;      /* Time ESP32 will go to sleep (in seconds) */
  int Threshold;
public:
  // The parameters of the constructor takes the time in seconds whenever time is expected and the threshold of the touch pin 
  SleepLogic (int timeToSleep, int threshold);

  void setFactor(long value){this->uS_TO_S_FACTOR = value;}
  void setTimeToSleep(int value){this->TIME_TO_SLEEP = value;}
  void setThreshold(int value){this->Threshold = value;}
  void setT(int value){this->TIME_TO_SLEEP = value;}

  bool printWakeupReason();
  void printWakeupTouchpad();
  int sleepCountDown(int keyPad, byte noTouch, int sleepDuration, int frp, int sec, int secondForSleep, int minuteForSleep);
  void goToSleep(int sleepSeconds);
};

#endif