//
//  PathTracer.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "PathTracer.hpp"

#include "Image.hpp"

#include <stdio.h>

PathTracer::PathTracer() {
    // TODO
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
    return vec3(uv.x,uv.y,0);
}
