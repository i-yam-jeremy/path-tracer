//
//  OBJLoader.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/18/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef OBJLoader_hpp
#define OBJLoader_hpp

#include <string>
#include <vector>

/*
 A representation of data from an .obj file.
 */
class OBJ {
public:
    /*
     Loads the OBJ file specified by the given filename.
     */
    OBJ(std::string filename);
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
private:
    /*
      The non-indexed vertex positions in x,y,z order stored as a float vector.
    */
    std::vector<float> vertices;
    /*
      The non-indexed texture coordinates in x,y,z order stored as a float
      vector.
    */
    std::vector<float> texCoords;
    /*
      The non-indexed surface normals in x,y,z order stored as a float vector.
    */
    std::vector<float> normals;
};

#endif /* OBJLoader_hpp */
