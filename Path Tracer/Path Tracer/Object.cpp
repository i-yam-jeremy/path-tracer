//
//  Object.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Object.hpp"

#include <cmath>

#include "OBJ_Loader.h"

Object::Object(std::string filename, Material mat) {
    // Initialize Loader
    objl::Loader *Loader = new objl::Loader;

    // Load .obj File
    bool loadout = Loader->LoadFile(filename);

    if (!loadout) {
        std::cout << "Couldn't read OBJ file: " << filename << std::endl;
        exit(1);
    }
    
    this->loader = Loader;
    // TODO load mesh
    this->mat = mat;
}

bool rayPlaneIntersection(vec3 normal, vec3 p, Ray ray, float &outT) {
    float d = -dot(p, normal);
    float denominator = dot(ray.dir, normal);
    if (std::abs(denominator) > 0.00001) {
        float t = -(dot(ray.origin, normal) + d) / denominator;
        if (t < 0) return false;
        outT = t;
        return true;
    }
    return false;
}

float triArea(vec3 a, vec3 b, vec3 c) {
    return 0.5*length(cross(b-a, c-a));
}

bool rayTriangleIntersection(
                           objl::Vertex A, objl::Vertex B, objl::Vertex C,
                           Ray ray,
                           float &outT,
                           vec3 &outNormal)
{
    vec3 N = vec3(A.Normal.X, A.Normal.Y, A.Normal.Z); // TODO update to use all vertices for smooth shading
    vec3 p = vec3(A.Position.X, A.Position.Y, A.Position.Z);
    float t;
    bool intersects = rayPlaneIntersection(N, p, ray, t);
    if (!intersects) return false;
    vec3 pos = ray.origin + ray.dir*t;
    
    vec3 a = vec3(A.Position.X, A.Position.Y, A.Position.Z);
    vec3 b = vec3(B.Position.X, B.Position.Y, B.Position.Z);
    vec3 c = vec3(C.Position.X, C.Position.Y, C.Position.Z);
    
    float fullTriArea = triArea(a, b, c);
    float aA = triArea(pos, b, c);
    float bA = triArea(pos, a, c);
    float cA = triArea(pos, a, b);

    if (abs(fullTriArea - (aA+bA+cA)) < 0.0001 && fullTriArea > 0.0001) {
        outNormal = N;
        outT = t;
        return true;
    }
    else {
        return false;
    }
}

objl::Vertex scaleVertex(float scale, objl::Vertex v) {
    v.Position.X *= scale;
    v.Position.Y *= scale;
    v.Position.Z *= scale;
    
    return v;
}


bool Object::intersects(Ray ray, vec3 &outPos, vec3 &outNormal) {
    objl::Loader loader = *((objl::Loader*)this->loader);
    objl::Mesh mesh = loader.LoadedMeshes[0];
    float closestT = 10000000000.0;
    int closestTriIndex = -1;
    vec3 closestTriNormal;
    for (int i = 0; i < mesh.Indices.size(); i+=3) {
        float t;
        vec3 normal;
        bool intersects = rayTriangleIntersection(
                                     scaleVertex(0.33, mesh.Vertices[i+0]),
                                     scaleVertex(0.33, mesh.Vertices[i+1]),
                                     scaleVertex(0.33, mesh.Vertices[i+2]),
                                     ray,
                                     t, normal);
        if (intersects && t < closestT) {
            closestT = t;
            closestTriIndex = i;
            closestTriNormal = normal;
        }
    }
    
    if (closestTriIndex != -1) {
        outPos = ray.origin + ray.dir*closestT;
        outNormal = closestTriNormal;
        return true;
    }
    else {
        return false;
    }
}

void Object::getReflectedRay(Ray ray, vec3 intersectionPos, vec3 normal, Ray &outRay, vec3 &outColorScale, bool &outAbsorbed) {
    this->mat.getReflectedRay(ray, intersectionPos, normal, outRay, outColorScale, outAbsorbed);
}
