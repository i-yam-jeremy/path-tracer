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
#include <filesystem>

#include "StringUtil.hpp"

Scene::Scene(std::string configFilename) {
    std::string baseDir = std::filesystem::path(configFilename).parent_path().string();
    std::ifstream file(configFilename);
    if (!file.is_open()) {
      std::cout << "Could not open scene config file: " << configFilename << std::endl;
      exit(1);
    }
    std::string line;

    while (std::getline(file, line)) {
        auto params = stringutil::split(line, ' ');
        if (params.size() == 0) continue;
        if (params[0] == "object") {
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
            objects.push_back(std::make_shared<Object>(baseDir + "/" + objRelativePath,
                                       Material(emissiveness, emissionColor, metalness, baseColor)));
        }
    }
}

std::vector<std::shared_ptr<Object>> Scene::getObjects() {
    return objects;
}