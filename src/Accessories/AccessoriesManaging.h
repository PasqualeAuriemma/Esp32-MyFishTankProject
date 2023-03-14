/*
 * Aquarium Project Pasquale
 * To clear the Epprom cache when the sensors give nan or inf or OVF or 0 values
 * ---------------------  PH   -------------------------
 * #include <EEPROM.h>
 * #define TDSADDR 0x08
 * void setup()
 * {
 * Serial.begin(115200);
 * for(int i = 0;i < 8; i++ ){
 *   EEPROM.write(TDSADDR+i, 0xFF);// write defaullt value to the EEPROM
 *   delay(10);
 *  }
 * }
 * void loop(){
 *  static int a = 0, value = 0;
 *  value = EEPROM.read(TDSADDR+a);
 *  Serial.print(TDSADDR+a,HEX);
 *  Serial.print(":");
 *  Serial.print(value);// print the new value of EEPROM block used by TDS meter. The correct is 255. 
 *  Serial.println();
 *  delay(10);
 *  a = a + 1;
 *  if (a == 8)
 *   while(1);
 * }
 * ---------------------  TDS   ------------------------
 * #include <EEPROM.h>
 * #define TDSADDR 0x08
 * void setup(){
 *  Serial.begin(115200);
 *  for(int i = 0;i < 8; i++ ){
 *   EEPROM.write(TDSADDR+i, 0xFF);// write defaullt value to the EEPROM
 *   delay(10);
 *  }
 * }
 * void loop(){
 *  static int a = 0, value = 0;
 *  value = EEPROM.read(TDSADDR+a);
 *  Serial.print(TDSADDR+a,HEX);
 *  Serial.print(":");
 *  Serial.print(value);// print the new value of EEPROM block used by TDS meter. The correct is 255. 
 *  Serial.println();
 *  delay(10);
 *  a = a + 1;
 *  if (a == 8)
 *   while(1);
 * }
 */

#ifndef AccessoriesManaging_H_
#define AccessoriesManaging_H_

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DFRobot_PH.h"
#include <string.h>
#include "GravityTDS.h"
#define VREF 5.00// analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point


class Accessories {

  private:
    GravityTDS gravityTds;
    DFRobot_PH ph;
    int phPin = 34;
    int tdsPin = 35;
    bool tempOnOffHeater = false;
    bool tempOnOffLight = false;
    bool firstTimePH;
    bool firstTimeEC;
    byte previousSecSend = 55; //random
    byte previousSecSendEC = 55; //random
    byte previousSecSendPH = 55; //random
    byte previousSecResend = 55; //random
    const char *neutral = "NEUTRAL";
    const char *acid = "ACID";
    const char *keyTemp = "Temp";
    const char *keyEC = "Ec";
    const char *keyPh = "Ph";
    float ecValueAverage[SCOUNT];  // store the EC values in the array to get an average value
    int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
    int analogBufferTemp[SCOUNT];
    int analogBufferIndex = 0, averageIndex = 0, copyIndexEC = 0;
    float averageVoltage = 0, ecValue= 0;
    float voltageEC;
    float tdsValue = 0.0;
    float tdsValueKS = 0.0;
    int analogBufferPH[SCOUNT]; // store the analog value in the array, read from ADC
    int analogBufferTempPH[SCOUNT];
    int analogBufferIndexPH = 0, copyIndexPH = 0;
    float averageVoltagePH = 0;
    float averageVoltageEC = 0;
    float voltagePH;
    float phValue= 0.0;
    float voltage;
    int p = 0;
  
  public:
    Accessories(int tdsPin, int phPin, int enableTdsPin, int enablePhPin);
    Accessories(){}
    ~Accessories() {}
    
    void activeSensorEC(int pin, bool onOff);

    void activeSensorPH(int pin, bool onOff);

    bool getFirstTimePH(){return this->firstTimePH;}

    bool getFirstTimeEC(){return this->firstTimeEC;}   

    void setFirstTimePH(bool value){this->firstTimePH = value;}

    void setFirstTimeEC(bool value){firstTimeEC = value;}   

    //Getting the temperature from one DS18B20 in DEG Celsius
    float getTemperatureDS18B20(float value);

    float getAnalogTemperature();

    float getEC_KeyStudio(float temperature);

    /****************************************************
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
    ******************************************************
    
     - pio device monitor --eol=CRLF --baud=115200 --echo --filter send_on_enter 
    
    ***********Notice and Trouble shooting****************
     1. This code is tested on Arduino Uno with Arduino IDE 1.0.5 r2 and 1.8.2.
     2. Calibration CMD:
        enter -> enter the calibration mode
        cal:tds value -> calibrate with the known tds value(25^c). e.g.cal:707
        exit -> save the parameters and exit the calibration mode
    ******************************************************/
    
    //Getting the EC from KS0429 Meter V.1 
    float getEC_DF(float temperature);
    
    /**
     * @fn getPH
     * @brief Getting the PH from Gravity: Analog pH Sensor / Meter Pro Kit V2
     * file DFRobot_PH.ino
     * @ https://github.com/DFRobot/DFRobot_PH
     *
     * This is the sample code for Gravity: Analog pH Sensor / Meter Kit V2, SKU:SEN0161-V2
     * In order to guarantee precision, a temperature sensor such as DS18B20 is needed, to execute automatic temperature compensation.
     * You can send commands in the serial monitor to execute the calibration.
     * 
     *  - pio device monitor --eol=CRLF --baud=115200 --echo --filter send_on_enter 
     * 
     * Serial Commands:
     *   enterph -> enter the calibration mode
     *   calph   -> calibrate with the standard buffer solution, two buffer solutions(4.0 and 7.0) will be automaticlly recognized
     *   exitph  -> save the calibrated parameters and exit from calibration mode
     *
     * Copyright   [DFRobot](http://www.dfrobot.com), 2018
     * Copyright   GNU Lesser General Public License
     *
     * version  V1.0
     * date  2018-04 
     * @return PH value
     */
    float getPH(float temperature);

    //Getting the PH from Gravity: Analog pH Sensor / Meter Pro Kit V2
    float getPH1(float temperature);

    char *upperCase(char *str);
    int getMedianNum(int bArray[], int iFilterLen);
    float getAverage(float bArray[], int iFilterLen);
    int getAverage(int bArray[], int iFilterLen);
    bool readSerial(char result[]);

    //Checking when turn on or turn off the thermometer into aquarioum
    bool manageAcquarioumHeater(int temperature, byte min, byte max);

    //Managing the right time to turn on the light in the aquarium.
    //Compare the turning on lights time with the real time in minutes to turn on or turn off the
    //light of the fish tank. If the start time is greater than the end time, it will  be added a
    //day in minutes to end time and real time in order to match the end time with the next day.
    bool manageAquariumLights(int h, int m, int startHour, int endHour, int startMinutes, int endMinutes);

    //The function argument pivot is very important because it decides how many times to send
    //the value to web site. For example, if the pivot is four the value will be sent to web
    //site eight time. The seconds should be equal to result of division between hour
    //and pivot but different from last second when it is sent the value. The division and
    //the last second value are used to run the function once per second, because the function
    //is called four time in a second.  The result is to choose the right time to sent the
    //value on the web site. 
    //Check if it is the right time range to get EC values
    bool checkIfGetMeasurement(byte pivot, byte selectedMinute, byte hours, byte minutes, byte duration); 
    
    bool chackIfSendValueToWeb(byte pivot, byte selectedMinute, float value, String key, byte hours, byte minutes, byte seconds);
    
};

#endif