#include <mbed.h>
#include "matrix.hh"

Matrix::Matrix(const RGB_color &color)
{
    for (int i = 0; i < 64; i++)
        _pixels[i] = color;
}

const RGB_color &Matrix::getPixel(uint8_t i) const { return _pixels[i]; }

void Matrix::setPixel(uint8_t i, const RGB_color &color) { _pixels[i] = color; };

void Matrix::setColor(const RGB_color &color) 
{
    for (int i = 0; i < 64; i++)
        _pixels[i] = color; 
}

void Matrix::setMatrix(const char *data)
{
    for (int i = 0; i < 64; i++)
        _pixels[i] = RGB_color(data[i *3], data[i*3 +1], data[i*3 +2]);
}

void Matrix::printMatrix()
{
    for (int i = 0; i < 64; i++)
    {
        printf("matrix[%d] = ", i);
        _pixels[i].printColor();

        if ((i % 8 == 0) && (i != 63) && (i != 0))
            printf("\n");
    }
}