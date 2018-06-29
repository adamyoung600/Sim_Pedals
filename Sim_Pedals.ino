#include <HID-Project.h>
#include <HID-Settings.h>
#include <Joystick.h>

#include <HX711.h>

#define THROTTLE_DAT 4
#define THROTTLE_CLK 5
#define BRAKE_DAT 2
#define BRAKE_CLK 3
#define CLUTCH_DAT 6
#define CLUTCH_CLK 7

#define UP_SHIFT 8
#define DOWN_SHIFT 9

#define DISABLE_PIN 10

HX711 throttle(THROTTLE_DAT, THROTTLE_CLK);
HX711 brake(BRAKE_DAT, BRAKE_CLK);
HX711 clutch(CLUTCH_DAT, CLUTCH_CLK);

float scale_factor = 1000;

float throttle_raw;
float throttle_raw_max = 150.0;
float brake_raw;
float brake_raw_max = 450.0;
float clutch_raw;
float clutch_raw_max = 250.0;

Joystick_ joystick;

float max_output_value = 1023;

void setup() {
  delay(5000);
  
  throttle.tare();
  throttle.set_scale(scale_factor);
  brake.tare();
  brake.set_scale(scale_factor);
  clutch.tare();
  clutch.set_scale(scale_factor);
  
  joystick.begin();
}

int16_t convertValue(float raw_reading, float max_reading) {
  if(raw_reading > max_reading) {
    return max_output_value;
  }
  float result = (max_output_value * (raw_reading / max_reading));
  if(result < 0.0) {
    int16_t zero = 0;
    return zero;
  }
  int16_t ret_value = (int16_t)result;
  return ret_value;
}

void loop() {
  //Pedal loop
  joystick.setXAxis(convertValue(throttle.get_units(), throttle_raw_max));
  joystick.setYAxis(convertValue(brake.get_units(), brake_raw_max));
  joystick.setThrottle(convertValue(clutch.get_units(), clutch_raw_max));
}
