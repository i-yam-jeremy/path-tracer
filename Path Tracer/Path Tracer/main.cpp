//
//  main.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include <iostream>
#include <chrono>

#include "PathTracer.hpp"

#define RENDER_WIDTH (1920)
#define RENDER_HEIGHT (1080)

int main(int argc, const char * argv[]) {
    std::chrono::milliseconds startTimeMs = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    
    Scene scene;
    scene.addObject(new Object("/Users/i-yam-jeremy/Desktop/test.obj", Material(vec3(1,1,1), vec3(), false, false), vec3(0,0,0)));
    scene.addObject(new Object("/Users/i-yam-jeremy/Desktop/test2.obj", Material(vec3(1,1,1), vec3(), false, false), vec3(0,0,0)));
    scene.addObject(new Object("/Users/i-yam-jeremy/Desktop/test3.obj", Material(vec3(1,1,1), vec3(), false, false), vec3(0,0,0)));
    scene.addObject(new Object("/Users/i-yam-jeremy/Desktop/test4.obj", Material(vec3(1,1,1), vec3(), false, false), vec3(0,0,0)));
    scene.addObject(new Object("/Users/i-yam-jeremy/Desktop/test5.obj", Material(vec3(1,1,1), vec3(), false, false), vec3(0,0,0)));
    scene.addObject(new Object("/Users/i-yam-jeremy/Desktop/test6.obj", Material(vec3(1,1,1), vec3(), false, false), vec3(0,0,0)));
    
    PathTracer pathTracer(scene);
    pathTracer.render("/Users/i-yam-jeremy/Desktop/out.ppm", RENDER_WIDTH, RENDER_HEIGHT, 3000);
    
    std::chrono::milliseconds endTimeMs = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    
    std::cout << "Rendered in " << (endTimeMs - startTimeMs).count() << "ms" << std::endl;
    
    return 0;
}
