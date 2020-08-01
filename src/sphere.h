#pragma once

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

#include <cmath>

using std::sqrt;

class sphere : public hittable {
    public:
    sphere() {}
    sphere(point3 center, double radius, shared_ptr<material> material)
        : center(center), radius(radius), mat_ptr(material) {}

    bool hit(const ray &r, double t_min, double t_max,
             hit_record &rec) const final;
    bool bounding_box(double t0, double t1, aabb &output_box) const final;

    private:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
};