
// Objects used by the beer cooler application
#include <ArduinoMap.h>
#include "LED.h"



LED::LED(byte pin, Polarity p) {
  _pin = pin;
  polarity = p;
}

void LED::init() {
	pinMode(_pin, INPUT);
}

void LED::on() {
  pinMode(_pin, OUTPUT);
  if (polarity == HIGH_ON) digitalWrite(_pin, HIGH);
  else digitalWrite(_pin, LOW);
  status = ON;
}

void LED::off() {
// Safer just to turn the PIN HIGH Z (just disconnect the LED) and it uses less power
  digitalWrite(_pin, LOW); //necessary to unsure that no pull-up internal resistors
  pinMode(_pin, INPUT);
  status = OFF;
}

void LED::flash(unsigned count) {
  for (int i=0;i<count;i++) {
    on();
    delay(250);
    off();
    delay(250);
  }
}
