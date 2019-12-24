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
#include "Scene.hpp"
#include "Material.hpp"

#define RENDER_WIDTH (1920/4)
#define RENDER_HEIGHT (1080/4)

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << std::endl;
        std::cout << "  pt --listDevices" << std::endl;
        std::cout << "  pt sceneConfigFilename" << std::endl;
        exit(1);
    }
    if (std::string(argv[1]) == "--listDevices") {
        PathTracer::listOpenCLDevices();
        return 0;
    }

    std::string sceneConfigFilename = argv[1];

    std::chrono::milliseconds startTimeMs = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

    Scene *scene = new Scene(sceneConfigFilename);

    PathTracer pathTracer(scene);
    pathTracer.render("/Users/i-yam-jeremy/Desktop/out.ppm");

    delete scene;

    std::chrono::milliseconds endTimeMs = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

    std::cout << "Rendered in " << (endTimeMs - startTimeMs).count() << "ms" << std::endl;

    return 0;
}