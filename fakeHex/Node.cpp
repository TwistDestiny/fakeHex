#include"stdafx.h"
disjointSets* Node::judge = NULL;//用static成员变量必须要初始化
PLAYERS* Node::fatherBoard = NULL;
double ucb(double winrate, double allcount, double thiscount) {
	static const double c = 0.4;
	return winrate + c * sqrt(log(allcount) / thiscount);
}
double rave(double winrate, double winrate_rave, double allcount, double thiscount) {
	static const double c = 0.4;//0.33 0.35 0.4 0.5 0.6 
	static const double K = 3700;// 3500 3700 3000 4000
	double beta = sqrt(K / (3 * allcount + K));
	return (1 - beta) * winrate + (beta)* winrate_rave + c * sqrt(log(allcount) / thiscount);
}
Node::Node(Node* fatherNode, PLAYERS p, char movement)
{
	//this->rootBoard = rootBoard;
	presentmovement = movement;
	chessRight = p;
	thiscount = 0;
	wincount = 0;
	this->fatherNode = fatherNode;
	//judge = NULL;
	sonNodeSizes = 0;
	sonNodes = NULL;
	unExpansionNum = 0;
}

Node * Node::expansion()
{
	int size = 0;
	Node** Local_sonNodes = NULL;
	if (sonNodes.load() == NULL && this->IsWinnerComputer() == NOBODY) {
		int size_ = arraysize;
		char* rootBoard = getBoard();
		char* arraynode = new char[size_];
		for (int i = 0; i < size_; i++) {
			arraynode[i] = i;
			if (rootBoard[i] == NOBODY)size++;
		}
		sonNodeSizes = size;
		//unExpansionNum = size;
		int ptr = 0;
		Local_sonNodes = new Node*[sonNodeSizes];
		for (int j = 0; j < size_; j++) {
			int ptr2 = rand() % (size_ - j);
			int i = arraynode[ptr2];
			swap(arraynode[ptr2], arraynode[size_ - j - 1]);
			if (rootBoard[i] == NOBODY) {
				//	PLAYERS* sonBoard = new PLAYERS[size_]();
				//	for (int j = 0; j < size_; j++) sonBoard[j] = rootBoard[j];
				//	sonBoard[i] = (chessRight == COMPUTER ? PLAYER : COMPUTER);
				Node* node = new Node(this, (chessRight == COMPUTER ? PLAYER : COMPUTER), i);
				//		disjointSets* judge_ = judge->copy();
				//		judge_->right = node->chessRight;
				//		judge_->addMovement(i);
				//		node->judge = judge_;
				Local_sonNodes[ptr] = node;
				ptr++;
			}
		}
		Node** ptr_ = NULL;
		if (!sonNodes.compare_exchange_strong(ptr_, Local_sonNodes)) {
			for (int i = 0; i < size; i++) {
				delete Local_sonNodes[i];
				Local_sonNodes[i] = NULL;
			}
			delete[]Local_sonNodes;
		}
		delete[] rootBoard;
		delete[] arraynode;
	}
	int r = unExpansionNum.load();
	unExpansionNum++;
	if (r >= sonNodeSizes)
		return NULL;
	return sonNodes[r];
}
//todo:check 棋权取反是否会在多线程中产生问题；
Node * Node::selection()
{
	int size = sonNodeSizes;
	int bestChildId = 0;
	double maxScore = 0, score = 0;
	for (int i = 0; i < size; i++) {
		double winRate = (sonNodes[i]->chessRight == COMPUTER) ?
			(double)sonNodes[i]->wincount / sonNodes[i]->thiscount :
			1 - (double)sonNodes[i]->wincount / sonNodes[i]->thiscount;
		double winRate_rave = (sonNodes[i]->chessRight == COMPUTER) ?
			(double)sonNodes[i]->wincount_rave / sonNodes[i]->thiscount_rave :
			1 - (double)sonNodes[i]->wincount_rave / sonNodes[i]->thiscount_rave;
		score = ucb(winRate, this->thiscount, sonNodes[i]->thiscount);
		//score = rave(winRate, wincount_rave, this->thiscount_rave, sonNodes[i]->thiscount_rave);
		if (maxScore < score) {
			bestChildId = i;
			maxScore = score;
		}
	}
	//	printf("sssssss\n");
	return sonNodes[bestChildId];
}

Node* Node::getWinrateMax()
{
	int size = sonNodeSizes;
	int bestChildId = 0;
	double maxScore = 0, score = 0;
	for (int i = 0; i < size; i++) {
		double winRate = (sonNodes[i]->chessRight == COMPUTER) ?
			(double)sonNodes[i]->wincount_rave / sonNodes[i]->thiscount_rave :
			1 - (double)sonNodes[i]->wincount_rave / sonNodes[i]->thiscount_rave;
		score = winRate;
		if (maxScore < score) {
			bestChildId = i;
			maxScore = score;
		}
	}
	//	printf("sssssss\n");
	return sonNodes[bestChildId];
}

Node* Node::getWinrateMax_other()
{
	int size = sonNodeSizes;
	int bestChildId = 0;
	double maxScore = 0, score_0 = 0, count_max;
	for (int i = 0; i < size; i++) {
		double winRate = (sonNodes[i]->chessRight == COMPUTER) ?
			(double)sonNodes[i]->wincount / sonNodes[i]->thiscount :
			1 - (double)sonNodes[i]->wincount / sonNodes[i]->thiscount;
		score_0 = winRate;
		if (maxScore < score_0) {
			bestChildId = i;
			maxScore = score_0;
		}
	}
	//	printf("sssssss\n");
	return sonNodes[bestChildId];
}
Node* Node::getCountMax_other() {

	int size = sonNodeSizes;
	int bestChildId = 0;
	int maxScore = 0, score_0 = 0, count_max;
	for (int i = 0; i < size; i++) {
		count_max = sonNodes[i]->thiscount;
		if (maxScore < count_max) {
			bestChildId = i;
			maxScore = count_max;
		}
	}
	//	printf("sssssss\n");
	return sonNodes[bestChildId];
}
Node* Node::getCountMax() {

	int size = sonNodeSizes;
	int bestChildId = 0;
	int maxScore = 0, score_0 = 0, count_max;
	for (int i = 0; i < size; i++) {
		count_max = sonNodes[i]->thiscount_rave;
		if (maxScore < count_max) {
			bestChildId = i;
			maxScore = count_max;
		}
	}
	//	printf("sssssss\n");
	return sonNodes[bestChildId];
}
void Node::backUp(int winner)
{
	Node *temp = this;

	while (temp) {

		temp->thiscount += 1; //访问次数+MAX_THREADS
		temp->wincount += winner; //收益增加 
		temp->thiscount_rave += 1; //访问次数+MAX_THREADS
		temp->wincount_rave += winner; //收益增加 
		temp = temp->fatherNode;
		if (temp != NULL) {
			Node* rave_node;
			for (int i = 0; i < temp->sonNodeSizes; i++) {
				if (temp->sonNodes[i]->presentmovement == this->presentmovement) {
					rave_node = temp->sonNodes[i];
					rave_node->thiscount_rave += 1;
					rave_node->wincount_rave += winner;
					//	temp->wincount_rave += winner;
				}
			}
		}

	}
	temp = NULL;
}
/*
int unExpansionptr = rand() % unExpansionNum;
Node *node = sonNodes.at(unExpansionptr);
node->thiscount++;
swap(sonNodes.at(unExpansionptr), sonNodes.at(unExpansionNum));
unExpansionNum--;*/
PLAYERS Node::MakeRandomMovement()
{
	disjointSets* judge_ptr = getjudge();
	char* rootBoard = getBoard();
	//	char* virboard = new char[arraysize];
	int size = arraysize;
	int size_ = 0;
	char* nochessboard = new char[arraysize];
	for (int i = 0; i < size; i++) {
		if (rootBoard[i] == NOBODY) {
			nochessboard[size_] = i;
			size_++;
		}
		//	virboard[i] = rootBoard[i];
	}
	char currentMove = 126;
	//char* aitree=new char[size];
	//char* palyertree=new char[size];
	//for (int i = 0; i < size; i++) {
	//	aitree[i] = judge->disjointAI[i];
	//	palyertree[i] = judge->disjointPLAYER[i];
	//}
	PLAYERS player = NOBODY;
	judge_ptr->right = chessRight;
	while (player == NOBODY) {
		judge_ptr->right = (judge_ptr->right == COMPUTER ? PLAYER : COMPUTER);
		if (size_ != 0) {
			int count = 0;
			int ptr = rand() % size_;
			//while (step>30&&currentMove != 126 && abs(nochessboard[ptr] - currentMove) / boardSize > 3 && abs(nochessboard[ptr] % boardSize - currentMove% boardSize) > 3 && count < 3) {
			//	count++;
			//	ptr = rand() % size_;
			//}
			while (currentMove != 126 && abs(nochessboard[ptr] - currentMove) / boardSize > 4 && abs(nochessboard[ptr] % boardSize - currentMove % boardSize) > 4 && count < 9) {
				count++;
				ptr = rand() % size_;
			}
			//if (count != 0)
			//	cout << "xiuzheng" << endl;
			judge_ptr->addMovement(nochessboard[ptr]);
			std::swap(nochessboard[ptr], nochessboard[size_ - 1]);
			rootBoard[nochessboard[size_ - 1]] = judge_ptr->right;
			currentMove = nochessboard[size_ - 1];
			size_--;
		}
		else {
			judge_ptr->addMovement(nochessboard[0]);
			printf("judgeproblem!");
		}
		player = IsWinnerComputer(judge_ptr);
		if (player == NOBODY) {
			PLAYERS _right = (PLAYERS)judge_ptr->right;
			char ans = isvirtualconnected(_right, rootBoard, currentMove);
			if (ans != 127)
				player = _right;
		}
		//delete judge_ptr;
	}
	//for (int i = 0; i < size; i++) {
	//	judge->disjointAI[i] = aitree[i];
	//	judge->disjointPLAYER[i] = palyertree[i];
	//}
	//judge->right = Right;
	delete[] rootBoard;
	delete[] nochessboard;
	delete judge_ptr;
	//delete[] virboard;
	//delete[]  aitree;
	//delete[]  palyertree;
	return player;
}

PLAYERS Node::IsWinnerComputer(disjointSets * judge_ptr)
{
	PLAYERS result = (PLAYERS)judge_ptr->isconnected();
	return result;
}

Node::~Node()
{
	//delete[] rootBoard;
	//rootBoard = NULL;
	int size = sonNodeSizes;
	if (sonNodes.load() != NULL) {
		for (int i = 0; i < size; i++) {
			delete sonNodes[i];
			sonNodes[i] = NULL;
		}
	}

	//delete[] sonNodes.load();
	//delete judge;
	//judge = NULL;

}

Node * Node::copy()
{
	//mu.lock();
	Node* copyed = new Node();
	copyed->sonNodes = NULL;
	//copyed->judge=judge->copy();
	//	PLAYERS* rootBoard_ = new PLAYERS[arraysize]();
	/*for (int i = 0; i < arraysize; i++) {
	rootBoard_[i] = rootBoard[i];
	}*/
	//	rootBoard_->assign(rootBoard->begin(), rootBoard->end()); 
	//copyed->rootBoard = rootBoard_;
	copyed->presentmovement = presentmovement;
	copyed->fatherNode = fatherNode.load();
	//copyed->unExpansionNum = presentmovement;
	//unsigned int thiscount;
	//double winRate;
	//double delta;
	//unsigned int wincount;
	copyed->chessRight = chessRight;
	//	mu.unlock();
	return copyed;
}

Node::Node()
{
}

PLAYERS Node::IsWinnerComputer()
{
	disjointSets* judge_ptr = this->getjudge();
	PLAYERS result = (PLAYERS)judge_ptr->isconnected();
	delete judge_ptr;
	return result;
}
Node ** Node::gengratesonNodes()
{
	return nullptr;
}
disjointSets * Node::getjudge()
{
	char* rootBoard = getBoard();
	disjointSets *ptr = judge->copy();
	for (int i = 0; i < arraysize; i++) {
		if (rootBoard[i] != fatherBoard[i]) {
			ptr->right = rootBoard[i];
			ptr->addMovement(i);
		}
	}
	delete[] rootBoard;
	return ptr;
}

char* Node::getBoard()
{
	char* p_ptr = new char[arraysize];
	for (int i = 0; i < arraysize; i++)
		p_ptr[i] = fatherBoard[i];
	Node* temp = this;
	PLAYERS countRight = chessRight;
	while (temp != NULL) {
		p_ptr[temp->presentmovement] = countRight;
		countRight = countRight == COMPUTER ? PLAYER : COMPUTER;
		temp = temp->fatherNode;
	}
	return p_ptr;
}
char Node::isvirtualconnected(PLAYERS j_right, char* board, char currentMove) {
	bool left = false, right = false;
	char v2real = 127;
	char vir_move = 126;
	point p;
	p.movement = currentMove;
	p.column = currentMove % boardSize;
	p.row = currentMove / boardSize;
	check_connected(v2real, vir_move, j_right, left, right, p, board);
	for (int i = 0; i < boardSize*boardSize; i++)
		if (board[i] == CONFIRMED)
			board[i] = j_right;
	if (left&&right)
		return v2real != 127 ? v2real : 126;
	else return 127;
}
void Node::check_connected(char& v2real, char& vir_move, PLAYERS jright, bool& left_up, bool& right_down, point p, char* board) {

	if (jright == PLAYER && p.column == boardSize - 1) {
		right_down = true;
		v2real = vir_move;
	}
	if (jright == PLAYER && p.column == 0) {
		left_up = true;
		v2real = vir_move;
	}
	if (jright == COMPUTER && p.row == boardSize - 1) {
		right_down = true;
		v2real = vir_move;
	}
	if (jright == COMPUTER && p.row == 0) {
		left_up = true;
		v2real = vir_move;
	}
	if (jright == PLAYER && p.column == 1 && p.row != boardSize - 1 && board[p.movement - 1] == NOBODY && board[p.movement + boardSize - 1] == NOBODY) {
		left_up = true;
		vir_move = p.movement - 1;
		v2real = vir_move;
	}

	if (jright == PLAYER && p.column == boardSize - 2 && p.row != 0 && board[p.movement + 1] == NOBODY && board[p.movement - boardSize + 1] == NOBODY) {
		right_down = true;
		vir_move = p.movement + 1;
		v2real = vir_move;
	}
	//---
	if (jright == COMPUTER && p.row == 1 && p.column != boardSize - 1 && board[p.movement - boardSize] == NOBODY && board[p.movement - boardSize + 1] == NOBODY) {
		left_up = true;
		vir_move = p.movement - boardSize + 1;
		v2real = vir_move;
	}

	if (jright == COMPUTER && p.row == boardSize - 2 && p.column != 0 && board[p.movement + boardSize] == NOBODY && board[p.movement + boardSize - 1] == NOBODY) {
		right_down = true;
		vir_move = p.movement + boardSize;
		v2real = vir_move;
	}
	//---
	if (p.column != boardSize - 1 && board[p.movement + 1] == jright) {//右
		board[p.movement + 1] = CONFIRMED;
		point p_ = p;
		p_.column++;
		p_.movement = p.movement + 1;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.row != 0 && p.column != boardSize - 1 && board[p.movement + 1 - boardSize] == jright) {//右上
		board[p.movement + 1 - boardSize] = CONFIRMED;
		point p_ = p;
		p_.column++;
		p_.row--;
		p_.movement = p.movement + 1 - boardSize;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.row != 0 && board[p.movement - boardSize] == jright) {//上
		board[p.movement - boardSize] = CONFIRMED;
		point p_ = p;
		p_.row--;
		p_.movement = p.movement - boardSize;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.row != boardSize - 1 && board[p.movement + boardSize] == jright) {//下
		board[p.movement + boardSize] = CONFIRMED;
		point p_ = p;
		p_.row++;
		p_.movement = p.movement + boardSize;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.column != 0 && board[p.movement - 1] == jright) {//左
		board[p.movement - 1] = CONFIRMED;
		point p_ = p;
		p_.column--;
		p_.movement = p.movement - 1;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.row != boardSize - 1 && p.column != 0 && board[p.movement - 1 + boardSize] == jright) {//左下
		board[p.movement - 1 + boardSize] = CONFIRMED;
		point p_ = p;
		p_.column--;
		p_.row++;
		p_.movement = p.movement - 1 + boardSize;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.row != 0 && p.column != 0 && board[p.movement - 1 - boardSize] == jright && board[p.movement - 1] == NOBODY && board[p.movement - boardSize] == NOBODY) {//column-1!=-1 row-1
		board[p.movement - 1 - boardSize] = CONFIRMED;
		point p_ = p;
		p_.column--;
		p_.row--;
		p_.movement = p.movement - 1 - boardSize;
		vir_move = p.movement - 1;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.row > 1 && p.column != boardSize - 1 && board[p.movement - boardSize] == NOBODY && board[p.movement + 1 - boardSize] == NOBODY && board[p.movement - 2 * boardSize + 1] == jright) {//column+1!=-1 row-2
		board[p.movement - 2 * boardSize + 1] = CONFIRMED;
		point p_ = p;
		p_.column++;
		p_.row -= 2;
		p_.movement = p.movement + 1 - 2 * boardSize;
		vir_move = p.movement + 1 - boardSize;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.row != 0 && p.column < boardSize - 2 && board[p.movement + 1] == NOBODY && board[p.movement + 1 - boardSize] == NOBODY && board[p.movement - boardSize + 2] == jright) {//column+2 row-1
		board[p.movement - boardSize + 2] = CONFIRMED;
		point p_ = p;
		p_.column += 2;
		p_.row--;
		p_.movement = p.movement - boardSize + 2;
		vir_move = p.movement + 1 - boardSize;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.column != boardSize - 1 && p.row != boardSize - 1 && board[p.movement + 1] == NOBODY && board[p.movement + boardSize] == NOBODY && board[p.movement + boardSize + 1] == jright) {//column+1 row+1
		board[p.movement + boardSize + 1] = CONFIRMED;
		point p_ = p;
		p_.column++;
		p_.row++;
		p_.movement = p.movement + boardSize + 1;
		vir_move = p.movement + boardSize;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.column != 0 && p.row < boardSize - 2 && board[p.movement + boardSize - 1] == NOBODY && board[p.movement + boardSize] == NOBODY && board[p.movement + 2 * boardSize - 1] == jright) {//column-1 row+2
		board[p.movement + 2 * boardSize - 1] = CONFIRMED;
		point p_ = p;
		p_.column--;
		p_.row += 2;
		p_.movement = p.movement + 2 * boardSize - 1;
		vir_move = p.movement + boardSize;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
	if (p.column > 1 && p.row != boardSize - 1 && board[p.movement + boardSize - 1] == NOBODY && board[p.movement - 1] == NOBODY && board[p.movement + boardSize - 2] == jright) {//column-2 row+1
		board[p.movement + boardSize - 2] = CONFIRMED;
		point p_ = p;
		p_.column -= 2;
		p_.row++;
		p_.movement = p.movement + boardSize - 2;
		vir_move = p.movement - 1;
		check_connected(v2real, vir_move, jright, left_up, right_down, p_, board);
	}
}
