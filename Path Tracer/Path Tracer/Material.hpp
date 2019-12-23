//
//  Material.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include "cl.hpp"

struct __attribute__ ((packed)) Material {
    cl_float emissiveness;
    cl_float3 emissionColor;
    cl_float metalness;
    cl_float3 baseColor;
    Material(cl_float emissiveness, cl_float3 emissionColor, cl_float metalness, cl_float3 baseColor);
    Material();
};

cl_float3 make_cl_float3(float x, float y, float z);

#endif /* Material_hpp */
