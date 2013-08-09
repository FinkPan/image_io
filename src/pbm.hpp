#ifndef _PBM_HPP_
#define _PBM_HPP_

#include "image_io.hpp"

class Pbm: public Image_base
{

public:
    Pbm(const std::string &filepath)
    {
        Read(filepath);
    }
private:
    void Read(const std::string&);
    void ReadMatrixP1(std::ifstream&,int offset);
    void ReadMatrixP4(std::ifstream&,int offset);
};

#endif