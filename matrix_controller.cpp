
#include <mbed.h>
#include "matrix_controller.hh"
#include "matrix.hh"
#include "rgb_color.hh"

Matrix_controller::Matrix_controller(): SB(PC_5), LAT(PC_4), RST(PC_3), SCK(PB_1), SDA(PA_4), 
rows(PB_2, PA_15, PA_2, PA_7, PA_6, PA_5, PB_0, PA_3)
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

void Matrix_controller::pulse_SCK()
{
    SCK = 0;
    SCK = 1;
    SCK = 0;
}

void Matrix_controller::pulse_LAT()
{
    LAT = 1;
    LAT = 0;
    LAT = 1;
}

void Matrix_controller::deactivate_rows() { rows = 0; }

void Matrix_controller::activate_row(int row) { rows = 1 << row; }

void Matrix_controller::init_bank0()
{
    for(int i = 0; i < 18; i++)
        send_byte(0xFF, 0);
    pulse_LAT();
}

void Matrix_controller::send_byte(const uint8_t val, int bank)
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

void Matrix_controller::sendMatrix(const Matrix &m)
{
    for (int row = 0; row < 8; row++)
    {
        for(int i = 7; i >= 0; i--)
        {
            RGB_color color = m.getPixel(row * 8 + i);

            send_byte(color.getB(), 1);
            send_byte(color.getG(), 1);
            send_byte(color.getR(), 1);
        }

        pulse_LAT();
        activate_row(row);
    }
}

