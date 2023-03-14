#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Screen.h"

#ifndef OLED_h
#define OLED_h
/*
  Aquarium Project Pasquale
*/

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin) 4
#define LOGO_HEIGHT 79
#define LOGO_WIDTH 77
#define NUMFLAKES 10 // Number of snowflakes in the animation example

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char PROGMEM salmon[770] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xfe, 0x00, 0x7e, 0x00, 0x00, 0x3f, 0xff, 0xfe,
    0xff, 0xff, 0xf8, 0x00, 0x1f, 0xfc, 0x00, 0x0f, 0xff, 0xfe,
    0xff, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xc0, 0x03, 0xff, 0xfe,
    0xff, 0xff, 0xc0, 0x00, 0x00, 0x1f, 0xf8, 0x01, 0xff, 0xfe,
    0xff, 0xff, 0x80, 0x00, 0x00, 0x03, 0xfe, 0x00, 0xff, 0xfe,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x7f, 0xfe,
    0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x3f, 0xfe,
    0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x1f, 0xfe,
    0xff, 0xf8, 0x00, 0x60, 0x00, 0x00, 0x07, 0xfc, 0x0f, 0xfe,
    0xff, 0xf0, 0x00, 0xc0, 0x00, 0x00, 0x01, 0xff, 0x07, 0xfe,
    0xff, 0xe0, 0x01, 0x80, 0x00, 0x00, 0x00, 0xff, 0x83, 0xfe,
    0xff, 0xc0, 0x03, 0x80, 0x00, 0x00, 0x00, 0x7f, 0xc1, 0xfe,
    0xff, 0xc0, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xe1, 0xfe,
    0xff, 0x82, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0xfe,
    0xff, 0x02, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0xfe,
    0xff, 0x06, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0x7e,
    0xfe, 0x06, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x7e,
    0xfe, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x3e,
    0xfc, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xbe,
    0xfc, 0x1f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x9e,
    0xfc, 0x3f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xde,
    0xf8, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xee,
    0xf8, 0x3f, 0xdf, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xee,
    0xf8, 0x7f, 0xe3, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe,
    0xf0, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x02, 0x00, 0x0f, 0xfe,
    0xf0, 0x7f, 0xff, 0x00, 0x00, 0x00, 0x01, 0x80, 0x0f, 0xfe,
    0xf0, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x01, 0xc0, 0x07, 0xfe,
    0xf0, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xe0, 0x07, 0xfe,
    0xf0, 0x7f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0xf0, 0x03, 0xfe,
    0xf0, 0x7f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xf8, 0x03, 0xfe,
    0xf0, 0x3f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xfc, 0x23, 0xfe,
    0xe0, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7e, 0x21, 0xfe,
    0xe0, 0x3f, 0xfe, 0xff, 0xff, 0x80, 0x00, 0x7f, 0x31, 0xfe,
    0xe0, 0x1f, 0xff, 0xfe, 0xff, 0xc0, 0x00, 0x7f, 0xf9, 0xfe,
    0xf0, 0x1f, 0xfd, 0xfe, 0xff, 0x40, 0x00, 0xff, 0xf8, 0xfe,
    0xf0, 0x0f, 0xfd, 0xff, 0xf9, 0x80, 0x00, 0xff, 0xfc, 0xfe,
    0xf0, 0x07, 0xfd, 0xff, 0xdf, 0x80, 0x01, 0xff, 0xfe, 0xfe,
    0xf0, 0x01, 0xfd, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xfe, 0xfe,
    0xf0, 0x00, 0x7e, 0xff, 0xf8, 0x00, 0x1f, 0xf9, 0xfe, 0xfe,
    0xf0, 0x3f, 0xff, 0x7f, 0xe0, 0x00, 0x07, 0xe7, 0xfe, 0x7e,
    0xf0, 0x3f, 0xff, 0xdf, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0x7e,
    0xf0, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x7e,
    0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x7e,
    0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x7e,
    0xf8, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfe, 0x7e,
    0xfc, 0x10, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfe, 0x7e,
    0xfc, 0x10, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x7e,
    0xfc, 0x08, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfe, 0x7e,
    0xfe, 0x0c, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xfe, 0x7e,
    0xfe, 0x04, 0x00, 0x00, 0x01, 0xff, 0x3f, 0xbf, 0xfc, 0x7e,
    0xff, 0x06, 0x00, 0x00, 0x00, 0x7f, 0xbf, 0xdf, 0xf8, 0xfe,
    0xff, 0x03, 0x00, 0x00, 0x00, 0xcf, 0xff, 0xdf, 0xf8, 0xfe,
    0xff, 0x83, 0x80, 0x00, 0x00, 0xfb, 0xff, 0xdf, 0xe0, 0xfe,
    0xff, 0x81, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0xbf, 0xc1, 0xfe,
    0xff, 0xc1, 0xe0, 0x00, 0x00, 0x1f, 0xff, 0x3f, 0x01, 0xfe,
    0xff, 0xe0, 0xf0, 0x00, 0x00, 0x03, 0xfe, 0xff, 0xff, 0xfe,
    0xff, 0xf0, 0x78, 0x00, 0x00, 0x00, 0x7d, 0xff, 0xff, 0xfe,
    0xff, 0xf8, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe,
    0xff, 0xf8, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe,
    0xff, 0xfc, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe,
    0xff, 0xfe, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe,
    0xff, 0xff, 0x87, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe,
    0xff, 0xff, 0xc3, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe,
    0xff, 0xff, 0xe1, 0xff, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe,
    0xff, 0xff, 0xf8, 0xff, 0xe0, 0x00, 0x00, 0x1f, 0xff, 0xfe,
    0xff, 0xff, 0xfc, 0x7f, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0x1f, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe};

class Oled : public Screen
{
public:
  Oled(int okButton)
  {

    Wire.begin();
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for (;;)
        ; // Don't proceed, loop forever
    }

    this->okButton = okButton;
  }

  ~Oled() {}

  // Screen with time, temperature, ec and ph
  void mainScreen(char *_time, float temperature, float ec, float phFinal) override
  {
    // display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE, BLACK); // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
    display.println(_time);
    display.setCursor(0, 20); // Start at top-left corner
    display.setTextSize(1);   // Draw 2X-scale text
    display.print(F("Temp: "));
    display.print(temperature);
    display.print(F(" "));
    display.cp437(true);
    display.write(167); // 174 175 16 17
    display.println(F("C"));

    display.setCursor(0, 30); // Start at top-left corner
    display.print(F("EC: "));
    display.print(ec);
    display.print(F(" "));
    display.write(230);
    display.println(F("S/cm"));

    display.setCursor(0, 40); // Start at top-left corner
    display.print(F("PH: "));
    display.println(phFinal);

    display.display();
  }

  void initScreen() override
  {
    drawbitmap(); // Draw logo bitmap image
    delay(3000);

    initMessages();
    delay(2000);

    drawroundrect();
    delay(1000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 30);
    display.println(F("Ready!!!"));
    display.display();
    delay(1000);
    // Clear the buffer
    display.clearDisplay();
    display.display();

    // Turning off the dispaly OLED
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }

  // Show the rele symbols on screen
  void showReleSymbol(bool *rele, byte num) override
  {
    display.clearDisplay();
    for (int p = 0; p < num; p++)
    {
      if (!rele[p])
      {
        display.drawCircle(70 + p * 12, 55, 5, WHITE);
      }
      else
      {
        display.fillCircle(70 + p * 12, 55, 5, INVERSE);
      }
    }
    display.display();
  }

  void showMonitoring(const char *key, float value, float temperature) override
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.println(F("Monitoring"));
    display.drawFastHLine(0, 16, 128, WHITE);

    display.setCursor(0, 28);
    display.setTextSize(1); // Draw 1X-scale text
    display.print(F("Temperature: "));
    display.print(temperature);
    display.print(F(" "));
    display.cp437(true);
    display.write(167);
    display.println(F("C"));
    display.setCursor(0, 45);
    display.print(key);
    display.print(F(": "));
    display.println(value);
    display.fillRect(80, 50, 40, 20, WHITE);
    display.setTextColor(INVERSE); // Draw INVERSE text
    display.setCursor(88, 53);
    display.println(F("Exit"));

    display.display();
  }

  void turnOnLight()
  {
    display.ssd1306_command(SSD1306_DISPLAYON);
  }

  void turnOffLight()
  {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }

  // Showing menu page where there are the options to choose
  void showYesOrNoSelection1(byte selectedItem, int rowNumber, String *listMenu) override
  {
    head("CHOICE");
    if (rowNumber < 4)
    {
      showYesNoDispaly("turn on the ", listMenu, rowNumber);
    }
    else
    {
      showYesNoDispaly(" ", listMenu, rowNumber);
    }
    yesAndNoButton(selectedItem);
    display.display();
  }

  // Showing connection status on screen
  void connectionStatusPage(const char *connStatus) override
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("Connection"));
    display.println(F("status: "));
    display.println(connStatus);
    display.setTextSize(1);
    display.fillRect(80, 50, 40, 20, WHITE);
    display.setTextColor(INVERSE); // Draw INVERSE text
    display.setCursor(88, 53);
    display.println(F("Exit"));
    display.display();
  }

protected:
  void initMessages(void)
  {
    display.clearDisplay();
    display.setTextSize(2);      // Normal 2x pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 20);
    display.println(F("AQUARIUM"));
    display.setCursor(65, 41);
    display.println(F("PIA12"));
    display.display();
    // Scroll from left to right, only one string with (0x00, 0x04)
    display.startscrollright(0x00, 0x04);
    delay(1500);
    display.stopscroll();
    display.clearDisplay();
    display.setCursor(20, 20);
    display.println(F("AQUARIUM"));
    display.setCursor(65, 41);
    display.println(F("PIA12"));
    display.display();
    // Scroll from right to left, only second string with (0x05, 0x07)
    display.startscrollleft(0x05, 0x07);
    delay(2000);
    display.stopscroll();
  }

  void drawroundrect(void)
  {
    display.clearDisplay();
    for (int16_t i = 0; i < display.height() / 2 - 2; i += 2)
    {
      display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i,
                            display.height() / 4, WHITE);
      display.display();
      delay(100);
    }
  }

  void drawbitmap(void)
  {
    display.clearDisplay();
    display.drawBitmap(
        (display.width() - LOGO_WIDTH) / 2,
        (display.height() - LOGO_HEIGHT) / 2,
        salmon, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
  }

  // Showing rows items with selecting arrows in circle way
  void showListMenu(int listSize, int rowItem, String *listMenu) override
  {
    int before, after;
    if (rowItem == 0)
    {
      before = listSize - 1;
      after = rowItem + 1;
    }
    else if (rowItem == listSize - 1)
    {
      before = rowItem - 1;
      after = 0;
    }
    else
    {
      before = rowItem - 1;
      after = rowItem + 1;
    }
    display.clearDisplay();
    display.cp437(true);
    display.setTextSize(2);
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.println(F("MENU'"));
    display.drawFastHLine(0, 16, 128, WHITE);
    display.setCursor(8, 20);
    display.write(24);
    display.setCursor(0, 34);
    display.write(27);
    display.setCursor(16, 34);
    display.write(26);
    display.setCursor(8, 46);
    display.write(25);
    display.setCursor(26, 25);
    display.setTextSize(1); // Draw 1X-scale text
    display.println(listMenu[before]);
    display.setCursor(34, 38);
    display.print(listMenu[rowItem]);
    display.setCursor(26, 51);
    display.println(listMenu[after]);
    display.display();
  }

  // Showing menu page where there are the options to choose 5
  void showYesOrNoSelection2(byte selectedItem, int rowNumber, String *listMenu) override
  {
    head("CHOICE");
    showYesNoDispaly("activate ", listMenu, rowNumber);
    yesAndNoButton(selectedItem);
    display.display();
  }

  // Showing menu page where there are the options to choose 9
  void showYesOrNoSelection3(byte selectedItem, int rowNumber, String *listMenu) override
  {
    head("CHOICE");
    showYesNoDispaly("enable ", listMenu, rowNumber);
    yesAndNoButton(selectedItem);
    display.display();
  }

  void showYesOrNoSelection4(byte selectedItem, int rowNumber, bool onOffTemp, bool onOffEc, bool onOffPh) override
  {
    head("CHOICE");
    display.setCursor(1, 20);
    display.setTextSize(1); // Normal 1:1 pixel scale
    if (rowNumber == 10)
    {
      showSendingActivation(onOffTemp);
      //        showSendingActivation(config.onOffTemperatureSending);
    }
    else if (rowNumber == 11)
    {
      showSendingActivation(onOffEc);
      //        showSendingActivation(config.onOffECSending);
    }
    else if (rowNumber == 12)
    {
      showSendingActivation(onOffPh);
      //        showSendingActivation(config.onOffPhSending);
    }
    display.println(F("?"));
    yesAndNoButton(selectedItem);
    display.display();
  }

  void yesAndNoButton(byte selectedItem)
  {
    display.setTextSize(2);
    if (selectedItem == 0)
    {
      showFillButton(13, 43, 36, 21, 14, "YES");
      showVoidButton(60, 43, 36, 21, 67, "NO");
    }
    else
    {
      showVoidButton(13, 43, 36, 21, 14, "YES");
      showFillButton(60, 43, 36, 21, 67, "NO");
    }
  }

  void showYesNoDispaly(String verb, String *listMenu, byte rowNumber)
  {
    display.setCursor(1, 20);
    display.setTextSize(1); // Normal 1:1 pixel scale
    display.println(F("Would you like to"));
    display.print(verb);
    display.print(listMenu[rowNumber]);
    display.println(F("?"));
  }

  void showVoidButton(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, String text)
  {
    display.drawRect(x0, y0, x1, y1, WHITE);
    display.setTextColor(WHITE); // Draw INVERSE text
    display.setCursor(x2, y0 + 3);
    display.println(text);
  }

  void showFillButton(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, String text)
  {
    display.fillRect(x0, y0, x1, y1, WHITE);
    display.setTextColor(INVERSE);
    display.setCursor(x2, y0 + 3);
    display.println(text);
  }

  // Used to get the right sensor status
  void showSendingActivation(bool item)
  {
    if (item)
    {
      display.print(F("It is activete "));
      display.print(F("would you like to deactivate"));
    }
    else
    {
      display.print(F("It is inactive "));
      display.print(F("would you like to activate"));
    }
  }

  // Waiting animation
  void waitingActionMenu() override
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(20, 0);
    display.println(waitingString);
    delay(1000);
    drawroundrect();
    delay(1000);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 30);
    display.println(F("Done!!!"));
    display.display();
    delay(1000);
  }

  void showLightsTimerSetting(int startHour, int startMinutes, int endHour, int endMinutes) override
  {
    display.clearDisplay();
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.println(F("TIMER"));
    display.drawFastHLine(0, 16, 128, WHITE);
    display.setTextSize(1);
    display.setCursor(0, 21);
    display.print(F("Start Timer: "));
    showStringNumber(75, 21, startHour);
    display.setCursor(87, 21);
    display.print(F(":"));
    showStringNumber(92, 21, startMinutes);
    display.setCursor(0, 35);
    display.print(F("End Timer: "));
    showStringNumber(75, 35, endHour);
    display.setCursor(87, 35);
    display.print(F(":"));
    showStringNumber(92, 35, endMinutes);
    foo();
    display.display();
  }

  void showStringNumber(byte i, byte j, int number) override
  {
    display.setCursor(i, j);
    if (number < 10)
    {
      display.print(F("0"));
      display.setCursor(i + 6, j);
      display.print(number);
    }
    else
    {
      display.print(number);
    }
  }

  void head(String const title)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.println(title);
    display.drawFastHLine(0, 16, 128, WHITE);
  }

  void foo()
  {
    display.setCursor(18, 55);
    display.print(F("BACK"));
    display.setCursor(88, 55);
    display.print(F("SET"));
    display.setTextSize(2);
    display.setCursor(5, 52);
    display.write(27);
    display.setCursor(108, 52);
    display.write(26);
  }

  void showTime(byte _hour, byte _minutes, String title) override
  {
    head("TIME");
    display.setTextSize(1);
    display.setCursor(0, 21);
    display.print(title);
    showStringNumber(45, 35, _hour);
    display.print(F(":"));
    showStringNumber(62, 35, _minutes);
    foo();
    display.display();
  }

  void showTimeDate(byte _hour, byte _minutes, int _day, int _month, int _year, String title) override
  {
    head("TIME/DATE");
    display.setTextSize(1);
    display.setCursor(0, 21);
    display.print(title);
    showStringNumber(45, 32, _hour);
    display.print(F(":"));
    showStringNumber(62, 32, _minutes);
    showStringNumber(35, 43, _day);
    display.print(F("/"));
    showStringNumber(52, 43, _month);
    display.print(F("/"));
    showStringNumber(71, 43, _year);
    foo();
    display.display();
  }

  // Managing the cursor to move inside the time selection menu
  void manageLightsTimerSetting(int row, String one, String two, int h, int m) override
  {
    display.clearDisplay();
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.print(one);
    display.print(F(" "));
    display.print(two);
    display.cp437(true);
    display.drawFastHLine(0, 16, 128, WHITE);
    display.setCursor(2, 20);
    display.write(24);
    display.setCursor(2, 40);
    display.write(25);
    display.setTextSize(1);
    display.setCursor(16, 21);
    display.print(F("Hours: "));
    showStringNumber(82, 21, h);
    display.setCursor(16, 35);
    display.print(F("Minutes: "));
    showStringNumber(82, 35, m);
    if (row == 0)
    {
      display.setTextSize(2);
      display.setCursor(70, 18);
      display.write(27);
      display.setCursor(96, 18);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 1)
    {
      display.setTextSize(2);
      display.setCursor(70, 32);
      display.write(27);
      display.setCursor(96, 32);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 2)
    {
      display.setTextSize(1);
      showFillButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 3)
    {
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showFillButton(73, 49, 31, 14, 78, "EXIT");
    }
    display.display();
  }

  // Managing the cursor that selects the temperature to manage the termometer
  void showDateSetting(int row, int y, int m, int d) override
  {
    head("NEW DATA");
    display.cp437(true);
    display.setCursor(2, 20);
    display.write(24);
    display.setCursor(2, 40);
    display.write(25);
    display.setTextSize(1);
    display.setCursor(16, 18);
    display.print(F("Year: "));
    showStringNumber(65, 18, y);
    display.setCursor(16, 27);
    display.print(F("Month: "));
    showStringNumber(65, 27, m);
    display.setCursor(16, 36);
    display.print(F("Day: "));
    showStringNumber(65, 36, d);
    if (row == 0)
    {
      display.setTextSize(2);
      display.setCursor(50, 15);
      display.write(27);
      display.setCursor(91, 15);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 1)
    {
      display.setTextSize(2);
      display.setCursor(52, 24);
      display.write(27);
      display.setCursor(84, 24);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 2)
    {
      display.setTextSize(2);
      display.setCursor(52, 32);
      display.write(27);
      display.setCursor(84, 32);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 3)
    {
      display.setTextSize(1);
      showFillButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 4)
    {
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showFillButton(73, 49, 31, 14, 78, "EXIT");
    }
    display.display();
  }

  void showActualTemperatureRange(byte _max, byte _min) override
  {
    display.clearDisplay();
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.print(F("RANGE"));
    display.print(F(" "));
    display.cp437(true);
    display.write(167);
    display.println(F("C"));
    display.drawFastHLine(0, 16, 128, WHITE);
    display.setTextSize(1);
    display.setCursor(0, 21);
    display.print(F("Min degrees: "));
    showStringNumber(75, 21, _min);
    display.print(F(" "));
    display.write(167);
    display.println(F("C"));
    display.setCursor(0, 35);
    display.print(F("Max degrees: "));
    showStringNumber(75, 35, _max);
    display.print(F(" "));
    display.write(167);
    display.println(F("C"));
    foo();
    display.display();
  }

  // Managing the cursor that selects the temperature to manage the termometer
  void showMinMaxRangeSetting(int row, String one, int h, int m) override
  {
    head("NEW RANGE");
    display.cp437(true);
    display.setCursor(2, 20);
    display.write(24);
    display.setCursor(2, 40);
    display.write(25);
    display.setTextSize(1);
    display.setCursor(16, 21);
    display.print(F("Min: "));
    showStringNumber(58, 21, h);
    display.print(F(" "));
    display.write(167);
    display.println("C");
    display.setCursor(16, 35);
    display.print(F("Max: "));
    showStringNumber(58, 36, m);
    display.print(F(" "));
    display.write(167);
    display.println("C");
    if (row == 0)
    {
      display.setTextSize(2);
      display.setCursor(45, 18);
      display.write(27);
      display.setCursor(90, 18);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 1)
    {
      display.setTextSize(2);
      display.setCursor(45, 30);
      display.write(27);
      display.setCursor(90, 30);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 2)
    {
      display.setTextSize(1);
      showFillButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 3)
    {
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showFillButton(73, 49, 31, 14, 78, "EXIT");
    }
    display.display();
  }

  void showManageFreqSetting(int row, int t, int e, int p) override
  {
    head("SET RATES");
    display.cp437(true);
    display.setCursor(2, 20);
    display.write(24);
    display.setCursor(2, 40);
    display.write(25);
    display.setTextSize(1);
    display.setCursor(16, 19);
    display.print(F("Temperature: "));
    display.setCursor(102, 19);
    display.print(24 / freqNumbber[t]);
    display.setCursor(16, 29);
    display.print(F("EC/TDS: "));
    display.setCursor(102, 29);
    display.print(24 / freqNumbber[e]);
    display.setCursor(16, 38);
    display.print(F("PH: "));
    display.setCursor(102, 38);
    display.print(24 / freqNumbber[p]);
    if (row == 0)
    {
      display.setTextSize(2);
      display.setCursor(90, 16);
      display.write(27);
      display.setCursor(114, 16);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 1)
    {
      display.setTextSize(2);
      display.setCursor(90, 26);
      display.write(27);
      display.setCursor(114, 26);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 2)
    {
      display.setTextSize(2);
      display.setCursor(90, 36);
      display.write(27);
      display.setCursor(114, 36);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 3)
    {
      display.setTextSize(1);
      showFillButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 4)
    {
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showFillButton(73, 49, 31, 14, 78, "EXIT");
    }
    display.display();
  }

  void showManageFreqFilterSetting(int row, int f) override
  {
    head("SET RATES");
    display.cp437(true);
    display.setCursor(2, 20);
    display.write(24);
    display.setCursor(2, 40);
    display.write(25);
    display.setTextSize(1);
    display.setCursor(16, 19);
    display.print(F("Filter: "));
    display.setCursor(102, 19);
    display.print(24 / freqNumbber[f]);
    if (row == 0)
    {
      display.setTextSize(2);
      display.setCursor(90, 16);
      display.write(27);
      display.setCursor(114, 16);
      display.write(26);
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 1)
    {
      display.setTextSize(1);
      showFillButton(30, 49, 24, 14, 35, "OK");
      showVoidButton(73, 49, 31, 14, 78, "EXIT");
    }
    else if (row == 2)
    {
      display.setTextSize(1);
      showVoidButton(30, 49, 24, 14, 35, "OK");
      showFillButton(73, 49, 31, 14, 78, "EXIT");
    }
    display.display();
  }

  void showWebRateSending(byte freqT, byte freqE, byte freqP)
  {
    head("WEB RATES");

    display.setTextSize(1);
    display.setCursor(0, 19);
    display.print(F("Temperature: "));
    if (freqNumbber[freqT] == 23)
    {
      showStringNumber(75, 20, 1);
    }
    else
    {
      showStringNumber(75, 20, 24 / freqNumbber[freqT]);
    }
    display.setCursor(0, 30);
    display.print(F("EC/TDS: "));
    if (freqE == 23)
    {
      showStringNumber(75, 30, 1);
    }
    else
    {
      showStringNumber(75, 30, 24 / freqNumbber[freqE]);
    }
    display.setCursor(0, 40);
    display.print(F("PH: "));
    if (freqP == 23)
    {
      showStringNumber(75, 40, 1);
    }
    else
    {
      showStringNumber(75, 40, 24 / freqNumbber[freqP]);
    }
    foo();
    display.display();
  }
};
#endif