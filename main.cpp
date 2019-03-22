#include <mbed.h>
#include "i2c.h"
#include "matrix.hh"
#include "matrix_controller.hh"

Matrix_controller matrix_ctrl;

// Gestion d'affichage
static Queue<Matrix, 1> matrix_full;
static Queue<Matrix, 2> matrix_empty;

static Matrix image1;
static Matrix image2;

static EventQueue queue;

static Thread displayThread;
static Thread t;

static InterruptIn button(BUTTON1);
static Mutex m;
static uint8_t button_pressed_count;

static void display()
{
  osEvent evt = matrix_full.get();

  Matrix *current_buffer = (Matrix *)evt.value.p;

  while (1)
  {
    matrix_ctrl.sendMatrix(current_buffer);

    evt = matrix_full.get(0);

    matrix_empty.put(current_buffer);
    current_buffer = (Matrix *)evt.value.p;
  }
}

static void rise_handle(void)
{  
  m.lock();
  button_pressed_count++;
  m.unlock();
}

int main()
{
  t.start(callback(&queue, &EventQueue::dispatch_forever));

  button.rise(queue.event(rise_handle));

  matrix_empty.put(&image1);
  matrix_empty.put(&image2);

  displayThread.start(display);

  i2c_slave_controller i2c_slave_ctrl(PB_9, PB_8, 0x2A << 1, &matrix_full, &matrix_empty, m, &button_pressed_count);
}