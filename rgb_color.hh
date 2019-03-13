#ifndef LED_H_
# define LED_H_

#include <mbed.h>

class RGB_color
{
private:
    uint8_t _r;
    uint8_t _g;
    uint8_t _b;

public:
    RGB_color();
    RGB_color(uint8_t, uint8_t, uint8_t);

    uint8_t getR() const;
    uint8_t getG() const;
    uint8_t getB() const;

    void setR(uint8_t);
    void setG(uint8_t);
    void setB(uint8_t);

    void printColor();

    // operators overloading
    RGB_color& operator=(const RGB_color &);
    RGB_color& operator=(const uint8_t &);
};

#endif