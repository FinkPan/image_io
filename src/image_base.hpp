#ifndef _IMAGE_BASE_HPP_
#define _IMAGE_BASE_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdint>  //����һ��8λ��int8_t
#include <sstream>


class Image_base
{
public:
    typedef std::vector<int8_t>  Image1byteMatrix;
    typedef std::vector<int16_t> Image2byteMatrix;

    Image_base():width_(0),height_(0){}
    virtual void ReadImage(const std::string&)=0;
    virtual void WriteImage(const std::string&,bool bBinary = true)=0;
    
    std::string GetFileFormat(){ return file_format_; }
    int GetWidth(){ return width_; }
    int GetHeight(){ return height_; }

protected:
    virtual void BinaryMatrixToTextMatrix() = 0;
    void SkipComments(std::ifstream&);
    std::string file_format_;	        //�ļ���ʽ
    int width_;				            //���
    int height_;			            //�߶�
    Image1byteMatrix matrix_byte_;	   //BinaryMatrix
    Image2byteMatrix matrix_int_;      //TextMatrix
};

#endif