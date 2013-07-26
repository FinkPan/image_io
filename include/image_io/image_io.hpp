#ifndef _IMAGE_IO
#define _IMAGE_IO

#include <string>
#include <vector>

class image_io;
class bpm;

class image_io
{
public:
    image_io():width_(0), height_(0),max_value_(0){}
    bool ReadFile(const std::string&);
    bool WriteFile(const std::string&,bool bBinary);
private:
    std::string file_format_;	//文件格式
    std::string comment_;		//文件注释
    std::vector<std::string> vec_comment_;
    int width_;				//宽度
    int height_;			//高度
    int max_value_;			//最大值
    std::vector<char> vec_matrix_;	//matrix
    std::vector<int> vec_text_matrix_;	//文本Matrix 当需要输出文本格式文件时可用
private:
    //将VecBinaryMatrix转为ToVecTextMatrix
    void VecMatrixToVecTextMatrix();
};

#endif