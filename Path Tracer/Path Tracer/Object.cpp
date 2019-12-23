//
//  Object.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Object.hpp"

Object::Object(std::string filename, Material mat, vec3 pos) {
    this->obj = new OBJ(filename, pos);
    // TODO load mesh
    this->mat = mat;
}

Object::~Object() {
    delete this->obj;
}

std::vector<float> Object::getVertexBuffer() {
    return this->obj->getVertexBuffer();
}
