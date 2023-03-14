#ifndef SDManaging_H
#define SDManaging_H

/*
  SD card test for esp32

  This example shows how use the utility libraries

  The circuit:
    SD card attached to SPI bus as follows:
        SS    = 5;
        MOSI  = 23;
        MISO  = 19;
        SCK   = 18;


   by Mischianti Renzo <https://www.mischianti.org>

   https://www.mischianti.org
*/
// include the SD library:

#include <Arduino.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include <SD.h>
#include "FS.h"
#include "SPI.h"
   
  // ------------------   SD varables   ----------------------------
  struct Config
  {
    int startHour;
    int startMinutes;
    int endHour;
    int endMinutes;
    byte tempMax;
    byte tempMin;
    bool autoEnabled;
    bool manteinEnabled;
    bool onOffLightAuto;
    bool onOffHeater;
    bool onOffEC;
    bool onOffPH;
    bool onOffTemperature;
    bool onOffFilter;
    bool onOffTemperatureSending;
    bool onOffECSending;
    bool onOffPhSending;
    byte freqUpdateWebTemperature;
    byte freqUpdateWebEC;
    byte freqUpdateWebPH;
    byte freqFilter;
    byte hourLoading;
    byte minLoading;
    bool relay0;
    bool relay1;
    bool relay2;
    bool relay3;
    double ph;
    double ec;
    double temperature;
    char hostname[64];
    char ssid[16];
    char password[16];
    char cert[2000];
  };

class SDCard{
private:
  // File myFile;
  const char *filename = "/SETTINGS.JSON"; // <- SD library uses 8.3 filenames
  const char *filename1 = "/CONNECTION.JSON"; // <- SD library uses 8.3 filenames
  Config config;                    // <- global configuration object
 

public:

  SDCard(int pin);

  ~SDCard() {}

  void setStartHour(int value){config.startHour = value;}
  void setStartMinutes(int value){config.startMinutes = value;}
  void setEndHour(int value){config.endHour = value;}
  void setEndMinutes(int value){config.endMinutes = value;}
  void setTempMax(byte value){config.tempMax = value;}
  void setTempMin(byte value){config.tempMin = value;}
  void setAutoEnabled(bool value){config.autoEnabled = value;}
  void setManteinEnabled(bool value){config.manteinEnabled = value;}
  void setOnOffLightAuto(bool value){config.onOffLightAuto = value;}
  void setOnOffHeater(bool value){config.onOffHeater = value;}
  void setOnOffEC(bool value){config.onOffEC = value;}
  void setOnOffPH(bool value){config.onOffPH = value;}
  void setOnOffTemperature(bool value){config.onOffTemperature = value;}
  void setOnOffFilter(bool value){config.onOffFilter = value;}
  void setOnOffTemperatureSending(bool value){config.onOffTemperatureSending = value;}
  void setOnOffECSending(bool value){config.onOffECSending = value;}
  void setOnOffPhSending(bool value){config.onOffPhSending = value;}
  void setFreqUpdateWebTemperature(byte value){config.freqUpdateWebTemperature = value;}
  void setFreqUpdateWebEC(byte value){config.freqUpdateWebEC = value;}
  void setFreqUpdateWebPH(byte value){config.freqUpdateWebPH = value;}
  void setFreqFilter(byte value){config.freqFilter = value;}
  void setHourLoading(byte value){config.hourLoading = value;}
  void setMinLoading(byte value){config.minLoading = value;}
  void setRelay0(bool value){config.relay0 = value;}
  void setRelay1(bool value){config.relay1 = value;}
  void setRelay2(bool value){config.relay2 = value;}
  void setRelay3(bool value){config.relay3 = value;}
  void setTemperature(double value){config.temperature = value;}
  void setEc(double value){config.ec = value;}
  void setPh(double value){config.ph = value;}

  double getTemperature(){return config.temperature;}
  double getEc(){return config.ec;}
  double getPh(){return config.ph;}
  int getStartHour(){return config.startHour;}
  int getStartMinutes(){return config.startMinutes;}
  int getEndHour(){return config.endHour;}
  int getEndMinutes(){return config.endMinutes;}
  byte getTempMax(){return config.tempMax;}
  byte getTempMin(){return config.tempMin;}
  bool getAutoEnabled(){return config.autoEnabled;}
  bool getManteinEnabled(){return config.manteinEnabled;}
  bool getOnOffLightAuto(){return config.onOffLightAuto;}
  bool getOnOffHeater(){return config.onOffHeater;}
  bool getOnOffEC(){return config.onOffEC;}
  bool getOnOffPH(){return config.onOffPH;}
  bool getOnOffTemperature(){return config.onOffTemperature;}
  bool getOnOffFilter(){return config.onOffFilter;}
  bool getOnOffTemperatureSending(){return config.onOffTemperatureSending;}
  bool getOnOffECSending(){return config.onOffECSending;}
  bool getOnOffPhSending(){return config.onOffPhSending;}
  byte getFreqUpdateWebTemperature(){return config.freqUpdateWebTemperature;}
  byte getFreqUpdateWebEC(){return config.freqUpdateWebEC;}
  byte getFreqUpdateWebPH(){return config.freqUpdateWebPH;}
  byte getFreqFilter(){return config.freqFilter;}
  byte getHourLoading(){return config.hourLoading;}
  byte getMinLoading(){return config.minLoading;}
  bool getRelay0(){return config.relay0;}
  bool getRelay1(){return config.relay1;}
  bool getRelay2(){return config.relay2;}
  bool getRelay3(){return config.relay3;}
  char* getSSID(){return config.ssid;}
  char* getPassword(){return config.password;}
  char* getHostName(){return config.hostname;}
  char* getCert(){return config.cert;}

  // Saves the configuration to a file
  void saveConfiguration();

  // Saves the configuration to a file
  void setConfiguration();

  // Loads the configuration from a file
  void loadConfiguration();

  void loadCertificate();

  String resendValueToWeb(String value, String key,  String timeStamp);

  // Prints the content of a file to the Serial
  void printFile(const char *filename);

  // Prints the content of a file to the Serial
  void loadingNotSendedMeasure(int D);

  // Saves the configuration to a file
  void saveValue(const String filename, const String value, const String timeStamp, const String key);

  void readFile(fs::FS &fs, const char *path);

  void appendFile(fs::FS &fs, const char *path, const char *message);

  void deleteFile(fs::FS &fs, const char *path);

  void onAutomaticProcess();

  void offAutomaticProcess();

  void activeTemperatureMonitoring(boolean value);

  void activeECMonitoring(boolean value);

  void activePHMonitoring(boolean value);

};

#endif