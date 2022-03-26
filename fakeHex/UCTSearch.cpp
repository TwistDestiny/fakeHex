#include"stdafx.h"
static Node* tempNode = NULL;
atomic_int allnodes = 0;
void UCTSearch::UCTSearchMove( PLAYERS* hex_board, int timeLimit, int m_board_size, disjointSets* judge)
{
	allnodes = 0;
	allcount = 0;
	int test = 0;
	int hexsize = 121;
	PLAYERS* rootboard = new PLAYERS[hexsize];
	char* rootboard_ = new char[hexsize];
	for (int i = 0; i < hexsize; i++) {
		rootboard[i] = hex_board[i];
		rootboard_[i] = hex_board[i];
		if (hex_board[i] != NOBODY)test++;
	}
	if (test == 0) {
		bestMovement = arraysize / 2;
		rootNode = NULL;
		return;
	}
	if (tempNode == NULL)
		rootNode = new Node(NULL, PLAYER, 127);
	else {
		rootNode = tempNode;
		//cout << "i am not null"<<endl;
	}

	int t = 121;
	rootNode->judge = judge;
	delete rootNode->fatherBoard;
	rootNode->fatherBoard = rootboard;
	/*for (int s = 0; s < arraysize; s++) {
		if (rootboard_[s] != PLAYER) {
			bestMovement = rootNode->isvirtualconnected(COMPUTER, rootboard_, s);
			if (bestMovement < 121) {
				cout << "vir ro real;" << endl;
				delete rootNode;
				rootNode = NULL;
				if (rootboard_[s] != COMPUTER)
					bestMovement = s;
				return;
			}
		}
		if (rootboard_[s] == NOBODY) {
			bestMovement = rootNode->isvirtualconnected(PLAYER, rootboard_, s);
			if (bestMovement < 121) {
				cout << "near lose!" << endl;
				bestMovement = s;
				delete rootNode;
				rootNode = NULL;
				return;
			}
		}
	}*/
	delete[] rootboard_;
	time_t startTime = clock();
	int i = 0;
	while (rootNode->IsWinnerComputer() != NOBODY) {
		bestMovement = 127;
		break;
	}
	//while (clock() - startTime <= timeLimit) { //尚未耗尽计算时长 
	//	Node* selectedNode = TreePolicy(rootNode);
	//	int result = DefaultPolicy(selectedNode); //运用模拟策略对选中节点进行一次随机模拟 
	//	selectedNode->backUp(result);
	//	i++;
	//}
	//vector<thread> pthread;
	//	for (int i = 0; i < MAX_THREADS; i++) {
	thread p1(&UCTSearch::mutlyThreadSearch, this, timeLimit, rootNode, 1);
	thread p2(&UCTSearch::mutlyThreadSearch, this, timeLimit, rootNode, 2);
	thread p3(&UCTSearch::mutlyThreadSearch, this, timeLimit, rootNode, 3);
	thread p4(&UCTSearch::mutlyThreadSearch, this, timeLimit, rootNode, 4);
	//thread p5(&UCTSearch::mutlyThreadSearch, this, timeLimit, rootNode, 5);
	//thread p6(&UCTSearch::mutlyThreadSearch, this, timeLimit, rootNode, 6);
	//thread p7(&UCTSearch::mutlyThreadSearch, this, timeLimit, rootNode, 7);
	//thread p8(&UCTSearch::mutlyThreadSearch, this, timeLimit, rootNode, 8);
	p1.join();
	p2.join();
	p3.join();
	p4.join();
	//p5.join();
	//p6.join();
	//p7.join();
	//p8.join();

	//	}
	//	printf("\nnodes:%d\n",i);
	printf("engine searched %d nodes\n", allnodes.load());

	bestMovement = rootNode->getWinrateMax_other()->presentmovement;
	int othermove1 = rootNode->getWinrateMax()->presentmovement;
	int othermove2 = rootNode->getCountMax()->presentmovement;
	int othermove3 = rootNode->getCountMax_other()->presentmovement;
	int othermove4 = rootNode->selection()->presentmovement;
	cout << "winratemaxnorave y=" << bestMovement / boardSize << "x=" << bestMovement % boardSize << endl;
	cout << "winratemaxrave y=" << othermove1 / boardSize << "x=" << othermove1 % boardSize << endl;
	cout << "countrave y=" << othermove2 / boardSize << "x=" << othermove2 % boardSize << endl;
	cout << "countnorave y=" << othermove3 / boardSize << "x=" << othermove3 % boardSize << endl;
	cout << "secti y=" << othermove4 / boardSize << "x=" << othermove4 % boardSize << endl;
	//double winrateai = (double)rootNode->getWinrateMax()->wincount / rootNode->getWinrateMax()->thiscount;
	//double winratepl = (double)rootNode->getWinrateMax()->getWinrateMax()->wincount / rootNode->getWinrateMax()->getWinrateMax()->thiscount;
	//printf("winrate ai is %lf", winrateai);
	//printf("winrate ai is %lf", winratepl);

}
void UCTSearch::mutlyThreadSearch(int timeLimit, Node * rootNode, int threadid)
{
	int i = 0;
	printf("\nstart thread%d\n", threadid);
	time_t startTime = clock();
	while (clock() - startTime <= timeLimit) { //尚未耗尽计算时长 
		Node* selectedNode = TreePolicy(rootNode);
		int result = DefaultPolicy_nothread(selectedNode); //运用模拟策略对选中节点进行一次随机模拟 
		selectedNode->backUp(result);
		i++;
	}
	printf("\nthread%d search nodes:%d\n", threadid, i);
	allnodes += i;
}

int UCTSearch::DefaultPolicy_nothread(Node * selectedNode)
{
	PLAYERS p = selectedNode->IsWinnerComputer();
	if (p == NOBODY)
	{
		//	printf("\n%dishere?\n", std::this_thread::get_id());
		Node* perNode = selectedNode->copy();
		PLAYERS winner_player_thread = perNode->MakeRandomMovement();
		delete perNode;
		return winner_player_thread == COMPUTER ? 1 : 0;
	}
	else {
		return p == COMPUTER ? 1 : 0;
	}
}

UCTSearch::UCTSearch()
{
	rootNode = NULL;
}


Node* UCTSearch::TreePolicy(Node* selectedNode)
{
	while (selectedNode->IsWinnerComputer() == NOBODY) { //节点不是终止节点 
														 //	printf("isid:%d\n",std::this_thread::get_id());
		Node* temp = selectedNode->expansion();
		if (temp) //且拥有未被访问的子状态 
			return temp; //扩展该节点 
		else
			selectedNode = selectedNode->selection(); //选择最优子节点 
	}
	return selectedNode;
}
int UCTSearch::DefaultPolicy(Node * selectedNode)
{

	PLAYERS p = selectedNode->IsWinnerComputer();
	if (p == NOBODY)
	{
		concurrency::concurrent_vector<std::future<PLAYERS>> futures;
		//	PLAYERS winner_player_thread1 = selectedNode->MakeRandomMovement();            //Player makes movement then changes to computer and so on
		vector<Node*> p;
		for (int i = 0; i < MAX_THREADS; i++) {
			Node* perNode = selectedNode->copy();
			p.push_back(perNode);
			futures.push_back(async(std::launch::async, &Node::MakeRandomMovement, perNode));
		}
		int size_ = futures.size();
		int playerwin = 0, aiwin = 0;
		Node* perNode = selectedNode->copy();
		PLAYERS ptr = perNode->MakeRandomMovement();
		if (ptr == COMPUTER) aiwin++;
		delete perNode;

		for (int i = 0; i < size_; i++) {
			PLAYERS p = futures[i].get();
			if (p == COMPUTER) aiwin++;
		}
		for (int i = 0; i < MAX_THREADS; i++)
			delete p[i];
		//cout << "count:" <<aiwin<< endl;
		return aiwin;
	}
	else {
		return p == COMPUTER ? 3 : 0;
	}

}

UCTSearch::~UCTSearch()
{
	//delete rootNode;
	//rootNode = NULL;
	tempNode = NULL;
	if (rootNode == NULL)
		return;
	int size1 = rootNode->sonNodeSizes;
	for (int i = 0; i < size1; i++) {
		if (rootNode->sonNodes[i]->presentmovement == move_ && rootNode->sonNodes[i]->sonNodes.load() != NULL) {
			int size = rootNode->sonNodes[i]->sonNodeSizes;
			for (int j = 0; j < size; j++) {
				if (rootNode->sonNodes[i]->sonNodes[j]->presentmovement == playerMove) {
					tempNode = rootNode->sonNodes[i]->sonNodes[j];
					cout << "treget!" << endl;
					tempNode->fatherNode = NULL;
					continue;
				}
				delete rootNode->sonNodes[i]->sonNodes[j];
				rootNode->sonNodes[i]->sonNodes[j] = NULL;
				rootNode->sonNodes[i]->sonNodeSizes = 0;
			}
			continue;
		}
		delete rootNode->sonNodes[i];
		rootNode->sonNodes[i] = NULL;
		rootNode->sonNodeSizes = 0;
	}
	/*rootNode= tempNode;*/
	//if (rootNode != NULL) {
	//	int size = rootNode->sonNodeSizes;
	//	for (int j = 0; j < size; j++) {
	//		int size_ = rootNode->sonNodes[j]->sonNodeSizes;
	//		for (int k = 0; k < size_; k++) {
	//			delete rootNode->sonNodes[j]->sonNodes[k];
	//			rootNode->sonNodes[j]->sonNodes[k] = NULL;
	//		}
	//		rootNode->sonNodes[j]->sonNodeSizes = 0;
	//	}
	//}
}

/*x
10-x

char getBestNodeInStart(char* board, bool isFirst) {
	if (isFirst) {
		if (step == 2) {
			if (playerMove == 49 || playerMove == 50)return 48;
			if (playerMove == 59)return 51;
			if (playerMove == 70 || playerMove == 71)return 72;
			if (playerMove == 72)return 81;
			if (playerMove == 61)return 69;
			//	if (playerMove / 11 < 6 && (playerMove % 11 > 8 || playerMove % 11 > 8 < 3))return 39;
			if (playerMove == 40 || playerMove == 38)return  39;
			if (playerMove == 39)return  48;
			if (playerMove / 11 == 6 && playerMove % 11 < 6)return 51;
			if (playerMove / 11 == 6 && playerMove % 11 > 6)return 69;

		}
	}
}*/
