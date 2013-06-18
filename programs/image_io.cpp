#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

#include "image_io.h"
using namespace std;

#  define EQUAL(a,b)              (stricmp(a,b)==0)


static void Usage()		//Usage
{
	printf(
		"Usage: image_io [-in inputfile] [-ou [-b] outfile]\n"
		"-in : inputfile.\n"
		"-ou : outfile\n"
		"-b : write outfile by binary."
		);
}

void imageIO::ReadimageIOFile(const string &filename)
{
	ifstream filestr;
	int filelength;		//文件长度
	filestr.open(filename,ios_base::in | ios_base::binary);
	if (filestr)
	{
		//get filelength of file:
		filestr.seekg (0, ios::end);
		filelength = filestr.tellg();
		filestr.seekg (0, ios::beg);

		//将数据一次性写入str_image_data
		str_image_data.resize(filelength);
		filestr.read (&str_image_data[0],filelength);

		size_t pose_not_letter_or_number = 0;	//指向非字母,非数字
		size_t pose_is_letter_or_number = 0;		//指向字母或者数字

		/******************处理文件格式***********************/
		str_file_format.resize(2);							//文件格式固定为P1~P6	即2个字节
		str_file_format = str_image_data.substr(0,2);		
		/******************处理文件格式***********************/


			/******************处理注释***********************/
		char cdata;			//检查第一个字符是否为'#',用来判断是否为注释
		int nCommentstart;	//标记注释起始位置
		pose_not_letter_or_number = str_image_data.find_first_of(" \r\n");									//寻找第一个回车符位置
		pose_is_letter_or_number = str_image_data.find_first_not_of(" \r\n",pose_not_letter_or_number);		//从第一个回车符开始寻找第一个不是回车符的位置
		nCommentstart = pose_is_letter_or_number;	
		cdata = str_image_data[pose_is_letter_or_number];
		while (cdata == '#')	
		{
			pose_not_letter_or_number = str_image_data.find_first_of("\r\n",pose_is_letter_or_number);	//仅查找\r\n,忽略空格
			pose_is_letter_or_number = str_image_data.find_first_not_of(" \r\n",pose_not_letter_or_number); 
			cdata = str_image_data[pose_is_letter_or_number];
			str_comment = str_image_data.substr(nCommentstart, pose_not_letter_or_number - nCommentstart);
			nCommentstart = pose_is_letter_or_number;
			vec_comment.push_back(str_comment);
		}
		/******************处理注释***********************/

		/******************处理width***********************/
		pose_not_letter_or_number = str_image_data.find_first_of(" \r\n",pose_is_letter_or_number);
		str_width = str_image_data.substr(pose_is_letter_or_number, pose_not_letter_or_number - pose_is_letter_or_number);	
		nwidth = stoi(str_width);
		/******************处理width***********************/

		/******************处理Height***********************/
		pose_is_letter_or_number = str_image_data.find_first_not_of(" \r\n",pose_not_letter_or_number);
		pose_not_letter_or_number = str_image_data.find_first_of(" \r\n",pose_is_letter_or_number);
		str_height = str_image_data.substr(pose_is_letter_or_number, pose_not_letter_or_number - pose_is_letter_or_number);
		nheight = stoi(str_height);
		/******************处理Height***********************/

		if (str_file_format != "P1" && str_file_format != "P4")	//P1, P4 没有最大值
		{
			/******************处理str_max_value***********************/
			pose_is_letter_or_number = str_image_data.find_first_not_of(" \r\n",pose_not_letter_or_number);
			pose_not_letter_or_number = str_image_data.find_first_of(" \r\n",pose_is_letter_or_number);
			str_max_value = str_image_data.substr(pose_is_letter_or_number, pose_not_letter_or_number - pose_is_letter_or_number);
			nmax_value = stoi(str_max_value);
			/******************处理str_max_value***********************/
		}

		/******************处理matrix***********************/
		pose_is_letter_or_number = str_image_data.find_first_not_of(" \r\n",pose_not_letter_or_number);
		pose_not_letter_or_number = str_image_data.find_first_of(" \r\n",pose_is_letter_or_number);
		str_matrix = str_image_data.substr(pose_is_letter_or_number, std::string::npos - pose_is_letter_or_number);	//将剩下的数据全部放到str_matrix
		/******************处理matrix***********************/

		if (str_file_format == "P1" || str_file_format == "P2" ||str_file_format == "P3" )	//P1,P2,P3格式写入vec_text_matrix
			TextStrMatrixToVecTextMatrix(str_matrix);

		if (str_file_format == "P4" || str_file_format == "P5" || str_file_format == "P6")	//P4,P5,P6格式写入vec_binary_matrix
			BinaryStrMatrixToVecBinaryMatrix(str_matrix);

		std::cout << "Finsh read the file." << endl;
	}
	else
		std::cout << "无法打开: " << filename << endl;

}

void imageIO::WriteimageIOFile(const string &filename,bool bBinary)
{
	ofstream ofilestr;
	int noutput_width;
	ofilestr.open(filename,ios_base::out | ios_base::binary);
	if (ofilestr)
	{
		string str_output_fileformat = "";
		int noutput_width = nwidth;

		if (!bBinary)	//输出为文本格式
		{
			//输出文件格式
			if (str_file_format == "P4")	//如果二进制输出文本文件,对应文件格式需要改变.并转换相应的Matrix
			{
				str_output_fileformat = "P1";
				VecBinaryMatrixToVecTextMatrix();
			}
			else if (str_file_format == "P5")	
			{
				str_output_fileformat = "P2";
				VecBinaryMatrixToVecTextMatrix();
			}
			else if (str_file_format == "P6")	
			{
				str_output_fileformat = "P3";
				VecBinaryMatrixToVecTextMatrix();
			}
			else
				str_output_fileformat = str_file_format;

			ofilestr << str_output_fileformat << endl; 

			//输出注释,注释可有可无
			if (!vec_comment.empty())
				for (vector<string>::iterator vec_str_iter = vec_comment.begin(); vec_str_iter != vec_comment.end(); ++vec_str_iter)
					ofilestr << *vec_str_iter << endl;

			//输出宽度和高度
			ofilestr << str_width << " " << str_height << endl;

			//输出最大值,P1,P4格式没有最大值.
			if (!str_max_value.empty())
				ofilestr << str_max_value << endl; 

			//输出Matrix
			for (int i = 0; i != vec_text_matrix.size(); ++i)
			{
				ofilestr.width(3);	//文本文件设置宽度为3,对齐数字比较整齐.
				ofilestr << vec_text_matrix[i] << " ";
				int nenter = i +1;	
				if (str_output_fileformat == "P3")		//P3格式3个宽度单位
					noutput_width = nwidth*3;
				if (nenter % noutput_width == 0)		//为了格式对齐.输出回车行
					ofilestr << endl;
			}
		}
		if (bBinary)	//输出为二进制格式
		{

			if (str_file_format == "P1")		//如果文本文件输出二进制文件,对应文件格式需要改变,并转换相应的Matrix
			{
				str_output_fileformat = "P4";
				VecTextMatrixToVecBinaryMatrix();
			}
			else if (str_file_format == "P2")
			{
				str_output_fileformat = "P5";
				VecTextMatrixToVecBinaryMatrix();
			}
			else if (str_file_format == "P3")
			{
				str_output_fileformat = "P6";
				VecTextMatrixToVecBinaryMatrix();
			}
			else
				str_output_fileformat = str_file_format;

			//输出文件格式
			ofilestr << str_output_fileformat << endl;	

			//输出注释,注释可有可无
			if (!vec_comment.empty())
				for (vector<string>::iterator vec_str_iter = vec_comment.begin(); vec_str_iter != vec_comment.end(); ++vec_str_iter)
					ofilestr << *vec_str_iter << endl;

			//输出宽度和高度
			ofilestr << str_width << " " << str_height << endl;

			if (str_output_fileformat != "P4")	//二进制输出P4没有最大值,其他最大值为255 (该值参考PS所设)
				ofilestr << "255" << endl; 

			for (int i = 0; i != vec_binary_matrix.size(); ++i)
				ofilestr.write((char*)&vec_binary_matrix[i],1);
		}
		ofilestr.close();
		std::cout << "Finish write the file:" << filename  << endl;
	}
	else
		std::cout << "Can't not open the output file: " << filename << endl;
}

//将二进制Matrix转为vec_binary_matrix
void imageIO::BinaryStrMatrixToVecBinaryMatrix(const string &strSequence)
{
	size_t nsize = strSequence.size();
	vec_binary_matrix.resize(nsize);
	for (int i = 0; i < nsize; ++i)
		vec_binary_matrix[i] = strSequence[i];
}

//将文本Matrix转为vec_text_matrix
void imageIO::TextStrMatrixToVecTextMatrix(const string &strSequence)
{
	size_t pose_not_letter_or_number = 0;
	size_t pose_is_letter_or_number = 0;
	int nMatrix;
	size_t nsize = strSequence.size();
	pose_not_letter_or_number = strSequence.find_first_not_of("0123456789");
	string strtemp;
	while(string::npos != pose_is_letter_or_number)
	{
		strtemp = strSequence.substr(pose_is_letter_or_number,pose_not_letter_or_number-pose_is_letter_or_number);
		nMatrix = stoi(strtemp);
		vec_text_matrix.push_back(nMatrix);
		pose_is_letter_or_number = strSequence.find_first_of("0123456789",pose_not_letter_or_number);
		pose_not_letter_or_number = strSequence.find_first_not_of("0123456789",pose_is_letter_or_number);
	}
}

//将VecBinaryMatrix转为ToVecTextMatrix
void imageIO::VecBinaryMatrixToVecTextMatrix()
{
	size_t nsize = vec_binary_matrix.size();
	if (str_file_format == "P4")	//P4文件格式: 一个像素占1位,8个像素占1个字节.
	{
		int nbitwidth;	//行标记,如果行nbitwidth大于nwidth,忽略后面行;
		//vec_text_matrix.resize(nwidth*nheight);
		for (int i = 0; i < nsize; ++i)
		{
			nbitwidth = 0;	//重置行
			for (int j = 0; j < 8; j++)
			{
				//将一个字节每一位和vec_binary_matrix[i] 与 运算,如果为真,vec_text_matrix[i],写入1,否则写入0
				if((nbitwidth < nwidth) && vec_binary_matrix[i] & (0x80 >> j))		
				{
					vec_text_matrix.push_back(1);
					++nbitwidth;
				}
				else if((nbitwidth < nwidth))
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
		for (int i = 0; i < nsize; ++i)
		{
			nhex = vec_binary_matrix[i];
			ndec = nhex & nmark;	//屏蔽高8位数
			vec_text_matrix[i] = ndec;
		}
	}
}

//将VecTextMatrix转为VecBinaryMatrix
void imageIO::VecTextMatrixToVecBinaryMatrix()
{
	size_t nsize = vec_text_matrix.size();
	if (str_file_format == "P1") //P1文件格式: 一个像素占1位,8个像素占1个字.
	{
		for (int i = 0; i != nsize; )
		{
			int nbit = 0;	//1个字节有8位,位标记,每8位就需要重置.
			char cz = 0x00;
  			for (int k = 0; k != nwidth; ++k)
  			{
				if (nbit == 8)	//完成一个字节,写入vec_binary_matrix,并更新 位标记
				{
					vec_binary_matrix.push_back(cz);
					cz = 0x00;
					nbit =0;
				}
				if (vec_text_matrix[i] == 1)	
					cz = cz | (0x80>>nbit);		//如果数值为1则进行 或 运算,将8位用一个字节表示.
				++nbit;
				++i;
			}
			vec_binary_matrix.push_back(cz);	//写入未满8位的字节,并更新 位标记
			cz = 0x00;
		}
	}
	else	//P2,文件格式: 一个像素占1个字节
	{
		int nbinary_value;
		int ndecimal_value;
		float stemp;
		for (int i = 0; i != nsize; ++i)
		{
			ndecimal_value = vec_text_matrix[i];

			//按照比例最大值比例将文本文件值转换为最大值为255对应的值
			stemp =  float(ndecimal_value * 255) /nmax_value;		
			if (abs(stemp - ((int)stemp)) == 0)		//这样上取值是否可行?或者有没有更规范的上取值的方法?
				nbinary_value = stemp;
			else
				nbinary_value = stemp + 1;

			vec_binary_matrix.push_back(nbinary_value);	//将值写入vec_binary_matrix
		}
	}
}

int main(int argc, char **argv)
{
	imageIO mypbm;
	const char* pszInFile = NULL;
	const char* pszOutFile = NULL;
	bool bBinaryFile = false;

	if(argc == 1)
		Usage();

	for (int i = 1; i < argc; i++)
	{
		if (EQUAL(argv[i],"-in"))
		{
			pszInFile = argv[++i];
		}
		else if (EQUAL(argv[i],"-ou"))
		{
			if ((EQUAL(argv[++i],"-b")))
			{
				bBinaryFile = true;
				pszOutFile = argv[++i];
			}
			else
				pszOutFile = argv[i];
	
		}
		else
		{
			i = argc;
			Usage();
		}
	}
	if (!pszInFile == NULL && !pszOutFile == NULL)
	{
		mypbm.ReadimageIOFile(pszInFile);
		mypbm.WriteimageIOFile(pszOutFile,bBinaryFile);
	}
	else
		std::cout << "输入参数有错." << endl;

	return 0;
}