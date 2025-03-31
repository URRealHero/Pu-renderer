# Pu-renderer
Implementation of a personalised renderer, start from Ray-Tracing weekend series. Now only implement ray tracing renderer.
Next, I will realize the Rasterizer, which serve as the purpose in the graphics pipeline.

### Already done
- Mar 31, 2025 finished __Raytracing in one weekend__ coding.

### Todo
- [ ] Review the code, get the raytracing pipeline
- [ ] Rasterizer
- [ ] Ray Tracing series!
- [ ] Triangles, Parallelism and Shadow Rays!


## Ray Tracing Project Structure
```
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
```
## What do renderer do?
- Use viewport to get the 3D scene
- Image samples from viewport plane