#pragma once
#include"board.h"
#include"Node.h"
#include"tool.h"
#include"thread"
#include <concurrent_vector.h>
#include <future>
class UCTSearch {
public:
	Node * rootNode;
	int bestMovement;
	atomic_long allcount;
	void UCTSearchMove(PLAYERS* hex_board, int timeLimit, int m_board_size, disjointSets* judge);
	UCTSearch();
	Node* TreePolicy(Node* selectedNode);
	int DefaultPolicy(Node* selectedNode);
	~UCTSearch();
	void mutlyThreadSearch(int timeLimit, Node* rootNode, int threadid);
	int UCTSearch::DefaultPolicy_nothread(Node * selectedNode);

};
