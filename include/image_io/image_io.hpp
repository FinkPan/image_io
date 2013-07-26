#ifndef _IMAGE_IO
#define _IMAGE_IO

#include <string>
#include <vector>

class image_io;
class bpm;

class image_io
{
public:
    image_io():width_(0), height_(0),max_value_(0){}
    bool ReadFile(const std::string&);
    bool WriteFile(const std::string&,bool bBinary);
private:
    std::string file_format_;	//�ļ���ʽ
    std::string comment_;		//�ļ�ע��
    std::vector<std::string> vec_comment_;
    int width_;				//���
    int height_;			//�߶�
    int max_value_;			//���ֵ
    std::vector<char> vec_matrix_;	//matrix
    std::vector<int> vec_text_matrix_;	//�ı�Matrix ����Ҫ����ı���ʽ�ļ�ʱ����
private:
    //��VecBinaryMatrixתΪToVecTextMatrix
    void VecMatrixToVecTextMatrix();
};

#endif