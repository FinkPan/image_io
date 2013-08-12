#include "pgm.hpp"

void Pgm::ReadImage(std::ifstream &filedata)
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

    if (file_format_ == "P2" || file_format_ == "p2")
        ReadMatrixP2(filedata);
    else if(file_format_ == "P5" || file_format_ == "p5")
        ReadMatrixP5(filedata);
    else
        std::cout << "PGM�ļ���ʽ: "<< file_format_ 
                  << " �Ƿ�!" << std::endl;
}

void Pgm::ReadMatrixP2(std::ifstream &filedata)
{
    std::cout << "\n��ȡP2�ļ�!" << std::endl;  
    if (maxvalue_ != 255)
    {
        float fratio = 0.0;
        std::cout << "��PGM�ļ����ֵ���Ǳ�׼��255,���㷨��תΪ255\n";
        fratio =  float(maxvalue_)/255;
        int8_t n8data = 0x00;
        float fdata = 0.0;
        for (size_t i = 0; i != width_ * height_; ++i)
        {
            filedata >> fdata;
            if (fdata > static_cast<float>(maxvalue_))//�����ֵ�Ƿ�������ֵ
            {
                std::cout << "��ֵ�������ֵ! " << maxvalue_ << std::endl;
                exit(1);
            }
            fdata = fdata/fratio;
            n8data = static_cast<int>(ceil(fdata));
            matrix_.push_back(n8data);
        }
        maxvalue_ = 255;
    }
    else
    {
        int8_t n8data = 0x00;
        int ndata = 0;
        for (size_t i = 0; i != width_ * height_; ++i)
        {
            filedata >> ndata;
            if (ndata > maxvalue_) //�����ֵ�Ƿ�������ֵ
            {
                std::cout << "��ֵ�������ֵ!\n";
                exit(1);
            }
            n8data = static_cast<int>(ndata);
            matrix_.push_back(n8data);
        }
    }
    std::cout << "Finsh read the file." << std::endl;

}

void Pgm::ReadMatrixP5(std::ifstream &filedata)
{
    std::cout << "\n��ȡP5�ļ�!" << std::endl;

    int8_t n8data = 0x00;
    for (int i = 0; i != width_ * height_; ++i)
    {
        filedata >> n8data;
        matrix_.push_back(n8data);
    }
    std::cout << "Finsh read the file." << std::endl;

}

void Pgm::WriteImage(const std::string &filepath, bool bBinary)
{
    std::ofstream ofiledata(filepath,std::ios_base::out | std::ios_base::binary);
    if (ofiledata)
    {
        if (bBinary)
        {
            ofiledata << "P5" << std::endl;
            //�����Ⱥ͸߶�
            ofiledata << width_ << " " << height_ << std::endl;
            ofiledata << 255 << std::endl;	//���е����ֵ��Ϊ255
            for (int i = 0; i != matrix_.size(); ++i)
                ofiledata.put(static_cast<char>(matrix_[i]));
        }
        else    //����ı���ʽPGM
        {
            ofiledata << "P2" << std::endl;
            //�����Ⱥ͸߶�
            ofiledata << width_ << " " << height_ << std::endl;
            ofiledata << 255 << std::endl;	//���е����ֵ��Ϊ255
            BinaryMatrixToTextMatrix();
            int nenter = 0;
            for (int i = 0; i != textmatrix_.size(); ++i)
            {
                ofiledata.width(3);	//�ı��ļ����ÿ��Ϊ3,�������ֱȽ�����
                ofiledata << std::left << textmatrix_[i];
                nenter = i +1;	
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
        std::cout << "\n�޷���: " << filepath << "\n";
}

void Pgm::BinaryMatrixToTextMatrix()
{
    size_t nsize = matrix_.size();
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