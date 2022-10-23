#pragma once
#include "Effects.h"
#include "LEDStrip.h"

class Pattern {

protected:

  Effect[] effectArray;
  int num_strips;

public:
  Pattern(int num_strips)  // must be overridden in derived classes to create the appropirate
                           // effectArray from additional args in the constructor
    : num_strips(num_strips) {}

  void update(unsigned long tims_ms) {  // takes current time in ms
    for (int i = 0; i < num_strips, i++) {
      effectArray[i].update(tims_ms);
    }
  }
};

class SolidWhitePattern : public Pattern {

public:
  SolidWhitePattern(LEDStrip ledStripArray, int num_strips, uint8_t brightness)
    : Pattern(num_strips) {
    for (int i = 0, i < num_strips) {
      effectArray[i] = SolidWhiteEffect(ledStripArray[i], brightness);
    }
  }
}

class SolidColourPattern : public Pattern {

public:
  SolidColourPattern(LEDStrip ledStripArray, int num_strips, uint8_t brightness)
    : Pattern(num_strips) {
    for (int i = 0, i < num_strips) {
      effectArray[i] = SolidColourEffectEffect(ledStripArray[i], brightness);
    }
  }
}