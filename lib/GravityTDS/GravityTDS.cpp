/***************************************************
 DFRobot Gravity: Analog TDS Sensor/Meter
 <https://www.dfrobot.com/wiki/index.php/Gravity:_Analog_TDS_Sensor_/_Meter_For_Arduino_SKU:_SEN0244>
 
 ***************************************************
 This sample code shows how to read the tds value and calibrate it with the standard buffer solution.
 707ppm(1413us/cm)@25^c standard buffer solution is recommended.
 
 Created 2018-1-3
 By Jason <jason.ling@dfrobot.com@dfrobot.com>
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution.
 ****************************************************/

//#include <EEPROM.h>
#include "GravityTDS.h"
#include "ADC_lut.h"
#include <Preferences.h>
#include <driver/adc.h>


/*Remove a Namespace
In the Arduino implementation of Preferences, there is no method of completely removing a namespace. As a result,
 over the course of several projects, the ESP32 non-volatile storage (nvs) Preferences partition may become full. 
 To completely erase and reformat the NVS memory used by Preferences, create a sketch that contains:

#include <nvs_flash.h>

void setup() {
  nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init(); // initialize the NVS partition.
  while(true);
}

void loop() {
}*/

Preferences ecPreferences;


GravityTDS::GravityTDS(){
    this->pin = T1;
    this->temperature = 25.0;
    this->aref = 3.3;
    this->adcRange = 1024.0;
    this->kValueAddress = 8;
    this->kValue = 1.0;
}

GravityTDS::~GravityTDS(){}

void GravityTDS::setPin(int pin){
	this->pin = pin;
}

void GravityTDS::setTemperature(float temp){
	this->temperature = temp;
}

void GravityTDS::setAref(float value){
	this->aref = value;
}

void GravityTDS::setAdcRange(float range){
      this->adcRange = range;
}

void GravityTDS::setKvalueAddress(int address){
      this->kValueAddress = address;
}

void GravityTDS::begin(){
	//pinMode(this->pin, INPUT);
  adc1_config_width(ADC_WIDTH_BIT_12); 
  adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_6);
   
	readKValues();
}

float GravityTDS::getKvalue(){
	return this->kValue;
}

int GravityTDS::ADCRead(){
  int _a = adc1_get_raw(ADC1_CHANNEL_7);
  int _b = ADC_LUT[_a];
  return _b;
}

int GravityTDS::ADCRead(int analogValue){
  int _b = ADC_LUT[analogValue];
  return _b;
}

void GravityTDS::update(int analogValue){
  this->analogValue = ADCRead(analogValue);
	this->voltage = this->analogValue/this->adcRange*this->aref;
	this->ecValue=(133.42*this->voltage*this->voltage*this->voltage - 255.86*this->voltage*this->voltage + 857.39*this->voltage)*this->kValue;
	this->ecValue25  =  this->ecValue / (1.0+0.02*(this->temperature-25.0));  //temperature compensation
	// Serial.print("AnalogValue: ");
  // Serial.print(this->analogValue);
  // Serial.print(" Voltage: ");
  // Serial.print(this->voltage);
  // Serial.print(" Temperature: ");
  // Serial.print(temperature);
  // Serial.print(" EcValue: ");
  // Serial.println(this->ecValue25);
  //Serial.print(" kValue: ");
  //Serial.println(this->kValue);
	this->tdsValue = ecValue25 * TdsFactor;
	if(cmdSerialDataAvailable() > 0){
    ecCalibration(cmdParse());  // if received serial cmd from the serial monitor, enter into the calibration mode
  }
}

void GravityTDS::update(){
  this->analogValue = ADCRead();
	this->voltage = this->analogValue/this->adcRange*this->aref;
	this->ecValue=(133.42*this->voltage*this->voltage*this->voltage - 255.86*this->voltage*this->voltage + 857.39*this->voltage)*this->kValue;
	this->ecValue25  =  this->ecValue / (1.0+0.02*(this->temperature-25.0));  //temperature compensation
	// Serial.print("AnalogValue: ");
  // Serial.print(this->analogValue);
  // Serial.print(" Voltage: ");
  // Serial.print(this->voltage);
  // Serial.print(" Temperature: ");
  // Serial.print(temperature);
  // Serial.print(" EcValue: ");
  // Serial.println(this->ecValue25);
  //Serial.print(" kValue: ");
  //Serial.println(this->kValue);
	this->tdsValue = ecValue25 * TdsFactor;
	if(cmdSerialDataAvailable() > 0){
    ecCalibration(cmdParse());  // if received serial cmd from the serial monitor, enter into the calibration mode
  }
}

void GravityTDS::beginWithAds(){
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  Serial.println("ADS begin ...");
  // ads.begin(); 
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");  
  }
	readKValues();
}

void GravityTDS::updateWithAds(){
  this->analogValue = ads.readADC_SingleEnded(this->pin); // read the analog value
	this->voltage = ads.computeVolts(this->analogValue);
	this->ecValue=(133.42*this->voltage*this->voltage*this->voltage - 255.86*this->voltage*this->voltage + 857.39*this->voltage)*this->kValue;
	this->ecValue25  =  this->ecValue / (1.0+0.02*(this->temperature-25.0));  //temperature compensation
	Serial.print("AnalogValue: ");
  Serial.print(this->analogValue);
  Serial.print(" Voltage: ");
  Serial.print(this->voltage);
  Serial.print(" Temperature: ");
  Serial.print(temperature);
  Serial.print(" EcValue: ");
  Serial.println(this->ecValue25);
  Serial.print(" kValue: ");
  Serial.println(this->kValue);
	this->tdsValue = ecValue25 * TdsFactor;
	if(cmdSerialDataAvailable() > 0){
    ecCalibration(cmdParse());  // if received serial cmd from the serial monitor, enter into the calibration mode
  }
}

float GravityTDS::getTdsValue(){
	return tdsValue;
}

float GravityTDS::getEcValue(){
      return ecValue25;
}

void GravityTDS::readKValues(){
    ecPreferences.begin(this->ecConf, false);
    this->kValue = ecPreferences.getFloat(this->kValueName, 1.0);
    Serial.print("EC -> kValue: ");
    Serial.println(this->kValue);  
    ecPreferences.end();
}

void GravityTDS::clearAllNamespaceVariables(){
  ecPreferences.clear();
}

void GravityTDS::removeNamespaceVariable(){
  ecPreferences.remove(this->kValueName);
}

boolean GravityTDS::cmdSerialDataAvailable(){
  char cmdReceivedChar;
  static unsigned long cmdReceivedTimeOut = millis();
  while (Serial.available()>0) 
  {   
    if (millis() - cmdReceivedTimeOut > 500U) 
    {
      cmdReceivedBufferIndex = 0;
      memset(cmdReceivedBuffer,0,(ReceivedBufferLengthEC+1));
    }
    cmdReceivedTimeOut = millis();
    cmdReceivedChar = Serial.read();
    if (cmdReceivedChar == '\n' || cmdReceivedBufferIndex==ReceivedBufferLengthEC){
      cmdReceivedBufferIndex = 0;
      upperCase(cmdReceivedBuffer);
      return true;
    }else{
      cmdReceivedBuffer[cmdReceivedBufferIndex] = cmdReceivedChar;
      cmdReceivedBufferIndex++;
    }
  }
  return false;
}

byte GravityTDS::cmdParse(){
  byte modeIndex = 0;
  if(strstr(cmdReceivedBuffer, "ENTER") != NULL) 
      modeIndex = 1;
  else if(strstr(cmdReceivedBuffer, "EXIT") != NULL) 
      modeIndex = 3;
  else if(strstr(cmdReceivedBuffer, "CAL:") != NULL)   
      modeIndex = 2;
  return modeIndex;
}

char *GravityTDS::upperCase(char *str){
  // starting index in a char{} array is always zero, 0 
  int i=0;
  while(str[i]!='\0'){
    int asc = str[i];
    
    // When ASCII value of a space is found 
    if(asc==32){
      // we don't do anything
    }

    // When alphabetic characters are found 
    if(asc>=97 && asc<=122){
      asc = asc - 32;
      str[i] = asc;
    }
    
    // incrementing the index to point to next char value in string 
    i++;
  }	
  return str;
}

void GravityTDS::ecCalibration(byte mode){
    char *cmdReceivedBufferPtr;
    static boolean ecCalibrationFinish = 0;
    static boolean enterCalibrationFlag = 0;
    float KValueTemp,rawECsolution;
    switch(mode)
    {
      case 0:
      if(enterCalibrationFlag)
         Serial.println(F("Command Error"));
      break;
      
      case 1:
      enterCalibrationFlag = 1;
      ecCalibrationFinish = 0;
      Serial.println();
      Serial.println(F(">>>Enter Calibration Mode<<<"));
      Serial.println(F(">>>Please put the probe into the standard buffer solution<<<"));
      Serial.println();
      break;
     
      case 2:
      cmdReceivedBufferPtr=strstr(cmdReceivedBuffer, "CAL:");
      cmdReceivedBufferPtr+=strlen("CAL:");
      rawECsolution = strtod(cmdReceivedBufferPtr,NULL)/(float)(TdsFactor);
      rawECsolution = rawECsolution*(1.0+0.02*(temperature-25.0));
      if(enterCalibrationFlag)
      {
         
          KValueTemp = rawECsolution/(133.42*voltage*voltage*voltage - 255.86*voltage*voltage + 857.39*voltage);  //calibrate in the  buffer solution, such as 707ppm(1413us/cm)@25^c
          Serial.print("KValueTemp:");
          Serial.print(KValueTemp);
          Serial.print(" rawECsolution:");
          Serial.print(rawECsolution);
          Serial.print("  ecvalue:");
          Serial.println(ecValue);
          if((rawECsolution>0) && (rawECsolution<2000) && (KValueTemp>0.25) && (KValueTemp<4.0))
          {
              Serial.println();
              Serial.print(F(">>>Confrim Successful,K:"));
              Serial.print(KValueTemp);
              Serial.println(F(", Send EXIT to Save and Exit<<<"));
              kValue = KValueTemp;
              ecCalibrationFinish = 1;
          }
          else{
            Serial.println();
            Serial.println(F(">>>Confirm Failed,Try Again<<<"));
            Serial.println();
            ecCalibrationFinish = 0;
          }        
      }
      break;

        case 3:
        if(enterCalibrationFlag)
        {
            Serial.println();
            if(ecCalibrationFinish)
            {  
              ecPreferences.begin(this->ecConf, false);
              ecPreferences.putFloat(this->kValueName, kValue);
              ecPreferences.end();
              Serial.print(F(">>>Calibration Successful,K Value Saved"));
            }
            else Serial.print(F(">>>Calibration Failed"));       
            Serial.println(F(",Exit Calibration Mode<<<"));
            Serial.println();
            ecCalibrationFinish = 0;
            enterCalibrationFlag = 0;
        }
        break;
    }
}

void GravityTDS::manualFlashMemoryWrite(float voltage){
    ecPreferences.begin(this->ecConf, false);
    ecPreferences.putFloat(this->kValueName, voltage);
    ecPreferences.end();
    Serial.println(F("EC -> Manual Calibration Successful FlashMemory"));
}

void GravityTDS::manualCalibration(float voltage){
    this->kValue =  voltage;
    Serial.println(F(">>>Manual Valorization Successful")); 
}

