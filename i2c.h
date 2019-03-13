#ifndef I2C_H_
#define I2C_H_

#include "mbed.h"

class i2c_slave_controller: public I2CSlave
{
private:
    char _i2c_index;

    DigitalOut led1;
    DigitalOut led2;
    DigitalOut led3;

    char scratchpads[16];

    Thread *thread;

    static void i2c_handle(void const *arg);

    void i2c_handle_write();
    void i2c_handle_read();

public:
    i2c_slave_controller(PinName sda, PinName scl, int addr);

    void start();
};

#endif