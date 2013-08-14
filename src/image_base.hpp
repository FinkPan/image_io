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
    typedef std::vector<int8_t> ImageBinaryMatrix;
    typedef std::vector<int> ImageTextMatrix;

    Image_base():width_(0),height_(0){}
    virtual void ReadImage(std::ifstream&)=0;
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
    ImageBinaryMatrix matrix_;	        //BinaryMatrix
    ImageTextMatrix   textmatrix_;      //TextMatrix
};

#endif