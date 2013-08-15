#include "pgm.hpp"
#include <iterator>
#include <fstream>
#include <algorithm>

void Pgm::ReadImage(const std::string &filename)
{
    std::ifstream filedata;
    filedata.open(filename,std::ios_base::in | std::ios_base::binary);
    if (filedata)
    {
        std::string tempstr;
        filedata >> file_format_;   //读取文件格式
        SkipComments(filedata);
        filedata >> width_;         //读取宽度
        SkipComments(filedata);
        filedata >> height_;        //读取高度
        SkipComments(filedata);
        filedata >> maxvalue_;      //读取最大值   
        SkipComments(filedata);

        if (maxvalue_ > 255)
        {
            std::cout << "PGM文件最大值大于255,程序大于255灰度值不支持!" << std::endl;
            exit(1);
        }

        if (file_format_ == "P2")
            ReadMatrixP2(filedata);
        else if(file_format_ == "P5")
            ReadMatrixP5(filedata);
        else
            std::cout << "PGM文件格式: "<< file_format_ 
                      << " 非法!" << std::endl;

        filedata.close();
    }
    else
        std::cout << "读取文件( " << filename <<  " )失败!\n";
}

void Pgm::ReadMatrixP2(std::ifstream &filedata)
{
    std::istream_iterator<int> in_iterator(filedata);  //直接输出matrix_byte_
    std::istream_iterator<int> eof;
    std::copy(in_iterator,eof,back_inserter(matrix_byte_));  
}

void Pgm::ReadMatrixP5(std::ifstream &filedata)
{
    std::istream_iterator<int8_t> in_iterator(filedata);  //直接输出matrix_byte_
    std::istream_iterator<int8_t> eof;
    std::copy(in_iterator,eof,back_inserter(matrix_byte_));
}

void Pgm::WriteImage(const std::string &filepath, bool bBinary)
{
    std::ofstream ofiledata(filepath,std::ios_base::out | std::ios_base::binary);
    if (ofiledata)
    {
        if (bBinary)
        {
            ofiledata << "P5" << std::endl;
            //输出宽度和高度
            ofiledata << width_ << " " << height_ << std::endl;
            ofiledata << maxvalue_ << std::endl;	//所有的最大值都为255
            std::ostream_iterator<int8_t> out_iterator(ofiledata);  //直接输出matrix_byte_
            std::copy(matrix_byte_.begin(),matrix_byte_.end(),out_iterator);
        }
        else    //输出文本格式PGM
        {
            ofiledata << "P2" << std::endl;
            //输出宽度和高度
            ofiledata << width_ << " " << height_ << std::endl;
            ofiledata << maxvalue_ << std::endl;	//所有的最大值都为255
            BinaryMatrixToTextMatrix();
            int nenter = 0;
            for (int i = 0; i != matrix_int_.size(); ++i)
            {
                ofiledata.width(3);	//文本文件设置宽度为3,对齐数字比较整齐
                ofiledata << std::left << matrix_int_[i];
                nenter = i +1;	
                    if ((nenter % width_ == 0) && (nenter != matrix_int_.size()))
                    {
                        ofiledata << std::endl;
                    }
                    else if(nenter == matrix_int_.size())
                        ;
                    else
                        ofiledata << " ";
            }
        }
        ofiledata.close();
    }
    else
        std::cout << "\n无法打开: " << filepath << "\n";
}

void Pgm::BinaryMatrixToTextMatrix()
{
    matrix_int_.resize(matrix_byte_.size());
    for (size_t i = 0; i != matrix_byte_.size(); ++i)
        matrix_int_[i] = matrix_byte_[i] & 0xff;
}

int Pgm::GetPixelValue(int row, int column) const
{
    return matrix_byte_[row+column*width_];
}

void Pgm::SetPixelValue(int row, int column,const int setvalue)
{
    if (setvalue > maxvalue_)
    {
        std::cout << "设置的值: " << setvalue << " 大于最大值: " << maxvalue_ << std::endl;
    }
    else
        matrix_byte_[row+column*width_] = setvalue;
}