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
    ~Scene();
    /*
      Returns the objects in this scene.
      @return The objects in this scene.
    */
    std::vector<Object*> getObjects();
    /*
      Returns the width of the output image that will be produced by the path
      tracer.
      @return The width of the output image.
    */
    int getRenderWidth();
    /*
      Returns the height of the output image that will be produced by the path
      tracer.
      @return The height of the output image.
    */
    int getRenderHeight();
    /*
      Returns the samples per pixel to use to render the output image.
      @return The samples per pixel.
    */
    int getSamplesPerPixel();
    /*
      Returns the index of the OpenCL platform that will be used to render this
      scene.
      @return The index of the OpenCL platform.
    */
    int getCLPlatformIndex();
    /*
      Returns the index of the OpenCL device that will be used to render this
      scene.
      @return The index of the OpenCL device.
    */
    int getCLDeviceIndex();

private:
    /*
      The objects in this scene.
    */
    std::vector<Object*> objects;
    /*
      The width of the output image that will be produced by the path tracer.
    */
    int width = 960;
    /*
      The height of the output image that will be produced by the path tracer.
    */
    int height = 540;
    /*
      The samples per pixel to use to render the output image.
    */
    int samplesPerPixel = 100;
    /*
      The index of the OpenCL platform that will be used to render this scene.
    */
    int clPlatformIndex = 0;
    /*
      The index of the OpenCL device that will be used to render this scene.
    */
    int clDeviceIndex = 0;

    /*
      Adds the given object to this scene.
      @param obj - The object to be added.
    */
    void addObject(Object* obj);
};

#endif /* Scene_hpp */
