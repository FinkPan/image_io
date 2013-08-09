#include "image_io.hpp"


int main()
{
    Pbm p1("E:\\git_workspace\\image_io\\build\\test_data\\p1.pgm");
    std::cout << "\nfileformat: " << p1.GetFileFormat()
              << "\nwidth: " << p1.GetWidth()
              << "\nheight: " << p1.GetHeight();
    p1.WriteImage("E:\\git_workspace\\image_io\\build\\test_data\\p1_out.pgm",false);
    p1.WriteImage("E:\\git_workspace\\image_io\\build\\test_data\\p4_out.pgm");
    Pgm p2("E:\\git_workspace\\image_io\\build\\test_data\\p2.pgm");
    std::cout << "\n\nfileformat: " << p2.GetFileFormat()
        << "\nwidth: " << p2.GetWidth()
        << "\nheight: " << p2.GetHeight()
        << "\nmaxvalue: " << p2.GetMaxValue();
    p2.WriteImage("E:\\git_workspace\\image_io\\build\\test_data\\p2_out.pgm",false);
    p2.WriteImage("E:\\git_workspace\\image_io\\build\\test_data\\p5_out.pgm");
    return 0;
}