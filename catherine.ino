// our code
#include "LEDStrip.h"
#include "Patterns.h"

// touch settings
#define TOUCH_PIN 4         // GPIO 4 is a touch pin
#define TOUCH_THRESHOLD 50  // values less than this are touches, the normal untouched value is ~70
#define TOUCH_DELAY_MS 350  // wait this long between touch detections, to avoid multiple detections for same press
unsigned long last_touch_time = 0;

// LED strip settings
#define NUMBER_OF_STRIPS 6

#define STRIP_0_WHITE 13
#define STRIP_0_COLOUR 14

#define STRIP_1_WHITE 26
#define STRIP_1_COLOUR 27

#define STRIP_2_WHITE 16
#define STRIP_2_COLOUR 17

#define STRIP_3_WHITE 18
#define STRIP_3_COLOUR 19

#define STRIP_4_WHITE 32
#define STRIP_4_COLOUR 33

#define STRIP_5_WHITE 22
#define STRIP_5_COLOUR 23

LEDStrip stripArray[6] = {
  LEDStrip(STRIP_0_WHITE, STRIP_0_COLOUR),
  LEDStrip(STRIP_1_WHITE, STRIP_1_COLOUR),
  LEDStrip(STRIP_2_WHITE, STRIP_2_COLOUR),
  LEDStrip(STRIP_3_WHITE, STRIP_3_COLOUR),
  LEDStrip(STRIP_4_WHITE, STRIP_4_COLOUR),
  LEDStrip(STRIP_5_WHITE, STRIP_5_COLOUR),
};

Pattern* activePattern;
int mode = 0;
int const total_modes = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // for debug prints

  // TODO: disable wifi and bluetooth, reduces power draw

  activePattern = selectActivePattern(mode);
}


void loop() {
  unsigned long time_ms = millis();  // get current time
  if (gotTouch()) {                  // check if we got a touch, if we did, change mode
    mode++;
    mode %= total_modes;
    delete activePattern;  // delete data that will have its reference overwritten, prevent memory leak
    activePattern = selectActivePAttern(mode, stripArray, NUMBER_OF_STRIPS);
  }
  activePattern->update(time_ms);  // continually update the active pattern
  delay(1);                        // give processor some chill time, may remove
}


Pattern* selectActivePattern(int mode, LEDStrip[] ledStripArray, int num_strips) {
  Pattern* pattern;
  float frequency;
  float phase_angle;
  uint8_t brightness;
  switch (mode) {
    case 0:
      Serial.println("Selecting mode 0");
      phase_angle = 0.;
      frequency = 1.;
      pattern = new SolidWhitePattern(ledStripArray, num_strips);
      return pattern;

    case 1:
      Serial.println("Selecting mode 1");
      phase_angle = 0.;
      frequency = 1.;
      pattern = new ColourFadePattern(*strip0, frequency, phase_angle);
      return pattern;

    case 2:
      Serial.println("Selecting mode 2");
      frequency = 2;
      brightness = 200;
      pattern = new AlternateEffect(*strip0, frequency, brightness);
      return pattern;
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