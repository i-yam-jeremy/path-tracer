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
    vec3 renderPixel(vec2 uv);
    vec3 renderPath(Ray ray, int bounceCount);
    
};

#endif /* PathTracer_hpp */
