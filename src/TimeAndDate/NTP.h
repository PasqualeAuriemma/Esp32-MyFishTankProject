/*
  Aquarium Project Pasquale
*/
#ifndef NTP_h
#define NTP_h

#include "TimeAndDateManaging.h"

class Ntp{
  private:
    const char* ntpServer = "pool.ntp.org";
    long gmtOffset_sec;
    int daylightOffset_sec;
    byte previousSec = 0;
  
  public:     
    Ntp(long _gmtOffset_sec, int _daylightOffset_sec){
      this->daylightOffset_sec = _daylightOffset_sec;
      this->gmtOffset_sec = _gmtOffset_sec;
    } 
  
    void setTime(){
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
    }
 
    int* getTime(int *time){
      struct tm timeinfo;
      if(!getLocalTime(&timeinfo)){
        time[0] = 0;
        time[1] = 0;
        time[2] = 0;
        return time;
      }else{
        if (timeinfo.tm_sec != previousSec) {
          time[0] = timeinfo.tm_hour;
          time[1] = timeinfo.tm_min;
          time[2] = timeinfo.tm_sec;
          previousSec = timeinfo.tm_sec;
        }  
        return time;
      }
    }

    int getSecond(){
      struct tm timeinfos;
      if(!getLocalTime(&timeinfos)){
        return 0;
      }else{
        return timeinfos.tm_sec;
      }
    }

    int getMinute(){
      struct tm timeinfos;
      if(!getLocalTime(&timeinfos)){
        return 0;
      }else{
        return timeinfos.tm_min;
      }
    }

    int getTimeStamp(){
      time_t seconds; 
      // Stores time seconds from 1970
      time(&seconds);
      return seconds;
    }

};


#endif