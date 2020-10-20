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
  std::cout << "\tPathTracer --listDevices" << std::endl;
  std::cout << "\tPathTracer width height samplesPerPixel clPlatformIndex clDeviceIndex imageOutputFrequency sceneConfigFilename outputImageName" << std::endl;
}

int main(int argc, const char * argv[]) {
    if (argc != 2 && argc != 9) {
      usage();
      exit(1);
    }
    if (std::string(argv[1]) == "--listDevices") {
        PathTracer::listOpenCLDevices();
        return 0;
    }

    if (argc != 9) {
      usage();
      exit(1);
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int samplesPerPixel = atoi(argv[3]);
    int clPlatformIndex = atoi(argv[4]);
    int clDeviceIndex = atoi(argv[5]);
    int imageOutputFrequency = atoi(argv[6]);
    std::string sceneConfigFilename = argv[7];
    std::string outputImageName = argv[8];

    std::chrono::milliseconds startTimeMs = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

    Scene scene(sceneConfigFilename);

    PathTracer pathTracer(&scene, width, height, samplesPerPixel, clPlatformIndex, clDeviceIndex, imageOutputFrequency);
    pathTracer.render(outputImageName);

    std::chrono::milliseconds endTimeMs = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

    std::cout << "Rendered in " << (endTimeMs - startTimeMs).count() << "ms" << std::endl;

    return 0;
}
