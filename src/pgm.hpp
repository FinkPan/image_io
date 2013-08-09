#ifndef _PGM_HPP_
#define _PGM_HPP_

#include "image_io.hpp"

class Pgm: public Image_base
{

public:
    Pgm(const std::string &filepath):maxvalue_(0)
    {
        ReadImage(filepath);
    }
    int GetMaxValue(){ return maxvalue_; }
    void WriteImage(const std::string&,bool bBinary = true);
private:
    void ReadImage(const std::string&);
    void ReadMatrixP2(std::ifstream&);
    void ReadMatrixP5(std::ifstream&);
    int maxvalue_;
};

#endif