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
		pmbFile >> ccomment;	//取出第二行第一个字符,如果是'#',则说明有注释行.
		if (ccomment == '#')
		{
			pmbFile.unget();	//Unget character
			getline(pmbFile,strComment);	//写入注释
		}
		else
			pmbFile.unget();	//没有注释行,忽略读取,以便后面从新读取.
		pmbFile >> nColumn >> nRow;		//读取 列,行
		for (int j = 0; j != nRow; ++j)	//处理行
		{
			
			for (int i = 0; i != nColumn; ++i)	//处理列
			{
				pmbFile >> ndata;		//因为不知道vector怎么处理流,便设了个中间变量用来push_back();
				vecRow.push_back(ndata);		
			} //一行处理完
			vecColumn.push_back(vecRow);
			vecRow.clear();	//清空每一行数据，以便处理新一行数据
		}

	}
	else
		cout << "can't open input file: " << filename << endl;	//读取文件失败提示用户信息.
	pmbFile.close();	//关闭文件
	cout << "Finsh reading input file." << endl;	//读取文件成果提示用户信息.

}

void pmb::writePMBFile(const string &filename)
{
	ofstream writePMB;
	writePMB.open(filename);
	
	if (writePMB.is_open())
	{
		writePMB << strFileForm << endl;	//写入文件格式
		if (strComment != "") //如果注释为不为空,写入注释.
		{
			writePMB << strComment << endl;	
		}
		writePMB << nColumn << " " << nRow;		//写入行列矩阵
		for (vector<vector<int>>::iterator iterRow = vecColumn.begin(); iterRow != vecColumn.end(); ++iterRow)	//处理行
		{
			vecRow = *iterRow;	//将行分离出来
			writePMB << endl;	//先换行再输出，这样可以避免文件最后有空行。
			for (vector<int>::iterator iterColunm = vecRow.begin(); iterColunm != vecRow.end(); ++iterColunm)	//处理列
			{
				writePMB << *iterColunm << " ";		//写入行
			}			
		}
	}
	else
		cout << "can't open the output file: " << filename  << endl;	//写入文件失败提示用户信息.
	writePMB.close();	//关闭文件.
	cout << "finsh write pmb file.\n" ;	//写入文件成果提示用户信息.
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