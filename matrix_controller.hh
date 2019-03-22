#ifndef MATRIX_CONTROLLER_HH_
#define MATRIX_CONTROLLER_HH_

#include <mbed.h>
#include "matrix.hh"

class Matrix_controller
{
private:
    DigitalOut SB;
    DigitalOut LAT;
    DigitalOut RST;
    DigitalOut SCK;
    DigitalOut SDA;

    BusOut rows;

    void pulse_SCK();
    void pulse_LAT();
    void deactivate_rows();
    void activate_row(int);
    void send_byte(uint8_t, int);
    void init_bank0();

public:
    Matrix_controller();

    void sendMatrix(Matrix *);
};

#endif