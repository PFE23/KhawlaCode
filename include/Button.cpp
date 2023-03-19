#include "Button.hpp"

Button::Button(uint8_t pin):PIN(pin){
}
void Button::begin(void (*pFn) (void)){
  pinMode(PIN, INPUT_PULLDOWN);
  pInterruptFn = pFn;
  attachInterrupt(PIN, *pInterruptFn, RISING);
}