#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;

class hittable_list : public hittable {
    public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const final;
    bool bounding_box(double t0, double t1, aabb& output_box) const final;

    public:
    std::vector<shared_ptr<hittable>> objects;
};
