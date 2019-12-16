//
//  Object.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <string>
#include <random>

#include "Material.hpp"

class Object {
    
public:
    Object(std::string filename, Material mat);
    bool intersects(Ray ray, vec3 &outPos, vec3 &outNormal);
    void getReflectedRay(Ray ray, vec3 intersectionPos, vec3 normal, Ray &outRay, vec3 &outColorScale, bool &outAbsorbed, std::mt19937 e2);
  
private:
    void *loader;
    Material mat;
    
};

#endif /* Object_hpp */
