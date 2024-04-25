// CServerControlDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Training4.h"
#include "afxdialogex.h"
#include "CServerControlDlg.h"


// CServerControlDlg 대화 상자

IMPLEMENT_DYNAMIC(CServerControlDlg, CDialogEx)

CServerControlDlg::CServerControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SVR_CONTROL_DIALOG, pParent)
{

}

CServerControlDlg::~CServerControlDlg()
{
}

void CServerControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CServerControlDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerControlDlg 메시지 처리기
