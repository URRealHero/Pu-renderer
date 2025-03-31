#include "headers.h"
#include "camera.h"
#include "hittable/material.h"
#include "hittable/hittable.h"
#include "hittable/hittable_list.h"
#include "hittable/sphere.h"

#include <cmath>
#include <iostream>

int main() {

    // 3D 建模部分

    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5)); // 大地的材质(漫反射材质, 灰色)
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material)); // 大地

    for (int a = -4; a < 4; a++) { 
        for (int b = -4; b < 4; b++) { // 生成随机球体
            auto choose_mat = generate_random_double();
            point3 center(a + 0.9*generate_random_double(), 0.2, b + 0.9*generate_random_double()); // 随机球心

            if ((center - point3(4, 0.2, 0)).length() > 0.9) { // 离中心球体足够远才生成
                shared_ptr<material> sphere_material; // 球体材质

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = generate_random_double(0, 0.5);
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

    // 下面是光线追踪部分，先定义相机位置与参数

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 50;
    cam.max_depth         = 10;

    cam.v_fov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    // 渲染(使用光追)
    cam.render(world);
}