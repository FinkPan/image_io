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
	int filelength;		//�ļ�����
	filestr.open(filename,ios_base::in | ios_base::binary);
	if (filestr)
	{
		//get filelength of file:
		filestr.seekg (0, ios::end);
		filelength = filestr.tellg();
		filestr.seekg (0, ios::beg);

		//������һ����д��str_image_data
		str_image_data.resize(filelength);
		filestr.read (&str_image_data[0],filelength);

		size_t pose_not_letter_or_number = 0;	//ָ�����ĸ,������
		size_t pose_is_letter_or_number = 0;		//ָ����ĸ��������

		/******************�����ļ���ʽ***********************/
		str_file_format.resize(2);							//�ļ���ʽ�̶�ΪP1~P6	��2���ֽ�
		str_file_format = str_image_data.substr(0,2);		
		/******************�����ļ���ʽ***********************/


			/******************����ע��***********************/
		char cdata;			//����һ���ַ��Ƿ�Ϊ'#',�����ж��Ƿ�Ϊע��
		int nCommentstart;	//���ע����ʼλ��
		pose_not_letter_or_number = str_image_data.find_first_of(" \r\n");									//Ѱ�ҵ�һ���س���λ��
		pose_is_letter_or_number = str_image_data.find_first_not_of(" \r\n",pose_not_letter_or_number);		//�ӵ�һ���س�����ʼѰ�ҵ�һ�����ǻس�����λ��
		nCommentstart = pose_is_letter_or_number;	
		cdata = str_image_data[pose_is_letter_or_number];
		while (cdata == '#')	
		{
			pose_not_letter_or_number = str_image_data.find_first_of("\r\n",pose_is_letter_or_number);	//������\r\n,���Կո�
			pose_is_letter_or_number = str_image_data.find_first_not_of(" \r\n",pose_not_letter_or_number); 
			cdata = str_image_data[pose_is_letter_or_number];
			str_comment = str_image_data.substr(nCommentstart, pose_not_letter_or_number - nCommentstart);
			nCommentstart = pose_is_letter_or_number;
			vec_comment.push_back(str_comment);
		}
		/******************����ע��***********************/

		/******************����width***********************/
		pose_not_letter_or_number = str_image_data.find_first_of(" \r\n",pose_is_letter_or_number);
		str_width = str_image_data.substr(pose_is_letter_or_number, pose_not_letter_or_number - pose_is_letter_or_number);	
		nwidth = stoi(str_width);
		/******************����width***********************/

		/******************����Height***********************/
		pose_is_letter_or_number = str_image_data.find_first_not_of(" \r\n",pose_not_letter_or_number);
		pose_not_letter_or_number = str_image_data.find_first_of(" \r\n",pose_is_letter_or_number);
		str_height = str_image_data.substr(pose_is_letter_or_number, pose_not_letter_or_number - pose_is_letter_or_number);
		nheight = stoi(str_height);
		/******************����Height***********************/

		if (str_file_format != "P1" && str_file_format != "P4")	//P1, P4 û�����ֵ
		{
			/******************����str_max_value***********************/
			pose_is_letter_or_number = str_image_data.find_first_not_of(" \r\n",pose_not_letter_or_number);
			pose_not_letter_or_number = str_image_data.find_first_of(" \r\n",pose_is_letter_or_number);
			str_max_value = str_image_data.substr(pose_is_letter_or_number, pose_not_letter_or_number - pose_is_letter_or_number);
			nmax_value = stoi(str_max_value);
			/******************����str_max_value***********************/
		}

		/******************����matrix***********************/
		pose_is_letter_or_number = str_image_data.find_first_not_of(" \r\n",pose_not_letter_or_number);
		pose_not_letter_or_number = str_image_data.find_first_of(" \r\n",pose_is_letter_or_number);
		str_matrix = str_image_data.substr(pose_is_letter_or_number, std::string::npos - pose_is_letter_or_number);	//��ʣ�µ�����ȫ���ŵ�str_matrix
		/******************����matrix***********************/

		if (str_file_format == "P1" || str_file_format == "P2" ||str_file_format == "P3" )	//P1,P2,P3��ʽд��vec_text_matrix
			TextStrMatrixToVecTextMatrix(str_matrix);

		if (str_file_format == "P4" || str_file_format == "P5" || str_file_format == "P6")	//P4,P5,P6��ʽд��vec_binary_matrix
			BinaryStrMatrixToVecBinaryMatrix(str_matrix);

		std::cout << "Finsh read the file." << endl;
	}
	else
		std::cout << "�޷���: " << filename << endl;

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

		if (!bBinary)	//���Ϊ�ı���ʽ
		{
			//����ļ���ʽ
			if (str_file_format == "P4")	//�������������ı��ļ�,��Ӧ�ļ���ʽ��Ҫ�ı�.��ת����Ӧ��Matrix
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

			//���ע��,ע�Ϳ��п���
			if (!vec_comment.empty())
				for (vector<string>::iterator vec_str_iter = vec_comment.begin(); vec_str_iter != vec_comment.end(); ++vec_str_iter)
					ofilestr << *vec_str_iter << endl;

			//�����Ⱥ͸߶�
			ofilestr << str_width << " " << str_height << endl;

			//������ֵ,P1,P4��ʽû�����ֵ.
			if (!str_max_value.empty())
				ofilestr << str_max_value << endl; 

			//���Matrix
			for (int i = 0; i != vec_text_matrix.size(); ++i)
			{
				ofilestr.width(3);	//�ı��ļ����ÿ��Ϊ3,�������ֱȽ�����.
				ofilestr << vec_text_matrix[i] << " ";
				int nenter = i +1;	
				if (str_output_fileformat == "P3")		//P3��ʽ3����ȵ�λ
					noutput_width = nwidth*3;
				if (nenter % noutput_width == 0)		//Ϊ�˸�ʽ����.����س���
					ofilestr << endl;
			}
		}
		if (bBinary)	//���Ϊ�����Ƹ�ʽ
		{

			if (str_file_format == "P1")		//����ı��ļ�����������ļ�,��Ӧ�ļ���ʽ��Ҫ�ı�,��ת����Ӧ��Matrix
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

			//����ļ���ʽ
			ofilestr << str_output_fileformat << endl;	

			//���ע��,ע�Ϳ��п���
			if (!vec_comment.empty())
				for (vector<string>::iterator vec_str_iter = vec_comment.begin(); vec_str_iter != vec_comment.end(); ++vec_str_iter)
					ofilestr << *vec_str_iter << endl;

			//�����Ⱥ͸߶�
			ofilestr << str_width << " " << str_height << endl;

			if (str_output_fileformat != "P4")	//���������P4û�����ֵ,�������ֵΪ255 (��ֵ�ο�PS����)
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

//��������MatrixתΪvec_binary_matrix
void imageIO::BinaryStrMatrixToVecBinaryMatrix(const string &strSequence)
{
	size_t nsize = strSequence.size();
	vec_binary_matrix.resize(nsize);
	for (int i = 0; i < nsize; ++i)
		vec_binary_matrix[i] = strSequence[i];
}

//���ı�MatrixתΪvec_text_matrix
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

//��VecBinaryMatrixתΪToVecTextMatrix
void imageIO::VecBinaryMatrixToVecTextMatrix()
{
	size_t nsize = vec_binary_matrix.size();
	if (str_file_format == "P4")	//P4�ļ���ʽ: һ������ռ1λ,8������ռ1���ֽ�.
	{
		int nbitwidth;	//�б��,�����nbitwidth����nwidth,���Ժ�����;
		//vec_text_matrix.resize(nwidth*nheight);
		for (int i = 0; i < nsize; ++i)
		{
			nbitwidth = 0;	//������
			for (int j = 0; j < 8; j++)
			{
				//��һ���ֽ�ÿһλ��vec_binary_matrix[i] �� ����,���Ϊ��,vec_text_matrix[i],д��1,����д��0
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
		for (int i = 0; i < nsize; ++i)
		{
			nhex = vec_binary_matrix[i];
			ndec = nhex & nmark;	//���θ�8λ��
			vec_text_matrix[i] = ndec;
		}
	}
}

//��VecTextMatrixתΪVecBinaryMatrix
void imageIO::VecTextMatrixToVecBinaryMatrix()
{
	size_t nsize = vec_text_matrix.size();
	if (str_file_format == "P1") //P1�ļ���ʽ: һ������ռ1λ,8������ռ1����.
	{
		for (int i = 0; i != nsize; )
		{
			int nbit = 0;	//1���ֽ���8λ,λ���,ÿ8λ����Ҫ����.
			char cz = 0x00;
  			for (int k = 0; k != nwidth; ++k)
  			{
				if (nbit == 8)	//���һ���ֽ�,д��vec_binary_matrix,������ λ���
				{
					vec_binary_matrix.push_back(cz);
					cz = 0x00;
					nbit =0;
				}
				if (vec_text_matrix[i] == 1)	
					cz = cz | (0x80>>nbit);		//�����ֵΪ1����� �� ����,��8λ��һ���ֽڱ�ʾ.
				++nbit;
				++i;
			}
			vec_binary_matrix.push_back(cz);	//д��δ��8λ���ֽ�,������ λ���
			cz = 0x00;
		}
	}
	else	//P2,�ļ���ʽ: һ������ռ1���ֽ�
	{
		int nbinary_value;
		int ndecimal_value;
		float stemp;
		for (int i = 0; i != nsize; ++i)
		{
			ndecimal_value = vec_text_matrix[i];

			//���ձ������ֵ�������ı��ļ�ֵת��Ϊ���ֵΪ255��Ӧ��ֵ
			stemp =  float(ndecimal_value * 255) /nmax_value;		
			if (abs(stemp - ((int)stemp)) == 0)		//������ȡֵ�Ƿ����?������û�и��淶����ȡֵ�ķ���?
				nbinary_value = stemp;
			else
				nbinary_value = stemp + 1;

			vec_binary_matrix.push_back(nbinary_value);	//��ֵд��vec_binary_matrix
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
		std::cout << "��������д�." << endl;

	return 0;
}