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

#include "Material.hpp"
#include "Scene.hpp"

class Object {
    
public:
    Object(std::string filename, Material mat);
    void getReflectedRay(Ray ray, Scene::Intersection in, Ray &outRay, vec3 &outColorScale, bool &outAbsorbed);
  
private:
    Material mat;
    
};

#endif /* Object_hpp */
