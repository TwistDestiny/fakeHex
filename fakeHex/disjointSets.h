#pragma once
#include<vector>
#include "tool.h"
class disjointSets {
public:
	short int right;
	short* disjointAI;
	short* disjointPLAYER;
	void createdisjoint(short* disjointai, short* disjointplayer, short movement, int right);
	void addMovement(short movement);
	int isconnected();
	disjointSets();
	~disjointSets();
	disjointSets* copy();
	int find(int p);

};