#include "Pbm.hpp"
#include <cstdlib>

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
    int bp1 = 0;
    char cp1 = 0x00;
    int cntwidth = 1;	//��ȼ�����
    int cntchar = 0;	//�ַ�λ��������
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
            ofiledata << "P4" << std::endl;
            //�����Ⱥ͸߶�
            ofiledata << width_ << " " << height_ << std::endl;
            for (int i = 0; i != matrix_.size(); ++i)
                ofiledata.put(static_cast<char>(matrix_[i]));
        }
        else
        {
            ofiledata << "P1" << std::endl;
            //�����Ⱥ͸߶�
            ofiledata << width_ << " " << height_ << std::endl;
            BinaryMatrixToTextMatrix();	//��������MatrixתΪ�ı�Matrix
            int nenter = 0;
            for (int i = 0; i != textmatrix_.size(); ++i)
            {
                ofiledata << textmatrix_[i];
                nenter = i + 1;
                //�������س�,��������հ׷�.
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
         std::cout << "\nд��PBM�ļ�ʧ��!" << std::endl;
}

void Pbm::BinaryMatrixToTextMatrix()
{
    int nbitwidth = 0;	//�б��,�����nbitwidth����nwidth,���Ժ�����;
    size_t nsize = matrix_.size();
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