#include <mbed.h>
#include "rgb_color.hh"

RGB_color::RGB_color()
{
    _r = 0;
    _g = 0;
    _b = 0;
}

RGB_color::RGB_color(uint8_t r, uint8_t g, uint8_t b): _r(r), _g(g), _b(b) {}

uint8_t RGB_color::getR() const { return _r; }

uint8_t RGB_color::getG() const { return _g; }

uint8_t RGB_color::getB() const { return _b; }

void RGB_color::setR(uint8_t r) { _r = r; }

void RGB_color::setG(uint8_t g) { _g = g; }

void RGB_color::setB(uint8_t b) { _b = b; }

void RGB_color::printColor()
{
    printf("rgb = r[%d], g[%d], b[%d]\n", _r, _g, _b);
}

RGB_color& RGB_color::operator=(const RGB_color &rhs)
{
    if (this == &rhs)
        return *this;

    _r = rhs.getR();
    _g = rhs.getG();
    _b = rhs.getB();

    return *this;
}

RGB_color& RGB_color::operator=(const uint8_t &c)
{
    this->setR(c);
    this->setG(c);
    this->setB(c);

    return *this;
}