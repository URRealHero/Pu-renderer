#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

#include <iostream>

using color = vec3;

inline double linear2gamma(double c_p){
    return std::sqrt(c_p);
}

void write_color(std::ostream& out, const color& pixel_color);


#endif