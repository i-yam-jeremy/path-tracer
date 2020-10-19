R"(

#define PI 3.1415926536
#define EPSILON 0.00001

/*
  Reflects the given vector I across the given normal N.
  @param I - The incoming vector.
  @param N - The normal.
  @return The outgoing reflected vector.
*/
float3 reflect(float3 I, float3 N) { return I - 2.0f*dot(N,I)*N; }

/*
  Represents a ray.
*/
typedef struct ray {
   /*
    The direction of the ray.
   */
   float3 dir;
   /*
    The origin of the ray.
   */
   float3 origin;
} Ray;

/*
  Creates a ray with the given origin and direction.
  @param origin - The ray origin.
  @param dir - The ray direction.
  @return A ray with the given origin and direction.
*/
Ray make_ray(float3 origin, float3 dir) {
   Ray ray;
   ray.origin = origin;
   ray.dir = dir;
   return ray;
}

/*
  Generates a uniformly-distributed random float in the interval [0, 1] based on
    the given random state. Updates the given random state.
  @param randState - A pointer to the random state.
  @return A random number in the interval [0, 1].
*/
float randUnit(uint *randState) {
   uint x = *randState;
   x ^= x << 13;
   x ^= x >> 17;
   x^= x << 5;
   *randState = x;
   return (float)(x) / 4294967296.0;
}

/*
  Generates a uniformly-distributed random float in the interval [lo, hi] based
    on the given random state. Updates the given random state.
  @param lo - The lower bound of the random range.
  @param hi - The upper bound of the random range.
  @param randState - A pointer to the random state.
  @return A random number in the interval [lo, hi].
*/
float rand(float lo, float hi, uint *randState) {
   return (hi-lo)*randUnit(randState) + lo;
}

/*
  A PBR material.
  It is packed so the structure matches the host material struct.
*/
typedef struct __attribute__ ((packed)) material {
  /*
    The probability the material will emit light.
  */
   float emissiveness;
   /*
     The color of the emitted light.
   */
   float3 emissionColor;
   /*
     The probability the material will reflect light like a metal.
     It is best to only use 0.0 or 1.0 for non-metals and metals, respectively.
   */
   float metalness;
   /*
     The base color of the material.
   */
   float3 baseColor;
} Material;

/*
  A Ray-Triangle intersection.
*/
typedef struct intersection {
    /*
      Specifies the intersection position on the ray.
      intersectionPos = ray.origin + t*ray.dir;
    */
    float t;
    /*
      The position where the intersection occurred.
    */
    float3 pos;
    /*
      The interpolated texture coordinates of the surface at the intersection
      point.
    */
    float2 texCoord;
    /*
      The interpolated normal of the surface at the intersection point.
    */
    float3 normal;
    /*
      The index of the intersected triangle out of all triangles in the scene.
    */
    int triIndex;
    /*
      Whether there was an intersection or not.
    */
    bool intersects;
} Intersection;

/*
  Returns the area of a triangle with vertices A,B,C.
  @param A - A vertex of the triangle.
  @param B - A vertex of the triangle.
  @param C - A vertex of the triangle.
  @return The triangle area.
*/
float triArea(float3 A, float3 B, float3 C) {
   return 0.5*length(cross(B-A, C-A));
}

/*
  Calculates the intersection between a ray and a triangle if one exists.
    The triangle is specified by the vertices A,B,C.
    Uses the Moller-Trumbore intersection algorithm.
  @param ray - The ray to used to check intersection.
  @param A - A vertex of the triangle.
  @param B - A vertex of the triangle.
  @param C - A vertex of the triangle.
  @param tcA - The texture coordinates of vertex A.
  @param tcB - The texture coordinates of vertex B.
  @param tcC - The texture coordinates of vertex C.
  @param nA - The normal of vertex A.
  @param nB - The normal of vertex B.
  @param nC - The normal of vertex C.
  @param triIndex - The index of the triangle out of all triangles in the scene.
  @return The intersection.
*/
Intersection rayTriangleIntersection(Ray ray, float3 A, float3 B, float3 C, float2 tcA, float2 tcB, float2 tcC, float3 nA, float3 nB, float3 nC, int triIndex) {
   Intersection notIntersection;
   notIntersection.t = 10000000000.0;
   notIntersection.pos = (float3)(0,0,0);
   notIntersection.normal = (float3)(0,0,0);
   notIntersection.triIndex = -1;
   notIntersection.intersects = false;
   
   float3 edge1, edge2, p, s, q;
   float det, inv_det, u, v;

   edge1 = B - A;
   edge2 = C - A;
   p = cross(ray.dir, edge2);
   det = dot(edge1, p);
   if (det > -EPSILON && det < EPSILON) {
    return notIntersection;
   }
   inv_det = 1.0/det;
   s = ray.origin - A;
   u = inv_det * dot(s, p);
   if (u < 0.0 || u > 1.0) {
    return notIntersection;
   }
   q = cross(s, edge1);
   v = inv_det * dot(ray.dir, q);
   if (v < 0.0 || u + v > 1.0) {
    return notIntersection;
   }

   float t = inv_det * dot(edge2, q);

   if (t > EPSILON) {
       Intersection in;
       in.t = t;
       float alpha, beta, gamma;
       gamma = v;
       beta = u;
       alpha = 1.0 - u - v;
       in.pos = ray.origin + t*ray.dir;
       in.texCoord = alpha*tcA + beta*tcB + gamma*tcC;
       in.normal = normalize(alpha*nA + beta*nB + gamma*nC);
       in.triIndex = triIndex;
       in.intersects = true;
       return in;
   } else {
    return notIntersection;
   }
}

/*
  Returns the intersection to the closest triangle along the ray. The closest
  triangle is defined as the intersection point being closest to the ray origin.
  @param ray - The ray used to check intersection.
  @param vertices - All vertices in the scene.
  @param texCoords - All texture coordinates in the scene.
  @param normals - All normals in the scene.
  @param triCount - The total number of triangles in the scene.
  @return The intersection.
*/
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

/*
  A light bounce or absorption.
*/
typedef struct bounce {
    /*
      Whether this bounce has an outgoing ray. There is an outgoing ray iff the
      light path was reflected.
    */
    bool hasOutRay;
    /*
      The outgoing ray. If hasOutRay is false, then this value is undefined and
      meaningless.
    */
    Ray outRay;
    /*
      The color factor of this bounce.
    */
    float3 color;
} Bounce;

/*
  Computes one bounce of the given light path.
  @param ray - The ray representing the light path.
  @param randState - The state for the random number generator.
  @param vertices - All vertices in the scene.
  @param texCoords - All texture coordinates in the scene.
  @param normals - All normals in the scene.
  @param materials - All materials in the scene.
  @param triCount - The total number of triangles in the scene.
  @return The bounced or absorbed light data.
*/
Bounce renderPath(Ray ray, uint *randState, global const float* vertices, global const float* texCoords, global const float* normals, global const Material* materials, const int triCount) {
   Bounce b;
   Intersection in = closestTriangle(ray, vertices, texCoords, normals, triCount);
   if (!in.intersects) {
       b.hasOutRay = false;
       b.color = (float3)(0.8,0.8,0.8);
       return b; // Background
   }
   Material mat = materials[in.triIndex];
   if (rand(0,1,randState) < mat.emissiveness) {
       b.hasOutRay = false;
       b.color = 5.0f*mat.emissionColor;
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
   float3 pathDir = (float3)(1,1,1);
   do {
    pathDir = (float3)(randUnit(randState), randUnit(randState), randUnit(randState));
   } while (dot(pathDir,pathDir) >= 1.0);
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

/*
  Renders one light path through a pixel.
  @param vertices - All vertices in the scene.
  @param texCoords - All texture coordinates in the scene.
  @param normals - All normals in the scene.
  @param materials - All materials in the scene (one material per triangle).
  @param triCount - The total number of triangles in the scene.
  @param width - The width of the rendered viewport in pixels.
  @param height - The height of the rendered viewport in pixels.
  @param samplesPerPixel - The number of samples per pixel.
  @param outPixels - The rendered pixel data.
  @param randStates - The random states for each pixel in the scene.
*/
void kernel render(global const float* vertices, global const float* texCoords, global const float* normals, global const Material* materials, const int triCount, const int width, const int height, const int samplesPerPixel, global float* outPixels, global uint* randStates){
   int x = get_global_id(0);
   int y = get_global_id(1);
   uint randState = randStates[y*width + x];
   float2 uvOffset = (float2)(rand(0,1,&randState), rand(0,1,&randState)) / (float)(height);
   float2 uv = uvOffset + (float2)(x - width/2.0, y - height/2.0) / (float)(height);
   float3 camera = (float3)(0, 0, -2);
   float3 rayDir = normalize((float3)(uv.x, uv.y, 0) - camera);
   Ray ray = make_ray(camera, rayDir);
   float3 color = (float3)(1,1,1);
   Bounce b = renderPath(ray, &randState, vertices, texCoords, normals, materials, triCount);
   color *= b.color;
   while (b.hasOutRay) {
       ray = b.outRay;
       b = renderPath(ray, &randState, vertices, texCoords, normals, materials, triCount);
       color *= b.color;
   }
   color /= samplesPerPixel;
   int pixelIndex = 3*(y*width + x);
   outPixels[pixelIndex+0] += color.x;
   outPixels[pixelIndex+1] += color.y;
   outPixels[pixelIndex+2] += color.z;
   randStates[y*width + x] = randState;
}                                                                               ;

)"