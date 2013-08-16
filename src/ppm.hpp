#ifndef _PPM_HPP_
#define _PPM_HPP_

#include "image_io.hpp"

class Ppm: public Image_base
{

public:
    struct PPMRGBbinary
    {
        int8_t R;
        int8_t G;
        int8_t B;
    };
    struct PPMRGBText
    {
        int16_t R;
        int16_t G;
        int16_t B;
    };

    typedef std::vector<PPMRGBbinary> PPMBinaryMatrix;
    typedef std::vector<PPMRGBText> PPMTextMatrix;

    Ppm():maxvalue_(0){}
    void ReadImage(const std::string&);
    void WriteImage(const std::string&,bool bBinary = true);

    int GetMaxValue(){ return maxvalue_; }
    PPMRGBText GetPixelValue(const unsigned int row,const unsigned int column);
    void SetPixelValue(const int row,const int column,int R,int G,int B);

private:
    void BinaryMatrixToTextMatrix();
    
    void ReadMatrixP3(std::ifstream&);
    void ReadMatrixP6(std::ifstream&);
    int maxvalue_;
    PPMBinaryMatrix matrix_byte_;
    PPMTextMatrix matrix_int_;
};

#endif