#include "mbed.h"
#include "i2c.h"
#include "matrix.hh"

i2c_slave_controller::i2c_slave_controller(PinName sda, PinName scl, char addr, Queue<Matrix, 1> * matrix_full, Queue<Matrix, 2> * matrix_empty, Mutex &m, uint8_t *button_pressed_count): 
I2CSlave(sda, scl), _addr(addr), _id('*'), _matrix_full(matrix_full), _matrix_empty(matrix_empty), led1(LED1), led2(LED2), led3(LED3),
_m(m), _button_pressed_count(button_pressed_count), _thread()
{
    address(_addr);

    for (int i = 0; i < 4; i++)
        scratchpad[0] = 0;

    threadStarter(this);
}

void i2c_slave_controller::threadStarter(void const *p)
{
    i2c_slave_controller *instance = (i2c_slave_controller*)p;
    instance->loop();
}

void i2c_slave_controller::loop()
{
    while (1)
    {
        // Waiting for a write from the master
        // printf("Waiting for write\r\n");
        if (wait_for_request())
        {
            // we received a write            
            // getting the command
            char cmd;

            if (read(&cmd, 1) == 0)
                printf("read the cmd with success: cmd = %x\r\n", cmd);
            else
                printf("read error\r\n");

            // now the master can either do a read or a writex
            handle_command(cmd);
        }
    }
}

void i2c_slave_controller::handle_command(char cmd)
{
    switch (cmd)
    {
        case 0x02: // (Écriture) Contenu à écrire dans le scratchpad ✅
        {
            char param1[4];

            get_write_request_parameters(param1, 4);
            for (int i = 0; i < 4; i++)
                scratchpad[i] = param1[i];
        }
        break;
        case 0x06: // (Écriture) Le bit 0 (poids faible) est écrit dans LED1, le bit 1 dans LED2 et le bit 2 dans LED3 ✅
        {
            char ledParam;

            get_write_request_parameters(&ledParam, 1);

            led1 = ledParam & (1 << 0);
            led2 = ledParam & (1 << 1);
            led3 = ledParam & (1 << 2);
        }
        break;
        case 0x10: // (Écriture) Image à afficher sur la matrice de leds ✅
        {
            char matrix_data[64*3];

            get_write_request_parameters(matrix_data, 64*3);

            osEvent evt = _matrix_empty->get();

            Matrix * tmp_image = (Matrix *) evt.value.p;

            printf("img = %p\n\r", tmp_image);
            
            if (tmp_image != NULL)
                tmp_image->setMatrix(matrix_data);

            _matrix_full->put(tmp_image);
        }
        break;
        case 0x80: // (Lecture) Nombre d'appui sur le bouton BUTTON1 depuis la dernière lecture ✅
        {
            _m.lock();
            char value = (char) *_button_pressed_count;
            *_button_pressed_count = 0;
            _m.unlock();

            if (answer_read_request(&value, 1) == 0)
                printf("answer_read_request for 0x80 with success\r\n");
            else
                printf("answer_read_request error\r\n");
        }
        break;
        case 0x82: // (Lecture) Contenu du scratchpad ✅
        {
            if (answer_read_request(scratchpad, 4) == 0)
                printf("answer_read_request for 0x82 with success\r\n");
            else
                printf("answer_read_request error\r\n");
        }
        break;
        case 0x83: // (Lecture) Adresse I²C de l'esclave sur 7 bits ✅
        {
            if (answer_read_request(&_id, 1) == 0)
                printf("answer_read_request for 0x83 with success\r\n");
            else
                printf("answer_read_request error\r\n");   
        } 
        break;
    }
}

int i2c_slave_controller::wait_for_request()
{
    int i;

    do
    {
        i = receive();

        switch (i)
        {
            case I2CSlave::ReadAddressed:
                return 0;
            case I2CSlave::WriteGeneral:
                return 1;
            case I2CSlave::WriteAddressed:
                return 1;
        }
    } while (i == I2CSlave::NoData);
}

/*
// Écrire une fonction int answer_read_request(const char *data, size_t len)
// qui attend une requête en lecture (en utilisant wait_for_request) et envoie le
// contenu de data (avec longueur len). Si une requête en écriture arrive à la place
// ou si une erreur d'écriture de data a lieu, la fonction renvoie 1, ou 0 si tout s'est bien passé.
*/
int i2c_slave_controller::answer_read_request(const char *data, size_t len)
{
    while (1)
    {
        int i = receive();

        switch (i)
        {
            case I2CSlave::ReadAddressed:
                // envoie le contenu de data (avec longueur len)
                if (write(data, len) == 0)
                    return 0;
                else
                    return 1;
            case I2CSlave::WriteGeneral:
                return 1;
            case I2CSlave::WriteAddressed:
                return 1;
        }
    }
}

/*
// Écrire une fonction int get_write_request_parameters(char *data, size_t len)
// qui attend une requête en écriture (en utilisant wait_for_request) et remplit le
// contenu de data (avec longueur len) à partir de ce qu'envoie le maître. 
// Si une requête en lecture arrive à la place ou si une erreur de lecture de data a lieu,
// la fonction renvoie 1, ou 0 si tout s'est bien passé.
*/
int i2c_slave_controller::get_write_request_parameters(char *param, size_t len)
{
    while (1)
    {
        int i = receive();

        switch (i)
        {
            case I2CSlave::ReadAddressed:
                return 1;
            case I2CSlave::WriteGeneral:
                if (read(param, len) == 0)
                    return 0;
                else
                    return 1;
            case I2CSlave::WriteAddressed:
                if (read(param, len) == 0)
                    return 0;
                else
                    return 1;        
        }
    }
}