#include "Pbm.hpp"

void Pbm::ReadImage(const std::string &filepath)
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
            for (int i = 0; i != textmatrix_.size(); ++i)
            {
                ofiledata.width(3);	//�ı��ļ����ÿ��Ϊ3,�������ֱȽ�����
                ofiledata << textmatrix_[i] << " ";
                int nenter = i +1;	
                if (nenter % width_ == 0)		//Ϊ�˸�ʽ����.����س���
                    ofiledata << std::endl;
            }
        
        }
         ofiledata.close();
    }
    else
         std::cout << "\nд��PBM�ļ�ʧ��!" << std::endl;
}