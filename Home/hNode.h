
#pragma once
#ifndef HNODE_H
#define HNODE_H

class HNode {
public:
	int weight;                 //权重
	//这里的全是结点在hTNodes中的编号
	int parent;
	int rchild;
	int lchild;

	HNode(int weight, int parent = 0, int rchild = 0, int lchild = 0) :
		weight(weight), parent(parent), rchild(rchild), lchild(lchild) {}
};

#endif // !HNODE_H
