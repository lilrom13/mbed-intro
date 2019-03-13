#ifndef MATRIX_HH_
#define MATRIX_HH_

#include <mbed.h>
#include "rgb_color.hh"

class Matrix
{
private:
    RGB_color _pixels[64];

public:
    Matrix();
    Matrix(const RGB_color &);

   const RGB_color &getPixel(const uint8_t) const;
   void setPixel(const uint8_t i, const RGB_color &);
   void setColor(const RGB_color &);
   void printMatrix();
};

#endif