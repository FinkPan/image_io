#ifndef _IMAGE_IO
#define _IMAGE_IO

#include <string>
#include <vector>

class image_io;
class bpm;

class image_io
{
public:
    image_io():width(0), height(0),max_value(0){}
    bool ReadFile(const std::string&);
    bool WriteFile(const std::string&,bool bBinary);
private:
    std::string str_file_format;	//�ļ���ʽ
    std::string str_comment;		//�ļ�ע��
    std::vector<std::string> vec_comment;
    int width;				//���
    int height;			//�߶�
    int max_value;			//���ֵ
    std::vector<char> vec_matrix;	//matrix
    std::vector<int> vec_text_matrix;	//�ı�Matrix ����Ҫ����ı���ʽ�ļ�ʱ����
private:
    //��VecBinaryMatrixתΪToVecTextMatrix
    void VecMatrixToVecTextMatrix();
};

#endif