#include"stdafx.h"
board* board::game_board=NULL;
board* board::getBoard()
{
	if (game_board == NULL)
		game_board = new board();
	return game_board;
}
PLAYERS board::rightChange()
{
	right = right == COMPUTER ? PLAYER : COMPUTER;
	return right;
}
/*
return 
NOBODY: Î´½áÊø
PLAYER £ºÍæ¼ÒÊ¤
COMPUTER£ºµçÄÔÊ¤
*/
PLAYERS board::gamestate()
{

	return (PLAYERS)(judge->isconnected());
}

void board::initBoard(PLAYERS firstHand)
{
	boardState = new PLAYERS[121];
	for (int i = 0; i < 121; i++)
		boardState[i] = NOBODY;
	this->firstHand = firstHand;
	judge = new disjointSets();
	short* disjointai = new short[arraysize];
	short* disjointplayer = new short[arraysize];
	for (int i = 0; i < arraysize; i++) {
		disjointai[i] = 127;
		disjointplayer[i] = 127;
	}
	judge->createdisjoint(disjointai, disjointplayer, 127, firstHand == PLAYER ? COMPUTER : PLAYER);
}

bool board::putChess(int row, int column,PLAYERS who)
{
	if (firstHand == PLAYER)
		swap(row, column);
	if (who == PLAYER) {
		playerMove = row * 11 + column;
		delete AI;
		AI = NULL;
	}
		
	if (boardState[row * 11 + column] == NOBODY) {
		boardState[row * 11 + column] = who;
		judge->right = (judge->right == COMPUTER ? PLAYER : COMPUTER);
		step++;
		judge->addMovement(row * 11 + column);
		return true;
	}
		return false;
}

char board::getAIStep()
{
	AI = new UCTSearch();
	disjointSets* temp = judge->copy();
	AI->UCTSearchMove(boardState, 10 * CLOCKS_PER_SEC, 121, temp);
	char node = AI->bestMovement;
	judge->right = (judge->right == COMPUTER ? PLAYER : COMPUTER);
	step++;
	judge->addMovement(node);
	boardState[node] = COMPUTER;
	move_ = node;
	delete temp;
	temp = NULL;
	int node_ = node;
	if (firstHand==PLAYER)
		node_ = (node%boardSize)*boardSize + node / boardSize;
	return node_;
}
