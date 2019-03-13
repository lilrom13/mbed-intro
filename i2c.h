#ifndef I2C_H_
#define I2C_H_

#include "mbed.h"

class i2c_slave_controller: public I2CSlave
{
private:
    char _i2c_index;

    // char cmds[11] = { 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0F, 0x80, 0x81 };

    DigitalOut led1;
    DigitalOut led2;
    DigitalOut led3;

    Thread *thread;

    static void i2c_handle(void const *arg);

    void i2c_handle_write();
    void i2c_handle_read();

public:
    i2c_slave_controller(PinName sda, PinName scl, int addr);

    void start();
};

#endif