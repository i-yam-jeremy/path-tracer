//
//  Ray.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Ray.hpp"

Ray::Ray(vec3 origin, vec3 dir) {
    this->origin = origin;
    this->dir = dir;
}
