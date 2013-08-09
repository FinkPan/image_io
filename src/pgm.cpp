#include "pgm.hpp"

void Pgm::Read(const std::string &filepath)
{
    std::ifstream filedata(filepath,std::ios_base::in | std::ios_base::binary);
    if (filedata)
    {
        std::string tempstr;
        filedata >> file_format_;   //��ȡ�ļ���ʽ
        SkipComments(filedata);
        filedata >> width_;         //��ȡ���
        SkipComments(filedata);
        filedata >> height_;        //��ȡ�߶�
        SkipComments(filedata);
        filedata >> maxvalue_;      //��ȡ���ֵ   
        SkipComments(filedata);

        int offset = filedata.tellg(); //��־����λ��
        if (file_format_ == "p2" || file_format_ == "P2")
            ReadMatrixP2(filedata,offset);
        else if(file_format_ == "p5" || file_format_ == "P5")
            ReadMatrixP5(filedata,offset);
        else
            std::cout << "\n��ȡPGM�ļ�ʧ��!" << std::endl;
    }
    else
        std::cout << "\n�޷���: " << filepath << "\n";
}

void Pgm::ReadMatrixP2(std::ifstream&,int offset)
{
    std::cout << "\n��ȡP2�ļ�!" << std::endl;



}
void Pgm::ReadMatrixP5(std::ifstream&,int offset)
{
    std::cout << "\n��ȡP5�ļ�!" << std::endl;



}
