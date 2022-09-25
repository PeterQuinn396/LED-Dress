// our code
#include "LEDStrip.h"
#include "Effects.h"


// touch settings
#define TOUCH_PIN 4         // GPIO 4 is a touch pin
#define TOUCH_THRESHOLD 50  // values less than this are touches, the normal untouched value is ~70
#define TOUCH_DELAY_MS 350  // wait this long between touch detections, to avoid multiple detections for same press
unsigned long last_touch_time = 0;

// LED strip settings
#define NUMBER_OF_STRIPS 1

#define STRIP_0_WHITE 16
#define STRIP_0_COLOUR 17
LEDStrip* strip0;

Effect* activeEffect;
int mode = 0;
int const total_modes = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // for debug prints

  // disable wifi and bluetooth, reduces power draw

  // set up the LED strips
  strip0 = new LEDStrip(STRIP_0_WHITE, STRIP_0_COLOUR);
  // setup the effect to run
  activeEffect = selectActiveEffect(mode);
}


void loop() {
  unsigned long time_ms = millis();  // get current time
  if (gotTouch()) {                  // check if we got a touch, if we did, change mode
    mode++;
    mode %= total_modes;
    activeEffect = selectActiveEffect(mode);
  }
  activeEffect->update(time_ms);  // continually update the active effect
  delay(1);                       // give processor some chill time, may remove
}


Effect* selectActiveEffect(int mode) {
  Effect* effect;
  float frequency;
  float phase_angle;
  uint8_t brightness;
  switch (mode) {
    case 0:
      Serial.println("Selecting mode 0");
      phase_angle = 0.;
      frequency = 1.;
      effect = new WhiteFadeEffect(*strip0, frequency, phase_angle);
      return effect;

    case 1:
      Serial.println("Selecting mode 1");
      phase_angle = 0.;
      frequency = 1.;
      effect = new ColourFadeEffect(*strip0, frequency, phase_angle);
      return effect;

    case 2:
      Serial.println("Selecting mode 2");
      frequency = 2;
      brightness = 200;
      effect = new AlternateEffect(*strip0, frequency, brightness);
      return effect;
  }
}

bool gotTouch() {
  // check if the touch sensor has been triggered
  int val = touchRead(TOUCH_PIN);
  unsigned long current_time = millis();
  if (val < TOUCH_THRESHOLD && current_time - last_touch_time > TOUCH_DELAY_MS) {
    last_touch_time = current_time;
    Serial.println("Got touch");
    return true;
  } else {
    return false;
  }
}