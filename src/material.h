#pragma once

#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"
#include "texture.h"
#include "vec3.h"

class material
{
    public:
    virtual ~material(){};
    virtual bool scatter(
        const ray &ray_in,
        const hit_record &rec,
        color &attenuation,
        ray &scattered) const = 0;
};

class lambertian : public material
{
    public:
    lambertian(const color &a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}
    bool scatter(
        const ray &ray_in,
        const hit_record &rec,
        color &attenuation,
        ray &scattered) const final;

    private:
    shared_ptr<texture> albedo;
};

class metal : public material
{
    public:
    metal(const color &a, double f)
        : albedo(make_shared<solid_color>(a)), fuzz(f)
    {
    }
    metal(shared_ptr<texture> a, double f) : albedo(a), fuzz(f) {}

    bool scatter(
        const ray &ray_in,
        const hit_record &rec,
        color &attenuation,
        ray &scattered) const final;

    private:
    shared_ptr<texture> albedo;
    double fuzz;
};

class dielectric : public material
{
    public:
    dielectric(double ior) : ior(ior) {}
    bool scatter(
        const ray &ray_in,
        const hit_record &rec,
        color &attenuation,
        ray &scattered) const;

    private:
    double ior;
};