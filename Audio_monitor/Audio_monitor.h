#ifndef Audio_monitor_h
#define Audio_monitor_h

#include "Arduino.h"


/// Singleton class that performs interrupt-driven sampling of the amplitude inputs from
/// a Shifty VU and maintains a running average.
class Audio_monitor
{
public:
  static const byte SAMPLE_INTERVAL = 1; // milliseconds
  static const unsigned int MAX_AMPLITUDE = 1023; //don't change this (for now anyway)
  static const float SENSITIVITY_MULTIPLIER = 1; //inputs get multiplied by this

  /// Gets the one static instance of the class
  static const Audio_monitor& instance();
  
  /// Returns the average of the last AMP_SIZE amplitude measurements
  int get_amplitude() const;
  
  /// Splits the amplitude measurement range into bins and returns which bin the
  /// current average amplitude falls within
  int get_amplitude_level( byte bins ) const;
  
private:
  static const int AMP_SIZE = 20;
  static const byte clockpin = 13;
  static const byte enablepin = 10;
  static const byte latchpin = 9;
  static const byte datapin = 11;
  static const byte audio_read_pin = 2;
  
  unsigned int amplitudes[AMP_SIZE];
  static Audio_monitor singleton;
  byte amp_cnt;
  unsigned int sum;
  
  /// Private so that no one can create additional instances
  Audio_monitor();
  
  // unimplemented
  Audio_monitor( const Audio_monitor& );
  Audio_monitor& operator=(const Audio_monitor&);
  
  ~Audio_monitor();

  /// Called by update_amplitude
  void update( unsigned int latest_value );
  
  /// Interrupt callback (has to be static)
  static void update_amplitude();
};



#endif