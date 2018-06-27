
//¹ş¸¥ÂüÊ÷²âÊÔ
#include "stdafx.h"
#include "huffman.h"
#include <iostream>

using namespace std;

int main() {

	const char * inPath = "infile.txt";
	const char * outPath = "outfile.txt";
	const char * resultPath = "result.txt";

	HTree * htree = new HTree();

	loadFile(inPath);
	printFile();

	htree->hufumanCoding();
	htree->prinTree();
	htree->printCoding();
	htree->inCoding(inPath, outPath, resultPath);

	cout << "done..." << endl;
	system("pause");
	return 0;
}