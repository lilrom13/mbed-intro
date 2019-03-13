#include <mbed.h>
#include "matrix.hh"

Matrix::Matrix() {
    for (int i = 0; i < 64; i++)
        _pixels[i] = 0;
}

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

void Matrix::printMatrix()
{
    for (int i = 0; i < 64; i++)
    {
        printf("matrix[%d] = ", i);
        _pixels[i].printColor();

        if (i % 8 == 0 && i != 63)
            printf("\n");
    }
}