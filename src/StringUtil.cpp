//
//  StringUtil.cpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/24/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#include "StringUtil.hpp"

#include <sstream>

std::vector<std::string> split(std::string s, char delim) {
    std::stringstream stream(s);
    std::string item;
    std::vector<std::string> split;
    while (std::getline(stream, item, delim)) {
        split.push_back(item);
    }
    return split;
}
