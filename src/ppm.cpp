#include "ppm.hpp"

void Ppm::ReadImage(std::ifstream &filedata)
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

        if (file_format_ == "P3" || file_format_ == "p3")
            ReadMatrixP3(filedata);
        else if(file_format_ == "P6" || file_format_ == "p6")
            ReadMatrixP6(filedata);
        else
            std::cout << "\n��ȡPGM�ļ�ʧ��!" << std::endl;
}

void Ppm::ReadMatrixP3(std::ifstream &filedata)
{
    std::cout << "\n��ȡP3�ļ�!" << std::endl;

    if (maxvalue_ != 255)
    {
        std::cout << "���ֵ����255,���Ǳ�׼��ppm�ļ�!\n";
    }
    else
    {
        int ndataR = 0; int ndataG = 0; int ndataB = 0;
        PPMRGBbinary colour;
        for (size_t i = 0; i != width_ * height_; ++i)
        {
            filedata >> ndataR;
            filedata >> ndataG;
            filedata >> ndataB;
            if (ndataR > maxvalue_ || ndataG > maxvalue_ || ndataB > maxvalue_) //�����ֵ�Ƿ�������ֵ
            {
                std::cout << "\n\n��ֵ�������ֵ255!���Ǳ�׼��ppm�ļ�!\n";
                exit(1);
            }
            colour.R = static_cast<int>(ndataR);
            colour.G = static_cast<int>(ndataG);
            colour.B = static_cast<int>(ndataB);

            matrix_byte_.push_back(colour);
        }
    }
    std::cout << "Finsh read the file." << std::endl;

}

void Ppm::ReadMatrixP6(std::ifstream &filedata)
{
    std::cout << "\n��ȡP6�ļ�!" << std::endl;
    PPMRGBbinary colour;
    for (int i = 0; i != width_ * height_; ++i)
    {
        filedata >> colour.R;
        filedata >> colour.G;
        filedata >> colour.B;
        matrix_byte_.push_back(colour);
    }
    std::cout << "Finsh read the file." << std::endl;

}

void Ppm::WriteImage(const std::string &filepath, bool bBinary)
{
    std::ofstream ofiledata(filepath,std::ios_base::out | std::ios_base::binary);
    if (ofiledata)
    {
        if (bBinary)    //��������Ƹ�ʽPPM
        {
            ofiledata << "P6" << std::endl;
            //�����Ⱥ͸߶�
            ofiledata << width_ << " " << height_ << std::endl;
            ofiledata << 255 << std::endl;	//���е����ֵ��Ϊ255
            for (int i = 0; i != matrix_byte_.size(); ++i)
            {
                ofiledata.put(static_cast<char>(matrix_byte_[i].R));
                ofiledata.put(static_cast<char>(matrix_byte_[i].G));
                ofiledata.put(static_cast<char>(matrix_byte_[i].B));
            }
        }
        else    //����ı���ʽPPM
        {
            ofiledata << "P3" << std::endl;
            //�����Ⱥ͸߶�
            ofiledata << width_ << " " << height_ << std::endl;
            ofiledata << 255 << std::endl;	//���е����ֵ��Ϊ255
            BinaryMatrixToTextMatrix();
            int nenter = 0;
            int ntotal = width_*height_;
            for (size_t i = 0; i != matrix_int_.size(); ++i)
            {
                ofiledata.width(3);	//�ı��ļ����ÿ��Ϊ3,�������ֱȽ�����
                ofiledata << std::left << matrix_int_[i].R << " ";
                ofiledata.width(3);
                ofiledata << std::left << matrix_int_[i].G << " ";
                ofiledata.width(3);
                ofiledata << std::left << matrix_int_[i].B;
                nenter = i +1;
                if ((nenter % width_ == 0) && (nenter != ntotal))
                {
                    ofiledata << std::endl;
                }
                else if(nenter == ntotal)
                    ;
                else
                    ofiledata << "    ";
            }
        }
        ofiledata.close();
    }
    else
        std::cout << "\n�޷���: " << filepath << "\n";
}

void Ppm::BinaryMatrixToTextMatrix()
{
    size_t nsize = matrix_byte_.size();
    matrix_int_.resize(nsize);
    PPMRGText textcolour;
    int nmark = 0x00ff;	//���θ�8λ��
    for (size_t i = 0; i < nsize; ++i)
    {
        textcolour.R = matrix_byte_[i].R & nmark;	//���θ�8λ��
        textcolour.G = matrix_byte_[i].G & nmark;
        textcolour.B = matrix_byte_[i].B & nmark;
        matrix_int_[i] = textcolour;
    }
}