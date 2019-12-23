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

class Object {
    
public:
    Object(std::string filename, Material material);
    ~Object();
    std::vector<float> getVertexBuffer();
    std::vector<float> getTexCoordBuffer();
    std::vector<float> getNormalBuffer();
    Material getMaterial();
  
private:
    OBJ *obj;
    Material material;
    
};

#endif /* Object_hpp */
