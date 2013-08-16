#include "ppm.hpp"

void Ppm::ReadImage(const std::string &filename)
{
    std::ifstream filedata;
    filedata.open(filename,std::ios_base::in | std::ios_base::binary);
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

        if (maxvalue_ > 255)
        {
            std::cout << "PPM�ļ����ֵ����255,�������255ֵ��֧��!" << std::endl;
            exit(1);
        }
        if (file_format_ == "P3")
            ReadMatrixP3(filedata);
        else if(file_format_ == "P6")
            ReadMatrixP6(filedata);
        else
            std::cout << "\n��ȡPGM�ļ�ʧ��!" << std::endl;
        filedata.close();
    }
}

void Ppm::ReadMatrixP3(std::ifstream &filedata)
{
    int ndataR = 0; int ndataG = 0; int ndataB = 0;
    PPMRGBbinary colour;
    for (size_t i = 0; i != width_ * height_; ++i)
    {
        filedata >> ndataR;
        filedata >> ndataG;
        filedata >> ndataB;
//         if (ndataR > maxvalue_ || ndataG > maxvalue_ || ndataB > maxvalue_) //�����ֵ�Ƿ�������ֵ
//         {
//             std::cout << "\n\n��ֵ�������ֵ!�ļ���ʽ����!\n";
//             exit(1);
//         }
        colour.R = static_cast<int>(ndataR);
        colour.G = static_cast<int>(ndataG);
        colour.B = static_cast<int>(ndataB);

        matrix_byte_.push_back(colour);
    }
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
            ofiledata << maxvalue_ << std::endl;	//���е����ֵ��Ϊ255
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
            ofiledata << maxvalue_ << std::endl;
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
    matrix_int_.resize(matrix_byte_.size());
    PPMRGBText textcolour;
    for (size_t i = 0; i != matrix_byte_.size(); ++i)
    {
        textcolour.R = matrix_byte_[i].R & 0xff;	//���θ�8λ��
        textcolour.G = matrix_byte_[i].G & 0xff;
        textcolour.B = matrix_byte_[i].B & 0xff;
        matrix_int_[i] = textcolour;
    }
}

Ppm::PPMRGBText Ppm::GetPixelValue(const unsigned int row,const unsigned int column)
{
    if (row > width_ || column > height_)
    {
        std::cout << "��������д����ļ�: " << width_ << " * " << height_;
    }
    else
    {
        BinaryMatrixToTextMatrix();
        return matrix_int_[column+row*width_];
    }

}

void Ppm::SetPixelValue(const int row,const int column,int R,int G,int B)
{
    if (R > maxvalue_ || G > maxvalue_ || B > maxvalue_)
    {
        std::cout << "�����RGBֵ����255,����֧�ִ���255��RGBֵ.\n";
    }
    PPMRGBbinary color;
    color.R = R;
    color.G = G;
    color.B = B;
    matrix_byte_[column+row*width_] = color;

}