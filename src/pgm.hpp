#ifndef _PGM_HPP_
#define _PGM_HPP_

#include "image_io.hpp"

class Pgm: public Image_base
{

public:
    Pgm(const std::string &filepath)
    {
        Read(filepath);
    }
    int GetMaxValue(){ return maxvalue_; }
private:
    void Read(const std::string&);
    void ReadMatrixP2(std::ifstream&,int offset);
    void ReadMatrixP5(std::ifstream&,int offset);
    int maxvalue_;
};

#endif