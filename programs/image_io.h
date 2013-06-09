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
	pmb(){strFileForm = "";strComment = ""; nColumn = 0; nRow = 0;}	//Ĭ�Ϲ��캯��,������ⲻ�Ǻܺ�.
private:
	string strFileForm;
	string strComment;
	int nColumn;
	int nRow;
	vector<int> vecRow;
	vector<vector<int>> vecColumn;
};

#endif