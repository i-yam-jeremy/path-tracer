//
//  Material.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Material.hpp"

Material::Material(cl_float emissiveness, cl_float3 emissionColor, cl_float metalness, cl_float3 baseColor) {
    this->emissiveness = emissiveness;
    this->emissionColor = emissionColor;
    this->metalness = metalness;
    this->baseColor = baseColor;
}

Material::Material() {
    this->emissiveness = 0.0;
    this->emissionColor = make_cl_float3(0, 0, 0);
    this->metalness = 0.0;
    this->baseColor = make_cl_float3(1, 1, 1);
}

cl_float3 make_cl_float3(float x, float y, float z) {
    cl_float3 v;
    v.s[0] = x;
    v.s[1] = y;
    v.s[2] = z;
    return v;
}
