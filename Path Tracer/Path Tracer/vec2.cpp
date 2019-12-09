//
//  vec2.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "vec2.hpp"

vec2::vec2(float x, float y) {
    this->x = x;
    this->y = y;
}

vec2::vec2(float a) {
    this->x = a;
    this->y = a;
}

vec2 operator+(const vec2 &a, const vec2 &b) {
    return vec2(a.x + b.x, a.y + b.y);
}

vec2 operator-(const vec2 &a, const vec2 &b) {
    return vec2(a.x - b.x, a.y - b.y);
}

vec2 operator*(const vec2 &a, const vec2 &b) {
    return vec2(a.x * b.x, a.y * b.y);
}
