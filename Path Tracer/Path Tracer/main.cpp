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

#define RENDER_WIDTH (1920/2)
#define RENDER_HEIGHT (1080/2)

int main(int argc, const char * argv[]) {
    std::chrono::milliseconds startTimeMs = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    
    PathTracer pathTracer;
    pathTracer.render("/Users/i-yam-jeremy/Desktop/out.ppm", RENDER_WIDTH, RENDER_HEIGHT);
    
    std::chrono::milliseconds endTimeMs = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    
    std::cout << "Rendered in " << (endTimeMs - startTimeMs).count() << "ms" << std::endl;
    
    std::cout << "Done\n";
    return 0;
}
