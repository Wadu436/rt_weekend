#pragma once

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include "rtweekend.h"

class bvh_node : public hittable
{
    private:
    shared_ptr<hittable> left, right;
    aabb b_box;

    public:
    bvh_node() {}
    bvh_node(hittable_list &list, double time0, double time1)
        : bvh_node(list.objects, 0, list.objects.size(), time0, time1)
    {
    }
    bvh_node(
        std::vector<shared_ptr<hittable>> &objects,
        size_t start,
        size_t end,
        double time0,
        double time1);

    bool
    hit(const ray &r, double t_min, double t_max, hit_record &rec) const final;
    bool bounding_box(double t0, double t1, aabb &output_box) const final
    {
        output_box = b_box;
        return true;
    }
};

inline bool box_compare(
    const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis)
{
    aabb box_a, box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b)) {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }

    return box_a.min()[axis] < box_b.min()[axis];
}
