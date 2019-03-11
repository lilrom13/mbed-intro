#include <mbed.h>

static DigitalOut led(LED3);

int main() {
  for (;;) {
    led = !led;
    wait(0.2);
  }
}
