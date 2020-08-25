#pragma once

#include "color.h"
#include "rtweekend.h"

class texture
{
    public:
    virtual ~texture(){};
    virtual color value(double u, double v, const point3 &p) const = 0;
};

class solid_color : public texture
{
    public:
    solid_color() {}
    solid_color(color col) : color_value(col) {}
    solid_color(double r, double g, double b) : color_value(r, g, b) {}

    virtual color value(double u, double v, const point3 &p) const override
    {
        return color_value;
    }

    private:
    color color_value;
};

class checker_texture : public texture
{
    public:
    checker_texture() {}
    checker_texture(color col1, color col2)
        : even(make_shared<solid_color>(col1)),
          odd(make_shared<solid_color>(col2))
    {
    }
    checker_texture(shared_ptr<texture> tex1, shared_ptr<texture> tex2)
        : even(tex1), odd(tex2)
    {
    }

    virtual color value(double u, double v, const point3 &p) const override
    {
        double scale = 5;
        auto sign = std::sin(scale * p.x()) * std::sin(scale * p.y()) *
                    std::sin(scale * p.z());

        if (sign > 0) {
            return even->value(u, v, p);
        } else {
            return odd->value(u, v, p);
        }
    }

    private:
    shared_ptr<texture> even, odd;
};

class checker_texture_2d : public texture
{
    public:
    checker_texture_2d() {}
    checker_texture_2d(color col1, color col2)
        : even(make_shared<solid_color>(col1)),
          odd(make_shared<solid_color>(col2))
    {
    }
    checker_texture_2d(shared_ptr<texture> tex1, shared_ptr<texture> tex2)
        : even(tex1), odd(tex2)
    {
    }

    virtual color value(double u, double v, const point3 &p) const override
    {
        double scale = 16;
        auto sign = std::sin(scale * 2 * pi * u) * std::sin(scale * pi * v);

        if (sign > 0) {
            return even->value(u, v, p);
        } else {
            return odd->value(u, v, p);
        }
    }

    private:
    shared_ptr<texture> even, odd;
};