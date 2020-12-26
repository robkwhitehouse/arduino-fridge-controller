# arduino-fridge-controller

*N.B.* Still under test/development - still buggy - use at your own peril!

Smart controller for a 12v Peltier cooler element. 
Peltier elements are very greedy on battery power and so the primary function of this controller is
to protect the  12v battery from excessive depletion.
The idea is to monitor battery voltage and only allow the  cooler to operate if there is 
sufficient juice in the battery. The first (only?) application for this is my little boat where
the 12v battery is also required to start the engine. So I don't ever want to deplete the battery
by more than 20%.   

The fully charged, open circuit, voltage of a "12v" lead acid battery is about 12.8
At 80% charge, the OC voltage will have dropped to about 12.6
I have used an Arduino Nano but I guess that most other Arduinos will work. I have assumed
that the ADC is 10bit by default - I guess that this might not always be true.

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
