#pragma once

#include <LiquidCrystal_I2C.h>

typedef const char hmss[10];
class MyLiquidCrystal : public LiquidCrystal_I2C{
public:
  MyLiquidCrystal(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
  void welcomeMessage();
  void updateValues(size_t, size_t, hmss, hmss, hmss, hmss);
};

extern MyLiquidCrystal lcd;  // set the LCD address to 0x27 for a 16 chars and 2 line display