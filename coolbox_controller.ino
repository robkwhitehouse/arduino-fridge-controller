#include <ArduinoMap.h>



/******
Smart controller for a 12v Peltier cooler element. 
Peltier elements are very greedy on battery power and so the primary function of this controller is
to protect the  12v battery from excessive depletion.
The idea is to monitor battery voltage and only allow the  cooler to operate if there is 
sufficient juice in the battery. The first (only?) application for this is my little boat where
the 12v battery is also required to start the engine. So I don't ever want to deplete the battery
by more than 20%.   

The fully charged, open circuit, voltage of a "12v" lead acid battery is about 12.8
At 80% charge, the OC voltage will have dropped to about 12.6

There are several external circuits;

1. A DS 18B20 digital temperatures sensor - monitors the internal cooler temperature. This
   sensor obviously needs to be installed inside the coolbox
2. A simple resistor voltage divider circuit to interface the ships battery voltage (0-20v) to the Arduino ADC (5v)
   N.B. The connections to the battery for this circuit *MUST* be completely separate wires from the the connections for
   the Peltier element, otherwise a false battery voltage reading will be obtained.  
3. An opto-isolated relay module - supplies 12v @ 5amps to the TEC (Peltier module)
4. An on/off rocker switch with a built in LED. The built-in LED Cathode is internally connected to the 12v supply.
   The LED anode is grounded via a NANO output pin (5). This is taken LOW to turn the LED on, or put in a HIGH-Z (input) mode to 
   turn the LED off. 
5. The LED will use morse code to flash out the current internal cooler temperature (preceded by "T" )
   and then the battery voltage preceded by morse "V". See hardware.cpp for the Morse code. 

******/

//External libs
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LowPower.h>

//My own libs
#include <LED.h>
#include "hardware.h"

#define DEBUG 1

//Arduino Pins

#define BATTV_PIN A0  //Analogue Pin 0 - battery voltage monitor

//DS18B20 Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS_PIN 2 //Pin D2
//on-off switch LED connected to Pin D5
#define LED_PIN 5
//TEC (Peltier) relay)
#define PELTIER_PIN 8      //Pin D8



//Create all our runtime objects

//The LED class constructor has two parameters;
//1. Which pin it is connected to and
//2. The polarity of the LED 
LED switchLED(LED_PIN, LED::LOW_ON);     

MorseFlasher morseFlasher(&switchLED);
ChillerLogic logic;
Battery battery(BATTV_PIN);
Peltier peltier(PELTIER_PIN);


DeviceAddress Thermometer;

float batt_voltage;
float beerTemp; 



// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS_PIN);
// Pass our oneWire reference to Dallas Temperature sensor. 
DallasTemperature sensors(&oneWire);

void setup() {
  sensors.begin(); //startup the OneWire bus
  peltier.init();
  switchLED.init();
  switchLED.off(); 
  
#ifdef DEBUG
  Serial.begin(9600);
#endif

//Flash "OK" -  provide a visual signal that things are functional on switch-on
  morseFlasher.flashString("OK"); // "--- -.-"

#ifdef DEBUG
// locate devices on the bus
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" OneWire devices.");
#endif //DEBUG

// search for devices on the bus and assign based on an index.
  if (!sensors.getAddress(Thermometer, 0)) Serial.println("Unable to find address for Device 0"); 

}

void loop() {
  
  sensors.requestTemperatures(); // Send the command to the temp. sensor to start measuring temp.

  // read the battery voltage on analog pin 0:
   battery.updateTimer();
   batt_voltage = battery.getVoltage();
   //If the Peltier is ON, this will reduce OC battery voltage by about 0.25v 
   //because of battery internal resistance
   if (peltier.currentState() == ON) batt_voltage += (PELTIER_CURRENT * BATT_RESISTANCE); //use Ohm's law to calc internal volts drop
  
  // read the cooler internal temperature 
  beerTemp = sensors.getTempC(Thermometer); 

 
 //Flash the current temp in degC
  morseFlasher.flashChar('T');
  delay(1000);
  int intTemp = round(beerTemp);
  morseFlasher.flashString(String(intTemp)); 
  delay(2000);
//Flash the voltage
  morseFlasher.flashChar('V');
  delay(1000);
  morseFlasher.flashString(String(batt_voltage)); 


  if (logic.decision(batt_voltage,beerTemp) != peltier.currentState()) {
    peltier.toggle();
    battery.resetTimer();
#ifdef DEBUG
    Serial.println("Toggling peltier relay");
#endif        
  }
      

#ifdef DEBUG 
  // print out the values you read:
  Serial.println("In loop()");
  Serial.print("Battery_voltage = ");
  Serial.println(batt_voltage);
   Serial.print("DS18B20 temp sensor = ");
  Serial.println(beerTemp);
  Serial.print("Chiller is switched ");
  if (peltier.currentState() == true)
      Serial.println("ON");
  else
      Serial.println("OFF");      
#endif

   delay(10*1000); //Sleep 10 seconds     
   
  //The big sleep
  //Low power mode for 24 seconds - clumsy but it's hard to do anything more efficient with a Nano. Saves about 8mA - geewhiz
//  LowPower.idle(SLEEP_8S,ADC_OFF,TIMER2_OFF,TIMER1_OFF,TIMER0_OFF,SPI_OFF,USART0_OFF,TWI_OFF); //sleep in low power mode for 8 seconds
//  LowPower.idle(SLEEP_8S,ADC_OFF,TIMER2_OFF,TIMER1_OFF,TIMER0_OFF,SPI_OFF,USART0_OFF,TWI_OFF); //sleep in low power mode for 8 seconds 
//  LowPower.idle(SLEEP_8S,ADC_OFF,TIMER2_OFF,TIMER1_OFF,TIMER0_OFF,SPI_OFF,USART0_OFF,TWI_OFF); //sleep in low power mode for 8 seconds 

}
