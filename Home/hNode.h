
#pragma once
#ifndef HNODE_H
#define HNODE_H

class HNode {
public:
	int weight;                 //Ȩ��
	//�����ȫ�ǽ����hTNodes�еı��
	int parent;
	int rchild;
	int lchild;

	HNode(int weight, int parent = 0, int rchild = 0, int lchild = 0) :
		weight(weight), parent(parent), rchild(rchild), lchild(lchild) {}
};

#endif // !HNODE_H
