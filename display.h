#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include <Adafruit_GFX.h>
#include <Arduino_GFX_Library.h>

// Display object (initialized in main sketch)
extern Arduino_GFX *display;

// ============= DISPLAY STATE VARIABLES =============
extern volatile bool turnSignalLeft;
extern volatile bool turnSignalRight;
extern float currentSpeed;
extern String currentStatus;  // "ROULE", "ARRET", "FREIN"

// ============= FUNCTION DECLARATIONS =============

void initDisplay();
void updateDisplay();
void drawTurnSignalLeft();
void drawTurnSignalRight();
void drawSpeedAndStatus();
void drawClearDisplay();
void drawCenterText(const char* text, int y, uint16_t color, int size);
void drawArrowLeft(int x, int y, int size, uint16_t color);
void drawArrowRight(int x, int y, int size, uint16_t color);

// ============= FUNCTION IMPLEMENTATIONS =============

void initDisplay() {
  // GC9A01 initialization is handled by Arduino_GFX library
  display->begin();
  display->fillScreen(COLOR_BLACK);
  display->setTextColor(COLOR_WHITE);
  display->setTextSize(TEXT_SIZE);
}

void drawClearDisplay() {
  display->fillScreen(COLOR_BLACK);
}

void drawCenterText(const char* text, int y, uint16_t color, int size) {
  display->setTextSize(size);
  display->setTextColor(color);
  
  int16_t x1, y1;
  uint16_t w, h;
  display->getTextBounds((char*)text, 0, 0, &x1, &y1, &w, &h);
  
  int x = (SCREEN_WIDTH - w) / 2;
  display->setCursor(x, y);
  display->println(text);
}

void drawArrowLeft(int cx, int cy, int size, uint16_t color) {
  // Pointed left arrow
  display->fillTriangle(
    cx + size/2, cy - size/2,  // top right
    cx + size/2, cy + size/2,  // bottom right
    cx - size/2, cy,           // left point
    color
  );
}

void drawArrowRight(int cx, int cy, int size, uint16_t color) {
  // Pointed right arrow
  display->fillTriangle(
    cx - size/2, cy - size/2,  // top left
    cx - size/2, cy + size/2,  // bottom left
    cx + size/2, cy,           // right point
    color
  );
}

void drawTurnSignalLeft() {
  drawClearDisplay();
  drawCenterText("CLIGNO", 30, COLOR_YELLOW, 2);
  drawCenterText("GAUCHE", 60, COLOR_YELLOW, 2);
  drawArrowLeft(120, 140, 60, COLOR_ORANGE);
}

void drawTurnSignalRight() {
  drawClearDisplay();
  drawCenterText("CLIGNO", 30, COLOR_YELLOW, 2);
  drawCenterText("DROITE", 60, COLOR_YELLOW, 2);
  drawArrowRight(120, 140, 60, COLOR_ORANGE);
}

void drawSpeedAndStatus() {
  drawClearDisplay();
  
  // Speed display at top
  char speedStr[10];
  dtostrf(currentSpeed, 5, 1, speedStr);
  
  display->setTextSize(3);
  display->setTextColor(COLOR_GREEN);
  drawCenterText(speedStr, 40, COLOR_GREEN, 3);
  
  display->setTextSize(1);
  display->setTextColor(COLOR_CYAN);
  drawCenterText("km/h", 80, COLOR_CYAN, 1);
  
  // Status display at bottom
  display->setTextSize(2);
  
  uint16_t statusColor = COLOR_WHITE;
  if (currentStatus == "ROULE") {
    statusColor = COLOR_GREEN;
  } else if (currentStatus == "FREIN") {
    statusColor = COLOR_RED;
  } else if (currentStatus == "ARRET") {
    statusColor = COLOR_GRAY;
  }
  
  drawCenterText(currentStatus.c_str(), 160, statusColor, 2);
}

void updateDisplay() {
  // Show turn signal if active
  if (turnSignalLeft) {
    drawTurnSignalLeft();
  } 
  else if (turnSignalRight) {
    drawTurnSignalRight();
  }
  else {
    // Show speed and status
    drawSpeedAndStatus();
  }
}

#endif // DISPLAY_H
