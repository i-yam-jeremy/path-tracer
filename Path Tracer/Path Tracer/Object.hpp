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
#include <random>
#include <vector>

#include "Material.hpp"
#include "OBJLoader.hpp"

class Object {
    
public:
    Object(std::string filename, Material mat);
    ~Object();
    std::vector<float> getVertexBuffer();
  
private:
    OBJ *obj;
    Material mat;
    
};

#endif /* Object_hpp */
