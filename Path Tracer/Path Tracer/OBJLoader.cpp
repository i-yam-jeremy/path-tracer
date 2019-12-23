//
//  OBJLoader.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/18/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "OBJLoader.hpp"

#include <fstream>
#include <sstream>

#include <iostream>

/**
 Splits a string into a vector using the given delimiter.
  s - the string to be split
  delim - the delimiter
 */
std::vector<std::string> split(std::string s, char delim) {
    std::stringstream stream(s);
    std::string item;
    std::vector<std::string> split;
    while (std::getline(stream, item, delim)) {
        split.push_back(item);
    }
    return split;
}

void readVertex(std::vector<float> &indexedVertices, std::vector<std::string> params, vec3 pos) {
    if (params.size() != 4 && params.size() != 5) {
        //TODO error/exception
    }
    
    float x = atof(params[1].c_str());
    float y = atof(params[2].c_str());
    float z = atof(params[3].c_str());
    indexedVertices.push_back(x + pos.x);
    indexedVertices.push_back(y + pos.y);
    indexedVertices.push_back(z + pos.z);
    if (params.size() == 5) {
        indexedVertices.push_back(atof(params[4].c_str()));
    }
}

void readFace(std::vector<float> &vertices, std::vector<float> &indexedVertices, std::vector<std::string> params) {
    if (params.size() < 4) {
        // TODO error/exception
    }
    
    if (params.size() > 4) {
        std::cout << "Only triangles currently supported" << std::endl;
        exit(1);
    }

    for (int i = 0; i < params.size()-1; i++) {
        std::vector<std::string> vertexData = split(params[i+1], '/');
        long vIndex = atoi(vertexData[0].c_str()) - 1;
        vertices.push_back(indexedVertices[3*vIndex+0]);
        vertices.push_back(indexedVertices[3*vIndex+1]);
        vertices.push_back(indexedVertices[3*vIndex+2]);
    }
}

OBJ::OBJ(std::string filename, vec3 pos) {
    std::ifstream file(filename);
    std::string line;
    std::vector<float> indexedVertices;
    while (std::getline(file, line)) {
        auto params = split(line, ' ');
        if (params[0] == "v") {
            readVertex(indexedVertices, params, pos);
        }
        else if (params[0] == "f") {
            readFace(this->vertices, indexedVertices, params);
        }
    }
}
        
std::vector<float>& OBJ::getVertexBuffer() {
    return vertices;
}
