#include "sphere.h"

#include <cmath>

void get_sphere_uv(const vec3 &p, double &u, double &v)
{
    auto phi = std::atan2(p.z(), p.x());
    auto theta = std::asin(p.y());
    u = 1 - (phi + pi) / (2 * pi);
    v = (theta + pi / 2) / pi;
}

bool sphere::hit(
    const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 oc = r.origin() - center();
    auto a = r.direction().length_squared();
    auto half_b = r.direction().dot(oc);
    auto c = oc.length_squared() - radius() * radius();
    auto discr = half_b * half_b - a * c;

    bool hit = false;
    double t = 0;

    if (discr > 0) {
        auto root = sqrt(discr);

        auto temp = (-half_b + root) / a;
        if (temp > t_min && temp < t_max) {
            t = temp;
            hit = true;
        }

        temp = (-half_b - root) / a;
        if (temp > t_min && temp < t_max) {
            t = temp;
            hit = true;
        }

        if (hit) {
            rec.t = t;
            rec.p = r.at(t);
            vec3 out_normal = (rec.p - center()) / radius();
            rec.set_face_normal(r, out_normal);
            rec.mat_ptr = mat_ptr;
            get_sphere_uv((rec.p - center()) / radius(), rec.u, rec.v);
        }
    }

    return hit;
}

bool sphere::bounding_box(double t0, double t1, aabb &output_box) const
{
    vec3 disp = {radius(), radius(), radius()};
    output_box = {center() - disp, center() + disp};
    return true;
}