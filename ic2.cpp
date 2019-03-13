#include "mbed.h"
#include "i2c.h"

i2c_slave_controller::i2c_slave_controller(PinName sda, PinName scl, int addr): I2CSlave(sda, scl), led1(LED1), led2(LED2), led3(LED3)
{
    address(addr);
    _i2c_index = 0x06;

    printf("addr = %d, i2c_index = %d\n", addr, _i2c_index);
}

void i2c_slave_controller::start()
{
    thread = new Thread(callback(i2c_handle, this));
}

void i2c_slave_controller::i2c_handle_write()
{
    char buf[1];

    int ret = read(&_i2c_index, 1);

    printf("ret = %d, i2c_index = %d, \n", ret, _i2c_index);

    if (ret != 0)
    {
        printf("empty paquet\n");
        return;
    }

    while (1)
    {
        if (read(buf, 1) < 0)
        {
            printf("end of transaction\n");
            stop();
            return;
        }

        switch (buf[0])
        {
            case 0x06:
                led1 = buf[0]? 1: 0;
                _i2c_index++;
                break;

            case 0x07:
                led2 = buf[0]? 1: 0;
                _i2c_index++;
                break;

            case 0x08:
                led3 = buf[0]? 1: 0;
                _i2c_index++;
                break;
        }
    }
}

void i2c_slave_controller::i2c_handle_read()
{
    // on va lire ce que le maître demande puis on lui répond
}

void i2c_slave_controller::i2c_handle(void const *arg)
{
    i2c_slave_controller* self = (i2c_slave_controller*)arg;

    while (1)
    {
        int i = self->receive();
        switch (i) {
            case I2CSlave::ReadAddressed:
                self->i2c_handle_read();
                break;
            case I2CSlave::WriteGeneral:
                self->i2c_handle_write();
                break;
            case I2CSlave::WriteAddressed:
                self->i2c_handle_write();
                break;
        }
    }
}