# Pu-renderer
Implementation of a personalised renderer, start from Ray-Tracing weekend series. This is only a renderer, which serve as the purpose in the graphics pipeline.
First six chapters done, finishing the simple rasterizer. 

## Project Structure
src/
├── main.cpp
├── utils.h # gathering header
├── camera.h # render helper and camera params
├── base_headers/ # core utilities
│ ├── vec3.h
│ ├── color.h # Color Processing
│ ├── ray.h # color ray
│ └── interval.h # interval definition
└── hittable/ # Hittable object definitions
├── hittable.h # Hittable object abstract header
├── hittable_list.h # list of hittable objects
└── sphere.h # sphere hittable