//
//  Material.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Material.hpp"

Material::Material() {
    this->baseColor = vec3(1);
    this->emissionColor = vec3(0);
    this->emissive = false;
    this->metal = false;
}

Material::Material(vec3 baseColor, vec3 emissionColor, bool emissive, bool metal) {
    this->baseColor = baseColor;
    this->emissionColor = emissionColor;
    this->emissive = emissive;
    this->metal = metal;
}

void Material::getReflectedRay(Ray ray, vec3 intersectionPos, vec3 normal, Ray &outRay, vec3 &outColorScale, bool &outAbsorbed, std::mt19937 e2) {
    if (metal) {
        
    }
    else if (emissive) {
        outColorScale = emissionColor*vec3(17);
        outAbsorbed = true;
    }
    else {
        std::uniform_real_distribution<float> dist(0.0, 1.0);
        if (dist(e2) < 0.25) {
            outColorScale = vec3(0);
            outAbsorbed = true;
            return;
        }
        else {
            vec3 N = normal;
            std::uniform_real_distribution<float> dist(0, 2.0*M_PI);
            float theta = dist(e2);
            float phi = dist(e2);
            vec3 s = vec3( // random point on sphere
                           1.0,
                           theta,
                           phi
                           );
            vec3 pathDir = vec3(
                                s.x*sin(s.z)*cos(s.y),
                                s.x*sin(s.z)*sin(s.y),
                                s.x*cos(s.z)
                                );
            pathDir = pathDir + N;
            pathDir = normalize(pathDir);
            float lambertReflectanceFactor = dot(N, pathDir);
            outRay = Ray(intersectionPos + 0.000001*N, pathDir);
            outColorScale = baseColor*vec3(lambertReflectanceFactor);
            outAbsorbed = false;
            return;
            /*outAbsorbed = false;
            outRay = Ray(intersectionPos + 0.0001*normal, ray.dir - vec3(2.0*dot(ray.dir, normal))*normal);
            outColorScale = 0.8*this->baseColor;*/
        }
        /*vec3 N = in.normal;
        std::uniform_real_distribution<float> dist(0, 2.0*M_PI);
        vec3 color = vec3(0);
        float theta = dist(e2);
        float phi = dist(e2);
        vec3 s = vec3( // random point on sphere
                       1.0,
                       theta,
                       phi
                       );
        vec3 pathDir = vec3(
                            s.x*sin(s.z)*cos(s.y),
                            s.x*sin(s.z)*sin(s.y),
                            s.x*cos(s.z)
                            );
        pathDir = pathDir + N;
        pathDir = normalize(pathDir);
        float lambertReflectanceFactor = dot(N, pathDir);
        color = color + vec3(lambertReflectanceFactor)*renderPath(Ray(in.pos + 0.000001*N, pathDir), bounceCount+1);
        return 0.8*color*in.objectColor;*/
    }
}