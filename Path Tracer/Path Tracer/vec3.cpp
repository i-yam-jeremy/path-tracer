//
//  vec3.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "vec3.hpp"

vec3::vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3::vec3(float a) {
    this->x = a;
    this->y = a;
    this->z = a;
}

vec3 operator+(const vec3 &a, const vec3 &b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 operator-(const vec3 &a, const vec3 &b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 operator*(const vec3 &a, const vec3 &b) {
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}
