#pragma once
#include "LEDStrip.h"


class Effect {

protected:
  LEDStrip ledStrip;

public:
  Effect(){};  // default constructor so an empty array of Effects can be initialized

  Effect(LEDStrip ledStrip)
    : ledStrip(ledStrip) {}
  void virtual update(unsigned long time_ms) {}  // takes the current time in millis()
};

class WhiteFadeEffect : public Effect {

  // fade on and off

private:
  float frequency;    // how many oscillations per second, can be fractional
  float phase_angle;  // in degrees

public:

  WhiteFadeEffect(LEDStrip ledStrip, float frequency, float phase_angle)
    : Effect(ledStrip), frequency(frequency), phase_angle(phase_angle) {
    this->ledStrip.setWhite();
  }

  void update(unsigned long time_ms) override {
    float time_in_seconds = time_ms / 1000.0;
    float phase_angle_radians = PI / 180 * this->phase_angle;
    float angle = TWO_PI * time_in_seconds * this->frequency;
    float val = cos(angle + phase_angle_radians);  // val is -1 to 1, need to map to 0-255
    val = (val + 1) / 2 * 255;
    this->ledStrip.setBrightness(int(val));
  }
};

class ColourFadeEffect : public WhiteFadeEffect {  // 99% same as white, so just inherit
public:
  ColourFadeEffect(LEDStrip ledStrip, float frequency, int phase_angle)
    : WhiteFadeEffect(ledStrip, frequency, phase_angle) {
    this->ledStrip.setColour();  // set to colour in constructor, instead of white
  }
};

class AlternateEffect : public Effect {

  // alternate between white and coloured with a certain frequency
  // if freqency is set over ~30, it will look like both are on at the same time.

private:
  float frequency;  // number of times per second to change between white and coloured
  unsigned long last_time;
  bool isWhite;
public:

  AlternateEffect(LEDStrip ledStrip, float frequency, uint8_t brightness)
    : Effect(ledStrip), frequency(frequency) {
    this->ledStrip.setBrightness(brightness);
    this->last_time = 0;
    this->isWhite = true;  // start white
  };

  void update(unsigned long time_ms) override {

    if (time_ms - this->last_time > 1. / frequency * 1000) {
      this->isWhite = !this->isWhite;  // flip colour
      this->last_time = time_ms;       // update last time
      if (isWhite) {
        this->ledStrip.setWhite();
      } else {
        this->ledStrip.setColour();
      }
    }
  }
};


class SolidWhiteEffect : public Effect {
public:
  SolidWhiteEffect(LEDStrip ledStrip, uint8_t brightness)
    : Effect(ledStrip) {
    this->ledStrip.setWhite();
    this->ledStrip.setBrightness(brightness);
  }
  void update(unsigned long time_ms) override {
    return;
  }
};

class SolidColourEffect : public Effect {
public:
  SolidColourEffect(LEDStrip ledStrip, uint8_t brightness)
    : Effect(ledStrip) {
    this->ledStrip.setColour();
    this->ledStrip.setBrightness(brightness);
  }
  void update(unsigned long time_ms) override {
    return;
  }
};