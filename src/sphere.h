#pragma once

#include "hittable.h"

#include <cmath>

using std::sqrt;

class sphere : public hittable {
    public:
    sphere() {}
    sphere(point3 center, double radius, shared_ptr<material> material) : center(center), radius(radius), mat_ptr(material) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    private:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
};