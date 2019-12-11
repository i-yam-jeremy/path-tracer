//
//  Object.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Object.hpp"

Object::Object(std::string filename, Material mat) {
    // TODO load mesh
    this->mat = mat;
}

void Object::getReflectedRay(Ray ray, Scene::Intersection in, Ray &outRay, vec3 &outColorScale, bool &outAbsorbed) {
    this->mat.getReflectedRay(ray, in, outRay, outColorScale, outAbsorbed);
}
