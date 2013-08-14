#include "image_io.hpp"
#include "gtest/gtest.h"
#include <fstream>

std::ifstream inputdata;

TEST(image_io,LoadImage)
{
    EXPECT_ANY_THROW(ImageLoad("E:\\git_workspace\\image_io\\build\\p1.pbm",inputdata));
}