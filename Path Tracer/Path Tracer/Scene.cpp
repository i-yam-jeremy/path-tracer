//
//  Scene.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Scene.hpp"

Scene::Scene() {

}

Scene::~Scene() {
    for (auto obj : this->objects) {
        delete obj;
    }
}

void Scene::addObject(Object* obj) {
    this->objects.push_back(obj);
}

std::vector<Object*> Scene::getObjects() {
    return objects;
}
