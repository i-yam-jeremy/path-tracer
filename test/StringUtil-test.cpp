#include "gtest/gtest.h"
#include "StringUtil.hpp"

TEST(StringUtil, test1) {
    //arrange
    std::vector<std::string> v;
    v.push_back("a");
    v.push_back("hi");
    v.push_back("ba%% 32");
    v.push_back(" def");
    //act
    auto out = stringutil::split("a,hi,ba%% 32, def", ',');
    //assert
    EXPECT_EQ(out.size(), v.size());
    for (size_t i = 0; i < v.size(); i++) {
        EXPECT_EQ(out[i], v[i]);
    }
}