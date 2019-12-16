//
//  PathTracer.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "PathTracer.hpp"

#include <cmath>
#include <iostream>
#include <thread>

#include "Image.hpp"
#include "Object.hpp"

PathTracer::PathTracer() {

}

void PathTracer::render(std::string filename, int width, int height) {
    Image image(width, height);
    const int threadPoolSize = 32;
    int threadPoolIndex = 0;
    std::thread *threads = new std::thread[threadPoolSize];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            vec2 uv = vec2(2.0*float(x - width/2) / float(height), 2.0*float(y - height/2) / float(height));
            PathTracer *that = this;
            threads[threadPoolIndex] = std::thread([&image, x, y, uv, &height, &width, &that] {
                image.setColor(x, y, that->renderPixel(uv, height));
                if (x == 0) {
                    std::cout << (100.0 * float((y+1)*width) / float(width*height)) << "%" << std::endl;
                }
            });
            threadPoolIndex++;
            if (threadPoolIndex == threadPoolSize) {
                threadPoolIndex = 0;
                for (int i = 0; i < threadPoolSize; i++) {
                    threads[i].join();
                }
            }
        }
    }
    
    for (int i = 0; i < threadPoolIndex; i++) {
        threads[i].join();
    }

    //delete[] threads;
    
    image.write(filename);
}

vec3 PathTracer::renderPixel(vec2 uv, int height) {
    vec3 camera = vec3(0, 0, -2); // TODO use FOV
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(-1.0/(height/2), 1.0/(height/2));
    
    vec3 color = vec3(0);
    int sampleCount = 10;
    for (int i = 0; i < sampleCount; i++) {
        vec2 offset = vec2(dist(e2), dist(e2));
        vec3 ray = normalize(vec3(uv.x+offset.x, uv.y+offset.y, 0) - camera);
        color = color + renderPath(Ray(camera, ray), 0);
    }
    color = color * vec3(1.0 / sampleCount);
    return color;
}

vec3 PathTracer::renderPath(Ray ray, int bounceCount) {
    Scene::Intersection in = this->scene.findIntersection(ray);
    if (in.intersects) {
        Ray outRay = Ray(vec3(), vec3());
        vec3 colorScale;
        bool absorbed;
        in.object->getReflectedRay(ray, in.pos, in.normal, outRay, colorScale, absorbed);
        if (absorbed) {
            return colorScale;
        }
        else {
            return colorScale*renderPath(outRay, bounceCount+1);
        }
    }
    return vec3(0);
}
