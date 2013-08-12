#include "Pbm.hpp"
#include <cstdlib>

void Pbm::ReadImage(std::ifstream &filedata)
{
    std::string tempstr;
    //读取文件格式
    filedata >> file_format_;
    //处理注释
    SkipComments(filedata);
    //读取width
    filedata >> width_;
    //处理注释
    SkipComments(filedata);
    //读取Height
    filedata >> height_;
    SkipComments(filedata);

    if (file_format_ == "P1" || file_format_ == "p1")
    {
        ReadMatrixP1(filedata);
    }
    else if (file_format_ == "P4" || file_format_ == "p4")
    {
            ReadMatrixP4(filedata);
    }
    else
    {
        std::cout << "file format err.\n";
    }

}

void Pbm::ReadMatrixP1(std::ifstream &filedata)
{
    int bp1 = 0;
    char cp1 = 0x00;
    int cntwidth = 1;	//宽度计数机
    int cntchar = 0;	//字符位数计数机
    for (int i = 0; i != width_ * height_; ++i)	//循环 nwidth * nheight 次
    {
        filedata >> bp1;
        if (bp1)	//bp1为真时写入数据
            cp1 = cp1 | (0x80 >> cntchar);
        if (cntchar == 7)//当满8位时重置cntchar.并将cp1插入vec_matrix
        {
            cntchar=0;	//位重置
            ++cntwidth;	//宽度自增
            matrix_.push_back(cp1);	//插入vec_matrix
            cp1 = 0x00;	//更新cp1
        }
        else if (cntwidth == width_)		//到达宽度时重置cntchar和cntwidth.并将cp1插入vec_matrix
        {
            cntchar = 0;	//位重置
            cntwidth = 1;	//宽度重置
            matrix_.push_back(cp1);	////插入vec_matrix
            cp1 = 0x00;	//更新cp1
        }
        else	//当位未满8位或者宽度未满nwidth时, 位计数机和宽度计数机自增
        {
            ++cntchar;
            ++cntwidth;
        }

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
    int8_t n8data = 0x00;
    for (size_t i = 0; i != sizematrix; ++i)
    {
        filedata >> n8data;
        matrix_.push_back(n8data);
    }
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
            for (int i = 0; i != matrix_.size(); ++i)
                ofiledata.put(static_cast<char>(matrix_[i]));
        }
        else
        {
            ofiledata << "P1" << std::endl;
            //输出宽度和高度
            ofiledata << width_ << " " << height_ << std::endl;
            BinaryMatrixToTextMatrix();	//将二进制Matrix转为文本Matrix
            int nenter = 0;
            for (int i = 0; i != textmatrix_.size(); ++i)
            {
                ofiledata << textmatrix_[i];
                nenter = i + 1;
                //宽度输出回车,否则输出空白符.
                if ((nenter % width_ == 0) && (nenter != textmatrix_.size()))
                {
                    ofiledata << std::endl;
                }
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
    int nbitwidth = 0;	//行标记,如果行nbitwidth大于nwidth,忽略后面行;
    size_t nsize = matrix_.size();
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