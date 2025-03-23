#include "utils.h"
#include "camera.h"
#include "hittable/hittable.h"
#include "hittable/hittable_list.h"
#include "hittable/sphere.h"

#include <cmath>
#include <iostream>

int main() {
    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100.3));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 400;
    cam.focal_length = 1;

    cam.render(world);
}