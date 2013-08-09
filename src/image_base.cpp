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
    if (file_format_ == "P4" | file_format_ == "P1")	//P1,P4�ļ���ʽ: һ������ռ1λ,8������ռ1���ֽ�.
    {
        int nbitwidth;	//�б��,�����nbitwidth����nwidth,���Ժ�����;
        for (size_t i = 0; i < nsize; ++i)
        {
            nbitwidth = 0;	//������
            for (int j = 0; j < 8; j++)
            {
                //��һ���ֽ�ÿһλ��matrix_[i] �� ����,���Ϊ��,matrix_[i],д��1,����д��0
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
                else	//��nbitwidth����nwidth,���Ժ�����;
                    break;	
            }
        }
    }
    else	//P5,P6�ļ���ʽ: һ������ռ1���ֽ�
    {
        textmatrix_.resize(nsize);
        int ndec,nhex;
        int nmark = 0x00ff;	//���θ�8λ��
        for (size_t i = 0; i < nsize; ++i)
        {
            nhex = matrix_[i];
            ndec = nhex & nmark;	//���θ�8λ��
            textmatrix_[i] = ndec;
        }
    }
}