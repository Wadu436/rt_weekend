#pragma once

#include "rtweekend.h"
#include "vec3.h"

#include <iostream>

struct image {
    unsigned char* image;
    int image_width;
    int image_height;
} typedef image;

void write_header(unsigned int image_width, unsigned int image_height);
void write_color(std::ostream& out, color pixel_color, unsigned int samples_per_pixel = 1);
void write_color(image img, unsigned int x, unsigned int y, color pixel_color, unsigned int samples_per_pixel);
void write_image(std::ostream& out, image img);