#pragma once
#include <cstddef> // size_t

template <size_t N, typename T>
struct Vec {
    T data[N];

    // TODO 1: 构造函数（默认构造、参数构造）
    // 示例：Vec(float x, float y) for Vec<2,float>
    
    // TODO 2: 运算符重载（+, -, *标量）
    Vec operator+(const Vec& other) const;
    
    // TODO 3: 点积 dot()
    T dot(const Vec& other) const;
    
    // TODO 4: 向量长度 length()
    T length() const;
    
    // TODO 5: 归一化 normalize()
    Vec& normalize();
};

// TODO 6: 特化 Vec<3,T> 的叉乘 cross()
template <typename T>
Vec<3, T> cross(const Vec<3, T>& a, const Vec<3, T>& b);

// TODO 7: 定义常用别名
using Vec2f = Vec<2, float>;
using Vec3f = Vec<3, float>;