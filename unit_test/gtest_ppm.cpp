#include "image_io.hpp"
#include "gtest/gtest.h"
#include <fstream>

Ppm PPM1;

TEST(Ppm,ReadImage)
{
    PPM1.ReadImage("E:\\git_workspace\\image_io\\build\\apps\\p3.ppm");
}

TEST(Ppm,GetMaxValue)
{
    EXPECT_EQ(255,PPM1.GetMaxValue());
}
TEST(Ppm,GetFileFormat)
{
    EXPECT_EQ("P3",PPM1.GetFileFormat());
}
TEST(Ppm,GetWidth)
{
    EXPECT_EQ(3,PPM1.GetWidth());
}
TEST(Ppm,GetHeight)
{
    EXPECT_EQ(2,PPM1.GetHeight());
}
TEST(Ppm,GetPixelValue)
{
    EXPECT_EQ(0,PPM1.GetPixelValue(0,2).R);
    EXPECT_EQ(0,PPM1.GetPixelValue(0,0).G);
}

TEST(Ppm,SetPixelValue)
{
    PPM1.SetPixelValue(0,2,23,23,23);
    EXPECT_EQ(23,PPM1.GetPixelValue(0,2).R);
}

TEST(Ppm,WriteImage)
{
    PPM1.WriteImage("E:\\git_workspace\\image_io\\build\\apps\\p3_out_gtest.ppm",false);
}