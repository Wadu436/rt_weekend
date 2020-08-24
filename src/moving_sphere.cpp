#include "moving_sphere.h"

bool moving_sphere::hit(
    const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 oc = r.origin() - center(r.time());
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
            vec3 out_normal = (rec.p - center(r.time())) / radius();
            rec.set_face_normal(r, out_normal);
            rec.mat_ptr = mat_ptr;
        }
    }

    return hit;
}

bool moving_sphere::bounding_box(double t0, double t1, aabb &output_box) const
{
    vec3 disp = {radius(), radius(), radius()};
    point3 corner_t0_1 = center(t0) - disp;
    point3 corner_t1_1 = center(t1) - disp;
    point3 corner_t0_2 = center(t0) + disp;
    point3 corner_t1_2 = center(t1) + disp;

    point3 corner1 = {
        std::fmin(corner_t0_1.x(), corner_t1_1.x()),
        std::fmin(corner_t0_1.y(), corner_t1_1.y()),
        std::fmin(corner_t0_1.z(), corner_t1_1.z())};

    point3 corner2 = {
        std::fmax(corner_t0_2.x(), corner_t1_2.x()),
        std::fmax(corner_t0_2.y(), corner_t1_2.y()),
        std::fmax(corner_t0_2.z(), corner_t1_2.z())};

    output_box = {corner1, corner2};
    return true;
}