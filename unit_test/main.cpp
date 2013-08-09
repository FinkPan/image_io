#include "image_io.hpp"


int main()
{
    Pbm p1("E:\\temp_image_io\\test_data\\p1.pbm");
    std::cout << "\nfileformat: " << p1.GetFileFormat()
              << "\nwidth: " << p1.GetWidth()
              << "\nheight: " << p1.GetHeight();
    //p1.GetMaxValue();
    Pgm p2("E:\\temp_image_io\\test_data\\p5.pgm");
    std::cout << "\n\nfileformat: " << p2.GetFileFormat()
        << "\nwidth: " << p2.GetWidth()
        << "\nheight: " << p2.GetHeight()
        << "\nmaxvalue: " << p2.GetMaxValue();
    
    return 0;
}