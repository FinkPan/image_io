#ifndef _PBM_HPP_
#define _PBM_HPP_

#include "image_io.hpp"

class Pbm: public Image_base
{

public:
    Pbm(std::ifstream &filedata)
    {
        ReadImage(filedata);
    }
    void WriteImage(const std::string&,bool bBinary = true);
private:
    void BinaryMatrixToTextMatrix();
    void ReadImage(std::ifstream&);
    void ReadMatrixP1(std::ifstream&);
    void ReadMatrixP4(std::ifstream&);
};

#endif