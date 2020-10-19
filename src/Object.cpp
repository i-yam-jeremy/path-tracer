//
//  Object.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Object.hpp"

Object::Object(std::string filename, Material material) {
    this->obj = std::make_unique<OBJ>(filename);
    this->material = material;
}

std::vector<float>& Object::getVertexBuffer() {
    return obj->getVertexBuffer();
}

std::vector<float>& Object::getTexCoordBuffer() {
    return obj->getTexCoordBuffer();
}

std::vector<float>& Object::getNormalBuffer() {
    return obj->getNormalBuffer();
}

Material Object::getMaterial() {
    return material;
}
