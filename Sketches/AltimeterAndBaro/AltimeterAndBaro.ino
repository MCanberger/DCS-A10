//Uncomment for testmode (No DCS needed)
//#define TEST

#ifndef TEST
  #define DCSBIOS_IRQ_SERIAL
  //#define DCSBIOS_RS485_SLAVE 2
  
  #include "DcsBios.h"
#endif

#define TXENABLE_PIN 2

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "characters.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define TCAAddress 0x70
#define AltAddress 0
#define HGAddress 1

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

int updateInterval = 100; //the number of milliseconds between updates

struct scrollDigit {
  int digit;
  int y;
};

struct disp {
  Adafruit_SSD1306 display;
  int address;
  int width;
  int numberOfDigits;
  scrollDigit digits[5];
};


disp displays[2] = 
{
  {Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), 0, 24, 5, {{0,0},{0,0},{0,0},{0,0},{0,0}}},
  {Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), 1, 16, 4, {{0,0},{0,0},{0,0},{0,0},{0,0}}}
};

void TCASelect(uint8_t addr) {
  if (addr > 7) return;
 
  Wire.beginTransmission(TCAAddress);
  Wire.write(1 << addr);
  Wire.endTransmission();  
}

void setup() {
  
  for (int i = 0; i<2; i++)
  {
    TCASelect(displays[i].address);
    if(!displays[i].display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
      for(;;); // Don't proceed, loop forever
    }
  }

  #ifdef TEST
    Serial.begin(9600);
  #else
    DcsBios::setup();
  #endif

  UpdateDisplays();
}

void UpdateDisplays()
{
  for (int j = 0; j<2; j++)
  {
    TCASelect(displays[j].address);
    displays[j].display.clearDisplay();
    for (int i = 0; i < displays[j].numberOfDigits; i++)
    {
      printScrollingDigit(displays[j].digits[i].digit, displays[j].width, displays[j].digits[i].y, i + 1, &displays[j]);
    }
    if (displays[j].width == 16)
    {
      displays[j].display.fillRect(0, 25, 128, 7, BLACK);
    }
    displays[j].display.display();
  }
}

void printScrollingDigit(int digit, int width, int y, int pos, disp *display)
{
//  int width = 24;
  int x = (width * pos) - width + pos;

  if (width == 24)
  {
    switch (digit)
    {
      case -1: display->display.drawBitmap(x, y, c24_Empty, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_1, 24, 32, 1); break;
      case 1: display->display.drawBitmap(x, y, c24_1, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_2, 24, 32, 1); break;
      case 2: display->display.drawBitmap(x, y, c24_2, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_3, 24, 32, 1); break;
      case 3: display->display.drawBitmap(x, y, c24_3, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_4, 24, 32, 1); break;
      case 4: display->display.drawBitmap(x, y, c24_4, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_5, 24, 32, 1); break;
      case 5: display->display.drawBitmap(x, y, c24_5, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_6, 24, 32, 1); break;
      case 6: display->display.drawBitmap(x, y, c24_6, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_7, 24, 32, 1); break;
      case 7: display->display.drawBitmap(x, y, c24_7, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_8, 24, 32, 1); break;
      case 8: display->display.drawBitmap(x, y, c24_8, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_9, 24, 32, 1); break;
      case 9: display->display.drawBitmap(x, y, c24_9, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_0, 24, 32, 1); break;
      default: display->display.drawBitmap(x, y, c24_0, 24, 32, 1); display->display.drawBitmap(x, y+33, c24_1, 24, 32, 1); break;
    }
  }
  else
  {
    switch (digit)
    {
      case -1: display->display.drawBitmap(x, y, c16_Empty, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_1, 16, 24, 1); break;
      case 1: display->display.drawBitmap(x, y, c16_1, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_2, 16, 24, 1); break;
      case 2: display->display.drawBitmap(x, y, c16_2, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_3, 16, 24, 1); break;
      case 3: display->display.drawBitmap(x, y, c16_3, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_4, 16, 24, 1); break;
      case 4: display->display.drawBitmap(x, y, c16_4, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_5, 16, 24, 1); break;
      case 5: display->display.drawBitmap(x, y, c16_5, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_6, 16, 24, 1); break;
      case 6: display->display.drawBitmap(x, y, c16_6, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_7, 16, 24, 1); break;
      case 7: display->display.drawBitmap(x, y, c16_7, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_8, 16, 24, 1); break;
      case 8: display->display.drawBitmap(x, y, c16_8, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_9, 16, 24, 1); break;
      case 9: display->display.drawBitmap(x, y, c16_9, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_0, 16, 24, 1); break;
      default: display->display.drawBitmap(x, y, c16_0, 16, 24, 1); display->display.drawBitmap(x, y+25, c16_1, 16, 24, 1); break;
    }
  }
}


void onAlt10000FtChange(unsigned int newValue)
{
  unsigned int mappedValue = newValue / 6553;
  unsigned int y = map(newValue, mappedValue * 6553, mappedValue * 6553 + 6553, 0, -33);

  if (mappedValue == 0)
  {
    mappedValue = -1;
  }
  displays[AltAddress].digits[0].digit = mappedValue;
  displays[AltAddress].digits[0].y = y;
}

void onAlt1000FtChange(unsigned int newValue)
{
  unsigned int mappedValue = newValue / 6553;
  unsigned int y = map(newValue, mappedValue * 6553, mappedValue * 6553 + 6553, 0, -33);

  displays[AltAddress].digits[1].digit = mappedValue;
  displays[AltAddress].digits[1].y = y;
}

void onAlt100FtChange(unsigned int newValue)
{
  unsigned int mappedValue = newValue / 6553;
  unsigned int y = map(newValue, mappedValue * 6553, mappedValue * 6553 + 6553, 0, -33);

  displays[AltAddress].digits[2].digit = mappedValue;
  displays[AltAddress].digits[2].y = y;
}

void onBaro0Change(unsigned int newValue)
{
  unsigned int mappedValue = newValue / 6554;
  unsigned int y = map(newValue, mappedValue * 6554, mappedValue * 6554 + 6554, 0, -25);

  displays[HGAddress].digits[3].digit = mappedValue;
  displays[HGAddress].digits[3].y = y;
}

void onBaro1Change(unsigned int newValue)
{
  unsigned int mappedValue = newValue / 6554;
  unsigned int y = map(newValue, mappedValue * 6554, mappedValue * 6554 + 6554, 0, -25);

  displays[HGAddress].digits[2].digit = mappedValue;
  displays[HGAddress].digits[2].y = y;
}

void onBaro2Change(unsigned int newValue)
{
  unsigned int mappedValue = newValue / 6554;
  unsigned int y = map(newValue, mappedValue * 6554, mappedValue * 6554 + 6554, 0, -25);

  displays[HGAddress].digits[1].digit = mappedValue;
  displays[HGAddress].digits[1].y = y;
}

void onBaro3Change(unsigned int newValue)
{
  unsigned int mappedValue = newValue / 6554;
  unsigned int y = map(newValue, mappedValue * 6554, mappedValue * 6554 + 6554, 0, -25);

  displays[HGAddress].digits[0].digit = mappedValue;
  displays[HGAddress].digits[0].y = y;
}

#ifndef TEST
  DcsBios::IntegerBuffer Alt10000FtBuffer(0x1080, 0xffff, 0, onAlt10000FtChange);
  DcsBios::IntegerBuffer Alt1000FtBuffer(0x1082, 0xffff, 0, onAlt1000FtChange);
  DcsBios::IntegerBuffer Alt100FtBuffer(0x1084, 0xffff, 0, onAlt100FtChange);
  DcsBios::IntegerBuffer Baro0Buffer(0x1086, 0xffff, 0, onBaro0Change);
  DcsBios::IntegerBuffer Baro1Buffer(0x1088, 0xffff, 0, onBaro1Change);
  DcsBios::IntegerBuffer Baro2Buffer(0x108a, 0xffff, 0, onBaro2Change);
  DcsBios::IntegerBuffer Baro3Buffer(0x108c, 0xffff, 0, onBaro3Change);
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
        onAlt10000FtChange(i);
        onAlt1000FtChange(i);
        onAlt100FtChange(i);
        onBaro0Change(i);
        onBaro1Change(i);
        onBaro2Change(i);
        onBaro3Change(i);
        i += 400;
    #endif
    UpdateDisplays();
  }
}
