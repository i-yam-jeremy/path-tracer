//
//  Scene.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <vector>

#include "vec3.hpp"
#include "Ray.hpp"
#include "Object.hpp"

class Scene {
    
public:
    struct Intersection {
        Intersection(bool intersects, vec3 pos, vec3 normal, Object *object) : intersects(intersects), pos(pos), normal(normal), object(object) {}
        bool intersects;
        vec3 pos;
        vec3 normal;
        Object *object;
    };
    Scene();
    Intersection findIntersection(Ray ray);
    
private:
    std::vector<Object> objects;
    
};

#endif /* Scene_hpp */
