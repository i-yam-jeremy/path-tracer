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
      Creates a PathTracer instance for the given scene and parameters.
      @param scene - The scene to render.
      @param width - The width of the output image.
      @param height - The height of the output image.
      @param samplesPerPixel - The number of light rays to sample per pixel.
      @param clPlatformIndex - The platform index for OpenCL (see PathTracer --listDevices)
      @param clDeviceIndex - The device index for OpenCL (see PathTracer --listDevices)
      @param imageOutputFrequency - How often (in units of samples) to output a partial rendered image.
    */
    PathTracer(Scene *scene, int width, int height, int samplesPerPixel, int clPlatformIndex, int clDeviceIndex, int imageOutputFrequency);
    /*
      Renders the scene and outputs the render to the given filename in PPM
      format.
      @param outputImageFilename - The path to the output image.
    */
    void render(std::string outputImageFilename);
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
     Controls how often to output a partial render. Measured in samples. For example,
        a value of 50 would output a partial render every 50 samples, so 50, 100, 150, etc.
        This is helpful for testing renders and exiting early if the output has converged enough,
        but it can affect performance significantly if the value is too low.
    */
    int imageOutputFrequency = 50;

    /*
        Gets the OpenCL device specified by the given platform and device indices.
    */
    cl::Device getDevice(int platformIndex, int deviceIndex);

    /*
        Container for all OpenCL buffers.
    */
    struct CLBufferCollection {
        cl::Buffer vertices, texCoords, normals, materials, outPixels, randStates;
    };

    /*
        Creates the necessary OpenCL buffers for the render kernel
    */
    CLBufferCollection createCLBuffers(cl::Context context, cl::CommandQueue queue, const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normals, std::vector<Material> materials);
    /*
        Builds an OpenCL program from the given context on the given device.
    */
    cl::Program buildProgram(cl::Context context, cl::Device device);
    /*
        Converts all objects' data into combined vectors containing vertices, normals, etc.
    */
    void processObjects(std::vector<std::shared_ptr<Object>> objects, std::vector<float>* vertices, std::vector<float>* texCoords, std::vector<float>* normals, std::vector<Material>* materials, int* triCount);
    /*
        Creates the executable render kernel from the given program, buffer collection, and total triangle count of the scene.
    */
    cl::Kernel createRenderKernel(cl::Program program, CLBufferCollection bc, int triCount);
    /*
        Writes a pixel buffer to the output image file.
    */
    void writeImage(std::string filename, int width, int height, float* pixels);
    /*
        Runs the OpenCL kernel with the given parameters.
    */
    void runKernel(cl::CommandQueue queue, cl::Kernel render, std::string outputImageFilename, CLBufferCollection bc);
};

#endif /* PathTracer_hpp */
