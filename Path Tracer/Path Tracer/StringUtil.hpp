//
//  StringUtil.hpp
//  Path Tracer
//
//  Created by Jeremy Berchtold on 12/24/19.
//  Copyright © 2019 Jeremy Berchtold. All rights reserved.
//

#ifndef StringUtil_hpp
#define StringUtil_hpp

#include <string>
#include <vector>

/*
 Splits a string into a vector using the given delimiter.
  s - the string to be split
  delim - the delimiter
 */
std::vector<std::string> split(std::string s, char delim);

#endif /* StringUtil_hpp */
