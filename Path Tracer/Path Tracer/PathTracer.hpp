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
    PathTracer(Scene scene);
    void render(std::string filename, int width, int height, int samplesPerPixel);
private:
    Scene scene;
    
};

#endif /* PathTracer_hpp */
