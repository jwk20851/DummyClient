#pragma once
#include "afxdialogex.h"


// CServerControlDlg 대화 상자

class CServerControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerControlDlg)

public:
	CServerControlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CServerControlDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SVR_CONTROL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
