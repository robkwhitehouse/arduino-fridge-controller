/*
 * Implementation of the various methods associted with the various hardware interfaces
 */
#include "hardware.h"

 /*****
 * chiller logic class. Decides whether the Peltier module should be ON or OFF
 * depends upon open circuit battery voltage and current coolbox temp. 
 */


boolean ChillerLogic::decision( float BatteryVolts, float ChillerTemp) 
{
  if (BatteryVolts < BATTOK) return(false);
  else {//battery OK
    if(ChillerTemp >= TARGET_TEMP)
      return(true); //Turn the cooler on
    else 
      return(false); //Coolbox is already < 7C - turn the cooler off
  } 
}

//Battery class

// Converts the 10bit ADC reading (which goes from 0 - 1023) to a voltage (0 - 20V):
// There is an external voltage divider (divide by 4) to convert the 
//Ships battery voltage (0-20v) to the 0-5v range of the analog input
const float voltageConversionConstant = 1023/20;

Battery::Battery(byte pin) {
  _pin = pin;
  stabilization_timer = 0;
}

float Battery::getVoltage(){
  if (stabilization_timer > 0) {
    //Still waiting for battery volts to stabilize after a recent turn on/off event
    //Do nothing  
  }
  else {
    stable_volts = analogRead(_pin) / voltageConversionConstant;
  }
  return stable_volts;
}

void Battery::resetTimer() {
  stabilization_timer = threeMinutes;
}

void Battery::updateTimer() {
  if (stabilization_timer > 0 ) {
#ifdef DEBUG
     Serial.print("Battery stablization timer has ");
     Serial.print(stabilization_timer);
     Serial.println(" milliseconds to run");
#endif
     long currentTimestamp = millis();
     stabilization_timer -= (currentTimestamp - previousTimestamp);
     previousTimestamp = currentTimestamp;
  }
}

//Peltier class
Peltier::Peltier(byte pin) {
  _pin = pin;
}

void Peltier::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW); //Default cooler unit to off
}

boolean Peltier::currentState() {
  return digitalRead(_pin);
}

void Peltier::toggle() {
  if ( digitalRead(_pin) == HIGH ) 
    digitalWrite(_pin, LOW);
  else
    digitalWrite(_pin, HIGH);   
}

//Morse Flasher Class methods

MorseFlasher::MorseFlasher( LED *Led ) {
  myLED = Led;
  
  morseMap['1'] = ".----";
  morseMap['2'] = "..---";
  morseMap['3'] = "...--";
  morseMap['4'] = "....-";
  morseMap['5'] = ".....";
  morseMap['6'] = "-....";
  morseMap['7'] = "--...";
  morseMap['8'] = "---..";
  morseMap['9'] = "----.";
  morseMap['0'] = "-----";
  morseMap['A'] = ".-";
  morseMap['B'] = "-...";
  morseMap['C'] = "-.-.";
  morseMap['D'] = "-..";
  morseMap['E'] = ".";
  morseMap['F'] = "..-.";
  morseMap['G'] = "--.";
  morseMap['H'] = "....";
  morseMap['I'] = "..";
  morseMap['J'] = ".---";
  morseMap['K'] = "-.-";
  morseMap['L'] = ".-..";
  morseMap['M'] = "--";
  morseMap['N'] = "-.";
  morseMap['O'] = "---";
  morseMap['P'] = ".--.";
  morseMap['Q'] = "--.-";
  morseMap['R'] = ".-.";
  morseMap['S'] = "...";
  morseMap['T'] = "-";
  morseMap['U'] = "..-";
  morseMap['V'] = "...-";
  morseMap['W'] = ".--";
  morseMap['X'] = "-..-";
  morseMap['Y'] = "-.--";
  morseMap['Z'] = "--..";
}


void MorseFlasher::flashChar( char myChar ) {
  unsigned chardelay;
  String morseString = morseMap[myChar];
  for (int i=0; i < morseString.length(); i++ )
  { if ( morseString[i] == '.' ) chardelay = 250;
    else chardelay = 500;
    myLED->on();
    delay(chardelay);
    myLED->off();
    delay( 500 );
  }

}

void MorseFlasher::flashString( String myString ) {
   for (int i=0; i < myString.length(); i++) {
       flashChar(myString[i]);
       delay(500);
   }
}
