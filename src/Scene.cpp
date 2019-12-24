//
//  Scene.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "Scene.hpp"

#include <fstream>
#include <iostream>

#include "StringUtil.hpp"

Scene::Scene(std::string configFilename) {
    std::string baseDir = ""; // TODO
    std::ifstream file(configFilename);
    std::string line;
    
    while (std::getline(file, line)) {
        auto params = split(line, ' ');
        if (params[0] == "spp") {
            this->samplesPerPixel = atoi(params[1].c_str());
        }
        else if (params[0] == "renderSize") {
            this->width = atoi(params[1].c_str());
            this->height = atoi(params[2].c_str());
        }
        else if (params[0] == "device") {
            this->clPlatformIndex = atoi(params[1].c_str());
            this->clDeviceIndex = atoi(params[2].c_str());
        }
        else if (params[0] == "object") {
            std::string objRelativePath = params[1];
            float emissiveness = atof(params[2].c_str());
            cl_float3 emissionColor = make_cl_float3(
                                                     atof(params[3].c_str()),
                                                     atof(params[4].c_str()),
                                                     atof(params[5].c_str()));
            float metalness = atof(params[6].c_str());
            cl_float3 baseColor = make_cl_float3(
                                                     atof(params[7].c_str()),
                                                     atof(params[8].c_str()),
                                                     atof(params[9].c_str()));
            this->addObject(new Object(baseDir + objRelativePath,
                                       Material(emissiveness, emissionColor, metalness, baseColor)));
        }
    }
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
