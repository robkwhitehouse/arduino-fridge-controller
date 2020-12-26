// Objects used by the beer cooler aplication
#ifndef LED_H 
#define LED_H
   
#include <arduino.h>
#include <ArduinoMap.h>

#define ON 1
#define OFF 0


class LED {	
  public:
  enum Polarity{HIGH_ON, LOW_ON};
  LED(byte pin, Polarity polarity );
  void init();
  void on();
  void off();
  byte status;
  void flash(unsigned count);
  private:
  int _pin;
  Polarity polarity;

};


#endif
