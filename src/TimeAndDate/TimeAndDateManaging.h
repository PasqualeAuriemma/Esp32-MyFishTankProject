#ifndef TimeAndDateManaging_H_
#define TimeAndDateManaging_H_

#include <Arduino.h>
#include <RTClib.h>

// Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      // Serial.print("Day of week: ");
      // Serial.println(&timeinfo, "%A");
      // Serial.print("Month: ");
      // Serial.println(&timeinfo, "%B");
      // Serial.print("Day of Month: ");
      // Serial.println(&timeinfo, "%d");
      // Serial.print("Year: ");
      // Serial.println(&timeinfo, "%Y");
      // Serial.print("Hour: ");
      // Serial.println(&timeinfo, "%H");
      // Serial.print("Hour (12 hour format): ");
      // Serial.println(&timeinfo, "%I");
      // Serial.print("Minute: ");
      // Serial.println(&timeinfo, "%M");
      // Serial.print("Second: ");
      // Serial.println(&timeinfo, "%S");
      
      // Serial.println("Time variables");
      // char timeHour[3];
      // strftime(timeHour,3, "%H", &timeinfo);
      // Serial.println(timeHour);
      // char timeWeekDay[10];
      // strftime(timeWeekDay,10, "%A", &timeinfo);
      // Serial.println(timeWeekDay);
      // Serial.println();

class TimeAndDate {

  private: 
    RTC_DS3231 RTC;
    const char* ntpServer = "pool.ntp.org";
    long  gmtOffset_sec;
    int   daylightOffset_sec;
    byte previousSec = 0;
        
  public:

    TimeAndDate(){
      RTC.begin();
      if (!RTC.begin()) {
        Serial.println("Couldn't find RTC!");
        RTC.adjust(DateTime(2003, 9, 18, 18, 0, 0));
      }
      if (RTC.lostPower()) {
        Serial.println("RTC lost power, lets set the time!");
        // following line sets the RTC to the date & time this sketch was compiled
        RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
      }
    };  

    TimeAndDate(long _gmtOffset_sec, int _daylightOffset_sec){
      this->daylightOffset_sec = _daylightOffset_sec;
      this->gmtOffset_sec = _gmtOffset_sec;
      Serial.println("Set time with NTP server.");
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      struct tm timeinfo;
      if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
      }else{
        Serial.println("-----------------------------------------");
        Serial.println(&timeinfo, "--- %A, %B %d %Y %H:%M:%S ---");
        Serial.println("-----------------------------------------");
      }
      
      int year = 1900 + timeinfo.tm_year;
      int month = 1 + timeinfo.tm_mon;
      RTC.begin();
      if (!RTC.begin()) {
        Serial.println("Couldn't find RTC!");
        RTC.adjust(DateTime(2003, 9, 18, 18, 0, 0));
      }else{
        RTC.adjust(DateTime(year, month, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
      }
    };  

    ~TimeAndDate() {}

    int* getTime(int *time){
      //Getting time from RTC Modul
      DateTime now = RTC.now();
      if (now.second() != previousSec) {
        time[0] = now.hour();;
        time[1] = now.minute();
        time[2] = now.second();
        previousSec = now.second();
      }  
      return time;
    }
    
    int getSecond(){
      DateTime now = RTC.now();
      return now.second();
    }

    int getMinute(){
      return RTC.now().minute();
    }

    int getHour(){
      return RTC.now().hour();
    }

    int getDay(){ 
      return RTC.now().day();
    }

    int getYear(){
      return RTC.now().year();
    } 
    
    int getMonth(){
      return (RTC.now().month());
    }  

    const char *getMonthName(){
      return monthName(getMonth());
    }  
    
    void setTimeAndData(const char* dateBuffer, const char *timeBufferSub){
      Serial.println(dateBuffer);
      Serial.println(timeBufferSub);
      RTC.adjust(DateTime(dateBuffer, timeBufferSub));

    }
    
    const char *monthName(int m) {
      switch (m) {
        case 1: return "Jan";
        case 2: return "Feb";
        case 3: return "Mar";
        case 4: return "Apr";
        case 5: return "May";
        case 6: return "Jun";
        case 7: return "Jul";
        case 8: return "Aug";
        case 9: return "Sep";
        case 10: return "Oct";
        case 11: return "Nov";
        case 12: return "Dec";
        default: return "Jan";
      }
    }

    int getTimeStamp(){ return RTC.now().unixtime();}

    int getDaylightOffset_sec(){ return daylightOffset_sec;}

    long getGmtOffset_sec(){ return gmtOffset_sec;}
    
    void setDaylightOffset_sec(int value){ this->daylightOffset_sec = value;}
    
    void setGmtOffset_sec(long value){ this->gmtOffset_sec = value;}
};

#endif