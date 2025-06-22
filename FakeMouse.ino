#ifndef ARDUINO_USB_MODE
#error This ESP32 SoC has no Native USB interface
#elif ARDUINO_USB_MODE == 1
#error This sketch should be used when USB is in OTG mode
#else

#define USB_MANUFACTURER "Terekhov Research"
#define USB_PRODUCT "FakeMouse"

#define PIN_BUTTON 16
#define PIN_LED_EMBEDDED 15

#include "USB.h"
#include "USBHIDMouse.h"

bool mouse_enabled = false;

USBHIDMouse Mouse;

int button_state;
int button_last_state = HIGH;

unsigned long debounce_last_time = 0;
unsigned long debounce_delay = 50;

void setup() {
  // initialize button
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // initialize led
  pinMode(PIN_LED_EMBEDDED, OUTPUT);

  // initialize mouse control:
  Mouse.begin();
  USB.productName(USB_PRODUCT);
  USB.manufacturerName(USB_MANUFACTURER);
  USB.begin();
}

int px = 0;
int py = 0;

void loop() {
  button_loop();

  if (mouse_enabled) {
    mouse_loop();
  }
}

void handle_click_button() {
  mouse_enabled = !mouse_enabled;

  if (mouse_enabled) {
    digitalWrite(PIN_LED_EMBEDDED, HIGH);
  } else {
    digitalWrite(PIN_LED_EMBEDDED, LOW);
  }
}

void button_loop() {
  int reading = digitalRead(PIN_BUTTON);

  if (reading != button_last_state) {
    debounce_last_time = millis();
  }

  if ((millis() - debounce_last_time) > debounce_delay) {
    if (reading != button_state) {
      button_state = reading;

      if (button_state == LOW) {
        handle_click_button();
      }
    }
  }

  button_last_state = reading;
}

void mouse_loop() {
  float a = 6.28 * millis() / 1000.0;

  int x = int(42.0 * cos(a));
  int y = int(42.0 * sin(a) * cos(a));

  Mouse.move(x - px, y - py, 0);

  px = x;
  py = y;

  delay(25);
}

#endif /* ARDUINO_USB_MODE */
