//
//  Scene.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Scene.hpp"

#include <math.h>

Scene::Scene() {
  
}

Scene::Intersection Scene::findIntersection(Ray ray) {
    vec3 center = vec3(0,0,0);
    float radius = 0.5;
    
    float a = dot(ray.dir, ray.dir);
    float b = 2*dot(ray.dir, ray.origin) - 2*dot(ray.dir, center);
    float c = -radius*radius + dot(ray.origin, ray.origin) - 2*dot(ray.origin, center) + dot(center, center);
    
    float disc = b*b - 4*a*c;
    
    if (disc < 0) {
        return Scene::Intersection(false, vec3());
    }

    float t = (-b - sqrt(disc)) / (2*a);
    if (t < 0) {
        t = (-b + sqrt(disc)) / (2*a);
    }
    
    return Scene::Intersection(true, ray.origin + t*ray.dir);
}
