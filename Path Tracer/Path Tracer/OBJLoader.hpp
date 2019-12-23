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
         Loads
         */
        OBJ(std::string filename);
        /*
         Returns the vertex data stored as x,y,z components in a float vector.
         */
        std::vector<float>& getVertexBuffer();
    private:
        std::vector<float> vertices;
};

#endif /* OBJLoader_hpp */
