#include "Pbm.hpp"
#include <cstdlib>
#include <iterator>
#include <fstream>
#include <bitset>


void Pbm::ReadImage(std::ifstream &filedata)
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
    bool bp1 = 0;
    char cp1 = 0x00;
    int cntwidth = 1;	//��ȼ�����
    int cntchar = 0;	//�ַ�λ��������
//     std::bitset<8> bitbyte;
//     for (size_t i = 0; i < height_; ++i)
//     {
//         for (size_t j = 0, bitoff = 0; j < width_; ++j, ++bitoff)
//         {
//             filedata >> bdata;
//             if (bitoff > 7)
//             {
//                 bitoff = 0;
//                 cp1.to_ulong();
//             }
//             bitbyte[bitoff] = bdata;
//            
//         }
//     }
// 





    for (int i = 0; i != width_ * height_; ++i)	//ѭ�� nwidth * nheight ��
    {
        filedata >> bp1;
        if (bp1)	//bp1Ϊ��ʱд������
            cp1 = cp1 | (0x80 >> cntchar);
        if (cntchar == 7)//����8λʱ����cntchar.����cp1����vec_matrix
        {
            cntchar=0;	//λ����
            ++cntwidth;	//�������
            matrix_.push_back(cp1);	//����vec_matrix
            cp1 = 0x00;	//����cp1
        }
        else if (cntwidth == width_)		//������ʱ����cntchar��cntwidth.����cp1����vec_matrix
        {
            cntchar = 0;	//λ����
            cntwidth = 1;	//�������
            matrix_.push_back(cp1);	////����vec_matrix
            cp1 = 0x00;	//����cp1
        }
        else	//��λδ��8λ���߿��δ��nwidthʱ, λ�������Ϳ�ȼ���������
        {
            ++cntchar;
            ++cntwidth;
        }
    }
}

void Pbm::ReadMatrixP4(std::ifstream &filedata)
{
    int column_width = 0;
    /**************��width_���������Ҫ���ٸ��ַ�*********/
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
    /**************��width_���������Ҫ���ٸ��ַ�*********/

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
            ofiledata << "P4" << "\n";
            //�����Ⱥ͸߶�
            ofiledata << width_ << " " << height_ << "\n";
            std::ostream_iterator<int8_t> out_iterator(ofiledata);  //ֱ�����matrix_
            std::copy(matrix_.begin(),matrix_.end(),out_iterator);
        }
        else
        {
            BinaryMatrixToTextMatrix();	//��������MatrixתΪ�ı�Matrix
            ofiledata << "P1" << "\n";
            //�����Ⱥ͸߶�
            ofiledata << width_ << " " << height_ << "\n";
 
            int nenter = 0;
            for (int i = 0; i != textmatrix_.size(); ++i)
            {
                ofiledata << textmatrix_[i];
                nenter = i + 1;
                //�������س�,��������հ׷�,�ļ���βʲô�������
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
         std::cout << "\nд��PBM�ļ�ʧ��!" << std::endl;
}

void Pbm::BinaryMatrixToTextMatrix()
{
    for (size_t i = 0, index = 0; i < height_; ++i,++index) //����height_��
    {
        for (int j = 0,bitoff = 0; j < width_; ++j,++bitoff) //����width_��
        {
            if (bitoff > 7) //λ����7����Ϊ0
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