/*
  Aquarium Project Pasquale
*/
#ifndef header_H
#define header_H

#include <Wire.h>

class Screen
{

protected:
  // ------------------   Frame rate   -----------------------------
  int frp = 1000 / 4;
  const int maxSize = 16;
  byte freqUpdateWebTemperatureIndex = 0;
  byte freqUpdateWebECIndex = 0;
  byte freqUpdateWebPHIndex = 0;
  byte freqFilterIndex = 0;
  bool manualSendingPH = false;
  bool manualSendingTemperature = false;
  bool manualSendingEC = false;
  bool monitorEC = false;
  bool monitorPH = false;
  bool connectionShow = false;
  int okButton;
  byte hourEMinTemp[4] = {0, 0, 0, 0};
  byte hourEMinTimeClockTemp[2] = {0, 0};
  byte hourEMinTimeRecovery[2] = {0, 0};
  int dateTemp[3] = {0, 0, 0};
  byte temperatureMinAndMaxTemp[2] = {0, 0};

  //----------------   Initialize Menu Array   ---------------------
  //-----------------------   Main Menu Page   ----------------------
  String mainMenu[4] = {"Manual", "Automation", "Maintenance", "Settings"};
  byte mainMenuSize = sizeof(mainMenu) / sizeof(mainMenu[0]);
  //-----------------------   Manual Menu Page  ---------------------
  String manualMenu[9] = {"Light", "Filter", "Heater", "Feeder", "Monitoring EC",
                          "Monitoring PH", "Send Temperat.", "Send EC", "Send PH"};
  byte manualMenuSize = sizeof(manualMenu) / sizeof(manualMenu[0]);
  //-----------------------   Setting Menu Page   -------------------
  String settingMenu[14] = {"Timer Light", "Time/Date", "Recovery", "Connection", "Reconnect",
                            "Heater Auto", "WebRate sending", "Thermometer", "EC Meter",
                            "PH Meter", "Temp. Sending", "EC/TDS Sending", "PH Sending", "Filter Auto"};
  byte settingMenuSize = sizeof(settingMenu) / sizeof(settingMenu[0]);
  int activeMenu = 1;
  bool changingPage = true;
  byte freqNumbber[8] = {24, 12, 8, 6, 4, 3, 2, 1};
  String monthNames[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  char waitingString[15] = "Waiting...";
  int rowItem = 0, colItem = 0;
  byte rowItemMainMenu = 0, rowItemManualMenu = 0, rowItemSettingMenu = 0;

public:
  Screen(){};

  ~Screen(){};

  /**
   * @fn menu_
   * @brief Showing Menu:
   * It is the main function to show the menu. The most important variables are 'activeMenu' and 'changingPage',
   * they allow to change menu page. There are some sections to select in order to manage the acquarium.
   * It is very important to know what is the row and the column of the menu in each option menu because they led
   * the choice of the action. To include specific actions between some modules, it is required to customize some
   * functions in the main page.
   * @param keyPad: key picked out in keypad module
   * @return boolean value to keep showing the menu' on screen or disable it.
   */
  bool menu_(int keyPad)
  {
    // se non è stato premuto nessun tasto ma si riceve un comando di cambiare pagina da qualche pagina precedente altrimenti rimane la stessa pagina statica
    if (keyPad == 0 && !changingPage)
    {
      return true;
    }
    // setta sempre a false il cambio pagina per cambiarla una sola volta nel caso e fermasi al controllo precedente
    if (changingPage)
    {
      setChangingPage(false);
    }
    // switch sulle varie sezioni che vengono scelte
    switch (activeMenu)
    {
    case 1: // Serial.println("Main Menu...");
      setRow(manageRowMenuCursors(keyPad, mainMenuSize, rowItem));
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      showListMenu(mainMenuSize, rowItem, mainMenu);
      if (colItem == -1)
      {
        exitFromMenu();
        return setMenuAndChangingPage(true, false);
      }
      else if (colItem == 1)
      {
        setChangingPage(true);
        switch (rowItem)
        {
        case 0:
          return moveFromMainMenu(2, rowItem);
        case 1:
          return moveFromMainMenu(5, rowItem);
        case 2:
          return moveFromMainMenu(5, rowItem);
        case 3:
          return moveFromMainMenu(3, rowItem);
        default:
          return setMenuAndChangingPage(true, false);
        }
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 2: // Serial.println("Menu Manual...");
      setRow(manageRowMenuCursors(keyPad, manualMenuSize, rowItem));
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      showListMenu(manualMenuSize, rowItem, manualMenu);
      if (colItem == -1)
      {
        return comingBackFromMenuPage(1, rowItemMainMenu);
      }
      else if (colItem == 1)
      {
        if (rowItem >= 4 && rowItem <= 5)
        {
          return moveFromManualMenu(15, rowItem);
        }
        else
        {
          return moveFromManualMenu(4, rowItem);
        }
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 3: // Serial.println("Menu Setting...");
      setRow(manageRowMenuCursors(keyPad, settingMenuSize, rowItem));
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      showListMenu(settingMenuSize, rowItem, settingMenu);
      if (colItem == -1)
      {
        return comingBackFromMenuPage(1, rowItemMainMenu);
      }
      else if (colItem == 1)
      {
        switch (rowItem)
        {
        case 0:
          return moveFromSettingMenu(6, rowItem);
        case 1:
          return moveFromSettingMenu(6, rowItem);
        case 2:
          return moveFromSettingMenu(6, rowItem);
        case 3:
          return moveFromSettingMenu(16, rowItem);
        case 4:
          return moveFromSettingMenu(16, rowItem);
        case 5:
          return moveFromSettingMenu(11, rowItem);
        case 6:
          return moveFromSettingMenu(14, rowItem);
        case 7:
          return moveFromSettingMenu(9, rowItem);
        case 8:
          return moveFromSettingMenu(9, rowItem);
        case 9:
          return moveFromSettingMenu(9, rowItem);
        case 10:
          return moveFromSettingMenu(10, rowItem);
        case 11:
          return moveFromSettingMenu(10, rowItem);
        case 12:
          return moveFromSettingMenu(10, rowItem);
        case 13:
          return moveFromSettingMenu(17, rowItem);
        default:
          return setMenuAndChangingPage(true, false);
        }
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 4: // Serial.println("Menu Yes o No for Manual Section...");
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      if (colItem == -1)
      {
        return comingBackFromMenuPage(2, rowItemManualMenu);
      }
      showYesOrNoSelection1(colItem, rowItemManualMenu, manualMenu);
      if (keyPad == okButton)
      {
        waitingActionMenu();
        if (rowItemManualMenu < 4)
        {
          manageReleSymbolAndAction(rowItemManualMenu, colItem);
        }
        else
        {
          switch (rowItemManualMenu)
          {
          case 6:
            if (colItem == 0)
            {
              setManualSendingTemperature(true);
            }
            break;
          case 7:
            if (colItem == 0)
            {
              setManualSendingEC(true);
            }
            break;
          case 8:
            if (colItem == 0)
            {
              setManualSendingPH(true);
            }
            break;
          default:
            break;
          }
        }
        exitFromMenu();
        return setMenuAndChangingPage(true, false);
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 5: // Serial.println("Menu Yes o No for Main Section...");
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      if (colItem == -1)
      {
        return comingBackFromMenuPage(1, rowItemMainMenu);
      }
      showYesOrNoSelection2(colItem, rowItemMainMenu, mainMenu);
      if (keyPad == okButton)
      {
        waitingActionMenu();
        manageAutomationProcessAndMaintenance(rowItemMainMenu, colItem);
        exitFromMenu();
        return setMenuAndChangingPage(true, false);
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 6: // Serial.println("Menu show time...");
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      if (colItem == -1)
      {
        return comingBackFromMenuPage(3, rowItemSettingMenu);
      }
      if (rowItemSettingMenu == 0)
      {
        lightsTimerSetting();
        showLightsTimerSetting(hourEMinTemp[0], hourEMinTemp[1], hourEMinTemp[2], hourEMinTemp[3]);
      }
      else if (rowItemSettingMenu == 1)
      {
        timeSetting();
        dateSetting();
        showTimeDate(hourEMinTimeClockTemp[0], hourEMinTimeClockTemp[1],
                     dateTemp[0], dateTemp[1], dateTemp[2], "Current Time/Date");
      }
      else
      {
        recoverySetting();
        showTime(hourEMinTimeRecovery[0], hourEMinTimeRecovery[1], "Recovery Time");
      }
      if (colItem == 1)
      {
        return moveFromMenu(7, 0);
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 7: // Serial.println("Menu set start time or real time...");
      switch (rowItemSettingMenu)
      {
      case 0:
        setRow(manageRowMenuCursors(keyPad, 4, rowItem));
        setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
        shiftHourAndMinutesMenu(0, 1, keyPad, "Start", "Time", rowItem, hourEMinTemp);
        if (keyPad == okButton)
        {
          if (rowItem == 2)
          {
            return moveFromMenu(8, 0);
          }
          else if (rowItem == 3)
          {
            return comingBackFromMenuPage(3, rowItemSettingMenu);
          }
          else
          {
            return setMenuAndChangingPage(false, true);
          }
        }
        else
        {
          return setMenuAndChangingPage(false, true);
        }
      case 1:
        setRow(manageRowMenuCursors(keyPad, 4, rowItem));
        setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
        shiftHourAndMinutesMenu(0, 1, keyPad, "Set", "Time", rowItem, hourEMinTimeClockTemp);
        if (keyPad == okButton)
        {
          if (rowItem == 2)
          {
            return moveFromSettingMenu(7, 3);
          }
          else if (rowItem == 3)
          {
            return comingBackFromMenuPage(3, rowItemSettingMenu);
          }
          else
          {
            return setMenuAndChangingPage(false, true);
          }
        }
        else
        {
          return setMenuAndChangingPage(false, true);
        }
      case 2:
        setRow(manageRowMenuCursors(keyPad, 4, rowItem));
        setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
        shiftHourAndMinutesMenu(0, 1, keyPad, "Set", "Time", rowItem, hourEMinTimeRecovery);
        if (keyPad == okButton)
        {
          if (rowItem == 2)
          {
            waitingActionMenu();
            saveRecoveryTime(hourEMinTimeRecovery);
            exitFromMenu();
            return setMenuAndChangingPage(true, false);
          }
          else if (rowItem == 3)
          {
            return comingBackFromMenuPage(3, rowItemSettingMenu);
          }
          else
          {
            return setMenuAndChangingPage(false, true);
          }
        }
        else
        {
          return setMenuAndChangingPage(false, true);
        }
      case 3:
        setRow(manageRowMenuCursors(keyPad, 5, rowItem));
        setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
        setDateMenu(0, 1, 2, keyPad, rowItem, dateTemp);
        if (keyPad == okButton)
        {
          if (rowItem == 3)
          {
            saveDate(dateTemp);
            delay(30);
            Serial.println(" 1M: ");
            saveTime(hourEMinTimeClockTemp);
            Serial.println(" 2M: ");
            waitingActionMenu();
            exitFromMenu();
            return setMenuAndChangingPage(true, false);
          }
          else if (rowItem == 4)
          {
            return comingBackFromMenuPage(3, 1);
          }
          else
          {
            return setMenuAndChangingPage(false, true);
          }
        }
        else
        {
          return setMenuAndChangingPage(false, true);
        }
      default:
        return setMenuAndChangingPage(true, false);
      }
    case 8: // Serial.println("Menu set end time...");
      setRow(manageRowMenuCursors(keyPad, 4, rowItem));
      setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
      shiftHourAndMinutesMenu(2, 3, keyPad, "End", "Time", rowItem, hourEMinTemp);
      if (keyPad == okButton)
      {
        if (rowItem == 2)
        {
          waitingActionMenu();
          saveTimerLights(hourEMinTemp);
          exitFromMenu();
          return setMenuAndChangingPage(true, false);
        }
        else if (rowItem == 3)
        {
          return comingBackFromMenuPage(3, rowItemSettingMenu);
        }
        else
        {
          return setMenuAndChangingPage(false, true);
        }
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 9: // Serial.println("Menu Settings Sensors activation...");
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      if (colItem == -1)
      {
        return comingBackFromMenuPage(3, rowItemSettingMenu);
      }
      showYesOrNoSelection3(colItem, rowItemSettingMenu, settingMenu);
      if (keyPad == okButton)
      {
        waitingActionMenu();
        manageSettingsSelections(rowItemSettingMenu, colItem);
        exitFromMenu();
        return setMenuAndChangingPage(true, false);
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 10: // Serial.println("Menu Yes or No Web Sending...");
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      if (colItem == -1)
      {
        comingBackFromMenuPage(3, rowItemSettingMenu);
      }
      yesOrNoSelection4(colItem, rowItemSettingMenu, settingMenu);
      if (keyPad == okButton)
      {
        waitingActionMenu();
        manageSettingsSelections(rowItemSettingMenu, colItem);
        exitFromMenu();
        return setMenuAndChangingPage(true, false);
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 11: // Serial.println("Menu show min - max temperature...");
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      actualTemperatureRange();
      showActualTemperatureRange(temperatureMinAndMaxTemp[1], temperatureMinAndMaxTemp[0]);
      if (colItem == 1)
      {
        moveFromMenu(12, 0);
      }
      else if (colItem == -1)
      {
        return comingBackFromMenuPage(3, rowItemSettingMenu);
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 12: // Serial.println("Menu set temperature...");
      setRow(manageRowMenuCursors(keyPad, 4, rowItem));
      setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
      settingNewMinMaxTemperatureRange(keyPad, "setGrad", rowItem, temperatureMinAndMaxTemp);
      if (keyPad == okButton)
      {
        if (rowItem == 2)
        {
          waitingActionMenu();
          saveMinMaxTemperature(temperatureMinAndMaxTemp);
          exitFromMenu();
          return setMenuAndChangingPage(true, false);
        }
        else if (rowItem == 3)
        {
          return comingBackFromMenuPage(3, rowItemSettingMenu);
        }
        else
        {
          return setMenuAndChangingPage(false, true);
        }
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 13: // Serial.println("Menu set Freq update web...");
      setRow(manageRowMenuCursors(keyPad, 5, rowItem));
      setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
      setFreqUpdateMenu(keyPad, rowItem);
      if (keyPad == okButton)
      {
        if (rowItem == 3)
        {
          waitingActionMenu();
          saveFreqUpdateWeb();
          exitFromMenu();
          return setMenuAndChangingPage(true, false);
        }
        else if (rowItem == 4)
        {
          comingBackFromMenuPage(3, rowItemSettingMenu);
        }
        else
        {
          return setMenuAndChangingPage(false, true);
        }
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 14: // Serial.println("Menu show WEB rate...");
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      freqNumberIndex();
      showWebRateSending(freqUpdateWebTemperatureIndex, freqUpdateWebECIndex, freqUpdateWebPHIndex);
      if (colItem == -1)
      {
        return comingBackFromMenuPage(3, rowItemSettingMenu);
      }
      else if (colItem == 1)
      {
        return moveFromMenu(13, 0);
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 15: // Serial.println("Menu show Monitoring EC / PH ...");
      setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
      if (rowItemManualMenu == 4)
      {
        setMonitorEC(true);
      }
      else
      {
        setMonitorPH(true);
      }
      if (keyPad == okButton)
      {
        waitingActionMenu();
        setMonitorEC(false);
        setMonitorPH(false);
        return comingBackFromMenuPage(2, rowItemManualMenu);
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 16: // Serial.println("Menu WIFI ...");
      setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
      if (getRowSettingMenuMem() == 4)
      {
        waitingActionMenu();
        reconnectMenu();
        exitFromMenu();
        return setMenuAndChangingPage(true, false);
      }
      else
      {
        sentConnectionRequest();
        setConnectionShow(true);
      }
      if (keyPad == okButton)
      {
        waitingActionMenu();
        setConnectionShow(false);
        return comingBackFromMenuPage(3, rowItemSettingMenu);
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 17: // Serial.println("Filter auto ...");
      setColumn(manageColMenuCursors(keyPad, -1, 2, colItem));
      if (colItem == -1)
      {
        return comingBackFromMenuPage(3, rowItemSettingMenu);
      }
      showYesOrNoSelection3(colItem, rowItemSettingMenu, settingMenu);
      if (keyPad == okButton)
      {
        if (colItem == 0)
        {
          return moveFromMenu(18, 0);
        }
        else if (colItem == 1)
        {
          waitingActionMenu();
          manageSettingsSelections(rowItemSettingMenu, colItem);
          exitFromMenu();
          return setMenuAndChangingPage(true, false);
        }
        else
        {
          return setMenuAndChangingPage(false, true);
        }
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    case 18: // Serial.println("Filter auto set freq...");
      setRow(manageRowMenuCursors(keyPad, 3, rowItem));
      setColumn(manageColMenuCursors(keyPad, 1, 1, colItem));
      setFreqFilterMenu(keyPad, rowItem);
      if (keyPad == okButton)
      {
        if (rowItem == 1)
        {
          waitingActionMenu();
          saveFreqUpdateWeb();
          manageSettingsSelections(rowItemSettingMenu, colItem);
          exitFromMenu();
          return setMenuAndChangingPage(true, false);
        }
        else if (rowItem == 2)
        {
          comingBackFromMenuPage(3, rowItemSettingMenu);
        }
        else
        {
          return setMenuAndChangingPage(false, true);
        }
      }
      else
      {
        return setMenuAndChangingPage(false, true);
      }
    default:
      return setMenuAndChangingPage(true, false);
    }
  }

  /**
   * @fn sentConnectionRequest
   * @brief Anonymous function to specify in the main file to get the connection status. 
   * @return Connection status.
   */ 
  void sentConnectionRequest();
  
  /**
   * @fn checkScreenBackLight
   * @brief Anonymous function to to specify in the main file if the back light of the screen should be on or off.
   * If keyPad is different from the value of no key pressed, it will turn on the back light of the screen and
   * it will set a value status and a timerOfLight's parameter. 
   * @param keyPad: key picked out in keypad module
   * @param noTouch: set the value when No key is pressed  
   * @param frp: rate of the loop 
   * @param sec: back light duration in seconds 
   * @return Enable or disable the back light of the screen.
   */ 
  void checkScreenBackLight(int keyPad, byte noTouch, int frp, byte sec);

  /**
   * @fn reconnectMenu
   * @brief Anonymous function to specify in the main file to reconnect to WIFI. 
   * @return Reconnect the board to WIFI.
   */ 
  void reconnectMenu();
  
  /**
   * @fn freqNumberIndex
   * @brief Anonymous function to specify in the main file to set the uploading rates of EC, PH and Temperature. 
   * @return Variables setting.
   */ 
  void freqNumberIndex();
 
  /**
   * @fn saveFreqUpdateWeb
   * @brief Anonymous function to specify in the main file to save the uploading rates of EC, PH and Temperature
   *  in SD Card. 
   * @return Variables saving.
   */ 
  void saveFreqUpdateWeb();
  
  /**
   * @fn saveMinMaxTemperature
   * @brief Anonymous function to specify in the main file to save the uploading minumum and maximum temperature
   *  in SD Card, in order to turn on or turn off the heater.
   * @param minMax: array with min and max temperatures
   * @return Variables saving.
   */ 
  void saveMinMaxTemperature(byte *minMax);

  /**
   * @fn yesOrNoSelection4
   * @brief Anonymous function to specify in the main file to show menu page where there are the options to choose
   * @param selectedItem: selected item 
   * @param rowNumber: number of the selected row
   * @param listMenu: list of section to show on screen
   * @return Variables saving.
   */ 
  void yesOrNoSelection4(byte selectedItem, int rowNumber, String *listMenu);
  
  /**
   * @fn actualTemperatureRange
   * @brief Anonimous function to specify in the main file to set the temperature range.
   * @return Variables setting.
   */ 
  void actualTemperatureRange();
  
  /**
   * @fn manageSettingsSelections
   * @brief Anonimous function to specify in the main file to manage some action to do when a specific option is selected.
   * @param item: selected item
   * @param colItem: column of this item
   * @return Action.
   */ 
  void manageSettingsSelections(int item, int colItem);
  
  /**
   * @fn saveFreqUpdateWeb
   * @brief Anonimous function to specify in the main file to save timer hours and minutes.
   * @param hourEMin: array with start and end hours and minutes
   * @return Variables saving.
   */ 
  void saveTimerLights(byte *hourEMin);

  /**
   * @fn saveTime
   * @brief Anonimous function to specify in the main file to save the right time.
   * @param hourEMin: array with hours and minutes
   * @return Time saving.
   */ 
  void saveTime(byte *hourEMin);
 
  /**
   * @fn saveRecoveryTime
   * @brief Anonimous function to specify in the main file to save time to do the recovery of EC, PH and Temperature data.
   * @param hourEMin: array with hours and minutes of the recovery action 
   * @return Variables saving.
   */ 
  void saveRecoveryTime(byte *hourEMin);

 /**
   * @fn saveDate
   * @brief Anonimous function to specify in the main file to save right date.
   * @param date: array with year, month and day
   * @return Date saving.
   */ 
  void saveDate(int *date);
  
  /**
   * @fn dateSetting
   * @brief Anonimous function to specify in the main file to set the adjust date page on screen.
   * @return Variables setting.
   */ 
  void dateSetting();

  /**
   * @fn timeSetting
   * @brief Anonimous function to specify in the main file to set the adjust time page on screen.
   * @return Variables setting.
   */ 
  void timeSetting();

  /**
   * @fn recoverySetting
   * @brief Anonimous function to specify in the main file to set the adjust recovery time page on screen.
   * @return Variables setting.
   */ 
  void recoverySetting();

  /**
   * @fn lightsTimerSetting
   * @brief Anonimous function to specify in the main file to set the adjust timer page on screen.
   * @return Variables setting.
   */ 
  void lightsTimerSetting();

  /**
   * @fn manageAutomationProcessAndMaintenance
   * @brief Anonimous function to specify in the main file to set the automation or maintenance procedures.
   * @param itemSelected: selected item
   * @param colItem: number of columns
   * @return Variables setting.
   */ 
  void manageAutomationProcessAndMaintenance(int itemSelected, int colItem);

  /**
   * @fn manageReleSymbolAndAction
   * @brief Anonimous function to specify in the main file to manage each rele
   * @param index: Index of the rele to manage
   * @param onOff: Value to set the rele on or off
   * @return Rele managing.
   */ 
  void manageReleSymbolAndAction(int index, int onOff);

  /**
   * @fn releSymbolMenu
   * @brief Anonimous function to specify in the main file to set each items showing each rele on screen.
   * @return Variables setting.
   */ 
  void releSymbolMenu();
  
  /**
   * @fn initScreen
   * @brief Virtual function to override in the subClass file to show the init Screen.
   * @return Showing init screen.
   */ 
  virtual void initScreen() = 0;
  
  /**
   * @fn showActualTemperatureRange
   * @brief Virtual function to override in the subClass file to show the temperature range of the
   * aquarium. If the aquarium temperature is out of this range, the system will turn on the heater.
   * @param _max: Max temperature of the range
   * @param _min: Min temperature of the range
   * @return Showing the temperature range.
   */ 
  virtual void showActualTemperatureRange(byte _max, byte _min) = 0;
  
  /**
   * @fn mainScreen
   * @brief Virtual function to override in the subClass file to show main screen.
   * @param _time: time string
   * @param temperature: temperature from the sensor
   * @param ec: Ec from the sensor
   * @param phFinal: Ph from the sensor
   * @return Showing main screen with time, ec, ph, temperature and rele information.
   */  
  virtual void mainScreen(char *_time, float temperature, float ec, float phFinal) = 0;
  
  /**
   * @fn showReleSymbol
   * @brief Virtual function to override in the subClass file to show each rele information on main screen page.
   * @param rele: list with rele pins
   * @param num: rele index
   * @return Showing each rele information.
   */ 
  virtual void showReleSymbol(bool *rele, byte num) = 0;
  
  /**
   * @fn waitingActionMenu
   * @brief Virtual function to override in the subClass file to show waiting page on screen.
   * @return Showing waiting page.
   */
  virtual void waitingActionMenu() = 0;
  
  /**
   * @fn showTimeDate
   * @brief Virtual function to override in the subClass file to show the date and time in their setting page.
   * @param _hour: hour from right time
   * @param _minutes: minute from right time
   * @param _day: day from right date
   * @param _month: month from right date
   * @param _year: year from right date
   * @param title: string to show in the setting page
   * @return Showing time and date information in the setting page.
   */ 
  virtual void showTimeDate(byte _hour, byte _minutes, int _day, int _month, int _year, String title) = 0;
  
  /**
   * @fn showYesOrNoSelection1
   * @brief Virtual function to override in the subClass file to show a selection page of the menu.
   * @param selectedItem: selected item 
   * @param rowNumber: row index
   * @param listMenu: list with menu options
   * @return Showing the selection page.
   */ 
  virtual void showYesOrNoSelection1(byte selectedItem, int rowNumber, String *listMenu) = 0;
   
  /**
   * @fn showYesOrNoSelection2
   * @brief Virtual function to override in the subClass file to show a selection page of the menu.
   * @param selectedItem: selected item 
   * @param rowNumber: row index
   * @param listMenu: list with menu options.
   * @return Showing the selection page.
   */ 
  virtual void showYesOrNoSelection2(byte selectedItem, int rowNumber, String *listMenu) = 0;
  
  /**
   * @fn showYesOrNoSelection3
   * @brief Virtual function to override in the subClass file to show a selection page of the menu.
   * @param selectedItem: selected item 
   * @param rowNumber: row index
   * @param listMenu: list with menu options
   * @return Showing the selection page.
   */ 
  virtual void showYesOrNoSelection3(byte selectedItem, int rowNumber, String *listMenu) = 0;
   
  /**
   * @fn showYesOrNoSelection4
   * @brief Virtual function to override in the subClass file to show a selection page of the menu.
   * @param selectedItem: selected item 
   * @param rowNumber: row index
   * @param onOffTemp: status of temperature monitoring
   * @param onOffEc: status of Ec monitoring
   * @param onOffPh: status of Ph monitoring
   * @return Showing the selection page.
   */ 
  virtual void showYesOrNoSelection4(byte selectedItem, int rowNumber, bool onOffTemp, bool onOffEc, bool onOffPh) = 0;
  
  /**
   * @fn showLightsTimerSetting
   * @brief Virtual function to override in the subClass file to show lights timer setting page.
   * @param startHour: hour when to turn on the lights
   * @param startMinutes: minute when to turn on the lights 
   * @param endHour: hour when to turn off the lights
   * @param endMinutes: minute when to turn off the lights
   * @return Showing lights timer setting page.
   */ 
  virtual void showLightsTimerSetting(int startHour, int startMinutes, int endHour, int endMinutes) = 0;
  
  /**
   * @fn manageLightsTimerSetting
   * @brief Virtual function to override in the subClass file to manage lights timer setting actions in the setting page.
   * @param row: selected row
   * @param one: first string to show in the title 
   * @param two: second string to show in the title
   * @param h: hour to show
   * @param m: minute to show
   * @return Managing lights timer action in the setting page.
   */ 
  virtual void manageLightsTimerSetting(int row, String one, String two, int h, int m) = 0;
  
  /**
   * @fn showStringNumber
   * @brief Virtual function to override in the subClass file to show and convert a number to a string;
   * @param i: cursor coordinate y
   * @param j: cursor coordinate x
   * @param number: number to show
   * @return Showing a number like string.
   */  
  virtual void showStringNumber(byte i, byte j, int number) = 0;

  /**
   * @fn showDateSetting
   * @brief Virtual function to override in the subClass file to show date setting page.
   * @param row: selected row
   * @param y: year to show 
   * @param m: month to show
   * @param d: day to show
   * @return Showing date setting page.
   */ 
  virtual void showDateSetting(int row, int y, int m, int d) = 0;
  
  /**
   * @fn showTime
   * @brief Virtual function to override in the subClass file to show time setting page.
   * @param _hour: hour to show
   * @param _minutes: minute to show
   * @param title: string to show
   * @return Showing time setting page.
   */ 
  virtual void showTime(byte _hour, byte _minutes, String title) = 0;
  
  /**
   * @fn showListMenu
   * @brief Virtual function to override in the subClass file to show a manu option with a selecting symbol in circle way.
   * @param listSize: size of the list of option
   * @param rowItem: selected row
   * @param listMenu: array with options
   * @return Showing menu options in circle way.
   */ 
  virtual void showListMenu(int listSize, int rowItem, String *listMenu) = 0;
  
  /**
   * @fn showMinMaxRangeSetting
   * @brief Virtual function to override in the subClass file to show minimun and maximum range of temperature in setting page.
   * @param row: selected row
   * @param one: string to show
   * @param h: max temperature to show
   * @param m: min temperature to show
   * @return Showing minimun and maximum range of temperature in setting page.
   */ 
  virtual void showMinMaxRangeSetting(int row, String one, int h, int m) = 0;
  
  /**
   * @fn showManageFreqSetting
   * @brief Virtual function to override in the subClass file to show and manage the uploading web rate in the setting page.
   * @param row: selected row
   * @param t: temperature web updating rate
   * @param e: ec web updating rate 
   * @param p: ph web updating rate 
   * @return Showing and managing the updating rate in setting page.
   */
  virtual void showManageFreqSetting(int row, int t, int e, int p) = 0;
  
  /**
   * @fn showManageFreqFilterSetting
   * @brief Virtual function to override in the subClass file to show and manage the filter rate in setting page.
   * @param row: selected row
   * @param f: filter rate to turn on the device
   * @return Showing and managing the filter rafe in setting page.
   */
  virtual void showManageFreqFilterSetting(int row, int f) = 0;

  /**
   * @fn showWebRateSending
   * @brief Virtual function to override in the subClass file to show the uploading web rate in setting page.
   * @param freqT: temperature web updating rate
   * @param freqE: ec web updating rate
   * @param freqP: ph web updating rate
   * @return Showing the web uploading rate setting page.
   */
  virtual void showWebRateSending(byte freqT, byte freqE, byte freqP) = 0;
  
  /**
   * @fn showMonitoring
   * @brief Virtual function to override in the subClass file to show the monitoring page.
   * @param key: sensor to show
   * @param value: value to show
   * @param temperature: temperature to show
   * @return Showing the monitoring page.
   */
  virtual void showMonitoring(const char *key, float value, float temperature) = 0;

  /**
   * @fn turnOffLight
   * @brief Virtual function to override in the subClass file to turn off the backlight of the screen.
   * @return Turning off the backlight of the screen.
   */
  virtual void turnOffLight() = 0;
  
  /**
   * @fn turnOLight
   * @brief Virtual function to override in the subClass file to turn on the backlight of the screen.
   * @return Turning on the backlight of the screen.
   */
  virtual void turnOnLight() = 0;

  /**
   * @fn connectionStatusPage
   * @brief Virtual function to override in the subClass file to show the connection status page on screen.
   * @param connStatus: string with connection status 
   * @return Showing the connection status page on screen.
   */ 
  virtual void connectionStatusPage(const char *connStatus) = 0;

  /**
   * @fn getFrameRate
   * @brief Getting the frame rate of the screen.
   * @return Int frame rate.
   */ 
  int getFrameRate() { return frp; }

  /**
   * @fn shiftHourAndMinutesMenu
   * @brief Setting time changing the hour or minute values.
   * @param i: hour index in array time
   * @param j: minute index in array time
   * @param keyPad: key pressed
   * @param firstS: string to show
   * @param secondS: string to show 
   * @param row: selected row on the screen
   * @param hourEMin: array with hour and minute values
   * @return New time value in its array.
   */ 
  void shiftHourAndMinutesMenu(const int i, const int j, const int keyPad, const String firstS, const String secondS, int row, byte *hourEMin)
  {
    if (row == 0)
    {
      hourEMin[i] = manageMenuRangeNumberCursors(keyPad, row, hourEMin[i], 0, 24);
    }
    else if (row == 1)
    {
      hourEMin[j] = manageMenuRangeNumberCursors(keyPad, row, hourEMin[j], 0, 60);
    }
    manageLightsTimerSetting(row, firstS, secondS, hourEMin[i], hourEMin[j]);
  }
  
  /**
   * @fn settingNewMinMaxTemperatureRange
   * @brief Setting temperature range changing the min and max temperature values.
   * @param keyPad: key pressed
   * @param firstS: string to show
   * @param secondS: string to show 
   * @param row: selected row on the screen
   * @param minMax: array with min and max values
   * @return New temperature range value in its array.
   */ 
  void settingNewMinMaxTemperatureRange(const int keyPad, const String firstS, int row, byte *minMax)
  {
    if (row == 0)
    {
      minMax[0] = manageMenuRangeNumberCursors(keyPad, row, minMax[0], 0, 99);
    }
    else if (row == 1)
    {
      minMax[1] = manageMenuRangeNumberCursors(keyPad, row, minMax[1], 0, 99);
    }
    showMinMaxRangeSetting(row, firstS, minMax[0], minMax[1]);
  }
  
  /**
   * @fn setFreqUpdateMenu
   * @brief Setting web update rates changing rate values for ec, ph and temperature.
   * @param keyPad: key pressed 
   * @param row: selected row on the screen
   * @return New web update rates in its array.
   */ 
  void setFreqUpdateMenu(int keyPad, int row)
  {
    if (row == 0)
    {
      freqUpdateWebTemperatureIndex = manageMenuRangeNumberCursors(keyPad, row, freqUpdateWebTemperatureIndex, 0, 8);
    }
    else if (row == 1)
    {
      freqUpdateWebECIndex = manageMenuRangeNumberCursors(keyPad, row, freqUpdateWebECIndex, 0, 8);
    }
    else if (row == 2)
    {
      freqUpdateWebPHIndex = manageMenuRangeNumberCursors(keyPad, row, freqUpdateWebPHIndex, 0, 8);
    }
    showManageFreqSetting(row, freqUpdateWebTemperatureIndex, freqUpdateWebECIndex, freqUpdateWebPHIndex);
  }

  /**
   * @fn setFreqFilterMenu
   * @brief Setting filter update rate changing rate value how time to turn on the filter.
   * @param keyPad: key pressed 
   * @param row: selected row on the screen
   * @return New web update rate in its array.
   */ 
  void setFreqFilterMenu(int keyPad, int row)
  {
    if (row == 0)
    {
      freqFilterIndex = manageMenuRangeNumberCursors(keyPad, row, freqFilterIndex, 0, 8);
    }
    showManageFreqFilterSetting(row, freqFilterIndex);
  }
  
  /**
   * @fn setDateMenu
   * @brief Setting date changing the year, month and day values.
   * @param i: year index in array date
   * @param j: month index in array date
   * @param z: day index in array date
   * @param keyPad: key pressed
   * @param row: selected row on the screen
   * @param date: array with year, month and day values
   * @return New date value in its array.
   */ 
  void setDateMenu(byte i, byte j, byte z, int keyPad, int row, int *date)
  {
    if (row == 0)
    {
      date[z] = manageYearRangeCursor(keyPad, row, date[z], 2060);
    }
    else if (row == 1)
    {
      date[j] = manageMenuRangeNumberCursors(keyPad, row, date[j], 1, 13);
    }
    else
    {
      date[i] = manageDayRangeCursor(keyPad, row, date[j], date[z], date[i]);
    }
    if (date[i] > _get_max_day(short(date[j] - 1), date[z]))
    {
      date[i] = _get_max_day(short(date[j] - 1), date[z]);
    }
    showDateSetting(row, date[z], date[j], date[i]);
  }
  
  /**
   * @fn setMenuAndChangingPage
   * @brief Moving the menu sections.
   * @param cp: bool value used to change the menu page on screen
   * @param ret: boolean value to continue with menu or go out of it
   * @return The boolean value of ret parameter.
   */ 
  bool setMenuAndChangingPage(bool cp, bool ret)
  {
    setChangingPage(cp);
    return ret;
  }
  
  /**
   * @fn moveFromMenu
   * @brief Setting changing page variable to move the menu sections.
   * @param menuPage: move to page with this index
   * @param row: selected row
   * @return Setting the page index.
   */ 
  bool moveFromMenu(byte menuPage, int row)
  {
    setActiveMenu(menuPage);
    setRow(row);
    setColumn(0);
    return setMenuAndChangingPage(true, true);
  }
  
  /**
   * @fn moveFromSettingMenu
   * @brief Setting changing page variable to move the menu sections.
   * @param menuPage: move to page with this index
   * @param row: selected row
   * @return Setting the page index.
   */ 
  bool moveFromSettingMenu(byte menuPage, int row)
  {
    setActiveMenu(menuPage);
    setRowSettingMenuMem(row);
    setRow(0);
    setColumn(0);
    return setMenuAndChangingPage(true, true);
  }
  
  /**
   * @fn moveFromManualMenu
   * @brief Setting changing page variable to move the menu sections.
   * @param menuPage: move to page with this index
   * @param row: selected row
   * @return Setting the page index.
   */ 
  bool moveFromManualMenu(byte menuPage, int row)
  {
    setActiveMenu(menuPage);
    setRowManualMenuMem(row);
    setRow(0);
    setColumn(0);
    return setMenuAndChangingPage(true, true);
  }
  
  /**
   * @fn moveFromMainMenu
   * @brief Setting changing page variable to move the menu sections.
   * @param menuPage: move to page with this index
   * @param row: selected row
   * @return Setting the page index.
   */ 
  bool moveFromMainMenu(byte menuPage, int row)
  {
    setActiveMenu(menuPage);
    setRowMainMenuMem(row);
    setRow(0);
    setColumn(0);
    return setMenuAndChangingPage(true, true);
  }
  
  /**
   * @fn comingBackFromMenuPage
   * @brief Setting changing page variable to move the menu sections.
   * @param menuPage: move to page with this index
   * @param row: selected row
   * @return Setting the page index and changing the page.
   */ 
  bool comingBackFromMenuPage(byte menuPage, byte row)
  {
    setActiveMenu(menuPage);
    setRow(row);
    setColumn(0);
    return setMenuAndChangingPage(true, true);
  }

  /**
   * @fn manageRowMenuCursors
   * @brief Managing the row cursor to move inside the menu.
   * @param keyPad: key pressed
   * @param menuSize: number of element in the array
   * @param exRow: selected row
   * @return Variables saving.
   */ 
  int manageRowMenuCursors(int keyPad, int menuSize, int exRow)
  {
    switch (keyPad)
    {
    case 3:
      if (exRow == 0)
      {
        return menuSize - 1;
      }
      else
      {
        return exRow - 1;
      }
    case 4:
      if (exRow == menuSize - 1)
      {
        return 0;
      }
      else
      {
        return exRow + 1;
      }
    default:
      return exRow;
    }
  }

  /**
   * @fn manageColMenuCursors
   * @brief Managing the col cursor to move inside the menu.
   * @param keyPad: key pressed
   * @param minCol: value of min column
   * @param maxCol: value of max column
   * @param exCol: selected row
   * @return Variables saving.
   */
  int manageColMenuCursors(int keyPad, int minCol, int maxCol, int exCol)
  {
    switch (keyPad)
    {
    case 1:
      if (exCol != maxCol - 1)
      {
        return exCol + 1;
      }
      else
      {
        return exCol;
      }
    case 2:
      if (exCol != minCol)
      {
        return exCol - 1;
      }
      else
      {
        return exCol;
      }
    default:
      return exCol;
    }
  }
  
  /**
   * @fn exitFromMenu
   * @brief Setting menu variables before to exit of menu.
   * @return Variables setting.
   */ 
  void exitFromMenu()
  {
    setRow(0);
    setColumn(0);
    setRowMainMenuMem(0);
    setRowManualMenuMem(0);
    setRowSettingMenuMem(0);
    setActiveMenu(1);
    releSymbolMenu();
  }
  
  /**
   * @fn setChangingPage
   * @brief Setting changingPage value.
   * @param value: value to set
   * @return ChangingPage boolean value.
   */ 
  void setChangingPage(bool value) { this->changingPage = value; }
  
  /**
   * @fn setHourAndMinutes
   * @brief Set the temp array with time values through the input parameters.
   * @param _hour: selected hour
   * @param _minutes: selected minute
   * @return Setting array values.
   */ 
  void setHourAndMinutes(const byte _hour, const byte _minutes)
  {
    hourEMinTimeClockTemp[0] = _hour;
    hourEMinTimeClockTemp[1] = _minutes;
  }

  /**
   * @fn setRecoveryHourAndMinutes
   * @brief Set the temp array with recovery time values through the input parameters.
   * @param _hour: selected hour
   * @param _minutes: selected minute
   * @return Setting array values.
   */ 
  void setRecoveryHourAndMinutes(const byte _hour, const byte _minutes)
  {
    hourEMinTimeRecovery[0] = _hour;
    hourEMinTimeRecovery[1] = _minutes;
  }
   
  /**
   * @fn setDayMonthYear
   * @brief Set the temp array with date values through the input parameters.
   * @param _day: selected hour
   * @param _month: selected minute
   * @param _year: selected minute 
   * @return Setting array values.
   */ 
  void setDayMonthYear(const int _day, const int _month, const int _year)
  {
    dateTemp[0] = _day;
    dateTemp[1] = _month;
    dateTemp[2] = _year;
  }
  
  /**
   * @fn setTimerLights
   * @brief Set the temp array with timer values through the input parameters.
   * @param startH: hour to turn on the light
   * @param startM: minute to turn on the light
   * @param endH: hour to turn off the light
   * @param endM: minute to turn off the light
   * @return Setting array values.
   */ 
  void setTimerLights(int startH, int startM, int endH, int endM)
  {
    hourEMinTemp[0] = startH;
    hourEMinTemp[1] = startM;
    hourEMinTemp[2] = endH;
    hourEMinTemp[3] = endM;
  }
  
  /**
   * @fn setRow
   * @param value: row value.
   * @return Setting row value.
   */ 
  void setRow(int value) { this->rowItem = value; }
  
  /**
   * @fn getRow
   * @return Getting row value.
   */ 
  int getRow() { return rowItem; }

  /**
   * @fn setMonitorEC
   * @param value: monitoring value
   * @return Setting boolean value to enable the EC monitoring.
   */ 
  void setMonitorEC(bool value) { this->monitorEC = value; }
  
  /**
   * @fn checkIfShowECMonitoringOnScreen
   * @return Getting value to enable the EC monitoring.
   */ 
  bool checkIfShowECMonitoringOnScreen() { return monitorEC; }
  
  /**
   * @fn setConnectionShow
   * @param value: value to set
   * @return Setting value to enable the connection status.
   */ 
  void setConnectionShow(bool value) { this->connectionShow = value; }
  
  /**
   * @fn getConnectionShow
   * @return Getting value to enable the connection status.
   */ 
  bool getConnectionShow() { return connectionShow; }
  
  /**
   * @fn setMonitorPH  
   * @param value: value to set 
   * @return Setting value to enable the connection status.
   */ 
  void setMonitorPH(bool value) { this->monitorPH = value; }
  
  /**
   * @fn setManualSendingEC
   * @param value: value to set
   * @return Setting value to enable the manual web sending of Ec value.
   */ 
  void setManualSendingEC(bool value) { this->manualSendingEC = value; }
  
  /**
   * @fn checkIfSendECToWebManually
   * @return Getting value to enable the manual web sending of Ec value.
   */ 
  bool checkIfSendECToWebManually() { return manualSendingEC; }
  
  /**
   * @fn setManualSendingTemperature
   * @param value: value to set
   * @return Setting value to enable the manual web sending of temperature value.
   */ 
  void setManualSendingTemperature(bool value) { this->manualSendingTemperature = value; }
  
  /**
   * @fn checkIfSendTemperatureToWebManually
   * @return Getting value to enable the manual web sending of temperature value.
   */ 
  bool checkIfSendTemperatureToWebManually() { return manualSendingTemperature; }
  
  /**
   * @fn setManualSendingPH
   * @param value: value to set
   * @return Setting value to enable the manual web sending of Ph value.
   */ 
  void setManualSendingPH(bool value) { this->manualSendingPH = value; }
  
  /**
   * @fn checkIfSendPHToWebManually
   * @return Getting value to enable the manual web sending of Ph value.
   */ 
  bool checkIfSendPHToWebManually() { return manualSendingPH; }
  
  /**
   * @fn checkIfShowPHMonitoringOnScreen
   * @return Getting value to enable the Ph manual monitoring.
   */ 
  bool checkIfShowPHMonitoringOnScreen() { return monitorPH; }
  
  /**
   * @fn setActiveMenu
   * @param value: value to set
   * @return Setting activeMenu variable.
   */ 
  void setActiveMenu(byte value) { this->activeMenu = value; }
  
  /**
   * @fn getActiveMenu 
   * @return Getting activeMenu variable value.
   */ 
  byte getActiveMenu() { return activeMenu; }
  
  /**
   * @fn setColumn
   * @param value: selected minute
   * @return Setting column value.
   */ 
  void setColumn(int value) { this->colItem = value; }
  
  /**
   * @fn getColumn
   * @return Column value.
   */ 
  int getColumn() { return colItem; }
  
  /**
   * @fn setRowMainMenuMem
   * @param value: value to set
   * @return Setting row value.
   */ 
  void setRowMainMenuMem(byte value) { this->rowItemMainMenu = value; }
  
  /**
   * @fn setTemperatureRange
   * @brief Set the temp array with Temperature range values through the input parameters.
   * @param _min: selected min temperature value
   * @param _max: selected max temperature value
   * @return Setting array values.
   */ 
  void setTemperatureRange(byte _min, byte _max)
  {
    temperatureMinAndMaxTemp[0] = _min;
    temperatureMinAndMaxTemp[1] = _max;
  }

  /**
   * @fn setRowManualMenuMem
   * @param value: value to set
   * @return Setting row value.
   */ 
  void setRowManualMenuMem(byte value) { this->rowItemManualMenu = value; }
  
  /**
   * @fn setRowSettingMenuMem
   * @param value: value to set
   * @return Setting row value.
   */ 
  void setRowSettingMenuMem(byte value) { this->rowItemSettingMenu = value; }
  
  /**
   * @fn getRowMainMenuMem
   * @return Getting row value.
   */ 
  byte getRowMainMenuMem() { return rowItemMainMenu; }
  
  /**
   * @fn getRowManualMenuMem
   * @return Getting row value.
   */ 
  byte getRowManualMenuMem() { return rowItemManualMenu; }
  
  /**
   * @fn getRowSettingMenuMem
   * @return Getting row value.
   */ 
  byte getRowSettingMenuMem() { return rowItemSettingMenu; }
  
  /**
   * @fn indexNumber
   * @brief Finding index of comparable value
   * @param freq: rate value
   * @return Index of same value.
   */ 
  byte indexNumber(byte freq)
  {
    if (freq == 23)
    {
      return indexOf(24, freqNumbber, 8);
    }
    else
    {
      return indexOf(freq, freqNumbber, 8);
    }
  }
  
  /**
   * @fn manageYearRangeCursor
   * @brief Managing the cursor to move inside the menu for year setting
   * @param keyPad: key pressed
   * @param row: selected row
   * @param number: current year value
   * @param maxItem: Maximum selectable year 
   * @return New year value.
   */ 
  int manageYearRangeCursor(int keyPad, int row, int number, int maxItem)
  {
    if (number == 0)
    {
      number = 2020;
    }
    switch (keyPad)
    {
    case 1:
      if (number == maxItem - 1)
      {
        return 2020;
      }
      else
      {
        return ++number;
      }
    case 2:
      if (number == 2020)
      {
        return maxItem - 1;
      }
      else
      {
        return --number;
      }
    default:
      return number;
    }
  }
  
  /**
   * @fn manageDayRangeCursor
   * @brief Managing the cursor to move inside the menu for day setting
   * @param keyPad: key pressed
   * @param row: selected row
   * @param _month: current month value
   * @param _year: current year value 
   * @param number: current day value
   * @return New day value.
   */ 
  int manageDayRangeCursor(int keyPad, int row, int _month, int _year, int number)
  {
    switch (keyPad)
    {
    case 1:
      if (number == _get_max_day((short(_month - 1)), _year))
      {
        return 1;
      }
      else
      {
        return ++number;
      }
    case 2:
      if (number == 1)
      {
        return _get_max_day(short(_month - 1), _year);
      }
      else
      {
        return --number;
      }
    default:
      return number;
    }
  }

  /**
   * @fn manageMenuRangeNumberCursors
   * @brief Managing the cursor to move inside the menu 
   * @param keyPad: key pressed 
   * @param row: selected row 
   * @param number: current cursor value
   * @param minItem: minimum value of the range
   * @param maxItem: maximun value of the range  
   * @return Variables setting.
   */ 
  int manageMenuRangeNumberCursors(int keyPad, int row, int number, int minItem, int maxItem)
  {
    switch (keyPad)
    {
    case 1:
      if (number == maxItem - 1)
      {
        return minItem;
      }
      else
      {
        return ++number;
      }
    case 2:
      if (number == minItem)
      {
        return maxItem - 1;
      }
      else
      {
        return --number;
      }
    default:
      return number;
    }
  }
  
  /**
   * @fn substring
   * @brief C substring function definition.
   * @param s: string in input
   * @param sub: strinf to return
   * @param p: start of substring
   * @param l: end of substring
   * @return substring.
   */ 
  void substring(char s[], char sub[], int p, int l)
  {
    int c = 0;
    while (c < l)
    {
      sub[c] = s[p + c - 1];
      c++;
    }
    sub[c] = '\0';
  }

  /**
   * @fn _get_max_day
   * @brief Finding the max day of each month 
   * @param month: month where to find the max day
   * @param year: year of the month to find the max day
   * @return Max day of the month.
   */ 
  short _get_max_day(short month, int year)
  {
    if (month == 0 || month == 2 || month == 4 || month == 6 || month == 7 || month == 9 || month == 11)
      return 31;
    else if (month == 3 || month == 5 || month == 8 || month == 10)
      return 30;
    else
    {
      if (year % 4 == 0)
      {
        if (year % 100 == 0)
        {
          if (year % 400 == 0)
            return 29;
          return 28;
        }
        return 29;
      }
      return 28;
    }
  }

  /**
   * @fn setFreq
   * @brief Setting the value of the rate from and array.
   * @param freq: rate index
   * @return rate value.
   */ 
  int setFreq(int freq)
  {
    if (freq == 0)
    {
      return 23;
    }
    else
    {
      return freqNumbber[freq];
    }
  }
  
  /**
   * @fn indexOf
   * @brief Decreasing array count till it reaches negative, array index if current element equals provided element
   * @param elm: key pressed
   * @param ar: key pressed
   * @param ar_cnt: key pressed 
   * @return array index if current element equals provided element, -1 if element is not present.
   */ 
  int indexOf(const int elm, const byte *ar, int ar_cnt)
  {
    // decreasing array count till it reaches negative
    // arr_cnt - 1 to 0
    while (ar_cnt--)
    {
      // Return array index if current element equals provided element
      if (ar[ar_cnt] == elm)
        return ar_cnt;
    }
    // Element not present
    return -1; // Should never reaches this point
  }
};

#endif
