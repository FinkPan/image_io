#include "image_io.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>  //定义一个8位的int8_t
#include <cmath>	//上取整函数 ceil()
#include <iostream>

bool image_io::ReadFile(const std::string& filename)
{
    std::ifstream filedata;
    filedata.open(filename,std::ios_base::in | std::ios_base::binary);
    if (filedata)
    {
        //读取文件格式
        filedata >> str_file_format;

        //处理注释
        char cdata;			//检查第一个字符是否为'#',用来判断是否为注释
        filedata >> cdata;
        while (cdata == '#')	
        {
            filedata.putback(cdata);
            getline(filedata,str_comment);
            vec_comment.push_back(str_comment);
            filedata >> cdata;
        }
        filedata.putback(cdata); //不是'#'将字符放回流里

        //读取width
        filedata >> width;

        //读取Height
        filedata >> height;

        //读取str_max_value
        float fratio;
        bool bmax_value = false;	//最大值是否为255
        if (str_file_format != "P1" && str_file_format != "P4")	//P1, P4 没有最大值
        {
            filedata >> max_value;
            if (max_value != 255)
            {	//按照比例最大值比例将文本文件值转换为最大值为255对应的值
                fratio =  float(max_value)/255;
                bmax_value = true;	//最大值不是255
            }
        }

        //处理matrix
        if (str_file_format == "P1")
        {
            bool bp1 = false;
            char cp1 = 0x00;
            int cntwidth = 1;	//宽度计数机
            int cntchar = 0;	//字符位数计数机
            for (int i = 0; i != width * height; ++i)	//循环 nwidth * nheight 次
            {
                filedata >> bp1;
                if (bp1)	//bp1为真时写入数据
                    cp1 = cp1 | (0x80 >> cntchar);
                if (cntchar == 7)//当满8位时重置cntchar.并将cp1插入vec_matrix
                {
                    cntchar=0;	//位重置
                    ++cntwidth;	//宽度自增
                    vec_matrix.push_back(cp1);	//插入vec_matrix
                    cp1 = 0x00;	//更新cp1
                }
                else if (cntwidth == width)		//到达宽度时重置cntchar和cntwidth.并将cp1插入vec_matrix
                {
                    cntchar = 0;	//位重置
                    cntwidth = 1;	//宽度重置
                    vec_matrix.push_back(cp1);	////插入vec_matrix
                    cp1 = 0x00;	//更新cp1
                }
                else	//当位未满8位或者宽度未满nwidth时, 位计数机和宽度计数机自增
                {
                    ++cntchar;
                    ++cntwidth;
                }
            }
        }
        else if ((   str_file_format == "P2" 
                   | str_file_format == "P3") 
                  && bmax_value == false)
        {
            int nlocalnwidth = width;
            if (str_file_format == "P3")	//P3宽是nwidth的3倍
                nlocalnwidth = width*3;
            int n_max_element = nlocalnwidth * height;
            int8_t n8_matrix = 0x00;
            int ntemp;
            for (int i = 0; i != n_max_element; ++i)
            {
                char cp1 = 0x00;
                filedata >> ntemp;
                n8_matrix = ntemp;
                vec_matrix.push_back(n8_matrix);
            }
        }
        else if (( str_file_format == "P2" 
                |  str_file_format == "P3") 
                && bmax_value == true)
        {
            int nlocalnwidth = width;
            if (str_file_format == "P3")	//P3宽是nwidth的3倍
                nlocalnwidth = width*3;
            int n_max_element = nlocalnwidth * height;
            float fmatrix = 0;
            int8_t n8_matrix = 0x00;
            for (int i = 0; i != n_max_element; ++i)
            {
                char cp1 = 0x00;
                filedata >> fmatrix;
                fmatrix = fmatrix / fratio;
                n8_matrix = (int)ceil(fmatrix);		//上取整
                vec_matrix.push_back(n8_matrix);
            }
        }
        else if (str_file_format == "P4" || str_file_format == "P5" || str_file_format == "P6")	//P4,P5,P6格式写入vec_binary_matrix
        {

            int nlocalnwidth = width;
            if (str_file_format == "P4")	//P4文件格式一个字符表示8个像素
            {
                /**************由nwidth计算出宽需要多少个字符*********/
                div_t divresult;
                divresult = div (width,8);
                int nquot = divresult.quot;
                int nrem = divresult.rem;
                if (divresult.quot == 0)
                    nlocalnwidth = 1;
                else if (divresult.rem == 0)
                    nlocalnwidth = divresult.quot;
                else
                    nlocalnwidth = divresult.quot + 1;
                /**************由nwidth计算出宽需要多少个字符*********/
            }
            if (str_file_format == "P6")	//P6宽是nwidth的3倍
                nlocalnwidth = width*3;
            int n_max_element = nlocalnwidth * height;
            char cz = 0;
            for (int i = 0; i != n_max_element; ++i)
            {
                filedata >> cz;
                vec_matrix.push_back(cz);
            }
        }
        std::cout << "Finsh read the file." << std::endl;
        return true;
    }
    else
    {
        std::cout << "无法打开: " << filename << std::endl;
        return false;
    }
}
bool image_io::WriteFile(const std::string& filename,bool bBinary)
{
    std::ofstream ofilestr;
    ofilestr.open(filename,std::ios_base::out | std::ios_base::binary);
    if (ofilestr)
    {
        std::string str_output_fileformat;
        int noutput_width = width;

        if (!bBinary)	//输出为文本格式
        {
            //输出文件格式
            if (str_file_format == "P4")	//如果二进制输出文本文件,对应文件格式需要改变
                str_output_fileformat = "P1";
            else if (str_file_format == "P5")	
                str_output_fileformat = "P2";
            else if (str_file_format == "P6")	
                str_output_fileformat = "P3";
            else
                str_output_fileformat = str_file_format;

            ofilestr << str_output_fileformat << std::endl; 

            //输出注释,注释可有可无
            if (!vec_comment.empty())
                for (std::vector<std::string>::iterator vec_str_iter = vec_comment.begin(); 
                        vec_str_iter != vec_comment.end(); ++vec_str_iter)
                    ofilestr << *vec_str_iter << std::endl;

            //输出宽度和高度
            ofilestr << width << " " << height << std::endl;

            //输出最大值,P1,P4格式没有最大值.
            if (max_value!=0)
                ofilestr << 255 << std::endl;	//所有的最大值都为255

            //输出Matrix
            VecMatrixToVecTextMatrix();	// 由于全部是二进制的matrix,所以需要转换为文本的matrix
            for (int i = 0; i != vec_text_matrix.size(); ++i)
            {
                ofilestr.width(3);	//文本文件设置宽度为3,对齐数字比较整齐.
                ofilestr << vec_text_matrix[i] << " ";
                int nenter = i +1;	
                if (str_output_fileformat == "P3")		//P3格式3个宽度单位
                    noutput_width = width*3;
                if (nenter % noutput_width == 0)		//为了格式对齐.输出回车行
                    ofilestr << std::endl;
            }
        }
        if (bBinary)	//输出为二进制格式
        {
            if (str_file_format == "P1")		//如果文本文件输出二进制文件,对应文件格式需要改变
                str_output_fileformat = "P4";
            else if (str_file_format == "P2")
                str_output_fileformat = "P5";
            else if (str_file_format == "P3")
                str_output_fileformat = "P6";
            else
                str_output_fileformat = str_file_format;

            //输出文件格式
            ofilestr << str_output_fileformat << std::endl;	

            //输出注释,注释可有可无
            if (!vec_comment.empty())
                for (std::vector<std::string>::iterator vec_str_iter = vec_comment.begin(); vec_str_iter != vec_comment.end(); ++vec_str_iter)
                    ofilestr << *vec_str_iter << std::endl;

            //输出宽度和高度
            ofilestr << width << " " << height << std::endl;

            if (max_value!=0)	//二进制输出P4没有最大值,其他最大值为255 (该值参考PS所设)
                ofilestr << 255 << std::endl; 

            for (int i = 0; i != vec_matrix.size(); ++i)
                ofilestr.write((char*)&vec_matrix[i],1);
        }
        ofilestr.close();
        std::cout << "Finish write the file:" << filename  << std::endl;
        return true;
    }
    else
    {
        std::cout << "Can't not open the output file: " << filename << std::endl;
        return false;
    }
}

//将VecBinaryMatrix转为ToVecTextMatrix
void image_io::VecMatrixToVecTextMatrix()
{
    size_t nsize = vec_matrix.size();
    if (str_file_format == "P4" | str_file_format == "P1")	//P1,P4文件格式: 一个像素占1位,8个像素占1个字节.
    {
        int nbitwidth;	//行标记,如果行nbitwidth大于nwidth,忽略后面行;
        for (size_t i = 0; i < nsize; ++i)
        {
            nbitwidth = 0;	//重置行
            for (int j = 0; j < 8; j++)
            {
                //将一个字节每一位和vec_matrix[i] 与 运算,如果为真,vec_matrix[i],写入1,否则写入0
                if((nbitwidth < width) && vec_matrix[i] & (0x80 >> j))		
                {
                    vec_text_matrix.push_back(1);
                    ++nbitwidth;
                }
                else if((nbitwidth < width))
                {
                    vec_text_matrix.push_back(0);
                    ++nbitwidth;
                }
                else	//行nbitwidth大于nwidth,忽略后面行;
                    break;	
            }
        }
    }
    else	//P5,P6文件格式: 一个像素占1个字节
    {
        vec_text_matrix.resize(nsize);
        int ndec,nhex;
        int nmark = 0x00ff;	//屏蔽高8位数
        for (size_t i = 0; i < nsize; ++i)
        {
            nhex = vec_matrix[i];
            ndec = nhex & nmark;	//屏蔽高8位数
            vec_text_matrix[i] = ndec;
        }
    }
}

