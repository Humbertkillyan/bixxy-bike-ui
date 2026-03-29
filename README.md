# 🚲 BIXXY Bike UI - Turn Signals & Status Display

Arduino project for an electric bike UI using **Seeed Studio XIAO ESP32-S3** with a **Round Display** to show turn signals (left/right) and motorcycle/bike status (Speed, State: RUNNING/STOPPED/BRAKING).

## 🎯 Features

### Turn Signals (Clignotants)
- **Left Button (D1):** Displays a left-pointing arrow on the round display with "CLIGNO GAUCHE" text
- **Right Button (D2):** Displays a right-pointing arrow on the round display with "CLIGNO DROITE" text
- **Toggle:** Pressing the same button again turns off the signal
- **Mutex Logic:** Activating one turn signal automatically cancels the other

### Main Display (Speed & Status)
When no turn signal is active, the display shows:
- **Speed:** in km/h (large green text at top)
- **Status:** One of three states (bottom):
  - `ROULE` (GREEN) - Bike is moving
  - `ARRET` (GRAY) - Bike is stopped
  - `FREIN` (RED) - Braking

### Communication
- **UART Protocol:** Receives speed and status from motor controller via D6/D7 (115200 baud)
- **Button Feedback:** Transmits turn signal state changes back to the motor controller
- **Serial Monitor:** Debug output on USB Serial (115200 baud)

## 🔧 Hardware

### Components
- **MCU:** Seeed Studio XIAO ESP32-S3
- **Display:** Round Display for XIAO (1.28", 240x240px, GC9A01)
- **Buttons:** 2x simple push buttons with internal pull-up
- **UART:** Connected to motor controller

### Wiring

| Component | Pin | GPIO | Notes |
|-----------|-----|------|-------|
| Left Turn Button | D1 | GPIO2 | Pull-up enabled via `INPUT_PULLUP` |
| Right Turn Button | D2 | GPIO3 | Pull-up enabled via `INPUT_PULLUP` |
| UART TX (to motor) | D6 | GPIO43 | Data out to motor controller |
| UART RX (from motor) | D7 | GPIO44 | Data in from motor controller |
| Round Display | XIAO Connector | - | Direct pin-to-pin connection |

### Connections
```
XIAO ESP32-S3
    ↓
[Round Display] ← Direct connection (no extra wiring needed)

D1 ← Button Left
D2 ← Button Right
D6 → UART TX to Motor Controller
D7 ← UART RX from Motor Controller
```

## 📚 Libraries Required

### Installation in Arduino IDE

1. **Seeed_Arduino_RoundDisplay** (Graphics & Touch)
   ```
   https://github.com/Seeed-Studio/Seeed_Arduino_RoundDisplay.git
   ```

2. **Seeed_GFX** (Drawing primitives)
   ```
   https://github.com/Seeed-Studio/Seeed_GFX.git
   ```

3. **ESP32 Core** (if not already installed)
   - Board Manager URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

### Installation Steps

```
Arduino IDE → Sketch → Include Library → Add .ZIP Library
Select: Seeed_Arduino_RoundDisplay.zip
Select: Seeed_GFX.zip
```

## 🚀 Getting Started

### 1. Hardware Setup
- Solder headers to XIAO ESP32-S3 (if not pre-soldered)
- Insert XIAO into Round Display connector (USB-C facing outward)
- Connect buttons to D1 and D2
- Connect motor controller UART to D6/D7

### 2. Arduino IDE Configuration
```
Board: XIAO_ESP32S3
Port: COM[X] (adjust to your system)
Speed: 240 MHz
Upload: 460800 baud
```

### 3. Upload
```
File → Open → BIXXY_BIKE_UI.ino
Verify (Ctrl+R)
Upload (Ctrl+U)
```

### 4. Testing

#### Physical Test
- Power on the system
- Display should show "STARTING..." then speed/status
- Press D1 → Left arrow should appear
- Press D1 again → Left arrow should disappear
- Press D2 → Right arrow should appear
- Press D2 again → Right arrow should disappear

#### Serial Monitor Test
```
Tools → Serial Monitor (115200 baud)
Expected output:
  === BIXXY BIKE UI - Initialization ===
  [1/4] Initializing display...
  [2/4] Initializing buttons...
  [3/4] Initializing UART...
  [4/4] Setup complete!
  === System Ready ===
```

#### Command Line Testing (via Serial Monitor)
```
left          → Enable left turn signal
right         → Enable right turn signal  
off           → Disable all turn signals
speed:25.5    → Set speed to 25.5 km/h
status:ROULE  → Set status to ROULE (ROULE/ARRET/FREIN)
```

## 💾 Protocol Specification

### UART Format (115200 baud, 8N1)

#### Outgoing (to Motor Controller)
```
TURN:LEFT       # Left turn signal activated
TURN:RIGHT      # Right turn signal activated
TURN:CANCEL     # Turn signals deactivated
```

#### Incoming (from Motor Controller)
```
STATUS:ROULE:25.5    # Status ROULE, Speed 25.5 km/h
STATUS:ARRET:0       # Status ARRET, Speed 0 km/h
STATUS:FREIN:15.2    # Status FREIN (braking), Speed 15.2 km/h
```

## 📁 File Structure

```
bixxy-bike-ui/
├── README.md                    # This file
├── BIXXY_BIKE_UI.ino           # Main sketch
├── config.h                     # Pin definitions & constants
├── driver.h                     # Seeed_GFX configuration
├── buttons.h                    # Button interrupt handlers
├── uart.h                       # UART communication functions
├── display.h                    # Display rendering functions
└── .gitignore
```

## 🔍 Debugging

### Serial Monitor Output
```
SENT: TURN:LEFT
RECEIVED: STATUS:ROULE:28.5
Parsed - Status: ROULE | Speed: 28.5 km/h
STATUS -> Left: ON | Right: OFF | Speed: 28.5 km/h | Status: ROULE
```

### Common Issues

| Issue | Cause | Solution |
|-------|-------|----------|
| Black screen after upload | Display off | Toggle Round Display power switch (bottom-left) |
| No display at all | Driver.h missing | Ensure `driver.h` in project root |
| Buttons not responsive | GPIO conflict | Check no other code uses D1/D2 |
| UART no data | Wrong pins | Verify D6/D7 are GPIO43/44 in config.h |
| Compilation error: TFT_eSPI | Conflict | Remove TFT_eSPI from Arduino libraries folder |

## 📋 Notes for Future Developers

### GPIO Constraints (XIAO ESP32-S3)
- **Strapping Pins:** GPIO0, 45, 46 → Do NOT use for user I/O
- **Touch Pins:** GPIO2, 3 are capacitive touch → Can also be digital GPIO
- **Reserved:** GPIO10-17, 38-40, 47-48 are used by optional camera

### Memory Usage
- Current sketch: ~250KB Flash / 100KB SRAM
- Available: 8MB Flash / 8MB PSRAM
- Future expansion: Add complex graphics or voice feedback

### UART Limitations
- Only one Hardware Serial (Serial1) available on XIAO ESP32-S3
- If need multiple UARTs, use software serial (slower)
- Update frequency for motor data: Currently ~10Hz (100ms)

### Display Refresh Rate
- `UPDATE_INTERVAL`: 100ms (10Hz) - Can decrease for smoother animation
- Power budget: Round Display draws ~35mA at full brightness
- Battery considerations: Use battery monitoring via ADC if needed

## 📝 License

This project is part of the BIXXY Bike ecosystem.

## 🤝 Related Projects

- [BIXXY_BIKE_BACKLIGHT](https://github.com/HumbertNguyen/BIXXY_BIKE_BACKLIGHT) - Motor controller and backlight system

---

**Last Updated:** March 29, 2026  
**Version:** 1.0  
**Status:** Active Development
