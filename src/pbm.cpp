#include "Pbm.hpp"

void Pbm::Read(const std::string &filepath)
{
    std::ifstream filedata(filepath,std::ios_base::in | std::ios_base::binary);
    if (filedata)
    {
            std::string tempstr;
            //��ȡ�ļ���ʽ
            filedata >> file_format_;
            //����ע��
            SkipComments(filedata);
            //��ȡwidth
            filedata >> width_;
            //����ע��
            SkipComments(filedata);
            //��ȡHeight
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
    int cntwidth = 1;	//��ȼ�����
    int cntchar = 0;	//�ַ�λ��������
    filedata.seekg(offset);
    for (int i = 0; i != width_ * height_; ++i)	//ѭ�� nwidth * nheight ��
    {
//         filedata >> bp1;
//         if (bp1)	//bp1Ϊ��ʱд������
//             cp1 = cp1 | (0x80 >> cntchar);
//         if (cntchar == 7)//����8λʱ����cntchar.����cp1����vec_matrix
//         {
//             cntchar=0;	//λ����
//             ++cntwidth;	//�������
//             matrix.push_back(cp1);	//����vec_matrix
//             cp1 = 0x00;	//����cp1
//         }
//         else if (cntwidth == width)		//������ʱ����cntchar��cntwidth.����cp1����vec_matrix
//         {
//             cntchar = 0;	//λ����
//             cntwidth = 1;	//�������
//             matrix.push_back(cp1);	////����vec_matrix
//             cp1 = 0x00;	//����cp1
//         }
//         else	//��λδ��8λ���߿��δ��nwidthʱ, λ�������Ϳ�ȼ���������
//         {
//             ++cntchar;
//             ++cntwidth;
//         }



    }
}

void Pbm::ReadMatrixP4(std::ifstream&,int offset)
{

}