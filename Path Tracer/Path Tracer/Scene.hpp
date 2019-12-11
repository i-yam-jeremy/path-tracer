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
        Intersection(bool intersects, vec3 pos, vec3 normal, int objectId, vec3 objectColor) : intersects(intersects), pos(pos), normal(normal), objectId(objectId), objectColor(objectColor) {}
        bool intersects;
        vec3 pos;
        vec3 normal;
        int objectId;
        vec3 objectColor;
    };
    Scene();
    Intersection findIntersection(Ray ray);
    
};

#endif /* Scene_hpp */
