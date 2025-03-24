#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
#include "hittable/hittable.h"

class camera {
  public:
    // Public parameters with sensible defaults
    double aspect_ratio = 16.0/9.0;  // Ratio of image width over height
    int    image_width  = 400;       // Rendered image width in pixel count
    double focal_length = 1.0;       // Distance from camera center to viewport plane
    double viewport_height = 2.0;    // Height of the viewport
    
    int samples_per_pixel = 10; // sample times

    void render(const hittable& world) {
        initialize(); // Re-initialize whenever parameters might have changed

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0,0,0);
                for (int cnt = 0; cnt < samples_per_pixel; cnt++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world);
                }
                pixel_color /= samples_per_pixel;
                write_color(std::cout, pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
    }

  private:
    int    image_height;   // Rendered image height
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    point3 center;         // Camera center position
    double pix_sample_rate; // Sample rate per pixel

    void initialize() {
        // Calculate image height (must be at least 1)
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        center = point3(0, 0, 0); // Camera center position
        pix_sample_rate = 1.0 / samples_per_pixel;
        // Calculate viewport dimensions
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
        
        // Calculate basis vectors for viewport orientation
        vec3 viewport_u = vec3(viewport_width, 0, 0);
        vec3 viewport_v = vec3(0, -viewport_height, 0);

        // Calculate pixel deltas
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate upper-left viewport pixel location
        auto viewport_upper_left = center 
                                 - vec3(0, 0, focal_length)  // Move along -Z axis
                                 - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int i, int j) const{
        auto offset = sample_square();
        auto pixel_center = pixel00_loc + (i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v;

        auto ray_ori = center;
        auto ray_d = pixel_center - ray_ori;

        return ray(ray_ori, ray_d);
    }

    inline vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(generate_random_double() - 0.5, generate_random_double() - 0.5, 0); // 在这个方块里随机取一个点
    }

    color ray_color(const ray& r, const hittable& world) {
        hit_record rec;
        if (world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1,1,1));
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};

#endif