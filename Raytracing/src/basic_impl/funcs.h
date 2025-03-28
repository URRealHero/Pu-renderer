#ifndef FUNCS_H
#define FUNCS_H

#include <cstdlib>
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

#endif