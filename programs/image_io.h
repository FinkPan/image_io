#ifndef _IMAGE_IO_H_
#define _IMAGE_IO_H_

#include <string>
#include <vector>
using namespace std;

class imageIO
{
public:
	void ReadimageIOFile(const string &filename);

	void WriteimageIOFile(const string &filename,bool bBinary);

	//��VecBinaryMatrixתΪToVecTextMatrix
	void VecBinaryMatrixToVecTextMatrix();

	//��VecTextMatrixתΪVecBinaryMatrix
	void VecTextMatrixToVecBinaryMatrix();


	imageIO()
		: str_file_format(""),str_comment(""),nwidth(0), nheight(0), nmax_value(0){}	//Ĭ���๹�캯��

private:
	string str_file_format;	//�ļ���ʽ
	string str_comment;		//�ļ�ע��
	vector<string> vec_comment;
	int nwidth;				//���
	int nheight;			//�߶�
	int nmax_value;			//���ֵ

	vector<int> vec_binary_matrix;	//������Matrix
	vector<int> vec_text_matrix;	//�ı�Matrix
};

#endif