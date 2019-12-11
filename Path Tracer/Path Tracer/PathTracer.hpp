//
//  PathTracer.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef PathTracer_hpp
#define PathTracer_hpp

#include <string>
#include <random>

#include "vec2.hpp"
#include "vec3.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

class PathTracer {

public:
    PathTracer();
    void render(std::string filename, int width, int height);
    
private:
    Scene scene;
    std::random_device rd;
    std::mt19937 e2;
    vec3 renderPixel(vec2 uv, int height);
    vec3 renderPath(Ray ray, int bounceCount);
    
};

#endif /* PathTracer_hpp */
