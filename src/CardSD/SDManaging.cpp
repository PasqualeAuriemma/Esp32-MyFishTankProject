/*
  Aquarium Project Pasquale
*/

#include "SDManaging.h"

SDCard::SDCard(int pin)
{
  Serial.print("\nInitializing SD card...");
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!SD.begin())
  {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    // while (1);
  }
  else
  {
    Serial.println("Wiring is correct and a card is present.");
    // print the type of card
    if (SD.exists(filename))
    {
      // Should load default config if run for the first time
      Serial.println(F("Loading configuration!"));
      loadConfiguration();
      loadCertificate();
    }
    else
    {
      // Create configuration file
      Serial.println(F("Setting configuration!"));
      setConfiguration();
    }
  }
}

void SDCard::saveConfiguration()
{
  // Delete existing file, otherwise the configuration is appended to the file
  SD.remove(filename);

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);

  if (!file)
  {
    Serial.println(F("Failed to create file in saveConfiguration!"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<1536> doc;

  // Set the values in the document
  doc["startHour"] = config.startHour;
  doc["startMinutes"] = config.startMinutes;
  doc["endHour"] = config.endHour;
  doc["endMinutes"] = config.endMinutes;
  doc["tempMax"] = config.tempMax;
  doc["tempMin"] = config.tempMin;
  doc["autoEnabled"] = config.autoEnabled;
  doc["manteinEnabled"] = config.manteinEnabled;
  doc["onOffLightAuto"] = config.onOffLightAuto;
  doc["onOffHeater"] = config.onOffHeater;
  doc["onOffEC"] = config.onOffEC;
  doc["onOffPH"] = config.onOffPH;
  doc["onOffTemperature"] = config.onOffTemperature;
  doc["onOffFilter"] = config.onOffFilter;
  doc["onOffTemperatureSending"] = config.onOffTemperatureSending;
  doc["onOffECSending"] = config.onOffECSending;
  doc["onOffPhSending"] = config.onOffPhSending;
  doc["freqUpdateWebTemperature"] = config.freqUpdateWebTemperature;
  doc["freqUpdateWebEC"] = config.freqUpdateWebEC;
  doc["freqUpdateWebPH"] = config.freqUpdateWebPH;
  doc["freqFilter"] = config.freqFilter;
  doc["hourLoading"] = config.hourLoading;
  doc["minLoading"] = config.minLoading;
  doc["relay0"] = config.relay0;
  doc["relay1"] = config.relay1;
  doc["relay2"] = config.relay2;
  doc["relay3"] = config.relay3;
  doc["temperature"] = config.temperature;
  doc["ec"] = config.ec;
  doc["ph"] = config.ph;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0)
  {
    Serial.println(F("Failed to write to file in saveConfiguration!"));
    return;
  }

  // Close the file
  file.close();
}

void SDCard::setConfiguration()
{
  // Delete existing file, otherwise the configuration is appended to the file
  // deleteFile(SD, filename);

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file)
  {
    Serial.println(F("Failed to create file in setConfiguration!"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<1536> doc;

  // Set the values in the document
  doc["startHour"] = 0;
  doc["startMinutes"] = 0;
  doc["endHour"] = 0;
  doc["endMinutes"] = 0;
  doc["tempMax"] = 0;
  doc["tempMin"] = 0;
  doc["autoEnabled"] = true;
  doc["manteinEnabled"] = false;
  doc["onOffLightAuto"] = false;
  doc["onOffHeater"] = false;
  doc["onOffEC"] = false;
  doc["onOffPH"] = false;
  doc["onOffTemperature"] = false;
  doc["onOffFilter"] = false;
  doc["onOffTemperatureSending"] = false;
  doc["onOffECSending"] = false;
  doc["onOffPhSending"] = false;
  doc["freqUpdateWebTemperature"] = 1;
  doc["freqUpdateWebEC"] = 1;
  doc["freqUpdateWebPH"] = 1;
  doc["freqFilter"] = 1;
  doc["hourLoading"] = 0;
  doc["minLoading"] = 3;
  doc["relay0"] = false;
  doc["relay1"] = false;
  doc["relay2"] = false;
  doc["relay3"] = false;
  doc["temperature"] = 0.0;
  doc["ec"] = 0.0;
  doc["ph"] = 0.0;
  config.startHour = doc["startHour"];
  config.startMinutes = doc["startMinutes"];
  config.endHour = doc["endHour"];
  config.endMinutes = doc["endMinutes"];
  config.tempMax = doc["tempMax"];
  config.tempMin = doc["tempMin"];
  config.autoEnabled = doc["autoEnabled"];
  config.manteinEnabled = doc["manteinEnabled"];
  config.onOffLightAuto = doc["onOffLightAuto"];
  config.onOffHeater = doc["onOffHeater"];
  config.onOffEC = doc["onOffEC"];
  config.onOffPH = doc["onOffPH"];
  config.onOffTemperature = doc["onOffTemperature"];
  config.onOffFilter = doc["onOffFilter"];
  config.onOffTemperatureSending = doc["onOffTemperatureSending"];
  config.onOffECSending = doc["onOffECSending"];
  config.onOffPhSending = doc["onOffPhSending"];
  config.freqUpdateWebTemperature = doc["freqUpdateWebTemperature"];
  config.freqUpdateWebEC = doc["freqUpdateWebEC"];
  config.freqUpdateWebPH = doc["freqUpdateWebPH"];
  config.freqFilter = doc["freqFilter"];
  config.hourLoading = doc["hourLoading"];
  config.minLoading = doc["minLoading"];
  config.relay0 = doc["relay0"];
  config.relay1 = doc["relay1"];
  config.relay2 = doc["relay2"];
  config.relay3 = doc["relay3"];
  config.temperature = doc["temperature"];
  config.ec = doc["ec"];
  config.ph = doc["ph"];

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0)
  {
    Serial.println(F("Failed to write the file in setConfiguration!"));
    return;
  }
  // Close the file
  file.close();
}

void SDCard::loadConfiguration()
{
  // Open file for reading
  File file = SD.open(filename, FILE_READ);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<1536> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.println(F("Failed to read file, using default configuration in loadConfiguration!"));
    Serial.println(error.f_str());
    return;
  }
  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
  // Copy values from the JsonDocument to the Config
  config.startHour = doc["startHour"];
  config.startMinutes = doc["startMinutes"];
  config.endHour = doc["endHour"];
  config.endMinutes = doc["endMinutes"];
  config.tempMax = doc["tempMax"];
  config.tempMin = doc["tempMin"];
  config.autoEnabled = doc["autoEnabled"];
  config.manteinEnabled = doc["manteinEnabled"];
  config.onOffLightAuto = doc["onOffLightAuto"];
  config.onOffHeater = doc["onOffHeater"];
  config.onOffEC = doc["onOffEC"];
  config.onOffPH = doc["onOffPH"];
  config.onOffTemperature = doc["onOffTemperature"];
  config.onOffFilter = doc["onOffFilter"];
  config.onOffTemperatureSending = doc["onOffTemperatureSending"];
  config.onOffECSending = doc["onOffECSending"];
  config.onOffPhSending = doc["onOffPhSending"];
  config.freqUpdateWebTemperature = doc["freqUpdateWebTemperature"];
  config.freqUpdateWebEC = doc["freqUpdateWebEC"];
  config.freqUpdateWebPH = doc["freqUpdateWebPH"];
  config.freqFilter = doc["freqFilter"];
  config.hourLoading = doc["hourLoading"];
  config.minLoading = doc["minLoading"];
  config.relay0 = doc["relay0"];
  config.relay1 = doc["relay1"];
  config.relay2 = doc["relay2"];
  config.relay3 = doc["relay3"];
  config.temperature = doc["temperature"];
  config.ec = doc["ec"];
  config.ph = doc["ph"];
}

void SDCard::loadCertificate()
{
  // Open file for reading
  File file = SD.open(filename1, FILE_READ);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<2560> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.println(F("Failed to read file, using default configuration in loadConfiguration!"));
    Serial.println(error.f_str());
    return;
  }
  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
  // Copy values from the JsonDocument to the Config
  strlcpy(config.ssid, doc["ssid"], sizeof(config.ssid));
  strlcpy(config.password, doc["password"], sizeof(config.password));
  strlcpy(config.hostname,          // <- destination
          doc["hostname"],          // <- source
          sizeof(config.hostname)); // <- destination's capacity
  strlcpy(config.cert, doc["cert"], sizeof(config.cert));
}

String SDCard::resendValueToWeb(String value, String key, String timeStamp)
{
  String payload = "";
  payload += key;
  payload += "=";
  payload += value;
  payload += ":";
  payload += "Date=";
  payload += timeStamp;
  payload += ";";
  return payload;
}

// Prints the content of a file to the Serial
void SDCard::printFile(const char *filename)
{
  // Open file for reading
  File file = SD.open(filename);
  if (!file)
  {
    Serial.println(F("Failed to read file in printFile!"));
    return;
  }

  // Extract each characters by one by one
  while (file.available())
  {
    Serial.print((char)file.read());
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void SDCard::loadingNotSentMeasure(int D)
{
  // Open file for reading
  String filename = "temp_" + String(D) + ".txt";
  String j2;
  File file = SD.open(filename);
  Serial.println(filename);
  if (!file)
  {
    Serial.println(F("Failed to read file in loadingNotSendedMeasure!"));
    return;
  }
  // Extract each characters by one by one
  while (file.available())
  {
    char c = (char)file.read();
    if (c == ';')
    {
      StaticJsonDocument<200> doc;
      DeserializationError err = deserializeJson(doc, j2);
      if (err)
      {
        Serial.println("dese() failed!");
        return;
      }
      j2 = "";
      const char *key = doc["key"];
      const char *value = doc["value"];
      const char *timest = doc["timestamp"];
      // Serial.println(key);
      // Serial.println(value);
      // Serial.println(timest);
      resendValueToWeb(value, key, timest);
      delay(10);
    }
    else
    {
      j2.concat(c);
    }
  }
  // Serial.print((char)file.read());
  //  Close the file
  file.close();
  Serial.println(F("romove loadingNotSendedMeasure!"));
  SD.remove(filename);
}

// Saves the configuration to a file
void SDCard::saveValue(const String filename, const String value, const String timeStamp, const String key)
{
  // Delete existing file, otherwise the configuration is appended to the file
  // SD.remove(filename);

  // Open file for writing
  //  File file = SD.open(filename, FILE_WRITE);

  // Try and append

  File file = SD.open(filename, FILE_APPEND);
  if (!file)
  {
    // It failed, so try and make a new file.
    file = SD.open(filename, FILE_WRITE);
    if (!file)
    {
      // It failed too, so give up.
      Serial.println("Failed to open" + filename + ".txt in saveValue!");
      return;
    }
  }
  //  if (!file) {
  //    Serial.println(F("Failed to create file"));
  //    return;
  //  }
  // Serial.println("STO SALVANDO!!!");
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["key"] = key;
  doc["value"] = value;
  doc["timestamp"] = timeStamp;

  // Serialize JSON to file
  if (file)
  {
    WriteBufferingStream bufferedFile(file, 64);
    serializeJson(doc, bufferedFile);
    bufferedFile.flush();
    file.println(";");
    // close the file:
    file.close();
  }
  else
  {
    Serial.println(F("Failed to write to file! in saveValue"));
    return;
  }
}

void SDCard::readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\n", path);
  File file = fs.open(path);
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.print("Read from file: ");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();
}

void SDCard::appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.println("Message appended");
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}

void SDCard::deleteFile(fs::FS &fs, const char *path)
{
  Serial.printf("Deleting file: %s\n", path);
  Serial.println(F("save conf 2!"));
  if (fs.remove(path))
  {
    Serial.println("File deleted");
  }
  else
  {
    Serial.println("Delete failed");
  }
}

void SDCard::onAutomaticProcess()
{
  config.onOffLightAuto = true;
  config.onOffHeater = true;
  config.onOffFilter = true;
  // config.onOffEC = true;
  // config.onOffPH = true;
  // config.onOffTemperature = true;
  // config.onOffTemperatureSending = true;
  // config.onOffECSending = true;
  // config.onOffPhSending = true;
  config.autoEnabled = true;
  config.manteinEnabled = false;
}

void SDCard::offAutomaticProcess()
{
  config.onOffLightAuto = false;
  config.onOffHeater = false;
  config.onOffFilter = false;
  config.onOffEC = false;
  config.onOffPH = false;
  config.onOffTemperature = false;
  config.onOffTemperatureSending = false;
  config.onOffECSending = false;
  config.onOffPhSending = false;
  config.autoEnabled = false;
  config.manteinEnabled = true;
}

void SDCard::activeTemperatureMonitoring(boolean value)
{
  config.onOffTemperatureSending = value;
  config.onOffTemperature = value;
}

void SDCard::activeECMonitoring(boolean value)
{
  config.onOffECSending = value;
  config.onOffEC = value;
}

void SDCard::activePHMonitoring(boolean value)
{
  config.onOffPhSending = value;
  config.onOffPH = value;
}
