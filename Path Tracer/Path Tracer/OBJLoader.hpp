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

class OBJ {
    public:
        OBJ(std::string filename);
        std::vector<float>& getVertexBuffer();
    private:
        std::vector<float> vertices;
};

#endif /* OBJLoader_hpp */
