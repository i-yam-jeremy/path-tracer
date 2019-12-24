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
  Utility functions for processing strings.
*/
namespace stringutil {
  /*
    Splits a string into a vector using the given delimiter.
    @param s - The string to be split.
    @param delim - The delimiter.
    @return A vector containing the split strings.
   */
  std::vector<std::string> split(std::string s, char delim);
};

#endif /* StringUtil_hpp */
