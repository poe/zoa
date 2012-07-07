#include <LED_strip.h>
#include <MsTimer2.h>

void setup() { }

void loop()
{
  LED_strip strip;
  
  while (1)
  {
    strip.update_bar_graph();
    delay(Audio_monitor::SAMPLE_INTERVAL);
  }
}


