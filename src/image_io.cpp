#include "image_io.hpp"

std::string ImageLoad(const std::string &filename,std::ifstream &filedata)
{
    std::string fileformat;
    filedata.open(filename,std::ios_base::in | std::ios_base::binary);
    if (filedata)
    {
        filedata >> fileformat;
        filedata.seekg(0,filedata.beg);
        return fileformat;
    }
    else
    {
        std::cout << "¶ÁÈ¡ÎÄ¼þÊ§°Ü!\n";
        exit(1);
    }
}