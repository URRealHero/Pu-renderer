#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include <cstdlib>

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& albedo, double p) : albedo(albedo), prob(p) {}
        lambertian(const color& a) : albedo(a), prob(1.0) {}
    
        bool scatter([[maybe_unused]] const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
            const override {
            if (generate_random_double() < prob) {
                auto scatter_direction = rec.normal + random_unit_vector();
                scattered = ray(rec.p, scatter_direction);
                attenuation = albedo;
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

#endif