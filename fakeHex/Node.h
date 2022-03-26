#pragma once
#include<vector>
#include"tool.h"
#include"atomic"
#include"disjointSets.h"
struct  point { char column; char row; char movement; };
using namespace std;
class Node {
public:

	static disjointSets* judge;
	//PLAYERS* rootBoard;
	static PLAYERS* fatherBoard;
	char presentmovement;
	atomic_int unExpansionNum;
	atomic_int sonNodeSizes;
	atomic<Node**> sonNodes;
	atomic<Node*> fatherNode;
	atomic_uint thiscount;
	atomic_uint wincount;
	atomic_uint thiscount_rave;
	atomic_uint wincount_rave;
	PLAYERS chessRight;
	Node(Node* fatherNode, PLAYERS p, char movement);
	Node* expansion();
	Node* selection();
	Node* getWinrateMax();
	Node* getWinrateMax_other();
	Node* getCountMax_other();
	Node* getCountMax();
	void backUp(int winner);
	PLAYERS MakeRandomMovement();
	PLAYERS IsWinnerComputer(disjointSets* judge_ptr);
	PLAYERS IsWinnerComputer();
	void check_connected(char& v2real, char& vir_move, PLAYERS jright, bool& left_up, bool& right_down, point p, char* board);
	char isvirtualconnected(PLAYERS j_right, char* board, char currentMove);
	Node** gengratesonNodes();
	disjointSets* getjudge();
	char* getBoard();
	~Node();
	Node* copy();
	Node();
};
