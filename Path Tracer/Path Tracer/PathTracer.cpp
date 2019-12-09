//
//  PathTracer.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "PathTracer.hpp"

#include "Image.hpp"

#include <cmath>
#include <random>

#include <stdio.h>

PathTracer::PathTracer() {
    
}

void PathTracer::render(std::string filename, int width, int height) {
    Image image(width, height);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            vec2 uv(2.0*float(x - width/2) / float(height), 2.0*float(y - height/2) / float(height));
            image.setColor(x, y, renderPixel(uv));
        }
    }
    
    image.write(filename);
}

vec3 PathTracer::renderPixel(vec2 uv) {
    vec3 camera = vec3(0, 0, -2); // TODO use FOV
    vec3 ray = normalize(vec3(uv.x, uv.y, 0) - camera);
    return renderPath(Ray(camera, ray), 0);
}

vec3 PathTracer::renderPath(Ray ray, int bounceCount) {
    Scene::Intersection in = this->scene.findIntersection(ray);
    if (in.intersects) {
        if (bounceCount == 2) {
            return in.normal;//vec3(1);
        }
        else {
            vec3 N = in.normal;
            vec3 sphereCoordNormal = vec3( // vec3(radius, theta, phi)
                                          length(N),
                                          std::atan2(N.y, N.x),
                                          std::atan2(std::sqrt(N.x*N.x + N.y*N.y), N.z));
            std::random_device rd;
            std::mt19937 e2(rd());
            std::uniform_real_distribution<float> dist(-1, 1);
            vec3 color = vec3(0);
            int sampleCount = 1;
            //printf("(%f, %f, %f) -> (%f, %f, %f)\n", N.x, N.y, N.z, sphereCoordNormal.x, sphereCoordNormal.y, sphereCoordNormal.z);
            for (int i = 0; i < sampleCount; i++) {
                float deltaTheta = 0.0;//(M_PI/2.0) * dist(e2);
                float deltaPhi = 0.0;//(M_PI/2.0) * dist(e2);
                vec3 s = vec3( // sphereCoordPathDir
                               sphereCoordNormal.x,
                               sphereCoordNormal.y + deltaTheta,
                               sphereCoordNormal.z + deltaPhi
                               );
                vec3 pathDir = vec3(
                                    s.x*sin(s.z)*cos(s.y),
                                    s.x*sin(s.z)*sin(s.y),
                                    s.x*cos(s.z)
                                    );
                //printf("(%f, %f, %f) -> (%f, %f, %f)\n", N.x, N.y, N.z, pathDir.x, pathDir.y, pathDir.z);
                //float lambertReflectanceFactor = dot(N, pathDir);
                color = color + /*vec3(lambertReflectanceFactor)**/renderPath(Ray(in.pos + 0.0001*N, pathDir), bounceCount+1);
            }
            color = color * vec3(1.0/sampleCount);
            return color;
        }
    }
    return vec3(0);
}
