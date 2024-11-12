#ifndef ARDUINO_USB_MODE
#error This ESP32 SoC has no Native USB interface
#elif ARDUINO_USB_MODE == 1
#error This sketch should be used when USB is in OTG mode
#else

#define USB_MANUFACTURER "Terekhov Research"
#define USB_PRODUCT "FakeMouse"

#include "USB.h"
#include "USBHIDMouse.h"

USBHIDMouse Mouse;

void setup() {
  // initialize mouse control:
  Mouse.begin();
  USB.begin();
}

int px = 0;
int py = 0;

void loop() {
  float a = 6.28 * millis() / 1000.0;

  int x = int(42.0 * cos(a));
  int y = int(42.0 * sin(a) * sin(a));

  Mouse.move(x - px, y - py, 0);

  px = x;
  py = y;

  delay(50);
}
#endif /* ARDUINO_USB_MODE */
