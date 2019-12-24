
#define PI 3.1415926536
float3 reflect(float3 I, float3 N) { return I - 2.0f*dot(N,I)*N; }
typedef struct ray {
   float3 dir;
   float3 origin;
} Ray;
Ray make_ray(float3 origin, float3 dir) {
   Ray ray;
   ray.origin = origin;
   ray.dir = dir;
   return ray;
}
float randUnit(uint *randState) {
   uint x = *randState;
   x ^= x << 13;
   x ^= x >> 17;
   x^= x << 5;
   *randState = x;
   return float(x) / 4294967296.0;
}
float rand(float lo, float hi, uint *randState) {
   return (hi-lo)*randUnit(randState) + lo;
}
typedef struct __attribute__ ((packed)) material {
   float emissiveness;
   float3 emissionColor;
   float metalness;
   float3 baseColor;
} Material;
typedef struct intersection {
   float t;
   float3 pos;
   float2 texCoord;
   float3 normal;
   int triIndex;
   bool intersects;
} Intersection;
float triArea(float3 A, float3 B, float3 C) {
   return 0.5*length(cross(B-A, C-A));
}
Intersection rayTriangleIntersection(Ray ray, float3 A, float3 B, float3 C, float2 tcA, float2 tcB, float2 tcC, float3 nA, float3 nB, float3 nC, int triIndex) {
   Intersection notIntersection;
   notIntersection.t = 10000000000.0;
   notIntersection.pos = (float3)(0,0,0);
   notIntersection.normal = (float3)(0,0,0);
   notIntersection.triIndex = -1;
   notIntersection.intersects = false;
   float3 N = normalize(cross(A-B, A-C));
   float3 p = A;
    //Plane intersection
   float d = -dot(p, N);
   float denominator = dot(ray.dir, N);
   if (fabs(denominator) < 0.00001) return notIntersection;
   float t = -(dot(ray.origin, N) + d) / denominator;
   if (t < 0) return notIntersection;
    //Triangle intersection
   float3 pos = ray.origin + ray.dir*t;
   float fullTriArea = triArea(A,B,C);
   float aA = triArea(pos, B, C);
   float bA = triArea(pos, A, C);
   float cA = triArea(pos, A, B);
   float alpha = aA / fullTriArea;
   float beta = bA / fullTriArea;
   float gamma = cA / fullTriArea;
   if (fabs(fullTriArea - (aA+bA+cA)) < 0.0001 && fullTriArea > 0.0001) {
       Intersection in;
       in.t = t;
       in.pos = pos;
       in.texCoord = alpha*tcA + beta*tcB + gamma*tcC;
       in.normal = normalize(alpha*nA + beta*nB + gamma*nC);
       in.triIndex = triIndex;
       in.intersects = true;
       return in;
   }
   return notIntersection;
}
Intersection closestTriangle(Ray ray, global const float* vertices, global const float* texCoords, global const float* normals, const int triCount) {
   Intersection in;
   in.t = 10000000000.0;
   in.pos = (float3)(0,0,0);
   in.normal = (float3)(0,0,0);
   in.triIndex = -1;
   in.intersects = false;
   for (int i = 0; i < triCount; i++) {
       float3 A = (float3)(vertices[9*i + 0], vertices[9*i + 1], vertices[9*i + 2]);
       float3 B = (float3)(vertices[9*i + 3], vertices[9*i + 4], vertices[9*i + 5]);
       float3 C = (float3)(vertices[9*i + 6], vertices[9*i + 7], vertices[9*i + 8]);
       float2 tcA = (float2)(texCoords[6*i + 0], texCoords[6*i + 1]);
       float2 tcB = (float2)(texCoords[6*i + 2], texCoords[6*i + 3]);
       float2 tcC = (float2)(texCoords[6*i + 4], texCoords[6*i + 5]);
       float3 nA = (float3)(normals[9*i + 0], normals[9*i + 1], normals[9*i + 2]);
       float3 nB = (float3)(normals[9*i + 3], normals[9*i + 4], normals[9*i + 5]);
       float3 nC = (float3)(normals[9*i + 6], normals[9*i + 7], normals[9*i + 8]);
       Intersection triIn = rayTriangleIntersection(ray, A, B, C, tcA, tcB, tcC, nA, nB, nC, i);
       if (triIn.intersects && triIn.t < in.t) {
           in = triIn;
       }
   }
   return in;
}
typedef struct bounce {
   bool hasOutRay;
   Ray outRay;
   float3 color;
} Bounce;
Bounce renderPath(Ray ray, uint *randState, int bounceCount, global const float* vertices, global const float* texCoords, global const float* normals, global const Material* materials, const int triCount) {
   Bounce b;
   Intersection in = closestTriangle(ray, vertices, texCoords, normals, triCount);
   if (!in.intersects) {
       b.hasOutRay = false;
       b.color = (float3)(0,0,0);
       return b; // Background
   }
   Material mat = materials[in.triIndex];
   if (rand(0,1,randState) < mat.emissiveness) {
       b.hasOutRay = false;
       b.color = 15.0f*mat.emissionColor;
       return b; // Emitter
   }
   if (rand(0,1,randState) < 0.25) {
       b.hasOutRay = false;
       b.color = (float3)(0,0,0);
       return b; // Absorbed
   }
   if (rand(0,1,randState) < mat.metalness) {
       b.hasOutRay = true;
       b.outRay = make_ray(in.pos, reflect(ray.dir, in.normal));
       b.color = mat.baseColor;
       return b;
   }
   float3 N = in.normal;
   float theta = rand(0, 2*PI, randState);
   float phi = rand(0, 2*PI, randState);
   float3 s = (float3)(1.0, theta, phi); // Random point on sphere
   float3 pathDir = (float3)(s.x*sin(s.z)*cos(s.y),
                             s.x*sin(s.z)*sin(s.y),
                             s.x*cos(s.z));
   pathDir = pathDir + N;
   pathDir = normalize(pathDir);
   float lambertReflectanceFactor = dot(N, pathDir);
   Ray reflectedRay = make_ray(in.pos + 0.000001f*N, pathDir);
   float3 colorScale = lambertReflectanceFactor*mat.baseColor;
   b.hasOutRay = true;
   b.outRay = reflectedRay;
   b.color = colorScale;
   return b;
}
void kernel render(global const float* vertices, global const float* texCoords, global const float* normals, global const Material* materials, const int triCount, const int width, const int height, const int samplesPerPixel, global float* outPixels, global uint* randStates){
   int x = get_global_id(0);
   int y = get_global_id(1);
   uint randState = randStates[y*width + x];
   float2 uvOffset = (float2)(rand(0,1,&randState), rand(0,1,&randState)) / float(height);
   float2 uv = uvOffset + (float2)(x - width/2.0, y - height/2.0) / float(height);
   float3 camera = (float3)(0, 0, -2);
   float3 rayDir = normalize((float3)(uv.x, uv.y, 0) - camera);
   Ray ray = make_ray(camera, rayDir);
   float3 color = (float3)(1,1,1);
   Bounce b = renderPath(ray, &randState, 0, vertices, texCoords, normals, materials, triCount);
   color *= b.color;
   while (b.hasOutRay) {
       ray = b.outRay;
       b = renderPath(ray, &randState, 0, vertices, texCoords, normals, materials, triCount);
       color *= b.color;
   }
   color /= samplesPerPixel;
   int pixelIndex = 3*(y*width + x);
   outPixels[pixelIndex+0] += color.x;
   outPixels[pixelIndex+1] += color.y;
   outPixels[pixelIndex+2] += color.z;
   randStates[y*width + x] = randState;
}                                                                               ;
