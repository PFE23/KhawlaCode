#include "MyLiquidCrystal.hpp"
#include <Wire.h>

MyLiquidCrystal lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

MyLiquidCrystal::MyLiquidCrystal(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows):
  LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows){
}

void MyLiquidCrystal::welcomeMessage(){
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(6, 1);
  lcd.print("welcome");
  lcd.setCursor(2, 2);
  lcd.print("Flow Gest 1.0.0");
  lcd.setCursor(2, 3);
  lcd.print("Starting ");
  for (size_t i = 0; i < 6; i++){
    lcd.print(".");
    delay(300);
  }/**/
  
}

void MyLiquidCrystal::updateValues(size_t nb, size_t nb1
    , hmss mn, hmss mx, hmss tw, hmss tp){
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("Pieces : %03d --> %03d", nb, nb1);
  lcd.setCursor(0, 1);
  lcd.printf("MM %s %s", mn, mx);
  lcd.setCursor(0, 2);
  lcd.printf("TT %s %s", tw, tp);
  lcd.setCursor(0, 3);
  lcd.print("Min Max Twork Tpause");
  /*lcd.setCursor(0, 2);
  lcd.print("Flow Gest 1.0.0");
  lcd.setCursor(2, 3);
  lcd.print("Starting ");*/
}
