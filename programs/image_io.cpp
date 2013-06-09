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
	int ndata;
	char ccomment;
	pmbFile.open(filename);
	if(pmbFile.is_open())
	{

		pmbFile >> strFileForm;
		pmbFile >> ccomment;	//ȡ���ڶ��е�һ���ַ�,�����'#',��˵����ע����.
		if (ccomment == '#')
		{
			pmbFile.unget();	//Unget character
			getline(pmbFile,strComment);	//д��ע��
		}
		else
			pmbFile.unget();	//û��ע����,���Զ�ȡ,�Ա������¶�ȡ.
		pmbFile >> nColumn >> nRow;		//��ȡ ��,��
		for (int j = 0; j != nRow; ++j)	//������
		{
			
			for (int i = 0; i != nColumn; ++i)	//������
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
	writePMB.open(filename);
	
	if (writePMB.is_open())
	{
		writePMB << strFileForm << endl;	//д���ļ���ʽ
		if (strComment != "") //���ע��Ϊ��Ϊ��,д��ע��.
		{
			writePMB << strComment << endl;	
		}
		writePMB << nColumn << " " << nRow;		//д�����о���
		for (vector<vector<int>>::iterator iterRow = vecColumn.begin(); iterRow != vecColumn.end(); ++iterRow)	//������
		{
			vecRow = *iterRow;	//���з������
			writePMB << endl;	//�Ȼ�����������������Ա����ļ�����п��С�
			for (vector<int>::iterator iterColunm = vecRow.begin(); iterColunm != vecRow.end(); ++iterColunm)	//������
			{
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