
// fakeHexDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "fakeHex.h"
#include "fakeHexDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CfakeHexDlg 对话框



CfakeHexDlg::CfakeHexDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FAKEHEX_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfakeHexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, MsgList);
}

BEGIN_MESSAGE_MAP(CfakeHexDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_START_AI32771, &CfakeHexDlg::OnStartAi)
	ON_COMMAND(ID_START_32772, &CfakeHexDlg::OnStartPlayer)
END_MESSAGE_MAP()


// CfakeHexDlg 消息处理程序

BOOL CfakeHexDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);  //IDR_MENU1为菜单栏ID号
	SetMenu(&menu);
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CfakeHexDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CfakeHexDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CfakeHexDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CfakeHexDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(mboard==NULL)
	::MessageBox(NULL, _T("未开始"), _T("游戏提示"), MB_OK);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint p;
	int   p_y = (point.y - 30) / 42, p_x = ((point.x - p_y * 24.25) - 15) / 48.5;
	if (p_x >= 0 && p_x < 11 && p_y >= 0 && p_y < 11 && ((point.y-30)%42)<28) {
		p.x = p_x; p.y = p_y;
		if (mboard != NULL && mboard->right == PLAYER) {
			mboard->putChess(p_y, p_x, PLAYER);
			drawHex(p, mboard->firstHand== PLAYER);
			mboard->rightChange();
			char node = mboard->getAIStep();
			mboard->rightChange();
			CPoint p_;
			p_.x = node % 11;
			p_.y = node / 11;
			drawHex(p_, mboard->firstHand == COMPUTER);
			if (mboard->gamestate() != NOBODY) {
				if (mboard->gamestate() == COMPUTER)
					::MessageBox(NULL, _T("AI获胜"), _T("游戏结束"), MB_OK);
				else
					::MessageBox(NULL, _T("玩家获胜"), _T("游戏结束"), MB_OK);
			}
		}
		
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CfakeHexDlg::drawHex(CPoint p, bool i)
{
	int red = i ? 255 : 0;
	int blue = i ? 0 : 255;
	int startx = p.x * 48.5 + 15 + p.y * 24.25 , starty = p.y * 42 + 30;
	CDC* pDC = GetDC(); // 得到DC
	pDC->BeginPath();
	pDC->MoveTo(startx, starty);
	pDC->LineTo(startx + 23, starty - 13);
	pDC->LineTo(startx + 47, starty);
	pDC->LineTo(startx + 47, starty + 28);
	pDC->LineTo(startx + 23, starty + 41);
	pDC->LineTo(startx, starty + 28);
	pDC->LineTo(startx, starty);
	pDC->EndPath();
	CRgn rgn;
	rgn.CreateFromPath(pDC);
	pDC->InvertRgn(&rgn);
	CBrush brush(RGB(red, 0, blue));
	//brush.CreateSolodBrush(0));//红色画刷
	//提示
	string color= red == 255 ? "red" : "blue";
	string str_ = getTime() +" ["+color+"] "+"[" + i2s(p.x+1) + "," + i2s(p.y+1)+"]";
	CString str2(str_.c_str());
	MsgList.AddString(str2);
	pDC->FillRgn(&rgn, &brush);
}

void CfakeHexDlg::OnStartAi()
{
	mboard = board::getBoard();
	mboard->initBoard(COMPUTER);
	// TODO: 在此添加命令处理程序代码
	mboard->right = COMPUTER;
	char node = mboard->getAIStep();
	mboard->rightChange();
	CPoint p_;
	p_.x = node % 11;
	p_.y = node / 11;
	drawHex(p_, mboard->firstHand == COMPUTER);
}


void CfakeHexDlg::OnStartPlayer()
{
	mboard = board::getBoard();
	mboard->initBoard(PLAYER);
	mboard->right = PLAYER;
	// TODO: 在此添加命令处理程序代码
}
