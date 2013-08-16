#ifndef _PGM_HPP_
#define _PGM_HPP_

#include "image_io.hpp"

class Pgm: public Image_base
{
public:
    Pgm():maxvalue_(0){};
    void ReadImage(const std::string&);
    void WriteImage(const std::string&,bool bBinary = true);

    int  GetMaxValue(){ return maxvalue_; }
    int  GetPixelValue(int row, int column) const;
    void SetPixelValue(int row, int column, const int setvalue);
private:
    void BinaryMatrixToTextMatrix();
    void ReadMatrixP2(std::ifstream&);
    void ReadMatrixP5(std::ifstream&);
    int maxvalue_;
};

#endif