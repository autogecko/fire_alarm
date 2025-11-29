# Hardware Specification

### Components
- **Board**: Arduino Uno WiFi
- **Display**: 1.44" ST7735 SPI LCD
- **Input Button**: Push button
- **Variable Resistor**: Potentiometer connected as a voltage divider
- **IV Sensor**: Analog input sensor
- **Speaker**: Piezo speaker
- **Bell Button**: Digital output to control a PC817 optocoupler

### Pin Connections
- **Display**
  - SCK: Pin 13
  - MOSI: Pin 11
  - CS: Pin 10
  - DC: Pin 9
  - RST: Pin 8
  - BL: 3.3V
- **Other Components**
  - Input Button: GPIO 4
  - Bell Button: GPIO 5
  - Speaker: GPIO 3
  - IV Sensor: A0
  - Variable Resistor: A1

# Functional Specification

### Core Logic
The device monitors an IV drip using the `ivSensor`. If the sensor value exceeds a reference threshold `ivRef`, it enters `Emergency Mode`. The `ivRef` threshold can be adjusted by the user. The `Button2` library should be used for button handling.

### Boot Sequence
1.  **Display**: Shows "Hello" and briefly cycles through RED, YELLOW, and BLUE backgrounds.
2.  **Speaker**: Beeps for 3 seconds.

---

### Modes of Operation

#### 1. Normal Mode
This is the default operating mode.

- **Display**: Shows "GOOD" in the center with a green background.
- **Speaker**: Off.
- **Bell Button**: Low (off).
- **Button Actions**:
  - **Short Press**: Activates the Bell Button (sets output HIGH) for 3 seconds.
  - **Long Press**: Enters `Setting Mode`.
- **State Transition**:
  - Enters `Emergency Mode` if `ivSensor` value > `ivRef`.

#### 2. Emergency Mode
This mode is triggered when the IV sensor detects a problem.

- **Display**: Shows "CAUTION" in the center with a blinking red background.
- **Speaker**: Beeps continuously (3 seconds ON, 1 second OFF).
- **Bell Button**: Activates for 2 seconds upon entering this mode.
- **Button Actions**:
  - **Short Press**: Updates `ivRef` with the current value from the Variable Resistor (A1). After 3 seconds, returns to `Normal Mode`.
- **State Transition**:
  - Returns to `Normal Mode` after a short button press and a 3-second delay.

#### 3. Setting Mode
This mode allows the user to set the `ivRef` threshold.

- **Display**: Shows "SETTING" in the center with a yellow background.
- **Button Actions**:
  - **Long Press**: Updates `ivRef` with the current value from the Variable Resistor (A1). After 3 seconds, returns to `Normal Mode`.
- **State Transition**:
  - Returns to `Normal Mode` after a long button press and a 3-second delay.

---

### Serial Monitoring
- Every 2 seconds, the device prints the following values to the serial monitor:
  - `ivValue` (current reading from IV Sensor)
  - `ivRef` (current reference threshold)
  - `mode` (current operating mode)
