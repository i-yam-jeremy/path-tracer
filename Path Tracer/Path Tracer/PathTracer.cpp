//
//  PathTracer.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "PathTracer.hpp"

#include <cmath>
#include <iostream>
#include <thread>
#include "cl.hpp"

#include "Image.hpp"
#include "Object.hpp"

PathTracer::PathTracer() {

}

void PathTracer::render(std::string filename, int width, int height) {
    std::vector<cl::Platform> allPlatforms;
    cl::Platform::get(&allPlatforms);
    if (allPlatforms.size() == 0) {
        std::cout << "No platforms for OpenCL" << std::endl;
        exit(1);
    }
    cl::Platform defaultPlatform = allPlatforms[0];
    std::cout << "Using platform: " << defaultPlatform.getInfo<CL_PLATFORM_NAME>() << std::endl;
    //get default device of the default platform
    std::vector<cl::Device> allDevices;
    defaultPlatform.getDevices(CL_DEVICE_TYPE_ALL, &allDevices);
    if(allDevices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device defaultDevice=allDevices[0];
    std::cout<< "Using device: "<<defaultDevice.getInfo<CL_DEVICE_NAME>()<<"\n";
    
    cl::Context context({defaultDevice});
    cl::Program::Sources sources;
    
    std::string kernel_code=
            "   void kernel render(global const float* vertices, global const float* materials, global const int width, global const int height, global int* outPixels){       "
            "       /*C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];*/                 "
            "   }                                                                               ";
    
    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(context,sources);
    if(program.build({defaultDevice})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(defaultDevice)<<"\n";
        exit(1);
    }
    
   // create buffers on the device
   cl::Buffer buffer_vertices(context,CL_MEM_READ_WRITE,sizeof(int)*10);
   cl::Buffer buffer_materials(context,CL_MEM_READ_WRITE,sizeof(int)*10);
   cl::Buffer buffer_outPixels(context,CL_MEM_READ_WRITE,sizeof(float)*3*width*height);

   int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
   int B[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

   //create queue to which we will push commands for the device.
   cl::CommandQueue queue(context,defaultDevice);

   //write arrays A and B to the device
   queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(int)*10,A);
   queue.enqueueWriteBuffer(buffer_B,CL_TRUE,0,sizeof(int)*10,B);

   //alternative way to run the kernel
   cl::Kernel kernel_render = cl::Kernel(program,"render");
   kernel_render.setArg(0,buffer_vertices);
   kernel_render.setArg(1,buffer_materials);
   kernel_render.setArg(2,buffer_outPixels);
   kernel_render.setArg(3, width);
   kernel_render.setArg(4, height);
   queue.enqueueNDRangeKernel(kernel_render,cl::NullRange,cl::NDRange(10),cl::NullRange);
   queue.finish();

   int C[10];
   //read result C from the device to array C
   queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*10,C);

   std::cout<<" result: \n";
   for(int i=0;i<10;i++){
       std::cout<<C[i]<<" ";
   }
    

    Image image(width, height);
    const int threadPoolSize = 32;
    int threadPoolIndex = 0;
    std::thread *threads = new std::thread[threadPoolSize];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            vec2 uv = vec2(2.0*float(x - width/2) / float(height), 2.0*float(y - height/2) / float(height));
            PathTracer *that = this;
            threads[threadPoolIndex] = std::thread([&image, x, y, uv, &height, &width, &that] {
                image.setColor(x, y, that->renderPixel(uv, height));
                if (x == 0) {
                    std::cout << (100.0 * float((y+1)*width) / float(width*height)) << "%" << std::endl;
                }
            });
            threadPoolIndex++;
            if (threadPoolIndex == threadPoolSize) {
                threadPoolIndex = 0;
                for (int i = 0; i < threadPoolSize; i++) {
                    threads[i].join();
                }
            }
        }
    }
    
    for (int i = 0; i < threadPoolIndex; i++) {
        threads[i].join();
    }

    //delete[] threads;
    
    image.write(filename);
}

vec3 PathTracer::renderPixel(vec2 uv, int height) {
    vec3 camera = vec3(0, 0, -2); // TODO use FOV
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(-1.0/(height/2), 1.0/(height/2));
    
    vec3 color = vec3(0);
    int sampleCount = 10;
    for (int i = 0; i < sampleCount; i++) {
        vec2 offset = vec2(dist(e2), dist(e2));
        vec3 ray = normalize(vec3(uv.x+offset.x, uv.y+offset.y, 0) - camera);
        color = color + renderPath(Ray(camera, ray), 0);
    }
    color = color * vec3(1.0 / sampleCount);
    return color;
}

vec3 PathTracer::renderPath(Ray ray, int bounceCount) {
    Scene::Intersection in = this->scene.findIntersection(ray);
    if (in.intersects) {
        Ray outRay = Ray(vec3(), vec3());
        vec3 colorScale;
        bool absorbed;
        in.object->getReflectedRay(ray, in.pos, in.normal, outRay, colorScale, absorbed);
        if (absorbed) {
            return colorScale;
        }
        else {
            return colorScale*renderPath(outRay, bounceCount+1);
        }
    }
    return vec3(0);
}
