#include "gtest/gtest.h"
#include "OBJLoader.hpp"

#include <fstream>
#include <string>
#include <filesystem>

TEST(OBJLoader, test1) {
    //arrange
    std::string objPath = std::filesystem::temp_directory_path().string() + "/test.obj";
    std::ofstream out(objPath);
    out << R"(
# TEST COMMENT
mtllib test7.mtl
o Plane
v -1.0 -1.0 0.0
v 1.0 -1.0 0.0
v 1.0 1.0 0.0
v -1.0 1.0 0.0
vt 0.000000 0.000000
vt 1.000000 0.000000
vt 1.000000 1.000000
vt 0.000000 1.000000
vn 0 0 1
usemtl None
s off
f 1/1/1 2/2/1 3/3/1 4/4/1
        )";
    out.close();

    std::vector<float> expectedVbuf{
        -1, -1, 0,
         1, -1, 0,
         1,  1, 0,
         1,  1, 0,
        -1,  1, 0,
        -1, -1, 0,
    };

    //act
    OBJ obj(objPath);

    //assert
    auto vbuf = obj.getVertexBuffer();
    EXPECT_EQ(vbuf.size(), expectedVbuf.size());
    for (size_t i = 0; i < vbuf.size(); i++) {
        EXPECT_EQ(vbuf[i], expectedVbuf[i]);
    }
}