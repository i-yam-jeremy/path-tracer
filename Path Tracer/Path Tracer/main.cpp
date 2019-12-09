//
//  main.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/8/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include <iostream>

#include "Image.hpp"

int main(int argc, const char * argv[]) {
    
    Image image(512, 512);
    
    for (int y = 0; y < 512; y++) {
        for (int x = 0; x < 512; x++) {
            image.setColor(x, y, float(x)/512.0, float(y)/512.0, 0.0);
        }
    }
    
    image.write("/Users/i-yam-jeremy/Desktop/out.ppm");
    
    std::cout << "Done\n";
    return 0;
}
