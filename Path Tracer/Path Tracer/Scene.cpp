//
//  Scene.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Scene.hpp"

#include <cmath>

#include <stdio.h>

Scene::Scene() {
  
}

Scene::Intersection raySphereIntersection(vec3 center, float radius, Ray ray, int objectId, vec3 objectColor) {
    float a = dot(ray.dir, ray.dir);
    float b = 2*dot(ray.dir, ray.origin) - 2*dot(ray.dir, center);
    float c = -radius*radius + dot(ray.origin, ray.origin) - 2*dot(ray.origin, center) + dot(center, center);
    
    float disc = b*b - 4*a*c;
    
    if (disc < 0) {
        return Scene::Intersection(false, vec3(), vec3(), -1, vec3());
    }

    float t = (-b - std::sqrt(disc)) / (2*a);
    if (t < 0) {
        t = (-b + std::sqrt(disc)) / (2*a);
        if (t < 0) {
            return Scene::Intersection(false, vec3(), vec3(), -1, vec3());
        }
    }
    
    vec3 pos = ray.origin + t*ray.dir;
    vec3 normal = normalize(pos - center);
    return Scene::Intersection(true, pos, normal, objectId, objectColor);
}

Scene::Intersection rayPlaneIntersection(vec3 normal, vec3 p, Ray ray, int objectId, vec3 objectColor) {
    float d = -dot(p, normal);
    float denominator = dot(ray.dir, normal);
    if (std::abs(denominator) > 0.00001) {
        float t = -(dot(ray.origin, normal) + d) / denominator;
        if (t < 0) return Scene::Intersection(false, vec3(), vec3(), -1, vec3());
        return Scene::Intersection(true, ray.origin + t*ray.dir, normal, objectId, objectColor);
    }
    return Scene::Intersection(false, vec3(), vec3(), -1, vec3());
}

Scene::Intersection Scene::findIntersection(Ray ray) {
    Scene::Intersection in = raySphereIntersection(vec3(0,0,0), 0.5, ray, 1, vec3(1,1,1));
    if (in.intersects) return in;
    
    in = raySphereIntersection(vec3(-1,-1,-1), 0.5, ray, 2, vec3(1,1,1));
    if (in.intersects) return in;
    
    in = rayPlaneIntersection(vec3(0,-1,0), vec3(0,1,0), ray, 3, vec3(0,1,0));
    if (in.intersects) return in;
    
    in = rayPlaneIntersection(vec3(0,0,-1), vec3(0,1,0), ray, 4, vec3(1,1,1));
    if (in.intersects) return in;
    
    return in;
}
