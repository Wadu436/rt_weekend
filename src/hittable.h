#pragma once

#include "aabb.h"
#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    void set_face_normal(const ray &r, const vec3 &outward_normal)
    {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
} typedef hit_record;

class hittable
{
    public:
    virtual ~hittable(){};
    virtual bool
    hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
    virtual bool bounding_box(double t0, double t1, aabb &output_box) const = 0;
};