#include "Audio_monitor.h"
#include "../MsTimer2/MsTimer2.h"

/// Turns on/off serial diagnostic output
boolean serial_debug = false;

//-----------------------------------------------------

Audio_monitor Audio_monitor::singleton = Audio_monitor();

Audio_monitor::Audio_monitor()
: amp_cnt(0), sum(0)//, amp(0), last_reading(0)
{
  // for Shifty
  pinMode(datapin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(enablepin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(1<<SPR0);
  digitalWrite(latchpin, LOW);
  digitalWrite(enablepin, LOW);
  analogReference(INTERNAL);

  memset(amplitudes, 0, sizeof(amplitudes));
  
  /// Configure and start the interrupt that will call update_amplitude every SAMPLE_INTERVAL ms
  MsTimer2::set(SAMPLE_INTERVAL, &update_amplitude );
  MsTimer2::start();
  
  if ( serial_debug )
  {
    Serial.begin(9600);
  }
}

//-----------------------------------------------------

Audio_monitor::~Audio_monitor()
{
  MsTimer2::stop();
}

//-----------------------------------------------------

const Audio_monitor& Audio_monitor::instance()
{
  return singleton;
}

//-----------------------------------------------------

int Audio_monitor::get_amplitude() const
{
  unsigned int amp = round( sum / AMP_SIZE );
  if ( serial_debug )
  {
    //Serial.println( amp );
  }
  return amp;
}

//-----------------------------------------------------

int Audio_monitor::get_amplitude_level( byte bins ) const
{
  return map(get_amplitude(),0,MAX_AMPLITUDE,0,bins-1);
}

//-----------------------------------------------------

void Audio_monitor::update( unsigned int latest_value )
{
  latest_value > MAX_AMPLITUDE ? MAX_AMPLITUDE : latest_value;
  /// Store the latest value in the amplitudes array and also update the sum
  /// so that we don't have to recalculate it every time we want to calc the average
  sum -= amplitudes[amp_cnt];
  amplitudes[amp_cnt] = latest_value;
  sum += latest_value;
  ++amp_cnt;
  if ( amp_cnt >= AMP_SIZE )
  {
    amp_cnt = 0;
  }
  if ( serial_debug )
  {
    Serial.println( latest_value );
  }
}

//-----------------------------------------------------
/*
void Audio_monitor::update( unsigned int latest_value )
{
  if( latest_value > last_reading )
  {
      amp = latest_value; // Never miss a new peak
  }
  else
  {
      amp = (last_reading * 7 + latest_value) / 8; // But smooth out the decay
  }
  last_reading = latest_value;
}
*/
//-----------------------------------------------------

void Audio_monitor::update_amplitude()
{
  singleton.update( analogRead(audio_read_pin) * SENSITIVITY_MULTIPLIER );
}
