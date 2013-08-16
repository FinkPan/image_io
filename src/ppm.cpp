#include "ppm.hpp"

void Ppm::ReadImage(const std::string &filename)
{
    std::ifstream filedata;
    filedata.open(filename,std::ios_base::in | std::ios_base::binary);
    if (filedata)
    {
        std::string tempstr;
        filedata >> file_format_;   //读取文件格式
        SkipComments(filedata);
        filedata >> width_;         //读取宽度
        SkipComments(filedata);
        filedata >> height_;        //读取高度
        SkipComments(filedata);
        filedata >> maxvalue_;      //读取最大值   
        SkipComments(filedata);

        if (maxvalue_ > 255)
        {
            std::cout << "PPM文件最大值大于255,程序大于255值不支持!" << std::endl;
            exit(1);
        }
        if (file_format_ == "P3")
            ReadMatrixP3(filedata);
        else if(file_format_ == "P6")
            ReadMatrixP6(filedata);
        else
            std::cout << "\n读取PGM文件失败!" << std::endl;
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
//         if (ndataR > maxvalue_ || ndataG > maxvalue_ || ndataB > maxvalue_) //检查数值是否大于最大值
//         {
//             std::cout << "\n\n数值大于最大值!文件格式错误!\n";
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
    std::cout << "\n读取P6文件!" << std::endl;
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
        if (bBinary)    //输出二进制格式PPM
        {
            ofiledata << "P6" << std::endl;
            //输出宽度和高度
            ofiledata << width_ << " " << height_ << std::endl;
            ofiledata << maxvalue_ << std::endl;	//所有的最大值都为255
            for (int i = 0; i != matrix_byte_.size(); ++i)
            {
                ofiledata.put(static_cast<char>(matrix_byte_[i].R));
                ofiledata.put(static_cast<char>(matrix_byte_[i].G));
                ofiledata.put(static_cast<char>(matrix_byte_[i].B));
            }
        }
        else    //输出文本格式PPM
        {
            ofiledata << "P3" << std::endl;
            //输出宽度和高度
            ofiledata << width_ << " " << height_ << std::endl;
            ofiledata << maxvalue_ << std::endl;
            BinaryMatrixToTextMatrix();
            int nenter = 0;
            int ntotal = width_*height_;
            for (size_t i = 0; i != matrix_int_.size(); ++i)
            {
                ofiledata.width(3);	//文本文件设置宽度为3,对齐数字比较整齐
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
        std::cout << "\n无法打开: " << filepath << "\n";
}

void Ppm::BinaryMatrixToTextMatrix()
{
    matrix_int_.resize(matrix_byte_.size());
    PPMRGBText textcolour;
    for (size_t i = 0; i != matrix_byte_.size(); ++i)
    {
        textcolour.R = matrix_byte_[i].R & 0xff;	//屏蔽高8位数
        textcolour.G = matrix_byte_[i].G & 0xff;
        textcolour.B = matrix_byte_[i].B & 0xff;
        matrix_int_[i] = textcolour;
    }
}

Ppm::PPMRGBText Ppm::GetPixelValue(const unsigned int row,const unsigned int column)
{
    if (row > width_ || column > height_)
    {
        std::cout << "输入的行列大于文件: " << width_ << " * " << height_;
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
        std::cout << "输入的RGB值大于255,程序不支持大于255的RGB值.\n";
    }
    PPMRGBbinary color;
    color.R = R;
    color.G = G;
    color.B = B;
    matrix_byte_[column+row*width_] = color;

}