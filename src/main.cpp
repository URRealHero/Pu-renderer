#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - std::sqrt(discriminant) ) / (2.0*a); // 计算最近的交点(现在还不需要考虑t是负值)
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1); // Visualize the normal vector
    }


    vec3 unit_direction = unit_vector(r.direction());

    auto a = 0.5*(unit_direction.y() + 1.0); // Scale y from [-1, 1] to [0, 1]
    return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);

}

int main() {

    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the height of the image
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = image_height < 1 ? 1 : image_height;

    // Camera
    auto focal_length = 1;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    auto camera_centre = point3(0, 0, 0);


    // Calculate the vectors across the horizontal and down the vertical viewport edges. (这个Viewport是反向的)
    auto V_u = vec3(viewport_width, 0, 0);
    auto V_v = vec3(0, -viewport_height, 0);
    auto pix_delta_u = V_u / image_width;
    auto pix_delta_v = V_v / image_height;

    // Calculate the location of the upper left pixel.
    auto upper_left = camera_centre - V_u/2 - V_v/2 - vec3(0, 0, focal_length);
    auto pix_00_loc = upper_left + 0.5*(pix_delta_u + pix_delta_v);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pix_00_loc + (i * pix_delta_u) + (j * pix_delta_v);
            auto ray_direction = pixel_center - camera_centre;
            ray r(camera_centre, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}