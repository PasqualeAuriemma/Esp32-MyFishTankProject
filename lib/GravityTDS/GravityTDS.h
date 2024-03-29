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

#ifndef GRAVITY_TDS_H
#define GRAVITY_TDS_H

#include "Arduino.h"

#define ReceivedBufferLengthEC 15
#define TdsFactor 0.5  // tds = ec / 2

class GravityTDS{
public:
    GravityTDS();
    ~GravityTDS();
    
    void show();
    void begin();  //initialization
    void update(); //read and calculate
    void beginWithAds();  //initialization
    void updateWithAds(float analogValue, float voltage); //read and calculate
    void setPin(int pin); 
    void setTemperature(float temp);  //set the temperature and execute temperature compensation
    void setAref(float value);  //reference voltage on ADC, default 5.0V on Arduino UNO
    void setAdcRange(float range);  //1024 for 10bit ADC;4096 for 12bit ADC
    void setKvalueAddress(int address); //set the EEPROM address to store the k value,default address:0x08
    float getKvalue(); 
    float getTdsValue();
    float getEcValue();
    char *upperCase(char *str);
    
    /**
     * @fn manualCalibration
     * @brief Manual Calibration of the sensor
     * @param voltage     : Voltage value
     * @return Upserting variables neutral or acid
     */
    void manualCalibration(float voltage);
    
    /**
     * @fn manualEEPROMWrite
     * @brief Manual writing with Calibration values of the sensor
     * @param voltage     : Voltage value
     * @return Upserting of the Flash Memory
     */
    void manualFlashMemoryWrite(float voltage);

    /**
     * @fn clearAllNamespaceVariables
     * @brief Clearing all the namespace's variables
     * @return Namespace cleaned
     */
    void clearAllNamespaceVariables();

    /**
     * @fn removeNamespaceVariable
     * @brief Removing namespace's tds kValue 
     * @return Namespace without kValue variable
     */
    void removeNamespaceVariable();

private:
    
    int pin;
    float aref;  // default 5.0V on Arduino UNO
    float adcRange;
    float temperature;
    int kValueAddress;     //the address of the K value stored in the EEPROM
    char cmdReceivedBuffer[ReceivedBufferLengthEC+1];   // store the serial cmd from the serial monitor
    byte cmdReceivedBufferIndex;
    const char* kValueName = "kValue";
    const char* ecConf = "tds-conf";
    float kValue;      // k value of the probe,you can calibrate in buffer solution ,such as 706.5ppm(1413us/cm)@25^C 
    float analogValue;
    float voltage;
    float ecValue; //before temperature compensation
    float ecValue25; //after temperature compensation
    float tdsValue;
    
    void readKValues();
    boolean cmdSerialDataAvailable();
    byte cmdParse();
    void ecCalibration(byte mode);
    int ADCRead();
    int ADCRead(int analogValue);
    void update(int analogValue);
};  

#endif
