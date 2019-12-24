//
//  Scene.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <vector>
#include <string>

#include "Object.hpp"

class Scene {
    
public:
    Scene(std::string configFilename);
    ~Scene();
    std::vector<Object*> getObjects();
    
private:
    std::vector<Object*> objects;
    int width = 960;
    int height = 540;
    int samplesPerPixel = 100;
    int clPlatformIndex = 0;
    int clDeviceIndex = 0;
    
    void addObject(Object* obj);
};

#endif /* Scene_hpp */
