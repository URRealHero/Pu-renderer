#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <cstdlib>
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

inline double generate_random_double(){
    return rand() / (RAND_MAX + 1.0); // Normed rand number
}

inline double generate_random_double(double min, double max){
    return min + (max - min) * generate_random_double();
}


// Common Headers

#include "basic_impl/color.h"
#include "basic_impl/ray.h"
#include "basic_impl/vec3.h"
#include "basic_impl/interval.h"



#endif