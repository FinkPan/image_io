#include "image_io.hpp"
#include <limits> //std::numeric_limits<std::streamsize>::max()

void Image_base::SkipComments(std::ifstream &inputdata)
{
    char comment;
    inputdata >> comment;
    while(comment == '#')
    {
        inputdata.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        comment = inputdata.get();
    }
    inputdata.unget();

}