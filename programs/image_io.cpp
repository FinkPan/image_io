#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "image_io.h"
using namespace std;

#  define EQUAL(a,b)              (stricmp(a,b)==0)


static void Usage()		//Usage
{
	printf(
		"Usage: image_io [-in xmlfile] [-ou outfile]\n"
		);
}


void pmb::readPMBFile(const string &filename)
{
	ifstream pmbFile;
	int ndata;	////��Ϊ��֪��vector��ô������,�����˸��м��������push_back();
	char c_is_comment;	//�жϵ�һ����ĸ�Ƿ�Ϊ'#'.�����'#',��˵����ע����.
	int nWidthLocal;	//P3ʱ nWidthLocalΪnWidth��3��.
	string str_is_comment;	////д��ע��
	pmbFile.open(filename);
	if(pmbFile.is_open())
	{
		pmbFile >> strFileFormat;	//�ļ���һ��Ϊ�ļ���ʽ.
		pmbFile >> c_is_comment;	//�ڶ��п�ʼȡ��һ���ַ��ж�,�����'#',��˵����ע����.
		while(c_is_comment == '#')
		{
			pmbFile.unget();	//Unget character
			getline(pmbFile,str_is_comment);	//д��ע��
			strComment.push_back(str_is_comment);
			pmbFile >> c_is_comment;	//����while����
		}
		pmbFile.unget();	//û��ע����,���Զ�ȡ,�Ա������¶�ȡ.
		pmbFile >> nWidth >> nHeigth;		//��ȡ ��,��]
		nWidthLocal = nWidth;
		if ("P2" == strFileFormat)
		{
			pmbFile >> nMaxColor;
		}
		if ("P3" == strFileFormat)
		{
			pmbFile >> nMaxColor;
			nWidthLocal *= 3;		//P3ʱ nWidthΪ3��.
		}
		for (int j = 0; j != nHeigth; ++j)	//������
		{
			for (int i = 0; i != nWidthLocal; ++i)	//������
			{
				pmbFile >> ndata;		//��Ϊ��֪��vector��ô������,�����˸��м��������push_back();
				vecRow.push_back(ndata);		
			} //һ�д�����
			vecColumn.push_back(vecRow);
			vecRow.clear();	//���ÿһ�����ݣ��Ա㴦����һ������
		}
	}
	else
		cout << "can't open input file: " << filename << endl;	//��ȡ�ļ�ʧ����ʾ�û���Ϣ.
	pmbFile.close();	//�ر��ļ�
	cout << "Finsh reading input file." << endl;	//��ȡ�ļ��ɹ���ʾ�û���Ϣ.
}

void pmb::writePMBFile(const string &filename)
{
	ofstream writePMB;
	int nwidth = 1;

	writePMB.open(filename);
	if (writePMB.is_open())
	{
		writePMB << strFileFormat << endl;	//д���ļ���ʽ
		if (strFileFormat != "") //���ע��Ϊ��Ϊ��,д��ע��.
		{
			for (vector<string>::iterator iterComment = strComment.begin(); iterComment != strComment.end(); ++iterComment)
			{
				writePMB << *iterComment << endl;
			}
				
		}
		writePMB << nWidth << " " << nHeigth;		//д�����о���

		if ("P2" == strFileFormat)		//P3��ʽ ������Ϊ2
		{
			writePMB << endl;
			writePMB << nMaxColor;
			nwidth = 2;
		}
		if ("P3" == strFileFormat)		//P3��ʽ ������Ϊ3
		{
			writePMB << endl;
			writePMB << nMaxColor;
			nwidth = 3;
		}

		for (vector<vector<int>>::iterator iterRow = vecColumn.begin(); iterRow != vecColumn.end(); ++iterRow)	//������
		{
			vecRow = *iterRow;	//���з������
			writePMB << endl;	//�Ȼ�����������������Ա����ļ�����п��С�
			for (vector<int>::iterator iterColunm = vecRow.begin(); iterColunm != vecRow.end(); ++iterColunm)	//������
			{
				writePMB.setf(ios::left);
				writePMB.width(nwidth);		//���������ʽ���
				writePMB << *iterColunm << " ";		//д����	
			}			
		}
	}
	else
		cout << "can't open the output file: " << filename  << endl;	//д���ļ�ʧ����ʾ�û���Ϣ.
	writePMB.close();	//�ر��ļ�.
	cout << "finsh write pmb file.\n" ;	//д���ļ��ɹ���ʾ�û���Ϣ.
}

int main(int argc, char **argv)
{
	pmb mypbm;
	const char* pszInFile = NULL;
	const char* pszOutFile = NULL;
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
			pszOutFile = argv[++i];
		}
		else
		{
			i = argc;
			Usage();
		}
	}
	if (!pszInFile == NULL && !pszOutFile == NULL)
	{
		mypbm.readPMBFile(pszInFile);
		mypbm.writePMBFile(pszOutFile);
	}

	system("PAUSE");
	return 0;
}