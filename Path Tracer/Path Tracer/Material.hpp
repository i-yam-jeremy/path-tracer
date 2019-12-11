//
//  Material.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include "vec3.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

class Material {

public:
    Material();
    Material(vec3 baseColor, vec3 emissionColor, bool emissive, bool metal);
    void getReflectedRay(Ray ray, Scene::Intersection in, Ray &outRay, vec3 &outColorScale, bool &outAbsorbed);

private:
    vec3 baseColor;
    vec3 emissionColor;
    bool emissive;
    bool metal;
    
};

#endif /* Material_hpp */
