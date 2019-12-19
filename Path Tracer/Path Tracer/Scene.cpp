//
//  Scene.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Scene.hpp"

Scene::Scene() {
    this->objects.push_back(new Object("/Users/i-yam-jeremy/Desktop/test.obj", Material(vec3(1,1,1), vec3(), false, false)));
}

Scene::~Scene() {
    for (auto obj : this->objects) {
        delete obj;
    }
}
