#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers

#include "basic_impl/color.h"
#include "basic_impl/ray.h"
#include "basic_impl/vec3.h"
#include "basic_impl/interval.h"



#endif