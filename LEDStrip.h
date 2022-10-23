#include "esp32-hal-gpio.h"
#pragma once

class LEDStrip {
  // A class that acts as an interface for controlling a single LEDStrip of fairy lights.
  // The pins given in the constructor should be PWM pins that drive the H-bridge powering
  // the LEDStrip.

private:
  uint8_t whitePin;
  uint8_t colourPin;
  uint8_t currentBrightness;
  uint8_t activePin;
  uint8_t inactivePin;

public:
  LEDStrip(uint8_t whitePin, uint8_t colourPin)
    : whitePin(whitePin), colourPin(colourPin) {
    this->currentBrightness = 0;
    pinMode(whitePin, OUTPUT);  // set pin mode to output, (defaults to output typically, but good practice)
    pinMode(colourPin, OUTPUT);
    analogWrite(this->whitePin, 0);  // set both pins to low, so strip starts off
    analogWrite(this->colourPin, 0);
    setWhite();  // set the strip to start as white
  }

  void setWhite() {
    this->activePin = this->whitePin;
    this->inactivePin = this->colourPin;
    setBrightness(this->currentBrightness);
  }

  void setColour() {
    this->activePin = this->colourPin;
    this->inactivePin = this->whitePin;
    setBrightness(this->currentBrightness);
  }

  void setBrightness(uint8_t brightnessLevel) {     // brightness from 0-255
    analogWrite(this->inactivePin, 0);              // ensure inactive pin is off
    analogWrite(this->activePin, brightnessLevel);  // set to brightness level
    this->currentBrightness = brightnessLevel;
  }
};