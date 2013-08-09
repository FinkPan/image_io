#ifndef _IMAGE_BASE_HPP_
#define _IMAGE_BASE_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

enum imageType
{
    PBM_P1 = 1,      
    PGM_P2,     
    PPM_P3,     
    PBM_P4,
    PGM_P5,
    PPM_P6
};

class Image_base
{
public:
    Image_base():width_(0),height_(0){}
    virtual void Read(const std::string&)=0;
    void SkipComments(std::ifstream&);

    std::string GetFileFormat(){ return file_format_; }
    int GetWidth(){ return width_; }
    int GetHeight(){ return height_; }

protected:
    std::string file_format_;	        //文件格式
    int width_;				            //宽度
    int height_;			                //高度
    std::vector<char> matrix_;	        //matrix
};

#endif