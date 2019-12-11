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

#include "OBJ_Loader.h"

Scene::Scene() {
    // Initialize Loader
    objl::Loader *Loader = new objl::Loader;

    // Load .obj File
    std::string filename = "/Users/i-yam-jeremy/Desktop/test.obj";
    bool loadout = Loader->LoadFile(filename);

    if (!loadout) {
        std::cout << "Couldn't read OBJ file: " << filename << std::endl;
        exit(1);
    }
    
    this->loader = Loader;
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

float triArea(vec3 a, vec3 b, vec3 c) {
    return 0.5*length(cross(b-a, c-a));
}

Scene::Intersection rayTriangleIntersection(
                           objl::Vertex A, objl::Vertex B, objl::Vertex C,
                           Ray ray,
                           int objectId,
                           vec3 objectColor)
{
    vec3 N = vec3(A.Normal.X, A.Normal.Y, A.Normal.Z); // TODO update to use all vertices for smooth shading
    vec3 p = vec3(A.Position.X, A.Position.Y, A.Position.Z);
    Scene::Intersection in = rayPlaneIntersection(N, p, ray, objectId, objectColor);
    if (!in.intersects) return in;
    
    vec3 a = vec3(A.Position.X, A.Position.Y, A.Position.Z);
    vec3 b = vec3(B.Position.X, B.Position.Y, B.Position.Z);
    vec3 c = vec3(C.Position.X, C.Position.Y, C.Position.Z);
    
    float fullTriArea = triArea(a, b, c);
    float aA = triArea(in.pos, b, c);
    float bA = triArea(in.pos, a, c);
    float cA = triArea(in.pos, a, b);

    if (abs(fullTriArea - (aA+bA+cA)) < 0.0001 && fullTriArea > 0.0001) {
        return in;
    }
    else {
        return Scene::Intersection(false, vec3(), vec3(), -1, vec3());
    }
}

objl::Vertex scaleVertex(float scale, objl::Vertex v) {
    v.Position.X *= scale;
    v.Position.Y *= scale;
    v.Position.Z *= scale;
    
    return v;
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
    Scene::Intersection closestIntersection = Scene::Intersection(false, vec3(), vec3(), -1, vec3());
    
    Scene::Intersection in = raySphereIntersection(vec3(-1,-1,-1), 0.5, ray, 2, vec3(1,1,1));
    checkClosestIntersection(in, closestPointDistance, closestIntersection, ray);
    
    /*in = rayPlaneIntersection(vec3(0,-1,0), vec3(0,1,0), ray, 3, vec3(0,1,0));
    checkClosestIntersection(in, closestPointDistance, closestIntersection, ray);
    
    in = rayPlaneIntersection(vec3(0,0,-1), vec3(0,1,0), ray, 4, vec3(1,1,1));
    checkClosestIntersection(in, closestPointDistance, closestIntersection, ray);*/
    
    objl::Loader loader = *((objl::Loader*)this->loader);
    objl::Mesh mesh = loader.LoadedMeshes[0];
    for (int i = 0; i < mesh.Indices.size(); i+=3) {
        in = rayTriangleIntersection(
                                     scaleVertex(0.5, mesh.Vertices[i+0]),
                                     scaleVertex(0.5, mesh.Vertices[i+1]),
                                     scaleVertex(0.5, mesh.Vertices[i+2]),
                                     ray,
                                     5,
                                     vec3(1,1,1));
        checkClosestIntersection(in, closestPointDistance, closestIntersection, ray);
    }
    
    return closestIntersection;
}
