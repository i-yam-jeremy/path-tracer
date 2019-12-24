//
//  Object.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/11/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <string>

#include "Material.hpp"
#include "OBJLoader.hpp"

/*
  A 3D object defined by a mesh and a material.
*/
class Object {

public:
    /*
      Creates an object from the given OBJ file and material.
      @param objFilename - The path to the OBJ file that defines the mesh.
      @param material - The material to use for this object.
    */
    Object(std::string objFilename, Material material);
    ~Object();
    /*
      Returns the non-indexed vertex positions in x,y,z order stored as a float
      vector.
      @return The non-indexed vertex positions.
    */
    std::vector<float>& getVertexBuffer();
    /*
      Returns the non-indexed texture coordinates in x,y,z order stored as a
      float vector.
      @return The non-indexed texture coordinates.
    */
    std::vector<float>& getTexCoordBuffer();
    /*
      Returns the non-indexed surface normals in x,y,z order stored as a float
      vector.
      @return The non-indexed normals.
    */
    std::vector<float>& getNormalBuffer();
    /*
      Returns the material used by this object.
      @return The material used by this object.
    */
    Material getMaterial();

private:
    /*
      The loaded OBJ mesh.
    */
    OBJ *obj;
    /*
      The material used by this object.
    */
    Material material;

};

#endif /* Object_hpp */
