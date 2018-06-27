
//哈弗曼树的声明与定义
#pragma once
#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include "tuple.h"
#include "hNode.h"
using namespace std;

const int MAX = 125;
vector<Tuple *> res;                   //全局变量, 存放统计文件的结果, 各个字符出现的个数
vector<HNode *> hTNodes;               //存储哈弗曼树的各个结点, 包括空结点
vector<string> HTCode;                 //存储哈弗曼编码表
int result[MAX] = { 0 };               //统计字符及其出现的次数, 以字符的asc作为数组下标
static int s1 = 0;           
static int s2 = 0;

void loadFile(const char * path) {
	//建立文件输入流
	ifstream infile(path, ios_base::in | ios_base::binary);
	//得到文件长度
	infile.seekg(0, infile.end);
	long size = infile.tellg();
	infile.seekg(0);
	char * buffer = new char[size];
	//将文件内容读取到内存中
	infile.read(buffer, size);

	//统计buffer中的内容
	for (int i = 0; i < size; i++) {
		//char 到 int 进行了隐式类型转换, 不计入换行符
		result[buffer[i]]++;
	}
	for (int i = 32; i < MAX; i++) {
		//这个判断条件有问题
		if (result[i] != 0) {
			res.push_back(new Tuple((char)i, result[i]));
		}
	}

	infile.close();
	delete[] buffer;
}

void printFile() {
	cout << "文件中各个字符的权重:   " << endl;
	cout.setf(ios_base::left);
	cout << setw(10) << "id";
	cout << setw(10) << "char";
	cout << setw(10) << "weight";
	cout << endl;
	for (int i = 0; i < res.size(); i++) {
		cout << setw(10) << i;
		cout << setw(10) << res[i]->getKey();
		cout << setw(10) << res[i]->getWeight();
		cout << endl;
	}
}

//从 hTNodes 数组中寻找两个 weight 最小的编号, 当几个weight相同时, 取asc小的那两个
//注意 s1, s2 是数组下标, 不是值
void select(int up) {
	int min1 = 10000;
	int min2 = 10000;
	int min3 = 10000;
	int location1 = 0;
	int location2 = 0;
	int location3 = 0;
	for (int i = 0; i < up; i++) {
		if ((hTNodes[i]->parent == 0) && (hTNodes[i]->weight < min1)) {
			min1 = hTNodes[i]->weight;
			//记录下最小的数的位置 
			location1 = i;
		}
	}
	for (int i = 0; i < location1; i++) {
		if ((hTNodes[i]->parent == 0) && (hTNodes[i]->weight < min2)) {
			min2 = hTNodes[i]->weight;
			location2 = i;
		}
	}
	for (int i = location1 + 1; i < up; i++) {
		if ((hTNodes[i]->parent == 0) && (hTNodes[i]->weight < min3)) {
			min3 = hTNodes[i]->weight;
			location3 = i;
		}
	}

	s1 = location1;
	//当两个结点权重相等时, 取编号较小的结点
	if (min2 <= min3) {
		s2 = location2;
	}
	else {
		s2 = location3;
	}
}

int searchBin(char key) {
	int low = 0;
	int high = res.size() - 1;
	int mid = -1;
	while (low <= high) {
		mid = (low + high) / 2;
		if (res[mid]->getKey() == key) {
			return mid;
		}
		else if (res[mid]->getKey() > key) {
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}
	return -1;
}

class HTree {
public:
	HNode * root;                        //哈弗曼树的根结点

	HTree();                             //构造函数
	HTree(const HTree * t);              //复制构造函数
	//~HTree();                            //析构函数

	void hufumanCoding();
	void prinTree();
	void printCoding();
	void inCoding(const char * inPath, const char * outPath, const char * resultPath); //利用已经求出的哈弗曼编码对原文进行编码和解码
};

HTree::HTree() {
	root = NULL;
}

void HTree::printCoding() {
	cout << "各个字符的权重及编码:   " << endl;
	cout.setf(ios_base::left);
	cout << setw(10) << "id";
	cout << setw(10) << "char";
	cout << setw(10) << "weight";
	cout << setw(10) << "coding";
	cout << endl;
	for (int i = 0; i < HTCode.size(); i++) {
		cout << setw(10) << i;
		cout << setw(10) << res[i]->getKey();
		cout << setw(10) << res[i]->getWeight();
		cout << setw(10) << HTCode[i].c_str();
		cout << endl;
	}
}

void HTree::prinTree() {
	cout << "建立的哈弗曼树:   " << endl;
	cout.setf(ios_base::left);
	cout << setw(10) << "id";
	cout << setw(10) << "weight";
	cout << setw(10) << "parent";
	cout << setw(10) << "lchild";
	cout << setw(10) << "rchild";
	cout << endl;
	for (int i = 0; i < hTNodes.size(); i++) {
		cout << setw(10) << i;
		cout << setw(10) << hTNodes[i]->weight;
		cout << setw(10) << hTNodes[i]->parent;
		cout << setw(10) << hTNodes[i]->lchild;
		cout << setw(10) << hTNodes[i]->rchild;
		cout << endl;
	}
}

void HTree::hufumanCoding() {
	int n = res.size();
	cout << "n:  " << n << endl;
	if (n < 1)
		return;
	//先构造所有结点, 一共 2n - 1 个
	int count = 2 * n - 1;
	int i = 0;
	//n 个树叶 
	for (i = 0; i < n; i++) {
		hTNodes.push_back(new HNode(res[i]->getWeight()));
	}
	for (; i < count; i++) {
		hTNodes.push_back(new HNode(0));
	}
	//创建哈弗曼树
	//在 hTNodes[0 ... j-1] 中选择 parent 为 0,(用来标记结点是否被选择过), 且 weight 最小的两个结点, 其序号是 s1, s2
	for (int j = n; j < count; j++) {
		//传值的问题
		select(j);
		hTNodes[s1]->parent = j;
		hTNodes[s2]->parent = j;
		hTNodes[j]->lchild = s1;
		hTNodes[j]->rchild = s2;
		hTNodes[j]->weight = hTNodes[s1]->weight + hTNodes[s2]->weight;
	}
	//由于使用了 0, 所以我要人为改变根结点的父结点编号, 令其为 -1,
	hTNodes.back()->parent = -1;
	//从叶子到根逆向求每个字符的哈弗曼编码
	char * cd = new char[n * sizeof(char)];          //分配求编码的空间
	cd[n - 1] = '\0';                                //编码结束符
	for (int i = 0; i < n; i++) {                    //逐个字符求哈弗曼编码
		int start = n - 1;
		int c = i;                                   //遍历的过程中, 记录上一步的父结点的编号
		int f = hTNodes[i]->parent;                  //得到当前结点的父结点的编号
		//由于使用了 0, 所以我要人为改变根结点的父结点编号, 令其为 -1 
		for (; f >= 0;) {
			if (hTNodes[f]->lchild == c) {
				cd[--start] = '0';
			}
			else {
				cd[--start] = '1';
			}
			c = f;
			f = hTNodes[f]->parent;                  //继续得到父结点, 直到父结点的编号为 0
		}
		//存进去的是地址, 当本函数结束时, 都没有了
		//所以改为传递字符串, 建立在堆上面的对象, 并没有在栈上面, 所以当函数结束时, 毫无关系
		string s = &cd[start];
		HTCode.push_back(s);
	}
	delete[] cd;

}

//从根结点出发，逐个读入电文中的二进制代码；若代码为0则走向左孩子，否则走向右孩子；
//一旦到达叶子结点，根据对应的id便可根据res[]找出对应的字符。然后又重新从根结点开始继续译码，直到二进制电文结束
void HTree::inCoding(const char * inPath, const char * outPath, const char * resultPath) {
	//首先对原文的每一个字符进行
	//建立文件输入流
	ifstream infile(inPath, ios_base::in | ios_base::binary);
	ofstream outfile(outPath, ios_base::out | ios_base::binary);
	ofstream resfile(resultPath, ios_base::out | ios_base::binary);
	//得到文件长度
	infile.seekg(0, infile.end);
	long size = infile.tellg();
	infile.seekg(0);
	char * buffer = new char[size];
	//将文件内容读取到内存中
	infile.read(buffer, size);
  
	//如何进行快速编码
	string s;
	int location = 0;
	for (int i = 0; i < size; i++) {
		//进行折半查找
		//buffer[i]估计会存在两个换行符...怎么办
		location = searchBin(buffer[i]);
		//拼接
		if (location >= 0) {
			s.append(HTCode[location]);
		}
	}
	outfile.write(s.c_str(), s.length());
	outfile.close();

	//进行译码
	string resultString;
	int sub = hTNodes.size() - 1;          //得到根结点的编号
	for (int i = 0; i < s.length(); i++) {
		//证明到了叶子
		if (hTNodes[sub]->lchild == 0 && hTNodes[sub]->rchild == 0) {
			resultString += res[sub]->getKey();
			//继续从头结点开始遍历
			sub = hTNodes.size() - 1; 
		}
		if (s[i] == '0') {
			sub = hTNodes[sub]->lchild;
		}
		else {
			sub = hTNodes[sub]->rchild;
		}
	}
	resfile.write(resultString.c_str(), resultString.length());


	delete[] buffer;
	infile.close();
	resfile.close();
}


#endif // ! HUFFMAN_H












