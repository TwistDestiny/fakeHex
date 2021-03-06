
// fakeHexDlg.h: 头文件
//

#pragma once
#include"tool.h"
// CfakeHexDlg 对话框
class CfakeHexDlg : public CDialogEx
{
// 构造
public:
	CfakeHexDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FAKEHEX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CListBox MsgList;
	board* mboard=NULL;
	void drawHex(CPoint p, bool i);
	afx_msg void OnStartAi();
	afx_msg void OnStartPlayer();
};
