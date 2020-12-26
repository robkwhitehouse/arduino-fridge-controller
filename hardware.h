/**
 * A set of classes to provide interfaces to the various hardware devices 
 */

 /*****
 * chiller logic class. Returns a true or false result depending upon the value of the battery voltage
 * and chiller temp. The voltgae is assumed to be the open circuit, battery terminal, voltage and this logic assumes that
 * any correction to actual voltage readings has been done by external code. E.g. if the peltier unit is actually on
 * then this will cause a rough 0.25v drop in the measured voltage due to battery internal resistance.
 */
#include <arduino.h>
/* You might need to tweak the constants defined here depending 
 *  upon your own battery and cooler element etc.
 */

// battery internal resistance (ohms)
#define BATT_RESISTANCE 0.0625 //typical value for a hybrid/marine battery
//#define BATT_RESISTANCE 0.02 //typical value for a car starter battery

#define BATTOK 12.7 //any open circuit voltage above 12.7 is totally fine
#define TARGET_TEMP 6.0 //degC - just how cool do you want to be?
#define PELTIER_CURRENT 4.0 //Amps - typical for a 50W TEC


#ifndef CHILLER_LOGIC_H
#define CHILLER_LOGIC_H

class ChillerLogic {
  public:
  boolean decision( float BatteryVolts, float ChillerTemp);
};

#endif

/** Interface for the battery voltage monitor
 *  
 */
#ifndef BATTERY_H
#define BATTERY_H
const long threeMinutes = 3*60000; //3 minutes in milliseconds 

 class Battery {
  private:
  long stabilization_timer; //how long remaining to wait (in ms) before batt volts is stable 
  float stable_volts; //previous stable reading
  byte _pin;
  long previousTimestamp;
  
  public:
  Battery(byte pin);//Which analogue (ADC) input is the battery connected to
  float getVoltage(); //returns battery terminal voltage
  void resetTimer();
  void updateTimer();
 };

#endif
 
 
 /*****
  * Will flash a character or String in morse code on the LED (supplied as a constructor parameter)
 */

#ifndef MORSE_FLASHER_H
#define MORSE_FLASHER_H
#include <LED.h>
#include <ArduinoMap.h>

class MorseFlasher {
  public:
  MorseFlasher( LED * );
  void flashChar(char);
  void flashString(String);
  private:
  LED *myLED;
  CreateMap(morseMap, char, String, 36);
};



#endif

#ifndef PELTIER_H
#define PELTIER_H
 /**
  * The peltier TEC (Thermo Electric Cooler) module
  * 
  */
class Peltier {
  public:
     Peltier(byte pin);
     void init();
     boolean currentState();
     void toggle(); //Turns it on or off
  private:
     byte _pin;
};     

#endif  
