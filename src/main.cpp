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

void usage() {
  std::cout << "Usage: " << std::endl;
  std::cout << "  pt --listDevices" << std::endl;
  std::cout << "  pt sceneConfigFilename outputImageName" << std::endl;
}

int main(int argc, const char * argv[]) {
    /*if (argc != 2 && argc != 3) {
      usage();
      exit(1);
    }
    if (std::string(argv[1]) == "--listDevices") {
        PathTracer::listOpenCLDevices();
        return 0;
    }

    if (argc != 3) {
      usage();
      exit(1);
    }*/

    std::string sceneConfigFilename = "C:/Users/Jeremy Berchtold/Documents/GitHub/path-tracer/scenes/scene2.config";// argv[1];
    std::string outputImageName = "C:/Users/Jeremy Berchtold/path-trace.ppm";

    std::chrono::milliseconds startTimeMs = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

    Scene scene(sceneConfigFilename);

    PathTracer pathTracer(&scene);
    pathTracer.render(outputImageName);

    std::chrono::milliseconds endTimeMs = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

    std::cout << "Rendered in " << (endTimeMs - startTimeMs).count() << "ms" << std::endl;

    return 0;
}
