#pragma once

#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"
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
    lambertian(const color &albedo) : albedo(albedo) {}
    bool scatter(
        const ray &ray_in,
        const hit_record &rec,
        color &attenuation,
        ray &scattered) const final;

    private:
    color albedo;
};

class metal : public material
{
    public:
    metal(const color &albedo, double f) : albedo(albedo), fuzz(f) {}
    bool scatter(
        const ray &ray_in,
        const hit_record &rec,
        color &attenuation,
        ray &scattered) const final;

    private:
    color albedo;
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