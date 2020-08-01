#pragma once

#include "rtweekend.h"

#include <iostream>
#include <cmath>

class vec3 {
    private:
    double e[3];

    public:
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    vec3(const vec3& o) : e{o.e[0], o.e[1], o.e[2]} {};

    inline double x() const { return e[0]; }
    inline double y() const { return e[1]; }
    inline double z() const { return e[2]; }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    static vec3 random_in_unit_sphere() {
        while(true) {
            auto p = vec3::random(-1, 1);
            if (p.length_squared() >= 1) continue;
            return p;
        }
    }

    static vec3 random_in_unit_disk() {
        while (true) {
            auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
            if (p.length_squared() >= 1) continue;
            return p;
        }
    }

    static vec3 random_unit_vector() {
        auto a = random_double(0, 2*pi);
        auto z = random_double(-1, 1);
        auto r = (1 - z*z);
        return vec3(r*cos(a), r*sin(a), z);
    }

    // Unitary operators
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); };
    inline double operator[](int i) const { return e[i]; };
    inline double& operator[](int i) { return e[i]; }

    // Compound assignment operators
    inline vec3& operator+=(const vec3& o) {
        e[0] += o.e[0];
        e[1] += o.e[1];
        e[2] += o.e[2];
        return *this;
    }
    inline vec3& operator-=(const vec3& o) {
        return *this += -o;
    }
    inline vec3& operator*=(const vec3 o) {
        e[0] *= o.e[0];
        e[1] *= o.e[1];
        e[2] *= o.e[2];
        return *this;
    }
    inline vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }
    inline vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    // Regular binary operators
    inline const vec3 operator+(const vec3& o) const {
        return vec3(*this) += o;
    }
    inline const vec3 operator-(const vec3& o) const {
        return vec3(*this) -= o;
    }
    inline const vec3 operator*(const vec3& o) const {
        return vec3(*this) *= o;
    }
    inline const vec3 operator*(const double t) const {
        return vec3(*this) *= t;
    }
    inline friend vec3 operator*(const double t, const vec3& o) {
        return vec3(o) *= t;
    }
    inline const vec3 operator/(const double t) const {
        return vec3(*this) /= t;
    }

    // IO operators
    inline friend std::ostream& operator<<(std::ostream& out, const vec3& v) {
        return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    }

    // Vector functions
    double dot(const vec3& o) const {
        return  e[0] * o.e[0] + 
                e[1] * o.e[1] + 
                e[2] * o.e[2];
    }
    vec3 cross(const vec3& o) const {
        return vec3(e[1] * o.e[2] - e[2] * o.e[1],
                    e[2] * o.e[0] - e[0] * o.e[2],
                    e[0] * o.e[1] - e[1] * o.e[0]);
    }
    inline vec3 unit_vector() const {
        return (*this) / length();
    }
    inline double length_squared() const {
        return dot(*this);
    }
    inline double length() const {
        return sqrt(length_squared());
    }

    vec3 reflect(const vec3& normal) const {
        return *this - normal * (2 * dot(normal));
    }
    vec3 refract(const vec3& normal, double etai_over_etat) {
        auto cos_theta = dot(-normal);
        vec3 r_out_perp = (*this + normal * cos_theta) * etai_over_etat;
        vec3 r_out_parallel = normal * -sqrt(fabs(1.0 - r_out_perp.length_squared()));
        return r_out_perp + r_out_parallel;
    }
};

using point3 = vec3; // 3D Point
using color = vec3;  // Color