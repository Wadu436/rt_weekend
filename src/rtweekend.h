#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// returns a random double in the range [0, 1)
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

// returns a random double in the range [min, max)
inline double random_double(double min, double max) {
    return min + random_double() * (max - min);
}

inline double clamp(double x, double min, double max) {
    return std::min(std::max(x, min), max);
}

// Common Headers

#include "ray.h"
#include "vec3.h"