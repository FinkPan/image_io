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
		//��ȡ�ļ���ʽ
		filestr >> str_file_format;

		/******************����ע��***********************/
		char cdata;			//����һ���ַ��Ƿ�Ϊ'#',�����ж��Ƿ�Ϊע��
		filestr >> cdata;
		while (cdata == '#')	
		{
			filestr.putback(cdata);
			getline(filestr,str_comment);
			vec_comment.push_back(str_comment);
			filestr >> cdata;
		}
		filestr.putback(cdata);
		/******************����ע��***********************/

		//��ȡwidth
		filestr >> nwidth;

		//��ȡHeight
		filestr >> nheight;

		//��ȡstr_max_value
		if (str_file_format != "P1" && str_file_format != "P4")	//P1, P4 û�����ֵ
			filestr >> nmax_value;

		/******************����matrix***********************/
		if (str_file_format == "P1" || str_file_format == "P2" || str_file_format == "P3" )	//P1,P2,P3��ʽд��vec_text_matrix
		{
			int nlocalnwidth = nwidth;
			if (str_file_format == "P3")	//P3����nwidth��3��
				nlocalnwidth = nwidth*3;
			int nmatrix = 0;
			int n_max_element = nlocalnwidth * nheight;
			for (int i = 0; i != n_max_element; ++i)
			{
				filestr >> nmatrix;
				vec_text_matrix.push_back(nmatrix);
			}
		}
		if (str_file_format == "P4" || str_file_format == "P5" || str_file_format == "P6")	//P4,P5,P6��ʽд��vec_binary_matrix
		{
			
			int nlocalnwidth = nwidth;
			if (str_file_format == "P4")	//P4�ļ���ʽһ���ַ���ʾ8������
			{
				/**************��nwidth���������Ҫ���ٸ��ַ�*********/
				div_t divresult;
				divresult = div (nwidth,8);
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
				nlocalnwidth = nwidth*3;
			int n_max_element = nlocalnwidth * nheight;
			char cz = 0;
			for (int i = 0; i != n_max_element; ++i)
			{
				filestr >> cz;
				vec_binary_matrix.push_back(cz);
			}
		}
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
			ofilestr << nwidth << " " << nheight << endl;

			//������ֵ,P1,P4��ʽû�����ֵ.
			if (nmax_value!=0)
				ofilestr << nmax_value << endl; 

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
			ofilestr << nwidth << " " << nheight << endl;

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

	//system("pause");
	return 0;
}