# 🔋 Smart 3-Cell Li-ion Battery Management System (BMS)

A **3-Cell (3S) Lithium-ion Battery Management System prototype** built using an **ESP32**, **I2C LCD**, LEDs, and a relay module.

The project is designed and simulated in **Wokwi**, with development/testing performed using the ESP32 Arduino environment. It demonstrates the basic logic of a BMS: **monitoring individual cell voltages, detecting overvoltage/undervoltage conditions, displaying battery status, and disconnecting the load when a fault is detected.**

> ⚠️ **Prototype:** This project is an educational BMS simulation. The voltage-sensing method used in Wokwi is simplified and should not be treated as a production-ready Li-ion protection circuit.

---

## 📌 Project Overview

A Lithium-ion battery pack requires monitoring to prevent cells from operating outside their specified voltage limits.

This project simulates a **3S Li-ion battery pack**, where each cell is represented by a potentiometer in Wokwi.

The ESP32:

1. Reads the three simulated cell voltages using ADC inputs.
2. Converts the ADC readings into a simulated cell-voltage range.
3. Checks each cell against the configured voltage thresholds.
4. Displays cell and pack voltage on an I2C LCD.
5. Turns the green LED ON when the system is healthy.
6. Turns the red LED ON when a fault is detected.
7. Activates/deactivates the relay according to the battery condition.

---

# 🎯 Features

* 🔋 3-cell battery monitoring
* 📊 Individual cell-voltage display
* ⚡ Pack-voltage calculation
* 🔻 Undervoltage detection
* 🔺 Overvoltage detection
* 🟢 Green LED for normal operation
* 🔴 Red LED for fault indication
* 📟 20×4 I2C LCD display
* 🔌 Relay-based load isolation
* 🧠 ESP32 ADC-based monitoring
* 🧪 Wokwi simulation
* ⏱️ 1-second monitoring interval
* 🖥️ Serial Monitor support

---

# 🛠️ Hardware Components

The current Wokwi simulation uses the following components:

| Component       | Quantity | Purpose                       |
| --------------- | -------: | ----------------------------- |
| ESP32 DevKit V1 |        1 | Main controller               |
| Potentiometer   |        3 | Simulated cell-voltage inputs |
| 20×4 I2C LCD    |        1 | Display battery information   |
| Green LED       |        1 | Healthy status                |
| Red LED         |        1 | Fault status                  |
| 220Ω Resistor   |        2 | LED current limiting          |
| Relay Module    |        1 | Simulated load isolation      |

---

# 🔌 Pin Configuration

The ESP32 GPIO assignments used in the firmware are:

| Function   |         ESP32 Pin |
| ---------- | ----------------: |
| Cell 1 ADC |           GPIO 32 |
| Cell 2 ADC |           GPIO 34 |
| Cell 3 ADC |           GPIO 35 |
| Red LED    |           GPIO 14 |
| Green LED  |           GPIO 12 |
| Relay      |           GPIO 27 |
| I2C SDA    | Default ESP32 I2C |
| I2C SCL    | Default ESP32 I2C |

The LCD is configured with I2C address:

```text
0x27
```

and initialized as:

```cpp
LiquidCrystal_I2C lcd(0x27, 20, 4);
```

---

# ⚙️ BMS Voltage Thresholds

The firmware uses the following thresholds:

```cpp
const float UNDER_VOLTAGE = 2.8;
const float OVER_VOLTAGE = 4.2;
```

### Cell Voltage Logic

|  Cell Voltage | Condition             |
| ------------: | --------------------- |
|      `< 2.8V` | 🔴 Undervoltage Fault |
| `2.8V – 4.2V` | 🟢 Normal             |
|      `> 4.2V` | 🔴 Overvoltage Fault  |

If **any one of the three cells** violates the configured limits, the entire system enters the fault state.

---

# 🧠 How the System Works

## 1. ADC Voltage Reading

The ESP32 reads the three analog inputs:

```cpp
int raw1 = analogRead(CELL1_PIN);
int raw2 = analogRead(CELL2_PIN);
int raw3 = analogRead(CELL3_PIN);
```

The ESP32 ADC produces values in the range:

```text
0 – 4095
```

---

## 2. Simulated Voltage Conversion

The raw ADC values are converted into a simulated cell voltage:

```cpp
float v1 = (raw1 / 4095.0) * 4.2;
float v2 = (raw2 / 4095.0) * 4.2;
float v3 = (raw3 / 4095.0) * 4.2;
```

This maps the ADC input to an approximate:

```text
0V → 4.2V
```

cell-voltage range for the simulation.

---

# 🔋 Pack Voltage Calculation

The total simulated pack voltage is calculated by adding the three cell voltages:

```cpp
float total_voltage = v1 + v2 + v3;
```

For example:

```text
Cell 1 = 3.5V
Cell 2 = 3.6V
Cell 3 = 3.5V

Pack Voltage = 10.6V
```

For a typical 3S Li-ion configuration, the theoretical fully charged pack voltage is approximately:

```text
4.2V × 3 = 12.6V
```

---

# 🚨 Fault Detection

The firmware checks whether any cell exceeds the configured limits.

### Overvoltage

```cpp
if (v1 > OVER_VOLTAGE ||
    v2 > OVER_VOLTAGE ||
    v3 > OVER_VOLTAGE)
```

If this condition is true:

```text
FAULT: OVERVOLTAGE
```

is displayed on the LCD.

---

### Undervoltage

If no overvoltage is detected, the firmware checks:

```cpp
else if (v1 < UNDER_VOLTAGE ||
         v2 < UNDER_VOLTAGE ||
         v3 < UNDER_VOLTAGE)
```

The LCD displays:

```text
FAULT: UNDERVOLTAGE
```

---

# 🛡️ Protection Response

When a fault is detected:

```cpp
digitalWrite(RELAY_PIN, LOW);
digitalWrite(RED_LED, HIGH);
digitalWrite(GREEN_LED, LOW);
```

### Fault State

```text
🔴 Red LED       → ON
🟢 Green LED     → OFF
🔌 Relay         → OFF / Tripped
📟 LCD            → Fault message
```

When all three cells are within the configured limits:

```cpp
digitalWrite(RELAY_PIN, HIGH);
digitalWrite(RED_LED, LOW);
digitalWrite(GREEN_LED, HIGH);
```

### Normal State

```text
🟢 Green LED     → ON
🔴 Red LED       → OFF
🔌 Relay         → ON
📟 LCD            → SYSTEM STATUS: OK
```

---

# 📟 LCD Display

The 20×4 I2C LCD displays:

### Line 1

```text
C1:3.50V  C2:3.60V
```

### Line 2

```text
C3:3.50V
```

### Line 3

```text
Pack Volts: 10.60V
```

### Line 4

Normal condition:

```text
SYSTEM STATUS: OK
```

Fault condition:

```text
FAULT: UNDERVOLTAGE
```

or

```text
FAULT: OVERVOLTAGE
```

---

# 🧪 Wokwi Simulation

The current simulation uses **three potentiometers** to represent the three individual battery cells.

```text
Potentiometer 1 → Cell 1
Potentiometer 2 → Cell 2
Potentiometer 3 → Cell 3
```

By adjusting the potentiometers, different battery conditions can be simulated.

### Normal Condition

Set all three simulated cells to approximately:

```text
3.5V – 4.0V
```

Expected result:

```text
🟢 Green LED ON
🔴 Red LED OFF
🔌 Relay ON
LCD → SYSTEM STATUS: OK
```

---

### Undervoltage Test

Set any cell below:

```text
2.8V
```

Expected result:

```text
🔴 Red LED ON
🟢 Green LED OFF
🔌 Relay OFF
LCD → FAULT: UNDERVOLTAGE
```

---

### Overvoltage Test

Set any cell above:

```text
4.2V
```

Expected result:

```text
🔴 Red LED ON
🟢 Green LED OFF
🔌 Relay OFF
LCD → FAULT: OVERVOLTAGE
```

---

# 🔄 System Flow

```text
             START
               │
               ▼
       Initialize ESP32
               │
               ▼
       Initialize LCD
               │
               ▼
       Read Cell 1 ADC
       Read Cell 2 ADC
       Read Cell 3 ADC
               │
               ▼
      Convert ADC Values
       to Cell Voltages
               │
               ▼
       Calculate Pack
          Voltage
               │
               ▼
        Check Thresholds
               │
        ┌──────┴──────┐
        │             │
     FAULT         HEALTHY
        │             │
        ▼             ▼
    Red LED ON    Green LED ON
    Relay OFF     Relay ON
    LCD Fault     LCD OK
        │             │
        └──────┬──────┘
               ▼
        Wait 1 Second
               │
               ▼
          Repeat Loop
```

---

# 📂 Recommended Repository Structure

```text
Smart-3S-BMS/
│
├── src/
│   └── main.cpp
│
├── diagram.json
│
├── wokwi.toml
│
├── README.md
│
├── images/
│   └── bms-simulation.png
│
└── LICENSE
```

---

# 💻 Technologies Used

### Hardware

* ESP32
* ADC
* I2C LCD
* LEDs
* Relay Module

### Software

* C/C++
* Arduino Framework
* Wokwi
* VS Code

### Concepts

* Battery monitoring
* ADC interfacing
* Threshold-based protection
* Embedded control systems
* I2C communication
* Fault detection
* Load isolation

---

# 🔮 Future Improvements

This prototype can be developed into a more advanced BMS by adding:

* [ ] 🌡️ Battery temperature monitoring
* [ ] ⚡ Current sensing
* [ ] ⚖️ Cell balancing
* [ ] 🔋 State of Charge (SoC) estimation
* [ ] ❤️ State of Health (SoH) estimation
* [ ] 📱 ESP32 Bluetooth monitoring
* [ ] 🌐 IoT battery dashboard
* [ ] 📝 Fault logging
* [ ] 🔌 MOSFET-based protection
* [ ] 🛑 Short-circuit protection
* [ ] 🔋 Dedicated multi-cell BMS IC
* [ ] 📊 Real-time battery monitoring dashboard

---

# ⚠️ Important Hardware Disclaimer

This project is currently a **simulation/prototype**, not a certified battery protection system.

The Wokwi potentiometers simulate cell-voltage inputs. The firmware's ADC-to-voltage conversion is also simplified for simulation.

For a real 3S Li-ion battery pack, **do not connect individual battery cells directly to ESP32 ADC pins**.

A practical hardware implementation requires an appropriately designed cell-monitoring circuit, protection circuitry, current sensing, temperature monitoring, suitable switching devices, and proper electrical isolation/level shifting where required.

A production BMS should also account for conditions such as:

* Overcurrent
* Short circuit
* Overtemperature
* Cell imbalance
* Charging control
* Discharge protection
* Fault recovery
* Safe power-up/power-down behavior

---

# 📚 What I Learned

Through this project, I explored:

* How ESP32 ADC inputs can be used for analog monitoring
* How threshold-based fault detection works
* How to interface an I2C LCD with ESP32
* How embedded systems respond to abnormal conditions
* How relays can be controlled using a microcontroller
* How a basic BMS protection algorithm can be simulated
* How to test embedded hardware logic using Wokwi
* Fundamentals of Li-ion battery protection

---

# 🚀 Project Goal

The goal of this project is to demonstrate the fundamental architecture of a smart battery monitoring system:

```text
        MONITOR
           ↓
        ANALYZE
           ↓
        DETECT
           ↓
         ALERT
           ↓
        PROTECT
```

Although this implementation is a simplified prototype, it provides a foundation for exploring more advanced **Battery Management Systems for EVs, robotics, drones, and energy-storage applications.**

---

# 👨‍💻 Author

**Prakhar Sharma**

### Areas of Interest

* Embedded Systems
* Electronics
* Electric Vehicles
* Battery Management Systems
* IoT
* Microcontrollers
* Hardware & Firmware

---

## ⭐ If you found this project useful

Feel free to ⭐ the repository, explore the simulation, and build upon the project.

**Monitor → Detect → Alert → Protect 🔋⚡**
