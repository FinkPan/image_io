#ifndef _IMAGE_IO
#define _IMAGE_IO

#include <string>
#include <vector>

class image_io;
class bpm;

class image_io
{
public:
    image_io():width(0), height(0),max_value(0){}
    bool ReadFile(const std::string&);
    bool WriteFile(const std::string&,bool bBinary);
private:
    std::string str_file_format;	//文件格式
    std::string str_comment;		//文件注释
    std::vector<std::string> vec_comment;
    int width;				//宽度
    int height;			//高度
    int max_value;			//最大值
    std::vector<char> vec_matrix;	//matrix
    std::vector<int> vec_text_matrix;	//文本Matrix 当需要输出文本格式文件时可用
private:
    //将VecBinaryMatrix转为ToVecTextMatrix
    void VecMatrixToVecTextMatrix();
};

#endif