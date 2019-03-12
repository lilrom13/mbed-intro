#include <mbed.h>
#include "mbed_events.h"

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_t;

static DigitalOut led1(LED1);
static DigitalOut led2(LED2);

static DigitalOut SB(PC_5);
static DigitalOut LAT(PC_4);
static DigitalOut RST(PC_3);
static DigitalOut SCK(PB_1);
static DigitalOut SDA(PA_4);

static InterruptIn button(BUTTON1);

static EventQueue queue(32 * EVENTS_EVENT_SIZE);
static Thread t;

BusOut rows(PB_2, PA_15, PA_2, PA_7, PA_6, PA_5, PB_0, PA_3);

static void led_g_on()
{
  led2 = 1;
}

static void led_g_off()
{
  led2 = 0;
}

static void pulse_SCK()
{
  SCK = 0;
  SCK = 1;
  SCK = 0;
}

static void pulse_LAT()
{
  LAT = 1;
  LAT = 0;
  LAT = 1;
}

static void deactivate_rows()
{
  rows = 0;
}

static void activate_row(int row)
{
  rows = 1 << row;
}

static void send_byte(uint8_t val, int bank)
{
    SB = bank;
    uint8_t MSB;

    for(int i = 0; i < 8; i++)
    {
      MSB = (val & (1 << (7 - i))) ? 1 : 0;
      SDA = MSB;
      pulse_SCK();
    }
}

void mat_set_row(int row, const rgb_t *color)
{
    for(int i = 7; i >= 0; i--){
      send_byte(color->b, 1);
      send_byte(color->g, 1);
      send_byte(color->r, 1);
      color++;
    }

    pulse_LAT();
    activate_row(row);
}

static void test_pixels()
{
  rgb_t matrix[8][8];

  for (int i=0; i<8; i++)
    for (int j=0; j<8; j++)
    {
      matrix[i][j].r = 255;
      matrix[i][j].g = 0;
      matrix[i][j].b = 0;
     }

  for(;;)
    for (int i = 0; i < 8; i++)
      mat_set_row(i, matrix[0]);
}

static void init_bank0()
{
  for(int i = 0; i < 18; i++)
      send_byte(0xFF, 0);
  pulse_LAT();
}

static void init_matrix()
{
  deactivate_rows();

  LAT = 1;
  SB = 1;
  SCK = 0;
  SDA = 0;
  RST = 0;
  
  wait(0.1);
  
  RST = 1;

  init_bank0();
}

static void fall_handler(void)
{
  printf("fall_handler in context %p\r\n", Thread::gettid());
  // Toggle LED
  led1 = 1;
  wait(0.1);
  led1 = 0;
}

int main() {
  // Start the event queue
  t.start(callback(&queue, &EventQueue::dispatch_forever));
  printf("Starting in context %p\r\n", Thread::gettid());
  // The 'fall' handler will execute in the context of thread 't'
  button.fall(queue.event(fall_handler));

  for (;;) {
    led2 = !led2;
    wait(0.2);
  }
  init_matrix();

  test_pixels();
}