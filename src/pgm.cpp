#include "pgm.hpp"

void Pgm::Read(const std::string &filepath)
{
    std::ifstream filedata(filepath,std::ios_base::in | std::ios_base::binary);
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

        int offset = filedata.tellg(); //标志数据位置
        if (file_format_ == "p2" || file_format_ == "P2")
            ReadMatrixP2(filedata,offset);
        else if(file_format_ == "p5" || file_format_ == "P5")
            ReadMatrixP5(filedata,offset);
        else
            std::cout << "\n读取PGM文件失败!" << std::endl;
    }
    else
        std::cout << "\n无法打开: " << filepath << "\n";
}

void Pgm::ReadMatrixP2(std::ifstream&,int offset)
{
    std::cout << "\n读取P2文件!" << std::endl;



}
void Pgm::ReadMatrixP5(std::ifstream&,int offset)
{
    std::cout << "\n读取P5文件!" << std::endl;



}
