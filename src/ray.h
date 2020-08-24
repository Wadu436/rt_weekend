#pragma once

#include "vec3.h"

class ray
{
    private:
    point3 orig;
    vec3 dir;
    double t;

    public:
    ray() {}
    ray(const point3 &origin, const vec3 &direction, double time)
        : orig(origin), dir(direction), t(time)
    {
    }

    inline point3 origin() const { return orig; }
    inline vec3 direction() const { return dir; }
    inline double time() const { return t; }

    inline point3 at(double t) const { return orig + t * dir; }
};