#include "pgm.hpp"

void Pgm::ReadImage(const std::string &filepath)
{
    std::ifstream ifiledata(filepath,std::ios_base::in | std::ios_base::binary);
    if (ifiledata)
    {

        std::string tempstr;
        ifiledata >> file_format_;   //��ȡ�ļ���ʽ
        SkipComments(ifiledata);
        ifiledata >> width_;         //��ȡ���
        SkipComments(ifiledata);
        ifiledata >> height_;        //��ȡ�߶�
        SkipComments(ifiledata);
        ifiledata >> maxvalue_;      //��ȡ���ֵ   
        SkipComments(ifiledata);

        if (file_format_ == "p2" || file_format_ == "P2")
            ReadMatrixP2(ifiledata);
        else if(file_format_ == "p5" || file_format_ == "P5")
            ReadMatrixP5(ifiledata);
        else
            std::cout << "\n��ȡPGM�ļ�ʧ��!" << std::endl;
        ifiledata.close();
    }
    else
        std::cout << "\n�޷���: " << filepath << "\n";
}

void Pgm::ReadMatrixP2(std::ifstream &ifiledata)
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
            ifiledata >> fdata;
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
            ifiledata >> ndata;
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

void Pgm::ReadMatrixP5(std::ifstream &ifiledata)
{
    std::cout << "\n��ȡP5�ļ�!" << std::endl;

    int8_t n8data = 0x00;
    for (int i = 0; i != width_ * height_; ++i)
    {
        ifiledata >> n8data;
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
        std::cout << "\n�޷���: " << filepath << "\n";
}