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

#include "Object.hpp"

class Scene {
    
public:
    Scene();
    ~Scene();
    void addObject(Object* obj);
    std::vector<Object*> getObjects();
    
private:
    std::vector<Object*> objects;
};

#endif /* Scene_hpp */
