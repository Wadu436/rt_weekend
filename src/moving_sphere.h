#pragma once

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

#include <cmath>

using std::sqrt;

class moving_sphere : public hittable
{
    public:
    moving_sphere() {}
    moving_sphere(
        point3 center0,
        point3 center1,
        double radius,
        double time0,
        double time1,
        shared_ptr<material> material)
        : cen0(center0), cen1(center1), r(radius), time0(time0), time1(time1),
          mat_ptr(material)
    {
    }

    bool
    hit(const ray &r, double t_min, double t_max, hit_record &rec) const final;
    bool bounding_box(double t0, double t1, aabb &output_box) const final;

    point3 center(double t) const
    {
        double alpha = clamp((t - time0) / (time1 - time0), 0, 1);
        return (1 - alpha) * cen0 + alpha * cen1;
    }
    inline double radius() const { return r; }

    private:
    point3 cen0, cen1;
    double r;
    double time0, time1; // time object starts/stops moving
    shared_ptr<material> mat_ptr;
};