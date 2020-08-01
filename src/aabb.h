#pragma once

#include "rtweekend.h"
#include "ray.h"
#include "vec3.h"

class aabb {
    private:
    point3 _min, _max;
    
    public:
    aabb() {}
    aabb(const point3& a, const point3& b);

    static aabb surrounding_box(aabb box0, aabb box1) {
        point3 small(
            std::fmin(box0.min().x(), box1.min().x()),
            std::fmin(box0.min().y(), box1.min().y()),
            std::fmin(box0.min().z(), box1.min().z())
            );

        point3 big(
            std::fmax(box0.max().x(), box1.max().x()),
            std::fmax(box0.max().y(), box1.max().y()),
            std::fmax(box0.max().z(), box1.max().z())
            );

        return aabb(small,big);
    }

    bool hit(const ray& r, double t_min, double t_max) const;

    inline point3 min() const {return _min; }
    inline point3 max() const {return _max; }
};