#ifndef CAMERA_H
#define CAMERA_H

#include "basic_impl/funcs.h"
#include "basic_impl/vec3.h"
#include "headers.h"
#include "hittable/material.h"
#include "hittable/hittable.h"

class camera {
  public:
    // Public parameters with sensible defaults
    double aspect_ratio = 16.0/9.0;  // Ratio of image width over height
    int    image_width  = 400;       // Rendered image width in pixel count
    // double focal_length = 1.0;       // Distance from camera center to viewport plane
    int max_depth = 10;     // 在这里定义光线最多scatter次数
    double v_fov = 90;     // Vertical field-of-view in degrees
    
    point3 lookfrom = point3(0, 0, 0); // Camera center position
    point3 lookat = point3(0, 0, -1);   // Camera look-at position
    vec3 vup = vec3(0, 1, 0);           // Camera up vector

    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus


    int samples_per_pixel = 10; // sample times (用于antialiasing)

    void render(const hittable& world) {
        initialize(); // Re-initialize whenever parameters might have changed

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0,0,0);
                for (int cnt = 0; cnt < samples_per_pixel; cnt++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world, max_depth);
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
    
    vec3  u,v,w; // Camera basis vectors (w is opposite to camara position)
    
    vec3   defocus_disk_u;       // Defocus disk horizontal radius
    vec3   defocus_disk_v;       // Defocus disk vertical radius

    void initialize() {
        // Calculate image height (must be at least 1)
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        pix_sample_rate = 1.0 / samples_per_pixel;
        center = lookfrom;
        // auto focal_length = (lookat-lookfrom).length();
        // Calculate viewport dimensions
        auto theta = degrees_to_radians(v_fov);
        auto h = tan(theta/2);
        auto viewport_height = 2*h*focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
        

        w = -unit_vector(lookat - lookfrom);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        
        // Calculate basis vectors for viewport orientation
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v; // 注意viewport坐标系是怎么建立的

        // Calculate pixel deltas
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate upper-left viewport pixel location
        auto viewport_upper_left = center 
                                 - focus_dist*w
                                 - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const{
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // sampled point around the pixel location i, j.
        auto offset = sample_square();
        auto pixel_center = pixel00_loc + (i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v;

        auto ray_ori = (defocus_angle<=0) ? center : center + defocus_disk_sample();
        auto ray_d = pixel_center - ray_ori;

        return ray(ray_ori, ray_d);
    }

    inline vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(generate_random_double() - 0.5, generate_random_double() - 0.5, 0); // 在这个方块里随机取一个点
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, const hittable& world, int depth) const {
        // 主要渲染算法
        if (depth <= 0) {
            return color(0,0,0); // 如果递归结束，说明光线碰撞次数过多，这条光路不再计算（人工设定的，避免自相交）
        }

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) {
            // 现在这条光线是否会碰撞到世界下的物体，碰到，则rec记录
            ray scattered;
            color attenuation;
            if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                // 根据材质，进行光线的下一次传播，每次都乘以材质的衰减系数
                return attenuation * ray_color(scattered, world, depth-1);
            }
            return color(0,0,0); // 如果不会scatter，说明光线被吸收，不再计算

            // return 0.5 * ray_color(ray(rec.p, dir), world, depth-1); // 0.5是衰减系数
        }
        // 如果不再有碰撞，这条光路会射到环境中，由于本代码里，环境是光源，所以这条光路能表现出颜色，颜色为天空颜色
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};

#endif