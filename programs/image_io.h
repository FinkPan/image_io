#ifndef _IMAGE_IO_H_
#define _IMAGE_IO_H_

#include <string>
#include <vector>
using namespace std;

class pmb
{
public:
	void readPMBFile(const string &filename);
	void writePMBFile(const string &filename);
	pmb()
		: strFileFormat(""), nWidth(0), nHeigth(0) {}	//默认类构造函数
private:
	string strFileFormat;
	vector<string> strComment;
	int nWidth;
	int nHeigth;
	int nMaxColor;
	vector<int> vecRow;
	vector<vector<int>> vecColumn;
};

#endif