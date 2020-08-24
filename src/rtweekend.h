#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Structs

struct interval {
    double start, end;
} typedef interval;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

// returns a random double in the range [0, 1)
inline double random_double() { return rand() / (RAND_MAX + 1.0); }

// returns a random double in the range [min, max)
inline double random_double(double min, double max)
{
    return min + random_double() * (max - min);
}

inline double clamp(double x, double min, double max)
{
    return std::min(std::max(x, min), max);
}

inline bool overlap(interval d, interval e, interval &f)
{
    // calculates overlap of intervals d and e and puts them into f
    f.start = std::max(d.start, e.start);
    f.end = std::min(d.end, e.end);
    return f.start < f.end;
}

inline int random_int(int min, int max)
{
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max + 1));
}