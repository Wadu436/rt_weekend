#include "material.h"

double schlick(double cosine, double ref_idx)
{
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 *= r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool lambertian::scatter(
    const ray &ray_in,
    const hit_record &rec,
    color &attenuation,
    ray &scattered) const
{
    point3 scatter_dir = rec.normal + vec3::random_unit_vector();
    scattered = ray(rec.p, scatter_dir);
    attenuation = albedo;
    return true;
}

bool metal::scatter(
    const ray &ray_in,
    const hit_record &rec,
    color &attenuation,
    ray &scattered) const
{
    vec3 reflected = ray_in.direction().reflect(rec.normal).unit_vector();
    scattered = ray(rec.p, reflected + fuzz * vec3::random_in_unit_sphere());
    attenuation = albedo;
    return scattered.direction().dot(rec.normal) > 0;
}

bool dielectric::scatter(
    const ray &ray_in,
    const hit_record &rec,
    color &attenuation,
    ray &scattered) const
{
    attenuation = color(1.0, 1.0, 1.0);
    double etai_over_etat = rec.front_face ? (1.0 / ior) : ior;
    vec3 unit_dir = ray_in.direction().unit_vector();

    double cos_theta = fmin(unit_dir.dot(-rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    vec3 reflected = unit_dir.reflect(rec.normal);
    scattered = ray(rec.p, reflected);

    if (etai_over_etat * sin_theta > 1.0) {
        return true;
    }

    double reflect_prob = schlick(cos_theta, etai_over_etat);
    if (random_double() >= reflect_prob) {
        vec3 refracted = unit_dir.refract(rec.normal, etai_over_etat);
        scattered = ray(rec.p, refracted);
    }
    return true;
}