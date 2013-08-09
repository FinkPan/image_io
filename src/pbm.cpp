#include "Pbm.hpp"

void Pbm::ReadImage(const std::string &filepath)
{
    std::ifstream filedata(filepath,std::ios_base::in | std::ios_base::binary);
    if (filedata)
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
    else
    {
        std::cout << "open file: " << filepath << "fail.\n";
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

void Pbm::ReadMatrixP4(std::ifstream&)
{

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
            for (int i = 0; i != textmatrix_.size(); ++i)
            {
                ofiledata.width(3);	//文本文件设置宽度为3,对齐数字比较整齐
                ofiledata << textmatrix_[i] << " ";
                int nenter = i +1;	
                if (nenter % width_ == 0)		//为了格式对齐.输出回车行
                    ofiledata << std::endl;
            }
        
        }
         ofiledata.close();
    }
    else
         std::cout << "\n写入PBM文件失败!" << std::endl;
}