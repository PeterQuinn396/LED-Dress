#include "HardwareSerial.h"
#pragma once
#include "Effects.h"
#include "LEDStrip.h"

class Pattern {

  // Patterns combine multiple effects (expecting 6) in a synchronized way
  // A Pattern should inherit from Pattern (gives the update method), and its constructor
  // must take (LEDStrip ledStripArray[], int num_strips) and can optionally take other arguments
  // to set up the effects in the desired way. The constructor of the derived class must fill in
  // every element of the effectArray defined in Pattern.

protected:

  int num_strips;
  Effect** effectArray;

public:
  Pattern(int num_strips)  // must be overridden in derived classes to create the appropirate
                           // effectArray from additional args in the constructor
    : num_strips(num_strips) {
    this->effectArray = new Effect*[num_strips];
  }

  void update(unsigned long tims_ms) {  // takes current time in ms
    //Serial.println("Updating pattern");
    for (int i = 0; i < num_strips; i++) {
      this->effectArray[i]->update(tims_ms);
    }
  }

  ~Pattern() {
    delete[] effectArray;
  }
};

class SolidWhitePattern : public Pattern {

public:
  SolidWhitePattern(LEDStrip ledStripArray[], int num_strips, uint8_t brightness)
    : Pattern(num_strips) {
    for (int i = 0; i < num_strips; i++) {
      effectArray[i] = new SolidWhiteEffect(ledStripArray[i], brightness);
    }
    Serial.println("Build solid white pattern");
  }
};

class SolidColourPattern : public Pattern {

public:
  SolidColourPattern(LEDStrip ledStripArray[], int num_strips, uint8_t brightness)
    : Pattern(num_strips) {
    for (int i = 0; i < num_strips; i++) {
      effectArray[i] = new SolidColourEffect(ledStripArray[i], brightness);
    }
    Serial.println("Build solid color pattern");
  }
};

class SequencePattern : public Pattern {

public:
  SequencePattern(LEDStrip ledStripArray[], int num_strips, int period_ms, uint8_t brightness, bool is_white)
    : Pattern(num_strips) {

    float duty_cycle = 1.0 / num_strips;
    float phase_ratio = 1.0 / num_strips;

    for (int i = 0; i < num_strips; i++) {
      effectArray[i] = new BlinkEffect(ledStripArray[i], period_ms, duty_cycle, i * phase_ratio, brightness, is_white);
    }
    Serial.println("Built sequence pattern");
  }
};

class WavePattern : public Pattern {
public:
  WavePattern(LEDStrip ledStripArray[], int num_strips, float frequency, bool is_white)
    : Pattern(num_strips) {
    for (int i = 0; i < num_strips; i++) {
      effectArray[i] = new FadeEffect(ledStripArray[i], frequency, i * 360.0 / num_strips, is_white);
    }
    Serial.print("Built Wave, is_white ");
    Serial.println(is_white);
  }
};