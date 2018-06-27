
//���������������붨��
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
vector<Tuple *> res;                   //ȫ�ֱ���, ���ͳ���ļ��Ľ��, �����ַ����ֵĸ���
vector<HNode *> hTNodes;               //�洢���������ĸ������, �����ս��
vector<string> HTCode;                 //�洢�����������
int result[MAX] = { 0 };               //ͳ���ַ�������ֵĴ���, ���ַ���asc��Ϊ�����±�
static int s1 = 0;           
static int s2 = 0;

void loadFile(const char * path) {
	//�����ļ�������
	ifstream infile(path, ios_base::in | ios_base::binary);
	//�õ��ļ�����
	infile.seekg(0, infile.end);
	long size = infile.tellg();
	infile.seekg(0);
	char * buffer = new char[size];
	//���ļ����ݶ�ȡ���ڴ���
	infile.read(buffer, size);

	//ͳ��buffer�е�����
	for (int i = 0; i < size; i++) {
		//char �� int ��������ʽ����ת��, �����뻻�з�
		result[buffer[i]]++;
	}
	for (int i = 32; i < MAX; i++) {
		//����ж�����������
		if (result[i] != 0) {
			res.push_back(new Tuple((char)i, result[i]));
		}
	}

	infile.close();
	delete[] buffer;
}

void printFile() {
	cout << "�ļ��и����ַ���Ȩ��:   " << endl;
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

//�� hTNodes ������Ѱ������ weight ��С�ı��, ������weight��ͬʱ, ȡascС��������
//ע�� s1, s2 �������±�, ����ֵ
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
			//��¼����С������λ�� 
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
	//���������Ȩ�����ʱ, ȡ��Ž�С�Ľ��
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
	HNode * root;                        //���������ĸ����

	HTree();                             //���캯��
	HTree(const HTree * t);              //���ƹ��캯��
	//~HTree();                            //��������

	void hufumanCoding();
	void prinTree();
	void printCoding();
	void inCoding(const char * inPath, const char * outPath, const char * resultPath); //�����Ѿ�����Ĺ����������ԭ�Ľ��б���ͽ���
};

HTree::HTree() {
	root = NULL;
}

void HTree::printCoding() {
	cout << "�����ַ���Ȩ�ؼ�����:   " << endl;
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
	cout << "�����Ĺ�������:   " << endl;
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
	//�ȹ������н��, һ�� 2n - 1 ��
	int count = 2 * n - 1;
	int i = 0;
	//n ����Ҷ 
	for (i = 0; i < n; i++) {
		hTNodes.push_back(new HNode(res[i]->getWeight()));
	}
	for (; i < count; i++) {
		hTNodes.push_back(new HNode(0));
	}
	//������������
	//�� hTNodes[0 ... j-1] ��ѡ�� parent Ϊ 0,(������ǽ���Ƿ�ѡ���), �� weight ��С���������, ������� s1, s2
	for (int j = n; j < count; j++) {
		//��ֵ������
		select(j);
		hTNodes[s1]->parent = j;
		hTNodes[s2]->parent = j;
		hTNodes[j]->lchild = s1;
		hTNodes[j]->rchild = s2;
		hTNodes[j]->weight = hTNodes[s1]->weight + hTNodes[s2]->weight;
	}
	//����ʹ���� 0, ������Ҫ��Ϊ�ı�����ĸ������, ����Ϊ -1,
	hTNodes.back()->parent = -1;
	//��Ҷ�ӵ���������ÿ���ַ��Ĺ���������
	char * cd = new char[n * sizeof(char)];          //���������Ŀռ�
	cd[n - 1] = '\0';                                //���������
	for (int i = 0; i < n; i++) {                    //����ַ������������
		int start = n - 1;
		int c = i;                                   //�����Ĺ�����, ��¼��һ���ĸ����ı��
		int f = hTNodes[i]->parent;                  //�õ���ǰ���ĸ����ı��
		//����ʹ���� 0, ������Ҫ��Ϊ�ı�����ĸ������, ����Ϊ -1 
		for (; f >= 0;) {
			if (hTNodes[f]->lchild == c) {
				cd[--start] = '0';
			}
			else {
				cd[--start] = '1';
			}
			c = f;
			f = hTNodes[f]->parent;                  //�����õ������, ֱ�������ı��Ϊ 0
		}
		//���ȥ���ǵ�ַ, ������������ʱ, ��û����
		//���Ը�Ϊ�����ַ���, �����ڶ�����Ķ���, ��û����ջ����, ���Ե���������ʱ, ���޹�ϵ
		string s = &cd[start];
		HTCode.push_back(s);
	}
	delete[] cd;

}

//�Ӹ��������������������еĶ����ƴ��룻������Ϊ0���������ӣ����������Һ��ӣ�
//һ������Ҷ�ӽ�㣬���ݶ�Ӧ��id��ɸ���res[]�ҳ���Ӧ���ַ���Ȼ�������´Ӹ���㿪ʼ�������룬ֱ�������Ƶ��Ľ���
void HTree::inCoding(const char * inPath, const char * outPath, const char * resultPath) {
	//���ȶ�ԭ�ĵ�ÿһ���ַ�����
	//�����ļ�������
	ifstream infile(inPath, ios_base::in | ios_base::binary);
	ofstream outfile(outPath, ios_base::out | ios_base::binary);
	ofstream resfile(resultPath, ios_base::out | ios_base::binary);
	//�õ��ļ�����
	infile.seekg(0, infile.end);
	long size = infile.tellg();
	infile.seekg(0);
	char * buffer = new char[size];
	//���ļ����ݶ�ȡ���ڴ���
	infile.read(buffer, size);
  
	//��ν��п��ٱ���
	string s;
	int location = 0;
	for (int i = 0; i < size; i++) {
		//�����۰����
		//buffer[i]���ƻ�����������з�...��ô��
		location = searchBin(buffer[i]);
		//ƴ��
		if (location >= 0) {
			s.append(HTCode[location]);
		}
	}
	outfile.write(s.c_str(), s.length());
	outfile.close();

	//��������
	string resultString;
	int sub = hTNodes.size() - 1;          //�õ������ı��
	for (int i = 0; i < s.length(); i++) {
		//֤������Ҷ��
		if (hTNodes[sub]->lchild == 0 && hTNodes[sub]->rchild == 0) {
			resultString += res[sub]->getKey();
			//������ͷ��㿪ʼ����
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












