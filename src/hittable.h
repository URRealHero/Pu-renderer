// 这是一个抽象类，表示一个可以被光线击中的物体
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record{
    public:
        point3 p;
        vec3 normal;
        double t;
};

class hittable{
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;

};


#endif