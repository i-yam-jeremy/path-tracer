//
//  Image.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Image.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    this->data = std::vector<float>(3*width*height);
}

int Image::getWidth() {
    return width;
}

int Image::getHeight() {
    return height;
}

float clamp(float x, float lo, float hi) {
    return std::max(lo, std::min(x, hi));
}

void Image::setColor(int x, int y, vec3 c) {
    long index = 3*(y*width + x);
    this->data[index + 0] = clamp(c.x, 0.0, 1.0);
    this->data[index + 1] = clamp(c.y, 0.0, 1.0);
    this->data[index + 2] = clamp(c.z, 0.0, 1.0);
    
}

void Image::write(std::string filename) {
    std::fstream f;
    f.open(filename, std::ios::out);

    f << "P3" << std::endl;
    f << width << " " << height << std::endl;
    f << "255" << std::endl;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            long index = 3*(y*width + x);
            int r = 255.0*data[index + 0];
            int g = 255.0*data[index + 1];
            int b = 255.0*data[index + 2];
            f << r << " " << g << " " << b << std::endl;
        }
    }

    f.close();
}
