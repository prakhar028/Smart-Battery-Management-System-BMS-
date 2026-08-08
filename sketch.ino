#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define CELL1_PIN 32
#define CELL2_PIN 34
#define CELL3_PIN 35
#define RED_LED 14
#define GREEN_LED 12
#define RELAY_PIN 27

// BMS Thresholds (Simulated Li-ion values scaled to 0-3.3V ADC)
const float UNDER_VOLTAGE = 2.8; // Dangerously low cell voltage
const float OVER_VOLTAGE = 4.2;  // Overcharged cell voltage

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(115200);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  
  // Default State: Battery connected
  digitalWrite(RELAY_PIN, HIGH); 
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
}

void loop() {
  // Read raw ADC values (0 to 4095)
  int raw1 = analogRead(CELL1_PIN);
  int raw2 = analogRead(CELL2_PIN);
  int raw3 = analogRead(CELL3_PIN);

  // Convert ADC to simulated cell voltage (scaled up to represent a 4.2V max cell)
  float v1 = (raw1 / 4095.0) * 4.2;
  float v2 = (raw2 / 4095.0) * 4.2;
  float v3 = (raw3 / 4095.0) * 4.2;
  float total_voltage = v1 + v2 + v3;

  // Check for faults
  bool fault = false;
  if (v1 > OVER_VOLTAGE || v2 > OVER_VOLTAGE || v3 > OVER_VOLTAGE) {
    lcd.setCursor(0, 3);
    lcd.print("FAULT: OVERVOLTAGE ");
    fault = true;
  } else if (v1 < UNDER_VOLTAGE || v2 < UNDER_VOLTAGE || v3 < UNDER_VOLTAGE) {
    lcd.setCursor(0, 3);
    lcd.print("FAULT: UNDERVOLTAGE");
    fault = true;
  } else {
    lcd.setCursor(0, 3);
    lcd.print("SYSTEM STATUS: OK  ");
  }

  // Safety Action
  if (fault) {
    digitalWrite(RELAY_PIN, LOW); // Cut off the battery pack
    digitalWrite(RED_LED, HIGH);   // Turn on alert light
    digitalWrite(GREEN_LED, LOW);
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Keep battery connected
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }

  // Display data on LCD
  lcd.setCursor(0, 0);
  lcd.print("C1:" + String(v1, 2) + "V  C2:" + String(v2, 2) + "V");
  lcd.setCursor(0, 1);
  lcd.print("C3:" + String(v3, 2) + "V");
  lcd.setCursor(0, 2);
  lcd.print("Pack Volts: " + String(total_voltage, 2) + "V");

  delay(1000); // Check status every second
}