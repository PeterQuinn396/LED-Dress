// our code
#include "LEDStrip.h"
#include "Patterns.h"

// touch settings
#define TOUCH_PIN_MODE 4    // GPIO 4 is a touch pin, will toggle active pattern
#define TOUCH_PIN_COLOR 15  // GPIO 0 is a touch pin, will toggle colour
#define TOUCH_THRESHOLD 50  // values less than this are touches, the normal untouched value is ~70
#define TOUCH_DELAY_MS 350  // wait this long between touch detections, to avoid multiple detections for same press
unsigned long last_touch_time = 0;

// LED strip settings
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

#define NUMBER_OF_STRIPS 6
LEDStrip ledStripArray[NUMBER_OF_STRIPS] = {
  LEDStrip(STRIP_0_WHITE, STRIP_0_COLOUR),
  LEDStrip(STRIP_1_WHITE, STRIP_1_COLOUR),
  LEDStrip(STRIP_2_WHITE, STRIP_2_COLOUR),
  LEDStrip(STRIP_3_WHITE, STRIP_3_COLOUR),
  LEDStrip(STRIP_4_WHITE, STRIP_4_COLOUR),
  LEDStrip(STRIP_5_WHITE, STRIP_5_COLOUR),
};

Pattern* activePattern;

#define TOTAL_MODES 6
int mode = 0;
bool is_white = true;  // should be true for white, false for colour

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // for debug prints

  pinMode(TOUCH_PIN_MODE, INPUT);
  pinMode(TOUCH_PIN_COLOR, INPUT);

  // TODO: disable wifi and bluetooth, reduces power draw
  mode = 0;
  is_white = true;
  activePattern = selectActivePattern(mode, is_white, ledStripArray, NUMBER_OF_STRIPS);
}


void loop() {
  unsigned long time_ms = millis();  // get current time
  if (gotTouch(TOUCH_PIN_MODE)) {    // check if we got a touch, if we did, change mode
    mode++;
    mode %= TOTAL_MODES;
    delete activePattern;  // delete data that will have its reference overwritten, prevent memory leak
    activePattern = selectActivePattern(mode, is_white, ledStripArray, NUMBER_OF_STRIPS);
  }

  if (gotTouch(TOUCH_PIN_COLOR)) {  // check if we got a touch, if we did, change mode
    is_white = !is_white;
    delete activePattern;  // delete data that will have its reference overwritten, prevent memory leak
    activePattern = selectActivePattern(mode, is_white, ledStripArray, NUMBER_OF_STRIPS);
  }

  activePattern->update(time_ms);  // continually update the active pattern
  delay(1);                        // give processor some chill time, may remove
}


Pattern* selectActivePattern(int mode, bool is_white, LEDStrip ledStripArray[], int num_strips) {
  Pattern* pattern;
  Serial.println(mode);
  switch (mode) {

    case 0:  // off
      {
        uint8_t brightness = 0;
        Serial.println("Selecting mode 0");
        pattern = new SolidPattern(ledStripArray, num_strips, brightness, is_white);
        return pattern;
      }
    case 1:  // solid white or colour
      {
        uint8_t brightness = 255;
        Serial.println("Selecting mode 1");
        pattern = new SolidPattern(ledStripArray, num_strips, brightness, is_white);
        return pattern;
      }
    case 2:  // wave
      {
        Serial.println("Selecting mode 2");
        float frequency = 1.;  // Hz
        pattern = new WavePattern(ledStripArray, num_strips, frequency, is_white);
        return pattern;
      }
    case 3:  // slow wave
      {
        Serial.println("Selecting mode 3");
        float frequency = .25;  // Hz
        pattern = new WavePattern(ledStripArray, num_strips, frequency, is_white);
        return pattern;
      }
    case 4:  // sequence
      {
        Serial.println("Selecting mode 4");
        int period_ms = 6000;
        uint8_t brightness = 255;
        pattern = new SequencePattern(ledStripArray, num_strips, period_ms, brightness, is_white);
        return pattern;
      }
    case 5:  // chaos
      {
        Serial.println("Selecting mode 5");
        pattern = new ChaosPattern(ledStripArray, num_strips);
        return pattern;
      }
  }
}

bool gotTouch(int pin) {
  // check if the touch mode sensor has been triggered
  int val = touchRead(pin);
  // Serial.print("------------ ");
  // Serial.println(val);
  unsigned long current_time = millis();
  if (val < TOUCH_THRESHOLD && current_time - last_touch_time > TOUCH_DELAY_MS) {
    last_touch_time = current_time;

    Serial.print("Got touch for pin ");
    Serial.println(pin);
    return true;
  } else {
    return false;
  }
}