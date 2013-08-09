#include "Pbm.hpp"

void Pbm::Read(const std::string &filepath)
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
            
            int offset = filedata.tellg();

            if (file_format_ == "p1" || file_format_ == "P1")
            {
                ReadMatrixP1(filedata,offset);
            }
            else if (file_format_ == "p4" || file_format_ == "P4")
            {

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

void Pbm::ReadMatrixP1(std::ifstream &filedata,int offset)
{
    int bp1 = 0;
    char cp1 = 0x00;
    int cntwidth = 1;	//宽度计数机
    int cntchar = 0;	//字符位数计数机
    filedata.seekg(offset);
    for (int i = 0; i != width_ * height_; ++i)	//循环 nwidth * nheight 次
    {
//         filedata >> bp1;
//         if (bp1)	//bp1为真时写入数据
//             cp1 = cp1 | (0x80 >> cntchar);
//         if (cntchar == 7)//当满8位时重置cntchar.并将cp1插入vec_matrix
//         {
//             cntchar=0;	//位重置
//             ++cntwidth;	//宽度自增
//             matrix.push_back(cp1);	//插入vec_matrix
//             cp1 = 0x00;	//更新cp1
//         }
//         else if (cntwidth == width)		//到达宽度时重置cntchar和cntwidth.并将cp1插入vec_matrix
//         {
//             cntchar = 0;	//位重置
//             cntwidth = 1;	//宽度重置
//             matrix.push_back(cp1);	////插入vec_matrix
//             cp1 = 0x00;	//更新cp1
//         }
//         else	//当位未满8位或者宽度未满nwidth时, 位计数机和宽度计数机自增
//         {
//             ++cntchar;
//             ++cntwidth;
//         }



    }
}

void Pbm::ReadMatrixP4(std::ifstream&,int offset)
{

}