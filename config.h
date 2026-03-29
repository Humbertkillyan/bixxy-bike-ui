#ifndef CONFIG_H
#define CONFIG_H

// ============= PIN CONFIGURATION =============
#define BUTTON_LEFT_PIN   D1   // GPIO2  - Cligno gauche (LEFT turn signal)
#define BUTTON_RIGHT_PIN  D2   // GPIO3  - Cligno droite (RIGHT turn signal)
#define UART_RX_PIN       D7   // GPIO44 - Réception données moteur
#define UART_TX_PIN       D6   // GPIO43 - Transmission état boutons

// ============= UART CONFIGURATION =============
#define UART_BAUDRATE     115200
#define UART_PORT         Serial1  // Hardware UART

// ============= DISPLAY CONFIGURATION =============
#define SCREEN_WIDTH      240
#define SCREEN_HEIGHT     240
#define TEXT_SIZE         2
#define ARROW_SIZE        40

// ============= COLORS (RGB565) =============
#define COLOR_BLACK       0x0000
#define COLOR_WHITE       0xFFFF
#define COLOR_GREEN       0x07E0
#define COLOR_RED         0xF800
#define COLOR_YELLOW      0xFFE0
#define COLOR_BLUE        0x001F
#define COLOR_ORANGE      0xFCA0
#define COLOR_CYAN        0x07FF
#define COLOR_GRAY        0x8410

// ============= TIMING CONFIGURATION =============
#define DEBOUNCE_DELAY    50   // ms - délai anti-rebond
#define UPDATE_INTERVAL   100  // ms - fréquence de mise à jour écran

// ============= STATUS STRINGS =============
#define STATUS_RUNNING    "ROULE"
#define STATUS_STOPPED    "ARRET"
#define STATUS_BRAKING    "FREIN"

// ============= SERIAL PROTOCOL =============
// Format: "TURN:LEFT" ou "TURN:RIGHT" ou "TURN:CANCEL:LEFT" etc.
#define MSG_TURN_LEFT     "TURN:LEFT"
#define MSG_TURN_RIGHT    "TURN:RIGHT"
#define MSG_TURN_CANCEL   "TURN:CANCEL"
#define MSG_STATUS_PREFIX "STATUS:"      // STATUS:ROULE:25.5 (speed km/h)

#endif // CONFIG_H
