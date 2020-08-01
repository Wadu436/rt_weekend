#include "aabb.h"

aabb::aabb(const point3 &a, const point3 &b) {
    _min = {std::fmin(a[0], b[0]), std::fmin(a[1], b[1]),
            std::fmin(a[2], b[2])};
    _max = {std::fmax(a[0], b[0]), std::fmax(a[1], b[1]),
            std::fmax(a[2], b[2])};
}

bool aabb::hit(const ray &r, double tmin, double tmax) const {
    for (int a = 0; a < 3; a++) {
        double inv_dir = 1.0 / r.direction()[a];
        double t0 = (_min[a] - r.origin()[a]) * inv_dir;
        double t1 = (_max[a] - r.origin()[a]) * inv_dir;

        if (inv_dir < 0.0f) {
            std::swap(t0, t1);
        }

        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin) {
            return false;
        }
    }
    return true;
}