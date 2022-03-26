#include"tool.h"
#include "stdafx.h"
int arraysize = 121;
int boardSize = 11;
int playerMove;
int move_;
string i2s(int i) { // ½«int ×ª»»³Éstring
	stringstream s;
	s << i;
	return s.str();
}
string getTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%H:%M:%S", localtime(&timep));
	string res = tmp;
	return "[" + res + "]";
}