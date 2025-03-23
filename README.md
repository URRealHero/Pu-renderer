# Pu-renderer
Implementation of a personalised renderer, start from Ray-Tracing weekend series
First six chapters done, finishing the simple rasterizer after MVP projection. Following MVP projection will be implemented next.

## 项目结构
src/
├── main.cpp
├── utils.h # 公用工具函数和基础依赖
├── base_headers/ # 核心基础头文件
│ ├── vec3.h # 三维向量类实现
│ ├── color.h # 颜色处理类
│ ├── ray.h # 光线类定义
│ └── interval.h # 区间定义
└── hittable/ # 可碰撞对象相关
├── hittable.h # 碰撞抽象定义
├── hittable_list.h # 碰撞对象集合
└── sphere.h # 球体碰撞实现