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

/*
  Manages storing and writing image data to a file.
*/
class Image {

public:
    /*
      Creates an empty image with dimensions width by height.
      @param width - The width of the image.
      @param height - The height of the image.
    */
    Image(int width, int height);
    /*
      Returns the width of the image.
      @return The width.
    */
    int getWidth();
    /*
      Returns the height of the image.
      @return The height.
    */
    int getHeight();
    /*
      Sets the color of the pixel at (x,y) to the color (r, g, b).
      @param x - The x-coordinate of the pixel.
      @param y - The y-coordinate of the pixel.
      @param r - The red component of the color. In the interval [0, 1].
      @param g - The green component of the color. In the interval [0, 1].
      @param b - The blue component of the color. In the interval [0, 1].
    */
    void setColor(int x, int y, float r, float g, float b);
    /*
      Writes this image in PPM format to the file specified by the filename.
      @param filename - The path to the file.
    */
    void write(std::string filename);

private:
    /*
      The width and height of the image.
    */
    int width, height;
    /*
      The color data of this image in RGB order stored as a float vector.
      data[3*(y*width + x) + componentOffset] is the value of the color
        component for the pixel at (x,y). The componentOffset is 0 for red,
        1 for green, and 2 for blue.
    */
    std::vector<float> data;
};

#endif /* Image_hpp */
