//Uncomment for testmode (No DCS needed)
#define TEST

#ifndef TEST
  #define DCSBIOS_IRQ_SERIAL
  #include "DcsBios.h"
#endif


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "characters.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

//Comment for barometric pressure
#define ALTIMETER

int updateInterval = 100; //the number of milliseconds between updates

struct scrollDigit {
  int digit; //The digit that is displayed
  int y; // The vertical position of the digit
};

struct disp {
  Adafruit_SSD1306 display;
  int width;
  int numberOfDigits;
  scrollDigit digits[5];
};

#ifdef ALTIMETER
  disp oled = {Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), 24, 5, {{0,0},{0,0},{0,0},{0,0},{0,0}}};
#else
  disp oled = {Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), 16, 4, {{0,0},{0,0},{0,0},{0,0},{0,0}}};
#endif

void setup() {
  if(!oled.display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    for(;;); // Don't proceed, loop forever
  }
#ifdef TEST
  Serial.begin(9600);
#else
  DcsBios::setup();
#endif
}

void UpdateDisplay()
{
  oled.display.clearDisplay();
  for (int i = 0; i < oled.numberOfDigits; i++)
  {
    printScrollingDigit(oled.digits[i].digit, oled.width, oled.digits[i].y, i + 1, &oled);
  }
  //Clear the area below the the numbers if we are using the small font
  if (oled.width == 16)
  {
    oled.display.fillRect(0, 25, 67, 7, BLACK);
  }
  
  oled.display.display();
}

int YPos()
{
  return (oled.width + 9) * -1;
}


void printScrollingDigit(int digit, int width, int y, int pos, disp *oled)
{
  int x = (width * pos) - width + pos;
#ifdef ALTIMETER
    switch (digit)
    {
      case -1: oled->display.drawBitmap(x, y, c24_Empty, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_1, 24, 32, 1); break;
      case 1: oled->display.drawBitmap(x, y, c24_1, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_2, 24, 32, 1); break;
      case 2: oled->display.drawBitmap(x, y, c24_2, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_3, 24, 32, 1); break;
      case 3: oled->display.drawBitmap(x, y, c24_3, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_4, 24, 32, 1); break;
      case 4: oled->display.drawBitmap(x, y, c24_4, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_5, 24, 32, 1); break;
      case 5: oled->display.drawBitmap(x, y, c24_5, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_6, 24, 32, 1); break;
      case 6: oled->display.drawBitmap(x, y, c24_6, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_7, 24, 32, 1); break;
      case 7: oled->display.drawBitmap(x, y, c24_7, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_8, 24, 32, 1); break;
      case 8: oled->display.drawBitmap(x, y, c24_8, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_9, 24, 32, 1); break;
      case 9: oled->display.drawBitmap(x, y, c24_9, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_0, 24, 32, 1); break;
      default: oled->display.drawBitmap(x, y, c24_0, 24, 32, 1); oled->display.drawBitmap(x, y+33, c24_1, 24, 32, 1); break;
    }
#else
    switch (digit)
    {
      case -1: oled->display.drawBitmap(x, y, c16_Empty, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_1, 16, 24, 1); break;
      case 1: oled->display.drawBitmap(x, y, c16_1, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_2, 16, 24, 1); break;
      case 2: oled->display.drawBitmap(x, y, c16_2, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_3, 16, 24, 1); break;
      case 3: oled->display.drawBitmap(x, y, c16_3, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_4, 16, 24, 1); break;
      case 4: oled->display.drawBitmap(x, y, c16_4, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_5, 16, 24, 1); break;
      case 5: oled->display.drawBitmap(x, y, c16_5, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_6, 16, 24, 1); break;
      case 6: oled->display.drawBitmap(x, y, c16_6, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_7, 16, 24, 1); break;
      case 7: oled->display.drawBitmap(x, y, c16_7, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_8, 16, 24, 1); break;
      case 8: oled->display.drawBitmap(x, y, c16_8, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_9, 16, 24, 1); break;
      case 9: oled->display.drawBitmap(x, y, c16_9, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_0, 16, 24, 1); break;
      default: oled->display.drawBitmap(x, y, c16_0, 16, 24, 1); oled->display.drawBitmap(x, y+25, c16_1, 16, 24, 1); break;
    }
#endif
}

#ifdef ALTIMETER
  void onAlt10000FtChange(unsigned int newValue)
  {
    unsigned int mappedValue = newValue / 6553;
    unsigned int y = map(newValue, mappedValue * 6553, mappedValue * 6553 + 6553, 0, YPos());

    if (mappedValue == 0)
    {
      mappedValue = -1;
    }
    #ifdef TEST
      Serial.println(mappedValue);
    #endif
    oled.digits[0].digit = mappedValue;
    oled.digits[0].y = y;
  }
  
  void onAlt1000FtChange(unsigned int newValue)
  {
    unsigned int mappedValue = newValue / 6553;
    unsigned int y = map(newValue, mappedValue * 6553, mappedValue * 6553 + 6553, 0, YPos());
  
    oled.digits[1].digit = mappedValue;
    oled.digits[1].y = y;
  }
  
  void onAlt100FtChange(unsigned int newValue)
  {
    unsigned int mappedValue = newValue / 6553;
    unsigned int y = map(newValue, mappedValue * 6553, mappedValue * 6553 + 6553, 0, YPos());
  
    oled.digits[2].digit = mappedValue;
    oled.digits[2].y = y;
  }
  
  #ifndef TEST
    DcsBios::IntegerBuffer Alt10000FtBuffer(0x1080, 0xffff, 0, onAlt10000FtChange);
    DcsBios::IntegerBuffer Alt1000FtBuffer(0x1082, 0xffff, 0, onAlt1000FtChange);
    DcsBios::IntegerBuffer Alt100FtBuffer(0x1084, 0xffff, 0, onAlt100FtChange);
  #endif
#else
  void onBaro0Change(unsigned int newValue)
  {
    unsigned int mappedValue = newValue / 6554;
    unsigned int y = map(newValue, mappedValue * 6554, mappedValue * 6554 + 6554, 0, YPos());
  
    oled.digits[3].digit = mappedValue;
    oled.digits[3].y = y;
  }
  
  void onBaro1Change(unsigned int newValue)
  {
    unsigned int mappedValue = newValue / 6554;
    unsigned int y = map(newValue, mappedValue * 6554, mappedValue * 6554 + 6554, 0, YPos());
  
    oled.digits[2].digit = mappedValue;
    oled.digits[2].y = y;
  }
  
  void onBaro2Change(unsigned int newValue)
  {
    unsigned int mappedValue = newValue / 6554;
    unsigned int y = map(newValue, mappedValue * 6554, mappedValue * 6554 + 6554, 0, YPos());
  
    oled.digits[1].digit = mappedValue;
    oled.digits[1].y = y;
  }
  
  void onBaro3Change(unsigned int newValue)
  {
    unsigned int mappedValue = newValue / 6554;
    unsigned int y = map(newValue, mappedValue * 6554, mappedValue * 6554 + 6554, 0, YPos());
  
    oled.digits[0].digit = mappedValue;
    oled.digits[0].y = y;
  }

  #ifndef TEST
    DcsBios::IntegerBuffer Baro0Buffer(0x1086, 0xffff, 0, onBaro0Change);
    DcsBios::IntegerBuffer Baro1Buffer(0x1088, 0xffff, 0, onBaro1Change);
    DcsBios::IntegerBuffer Baro2Buffer(0x108a, 0xffff, 0, onBaro2Change);
    DcsBios::IntegerBuffer Baro3Buffer(0x108c, 0xffff, 0, onBaro3Change);
  #endif
#endif

unsigned long time = 0;

unsigned int i = 0;
void loop() {
  #ifndef TEST
    DcsBios::loop();
  #endif

  time = millis();

  if (time % updateInterval == 0)
  {
    #ifdef TEST
      #ifdef ALTIMETER
        onAlt10000FtChange(i);
        onAlt1000FtChange(i);
        onAlt100FtChange(i);
        i += 400;
      #else
        onBaro0Change(i);
        onBaro1Change(i);
        onBaro2Change(i);
        onBaro3Change(i);
        i += 1;
      #endif
      Serial.println(i);
    #endif
    UpdateDisplay();
  }
}
