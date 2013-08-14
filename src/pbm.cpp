#include "Pbm.hpp"
#include <cstdlib>
#include <iterator>
#include <fstream>
#include <bitset>


void Pbm::ReadImage(const std::string &filename)
{
    std::ifstream filedata;
    filedata.open(filename,std::ios_base::in | std::ios_base::binary);
    if (filedata)
    {
        filedata >> file_format_; //读取文件格式
        SkipComments(filedata);   //处理注释
        filedata >> width_;       //读取width
        SkipComments(filedata);
        filedata >> height_;      //读取Height
        SkipComments(filedata);

        if (file_format_ == "P1" || file_format_ == "p1")
            ReadMatrixP1(filedata);
        else if (file_format_ == "P4" || file_format_ == "p4")
            ReadMatrixP4(filedata);
        else
            std::cout << "文件格式错误.\n";
    }
    else
        std::cout << "读取文件( " << filename <<  " )失败!\n";

}

void Pbm::ReadMatrixP1(std::ifstream &filedata)
{
    bool bdata = 0;
    std::bitset<8> bitbyte;
    int8_t n8data = 0x00;
    for (size_t i = 0; i < height_; ++i)
    {
        for (size_t j = 0, bitoff = 7; j < width_; ++j, --bitoff)
        {
            filedata >> bdata;
            if (bitoff == 0) //bitoff满8位时转为ini8_t
            {
                bitbyte[bitoff] = bdata;
                n8data = bitbyte.to_ulong();
                matrix_.push_back(n8data);

                bitoff = 8;     //重置bitoff,并重置bitbyte.
                bitbyte.reset();
                continue;
            }
            bitbyte[bitoff] = bdata; //bitoff不满8位且j未达到width_时写入matrix_
        }
        n8data = bitbyte.to_ulong(); //bitoff不满8位但j达到width_时写入matrix_
        matrix_.push_back(n8data);
    }
}

void Pbm::ReadMatrixP4(std::ifstream &filedata)
{
    int column_width = 0;
    /**************由width_计算出宽需要多少个字符*********/
    div_t divresult;
    divresult = div (width_,8);
    int nquot = divresult.quot;
    int nrem = divresult.rem;
    if (divresult.quot == 0)
        column_width = 1;
    else if (divresult.rem == 0)
        column_width = divresult.quot;
    else
        column_width = divresult.quot + 1;
    /**************由width_计算出宽需要多少个字符*********/

    size_t sizematrix = column_width*height_;
    std::istream_iterator<int8_t> in_iterator(filedata);  //直接输出matrix_
    std::istream_iterator<int8_t> eof;
    std::copy(in_iterator,eof,back_inserter(matrix_));

}

void Pbm::WriteImage(const std::string &filepath,bool bBinary)
{
    std::ofstream ofiledata(filepath,std::ios_base::out | std::ios_base::binary);
    if (ofiledata)
    {
        if (bBinary)
        {
            ofiledata << "P4" << std::endl;
            //输出宽度和高度
            ofiledata << width_ << " " << height_ << std::endl;
            std::ostream_iterator<int8_t> out_iterator(ofiledata);  //直接输出matrix_
            std::copy(matrix_.begin(),matrix_.end(),out_iterator);
        }
        else
        {
            BinaryMatrixToTextMatrix();	//将二进制Matrix转为文本Matrix
            ofiledata << "P1" << "\n";
            //输出宽度和高度
            ofiledata << width_ << " " << height_ << "\n";
            int nenter = 0;
            for (int i = 0; i != textmatrix_.size(); ++i)
            {
                ofiledata << textmatrix_[i];
                nenter = i + 1;
                //宽度输出回车,否则输出空白符,文件结尾什么都不输出
                if ((nenter % width_ == 0) && (nenter != textmatrix_.size()))
                    ofiledata << "\n";
                else if(nenter == textmatrix_.size())
                    ;
                else
                    ofiledata << " ";
            }
        }
         ofiledata.close();
    }
    else
         std::cout << "\n写入PBM文件失败!" << std::endl;
}

void Pbm::BinaryMatrixToTextMatrix()
{
    for (size_t i = 0, index = 0; i < height_; ++i,++index) //共有height_行
    {
        for (int j = 0,bitoff = 0; j < width_; ++j,++bitoff) //共有width_列
        {
            if (bitoff > 7) //位大于7重置为0
            {
                bitoff = 0;
                ++index; 
            }
            if(matrix_[index] & (0x80 >> bitoff))	
                textmatrix_.push_back(1);
            else
                textmatrix_.push_back(0);
        }
    }
}