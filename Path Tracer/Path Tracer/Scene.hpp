//
//  Scene.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include "vec3.hpp"
#include "Ray.hpp"

class Scene {
    
public:
    struct Intersection {
        Intersection(bool intersects, vec3 pos) : intersects(intersects), pos(pos) {}
        bool intersects;
        vec3 pos;
    };
    Scene();
    Intersection findIntersection(Ray ray);
    
};

#endif /* Scene_hpp */
