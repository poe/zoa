
/// Sketch that samples from an analog pin as quickly as possible and prints averages
/// to the serial monitor so you can read them a little more easily

const int SAMPLES = 1;
const int PIN = 3;
int data[SAMPLES];
int index;
const int ZERO_VALUE = 0;

/// if true, performs a moving average. if false, performs a binned average
/// (sends an average each time the array is fully filled).
boolean moving;

void setup()
{
  Serial.begin(9600);
  index = 0;
  moving = false;
  memset( data, 0, sizeof(data) ); // initialize array to 0
}


void loop()
{
  data[index] = abs( analogRead(PIN) - ZERO_VALUE );
  if ( moving || index == SAMPLES - 1 )
  {
    Serial.println( average(data) );
  }
  ++index;
  if ( index >= SAMPLES )
  {
    index = 0;
  }
}


int average( int* arr )
{
  long rval = 0;
  unsigned int samples = sizeof(arr);
  for ( unsigned int i = 0; i < samples; ++i )
  {
    rval += *arr;
    ++arr;
  }
  return round(rval/samples);
}
