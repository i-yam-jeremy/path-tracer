//
//  OBJLoader.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/18/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "OBJLoader.hpp"

#include <fstream>
#include <iostream>

#include "StringUtil.hpp"

void readVertex(std::vector<float> &indexedVertices, std::vector<std::string> params) {
    if (params.size() != 4 && params.size() != 5) {
        std::cout << "Invalid vertex definition in OBJ file." << std::endl;
        exit(1);
    }

    float x = atof(params[1].c_str());
    float y = atof(params[2].c_str());
    float z = atof(params[3].c_str());
    indexedVertices.push_back(x);
    indexedVertices.push_back(y);
    indexedVertices.push_back(z);

    // Ignore optional w component
}

void readTexCoord(std::vector<float> &indexedTexCoords, std::vector<std::string> params) {
    if (params.size() != 3 && params.size() != 4) {
        std::cout << "Invalid tex coord definition in OBJ file." << std::endl;
        exit(1);
    }

    float u = atof(params[1].c_str());
    float v = atof(params[2].c_str());
    indexedTexCoords.push_back(u);
    indexedTexCoords.push_back(v);

    // Ignore optional w component
}

void readNormal(std::vector<float> &indexedNormals, std::vector<std::string> params) {
    if (params.size() != 4) {
        std::cout << "Invalid normal definition in OBJ file." << std::endl;
        exit(1);
    }

    float x = atof(params[1].c_str());
    float y = atof(params[2].c_str());
    float z = atof(params[3].c_str());
    indexedNormals.push_back(x);
    indexedNormals.push_back(y);
    indexedNormals.push_back(z);
}

void readFace(std::vector<float> &vertices, std::vector<float> indexedVertices, std::vector<float> &texCoords, std::vector<float> indexedTexCoords, std::vector<float> &normals, std::vector<float> indexedNormals, std::vector<std::string> params) {
    if (params.size() < 4) {
        std::cout << "Invalid face definition in OBJ file." << std::endl;
        exit(1);
    }

    if (params.size() > 5) {
        std::cout << "Only triangles and quads are supported. Please remove n-gons from your mesh." << std::endl;
        exit(1);
    }

    std::vector<long> vIndices;
    std::vector<long> texCoordIndices;
    std::vector<long> normalIndices;
    for (int i = 0; i < params.size()-1; i++) {
        std::vector<std::string> vertexData = stringutil::split(params[i+1], '/');
        vIndices.push_back(atoi(vertexData[0].c_str()) - 1);
        texCoordIndices.push_back(atoi(vertexData[1].c_str()) - 1);
        normalIndices.push_back(atoi(vertexData[2].c_str()) - 1);
    }
    if (params.size() == 5) { // Quad
        // Convert quad v1,v2,v3,v4 into triangles v1,v2,v3 and v3,v4,v1
        vIndices.insert(vIndices.begin() + 3, vIndices[2]);
        vIndices.insert(vIndices.end(), vIndices[0]);
        texCoordIndices.insert(texCoordIndices.begin() + 3, texCoordIndices[2]);
        texCoordIndices.insert(texCoordIndices.end(), texCoordIndices[0]);
        normalIndices.insert(normalIndices.begin() + 3, normalIndices[2]);
        normalIndices.insert(normalIndices.end(), normalIndices[0]);
    }

    for (int i = 0; i < vIndices.size(); i++) {
        long vIndex = vIndices[i];
        vertices.push_back(indexedVertices[3*vIndex+0]);
        vertices.push_back(indexedVertices[3*vIndex+1]);
        vertices.push_back(indexedVertices[3*vIndex+2]);

        long texCoordIndex = texCoordIndices[i];
        texCoords.push_back(indexedTexCoords[2*texCoordIndex+0]);
        texCoords.push_back(indexedTexCoords[2*texCoordIndex+1]);

        long normalIndex = normalIndices[i];
        normals.push_back(indexedNormals[3*normalIndex+0]);
        normals.push_back(indexedNormals[3*normalIndex+1]);
        normals.push_back(indexedNormals[3*normalIndex+2]);
    }
}

OBJ::OBJ(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cout << "Could not open OBJ file: " << filename << std::endl;
      exit(1);
    }
    std::string line;
    std::vector<float> indexedVertices;
    std::vector<float> indexedTexCoords;
    std::vector<float> indexedNormals;
    while (std::getline(file, line)) {
        auto params = stringutil::split(line, ' ');
        if (params[0] == "v") {
            readVertex(indexedVertices, params);
        }
        else if (params[0] == "vt") {
            readTexCoord(indexedTexCoords, params);
        }
        else if (params[0] == "vn") {
            readNormal(indexedNormals, params);
        }
        else if (params[0] == "f") {
            readFace(this->vertices, indexedVertices, this->texCoords, indexedTexCoords, this->normals, indexedNormals, params);
        }
    }
}

std::vector<float>& OBJ::getVertexBuffer() {
    return vertices;
}

std::vector<float>& OBJ::getTexCoordBuffer() {
    return texCoords;
}

std::vector<float>& OBJ::getNormalBuffer() {
    return normals;
}
