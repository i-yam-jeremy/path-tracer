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
            "   typedef struct ray {"
            "       float3 dir;"
            "       float3 origin;"
            "   } Ray;"
            "   Ray make_ray(float3 origin, float3 dir) {"
            "       Ray ray;"
            "       ray.origin = origin;"
            "       ray.dir = dir;"
            "       return ray;"
            "   }"
            /*" uint randState = 1781351;"
            "   float randUnit() {"
            "       uint x = randState;"
            "       x ^= x << 13;"
            "       x ^= x >> 17;"
            "       x^= x << 5;"
            "       randState = x;"
            "       return float(x) / (uint(1) << 31);"
            "   }"
            "   float rand(float lo, float hi) {"
            "       return (lo-hi)*randUnit() + lo;"
            "   }"*/
            "   float3 renderPath(Ray ray, int bounceCount) {"
            "       return (float3)(0,0,0);"
            "   }"
            "   void kernel render(global const float* vertices, global const float* materials,  const int width, const int height, const int samplesPerPixel, global int* outPixels){       "
            "       int x = get_global_id(0) % width; "
            "       int y = get_global_id(0) / width; "
            "       float2 uv = (float2)(x - width/2.0, y - height/2.0) / float(height);       "
            "       float3 color = (float3)(0,0,0);"
            "       for (int i = 0; i < samplesPerPixel; i++) {"
            "           float3 camera = (float3)(0, 0, -2);"
            "           float3 rayDir = normalize(camera - (float3)(uv.x, uv.y, 0));"
            "           Ray ray = make_ray(rayDir, camera);"
            "           color += renderPath(ray, 0);"
            "       }"
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

   float vertices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
   float materials[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

   //create queue to which we will push commands for the device.
   cl::CommandQueue queue(context,defaultDevice);

   //write arrays A and B to the device
   queue.enqueueWriteBuffer(buffer_vertices,CL_TRUE,0,sizeof(vertices),vertices);
   queue.enqueueWriteBuffer(buffer_materials,CL_TRUE,0,sizeof(materials),materials);

   //alternative way to run the kernel
   cl::Kernel kernel_render = cl::Kernel(program,"render");
   kernel_render.setArg(0,buffer_vertices);
   kernel_render.setArg(1,buffer_materials);
   kernel_render.setArg(2, width);
   kernel_render.setArg(3, height);
   kernel_render.setArg(4, 100);
   kernel_render.setArg(5,buffer_outPixels);
   queue.enqueueNDRangeKernel(kernel_render,cl::NullRange,cl::NDRange(width*height),cl::NullRange);
   queue.finish();

   float *pixels = new float[3*width*height];
   //read result C from the device to array C
   queue.enqueueReadBuffer(buffer_outPixels,CL_TRUE,0,sizeof(int)*10,pixels);

    Image image(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = 3*(y*width + x);
            image.setColor(x, y, vec3(pixels[index+0], pixels[index+1], pixels[index+2]));
        }
    }
    
    image.write(filename);
    

    /*Image image(width, height);
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
    
    image.write(filename);*/
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
