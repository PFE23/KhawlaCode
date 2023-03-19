#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <Arduino.h>

/**
 * @brief Defines a button type. A button require a pin and an interruption to handle its activation.
 * 
 */
class Button{
public:
  /**
   * @brief Construct a new Button object
   * 
   * @param pin Pin number
   */
  Button(uint8_t pin);
  /**
   * @brief Initilize button behavior
   * 
   * @param pFn pointer to interruption
   */
  void begin(void (*pFn) (void));
private:
  const uint8_t PIN;
  void (*pInterruptFn) (void);
};
#endif