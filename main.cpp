#include <mbed.h>
#include "mbed_events.h"
#include "matrix.hh"
#include "matrix_controller.hh"

Matrix_controller matrix_ctrl;

// Gestion d'affichage
static Queue<Matrix, 1> matrix_full;
static Queue<Matrix, 2> matrix_empty;

static Thread displayThread;

static Matrix image1;
static Matrix image2;

static RGB_color RGB_color_tab[2] = { RGB_color(255, 0, 0), RGB_color(0, 255, 0) };
static uint8_t color_count = 0;

static void display()
{
  osEvent evt;
  Matrix *current_buffer;

  evt = matrix_full.get();

  if (evt.status == osEventMessage)
    current_buffer = (Matrix *)evt.value.p;

  while (1)
  {
    matrix_ctrl.sendMatrix(*current_buffer);

    evt = matrix_full.get(0);

    if (evt.status == osEventMessage)
    {
      matrix_empty.put(current_buffer);
      current_buffer = (Matrix *)evt.value.p;
    }
  }
}

static void blink_matrix()
{
  osEvent evt;
  Matrix *tmp_image;

  while (1)
  {
    evt = matrix_empty.get();

    if (evt.status == osEventMessage)
      tmp_image = (Matrix *) evt.value.p;

    tmp_image->setColor(RGB_color_tab[color_count++ % 2]);

    matrix_full.put(tmp_image);

    ThisThread::sleep_for(500);
  }
}

int main()
{
  matrix_empty.put(&image1);
  matrix_empty.put(&image2);

  displayThread.start(display);

  blink_matrix();
}