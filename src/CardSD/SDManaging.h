#ifndef SDManaging_H
#define SDManaging_H

/*
 * 
 */

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

class SDCard
{
private:
  // File myFile;
  const char *filename = "/SETTINGS.JSON";    // <- SD library uses 8.3 filenames
  const char *filename1 = "/CONNECTION.JSON"; // <- SD library uses 8.3 filenames
  Config config;                              // <- global configuration object

public:
  SDCard(int pin);

  ~SDCard() {}

  void setStartHour(int value) { config.startHour = value; }
  void setStartMinutes(int value) { config.startMinutes = value; }
  void setEndHour(int value) { config.endHour = value; }
  void setEndMinutes(int value) { config.endMinutes = value; }
  void setTempMax(byte value) { config.tempMax = value; }
  void setTempMin(byte value) { config.tempMin = value; }
  void setAutoEnabled(bool value) { config.autoEnabled = value; }
  void setManteinEnabled(bool value) { config.manteinEnabled = value; }
  void setOnOffLightAuto(bool value) { config.onOffLightAuto = value; }
  void setOnOffHeater(bool value) { config.onOffHeater = value; }
  void setOnOffEC(bool value) { config.onOffEC = value; }
  void setOnOffPH(bool value) { config.onOffPH = value; }
  void setOnOffTemperature(bool value) { config.onOffTemperature = value; }
  void setOnOffFilter(bool value) { config.onOffFilter = value; }
  void setOnOffTemperatureSending(bool value) { config.onOffTemperatureSending = value; }
  void setOnOffECSending(bool value) { config.onOffECSending = value; }
  void setOnOffPhSending(bool value) { config.onOffPhSending = value; }
  void setFreqUpdateWebTemperature(byte value) { config.freqUpdateWebTemperature = value; }
  void setFreqUpdateWebEC(byte value) { config.freqUpdateWebEC = value; }
  void setFreqUpdateWebPH(byte value) { config.freqUpdateWebPH = value; }
  void setFreqFilter(byte value) { config.freqFilter = value; }
  void setHourLoading(byte value) { config.hourLoading = value; }
  void setMinLoading(byte value) { config.minLoading = value; }
  void setRelay0(bool value) { config.relay0 = value; }
  void setRelay1(bool value) { config.relay1 = value; }
  void setRelay2(bool value) { config.relay2 = value; }
  void setRelay3(bool value) { config.relay3 = value; }
  void setTemperature(double value) { config.temperature = value; }
  void setEc(double value) { config.ec = value; }
  void setPh(double value) { config.ph = value; }

  double getTemperature() { return config.temperature; }
  double getEc() { return config.ec; }
  double getPh() { return config.ph; }
  int getStartHour() { return config.startHour; }
  int getStartMinutes() { return config.startMinutes; }
  int getEndHour() { return config.endHour; }
  int getEndMinutes() { return config.endMinutes; }
  byte getTempMax() { return config.tempMax; }
  byte getTempMin() { return config.tempMin; }
  bool getAutoEnabled() { return config.autoEnabled; }
  bool getManteinEnabled() { return config.manteinEnabled; }
  bool getOnOffLightAuto() { return config.onOffLightAuto; }
  bool getOnOffHeater() { return config.onOffHeater; }
  bool getOnOffEC() { return config.onOffEC; }
  bool getOnOffPH() { return config.onOffPH; }
  bool getOnOffTemperature() { return config.onOffTemperature; }
  bool getOnOffFilter() { return config.onOffFilter; }
  bool getOnOffTemperatureSending() { return config.onOffTemperatureSending; }
  bool getOnOffECSending() { return config.onOffECSending; }
  bool getOnOffPhSending() { return config.onOffPhSending; }
  byte getFreqUpdateWebTemperature() { return config.freqUpdateWebTemperature; }
  byte getFreqUpdateWebEC() { return config.freqUpdateWebEC; }
  byte getFreqUpdateWebPH() { return config.freqUpdateWebPH; }
  byte getFreqFilter() { return config.freqFilter; }
  byte getHourLoading() { return config.hourLoading; }
  byte getMinLoading() { return config.minLoading; }
  bool getRelay0() { return config.relay0; }
  bool getRelay1() { return config.relay1; }
  bool getRelay2() { return config.relay2; }
  bool getRelay3() { return config.relay3; }
  char *getSSID() { return config.ssid; }
  char *getPassword() { return config.password; }
  char *getHostName() { return config.hostname; }
  char *getCert() { return config.cert; }

  /**
   * @fn saveConfiguration
   * @brief Saving the configuration to a file in sd card.
   */
  void saveConfiguration();

  /**
   * @fn setConfiguration
   * @brief Setting the configuration to a file the first time in sd card.
   */
  void setConfiguration();

  /**
   * @fn loadConfiguration
   * @brief Loading the configuration from a fil in sd card.
   */
  void loadConfiguration();

  /**
   * @fn loadCertificate
   * @brief Loading the web certificate from a file in sd card.
   */
  void loadCertificate();

  /**
   * @fn resendValueToWeb
   * @brief Sending the value to web site again.
   */
  String resendValueToWeb(String value, String key, String timeStamp);

  /**
   * @fn printFile 
   * @brief Printing the content of a file to the Serial.
   * @param filename: name of the file to read
   */
  void printFile(const char *filename);

  /**
   * @fn loadingNotSentMeasure
   * @brief Loading not sent measure from a file 
   * @param D: day to read the file 
   */
  void loadingNotSentMeasure(int D);

  /**
   * @fn saveValue 
   * @brief Saving a monitoring value in a sd card file.
   * @param filename: name of the file to read 
   * @param value: value to save
   * @param timeStamp: timestamp of the monitoring value
   * @param key: value's sensor name  
   */
  void saveValue(const String filename, const String value, const String timeStamp, const String key);

  /**
   * @fn readFile 
   * @brief File reading.
   * @param fs: fs object
   * @param path: path of the file to read
   */
  void readFile(fs::FS &fs, const char *path);

  /**
   * @fn appendFile 
   * @brief File writing in append mode.
   * @param fs: fs object
   * @param path: path of the file to write
   * @param message: string to write
   */
  void appendFile(fs::FS &fs, const char *path, const char *message);
  
  /**
   * @fn deleteFile 
   * @brief File deleting.
   * @param fs: fs object
   * @param path: path of the file to read
   */
  void deleteFile(fs::FS &fs, const char *path);

  /**
   * @fn onAutomaticProcess 
   * @brief Setting and saving the auto process in the configuration file.
   */
  void onAutomaticProcess();

  /**
   * @fn offAutomaticProcess 
   * @brief Setting and saving the maintenace process in the configuration file.
   */
  void offAutomaticProcess();

  /**
   * @fn activeTemperatureMonitoring 
   * @brief Saving the value in the configuration file.
   * @param value: value to save
   */
  void activeTemperatureMonitoring(boolean value);

  /**
   * @fn activeECMonitoring 
   * @brief Saving the value in the configuration file.
   * @param value: value to save
   */
  void activeECMonitoring(boolean value);
  
  /**
   * @fn activePHMonitoring 
   * @brief Saving the value in the configuration file.
   * @param value: value to save
   */
  void activePHMonitoring(boolean value);
};

#endif