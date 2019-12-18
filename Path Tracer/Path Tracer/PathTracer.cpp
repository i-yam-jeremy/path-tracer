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

#include <chrono>
#include <thread>

#include "Image.hpp"
#include "Object.hpp"

PathTracer::PathTracer() {

}

typedef struct __attribute__ ((packed)) material {
    cl_float emissiveness;
    cl_float3 emissionColor;
    cl_float3 baseColor;
    material(cl_float emissiveness, cl_float3 emissionColor, cl_float3 baseColor) {
        this->emissiveness = emissiveness;
        this->emissionColor = emissionColor;
        this->baseColor = baseColor;
    };
} Mat;

cl_float3 make_cl_float3(float x, float y, float z) {
    cl_float3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
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
    cl::Device defaultDevice=allDevices[1];
    std::cout<< "Using device: "<<defaultDevice.getInfo<CL_DEVICE_NAME>()<<"\n";
    
    cl::Context context({defaultDevice});
    cl::Program::Sources sources;
    
    std::string kernel_code=
            "   #define PI 3.1415926536\n"
            "   typedef struct ray {"
            "       float3 dir;\n"
            "       float3 origin;\n"
            "   } Ray;\n"
            "   Ray make_ray(float3 origin, float3 dir) {"
            "       Ray ray;\n"
            "       ray.origin = origin;\n"
            "       ray.dir = dir;\n"
            "       return ray;\n"
            "   }"
            "   float randUnit(uint *randState) {"
            "       uint x = *randState;\n"
            "       x ^= x << 13;\n"
            "       x ^= x >> 17;\n"
            "       x^= x << 5;\n"
            "       *randState = x;\n"
            "       return float(x) / 4294967296.0;\n"
            "   }"
            "   float rand(float lo, float hi, uint *randState) {"
            "       return (hi-lo)*randUnit(randState) + lo;\n"
            "   }"
            "   typedef struct __attribute__ ((packed)) material {"
            "       float emissiveness;\n"
            "       float3 emissionColor;\n"
            "       float3 baseColor;\n"
            "   } Material;\n"
            "   typedef struct intersection {"
            "       float t;\n"
            "       float3 pos;\n"
            "       float3 normal;\n"
            "       int triIndex;\n"
            "       bool intersects;\n"
            "   } Intersection;\n"
            "   float triArea(float3 A, float3 B, float3 C) {"
            "       return 0.5*length(cross(B-A, C-A));\n"
            "   }"
            "   Intersection rayTriangleIntersection(Ray ray, float3 A, float3 B, float3 C, int triIndex) {"
            "       Intersection notIntersection;\n"
            "       notIntersection.t = 10000000000.0;\n"
            "       notIntersection.pos = (float3)(0,0,0);\n"
            "       notIntersection.normal = (float3)(0,0,0);\n"
            "       notIntersection.triIndex = -1;\n"
            "       notIntersection.intersects = false;\n"
            "       float3 N = normalize(cross(A-B, A-C));\n"
            "       float3 p = A;\n"
                    //Plane intersection
            "       float d = -dot(p, N);\n"
            "       float denominator = dot(ray.dir, N);\n"
            "       if (fabs(denominator) < 0.00001) return notIntersection;\n"
            "       float t = -(dot(ray.origin, N) + d) / denominator;\n"
            "       if (t < 0) return notIntersection;\n"
                    //Triangle intersection
            "       float3 pos = ray.origin + ray.dir*t;\n"
            "       float fullTriArea = triArea(A,B,C);\n"
            "       float aA = triArea(pos, B, C);\n"
            "       float bA = triArea(pos, A, C);\n"
            "       float cA = triArea(pos, A, B);\n"
            "       if (fabs(fullTriArea - (aA+bA+cA)) < 0.0001 && fullTriArea > 0.0001) {"
            "           Intersection in;\n"
            "           in.t = t;\n"
            "           in.pos = pos;\n"
            "           in.normal = N;\n"
            "           in.triIndex = triIndex;\n"
            "           in.intersects = true;\n"
            "           return in;\n"
            "       }"
            "       return notIntersection;\n"
            "   }"
            "   Intersection closestTriangle(Ray ray, global const float* vertices, const int triCount) {"
            "       Intersection in;\n"
            "       in.t = 10000000000.0;\n"
            "       in.pos = (float3)(0,0,0);\n"
            "       in.normal = (float3)(0,0,0);\n"
            "       in.triIndex = -1;\n"
            "       in.intersects = false;\n"
            "       for (int i = 0; i < triCount; i++) {"
            "           float3 A = (float3)(vertices[9*i + 0], vertices[9*i + 1], vertices[9*i + 2]);\n"
            "           float3 B = (float3)(vertices[9*i + 3], vertices[9*i + 4], vertices[9*i + 5]);\n"
            "           float3 C = (float3)(vertices[9*i + 6], vertices[9*i + 7], vertices[9*i + 8]);\n"
            "           Intersection triIn = rayTriangleIntersection(ray, A, B, C, i);\n"
            "           if (triIn.intersects && triIn.t < in.t) {"
            "               in = triIn;\n"
            "           }"
            "       }"
            "       return in;\n"
            "   }"
            "   typedef struct bounce {"
            "       bool hasOutRay;\n"
            "       Ray outRay;\n"
            "       float3 color;\n"
            "   } Bounce;\n"
            "   Bounce renderPath(Ray ray, uint *randState, int bounceCount, global const float* vertices, global const Material* materials, const int triCount) {"
            "       Bounce b;"
            "       Intersection in = closestTriangle(ray, vertices, triCount);\n"
            "       if (!in.intersects) {"
            "           b.hasOutRay = false;\n"
            "           b.color = (float3)(0,0,0);\n"
            "           return b;\n" // Background
            "       }"
            "       Material mat = materials[in.triIndex];\n"
            "       if (rand(0,1,randState) < mat.emissiveness) {"
            "           b.hasOutRay = false;"
            "           b.color = 17.0f*mat.emissionColor;\n"
            "           return b;\n" // Emitter
            "       }"
            "       if (rand(0,1,randState) < 0.25) {"
            "           b.hasOutRay = false;\n"
            "           b.color = (float3)(0,0,0);\n"
            "           return b;\n" // Absorbed
            "       }"
            "       float3 N = in.normal;\n"
            "       float theta = rand(0, 2*PI, randState);\n"
            "       float phi = rand(0, 2*PI, randState);\n"
            "       float3 s = (float3)(1.0, theta, phi);\n" // Random point on sphere
            "       float3 pathDir = (float3)(s.x*sin(s.z)*cos(s.y),"
            "                                 s.x*sin(s.z)*sin(s.y),"
            "                                 s.x*cos(s.z));\n"
            "       pathDir = pathDir + N;\n"
            "       pathDir = normalize(pathDir);\n"
            "       float lambertReflectanceFactor = dot(N, pathDir);\n"
            "       Ray reflectedRay = make_ray(in.pos + 0.000001f*N, pathDir);\n"
            "       float3 colorScale = lambertReflectanceFactor*mat.baseColor;\n"
            "       b.hasOutRay = true;\n"
            "       b.outRay = reflectedRay;\n"
            "       b.color = colorScale;\n"
            "       return b;\n"
            "   }"
            "   void kernel render(global const float* vertices, global const Material* materials, const int triCount, const int width, const int height, const int samplesPerPixel, global float* outPixels, global uint* randStates){       "
            "       int x = get_global_id(0);\n"
            "       int y = get_global_id(1);\n"
            "       uint randState = randStates[y*width + x];\n"
            "       float2 uv = (float2)(x - width/2.0, y - height/2.0) / float(height);       "
            "       float3 color = (float3)(0,0,0);\n"
            "       for (int i = 0; i < 1; i++) {"
            "           float3 camera = (float3)(0, 0, -2);\n"
            "           float3 rayDir = normalize((float3)(uv.x, uv.y, 0) - camera);\n"
            "           Ray ray = make_ray(camera, rayDir);\n"
            "           float3 c = (float3)(1,1,1);\n"
            "           Bounce b = renderPath(ray, &randState, 0, vertices, materials, triCount);\n"
            "           c *= b.color;\n"
            "           int bounceCount = 1;\n"
            "           while (b.hasOutRay && bounceCount < 10) {"
            "               ray = b.outRay;\n"
            "               b = renderPath(ray, &randState, 0, vertices, materials, triCount);\n"
            "               c *= b.color;\n"
            "               bounceCount++;\n"
            "           }"
            "           if (bounceCount < 10) color += c;\n"
            "       }"
            "       color /= samplesPerPixel;\n"
            "       int pixelIndex = 3*(y*width + x);\n"
            "       outPixels[pixelIndex+0] += color.x;\n"
            "       outPixels[pixelIndex+1] += color.y;\n"
            "       outPixels[pixelIndex+2] += color.z;\n"
            "       randStates[y*width + x] = randState;\n"
            "   }                                                                               ";
    
    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(context,sources);
    if(program.build({defaultDevice})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(defaultDevice)<<"\n";
        exit(1);
    }
    
    std::vector<float> vertices;
    vertices.push_back(-0.5);
    vertices.push_back(0.0);
    vertices.push_back(0.0);
    vertices.push_back(0.0);
    vertices.push_back(0.0);
    vertices.push_back(0.0);
    vertices.push_back(0.0);
    vertices.push_back(0.5);
    vertices.push_back(0.1);

    std::vector<Mat> materials;
    materials.push_back(Mat(1.0, make_cl_float3(1,1,1), make_cl_float3(0,0,0)));
    
    int triCount = 0;
    triCount += 1;
    
    for (auto obj : this->scene.objects) {
        std::vector<float> vbuf = obj.getVertexBuffer();
        triCount += vbuf.size()/9;
        vertices.insert(vertices.end(), vbuf.begin(), vbuf.end());
        for (int i = 0; i < vbuf.size()/9; i++) {
            materials.push_back(Mat(0.0, make_cl_float3(0,0,0), make_cl_float3(1,1,1)));
        }
    }

    
   // create buffers on the device
   cl::Buffer buffer_vertices(context,CL_MEM_READ_WRITE,sizeof(float)*vertices.size());
   cl::Buffer buffer_materials(context,CL_MEM_READ_WRITE,sizeof(Mat)*materials.size());
   cl::Buffer buffer_outPixels(context,CL_MEM_READ_WRITE,sizeof(float)*3*width*height);
   cl::Buffer buffer_randStates(context,CL_MEM_READ_WRITE,sizeof(unsigned int)*width*height);

   //create queue to which we will push commands for the device.
   cl::CommandQueue queue(context,defaultDevice);

   //write arrays A and B to the device
   float *pixels = new float[3*width*height]();
   unsigned int *randStates = new unsigned int[width*height];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            randStates[y*width + x] = (x+1)*(y+1);
        }
    }
   queue.enqueueWriteBuffer(buffer_vertices,CL_TRUE,0,sizeof(float)*vertices.size(),&vertices[0]);
   queue.enqueueWriteBuffer(buffer_materials,CL_TRUE,0,sizeof(Mat)*materials.size(),&materials[0]);
   queue.enqueueWriteBuffer(buffer_outPixels,CL_TRUE,0,sizeof(float)*3*width*height,pixels);
   queue.enqueueWriteBuffer(buffer_randStates,CL_TRUE,0,sizeof(unsigned int)*width*height,randStates);
   delete[] randStates;

   int samplesPerPixel = 1000;
    
   //alternative way to run the kernel
   cl::Kernel kernel_render = cl::Kernel(program,"render");
   kernel_render.setArg(0,buffer_vertices);
   kernel_render.setArg(1,buffer_materials);
   kernel_render.setArg(2, triCount);
   kernel_render.setArg(3, width);
   kernel_render.setArg(4, height);
   kernel_render.setArg(5, samplesPerPixel);
   kernel_render.setArg(6, buffer_outPixels);
   kernel_render.setArg(7, buffer_randStates);
    
    cl::Event eExecute;
    Image image(width, height);
    for (int i = 0; i < samplesPerPixel; i++) {
        queue.enqueueNDRangeKernel(kernel_render,cl::NullRange,cl::NDRange(width, height),cl::NullRange, NULL, &eExecute);
        eExecute.wait();
        std::cout << 100.0*float(i+1)/samplesPerPixel << "%" << std::endl;
    }
    queue.enqueueReadBuffer(buffer_outPixels,CL_TRUE,0,sizeof(float)*3*width*height,pixels);
    
    //queue.finish();
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = 3*(y*width + x);
            image.setColor(x, y, vec3(pixels[index+0], pixels[index+1], pixels[index+2]));
        }
    }
    
    image.write(filename);
    
    delete[] pixels;

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
