#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

#include <cstdlib>
#include <cstring>

#include "CTPL/ctpl.h"

struct box {
    int start_x, start_y, end_x, end_y;
} typedef box;

int completed_pixels;
int last_update;
std::mutex completed_pixels_mutex;

hittable_list random_scene(int density)
{
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -density; a < density; a++) {
        for (int b = -density; b < density; b++) {
            auto choose_mat = random_double();
            point3 center(
                a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
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

color ray_color(const ray &r, const hittable &world, int depth)
{
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        color attenuation;
        ray scattered;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color(0, 0, 0);
    }

    vec3 unit_dir = r.direction().unit_vector();
    auto t = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void update_progress(int image_size)
{
    std::cerr << std::fixed << std::setprecision(2);
    std::cerr << '\r' << 100 * double(completed_pixels) / image_size << '%';
    last_update = completed_pixels;
    // std::cerr << completed_lines << "\n";
}

void render_area(
    int id,
    const hittable &world,
    const camera &cam,
    image img,
    std::mutex *img_mutex,
    int samples_per_pixel,
    int max_depth,
    box bound)
{
    for (int i = bound.start_x; i < bound.end_x; ++i) {
        for (int j = bound.start_y; j < bound.end_y; ++j) {
            color pixel_color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; s++) {
                double u = double(i + random_double()) / (img.image_width - 1);
                double v = double(img.image_height - j + random_double()) /
                           (img.image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            img_mutex->lock();
            write_color(img, i, j, pixel_color, samples_per_pixel);
            img_mutex->unlock();

            completed_pixels_mutex.lock();
            completed_pixels++;
            if (completed_pixels - last_update > 640)
                update_progress(img.image_width * img.image_height);
            completed_pixels_mutex.unlock();
        }
    }

    completed_pixels_mutex.lock();
    update_progress(img.image_width * img.image_height);
    completed_pixels_mutex.unlock();
}

int main(int argc, char *argv[])
{
    // Image
    const int max_depth = 50;

    int image_width = 1600;
    int image_height = 900;
    int samples_per_pixel = 32;
    int bounds_size = 64;

    int threads = 4;

    // Handle arguments
    for (int i = 1; i < argc; i++) {
        if (i + 1 != argc) {
            if (strcmp(argv[i], "-t") == 0 ||
                strcmp(argv[i], "--threads") == 0) {
                threads = std::atoi(argv[i + 1]);
                i++;
            }

            if (strcmp(argv[i], "-s") == 0 ||
                strcmp(argv[i], "--samples") == 0) {
                samples_per_pixel = std::atoi(argv[i + 1]);
                i++;
            }

            if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) {
                image_width = std::atoi(argv[i + 1]);
                i++;
            }

            if (strcmp(argv[i], "-h") == 0 ||
                strcmp(argv[i], "--height") == 0) {
                image_height = std::atoi(argv[i + 1]);
                i++;
            }

            if (strcmp(argv[i], "-b") == 0 ||
                strcmp(argv[i], "--bounds") == 0) {
                bounds_size = std::atoi(argv[i + 1]);
                i++;
            }
        }
    }

    // Camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto aspect_ratio = double(image_width) / image_height;
    auto dist_to_focus = 10;
    auto fstop = 100;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, fstop, dist_to_focus);

    // World
    hittable_list world = random_scene(21);

    std::cerr << "Building BVH... ";

    bvh_node root = bvh_node(world, 0, 0);

    std::cerr << "Finished\n";

    // Render
    write_header(image_width, image_height);

    image img = {
        new unsigned char[image_height * image_width * 3]{0},
        image_width,
        image_height};
    std::mutex image_mutex;

    ctpl::thread_pool pool(threads);

    completed_pixels = 0;
    last_update = 0;

    update_progress(image_height);

    int horizontal_boxes = image_width % bounds_size == 0
                               ? image_width / bounds_size
                               : 1 + image_width / bounds_size;
    int vertical_boxes = image_height % bounds_size == 0
                             ? image_height / bounds_size
                             : 1 + image_height / bounds_size;

    std::chrono::high_resolution_clock::time_point start =
        std::chrono::high_resolution_clock::now();

    for (int i = 0; i < horizontal_boxes; ++i) {
        for (int j = 0; j < vertical_boxes; ++j) {
            box bounds = {
                i * bounds_size,
                j * bounds_size,
                std::min((i + 1) * bounds_size, image_width),
                std::min((j + 1) * bounds_size, image_height)};
            pool.push(
                render_area,
                root,
                cam,
                img,
                &image_mutex,
                samples_per_pixel,
                max_depth,
                bounds);
        }
    }

    pool.stop(true);

    // Output
    write_image(std::cout, img);

    std::cerr << "\nDone.\n";

    std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    std::cerr << std::fixed << std::setprecision(3);
    std::cerr << time_span.count() << " seconds.\n";

    // Cleanup
    delete[] img.image;

    return 0;
}