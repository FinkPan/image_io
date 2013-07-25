#ifndef _IMAGE_IO
#define _IMAGE_IO

#include <string>
#include <vector>

class image_io;
class bpm;

class image_io
{
public:
    image_io():iwidth(0), iheight(0),imax_value(0){}
    virtual void ReadFile(const std::string&);
    virtual void WriteFile(const std::string&);
protected:
    std::string str_file_format;	//�ļ���ʽ
    std::string str_comment;		//�ļ�ע��
    std::vector<std::string> vec_comment;
    int iwidth;				//���
    int iheight;			//�߶�
    int imax_value;			//���ֵ
    std::vector<char> vec_matrix;	//matrix
private:

};



class bpm: public image_io
{
public:
    bpm():image_io(){}
    void ReadFile(const std::string&);
    void WriteFile(const std::string&);
protected:

private:

};

#endif