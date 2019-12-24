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

/*
  A simple PBR material.
  It is packed so it can be directly sent to an OpenCL buffer and read from the
   device.
*/
struct __attribute__ ((packed)) Material {
  /*
    The probability the material will emit light.
  */
  cl_float emissiveness;
  /*
    The color of the emitted light.
  */
  cl_float3 emissionColor;
  /*
    The probability the material will reflect light like a metal.
    It is best to only use 0.0 or 1.0 for non-metals and metals, respectively.
  */
  cl_float metalness;
  /*
    The base color of the material.
  */
  cl_float3 baseColor;
  /*
    Creates a material with the given parameters.
    @param emissiveness - The probability the material will emit light.
    @param emissionColor - The color of the emitted light.
    @param metalness - The probability the material will reflect light like a
      metal. It is best to only use 0.0 or 1.0 for non-metals and metals,
      respectively.
    @param baseColor - The base color of the material.
  */
  Material(cl_float emissiveness, cl_float3 emissionColor, cl_float metalness, cl_float3 baseColor);
  /*
    Creates a default non-metallic, non-emitting Lambert material.
  */
  Material();
};

/*
  Creates a cl_float3 from the given x, y, z components.
  @param x - The x-component.
  @param y - The y-component.
  @param z - The z-component.
*/
cl_float3 make_cl_float3(float x, float y, float z);

#endif /* Material_hpp */
