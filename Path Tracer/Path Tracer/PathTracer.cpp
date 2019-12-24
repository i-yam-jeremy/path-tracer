//
//  PathTracer.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "PathTracer.hpp"

#include <iostream>
#include "cl.hpp"

#include "Image.hpp"
#include "Object.hpp"
#include "Material.hpp"

PathTracer::PathTracer(Scene *scene) {
    this->scene = scene;
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
    cl::Device device = allDevices[1];
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

    float *pixels = new float[3*width*height]();
    
    unsigned int *randStates = new unsigned int[width*height];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            randStates[y*width + x] = (x+1)*(y+1);
        }
    }

    queue.enqueueWriteBuffer(bc.vertices,CL_TRUE,0,sizeof(float)*vertices.size(),&vertices[0]);
    queue.enqueueWriteBuffer(bc.texCoords,CL_TRUE,0,sizeof(float)*texCoords.size(),&texCoords[0]);
    queue.enqueueWriteBuffer(bc.normals,CL_TRUE,0,sizeof(float)*normals.size(),&normals[0]);
    queue.enqueueWriteBuffer(bc.materials,CL_TRUE,0,sizeof(Material)*materials.size(),&materials[0]);
    queue.enqueueWriteBuffer(bc.outPixels,CL_TRUE,0,sizeof(float)*3*width*height,pixels);
    queue.enqueueWriteBuffer(bc.randStates,CL_TRUE,0,sizeof(unsigned int)*width*height,randStates);
    
    delete[] pixels;
    delete[] randStates;

    return bc;
}

std::string kernel_code=
"   #define PI 3.1415926536\n"
"   float3 reflect(float3 I, float3 N) { return I - 2.0f*dot(N,I)*N; }\n"
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
"       float metalness;\n"
"       float3 baseColor;\n"
"   } Material;\n"
"   typedef struct intersection {"
"       float t;\n"
"       float3 pos;\n"
"       float2 texCoord;\n"
"       float3 normal;\n"
"       int triIndex;\n"
"       bool intersects;\n"
"   } Intersection;\n"
"   float triArea(float3 A, float3 B, float3 C) {"
"       return 0.5*length(cross(B-A, C-A));\n"
"   }"
"   Intersection rayTriangleIntersection(Ray ray, float3 A, float3 B, float3 C, float2 tcA, float2 tcB, float2 tcC, float3 nA, float3 nB, float3 nC, int triIndex) {"
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
"       float alpha = aA / fullTriArea;\n"
"       float beta = bA / fullTriArea;\n"
"       float gamma = cA / fullTriArea;\n"
"       if (fabs(fullTriArea - (aA+bA+cA)) < 0.0001 && fullTriArea > 0.0001) {"
"           Intersection in;\n"
"           in.t = t;\n"
"           in.pos = pos;\n"
"           in.texCoord = alpha*tcA + beta*tcB + gamma*tcC;\n"
"           in.normal = normalize(alpha*nA + beta*nB + gamma*nC);\n"
"           in.triIndex = triIndex;\n"
"           in.intersects = true;\n"
"           return in;\n"
"       }"
"       return notIntersection;\n"
"   }"
"   Intersection closestTriangle(Ray ray, global const float* vertices, global const float* texCoords, global const float* normals, const int triCount) {"
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
"           float2 tcA = (float2)(texCoords[6*i + 0], texCoords[6*i + 1]);\n"
"           float2 tcB = (float2)(texCoords[6*i + 2], texCoords[6*i + 3]);\n"
"           float2 tcC = (float2)(texCoords[6*i + 4], texCoords[6*i + 5]);\n"
"           float3 nA = (float3)(normals[9*i + 0], normals[9*i + 1], normals[9*i + 2]);\n"
"           float3 nB = (float3)(normals[9*i + 3], normals[9*i + 4], normals[9*i + 5]);\n"
"           float3 nC = (float3)(normals[9*i + 6], normals[9*i + 7], normals[9*i + 8]);\n"
"           Intersection triIn = rayTriangleIntersection(ray, A, B, C, tcA, tcB, tcC, nA, nB, nC, i);\n"
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
"   Bounce renderPath(Ray ray, uint *randState, int bounceCount, global const float* vertices, global const float* texCoords, global const float* normals, global const Material* materials, const int triCount) {"
"       Bounce b;"
"       Intersection in = closestTriangle(ray, vertices, texCoords, normals, triCount);\n"
"       if (!in.intersects) {"
"           b.hasOutRay = false;\n"
"           b.color = (float3)(0,0,0);\n"
"           return b;\n" // Background
"       }"
"       Material mat = materials[in.triIndex];\n"
"       if (rand(0,1,randState) < mat.emissiveness) {"
"           b.hasOutRay = false;"
"           b.color = 15.0f*mat.emissionColor;\n"
"           return b;\n" // Emitter
"       }"
"       if (rand(0,1,randState) < 0.25) {"
"           b.hasOutRay = false;\n"
"           b.color = (float3)(0,0,0);\n"
"           return b;\n" // Absorbed
"       }"
"       if (rand(0,1,randState) < mat.metalness) {"
"           b.hasOutRay = true;\n"
"           b.outRay = make_ray(in.pos, reflect(ray.dir, in.normal));\n"
"           b.color = mat.baseColor;\n"
"           return b;\n"
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
"   void kernel render(global const float* vertices, global const float* texCoords, global const float* normals, global const Material* materials, const int triCount, const int width, const int height, const int samplesPerPixel, global float* outPixels, global uint* randStates){       "
"       int x = get_global_id(0);\n"
"       int y = get_global_id(1);\n"
"       uint randState = randStates[y*width + x];\n"
"       float2 uvOffset = (float2)(rand(0,1,&randState), rand(0,1,&randState)) / float(height);\n"
"       float2 uv = uvOffset + (float2)(x - width/2.0, y - height/2.0) / float(height);       "
"       float3 camera = (float3)(0, 0, -2);\n"
"       float3 rayDir = normalize((float3)(uv.x, uv.y, 0) - camera);\n"
"       Ray ray = make_ray(camera, rayDir);\n"
"       float3 color = (float3)(1,1,1);\n"
"       Bounce b = renderPath(ray, &randState, 0, vertices, texCoords, normals, materials, triCount);\n"
"       color *= b.color;\n"
"       while (b.hasOutRay) {"
"           ray = b.outRay;\n"
"           b = renderPath(ray, &randState, 0, vertices, texCoords, normals, materials, triCount);\n"
"           color *= b.color;\n"
"       }"
"       color /= samplesPerPixel;\n"
"       int pixelIndex = 3*(y*width + x);\n"
"       outPixels[pixelIndex+0] += color.x;\n"
"       outPixels[pixelIndex+1] += color.y;\n"
"       outPixels[pixelIndex+2] += color.z;\n"
"       randStates[y*width + x] = randState;\n"
"   }                                                                               ";

cl::Program buildProgram(cl::Context context, cl::Device device) {
    cl::Program::Sources sources;
    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(context,sources);
    if(program.build({device}) != CL_SUCCESS){
        std::cout <<" Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        exit(1);
    }
    
    return program;
}

void processObjects(std::vector<Object*> objects, std::vector<float> *vertices, std::vector<float> *texCoords, std::vector<float> *normals, std::vector<Material> *materials, int *triCount) {
    for (int i = 0; i < objects.size(); i++) {
        Object *obj = objects[i];
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

void runKernel(cl::CommandQueue queue, cl::Kernel render, int width, int height, int samplesPerPixel) {
    cl::Event eExecute;
     int batchSideLength = 32;
     for (int y = 0; y < height; y += batchSideLength) {
         for (int x = 0; x < width; x += batchSideLength) {
             float fractionComplete = batchSideLength*float(y*width/batchSideLength + x)/(width*height);
             std::cout << 100.0*fractionComplete << "%" << ", (" << x << ", " << y << ")" << std::endl;
             for (int i = 0; i < samplesPerPixel; i++) {
                 cl::NDRange range(std::min(width-x, batchSideLength), std::min(height-y, batchSideLength));
                 queue.enqueueNDRangeKernel(render, cl::NDRange(x,y), range, cl::NullRange, NULL, &eExecute);
                 eExecute.wait();
             }
         }
     }
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

void PathTracer::render(std::string filename, int platformIndex, int deviceIndex, int width, int height, int samplesPerPixel) {
    cl::Device device = getDevice(platformIndex, deviceIndex);
    cl::Context context({device});
    
    
    cl::Program program = buildProgram(context, device);
    
    std::vector<float> vertices, texCoords, normals;
    std::vector<Material> materials;
    int triCount = 0;
    processObjects(this->scene->getObjects(), &vertices, &texCoords, &normals, &materials, &triCount);


   //create queue to which we will push commands for the device.
   cl::CommandQueue queue(context, device);
    
    CLBufferCollection bc = createCLBuffers(context, queue, width, height, vertices, texCoords, normals, materials);
    
    cl::Kernel render = createRenderKernel(program, bc, triCount, width, height, samplesPerPixel);
    runKernel(queue, render, width, height, samplesPerPixel);
    
    float *pixels = new float[3*width*height];
    queue.enqueueReadBuffer(bc.outPixels,CL_TRUE,0,sizeof(float)*3*width*height,pixels);
    
    queue.finish();
    
    writeImage(filename, width, height, pixels);
    
    delete[] pixels;
}
