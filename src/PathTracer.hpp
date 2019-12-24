//
//  PathTracer.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef PathTracer_hpp
#define PathTracer_hpp

#include <string>

#include "Scene.hpp"

class PathTracer {

public:
    PathTracer(Scene *scene);
    void render(std::string filename);
    static void listOpenCLDevices();
private:
    Scene *scene;

};

#endif /* PathTracer_hpp */
