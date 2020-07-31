#pragma once

#include "rtweekend.h"

#include <iostream>

void write_header(unsigned int image_width, unsigned int image_height) {
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
}

void write_color(std::ostream& out, color pixel_color, unsigned int samples_per_pixel = 1) {
    // Write the translated [0,255] value of each color component.
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

void write_color(unsigned char* image, unsigned int image_width, unsigned int image_height, unsigned int x, unsigned int y, color pixel_color, unsigned int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);

    unsigned char ir = static_cast<int>(256 * clamp(r, 0.0, 0.999));
    unsigned char ig = static_cast<int>(256 * clamp(g, 0.0, 0.999));
    unsigned char ib = static_cast<int>(256 * clamp(b, 0.0, 0.999));

    unsigned int index = 3 * (y * image_width + x);
    image[index++] = ir;
    image[index++] = ig;
    image[index++] = ib;
}

void write_image(std::ostream& out, unsigned char* image, unsigned int image_width, unsigned int image_height) {
    unsigned int size = image_height*image_width*3;
    for(unsigned int i = 0; i < size; i++) {
        out << int(image[i]);
        i % 3 == 2 ? out << '\n' : out << ' ';
    }
}