#ifndef _PBM_HPP_
#define _PBM_HPP_

#include "image_io.hpp"

class Pbm: public Image_base
{

public:
    Pbm(const std::string &filename)
    {
        ReadImage(filename);
    }
    void WriteImage(const std::string&,bool bBinary = true);
    void ReadImage(const std::string&);
private:
    void BinaryMatrixToTextMatrix();
    void ReadMatrixP1(std::ifstream&);
    void ReadMatrixP4(std::ifstream&);
};

#endif