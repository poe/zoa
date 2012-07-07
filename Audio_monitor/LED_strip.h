#ifndef LED_strip_h
#define LED_strip_h

#include "Arduino.h"
#include "Audio_monitor.h"

/// Class to drive a strip of LEDs whose behavior is modulated by information from Audio_monitor. 
/// Simple preliminary version for testing.
class LED_strip
{
public:
  LED_strip();
  
  /// Update bar graph with current amplitude info
  void update_bar_graph();
  
  void set_all_dark();
  
  void set_bar_graph( byte level );
  
  //Takes the current strip color array and pushes it out
  void post_frame();
  
  
private:
  static const byte STRIP_LENGTH = 5; //Beware, several methods assume a hard-coded length of 5 for the time being
  static const byte SDI = 2; //Yellow wire digital pin
  static const byte CKI = 3; //Green wire digital pin
  
  // Fill with GGRRBB hex values
  long strip_colors[STRIP_LENGTH];
  
};


#endif
