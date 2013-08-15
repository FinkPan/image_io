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
    struct PPMRGText
    {
        int R;
        int G;
        int B;
    };

    typedef std::vector<PPMRGBbinary> PPMBinaryMatrix;
    typedef std::vector<PPMRGText> PPMTextMatrix;

    Ppm(std::ifstream &filedata):maxvalue_(0)
    {
        ReadImage(filedata);
    }
    int GetMaxValue(){ return maxvalue_; }
    void WriteImage(const std::string&,bool bBinary = true);
private:
    void BinaryMatrixToTextMatrix();
    void ReadImage(std::ifstream&);
    void ReadMatrixP3(std::ifstream&);
    void ReadMatrixP6(std::ifstream&);
    int maxvalue_;
    PPMBinaryMatrix matrix_byte_;
    PPMTextMatrix matrix_int_;
};

#endif