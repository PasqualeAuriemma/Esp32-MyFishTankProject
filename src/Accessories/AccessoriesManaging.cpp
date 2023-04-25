/*
 * Aquarium Project Pasquale
   Manual calibration: neutral:1496.48,acid:1948.57
   
 */

#include "AccessoriesManaging.h"

// GPIO where the DS18B20 is connected to
const int oneWireBus = 13;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature thermometerSensor(&oneWire);

Accessories::Accessories(int tdsPin, int phPin, int enableTdsPin, int enablePhPin)
{

  Serial.println("Init Accessories!");
  // inizialize ph class
  this->phPin = phPin;
  pinMode(this->phPin, INPUT);
  // ph.manualFlashMemoryWrite(acid, 1948.57); // if you want to add neutral value in Falsh memory manually
  // ph.manualFlashMemoryWrite(neutral, 1496.48); // if you want to add neutral value in Falsh memory manually
  ph.begin();

  // inizialize ec class
  this->tdsPin = 0; // pin ADS1115
  gravityTds.setPin(this->tdsPin);
  gravityTds.setAref(5);        // reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(4096); // 1024 for 10bit ADC;4096 for 12bit ADC
  //gravityTds.manualFlashMemoryWrite(0.89); //0.47 if you want to add neutral value in Falsh memory manually
  gravityTds.beginWithAds(); // initialization

  // Thermometer init
  thermometerSensor.begin();
  setFirstTimePH(true);
  setFirstTimeEC(true);

  // set Mosfet pins
  pinMode(enableTdsPin, OUTPUT);

  digitalWrite(enableTdsPin, HIGH);
  pinMode(enablePhPin, OUTPUT);
  digitalWrite(enablePhPin, HIGH);
}

float Accessories::getEC_DF(float temperature)
{
  gravityTds.setTemperature(temperature); // set the temperature and execute temperature compensation
  gravityTds.updateWithAds();                    // sample and calculate
  // tdsValue = gravityTds.getTdsValue();  // then get the value
  // read the analog value and store into the buffer
  analogBuffer[analogBufferIndex] = gravityTds.getEcValue(); // read the analog value and store into the buffer
  analogBufferIndex++;
  if (analogBufferIndex == SCOUNT)
  {
    analogBufferIndex = 0;
  }

  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U)
  { // time interval: 1s
    timepoint = millis();
    for (copyIndexEC = 0; copyIndexEC < SCOUNT; copyIndexEC++)
    {
      analogBufferTemp[copyIndexEC] = analogBuffer[copyIndexEC];
    }
    ecValue = getMedianNum(analogBufferTemp, SCOUNT);
  }
  Serial.print(ecValue); Serial.println(" uS/cm");
  return ecValue;
}

float Accessories::getEC_KeyStudio(float temperature)
{
  analogBuffer[analogBufferIndex] = analogRead(this->tdsPin); // read the analog value and store into the buffer
  analogBufferIndex++;
  if (analogBufferIndex == SCOUNT)
  {
    analogBufferIndex = 0;
  }
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 1000U)
  {
    if (averageIndex == SCOUNT)
    {
      averageIndex = 0;
    }
    printTimepoint = millis();
    for (copyIndexEC = 0; copyIndexEC < SCOUNT; copyIndexEC++)
    {
      analogBufferTemp[copyIndexEC] = analogBuffer[copyIndexEC];
    }
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 4096.0;                                                                                           // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);                                                                                                        // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient;                                                                                                     // temperature compensation
    ecValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge); // convert voltage value to ec value with european factor
    ecValueAverage[averageIndex] = ecValue;

    averageIndex++;
    //    Serial.print("voltage:");
    //    Serial.print(averageVoltage);
    //    Serial.print(" temperature:");
    //    Serial.print(temperature,1);
    //    Serial.print("  EC:");
    //    Serial.println(getAverage(ecValueAverage, SCOUNT),2);

    tdsValueKS = getAverage(ecValueAverage, SCOUNT);
  }
  return tdsValueKS;
}

float Accessories::getAverage(float bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int j;
  float sum = 0.0;
  for (j = 0; j < iFilterLen; j++)
  {
    sum += bTab[j];
  }
  return sum / iFilterLen;
}

int Accessories::getAverage(int bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int j;
  float sum = 0.0;
  for (j = 0; j < iFilterLen; j++)
  {
    sum += bTab[j];
  }
  return sum / iFilterLen;
}

int Accessories::getMedianNum(int bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}

bool Accessories::manageAcquarioumHeater(int temperature, byte min, byte max)
{
  if (temperature <= min && !tempOnOffHeater &&
      temperature != -1000 && temperature != 0)
  {
    tempOnOffHeater = true;
    return tempOnOffHeater;
  }
  else if ((temperature > max && tempOnOffHeater))
  {
    tempOnOffHeater = false;
    return tempOnOffHeater;
  }
  else
  {
    return tempOnOffHeater;
  }
}

float Accessories::getTemperatureDS18B20(float value)
{
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 15000U || millis() - timepoint < 200U)
  {
    timepoint = millis();
    thermometerSensor.requestTemperatures();
    float temperatureC = thermometerSensor.getTempCByIndex(0);
    // Serial.print(temperatureC);
    // Serial.println("ºC");
    return temperatureC;
  }
  else
  {
    return value;
  }
}

float Accessories::getAnalogTemperature()
{
  byte data[12];
  byte addr[8];
  if (!oneWire.search(addr))
  {
    // no more sensors on chain, reset search
    oneWire.reset_search();
    return -1000;
  }
  if (OneWire::crc8(addr, 7) != addr[7])
  {
    Serial.print("CRC is not valid!");
    return -1000;
  }
  if (addr[0] != 0x10 && addr[0] != 0x28)
  {
    Serial.print("Device is not recognized");
    return -1000;
  }
  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0x44, 1); // start conversion, with parasite power on at the end
                          ////byte present = ds.reset();
  oneWire.select(addr);
  oneWire.write(0xBE); // Read Scratchpad
  for (int i = 0; i < 9; i++)
  { // we need 9 bytes
    data[i] = oneWire.read();
  }
  oneWire.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB); // using two's compliment
  float TemperatureSum = tempRead / 16;
  return TemperatureSum;
}

float Accessories::getPH1(float temperature)
{
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U)
  { // time interval: 1s
    timepoint = millis();
    voltagePH = analogRead(this->phPin) / 1024.0 * 5000; // read the ph voltage
    phValue = ph.readPH(voltagePH, temperature);         // convert voltage to pH with temperature compensation
    // Serial.print("pH:");
    // Serial.print(phValue,2);
    char cmd[10];
    if (readSerial(cmd))
    {
      upperCase(cmd);
      if (strstr(cmd, "PH"))
      {
        ph.calibration(voltagePH, temperature, cmd); // PH calibration process by Serail CMD
      }
    }
  }
  return phValue;
}

float Accessories::getPH(float temperature)
{
  analogBufferPH[analogBufferIndexPH] = analogRead(this->phPin); // read the analog value and store into the buffer
  analogBufferIndexPH++;
  if (analogBufferIndexPH == SCOUNT)
  {
    analogBufferIndexPH = 0;
  }
  static unsigned long timepoint1 = millis();
  if (millis() - timepoint1 > 1000U)
  { // time interval: 1s
    timepoint1 = millis();
    for (copyIndexPH = 0; copyIndexPH < SCOUNT; copyIndexPH++)
    {
      analogBufferTempPH[copyIndexPH] = analogBufferPH[copyIndexPH];
    }
    voltage = getMedianNum(analogBufferTempPH, SCOUNT) / 4095.0 * 3300; // read the analog value more stable by the median filtering algorithm, and convert to voltage real value
    phValue = ph.readPH(voltage, temperature);                          // convert voltage to pH with temperature compensation

    Serial.print("PH -> voltage:");
    Serial.print(voltage);
    Serial.print(" temperature:");
    Serial.print(temperature, 1);
    Serial.print(" pH:");
    Serial.println(phValue, 2);

    char cmd[10];
    if (readSerial(cmd))
    {
      upperCase(cmd);
      if (strstr(cmd, "PH"))
      {
        ph.calibration(voltage, temperature, cmd); // PH calibration process by Serail CMD
      }
    }
  }
  // ph.calibration(voltage, temperature);
  return phValue;
}

bool Accessories::readSerial(char result[])
{
  while (Serial.available() > 0)
  {
    char inChar = Serial.read();
    if (inChar == '\n')
    {
      result[p] = '\0';
      Serial.flush();
      p = 0;
      return true;
    }
    if (inChar != '\r')
    {
      result[p] = inChar;
      p++;
    }
    delay(1);
  }
  return false;
}

char *Accessories::upperCase(char *str)
{
  // starting index in a char{} array is always zero, 0
  int i = 0;
  while (str[i] != '\0')
  {
    int asc = str[i];

    // When ASCII value of a space is found
    if (asc == 32)
    {
      // we don't do anything
    }

    // When alphabetic characters are found
    if (asc >= 97 && asc <= 122)
    {
      asc = asc - 32;
      str[i] = asc;
    }

    // incrementing the index to point to next char value in string
    i++;
  }
  return str;
}

bool Accessories::manageAquariumLights(int h, int m, int startHour, int endHour, int startMinutes, int endMinutes)
{

  int checkStart = 0, checkEnd = 0, tempTime = 0;

  if ((startHour > endHour) || (startHour == endHour &&
                                startMinutes > endMinutes))
  {
    checkStart = (startHour * 60) + startMinutes;
    checkEnd = (endHour * 60 + (24 * 60)) + endMinutes;
    if (h < endHour || (h == endHour && m <= endMinutes))
    {
      tempTime = ((h * 60) + (24 * 60)) + m;
    }
    else
    {
      tempTime = (h * 60) + m;
    }
  }
  else
  {
    checkStart = (startHour * 60) + startMinutes;
    checkEnd = (endHour * 60) + endMinutes;
    tempTime = (h * 60) + m;
  }

  if (checkStart <= tempTime && checkEnd > tempTime && !tempOnOffLight)
  {
    tempOnOffLight = true;
    return tempOnOffLight;
  }
  else if (checkEnd == tempTime && tempOnOffLight)
  {
    tempOnOffLight = false;
    return tempOnOffLight;
  }
  else
  {
    return tempOnOffLight;
  }

  //  Serial.println("------------------------------");
  //  Serial.println("orario timer per illuminazione");
  //  Serial.println("ORA ACCENSIONE = " + String(startHour));
  //  Serial.println("MINUTI ACCENSIONE = " + String(config.startMinutes));
  //  Serial.println("ORA SPEGNIMENTO = " + String(config.endHour));
  //  Serial.println("MINUTI SPEGNIMENTO = " + String(config.endMinutes));
  //  Serial.println("------------------------------");
}

bool Accessories::checkIfGetMeasurement(byte pivot, byte selectedMinute, byte hours, byte minutes, byte duration)
{
  if (hours % pivot == 0 && minutes >= selectedMinute && minutes <= (selectedMinute + duration))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Accessories::activeSensorEC(int pin, bool onOff)
{
  if (onOff && getFirstTimeEC())
  {
    digitalWrite(pin, LOW);
    setFirstTimeEC(false);
  }
  else if (!onOff && !firstTimeEC)
  {
    digitalWrite(pin, HIGH);
    firstTimeEC = true;
  }
}

void Accessories::activeSensorPH(int pin, bool onOff)
{
  if (onOff && getFirstTimePH())
  {
    digitalWrite(pin, LOW);
    setFirstTimePH(false);
  }
  else if (!onOff && !getFirstTimePH())
  {
    digitalWrite(pin, HIGH);
    setFirstTimePH(true);
  }
}

bool Accessories::chackIfSendValueToWeb(byte pivot, byte selectedMinute, float value, String key, byte hours, byte minutes, byte seconds)
{
  if (key == keyTemp && value != -1000.00 && value != 0.0)
  {
    if (hours % pivot == 0 && minutes == selectedMinute && seconds == int(hours / pivot) && seconds != previousSecSend)
    {
      previousSecSend = seconds;
      return true;
    }
    else
    {
      return false;
    }
  }
  else if (key == keyEC && int(value) != 1)
  {
    if (hours % pivot == 0 && minutes == selectedMinute && seconds == int(hours / pivot) && seconds != previousSecSendEC)
    {
      previousSecSendEC = seconds;
      return true;
    }
    else
    {
      return false;
    }
  }
  else if (key == keyPh && int(value) != 1)
  {
    if (hours % pivot == 0 && minutes == selectedMinute && seconds == int(hours / pivot) && seconds != previousSecSendPH)
    {
      previousSecSendPH = seconds;
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}