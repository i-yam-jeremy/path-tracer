//
//  Object.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Object.hpp"

Object::Object(std::string filename, Material material) {
    this->obj = new OBJ(filename);
    this->material = material;
}

Object::~Object() {
    delete this->obj;
}

std::vector<float> Object::getVertexBuffer() {
    return this->obj->getVertexBuffer();
}

Material Object::getMaterial() {
    return material;
}
