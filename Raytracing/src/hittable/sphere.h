// 实现球体继承抽象类hittable

#ifndef SPHERE_H
#define SPHERE_H

#include "../headers.h"
#include "hittable.h"
#include <future>
#include <memory>

class sphere : public hittable {
  public:
    sphere(const point3& center, double radius, std::shared_ptr<material> mat) : center(center), radius(std::fmax(0,radius)) , mat_ptr(mat) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a; // 小根
        if (root <= ray_t.min || ray_t.max <= root) {
            root = (h + sqrtd) / a; // 大根
            if (root <= ray_t.min || ray_t.max <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_norm(r, outward_normal);
        rec.mat_ptr = mat_ptr;

        return true;
    }

  private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat_ptr;
};

#endif