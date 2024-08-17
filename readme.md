# LED Controller

The code in the `multi` folder allows for the control up to six fairy light style LEDs, where white or colour is selected by which way the current is being driven. This was done using H-bridges to control the current flow.

An example for a single strip is given in the `single` folder.

The capacitive touch sensors are used to cycle different modes of operation.

The code is written for a ESP32 micro controller, which can be flashed using the Arduino IDE 2 (on Windows you may need to install the CP2102 driver).
