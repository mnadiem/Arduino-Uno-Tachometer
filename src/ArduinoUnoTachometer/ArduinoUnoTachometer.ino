#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Hardware configuration
constexpr uint8_t SENSOR_PIN = 2;  // External interrupt on Arduino Uno
constexpr uint8_t LCD_ADDRESS = 0x27;
constexpr uint8_t LCD_COLUMNS = 16;
constexpr uint8_t LCD_ROWS = 2;
constexpr uint8_t PULSES_PER_REVOLUTION = 2;

// If no pulse arrives within this interval, report zero RPM.
constexpr uint32_t NO_PULSE_TIMEOUT_US = 100000UL;

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// Shared with the interrupt service routine: copy these atomically in loop().
volatile uint32_t lastPulseMicros = 0;
volatile uint32_t pulsePeriodMicros = 0;
volatile bool hasValidPeriod = false;

uint32_t lastLcdUpdateMs = 0;
uint32_t lastSerialUpdateMs = 0;
uint32_t rpm = 0;

void onPulse() {
  const uint32_t now = micros();

  // The first edge establishes a reference time; it is not a valid period.
  if (lastPulseMicros != 0) {
    const uint32_t period = now - lastPulseMicros; // wrap-safe unsigned subtraction
    if (period > 0) {
      pulsePeriodMicros = period;
      hasValidPeriod = true;
    }
  }
  lastPulseMicros = now;
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), onPulse, RISING);

  lcd.setCursor(0, 0);
  lcd.print(F("Digital Tachometer"));
  lcd.setCursor(0, 1);
  lcd.print(F("Waiting for pulse"));
}

void loop() {
  uint32_t lastPulseSnapshot;
  uint32_t periodSnapshot;
  bool validPeriodSnapshot;

  // On the 8-bit ATmega328P, 32-bit shared values need an atomic snapshot.
  noInterrupts();
  lastPulseSnapshot = lastPulseMicros;
  periodSnapshot = pulsePeriodMicros;
  validPeriodSnapshot = hasValidPeriod;
  interrupts();

  const uint32_t nowMicros = micros();
  const bool timedOut =
      (lastPulseSnapshot == 0) ||
      (uint32_t)(nowMicros - lastPulseSnapshot) > NO_PULSE_TIMEOUT_US;

  if (timedOut || !validPeriodSnapshot || periodSnapshot == 0) {
    rpm = 0;
  } else {
    // Divide in stages to avoid overflowing period * pulsesPerRevolution.
    rpm = 60000000UL / periodSnapshot / PULSES_PER_REVOLUTION;
  }

  const uint32_t nowMs = millis();

  // Refresh the LCD at 5 Hz rather than on every loop iteration.
  if (nowMs - lastLcdUpdateMs >= 200) {
    lastLcdUpdateMs = nowMs;

    lcd.setCursor(0, 0);
    lcd.print(F("Rotational Speed"));
    lcd.setCursor(0, 1);
    lcd.print(F("RPM: "));
    lcd.print(rpm);
    lcd.print(F("       ")); // Clear leftover digits from the previous value.
  }

  if (nowMs - lastSerialUpdateMs >= 500) {
    lastSerialUpdateMs = nowMs;
    Serial.print(F("Pulse period (us): "));
    Serial.print(periodSnapshot);
    Serial.print(F("\tRPM: "));
    Serial.println(rpm);
  }
}
