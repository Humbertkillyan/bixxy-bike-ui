/*
  =====================================================
  BIXXY BIKE UI - Clignotants et Affichage de Statut
  =====================================================
  
  Projet: Interface utilisateur pour vélo électrique BIXXY
  Matériel: Seeed XIAO ESP32-S3 + Round Display
  
  Fonctionnalités:
  - Boutons d'interrupt pour clignotants (D1/D2)
  - Affichage des clignotants sur écran circulaire
  - Affichage vitesse/statut quand pas de cligno
  - Communication UART avec contrôleur moteur (D6/D7)
  
  Auteur: 
  Date: 2026-03-29
  =====================================================
*/

// ============= LIBRARIES =============
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

// Seeed GFX (must be before GFX library includes)
#include "driver.h"
#include <Adafruit_GFX.h>
#include <Arduino_GFX_Library.h>

// Custom headers
#include "config.h"
#include "buttons.h"
#include "uart.h"
#include "display.h"

// ============= DISPLAY OBJECT =============
Arduino_DataBusInterface *bus = new Arduino_HWSPI(
  GFX_NOT_DEFINED,  // DC
  SS,               // CS
  SCK,              // SCK
  MOSI,             // MOSI
  MISO              // MISO
);

Arduino_GFX *display = new Arduino_GC9A01(
  bus,
  GFX_NOT_DEFINED,  // RST
  0,                // rotation
  true              // IPS
);

// ============= GLOBAL STATE VARIABLES =============
volatile bool turnSignalLeft  = false;
volatile bool turnSignalRight = false;
float currentSpeed = 0.0;
String currentStatus = "ARRET";

volatile bool buttonLeftChanged  = false;
volatile bool buttonRightChanged = false;

unsigned long lastDisplayUpdate = 0;
unsigned long lastSerialSend    = 0;

// ============= SETUP =============
void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n=== BIXXY BIKE UI - Initialization ===");
  Serial.println("Board: Seeed XIAO ESP32-S3");
  Serial.println("Display: Round Display 1.28\" GC9A01");
  
  // Initialize display
  Serial.println("[1/4] Initializing display...");
  initDisplay();
  display->fillScreen(COLOR_BLACK);
  display->setTextColor(COLOR_WHITE);
  display->setTextSize(2);
  display->setCursor(10, 50);
  display->println("STARTING...");
  display->println("Initializing...");
  delay(500);
  
  // Initialize buttons
  Serial.println("[2/4] Initializing buttons...");
  initButtons();
  
  // Initialize UART
  Serial.println("[3/4] Initializing UART...");
  initUART();
  
  Serial.println("[4/4] Setup complete!");
  Serial.println("=== System Ready ===\n");
  
  // Initial display
  updateDisplay();
  
  lastDisplayUpdate = millis();
  lastSerialSend = millis();
}

// ============= MAIN LOOP =============
void loop() {
  unsigned long currentTime = millis();
  
  // ========== RECEIVE DATA FROM MOTOR CONTROLLER ==========
  receiveMotorStatus();
  
  // ========== SEND BUTTON STATUS IF CHANGED ==========
  if (buttonLeftChanged || buttonRightChanged) {
    sendButtonStatus();
    buttonLeftChanged = false;
    buttonRightChanged = false;
    lastSerialSend = currentTime;
  }
  
  // ========== PERIODIC SERIAL STATUS OUTPUT ==========
  if (currentTime - lastSerialSend > 1000) {
    Serial.print("STATUS -> Left: ");
    Serial.print(turnSignalLeft ? "ON" : "OFF");
    Serial.print(" | Right: ");
    Serial.print(turnSignalRight ? "ON" : "OFF");
    Serial.print(" | Speed: ");
    Serial.print(currentSpeed);
    Serial.print(" km/h | Status: ");
    Serial.println(currentStatus);
    
    lastSerialSend = currentTime;
  }
  
  // ========== UPDATE DISPLAY ==========
  if (currentTime - lastDisplayUpdate > UPDATE_INTERVAL) {
    updateDisplay();
    lastDisplayUpdate = currentTime;
  }
  
  delay(10);  // Small delay to prevent WDT issues
}

// ============= SERIAL EVENT (ALTERNATIVE INPUT) =============
void serialEvent() {
  // For debugging: receive commands via Serial Monitor
  while (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.length() == 0) return;
    
    // Simulate commands for testing
    if (command == "left") {
      turnSignalLeft = true;
      turnSignalRight = false;
      Serial.println("-> Left turn signal ON");
    }
    else if (command == "right") {
      turnSignalLeft = false;
      turnSignalRight = true;
      Serial.println("-> Right turn signal ON");
    }
    else if (command == "off") {
      turnSignalLeft = false;
      turnSignalRight = false;
      Serial.println("-> All turn signals OFF");
    }
    else if (command.startsWith("speed:")) {
      String speedStr = command.substring(6);
      currentSpeed = speedStr.toFloat();
      Serial.print("-> Speed set to ");
      Serial.print(currentSpeed);
      Serial.println(" km/h");
    }
    else if (command.startsWith("status:")) {
      String statusStr = command.substring(7);
      currentStatus = statusStr;
      Serial.print("-> Status set to ");
      Serial.println(currentStatus);
    }
    else {
      Serial.println("Commands: left, right, off, speed:XX.X, status:ROULE|ARRET|FREIN");
    }
  }
}
