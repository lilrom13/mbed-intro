#include "mbed.h"
#include "i2c.h"

i2c_slave_controller::i2c_slave_controller(PinName sda, PinName scl, int addr): I2CSlave(sda, scl), led1(LED1), led2(LED2), led3(LED3)
{
    address(addr);
    _i2c_index = 0;

    for (int i = 0; i < 16; i++)
        scratchpads[0] = 0;
}

void i2c_slave_controller::start()
{
    thread = new Thread(callback(i2c_handle, this));
}

void i2c_slave_controller::i2c_handle_write()
{
    char buf[1];
    int ret = 0;
    int scratchpad_index = 0;

    ret = read(&_i2c_index, 1);

    if (ret != 0)
    {
        printf("empty paquet\n");
        return;
    }

    while (1)
    {
        int index = 0;

        if (read(buf, 1) < 0)
        {
            printf("end of transaction\n");
            stop();
            return;
        }

        switch (buf[0])
        {
            case 0x02:
                scratchpad_index = 0;
                scratchpads[scratchpad_index + index++] = buf[0];
                _i2c_index++;
                break;
            
            case 0x03:
                scratchpad_index = 4;
                scratchpads[scratchpad_index + index++] = buf[0];
                _i2c_index++;
                break;

            case 0x04:
                scratchpad_index = 8;
                scratchpads[scratchpad_index + index++] = buf[0];
                _i2c_index++;
                break;

            case 0x05:
                scratchpad_index = 12;
                scratchpads[scratchpad_index + index++] = buf[0];
                _i2c_index++;
                break;

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
    char buf;
    ret = read(&buf, 1);

    if (ret != 0)
    {
        printf("empty paquet\n");
        return;
    }

    switch (buf)
    {
        case 0x0F:
            write(address, 1);
            break;
    
        default:
            break;
    }
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