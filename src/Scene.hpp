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
#include <memory>

#include "Object.hpp"

/*
  A 3D scene containing objects and path tracer settings.
*/
class Scene {

public:
    /*
      Loads a scene from the given config file.
      @param configFilename - The path to the scene config file.
    */
    Scene(std::string configFilename);
    /*
      Returns the objects in this scene.
      @return The objects in this scene.
    */
    std::vector<std::shared_ptr<Object>> getObjects();
private:
    /*
      The objects in this scene.
    */
    std::vector<std::shared_ptr<Object>> objects;
};

#endif /* Scene_hpp */
