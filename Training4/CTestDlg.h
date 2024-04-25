#pragma once
#include "afxdialogex.h"


// CTestDlg 대화 상자

class CTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTestDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
