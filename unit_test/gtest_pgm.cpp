#include "image_io.hpp"
#include "gtest/gtest.h"
#include <fstream>

Pgm PGM1;

TEST(Pgm,ReadImage)
{
    PGM1.ReadImage("E:\\git_workspace\\image_io\\build\\apps\\p2.pgm");
}

TEST(Pgm,GetMaxValue)
{
    EXPECT_EQ(15,PGM1.GetMaxValue());
}
TEST(Pgm,GetFileFormat)
{
    EXPECT_EQ("P2",PGM1.GetFileFormat());
}
TEST(Pgm,GetWidth)
{
    EXPECT_EQ(24,PGM1.GetWidth());
}
TEST(Pgm,GetHeight)
{
    EXPECT_EQ(7,PGM1.GetHeight());
}
TEST(Pgm,GetPixelValue)
{
    EXPECT_EQ(4,PGM1.GetPixelValue(0,3));
}

// TEST(Pgm,SetPixelValue)
// {
//     PGM1.SetPixelValue(0,3,15);
//     EXPECT_EQ(15,PGM1.GetPixelValue(0,3));
// }

TEST(Pgm,WriteImage)
{
    PGM1.WriteImage("E:\\git_workspace\\image_io\\build\\apps\\p2_out_gtest.pgm",false);
}