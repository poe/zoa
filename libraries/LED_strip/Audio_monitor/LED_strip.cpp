#include "LED_strip.h"


LED_strip::LED_strip()
{
  pinMode(SDI, OUTPUT);
  pinMode(CKI, OUTPUT);
  set_all_dark();
}

//-----------------------------------------------------

void LED_strip::update_bar_graph()
{
  set_bar_graph( Audio_monitor::instance().get_amplitude_level( STRIP_LENGTH+1 ) );
  post_frame();
}

//-----------------------------------------------------

void LED_strip::set_all_dark()
{
  memset(strip_colors, 0, sizeof(strip_colors));
}

//-----------------------------------------------------

void LED_strip::set_bar_graph( byte level )
{
    strip_colors[0] = 0x0000FF * (level > 0);
    strip_colors[1] = 0xFF0000 * (level > 1);
    strip_colors[2] = 0xFFFF00 * (level > 2);
    strip_colors[3] = 0x44FF00 * (level > 3);
    strip_colors[4] = 0x00FF00 * (level > 4);
}

//-----------------------------------------------------

void LED_strip::post_frame (void) {
  // Each LED requires 24 bits of data
  // MSB: R7, R6, R5..., G7, G6..., B7, B6... B0 
  // Once the 24 bits have been delivered, the IC immediately relays these bits to its neighbor
  // Pulling the clock low for 500us or more causes the IC to post the data.

  for (int i = 0 ; i < STRIP_LENGTH; i++) {
    for (byte color_bit = 23; color_bit != 255; color_bit--) {
      // Feed color bit 23 first (red data MSB)
      digitalWrite(CKI, LOW); // Only change data when clock is low
      digitalWrite(SDI, strip_colors[i] & (1L << color_bit) ? HIGH : LOW); 
      digitalWrite(CKI, HIGH); // Data is latched when clock goes high
    }
  }

  // Pull clock low to put strip into reset/post mode
  digitalWrite(CKI, LOW);
  delayMicroseconds(500); // Wait for 500us to go into reset
}
