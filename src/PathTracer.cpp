//
//  PathTracer.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "PathTracer.hpp"

#include <iostream>
#include <memory>
#include "cl.hpp"

#include "Image.hpp"
#include "Object.hpp"
#include "Material.hpp"

const std::string renderKernelCode =
#include "render.cl"
;

PathTracer::PathTracer(Scene *scene) {
    this->scene = scene;
}

void PathTracer::listOpenCLDevices() {
  std::vector<cl::Platform> allPlatforms;
  cl::Platform::get(&allPlatforms);
  if (allPlatforms.size() == 0) {
      std::cout << "No platforms" << std::endl;
      return;
  }
  for (int platformIndex = 0; platformIndex < allPlatforms.size(); platformIndex++) {
    cl::Platform platform = allPlatforms[platformIndex];
    std::cout << "Platform " << platformIndex << ": " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

    std::vector<cl::Device> allDevices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &allDevices);
    if(allDevices.size() == 0){
        std::cout << "\tNo devices." << std::endl;
    }
    for (int deviceIndex = 0; deviceIndex < allDevices.size(); deviceIndex++) {
      cl::Device device = allDevices[deviceIndex];
      std::cout << "\tDevice " << deviceIndex << ": " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    }
  }
}

cl::Device getDevice(int platformIndex, int deviceIndex) {
    std::vector<cl::Platform> allPlatforms;
    cl::Platform::get(&allPlatforms);
    if (allPlatforms.size() <= platformIndex) {
        std::cout << "Platform index " << platformIndex << " out of range. Only " << allPlatforms.size() << " platforms." << std::endl;
        exit(1);
    }
    cl::Platform platform = allPlatforms[platformIndex];
    std::cout << "Using platform: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

    std::vector<cl::Device> allDevices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &allDevices);
    if(allDevices.size() <= deviceIndex){
        std::cout << "Device index " << platformIndex << " out of range. Only " << allPlatforms.size() << " devices on this platform." << std::endl;
        exit(1);
    }
    cl::Device device = allDevices[deviceIndex];
    std::cout<< "Using device: "<< device.getInfo<CL_DEVICE_NAME>() << std::endl;

    return device;
}

struct CLBufferCollection {
    cl::Buffer vertices, texCoords, normals, materials, outPixels, randStates;
};

CLBufferCollection createCLBuffers(cl::Context context, cl::CommandQueue queue, int width, int height, std::vector<float> vertices, std::vector<float> texCoords, std::vector<float> normals, std::vector<Material> materials) {
    CLBufferCollection bc;

    bc.vertices = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*vertices.size());
    bc.texCoords = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*texCoords.size());
    bc.normals = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*normals.size());
    bc.materials = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(Material)*materials.size());
    bc.outPixels = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(float)*3*width*height);
    bc.randStates = cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(unsigned int)*width*height);

    auto pixels = std::make_unique<float[]>(3*width*height);

    auto randStates = std::make_unique<unsigned int[]>(width*height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            randStates[y*width + x] = (x+1)*(y+1);
        }
    }

    queue.enqueueWriteBuffer(bc.vertices,CL_TRUE,0,sizeof(float)*vertices.size(),&vertices[0]);
    queue.enqueueWriteBuffer(bc.texCoords,CL_TRUE,0,sizeof(float)*texCoords.size(),&texCoords[0]);
    queue.enqueueWriteBuffer(bc.normals,CL_TRUE,0,sizeof(float)*normals.size(),&normals[0]);
    queue.enqueueWriteBuffer(bc.materials,CL_TRUE,0,sizeof(Material)*materials.size(),&materials[0]);
    queue.enqueueWriteBuffer(bc.outPixels,CL_TRUE,0,sizeof(float)*3*width*height,pixels.get());
    queue.enqueueWriteBuffer(bc.randStates,CL_TRUE,0,sizeof(unsigned int)*width*height,randStates.get());

    return bc;
}

cl::Program buildProgram(cl::Context context, cl::Device device) {
    cl::Program::Sources sources;
    sources.push_back({renderKernelCode.c_str(),renderKernelCode.length()});

    cl::Program program(context,sources);
    if(program.build({device}) != CL_SUCCESS){
        std::cout <<" Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        exit(1);
    }

    return program;
}

void processObjects(std::vector<std::shared_ptr<Object>> objects, std::vector<float> *vertices, std::vector<float> *texCoords, std::vector<float> *normals, std::vector<Material> *materials, int *triCount) {
    for (int i = 0; i < objects.size(); i++) {
        auto obj = objects[i];
        std::vector<float> vbuf = obj->getVertexBuffer();
        *triCount += vbuf.size()/9;
        vertices->insert(vertices->end(), vbuf.begin(), vbuf.end());
        std::vector<float> texCoordBuf = obj->getTexCoordBuffer();
        texCoords->insert(texCoords->end(), texCoordBuf.begin(), texCoordBuf.end());
        std::vector<float> normalBuf = obj->getNormalBuffer();
        normals->insert(normals->end(), normalBuf.begin(), normalBuf.end());
        for (int j = 0; j < vbuf.size()/9; j++) {
            materials->push_back(obj->getMaterial());
        }
    }
}

cl::Kernel createRenderKernel(cl::Program program, CLBufferCollection bc, int triCount, int width, int height, int samplesPerPixel) {
    cl::Kernel render(program, "render");

    render.setArg(0, bc.vertices);
    render.setArg(1, bc.texCoords);
    render.setArg(2, bc.normals);
    render.setArg(3, bc.materials);
    render.setArg(4, triCount);
    render.setArg(5, width);
    render.setArg(6, height);
    render.setArg(7, samplesPerPixel);
    render.setArg(8, bc.outPixels);
    render.setArg(9, bc.randStates);

    return render;
}

void writeImage(std::string filename, int width, int height, float *pixels) {
    Image image(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = 3*(y*width + x);
            image.setColor(x, y, pixels[index+0], pixels[index+1], pixels[index+2]);
        }
    }

    image.write(filename);
}

void runKernel(cl::CommandQueue queue, cl::Kernel render, int width, int height, int samplesPerPixel, std::string filename, CLBufferCollection bc) {
    cl::Event eExecute;
    int batchSideLength = 32;
    auto pixels = std::make_unique<float[]>(3 * width * height);
    for (int i = 0; i < samplesPerPixel; i++) {
      float fractionComplete = float(i)/samplesPerPixel;
      std::cout << 100.0*fractionComplete << "% (" << i << "/" << samplesPerPixel << ")" << std::endl;
      for (int y = 0; y < height; y += batchSideLength) {
         for (int x = 0; x < width; x += batchSideLength) {
           cl::NDRange range(std::min(width-x, batchSideLength), std::min(height-y, batchSideLength));
           queue.enqueueNDRangeKernel(render, cl::NDRange(x,y), range, cl::NullRange, NULL, &eExecute);
           eExecute.wait();
         }
      }
      if (i % 50 == 0) {
        queue.enqueueReadBuffer(bc.outPixels,CL_TRUE,0,sizeof(float)*3*width*height,pixels.get());
        for (int j = 0; j < 3*width*height; j++) {
          // Scale pixels up to account because accumulated pixels are divided by samplesPerPixel
          pixels[j] = (samplesPerPixel/float(i+1))*pixels[j];
        }
        writeImage(filename, width, height, pixels.get());
      }
    }
}

void PathTracer::render(std::string filename) {
  int platformIndex = scene->getCLPlatformIndex();
  int deviceIndex = scene->getCLDeviceIndex();
  int width = scene->getRenderWidth();
  int height = scene->getRenderHeight();
  int samplesPerPixel = scene->getSamplesPerPixel();

  cl::Device device = getDevice(platformIndex, deviceIndex);
  cl::Context context({device});


  cl::Program program = buildProgram(context, device);

  std::vector<float> vertices, texCoords, normals;
  std::vector<Material> materials;
  int triCount = 0;
  processObjects(scene->getObjects(), &vertices, &texCoords, &normals, &materials, &triCount);


  //create queue to which we will push commands for the device.
  cl::CommandQueue queue(context, device);

  CLBufferCollection bc = createCLBuffers(context, queue, width, height, vertices, texCoords, normals, materials);

  cl::Kernel render = createRenderKernel(program, bc, triCount, width, height, samplesPerPixel);
  runKernel(queue, render, width, height, samplesPerPixel, filename, bc);

  auto pixels = std::make_unique<float[]>(3*width*height);
  queue.enqueueReadBuffer(bc.outPixels,CL_TRUE,0,sizeof(float)*3*width*height,pixels.get());

  queue.finish();

  writeImage(filename, width, height, pixels.get());
}
