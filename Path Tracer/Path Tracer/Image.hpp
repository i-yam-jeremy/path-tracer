//
//  Image.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef Image_hpp
#define Image_hpp

#include <string>
#include <vector>

#include "vec3.hpp"

class Image {
    
public:
    Image(int width, int height);
    int getWidth();
    int getHeight();
    void setColor(int x, int y, vec3 c);
    void write(std::string filename);
    
private:
    int width, height;
    std::vector<float> data;
};

#endif /* Image_hpp */
