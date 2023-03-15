/*
 * Aquarium Project Pasquale
 */

#include <Arduino.h>
#include "KeyPad/KeyPadManaging.h"
#include "Connection/ConnectionManaging.h"
#include "MenuScreen/LCD.h"
#include "MenuScreen/OLED.h"
#include "SleepMode/SleepModeFunctions.h"
#include "TimeAndDate/TimeAndDateManaging.h"
#include "CardSD/SDManaging.h"
#include "Accessories/AccessoriesManaging.h"

#define CHOOSE_SCREEN 2 // 1 LCD - 2 OLED
#define PIN_EC_SENSOR_POWER 17
#define PIN_PH_SENSOR_POWER 14
#define TdsSensorPin A7
#define PH_PIN A6

/*PIN MAPPING ESP32
   -------------------I/O DIGITALI--------------------------------
   G27 RELE' in 1
   G26 RELE' in 2
   G25 RELE' in 3
   G33 RELE' in 4
   G19 miso SD
   G23 mosi SD
   G18 sck SD
   G5  cs SD
   G17 MOSFET SWITCH EC
   G14 MOSFET SWITCH PH
   ---------------------------------------------------------------
   5v alimentazione board
   gnd alimentazione board
   -----------------I ANALOGICI-----------------------------------
   G4  Tasti keypad
   G35 TDS-EC Meter v 1.0 KS0429
   G34 PH
   G13 DS18B20
   G21 SDA
   G22 SCL
*/

//-----------------------------------------------------------------------------------------------------------

// dati salvati in una memoria speciale per essere ripresi dopo lo sleep
RTC_DATA_ATTR int bootCount = 0;
// const char* ssid = "FASTWEB-F2D36D";//"DESKTOP-B0CAUQR 2037";//"PIA_POCO_F3"; //;
// const char* password = "Tigrotto3";//"Pasquale";//"A30?2p10";
// const char*  server = "www.forestaaquatica.altervista.org";

// Global Variables
TimeAndDate *timeData;
int secondToSleep = -1;
int _timeStamp = 0;
long gmtOffset_sec = 3600; // one hour more
int daylightOffset_sec = 3600;
// int _timerSleep = 0; //temp
int wakeUpDuration = 5;
int sleepDuration = 30;
SleepLogic *sleepLogic;
ConnectionManaging *connection;
Accessories *accessories;
bool filterStatus = false;
bool filterStatusPrec = false;
bool lightStatus = false;
bool lightStatusPrec = false;
bool temperatureStatus = false;
bool temperatureStatusPrec = false;
bool heaterStatus = false;
bool heaterStatusPrec = false;
const char *connectionStatus;
// RTC_DATA_ATTR int time_[3] = {0, 0, 0};

//  ------------------   Rele variables   -------------------------
// RTC_DATA_ATTR bool releSymbol[4] = {false, false, false, false};
bool releSymbol[4] = {false, false, false, false};
int const numRele = sizeof(releSymbol) / sizeof(releSymbol[0]);
byte rele[4] = {A17, A19, A18, A5}; // 27, 26, 25, 33
// ------------------  Termometer  -------------------------------
float temperature = 0.0;
// ------------------  EC Meter  ---------------------------------
float ec = 0.0;
bool ecAuto = false;
bool ecManual = false;
bool manualSendingEC = false;
// ------------------  PH GRAVITY  -------------------------------
float ph = 0.0;
bool phManual = false;
bool phAuto = false;
bool manualSendingPH = false;
// ---------------   KeyPad variables   --------------------------
KeyPad *keyPad;
int key = 0;
byte code_keys[7] = {1, 2, 3, 4, 5, 6, 0}; // {right, left, up, down, ok, 'null at the moment', resetKey}
SDCard *sdCard;
// -------------------   Menu variables   ------------------------
bool menuOnOff = false;
Screen *menu;
// ------------   Display timer becklight   ----------------------
bool screenBackLightOnOff = false;
// int _timerLightDisplay = 0; //temp
char buffer[16];
int orario[3] = {0, 0, 0};
int *orario_;
byte previousSec = 0;
const char *keyTemp = "Temp";
const char *keyEC = "Ec";
const char *keyPh = "Ph";

void setup()
{
  // put your setup code here, to run once:

  // Initialize in and out port
  Serial.begin(115200);
  while (!Serial)
    continue;

  sdCard = new SDCard(SS); // pin G5

  sleepLogic = new SleepLogic(1800, 40);

  byte keypadAnalogPin = T0; // pin G4

  keyPad = new KeyPad(keypadAnalogPin, code_keys);

  // Calling Led or Oled constructor with okButton in input
  if (CHOOSE_SCREEN == 1)
  {
    menu = new Lcd(keyPad->getOkKeyPadValue());
  }
  else
  {
    menu = new Oled(keyPad->getOkKeyPadValue());
  }

  accessories = new Accessories(TdsSensorPin, PH_PIN, PIN_EC_SENSOR_POWER, PIN_PH_SENSOR_POWER);

  connection = new ConnectionManaging(sdCard->getSSID(), sdCard->getPassword(), sdCard->getHostName(), sdCard->getCert());

  // Check if get the time from ntp or normal RTC
  if (connection->connectWiFi())
  {
    timeData = new TimeAndDate(gmtOffset_sec, daylightOffset_sec);
    connection->disconnectWifi();
  }
  else
  {
    timeData = new TimeAndDate();
  }

  // sleepLogic->printWakeupTouchpad();

  delay(100);
  Serial.flush();

  // Setting the analogue pin for input pad touched and turn on the internal pullup resistor
  pinMode(keyPad->getPin(), INPUT_PULLUP);

  // Show init animation with the welcome message only the first time not after the sleep mode.
  if (sleepLogic->printWakeupReason())
  {
    for (int i = 0; i < numRele; i++)
    {
      pinMode(rele[i], OUTPUT);
      digitalWrite(rele[i], HIGH);
    }
    menu->initScreen();
  }
  else
  {
    // Waking up side with load relay
    releSymbol[0] = sdCard->getRelay0();
    releSymbol[1] = sdCard->getRelay1();
    releSymbol[2] = sdCard->getRelay2();
    releSymbol[3] = sdCard->getRelay3();
    for (int i = 0; i < numRele; i++)
    {
      pinMode(rele[i], OUTPUT);
      if (!releSymbol[i])
      {
        digitalWrite(rele[i], HIGH);
      }
      else
      {
        digitalWrite(rele[i], LOW);
      }
    }
    // Force to turn on the screen backlight in loading
    menu->checkScreenBackLight(keyPad->getUpKeyPadValue(), keyPad->getResetKeyPadValue(), menu->getFrameRate(), 30);
  }

  // Setting Auto Process or maintenance
  if (sdCard->getManteinEnabled())
  {
    Serial.println("Mantain");
    sdCard->offAutomaticProcess();
    // Turning off Filter, Termometer, Oxygen and mantein the Lights on the same status
    for (int i = 1; i < numRele; i++)
    {
      if (releSymbol[i] == 1)
      {
        menu->manageReleSymbolAndAction(i, 1);
      }
      delay(500);
    }
  }

  // Simulate the first touch to reset the count down
  sleepLogic->sleepCountDown(keyPad->getUpKeyPadValue(), keyPad->getResetKeyPadValue(), sleepDuration, menu->getFrameRate(),
                             (wakeUpDuration * 60), timeData->getSecond(), timeData->getMinute());

  // Printing rele symbols
  menu->releSymbolMenu();

  // Setting measuremnts from sdCard
  ph = sdCard->getPh();
  ec = sdCard->getEc();
  temperature = sdCard->getTemperature();
}

void loop()
{
  orario_ = timeData->getTime(orario);
  sprintf(buffer, "%02d:%02d:%02d", (orario_[0]), orario_[1], orario_[2]);
  _timeStamp = timeData->getTimeStamp();
  // Serial.print(orario_[0]);
  // Serial.print(" ");
  // Serial.print(orario_[1]);
  // Serial.print(" ");
  // Serial.println(orario_[2]);
  // Serial.println(temperature);
  //  TODO gia previsto metodo per gstione alla non connessione
  // Checking when loading Measure not sended because of some issues
  //  if (sdCard->getOnOffTemperatureSending() || sdCard->getOnOffPhSending() || sdCard->getOnOffECSending()){
  //    chackWhenResendMeasure(config.hourLoading, config.minLoading);
  //  }

  // Getting temperature value if it is enabled
  if (sdCard->getOnOffTemperature())
  {
    temperature = accessories->getTemperatureDS18B20(temperature);
    // Sending Temperature value to web page if it is selected by menu
    if (menu->checkIfSendTemperatureToWebManually())
    {
      connection->sendValueToWeb(temperature, keyTemp, String(_timeStamp));
      menu->setManualSendingTemperature(false);
    }
    // Sent the temperature to WEB DB
    if (sdCard->getOnOffTemperatureSending())
    {
      if (accessories->chackIfSendValueToWeb(sdCard->getFreqUpdateWebTemperature(), 2, temperature, keyTemp,
                                             orario_[0], orario_[1], orario_[2]))
      {
        connection->sendValueToWeb(temperature, keyTemp, String(_timeStamp));
      }
    }
  }

  // Checking when turning on lights automatically
  if (sdCard->getOnOffLightAuto())
  {
    lightStatus = accessories->manageAquariumLights(orario_[0], orario_[1], sdCard->getStartHour(),
                                                    sdCard->getEndHour(), sdCard->getStartMinutes(), sdCard->getEndMinutes());
    if (lightStatus != lightStatusPrec)
    {
      if (lightStatus)
      {
        // Serial.println("LUCI ON");
        menu->manageReleSymbolAndAction(0, 0);
        menu->releSymbolMenu();
      }
      else
      {
        // Serial.println("LUCI OFF");
        menu->manageReleSymbolAndAction(0, 1);
        menu->releSymbolMenu();
      }
      lightStatusPrec = lightStatus;
    }
  }

  // Checking when turning on filter automatically
  if (sdCard->getOnOffFilter())
  {
    filterStatus = accessories->checkIfGetMeasurement(sdCard->getFreqFilter(), 1, byte(orario_[0]), byte(orario_[1]), 59);
    if (filterStatus != filterStatusPrec)
    {
      if (filterStatus)
      {
        Serial.println("FILTER ON");
        menu->manageReleSymbolAndAction(1, 0);
        menu->releSymbolMenu();
      }
      else
      {
        Serial.println("FILTER OFF");
        menu->manageReleSymbolAndAction(1, 1);
        menu->releSymbolMenu();
      }
      filterStatusPrec = filterStatus;
    }
  }

  // Getting EC value automatically only when it is enabled and scheduled
  if (sdCard->getOnOffEC())
  {
    if (accessories->checkIfGetMeasurement(sdCard->getFreqUpdateWebEC(), 1, byte(orario_[0]), byte(orario_[1]), 3))
    {
      accessories->activeSensorEC(PIN_EC_SENSOR_POWER, true);
      ec = accessories->getEC_DF(int(temperature));
      ecAuto = true;
    }
    else if (ecAuto)
    {
      ecAuto = false;
      accessories->activeSensorEC(PIN_EC_SENSOR_POWER, false);
    }
    // Sending EC value to web page if it is selected by menu
    if (menu->checkIfSendECToWebManually())
    {
      connection->sendValueToWeb(ec, keyEC, String(_timeStamp));
      menu->setManualSendingEC(false);
    }
    // Showing EC value on screen if it is selected by menu
    if (menu->checkIfShowECMonitoringOnScreen())
    {
      accessories->activeSensorEC(PIN_EC_SENSOR_POWER, true);
      ec = accessories->getEC_DF(int(temperature));
      menu->showMonitoring(keyEC, ec, temperature);
      ecManual = true;
    }
    else if (ecManual)
    {
      accessories->activeSensorEC(PIN_EC_SENSOR_POWER, false);
      ecManual = false;
    }

    // Sending EC value to WEB DB
    if (sdCard->getOnOffECSending())
    {
      if (accessories->chackIfSendValueToWeb(sdCard->getFreqUpdateWebEC(), 4, ec, keyEC,
                                             orario_[0], orario_[1], orario_[2]))
      {
        connection->sendValueToWeb(ec, keyEC, String(_timeStamp));
      }
    }
  }
  else
  {
    if (menu->checkIfSendECToWebManually())
    {
      menu->setManualSendingEC(false);
    }
    if (menu->checkIfShowECMonitoringOnScreen())
    {
      menu->setMonitorEC(false);
    }
  }

  // Getting PH value automatically only when it is enabled and scheduled
  if (sdCard->getOnOffPH())
  {
    if (accessories->checkIfGetMeasurement(sdCard->getFreqUpdateWebPH(), 31, orario_[0], orario_[1], 3))
    {
      phAuto = true;
      accessories->activeSensorPH(PIN_PH_SENSOR_POWER, true);
      ph = accessories->getPH(int(temperature));
    }
    else if (phAuto)
    {
      phAuto = false;
      accessories->activeSensorPH(PIN_PH_SENSOR_POWER, false);
    }
    // Sending PH value to web page if it is selected by menu
    if (menu->checkIfSendPHToWebManually())
    {
      connection->sendValueToWeb(ph, keyPh, String(_timeStamp));
      menu->setManualSendingPH(false);
    }
    // Showing PH value on screen if it is selected by menu
    if (menu->checkIfShowPHMonitoringOnScreen())
    {
      phManual = true;
      accessories->activeSensorPH(PIN_PH_SENSOR_POWER, true);
      ph = accessories->getPH(int(temperature));
      menu->showMonitoring(keyPh, ph, temperature);
    }
    else if (phManual)
    {
      phManual = false;
      accessories->activeSensorPH(PIN_PH_SENSOR_POWER, false);
    }

    // Sending PH value to WEB DB
    if (sdCard->getOnOffPhSending())
    {
      if (accessories->chackIfSendValueToWeb(sdCard->getFreqUpdateWebPH(), 34, ph, keyPh,
                                             orario_[0], orario_[1], orario_[2]))
      {
        connection->sendValueToWeb(ph, keyPh, String(_timeStamp));
      }
    }
  }
  else
  {
    if (menu->checkIfSendPHToWebManually())
    {
      menu->setManualSendingPH(false);
    }
    if (menu->checkIfShowPHMonitoringOnScreen())
    {
      menu->setMonitorPH(false);
    }
  }

  if (menu->getConnectionShow())
  {
    menu->connectionStatusPage(connectionStatus);
  }

  // turning on the heater automatically if it is enabled
  if (sdCard->getOnOffHeater())
  {
    heaterStatus = accessories->manageAcquarioumHeater(temperature, sdCard->getTempMin(), sdCard->getTempMax());
    if (heaterStatus != heaterStatusPrec)
    {
      if (heaterStatus)
      {
        menu->manageReleSymbolAndAction(2, 0);
        menu->releSymbolMenu();
      }
      else
      {
        menu->manageReleSymbolAndAction(2, 1);
        menu->releSymbolMenu();
      }
      heaterStatusPrec = heaterStatus;
    }
  }

  // Getting kayboard value
  key = keyPad->getKeyPadValue();

  if (!menuOnOff && key == keyPad->getOkKeyPadValue())
  {
    menuOnOff = true;
  }

  // Check when turning on the screen backlight
  menu->checkScreenBackLight(key, keyPad->getResetKeyPadValue(), menu->getFrameRate(), 30);

  secondToSleep = sleepLogic->sleepCountDown(key, keyPad->getResetKeyPadValue(), sleepDuration, menu->getFrameRate(), (wakeUpDuration * 60),
                                             timeData->getSecond(), timeData->getMinute());

  //  digitalWrite(PIN_EC_SENSOR_POWER, LOW);
  //  ec = accessories->getEC_DF(temperature);
  //  digitalWrite(PIN_PH_SENSOR_POWER, LOW);
  //  phFinal = accessories->getPH(temperature);
  //  Serial.printf("timeSleep: %d\n",secondToSleep);
  //  Serial.printf("timestamp: %d\n", _timeStamp);
  //  Serial.println(String(releSymbol[0])+ " "+ String(releSymbol[1])+" "+String(releSymbol[2])+" "+String(releSymbol[3]));
  //  Serial.println("TIMER LIGHT DISPALY = " + String(_timerLightDisplay));
  //  Serial.println("KEY PAD VALUE = " + String(key));
  //  Serial.println("ACTIVE MENU = " + String(activeMenu));
  //  Serial.println("MENU = " + String(menuOnOff));
  //  Serial.println("EC = " + String(ec));
  //  Serial.println("TEMPERATURE = " + String(temperature));

  if (secondToSleep != -1)
  {
    sdCard->setEc(ec);
    sdCard->setPh(ph);
    sdCard->setTemperature(temperature);
    sdCard->saveConfiguration();
    delay(10);
    sleepLogic->goToSleep(secondToSleep);
  }

  // menu on screen
  if (menuOnOff)
  {
    menuOnOff = menu->menu_(key);
  }
  else
  { // menu off
    menu->mainScreen(buffer, temperature, ec, ph);
  }

  delay(menu->getFrameRate());
  key = keyPad->getResetKeyPadValue();
}

void Screen::checkScreenBackLight(int keyPad, byte noTouch, int frp, byte sec)
{
  static unsigned long timepoint = millis();
  if (keyPad != noTouch)
  {
    timepoint = millis();
    screenBackLightOnOff = true;
    menu->turnOnLight();
  }
  else if (screenBackLightOnOff && millis() - timepoint > (sec * 1000U))
  {
    menu->turnOffLight();
    screenBackLightOnOff = false;
  }
  else
  {
    return;
  }
}

void Screen::manageReleSymbolAndAction(int index, int onOff)
{
  if (onOff == 0)
  {
    digitalWrite(rele[index], LOW);
    releSymbol[index] = true;
    switch (index)
    {
    case 0:
      sdCard->setRelay0(true);
      break;
    case 1:
      sdCard->setRelay1(true);
      break;
    case 2:
      sdCard->setRelay2(true);
      break;
    case 3:
      sdCard->setRelay3(true);
      break;
    }
    sdCard->saveConfiguration();
  }
  else
  {
    digitalWrite(rele[index], HIGH);
    releSymbol[index] = false;
    switch (index)
    {
    case 0:
      sdCard->setRelay0(false);
      break;
    case 1:
      sdCard->setRelay1(false);
      break;
    case 2:
      sdCard->setRelay2(false);
      break;
    case 3:
      sdCard->setRelay3(false);
      break;
    }
    sdCard->saveConfiguration();
  }
}

void Screen::releSymbolMenu()
{
  showReleSymbol(releSymbol, numRele);
}

void Screen::manageAutomationProcessAndMaintenance(int itemSelected, int onOff)
{
  if (itemSelected == 1)
  {
    if (onOff == 0)
    {
      sdCard->onAutomaticProcess();
    }
  }
  else
  {
    if (onOff == 0)
    {
      sdCard->offAutomaticProcess();
    }
  }
  sdCard->saveConfiguration();
}

void Screen::lightsTimerSetting()
{
  setTimerLights(sdCard->getStartHour(), sdCard->getStartMinutes(), sdCard->getEndHour(), sdCard->getEndMinutes());
}

void Screen::timeSetting()
{
  setHourAndMinutes(timeData->getHour(), timeData->getMinute());
}

void Screen::recoverySetting()
{
  setRecoveryHourAndMinutes(sdCard->getHourLoading(), sdCard->getMinLoading());
}

void Screen::dateSetting()
{
  setDayMonthYear(timeData->getDay(), timeData->getMonth(), timeData->getYear());
}

void Screen::saveTime(byte *hourEMin)
{
  // char timeBuffer[8];
  // char timeBufferSub[8];
  //  char dateBuffer[11];
  //  Serial.print("1T: ");
  // sprintf(timeBuffer, "%02d:%02d:00", hourEMin[0], hourEMin[1]);
  //  Serial.print("2T: ");
  //  sprintf(dateBuffer, "%s %02d %04d",  timeData->getMonthName(), timeData->getDay(), timeData->getYear());
  //  Serial.print("3T: ");
  // substring(timeBuffer, timeBufferSub, 1, 8);
  //  Serial.print("Time buffer: "+String(dateBuffer)+ " "+String(timeBufferSub));
  //  Serial.print("T: ");
  //  Serial.println(sizeof(dateBuffer));
  timeData->setTimeAndData1(timeData->getYear(), timeData->getMonth(), timeData->getDay(), hourEMin[0], hourEMin[1], 0);
}

void Screen::saveDate(int *date)
{
  // char dateBuffer[11];
  // char timeBuffer[8];
  // char timeBufferSub[8];
  // sprintf(timeBuffer, "%02d:%02d:%02d", timeData->getHour(), timeData->getMinute(), timeData->getSecond());
  // sprintf(dateBuffer, "%s %02d %04d", timeData->monthName(date[1]), date[0], date[2]);
  // substring(timeBuffer, timeBufferSub, 1,8);
  // Serial.println("Date buffer date: " + String(dateBuffer) );
  // Serial.println("Date buffer time: " + String(timeBufferSub));
  // Serial.printf("D: %d, %d, %d, %d, %d, %d, ",date[2], date[1], date[0], timeData->getHour(), timeData->getMinute(), timeData->getSecond());
  // Serial.println(sizeof(dateBuffer));
  // Serial.println(sizeof(timeBufferSub));
  timeData->setTimeAndData1(date[2], date[1], date[0], timeData->getHour(), timeData->getMinute(), timeData->getSecond());
}

void Screen::saveTimerLights(byte *hourAndMin)
{
  sdCard->setStartHour(hourAndMin[0]);
  sdCard->setStartMinutes(hourAndMin[1]);
  sdCard->setEndHour(hourAndMin[2]);
  sdCard->setEndMinutes(hourAndMin[3]);
  sdCard->saveConfiguration();
}

void Screen::saveRecoveryTime(byte *hourAndMin)
{
  sdCard->setHourLoading(hourAndMin[0]);
  sdCard->setMinLoading(hourAndMin[1]);
  sdCard->saveConfiguration();
}

void Screen::manageSettingsSelections(int item, int colItem)
{
  switch (item)
  {
  case 7:
    if (colItem == 0)
    {
      sdCard->activeTemperatureMonitoring(true);
    }
    else
    {
      sdCard->activeTemperatureMonitoring(false);
    }
    sdCard->saveConfiguration();
    break;
  case 8:
    if (colItem == 0)
    {
      sdCard->activeECMonitoring(true);
    }
    else
    {
      sdCard->activeECMonitoring(false);
    }
    sdCard->saveConfiguration();
    break;
  case 9:
    if (colItem == 0)
    {
      sdCard->activePHMonitoring(true);
    }
    else
    {
      sdCard->activePHMonitoring(false);
    }
    sdCard->saveConfiguration();
    break;
  case 10:
    if (colItem == 0 && !sdCard->getOnOffTemperatureSending())
    {
      sdCard->setOnOffTemperatureSending(true);
    }
    else if (colItem == 1 && sdCard->getOnOffTemperatureSending())
    {
      sdCard->setOnOffTemperatureSending(false);
    }
    sdCard->saveConfiguration();
    break;
  case 11:
    if (colItem == 0 && !sdCard->getOnOffECSending())
    {
      sdCard->setOnOffECSending(true);
    }
    else if (colItem == 1 && sdCard->getOnOffECSending())
    {
      sdCard->setOnOffECSending(false);
    }
    sdCard->saveConfiguration();
    break;
  case 12:
    if (colItem == 0 && !sdCard->getOnOffPhSending())
    {
      sdCard->setOnOffPhSending(true);
    }
    else if (colItem == 1 && sdCard->getOnOffPhSending())
    {
      sdCard->setOnOffPhSending(false);
    }
    sdCard->saveConfiguration();
    break;
  case 13:
    if (colItem == 0 && !sdCard->getOnOffFilter())
    {
      sdCard->setOnOffFilter(true);
    }
    else if (colItem == 1 && sdCard->getOnOffFilter())
    {
      sdCard->setOnOffFilter(false);
      menu->manageReleSymbolAndAction(1, 1);
      menu->releSymbolMenu();
      filterStatusPrec = false;
    }
    sdCard->saveConfiguration();
    break;
  default:
    break;
  }
}

void Screen::actualTemperatureRange()
{
  menu->setTemperatureRange(sdCard->getTempMin(), sdCard->getTempMax());
}

void Screen::yesOrNoSelection4(byte selectedItem, int rowNumber, String *listMenu)
{
  showYesOrNoSelection4(colItem, rowItemSettingMenu, sdCard->getOnOffTemperatureSending(),
                        sdCard->getOnOffECSending(), sdCard->getOnOffPhSending());
}

void Screen::saveMinMaxTemperature(byte *minAndMax)
{
  sdCard->setTempMin(minAndMax[0]);
  sdCard->setTempMax(minAndMax[1]);
  sdCard->saveConfiguration();
}

void Screen::saveFreqUpdateWeb()
{
  sdCard->setFreqUpdateWebTemperature(setFreq(freqUpdateWebTemperatureIndex));
  sdCard->setFreqUpdateWebEC(setFreq(freqUpdateWebECIndex));
  sdCard->setFreqUpdateWebPH(setFreq(freqUpdateWebPHIndex));
  sdCard->setFreqFilter(setFreq(freqFilterIndex));
  sdCard->saveConfiguration();
}

void Screen::freqNumberIndex()
{
  freqUpdateWebTemperatureIndex = indexNumber(sdCard->getFreqUpdateWebTemperature());
  freqUpdateWebECIndex = indexNumber(sdCard->getFreqUpdateWebEC());
  freqUpdateWebPHIndex = indexNumber(sdCard->getFreqUpdateWebPH());
}

void Screen::reconnectMenu()
{
  connection->disconnectWifi();
  connection->connectWiFi();
}

void Screen::sentConnectionRequest()
{
  connection->connectWiFi();
  connectionStatus = connection->conectionStatus();
  connection->disconnectWifi();
}
