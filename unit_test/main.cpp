#include "image_io.hpp"
#include <iostream>
#include <string>

int main()
{
	bpm ima;
    std::string filename("test_data/p1.pbm");
    ima.ReadFile(filename);
    ima.WriteFile(filename);
	return 0;
}