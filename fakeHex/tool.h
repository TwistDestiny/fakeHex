#pragma once

#include"sstream"
#include"string"
#include"time.h"
#include <iostream>
using namespace std;
enum PLAYERS
{
	NOBODY = 0, PLAYER, COMPUTER, CONFIRMED
};
const int MAX_THREADS = 2; 
extern int step;
extern int playerMove;
extern int move_;
extern int arraysize;
extern int boardSize;
extern string i2s(int i);
extern string getTime();