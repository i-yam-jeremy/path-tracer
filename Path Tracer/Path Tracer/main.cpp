//
//  main.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include <iostream>

#include "PathTracer.hpp"

#define RENDER_WIDTH (1920/8)
#define RENDER_HEIGHT (1080/8)

int main(int argc, const char * argv[]) {
    
    PathTracer pathTracer;
    pathTracer.render("/Users/i-yam-jeremy/Desktop/out.ppm", RENDER_WIDTH, RENDER_HEIGHT);
    
    std::cout << "Done\n";
    return 0;
}
