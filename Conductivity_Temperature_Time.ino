#include <avr/sleep.h>
#include <avr/power.h>
#include <EEPROM.h>

volatile int interval = 0;
int eepromByte = 0;
boolean toggle = false;

void setup() {
//  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  delay(2000);
  noInterrupts();
  TIMSK2 = 0;
  ASSR |= (1 << AS2);
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  while (ASSR & 0x1F) {};
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  TIFR2 = 0x07;
  TIMSK2 |= (1 << TOIE2);
  interrupts();
}

ISR(TIMER2_OVF_vect) {
  interval+=1;
}

float conductivity() {
  int circReturn = 0;
  int resReturn = 0;
  float sumConductivity = 0.0;
  float meanConductivity = 0.0;
  float circCurrent = 0.0;
  uint8_t i = 0;
  for(i = 0; i < 10; i++) {
    digitalWrite(5, toggle);
    digitalWrite(6, !toggle);
    delay(1);
    circReturn = analogRead(A4) - analogRead(A5);
    resReturn = analogRead(A4) - analogRead(A3);
    circCurrent = resReturn / 560;
    sumConductivity += circCurrent / (float)(circReturn - resReturn);
    toggle = !toggle;
  }
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  meanConductivity = sumConductivity / i;
  return meanConductivity;
}

uint8_t temperature () {
  int outReturn = 0;
  int thermReturn = 0;
  uint8_t pctReturn = 0;
  digitalWrite(7, HIGH);
  outReturn = analogRead(2);
  thermReturn = analogRead(1);
  pctReturn = thermReturn * 200 / outReturn;
  digitalWrite(7, LOW);
  return pctReturn;
}

void logInt(uint16_t data) {
  uint8_t LowByte = data & 0xFF;
  uint8_t HighByte = (data >> 8) & 0xFF;
  EEPROM.write(eepromByte, LowByte);
  eepromByte++;
  EEPROM.write(eepromByte, HighByte);
}

boolean logData() {
  if(eepromByte > 1020) return false;
  int conduct = 0;
  uint8_t temp = 0;
  conduct = (int)conductivity();
  logInt(conduct);
  eepromByte++;
  temp = temperature();
  EEPROM.write(eepromByte, temp);
  eepromByte++;  
  return true;
}

void loop() {
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();
  sleep_mode();
  sleep_disable();
  power_all_enable();
//  Serial.println("Timer works");
  if(interval >= 450) {
    logData();
    interval = 0;
    delay(200);
  }
}
