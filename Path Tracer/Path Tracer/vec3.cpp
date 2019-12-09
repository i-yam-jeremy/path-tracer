//
//  vec3.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "vec3.hpp"

#include <math.h>

vec3::vec3() {
    this->x = this->y = this->z = 0.0;
}

vec3::vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3::vec3(float a) {
    this->x = this->y = this->z = a;
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

float dot(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

float length(vec3 v) {
    return sqrt(dot(v,v));
}
         
vec3 normalize(vec3 v) {
    float len = length(v);
    return vec3(v.x/len, v.y/len, v.z/len);
}
