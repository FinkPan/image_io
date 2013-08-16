#include "image_io.hpp"

int main(int argc, char **argv)
{
    Ppm myppm;
    myppm.ReadImage("E:\\temp_image_io\\test_data\\p3.ppm");
    
    Ppm::PPMRGText p1;
    p1 = myppm.GetPixelValue(1,1);
    std::cout << p1.R << " " << p1.G << " " << p1.B;

    myppm.SetPixelValue(1,1,63,203,201);
    p1 = myppm.GetPixelValue(1,1);
    std::cout << p1.R << " " << p1.G << " " << p1.B;
    myppm.WriteImage("E:\\temp_image_io\\test_data\\p3_out.ppm",true);
}