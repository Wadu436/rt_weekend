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

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = r.direction().dot(oc);
    auto c = oc.length_squared() - radius*radius;
    auto discr = half_b*half_b - a*c;

    bool hit = false;
    double t = 0;

    if (discr > 0) {
        auto root = sqrt(discr);
        
        auto temp = (-half_b + root) / a;
        if(temp > t_min && temp < t_max) {
            t = temp;
            hit = true;
        }

        temp = (-half_b - root) / a;
        if(temp > t_min && temp < t_max) {
            t = temp;
            hit = true;
        }

        if(hit) {
            rec.t = t;
            rec.p = r.at(t);
            vec3 out_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, out_normal);
            rec.mat_ptr = mat_ptr;
        }
    }   

    return hit;
};