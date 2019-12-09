//
//  Ray.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include "vec3.hpp"

class Ray {

public:
    vec3 origin, dir;
    Ray(vec3 origin, vec3 dir);
    
};


#endif /* Ray_hpp */
