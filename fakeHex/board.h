#pragma once
#include"tool.h"
#include "disjointSets.h"
#include "UCTSearch.h"
class board {
public:
	disjointSets * judge=NULL;
	UCTSearch* AI = NULL;
	int step;
	static board* game_board;
	PLAYERS firstHand;
	PLAYERS right;
	PLAYERS* boardState;//∆Â≈Ã
	static board* getBoard();
	PLAYERS rightChange();
	PLAYERS gamestate();
	void initBoard(PLAYERS firstHand);
	void loadBoard(PLAYERS* boardState, PLAYERS firstHand);
	bool putChess(int row, int column,PLAYERS who);
	char getAIStep();
	
};
