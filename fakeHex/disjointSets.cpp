#include "stdafx.h"
/*可以简化
*/
void disjointSets::createdisjoint(short* disjointai, short* disjointplayer, short movement, int right)
{
	this->right = right;
	int size = arraysize;
	disjointAI = new short[size]();
	for (int i = 0; i < size; i++)
		disjointAI[i] = (disjointai[i]);
	disjointPLAYER = new short[size]();
	for (int i = 0; i < size; i++)
		disjointPLAYER[i] = disjointplayer[i];
	if (movement == 127) {
		this->disjointAI = disjointai;
		this->disjointPLAYER = disjointplayer;
		return;
	}
	short* temp = (right == COMPUTER ? disjointAI : disjointPLAYER);
	temp[movement] = movement;
	int all = arraysize;
	int near__[6];
	near__[0] = movement - boardSize >= 0 ? movement - boardSize : 127;
	near__[1] = movement - boardSize + 1 >= 0 ? movement - boardSize + 1 : 127;
	near__[2] = movement + 1 < all ? movement + 1 : 127;
	near__[3] = movement + boardSize < all ? movement + boardSize : 127;
	near__[4] = movement + boardSize - 1 < all ? movement + boardSize - 1 : 127;
	near__[5] = movement - 1 >= 0 ? movement - 1 : 127;
	if (right == COMPUTER) {
		for (int i = 0; i < 6; i++) {
			if (near__[i] != 127 && temp[near__[i]] != 127) {
				int t = near__[i];
				//near__[i == 5 ? 0 : i + 1] = 127;
				//near__[i == 0 ? 5 : 1] = 127;  //当前结点被连接 相邻节点不用搜索
				int father = find(t);
				if (father == 127)printf("po");
				int rowfather = father / boardSize;
				int _father = find(movement);
				int rowthis = _father / boardSize;
				if (rowfather < rowthis) {
					temp[movement] = father;
					temp[t] = father;
					temp[_father] = father;
				}
				else {
					temp[t] = _father;
					temp[father] = _father;
				}
			}
		}
	}
	else {
		for (int i = 0; i < 6; i++) {
			if (near__[i] != 127 && temp[near__[i]] != 127) {
				int t = near__[i];
				//near__[i == 5 ? 0 : i + 1] = 127;
				//near__[i == 0 ? 5 : 1] = 127;  //当前结点被连接 相邻节点不用搜索
				int father = find(t);
				if (father == 127)printf("po");
				int columnfather = father % boardSize;
				int _father = find(movement);
				int columnthis = _father % boardSize;
				if (columnfather < columnthis) {
					temp[movement] = father;
					temp[t] = father;
					temp[_father] = father;
				}
				else {
					temp[t] = _father;
					temp[father] = _father;
				}
			}
		}
	}
	temp = NULL;
}
void disjointSets::addMovement(short movement)
{

	short* temp = (right == COMPUTER ? disjointAI : disjointPLAYER);
	if (temp[movement] < 121) {
		return;
	}
	temp[movement] = movement;
	int all = arraysize;
	int near__[6];
	near__[0] = movement - boardSize >= 0 ? movement - boardSize : 127;
	near__[1] = movement - boardSize + 1 >= 0 ? movement - boardSize + 1 : 127;
	near__[2] = movement + 1 < all ? movement + 1 : 127;
	near__[3] = movement + boardSize < all ? movement + boardSize : 127;
	near__[4] = movement + boardSize - 1 < all ? movement + boardSize - 1 : 127;
	near__[5] = movement - 1 >= 0 ? movement - 1 : 127;
	if (movement%boardSize == 0) {
		near__[5] = 127;
		near__[4] = 127;
	}
	if ((movement + 1) % boardSize == 0) {
		near__[2] = 127;
		near__[1] = 127;
	}
	if (right == COMPUTER) {
		for (int i = 0; i < 6; i++) {
			if (near__[i] != 127 && temp[near__[i]] != 127) {
				int t = near__[i];
				near__[i == 5 ? 0 : i + 1] = 127;
				near__[i == 0 ? 5 : 1] = 127;  //当前结点被连接 相邻节点不用搜索
				int father = find(t);
				if (father == 127)printf("po");
				int rowfather = father / boardSize;
				int _father = find(movement);
				int rowthis = _father / boardSize;
				if (rowfather < rowthis) {
					temp[movement] = father;
					temp[t] = father;
					temp[_father] = father;
				}
				else {
					temp[t] = _father;
					temp[father] = _father;
				}
			}
		}
	}
	else {
		for (int i = 0; i < 6; i++) {
			if (near__[i] != 127 && temp[near__[i]] != 127) {
				int t = near__[i];
				near__[i == 5 ? 0 : i + 1] = 127;
				near__[i == 0 ? 5 : 1] = 127;  //当前结点被连接 相邻节点不用搜索
				int father = find(t);
				if (father == 127)printf("po");
				int columnfather = father % boardSize;
				int _father = find(movement);
				int columnthis = _father % boardSize;
				if (columnfather < columnthis) {
					temp[movement] = father;
					temp[t] = father;
					temp[_father] = father;
				}
				else {
					temp[t] = _father;
					temp[father] = _father;
				}
			}
		}
	}

	temp = NULL;

}
int disjointSets::isconnected()
{
	if (right == COMPUTER) {
		for (int i = 0; i < boardSize; i++) {
			int t = i + boardSize * (boardSize - 1);
			if (find(t) != 127 && find(t) < boardSize) {
				return COMPUTER;
			}
		}
	}
	else {
		for (int i = 0; i < boardSize; i++) {
			int t = i * boardSize + boardSize - 1;
			if (find(t) != 127 && find(t) % boardSize == 0) {
				return PLAYER;
			}
		}
	}
	return NOBODY;
}
/*
int i = 0;
int row = i / board_size;
int column = i % board_size;
*/

disjointSets::disjointSets()
{
	disjointAI = NULL;
	disjointPLAYER = NULL;
}

disjointSets::~disjointSets()
{
	delete[] disjointAI;
	delete[] disjointPLAYER;
}

disjointSets * disjointSets::copy()
{
	disjointSets * temp = new disjointSets();
	temp->right = right;
	int size = arraysize;
	temp->disjointAI = new short[size]();
	for (int i = 0; i < size; i++) {
		temp->disjointAI[i] = disjointAI[i];
	}
	temp->disjointPLAYER = new short[size]();
	for (int i = 0; i < size; i++) {
		temp->disjointPLAYER[i] = disjointPLAYER[i];
	}
	return temp;
}

int disjointSets::find(int p)
{
	if (p == 127)
		return 127;
	short* temp = (right == COMPUTER ? disjointAI : disjointPLAYER);
	while (temp[p] != p && temp[p] != 127) {
		if (p == 127)
			return 127;
		p = temp[p];
	}
	temp = NULL;
	return p;
}
