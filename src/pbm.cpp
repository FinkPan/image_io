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
        filedata >> file_format_; //��ȡ�ļ���ʽ
        SkipComments(filedata);   //����ע��
        filedata >> width_;       //��ȡwidth
        SkipComments(filedata);
        filedata >> height_;      //��ȡHeight
        SkipComments(filedata);

        if (file_format_ == "P1" || file_format_ == "p1")
            ReadMatrixP1(filedata);
        else if (file_format_ == "P4" || file_format_ == "p4")
            ReadMatrixP4(filedata);
        else
            std::cout << "�ļ���ʽ����.\n";
    }
    else
        std::cout << "��ȡ�ļ�( " << filename <<  " )ʧ��!\n";

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
            if (bitoff == 0) //bitoff��8λʱתΪini8_t
            {
                bitbyte[bitoff] = bdata;
                n8data = bitbyte.to_ulong();
                matrix_.push_back(n8data);

                bitoff = 8;     //����bitoff,������bitbyte.
                bitbyte.reset();
                continue;
            }
            bitbyte[bitoff] = bdata; //bitoff����8λ��jδ�ﵽwidth_ʱд��matrix_
        }
        n8data = bitbyte.to_ulong(); //bitoff����8λ��j�ﵽwidth_ʱд��matrix_
        matrix_.push_back(n8data);
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
    std::istream_iterator<int8_t> in_iterator(filedata);  //ֱ�����matrix_
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
            //�����Ⱥ͸߶�
            ofiledata << width_ << " " << height_ << std::endl;
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