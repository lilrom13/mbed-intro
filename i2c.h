#ifndef I2C_H_
#define I2C_H_

#include "mbed.h"
#include "matrix.hh"

class i2c_slave_controller: public I2CSlave
{
private:
    char _addr;
    const char _id;

    Queue<Matrix, 1> *_matrix_full;
    Queue<Matrix, 2> *_matrix_empty;

    DigitalOut led1;
    DigitalOut led2;
    DigitalOut led3;

    Mutex &_m;
    uint8_t *_button_pressed_count;

    char scratchpad[4];

    Thread _thread;

    int wait_for_request();
    int answer_read_request(const char *, size_t);
    int get_write_request_parameters(char *, size_t);
    void handle_command(char);

    static void threadStarter(void const *p);

    void loop();
    
public:
    i2c_slave_controller(PinName sda, PinName scl, char addr, Queue<Matrix, 1> * matrix_full, Queue<Matrix, 2> * matrix_empty, Mutex &m, uint8_t *button_pressed_count);
};

#endif