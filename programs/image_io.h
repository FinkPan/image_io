#ifndef _IMAGE_IO_H_
#define _IMAGE_IO_H_

#include <string>
#include <vector>
using namespace std;

class imageIO
{
public:
	void ReadimageIOFile(const string &filename);
	//void WriteimageIOFile(const string &filename);
	void WriteimageIOFile(const string &filename,bool bBinary);

	//P1��ҪתΪvector<bool>ʱ����. (��ʱû����)
	void BinaryStringToVectorBool(const string &strSequence, vector<bool> &vecbSequence);
	
	//��������MatrixתΪVecBinaryMatrix
	void BinaryStrMatrixToVecBinaryMatrix(const string &strSequence);
	
	//���ı�MatrixתΪVecTextMatrix
	void TextStrMatrixToVecTextMatrix(const string &strSequence);	

	//��VecBinaryMatrixתΪToVecTextMatrix
	void VecBinaryMatrixToVecTextMatrix();

	//��VecTextMatrixתΪVecBinaryMatrix
	void VecTextMatrixToVecBinaryMatrix();


	imageIO()
		: str_image_data(""),str_file_format(""),str_width(""),str_height(""),str_matrix(""), str_max_value(""),\
			nwidth(0), nheight(0), nmax_value(0){}	//Ĭ���๹�캯��
private:
	string str_image_data;	//����image����

	string str_file_format;	//�ļ���ʽ
	string str_comment;		//�ļ�ע��
	string str_width;		//�ļ����string
	string str_height;		//�ļ��߶�string
	string str_matrix;		//�ļ���������
	string str_max_value;	//�ļ����ֵ

	int nwidth;				//���
	int nheight;			//�߶�
	int nmax_value;			//���ֵ

	vector<int> vec_binary_matrix;	//������Matrix
	vector<int> vec_text_matrix;	//�ı�Matrix

	vector<string> vec_comment;		//����ע��������֪,��vector������.
};

#endif