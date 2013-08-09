#include "pgm.hpp"

void Pgm::ReadImage(const std::string &filepath)
{
    std::ifstream ifiledata(filepath,std::ios_base::in | std::ios_base::binary);
    if (ifiledata)
    {

        std::string tempstr;
        ifiledata >> file_format_;   //读取文件格式
        SkipComments(ifiledata);
        ifiledata >> width_;         //读取宽度
        SkipComments(ifiledata);
        ifiledata >> height_;        //读取高度
        SkipComments(ifiledata);
        ifiledata >> maxvalue_;      //读取最大值   
        SkipComments(ifiledata);

        if (file_format_ == "p2" || file_format_ == "P2")
            ReadMatrixP2(ifiledata);
        else if(file_format_ == "p5" || file_format_ == "P5")
            ReadMatrixP5(ifiledata);
        else
            std::cout << "\n读取PGM文件失败!" << std::endl;
        ifiledata.close();
    }
    else
        std::cout << "\n无法打开: " << filepath << "\n";
}

void Pgm::ReadMatrixP2(std::ifstream &ifiledata)
{
    std::cout << "\n读取P2文件!" << std::endl;
    
    if (maxvalue_ != 255)
    {
        float fratio = 0.0;
        std::cout << "该PGM文件最大值不是标准的255,该算法将转为255\n";
        fratio =  float(maxvalue_)/255;
        int8_t n8data = 0x00;
        float fdata = 0.0;
        for (size_t i = 0; i != width_ * height_; ++i)
        {
            ifiledata >> fdata;
            if (fdata > static_cast<float>(maxvalue_))//检查数值是否大于最大值
            {
                std::cout << "数值大于最大值! " << maxvalue_ << std::endl;
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
            if (ndata > maxvalue_) //检查数值是否大于最大值
            {
                std::cout << "数值大于最大值!\n";
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
    std::cout << "\n读取P5文件!" << std::endl;

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
            //输出宽度和高度
            ofiledata << width_ << " " << height_ << std::endl;
            ofiledata << 255 << std::endl;	//所有的最大值都为255
            for (int i = 0; i != matrix_.size(); ++i)
                ofiledata.put(static_cast<char>(matrix_[i]));
        }
        else    //输出文本格式PGM
        {
            ofiledata << "P2" << std::endl;
            //输出宽度和高度
            ofiledata << width_ << " " << height_ << std::endl;
            ofiledata << 255 << std::endl;	//所有的最大值都为255
            BinaryMatrixToTextMatrix();
            for (int i = 0; i != textmatrix_.size(); ++i)
            {
                ofiledata.width(3);	//文本文件设置宽度为3,对齐数字比较整齐
                ofiledata << textmatrix_[i] << " ";
                int nenter = i +1;	
                if (nenter % width_ == 0)		//为了格式对齐.输出回车行
                    ofiledata << std::endl;
            }
        }
        ofiledata.close();
    }
    else
        std::cout << "\n无法打开: " << filepath << "\n";
}