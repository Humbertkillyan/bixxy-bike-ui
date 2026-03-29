#ifndef UART_H
#define UART_H

#include "config.h"

// ============= FUNCTION DECLARATIONS =============

void initUART();
void sendButtonStatus();
void receiveMotorStatus();
void parseMotorData(String data);

// ============= FUNCTION IMPLEMENTATIONS =============

void initUART() {
  // Initialize Hardware UART1 (Pins D6=TX, D7=RX correspond to GPIO43/44)
  Serial1.begin(UART_BAUDRATE, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
}

void sendButtonStatus() {
  // Send turn signal status to motor controller
  if (turnSignalLeft) {
    Serial1.println(MSG_TURN_LEFT);
  }
  else if (turnSignalRight) {
    Serial1.println(MSG_TURN_RIGHT);
  }
  else if (!turnSignalLeft && !turnSignalRight) {
    Serial1.println(MSG_TURN_CANCEL);
  }
  
  // Debug output
  Serial.print("SENT: ");
  if (turnSignalLeft) Serial.println(MSG_TURN_LEFT);
  else if (turnSignalRight) Serial.println(MSG_TURN_RIGHT);
  else Serial.println(MSG_TURN_CANCEL);
}

void receiveMotorStatus() {
  // Read incoming data from motor controller
  while (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    data.trim();
    
    if (data.length() > 0) {
      Serial.print("RECEIVED: ");
      Serial.println(data);
      parseMotorData(data);
    }
  }
}

void parseMotorData(String data) {
  // Expected format: "STATUS:ROULE:25.5" or "STATUS:ARRET:0" or "STATUS:FREIN:15.2"
  // Or simpler: "ROULE,25.5" or "ARRET,0"
  
  if (data.startsWith("STATUS:")) {
    // Remove "STATUS:" prefix
    data = data.substring(7);
    
    // Find comma or colon separator
    int separatorPos = data.indexOf(':');
    if (separatorPos == -1) {
      separatorPos = data.indexOf(',');
    }
    
    if (separatorPos != -1) {
      // Extract status
      String status = data.substring(0, separatorPos);
      currentStatus = status;
      
      // Extract speed
      String speedStr = data.substring(separatorPos + 1);
      currentSpeed = speedStr.toFloat();
      
      Serial.print("Parsed - Status: ");
      Serial.print(currentStatus);
      Serial.print(" | Speed: ");
      Serial.print(currentSpeed);
      Serial.println(" km/h");
    }
  }
}

#endif // UART_H
