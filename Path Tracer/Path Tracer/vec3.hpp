//
//  vec3.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef vec3_hpp
#define vec3_hpp

class vec3 {
  
public:
    float x, y, z;
    vec3();
    vec3(float a);
    vec3(float x, float y, float z);
    friend vec3 operator+(const vec3 &a, const vec3 &b);
    friend vec3 operator-(const vec3 &a, const vec3 &b);
    friend vec3 operator*(const vec3 &a, const vec3 &b);
    
};

float dot(vec3 a, vec3 b);
float length(vec3 v);
vec3 normalize(vec3 v);

#endif /* vec3_hpp */
