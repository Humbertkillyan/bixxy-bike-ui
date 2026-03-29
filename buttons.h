#ifndef BUTTONS_H
#define BUTTONS_H

#include "config.h"

// ============= BUTTON STATE VARIABLES =============
volatile bool turnSignalLeft  = false;
volatile bool turnSignalRight = false;
volatile unsigned long lastLeftInterrupt  = 0;
volatile unsigned long lastRightInterrupt = 0;
volatile bool buttonLeftChanged  = false;
volatile bool buttonRightChanged = false;

// ============= INTERRUPT HANDLERS =============

void IRAM_ATTR handleButtonLeftInterrupt() {
  unsigned long currentTime = millis();
  
  // Debounce: ignore if less than DEBOUNCE_DELAY ms since last interrupt
  if (currentTime - lastLeftInterrupt > DEBOUNCE_DELAY) {
    lastLeftInterrupt = currentTime;
    turnSignalLeft = !turnSignalLeft;
    buttonLeftChanged = true;
    
    // Cancel right signal if left is activated
    if (turnSignalLeft && turnSignalRight) {
      turnSignalRight = false;
      buttonRightChanged = true;
    }
  }
}

void IRAM_ATTR handleButtonRightInterrupt() {
  unsigned long currentTime = millis();
  
  // Debounce: ignore if less than DEBOUNCE_DELAY ms since last interrupt
  if (currentTime - lastRightInterrupt > DEBOUNCE_DELAY) {
    lastRightInterrupt = currentTime;
    turnSignalRight = !turnSignalRight;
    buttonRightChanged = true;
    
    // Cancel left signal if right is activated
    if (turnSignalRight && turnSignalLeft) {
      turnSignalLeft = false;
      buttonLeftChanged = true;
    }
  }
}

void initButtons() {
  // Configure button pins as inputs with internal pull-up
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  
  // Attach interrupts (FALLING = when button pressed to LOW)
  attachInterrupt(digitalPinToInterrupt(BUTTON_LEFT_PIN), handleButtonLeftInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_RIGHT_PIN), handleButtonRightInterrupt, FALLING);
}

#endif // BUTTONS_H
