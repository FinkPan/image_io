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

	//P1需要转为vector<bool>时可用. (暂时没有用)
	void BinaryStringToVectorBool(const string &strSequence, vector<bool> &vecbSequence);
	
	//将二进制Matrix转为VecBinaryMatrix
	void BinaryStrMatrixToVecBinaryMatrix(const string &strSequence);
	
	//将文本Matrix转为VecTextMatrix
	void TextStrMatrixToVecTextMatrix(const string &strSequence);	

	//将VecBinaryMatrix转为ToVecTextMatrix
	void VecBinaryMatrixToVecTextMatrix();

	//将VecTextMatrix转为VecBinaryMatrix
	void VecTextMatrixToVecBinaryMatrix();


	imageIO()
		: str_image_data(""),str_file_format(""),str_width(""),str_height(""),str_matrix(""), str_max_value(""),\
			nwidth(0), nheight(0), nmax_value(0){}	//默认类构造函数
private:
	string str_image_data;	//整个image数据

	string str_file_format;	//文件格式
	string str_comment;		//文件注释
	string str_width;		//文件宽度string
	string str_height;		//文件高度string
	string str_matrix;		//文件矩阵数据
	string str_max_value;	//文件最大值

	int nwidth;				//宽度
	int nheight;			//高度
	int nmax_value;			//最大值

	vector<int> vec_binary_matrix;	//二进制Matrix
	vector<int> vec_text_matrix;	//文本Matrix

	vector<string> vec_comment;		//由于注释行数不知,用vector来储存.
};

#endif