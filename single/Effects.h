#pragma once
#include "LEDStrip.h"


class Effect {

  // An effect controls the colour/brightness/timing for a single LEDStrip.
  // An effect should inherit from the abstract base class Effect, which enforces a common interface
  // for effects. The constructor of the derived class must take an parameters used for running the
  // effect (ex: freqency, brightness), and the update method must be overridden to acheive the desired effect.

protected:
  LEDStrip ledStrip;

public:
  Effect(){};  // default constructor so an empty array of Effects can be initialized

  Effect(LEDStrip ledStrip)
    : ledStrip(ledStrip) {}
  void virtual update(unsigned long time_ms) {}  // takes the current time in millis()
};

class FadeEffect : public Effect {

  // fade on and off

private:
  float frequency;    // how many oscillations per second, can be fractional
  float phase_angle;  // in degrees

public:

  FadeEffect(LEDStrip ledStrip, float frequency, float phase_angle, bool is_white)
    : Effect(ledStrip), frequency(frequency), phase_angle(phase_angle) {

    if (is_white) {
      Serial.println("Built white fade");
      this->ledStrip.setWhite();
    } else {
      Serial.println("Built colour fade");
      this->ledStrip.setColour();
    }
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

class BlinkEffect : public Effect {

private:
  float period;       // time in ms for pattern to repeat
  float duty_cycle;   // [0-1] how much of the period to be on for
  float phase_ratio;  // [0-1] offset fraction of the period
  uint8_t brightness;
  unsigned long last_time;
  int delay;
  bool is_white;

public:
  BlinkEffect(LEDStrip ledStrip, float period_ms, float duty_cycle, float phase_ratio, uint8_t brightness, bool is_white)
    : Effect(ledStrip), period(period_ms), duty_cycle(duty_cycle), phase_ratio(phase_ratio), brightness(brightness), is_white(is_white) {

    if (is_white)
      this->ledStrip.setWhite();
    else
      this->ledStrip.setColour();
    this->ledStrip.setBrightness(brightness);
    this->last_time = 0;
    this->delay = this->period * this->phase_ratio;

    Serial.println("Built blink effect");
  }

  void update(unsigned long time_ms) override {
    // Serial.println("Updating blink effect");

    unsigned long delta = time_ms - this->last_time;


    if (delta < delay) {
      //Serial.println("Set low");
      this->ledStrip.setBrightness(0);
    } else if (delta < delay + this->period * this->duty_cycle) {  // in the high time of the duty cycle
      //Serial.println("Set high");
      this->ledStrip.setBrightness(this->brightness);
    } else if (delta < this->period) {  // in the low part of the duty cycle
      //Serial.println("Set low");
      this->ledStrip.setBrightness(0);
    } else {
      this->last_time = time_ms;  // time to move to the next period
    }
  }
};


class SolidEffect : public Effect {

public:
  SolidEffect(LEDStrip ledStrip, uint8_t brightness, bool is_white)
    : Effect(ledStrip) {
    if (is_white) {
      this->ledStrip.setWhite();
      Serial.println("Built solid white effect");
    } else {
      this->ledStrip.setColour();
      Serial.println("Built solid colour effect");
    }
    this->ledStrip.setBrightness(brightness);
  }
  void update(unsigned long time_ms) override {
    //Serial.println("updating solid effect");
    return;
  }
};


class ChaosEffect : public Effect {

private:
  float speed;     // how quickly the effect is modulated, should be around .002
  float rng_seed;  // init conditions for the pattern, should be [-10, 10]

public:
  ChaosEffect(LEDStrip ledStrip, float speed, float rng_seed)
    : Effect(ledStrip), speed(speed), rng_seed(rng_seed) {

    this->ledStrip.setWhite();
    this->ledStrip.setBrightness(255);
    Serial.println("Built chaos effect");
  }

  void update(unsigned long time_ms) override {

    float x = time_ms % 120000;  // repeats every 2 mins, just so pattern doesn't degenerate
    x = this->speed * x;

    float sin_x = sin(pow(x, 1.1));
    float r = sin(sin_x + x + this->rng_seed);

    if (r > 0) {
      this->ledStrip.setWhite();
    } else {
      this->ledStrip.setColour();
    }

    int brightness = int(abs(255 * r));
    this->ledStrip.setBrightness(brightness);
    return;
  }
};


class ChaosEffectSingleColor : public Effect {

private:
  float speed;     // how quickly the effect is modulated, should be around .002
  float rng_seed;  // init conditions for the pattern, should be [-10, 10]
  bool is_white;  // use white or multicolor for the effect

public:
  ChaosEffectSingleColor(LEDStrip ledStrip, float speed, float rng_seed, bool is_white)
    : Effect(ledStrip), speed(speed), rng_seed(rng_seed), is_white(is_white) {

    if (this->is_white) {
      this->ledStrip.setWhite();
    } else {
      this->ledStrip.setColour();
    }

    this->ledStrip.setBrightness(255);
    Serial.println("Built single color chaos effect");
  }

  void update(unsigned long time_ms) override {

    float x = time_ms % 120000;  // repeats every 2 mins, just so pattern doesn't degenerate
    x = this->speed * x;

    float sin_x = sin(pow(x, 1.1));
    float r = sin(sin_x + x + this->rng_seed);

    // normalize between 0 and 1
    r = (r + 1) / 2;
    int brightness = int(abs(255 * r));
    this->ledStrip.setBrightness(brightness);
    return;
  }
};