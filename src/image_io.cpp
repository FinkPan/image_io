#include "image_io.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>  //����һ��8λ��int8_t
#include <cmath>	//��ȡ������ ceil()
#include <iostream>

bool image_io::ReadFile(const std::string& filename)
{
    std::ifstream filedata;
    filedata.open(filename,std::ios_base::in | std::ios_base::binary);
    if (filedata)
    {
        //��ȡ�ļ���ʽ
        filedata >> str_file_format;

        //����ע��
        char cdata;			//����һ���ַ��Ƿ�Ϊ'#',�����ж��Ƿ�Ϊע��
        filedata >> cdata;
        while (cdata == '#')	
        {
            filedata.putback(cdata);
            getline(filedata,str_comment);
            vec_comment.push_back(str_comment);
            filedata >> cdata;
        }
        filedata.putback(cdata); //����'#'���ַ��Ż�����

        //��ȡwidth
        filedata >> width;

        //��ȡHeight
        filedata >> height;

        //��ȡstr_max_value
        float fratio;
        bool bmax_value = false;	//���ֵ�Ƿ�Ϊ255
        if (str_file_format != "P1" && str_file_format != "P4")	//P1, P4 û�����ֵ
        {
            filedata >> max_value;
            if (max_value != 255)
            {	//���ձ������ֵ�������ı��ļ�ֵת��Ϊ���ֵΪ255��Ӧ��ֵ
                fratio =  float(max_value)/255;
                bmax_value = true;	//���ֵ����255
            }
        }

        //����matrix
        if (str_file_format == "P1")
        {
            bool bp1 = false;
            char cp1 = 0x00;
            int cntwidth = 1;	//��ȼ�����
            int cntchar = 0;	//�ַ�λ��������
            for (int i = 0; i != width * height; ++i)	//ѭ�� nwidth * nheight ��
            {
                filedata >> bp1;
                if (bp1)	//bp1Ϊ��ʱд������
                    cp1 = cp1 | (0x80 >> cntchar);
                if (cntchar == 7)//����8λʱ����cntchar.����cp1����vec_matrix
                {
                    cntchar=0;	//λ����
                    ++cntwidth;	//�������
                    vec_matrix.push_back(cp1);	//����vec_matrix
                    cp1 = 0x00;	//����cp1
                }
                else if (cntwidth == width)		//������ʱ����cntchar��cntwidth.����cp1����vec_matrix
                {
                    cntchar = 0;	//λ����
                    cntwidth = 1;	//�������
                    vec_matrix.push_back(cp1);	////����vec_matrix
                    cp1 = 0x00;	//����cp1
                }
                else	//��λδ��8λ���߿��δ��nwidthʱ, λ�������Ϳ�ȼ���������
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
            if (str_file_format == "P3")	//P3����nwidth��3��
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
            if (str_file_format == "P3")	//P3����nwidth��3��
                nlocalnwidth = width*3;
            int n_max_element = nlocalnwidth * height;
            float fmatrix = 0;
            int8_t n8_matrix = 0x00;
            for (int i = 0; i != n_max_element; ++i)
            {
                char cp1 = 0x00;
                filedata >> fmatrix;
                fmatrix = fmatrix / fratio;
                n8_matrix = (int)ceil(fmatrix);		//��ȡ��
                vec_matrix.push_back(n8_matrix);
            }
        }
        else if (str_file_format == "P4" || str_file_format == "P5" || str_file_format == "P6")	//P4,P5,P6��ʽд��vec_binary_matrix
        {

            int nlocalnwidth = width;
            if (str_file_format == "P4")	//P4�ļ���ʽһ���ַ���ʾ8������
            {
                /**************��nwidth���������Ҫ���ٸ��ַ�*********/
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
                /**************��nwidth���������Ҫ���ٸ��ַ�*********/
            }
            if (str_file_format == "P6")	//P6����nwidth��3��
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
        std::cout << "�޷���: " << filename << std::endl;
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

        if (!bBinary)	//���Ϊ�ı���ʽ
        {
            //����ļ���ʽ
            if (str_file_format == "P4")	//�������������ı��ļ�,��Ӧ�ļ���ʽ��Ҫ�ı�
                str_output_fileformat = "P1";
            else if (str_file_format == "P5")	
                str_output_fileformat = "P2";
            else if (str_file_format == "P6")	
                str_output_fileformat = "P3";
            else
                str_output_fileformat = str_file_format;

            ofilestr << str_output_fileformat << std::endl; 

            //���ע��,ע�Ϳ��п���
            if (!vec_comment.empty())
                for (std::vector<std::string>::iterator vec_str_iter = vec_comment.begin(); 
                        vec_str_iter != vec_comment.end(); ++vec_str_iter)
                    ofilestr << *vec_str_iter << std::endl;

            //�����Ⱥ͸߶�
            ofilestr << width << " " << height << std::endl;

            //������ֵ,P1,P4��ʽû�����ֵ.
            if (max_value!=0)
                ofilestr << 255 << std::endl;	//���е����ֵ��Ϊ255

            //���Matrix
            VecMatrixToVecTextMatrix();	// ����ȫ���Ƕ����Ƶ�matrix,������Ҫת��Ϊ�ı���matrix
            for (int i = 0; i != vec_text_matrix.size(); ++i)
            {
                ofilestr.width(3);	//�ı��ļ����ÿ��Ϊ3,�������ֱȽ�����.
                ofilestr << vec_text_matrix[i] << " ";
                int nenter = i +1;	
                if (str_output_fileformat == "P3")		//P3��ʽ3����ȵ�λ
                    noutput_width = width*3;
                if (nenter % noutput_width == 0)		//Ϊ�˸�ʽ����.����س���
                    ofilestr << std::endl;
            }
        }
        if (bBinary)	//���Ϊ�����Ƹ�ʽ
        {
            if (str_file_format == "P1")		//����ı��ļ�����������ļ�,��Ӧ�ļ���ʽ��Ҫ�ı�
                str_output_fileformat = "P4";
            else if (str_file_format == "P2")
                str_output_fileformat = "P5";
            else if (str_file_format == "P3")
                str_output_fileformat = "P6";
            else
                str_output_fileformat = str_file_format;

            //����ļ���ʽ
            ofilestr << str_output_fileformat << std::endl;	

            //���ע��,ע�Ϳ��п���
            if (!vec_comment.empty())
                for (std::vector<std::string>::iterator vec_str_iter = vec_comment.begin(); vec_str_iter != vec_comment.end(); ++vec_str_iter)
                    ofilestr << *vec_str_iter << std::endl;

            //�����Ⱥ͸߶�
            ofilestr << width << " " << height << std::endl;

            if (max_value!=0)	//���������P4û�����ֵ,�������ֵΪ255 (��ֵ�ο�PS����)
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

//��VecBinaryMatrixתΪToVecTextMatrix
void image_io::VecMatrixToVecTextMatrix()
{
    size_t nsize = vec_matrix.size();
    if (str_file_format == "P4" | str_file_format == "P1")	//P1,P4�ļ���ʽ: һ������ռ1λ,8������ռ1���ֽ�.
    {
        int nbitwidth;	//�б��,�����nbitwidth����nwidth,���Ժ�����;
        for (size_t i = 0; i < nsize; ++i)
        {
            nbitwidth = 0;	//������
            for (int j = 0; j < 8; j++)
            {
                //��һ���ֽ�ÿһλ��vec_matrix[i] �� ����,���Ϊ��,vec_matrix[i],д��1,����д��0
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
                else	//��nbitwidth����nwidth,���Ժ�����;
                    break;	
            }
        }
    }
    else	//P5,P6�ļ���ʽ: һ������ռ1���ֽ�
    {
        vec_text_matrix.resize(nsize);
        int ndec,nhex;
        int nmark = 0x00ff;	//���θ�8λ��
        for (size_t i = 0; i < nsize; ++i)
        {
            nhex = vec_matrix[i];
            ndec = nhex & nmark;	//���θ�8λ��
            vec_text_matrix[i] = ndec;
        }
    }
}

