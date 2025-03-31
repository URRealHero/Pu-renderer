#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include <cstdlib>

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0; // 这个函数的返回值表达光线是否继续传播
};

class lambertian : public material {
    public:
        lambertian(const color& albedo, double p) : albedo(albedo), prob(p) {}
        lambertian(const color& a) : albedo(a), prob(1.0) {} // albedo 是反照率，在漫反射中，albedo = reflectance
    
        bool scatter([[maybe_unused]] const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
            const override {
            if (generate_random_double() < prob) {
                auto scatter_direction = rec.normal + random_unit_vector();
                scattered = ray(rec.p, scatter_direction);
                attenuation = albedo; // 反射率（1-吸收率）
                return true;
            }
            return false;
        }
  
    private:
        color albedo;
        double prob;
  };

class metal: public material{
    public:
        metal(const color& albedo, double fuzz,const double p):albedo(albedo), fuzz(fuzz), prob(p) {}
        metal(const color& a, double fuzz):albedo(a), fuzz(fuzz), prob(1.0) {}

        bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override{
            if (generate_random_double() < prob) {
                vec3 scatter_direction = reflect(unit_vector(r_in.direction()), rec.normal) + fuzz*random_unit_vector();
                scattered = ray(rec.p, scatter_direction);
                attenuation = albedo/prob;
                return dot(scattered.direction(), rec.normal) > 0;
            }
            return false;
        }
    
    private:
        color albedo;
        double fuzz;
        double prob;
};



class dielectric : public material{
    public:
        dielectric(double ri): ref_idx(ri) {}
        bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0 / ref_idx) : ref_idx;

            vec3 unit_d = unit_vector(ray_in.direction());
            double cos_theta = fmin(dot(-unit_d, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            if(ri * sin_theta > 1.0 || reflectance(cos_theta, ri) > generate_random_double()){
                // ||后的式子模拟反射概率，一部分光线被反射，一部分被折射，用折射率和随机数对比来模拟（小于random的概率就是R反射率, 反之是折射率）
                vec3 reflected = reflect(unit_d, rec.normal);
                scattered = ray(rec.p, reflected);
                return true;
            }
            else{
                vec3 ref_d = refract(unit_d, rec.normal, ri);
                scattered = ray(rec.p, ref_d);
                return true;
            }
            

        }

    private:
        double ref_idx;

        static double reflectance(double cos, double refraction_index){
            // Schlick Approximation (approximating the reflectance)
            auto r0 = (1-refraction_index) / (1+refraction_index);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1-cos), 5);
        }
};

#endif