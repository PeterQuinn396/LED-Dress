#pragma once
#include "Effects.h"
#include "LEDStrip.h"

class Pattern {

protected:

  int num_strips;
  Effect* effectArray;

public:
  Pattern(int num_strips)  // must be overridden in derived classes to create the appropirate
                           // effectArray from additional args in the constructor
    : num_strips(num_strips) {
    effectArray = new Effect[num_strips];
  }

  void update(unsigned long tims_ms) {  // takes current time in ms
    for (int i = 0; i < num_strips; i++) {
      effectArray[i].update(tims_ms);
    }
  }

  ~Pattern() {
    delete effectArray;
  }
};

class SolidWhitePattern : public Pattern {

public:
  SolidWhitePattern(LEDStrip ledStripArray[], int num_strips, uint8_t brightness)
    : Pattern(num_strips) {
    for (int i = 0; i < num_strips; i++) {
      effectArray[i] = SolidWhiteEffect(ledStripArray[i], brightness);
    }
  }
};

class SolidColourPattern : public Pattern {

public:
  SolidColourPattern(LEDStrip ledStripArray[], int num_strips, uint8_t brightness)
    : Pattern(num_strips) {
    for (int i = 0; i < num_strips; i++) {
      effectArray[i] = SolidColourEffect(ledStripArray[i], brightness);
    }
  }
};