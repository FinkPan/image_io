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

	//将VecBinaryMatrix转为ToVecTextMatrix
	void VecBinaryMatrixToVecTextMatrix();

	//将VecTextMatrix转为VecBinaryMatrix
	void VecTextMatrixToVecBinaryMatrix();


	imageIO()
		: str_file_format(""),str_comment(""),nwidth(0), nheight(0), nmax_value(0){}	//默认类构造函数

private:
	string str_file_format;	//文件格式
	string str_comment;		//文件注释
	vector<string> vec_comment;
	int nwidth;				//宽度
	int nheight;			//高度
	int nmax_value;			//最大值

	vector<int> vec_binary_matrix;	//二进制Matrix
	vector<int> vec_text_matrix;	//文本Matrix
};

#endif