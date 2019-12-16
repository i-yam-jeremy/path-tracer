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

#include "Material.hpp"

Scene::Scene() {
    this->objects.push_back(Object("/Users/i-yam-jeremy/Desktop/test.obj", Material(vec3(1,1,1), vec3(1,1,1), true, false)));
    this->objects.push_back(Object("/Users/i-yam-jeremy/Desktop/test.obj", Material(vec3(1,1,1), vec3(), false, false)));
}

Scene::Intersection raySphereIntersection(vec3 center, float radius, Ray ray, int objectId, vec3 objectColor) {
    float a = dot(ray.dir, ray.dir);
    float b = 2*dot(ray.dir, ray.origin) - 2*dot(ray.dir, center);
    float c = -radius*radius + dot(ray.origin, ray.origin) - 2*dot(ray.origin, center) + dot(center, center);
    
    float disc = b*b - 4*a*c;
    
    if (disc < 0) {
        return Scene::Intersection(false, vec3(), vec3(), NULL);
    }

    float t = (-b - std::sqrt(disc)) / (2*a);
    if (t < 0) {
        t = (-b + std::sqrt(disc)) / (2*a);
        if (t < 0) {
            return Scene::Intersection(false, vec3(), vec3(), NULL);
        }
    }
    
    vec3 pos = ray.origin + t*ray.dir;
    vec3 normal = normalize(pos - center);
    return Scene::Intersection(true, pos, normal, NULL);
}

void checkClosestIntersection(Scene::Intersection &in, float &closestPointDistance, Scene::Intersection &closestIntersection, Ray ray) {
    if (in.intersects) {
        float d = length(in.pos - ray.origin);
        if (d < closestPointDistance) {
            closestPointDistance = d;
            closestIntersection = in;
        }
    }
}

Scene::Intersection Scene::findIntersection(Ray ray) {
    /*Scene::Intersection in = raySphereIntersection(vec3(0,0,0), 0.5, ray, 1, vec3(1,1,1));
    if (in.intersects) return in;*/
    
    float closestPointDistance = 1000000000.0;
    Scene::Intersection closestIntersection = Scene::Intersection(false, vec3(), vec3(), NULL);
    
    Scene::Intersection in = raySphereIntersection(vec3(-1,-1,-1), 0.5, ray, 2, vec3(1,1,1));
    if (in.intersects) {
        in.object = &(this->objects[0]);
    }
    checkClosestIntersection(in, closestPointDistance, closestIntersection, ray);
    
    /*in = rayPlaneIntersection(vec3(0,-1,0), vec3(0,1,0), ray, 3, vec3(0,1,0));
    checkClosestIntersection(in, closestPointDistance, closestIntersection, ray);
    
    in = rayPlaneIntersection(vec3(0,0,-1), vec3(0,1,0), ray, 4, vec3(1,1,1));
    checkClosestIntersection(in, closestPointDistance, closestIntersection, ray);*/
    
    for (int i = 1; i < this->objects.size(); i++) {
        vec3 pos, normal;
        bool intersects = this->objects[i].intersects(ray, pos, normal);
        if (intersects) {
            Scene::Intersection in = Scene::Intersection(true, pos, normal, &(this->objects[i]));
            checkClosestIntersection(in, closestPointDistance, closestIntersection, ray);
        }
    }
    
    return closestIntersection;
}
