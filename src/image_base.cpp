#include "image_io.hpp"
#include <limits> //std::numeric_limits<std::streamsize>::max()

void Image_base::SkipComments(std::ifstream &inputdata)
{
    char comment;
    inputdata >> comment;
    while(comment == '#')
    {
        inputdata.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        comment = inputdata.get();
    }
    inputdata.unget();

}

void Image_base::BinaryMatrixToTextMatrix()
{
    size_t nsize = matrix_.size();
    if (file_format_ == "P4" | file_format_ == "P1")	//P1,P4文件格式: 一个像素占1位,8个像素占1个字节.
    {
        int nbitwidth;	//行标记,如果行nbitwidth大于nwidth,忽略后面行;
        for (size_t i = 0; i < nsize; ++i)
        {
            nbitwidth = 0;	//重置行
            for (int j = 0; j < 8; j++)
            {
                //将一个字节每一位和matrix_[i] 与 运算,如果为真,matrix_[i],写入1,否则写入0
                if((nbitwidth < width_) && matrix_[i] & (0x80 >> j))		
                {
                    textmatrix_.push_back(1);
                    ++nbitwidth;
                }
                else if((nbitwidth < width_))
                {
                    textmatrix_.push_back(0);
                    ++nbitwidth;
                }
                else	//行nbitwidth大于nwidth,忽略后面行;
                    break;	
            }
        }
    }
    else	//P5,P6文件格式: 一个像素占1个字节
    {
        textmatrix_.resize(nsize);
        int ndec,nhex;
        int nmark = 0x00ff;	//屏蔽高8位数
        for (size_t i = 0; i < nsize; ++i)
        {
            nhex = matrix_[i];
            ndec = nhex & nmark;	//屏蔽高8位数
            textmatrix_[i] = ndec;
        }
    }
}