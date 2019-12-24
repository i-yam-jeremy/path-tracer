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

/*
  Handles rendering a Scene using path tracing.
*/
class PathTracer {

public:
    /*
      Creates a PathTracer instance for the given scene.
    */
    PathTracer(Scene *scene);
    /*
      Renders the scene and outputs the render to the given filename in PPM
      format.
      @param filename - The path to the output image.
    */
    void render(std::string filename);
    /*
      Prints the available OpenCL platforms and devices along with their
      indices. These indices are used to specify a device in a scene config.
    */
    static void listOpenCLDevices();
private:
    /*
      The scene that will be rendered.
    */
    Scene *scene;

};

#endif /* PathTracer_hpp */
