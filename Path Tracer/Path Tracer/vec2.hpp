//
//  vec2.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef vec2_hpp
#define vec2_hpp

class vec2 {
  
public:
    float x, y;
    vec2(float a);
    vec2(float x, float y);
    friend vec2 operator+(const vec2 &a, const vec2 &b);
    friend vec2 operator-(const vec2 &a, const vec2 &b);
    friend vec2 operator*(const vec2 &a, const vec2 &b);
    
};

#endif /* vec2_hpp */
