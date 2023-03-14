/*
  Aquarium Project Pasquale
*/
#ifndef LCD_h
#define LCD_h

#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include "Screen.h"

// All of the backpacks like the one shown are at 0x27.
#define I2C_ADDR   0x27 // <--Change to match your display. Use scanner.ino to find address.

LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);

class Lcd: public Screen{
  private:
    byte startReleSimbolsOnLCD = 11;
    String activationString = "Would activate?";
    String enableString = "Would enable?";
    String chooseString = "Choose ON o OFF";
    String waitingString = "Waiting...";
    String siString = "YES";
    String noString = "NO";
    String onString = "ON";
    String offString = "OFF";
    uint8_t SPENTO[8] = {0x0E, 0x11, 0x11, 0x11, 0x0A, 0x0E, 0x0E, 0x04,};
    uint8_t ACCESO[8] = {0x0E, 0x1F, 0x1F, 0x1F, 0x0E, 0x0E, 0x0E, 0x04,};
    byte gradi[8] = {B01110, B01010, B01110, B00000, B00000, B00000, B00000, B00000};
    byte Smile[8] = {B00000, B10001, B00000, B00000, B10001, B01110, B00000,};
    byte Freccia_su[8] = {B00100, B00100, B01110, B01110, B01110, B11111, B11111, B11111,};
    byte Freccia_lato_sx[8] = {B10000, B11000, B11110, B11111, B11111, B11110, B11000, B10000,};
    byte Freccia_lato_dx[8] = {B00001, B00011, B01111, B11111, B11111, B01111, B00011, B00001,};
    byte V[8] = {B00001, B00001, B00001, B00001, B00001, B1001, B01100, B00100,};
    byte vuoto[8] = {B00000, B00000, B00000, B00000, B00000, B0000, B00000, B00000,};
    byte off[8] = {B11111, B10001, B10001, B10001, B10001, B10001, B11111, B00000,};
    byte on[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B00000,};
    byte g_off[8] = {B00100, B01110, B01110, B01110, B00100, B00100, B00100, B00100,};
    byte g_on[8] = {B00100, B00100, B00100, B00100, B01110, B01110, B01110, B00100,};
  public:   
    Lcd(int okButton){
      
      //Initialize Display LCD
      lcd.init();
      //Setting lcd special characters
      lcd.createChar(0, SPENTO);
      lcd.createChar(1, ACCESO);
      lcd.createChar(2, gradi);
      lcd.createChar(3, Smile);
      lcd.createChar(4, Freccia_su);
      lcd.createChar(5, Freccia_lato_dx);
      lcd.createChar(6, Freccia_lato_sx);
      
      this->okButton = okButton;
    }

    ~Lcd() {}

    void initScreen() override{
      lcd.backlight();
      lcd.clear();
      //Set cursor at up left corner + 3 cells.
      lcd.setCursor (3, 0);
      lcd.print("Acquario di");
      //Set cursor at bottom left corner + 4 cells.
      lcd.setCursor ( 4, 1 );
      lcd.print("Pasquale");
      //Setting delay
      delay(4000);
      //Turning off screen backlight
      lcd.noBacklight();
      lcd.clear();
    }
    
    // LCD screen with time, temperature, ec and ph
    void mainScreen(char* _time, float temperature, float ec, float phFinal) override{
      lcd.setCursor(0, 0); lcd.print(_time);
      lcd.setCursor (0, 1); lcd.print(phFinal);
      lcd.setCursor (3, 1); lcd.print(" ");
      lcd.setCursor (4, 1); lcd.print(int(ec));
      lcd.setCursor (7, 1); lcd.print("uS/cm");
      lcd.setCursor (12, 1); lcd.print(" ");
      lcd.setCursor (13, 1); lcd.print(int(temperature));
      lcd.setCursor (15, 1); lcd.write(byte(2));
    }

    // Show the rele symbols on screen LCD
    void showReleSymbol(bool *rele, byte num) override{
      lcd.clear();
      lcd.setCursor(startReleSimbolsOnLCD, 0);
      for (int p = 0; p < num; p++) {
        if(!rele[p]){
          lcd.write(1);
        }else{
          lcd.write(0);
        }
      }
      delay(500);
    }

    void showMonitoring(const char *key, float value, float temperature) override{
      lcd.clear();
      lcd.setCursor(1, 0); lcd.print(key);
      lcd.setCursor (4, 0); lcd.print("Monitoring");
      lcd.setCursor(0, 1); lcd.print(int(temperature));
      lcd.setCursor (3, 1); lcd.write(byte(2));
      lcd.setCursor(6, 1); lcd.print(value);
      lcd.setCursor (13, 1); lcd.print("<-");
      lcd.setCursor (15, 1);
      lcd.write(6);
    }

    void turnOnLight() override{
      lcd.backlight(); 
    }

    void turnOffLight() override{
      lcd.noBacklight();
    }

    //Showing menu page where there are the options to choose
    void showYesOrNoSelection1(byte selectedItem, int rowNumber, String *listMenu) override{
      lcd.clear();
      //  Set cursor at up left corner.
      lcd.setCursor(0, 0);
      lcd.print(chooseString);
      //  Set cursor at bottom left corner + 2 cells or 6 cells and 9 cells or 13 cells.
      lcd.setCursor((selectedItem * 7) + 2, 1); lcd.write(byte(6));
      lcd.setCursor((selectedItem * 7) + 6, 1); lcd.write(byte(5));
      //  Set cursor at bottom left corner + 3 cells.
      lcd.setCursor(3, 1); lcd.print(onString);
      //  Set cursor at bottom left corner + 8 cells.
      lcd.setCursor(10, 1); lcd.print(offString);     
    }

    //Showing connection status on screen
    void connectionStatusPage(const char* connStatus) override{
      lcd.clear();
      lcd.setCursor(1, 0); lcd.print("Conn. Status:");
      lcd.setCursor(1, 1); lcd.print(connStatus);
      lcd.setCursor(10,1); lcd.print(offString);  
    }
  protected:    
    //Showing menu page where there are the options to choose 5
    void showYesOrNoSelection2(byte selectedItem, int rowNumber, String *listMenu) override{
      lcd.clear();
      //Set cursor at up left corner + 1 cells.
      lcd.setCursor(1, 0);
      lcd.print(activationString);
      //  Set cursor at bottom left corner + 2 cells or 7 cells and 5 cells or 10 cells.
      lcd.setCursor((selectedItem * 5) + 2, 1); lcd.write(byte(6));
      lcd.setCursor(((selectedItem + 1) * 5), 1); lcd.write(byte(5));
      //  Set cursor at bottom left corner + 3 cells.
      lcd.setCursor(3, 1); lcd.print(siString);
      //  Set cursor at bottom left corner + 8 cells.
      lcd.setCursor(8, 1); lcd.print(noString);
    }

    //Showing menu page where there are the options to choose 9
    void showYesOrNoSelection3(byte selectedItem, int rowNumber, String *listMenu) override{
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(enableString);
      //  Set cursor at bottom left corner + 2 cells or 7 cells and 5 cells or 10 cells.
      lcd.setCursor((selectedItem * 5) + 2, 1); lcd.write(byte(6));
      lcd.setCursor(((selectedItem + 1) * 5), 1); lcd.write(byte(5));
      //  Set cursor at bottom left corner + 3 cells.
      lcd.setCursor(3, 1); lcd.print(siString);
      //  Set cursor at bottom left corner + 8 cells.
      lcd.setCursor(8, 1); lcd.print(noString);
    }

    //Showing menu page where there are the options to choose 10
    void showYesOrNoSelection4(byte selectedItem, int rowNumber, bool onOffTemp, bool onOffEc, bool onOffPh) override{
      lcd.clear();
      lcd.setCursor(0, 0);
      //Serial.println(row);
      if (rowNumber == 10) {
        lcd.print("Send Temp is");
        lcd.setCursor(13, 0);
        lcd.print(enabledToSend(onOffTemp));
      } else if (rowNumber == 11) {
        lcd.print("Send EC is");
        lcd.setCursor(13, 0);
        lcd.print(enabledToSend(onOffEc));
      } else if (rowNumber == 12) {
        lcd.print("Send PH is");
        lcd.setCursor(13, 0);
        lcd.print(enabledToSend(onOffPh));
      }
      lcd.setCursor((selectedItem * 7) + 2, 1); lcd.write(byte(6));
      lcd.setCursor((selectedItem * 7) + 6, 1); lcd.write(byte(5));
      //  Set cursor at bottom left corner + 3 cells.
      lcd.setCursor(3, 1); lcd.print(onString);
      //  Set cursor at bottom left corner + 8 cells.
      lcd.setCursor(10, 1); lcd.print(offString);
    }
    
    //Used to get the right status in order to insert into LCD Screen
    String enabledToSend(bool item) {
      if (item) {
        return "ON";
      } else {
        return "OFF";
      }
    }

    // Showing row item with selecting symbol in circle way
    void showListMenu(int listSize, int rowItem, String *listMenu) override{
      if (listSize % 2 == 0) {
        if (rowItem % 2 == 0) {
          menuPage(listMenu[rowItem], listMenu[rowItem + 1], 1);
        } else {
          menuPage(listMenu[rowItem - 1], listMenu[rowItem], 2);
        }
      } else {
        if (rowItem == listSize - 1) {
          menuPage(listMenu[rowItem], listMenu[0], 1);
        } else if (rowItem % 2 == 0) {
          menuPage(listMenu[rowItem], listMenu[rowItem + 1], 1);
        } else {
          menuPage(listMenu[rowItem - 1], listMenu[rowItem], 2);
        }
      }
    }  

    //Showing menu page with the row items and their selecting symbol
    void menuPage(String menuItem1, String menuItem2, int arrowItem) {
      lcd.clear();
      //Set cursor at up or bottom left corner.
      lcd.setCursor(0, (arrowItem - 1));
      lcd.write(byte(6));
      //Set cursor at up left corner + 2 cells.
      lcd.setCursor(2, 0);
      lcd.print(menuItem1);
      //Set cursor at bottom left corner + 2 cells.
      lcd.setCursor(2, 1);
      lcd.print(menuItem2);
    }

    //Waiting animaction
    void waitingActionMenu() {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print(waitingString);
      for (int i = 0; i < maxSize; i++) {
        lcd.setCursor(i, 1);
        lcd.write(7);
        delay(3000 / maxSize);
      }
    }

    void showLightsTimerSetting(int startHour, int startMinutes, int endHour, int endMinutes) override{   
      showTitle(0, "Set Timer Luci ");
      showStringNumber(2, 1, startHour);
      lcd.setCursor(4, 1); lcd.print(":");
      showStringNumber(5, 1, startMinutes);
      showStringNumber(9, 1, endHour);
      lcd.setCursor (11, 1); lcd.print(":");
      showStringNumber(12, 1, endMinutes);
    }

  

    void showTime(byte _hour, byte _minutes, String title) override{
      showTitle(1, title);
      showStringNumber(5, 1, _hour);
      lcd.setCursor(7, 1); lcd.print(":");
      showStringNumber(8, 1, _minutes);
    }
    
    void showTimeDate(byte _hour, byte _minutes, int _day, int _month, int _year, String title) override{
      showTitle(1, title);
      showStringNumber(5, 1, _hour);
      lcd.setCursor(7, 1); lcd.print(":");
      showStringNumber(8, 1, _minutes);
	  /* showTitle(1, "Set Data");
      showStringNumber(3, 1, _day);
      lcd.setCursor(5, 1); lcd.print("/");
      showStringNumber(6, 1, _month);
      lcd.setCursor(8, 1); lcd.print("/");
      lcd.setCursor(9, 1); lcd.print(_year); */
    }

    // Managing the cursor to move inside the time selection menu
    void manageLightsTimerSetting(int row, String one, String two, int h, int m) override{
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("HH");
      if (row == 0) {
        lcd.setCursor (2, 0);
        lcd.write(6);
      }
      lcd.setCursor (3, 0); lcd.print(h);
      if (row == 0) {
        lcd.setCursor (5, 0);
        lcd.write(5);
      }
      lcd.setCursor (0, 1); lcd.print("MM");
      if (row == 1) {
        lcd.setCursor (2, 1);
        lcd.write(6);
      }
      lcd.setCursor (3, 1); lcd.print(m);
      if (row == 1) {
        lcd.setCursor (5, 1);
        lcd.write(5);
      }
      lcd.setCursor(7, 0); lcd.print(one); lcd.print(two);
      if (row == 2) {
        lcd.setCursor (7, 1);
        lcd.write(6);
      }
      lcd.setCursor (8, 1); lcd.print("OK");
      if (row == 3) {
        lcd.setCursor (12, 1);
        lcd.write(6);
      }
      lcd.setCursor (13, 1); lcd.print("<-");
    } 

    //Managing the cursor that selects the temperature to manage the termometer
    void showDateSetting(int row, int y, int m, int d) override{
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("Data");
      if (row == 0) {
        lcd.setCursor (1, 1);
        lcd.write(6);
      }
      lcd.setCursor (2, 1); lcd.print(y);
      if (row == 0) {
        lcd.setCursor (6, 1);
        lcd.write(5);
      }
      if (row == 1) {
        lcd.setCursor (7, 1);
        lcd.write(6);
      }
      lcd.setCursor (8, 1); lcd.print(m);
      if (row == 1) {
        lcd.setCursor (10, 1);
        lcd.write(5);
      }
      if (row == 2) {
        lcd.setCursor (11, 1);
        lcd.write(6);
      }
      lcd.setCursor(12, 1); lcd.print(d);
      if (row == 2) {
        lcd.setCursor (14, 1);
        lcd.write(5);
      }
      lcd.setCursor (8, 0); lcd.print("OK");
      if (row == 3) {
        lcd.setCursor (10, 0);
        lcd.write(6);
      }
      lcd.setCursor (13, 0); lcd.print("<-");
      if (row == 4) {
        lcd.setCursor (15, 0);
        lcd.write(6);
      }
    }

    void showActualTemperatureRange(byte _max, byte _min) override{
      showTitle(0, "Set Riscalda.");
      lcd.setCursor(1, 1); lcd.print("MIN:");
      showStringNumber(5, 1, _min);
      lcd.setCursor(8, 1); lcd.print("MAX:");
      showStringNumber(12, 1, _max);    
    }

    //Managing the cursor that selects the temperature to manage the termometer
    void showMinMaxRangeSetting(int row, String one, int h, int m) override{
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("MIN");
      if (row == 0) {
        lcd.setCursor (3, 0);
        lcd.write(6);
      }
      lcd.setCursor (4, 0); lcd.print(h);
      if (row == 0) {
        lcd.setCursor (6, 0);
        lcd.write(5);
      }
      lcd.setCursor (0, 1); lcd.print("MAX");
      if (row == 1) {
        lcd.setCursor (3, 1);
        lcd.write(6);
      }
      lcd.setCursor (4, 1); lcd.print(m);
      if (row == 1) {
        lcd.setCursor (6, 1);
        lcd.write(5);
      }
      lcd.setCursor(8, 0); lcd.print(one);
      if (row == 2) {
        lcd.setCursor (7, 1);
        lcd.write(6);
      }
      lcd.setCursor (8, 1); lcd.print("OK");
      if (row == 3) {
        lcd.setCursor (12, 1);
        lcd.write(6);
      }
      lcd.setCursor (13, 1); lcd.print("<-");
    }

    //Managing the cursor  to manage the updating frequence per item
    void showManageFreqSetting(int row, int t, int e, int p) override{
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("SET");
      lcd.setCursor (5, 0); lcd.print("T:");
      lcd.setCursor (7, 0); lcd.print(24 / freqNumbber[t]);
      if (row == 0) {
        lcd.setCursor (9, 0);
        lcd.write(5);
      }
      lcd.setCursor (11, 0); lcd.print("E:");
      lcd.setCursor (13, 0); lcd.print(24 / freqNumbber[e]);
      if (row == 1) {
        lcd.setCursor (15, 0);
        lcd.write(5);
      }
      lcd.setCursor (0, 1); lcd.print("FREQ");
      lcd.setCursor (5, 1); lcd.print("P:");
      lcd.setCursor (7, 1); lcd.print(24 / freqNumbber[p]);
      if (row == 2) {
        lcd.setCursor (9, 1);
        lcd.write(5);
      }
      lcd.setCursor (10, 1); lcd.print("OK");
      if (row == 3) {
        lcd.setCursor (12, 1);
        lcd.write(6);
      }
      lcd.setCursor (13, 1); lcd.print("<-");
      if (row == 4) {
        lcd.setCursor (15, 1);
        lcd.write(6);
      }
    }

    //Managing the cursor  to manage the updating frequence per item
    void showManageFreqFilterSetting(int row, int f) override{
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("SET");
      lcd.setCursor (5, 0); lcd.print("F:");
      lcd.setCursor (7, 0); lcd.print(24 / freqNumbber[f]);
      if (row == 0) {
        lcd.setCursor (9, 0);
        lcd.write(5);
      }
      
      lcd.setCursor (10, 1); lcd.print("OK");
      if (row == 3) {
        lcd.setCursor (12, 1);
        lcd.write(6);
      }
      lcd.setCursor (13, 1); lcd.print("<-");
      if (row == 4) {
        lcd.setCursor (15, 1);
        lcd.write(6);
      }
    }

    void showTitle(byte i, String title) {
      lcd.setCursor (0, 0); lcd.print("                ");
      lcd.setCursor (i, 0); lcd.print(title);
      lcd.setCursor (15, 0); lcd.write(6);
      lcd.setCursor(0, 1); lcd.print("                ");
    }
    void showStringNumber(byte i, byte j, int number) override{
      lcd.setCursor(i, j);
      if (number < 9) {
        lcd.print("0");
        lcd.setCursor(i + 1, j); lcd.print(number);
      } else {
        lcd.print(number);
      }
    }
     
    void showWebRateSending(byte freqT, byte freqE, byte freqP) override{
      showTitle(0, "SET FREQ UPDATE");
      lcd.setCursor(1, 1); lcd.print("T:");
      if (freqNumbber[freqT] == 23) {
        showStringNumber(3, 1, 1);
      } else {
        showStringNumber(3, 1, 24 / freqNumbber[freqT]);
      }
      lcd.setCursor(6, 1); lcd.print("E:");
      if (freqNumbber[freqE] == 23) {
        showStringNumber(8, 1, 1);
      } else {
        showStringNumber(8, 1, 24 / freqNumbber[freqE]);
      }
      lcd.setCursor (11, 1); lcd.print("P:");
      if (freqNumbber[freqP] == 23) {
        showStringNumber(13, 1, 1);
      } else {
        showStringNumber(13, 1, 24 / freqNumbber[freqP]);
      }
    }
};

#endif