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
#include <iostream>
#include <thread>

PathTracer::PathTracer() {
    this->e2 = std::mt19937(rd());
}

void PathTracer::render(std::string filename, int width, int height) {
    Image image(width, height);
    std::thread *threads = new std::thread[width*height];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            vec2 uv = vec2(2.0*float(x - width/2) / float(height), 2.0*float(y - height/2) / float(height));
            PathTracer *that = this;
            threads[y*width + x] = std::thread([&image, x, y, uv, &height, &that] {
                image.setColor(x, y, that->renderPixel(uv, height));
            });
        }
        std::cout << (100.0 * float((y+1)*width) / float(width*height)) << "%" << std::endl;
    }
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            threads[y*width + x].join();
        }
    }
    delete[] threads;
    
    image.write(filename);
}

vec3 PathTracer::renderPixel(vec2 uv, int height) {
    vec3 camera = vec3(0, 0, -2); // TODO use FOV
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(-1.0/(height/2), 1.0/(height/2));
    
    vec3 color = vec3(0);
    int sampleCount = 1000;
    for (int i = 0; i < sampleCount; i++) {
        vec2 offset = vec2(dist(e2), dist(e2));
        vec3 ray = normalize(vec3(uv.x+offset.x, uv.y+offset.y, 0) - camera);
        color = color + renderPath(Ray(camera, ray), 0);
    }
    color = color * vec3(1.0 / sampleCount);
    return color;
}

vec3 PathTracer::renderPath(Ray ray, int bounceCount) {
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    Scene::Intersection in = this->scene.findIntersection(ray);
    if (in.intersects) {
        if (in.objectId == 2) {
            return in.objectColor*vec3(10); // Light
        }
        else if (dist(e2) < 0.25) {
            return vec3(0);
        }
        else {
            vec3 N = in.normal;
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
            return 0.8*color*in.objectColor;
        }
    }
    return vec3(0);
}
