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
    point3 center = point3(0, 0, 0); // Camera center position

    void render(const hittable& world) {
        initialize(); // Re-initialize whenever parameters might have changed

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - center;
                ray r(center, ray_direction);

                color pixel_color = ray_color(r, world);
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

    void initialize() {
        // Calculate image height (must be at least 1)
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

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