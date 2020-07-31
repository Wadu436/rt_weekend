#include "rtweekend.h"

#include "color.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <chrono>

#include <CTPL/ctpl.h>

unsigned int completed_lines;
std::mutex completed_lines_mutex;

hittable_list random_scene(int density) {
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -density; a < density; a++) {
        for (int b = -density; b < density; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

color ray_color(const ray& r, const hittable& world, int depth) {
    if (depth <= 0) return color(0, 0, 0);

    hit_record rec;
    if(world.hit(r, 0.001, infinity, rec)) {
        color attenuation;
        ray scattered;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color(0, 0, 0);
    }

    vec3 unit_dir = r.direction().unit_vector();
    auto t = 0.5*(unit_dir.y() + 1.0);
    return (1.0-t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void update_progress(unsigned int image_height) {
    std::cerr << std::fixed << std::setprecision(2);
    std::cerr << '\r' << 100 * double(completed_lines) / image_height << '%';
    //std::cerr << completed_lines << "\n";
}

void scanline(int id, const hittable& world, const camera& cam, image img, std::mutex* img_mutex, unsigned int line, unsigned int samples_per_pixel, unsigned int max_depth) {
    for(unsigned int i = 0; i < img.image_width; ++i) {
        color pixel_color(0, 0, 0);

        for(unsigned int s = 0; s < samples_per_pixel; s++) {
            double u = double(i + random_double()) / (img.image_width - 1);
            double v = double(img.image_height - line + random_double()) / (img.image_height - 1);
            ray r = cam.get_ray(u, v);
            pixel_color += ray_color(r, world, max_depth);
        }

        img_mutex->lock();
        write_color(img, i, line, pixel_color, samples_per_pixel);
        img_mutex->unlock();
    }
    completed_lines_mutex.lock();
    completed_lines++;
    update_progress(img.image_height);
    completed_lines_mutex.unlock();
}

int main() {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    // Image
    const auto aspect_ratio = 3.0 / 2.0;
    const unsigned int image_width = 300;
    const unsigned int image_height = static_cast<int>(image_width / aspect_ratio);
    const unsigned int samples_per_pixel = 12;
    const unsigned int max_depth = 50;

    // Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10;
    auto fstop = 100;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, fstop, dist_to_focus);

    // World
    hittable_list world = random_scene(3);

    // Render
    write_header(image_width, image_height);

    image img = {new unsigned char[image_height*image_width*3] {0}, image_width, image_height};
    std::mutex image_mutex;

    ctpl::thread_pool pool(16);

    completed_lines = 0;

    for(unsigned int j = 0; j < image_height; ++j) {
        pool.push(scanline, world, cam, img, &image_mutex, j, samples_per_pixel, max_depth);
        //scanline(0, world, cam, img, &image_mutex, j, samples_per_pixel, max_depth);
    }

    pool.stop(true);

    // Output
    write_image(std::cout, img);

    std::cerr << "\nDone.\n";

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    std::cerr << std::fixed << std::setprecision(3);
    std::cerr << time_span.count() << " seconds.\n";

    // Cleanup
    delete[] img.image;
}