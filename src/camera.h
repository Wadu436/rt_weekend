#pragma once

#include "rtweekend.h"

class camera
{
    public:
    camera(
        point3 lookfrom,
        point3 lookat,
        vec3 vup,
        double vfov,
        double aspect_ratio,
        double fstop,
        double focus_dist,
        double t0,
        double t1)
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        double viewport_height = 2.0 * h;
        double viewport_width = aspect_ratio * viewport_height;

        w = (lookfrom - lookat).unit_vector();
        u = vup.cross(w).unit_vector();
        v = w.cross(u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner =
            origin - horizontal / 2.0 - vertical / 2.0 - focus_dist * w;
        auto aperture = focus_dist / fstop;
        lens_radius = aperture / 2.0;

        time0 = t0;
        time1 = t1;
    }

    ray get_ray(double s, double t) const
    {
        vec3 rd = lens_radius * vec3::random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        double time = random_double(time0, time1);

        return ray(
            origin + offset,
            lower_left_corner + s * horizontal + t * vertical - origin - offset,
            time);
    }

    private:
    point3 origin, lower_left_corner;
    vec3 horizontal, vertical, u, v, w;
    double lens_radius;
    double time0, time1; // open/close shutter time
};