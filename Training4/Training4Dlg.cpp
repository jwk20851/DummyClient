
// Training4Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Training4.h"
#include "Training4Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CStressTestDlg.h"
#include "CTestDlg.h"
#include "CServerControlDlg.h"

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeManuTab(NMHDR* pNMHDR, LRESULT* pResult);
};

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, DIC_MANU_TAB, &CAboutDlg::OnTcnSelchangeManuTab)
END_MESSAGE_MAP()

// CTraining4Dlg 대화 상자
CTraining4Dlg::CTraining4Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAINING4_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTraining4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, DIC_MANU_TAB, m_ManuTab);
}

BEGIN_MESSAGE_MAP(CTraining4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, DIC_MANU_TAB, &CTraining4Dlg::OnTcnSelchangeManuTab)
	ON_BN_CLICKED(IDC_QUIT, &CTraining4Dlg::OnBnClickedQuit)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTraining4Dlg 메시지 처리기
BOOL CTraining4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	TabSetting();
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTraining4Dlg::TabSetting() {
	m_ManuTab.InsertItem(0, _T("Stress Test"));
	m_ManuTab.InsertItem(1, _T("Func Test"));
	m_ManuTab.InsertItem(2, _T("Svr Control"));

	m_ManuTab.SetCurSel(0);

	CRect rect;
	m_ManuTab.GetWindowRect(&rect);

	pCStressTestDlg = new CStressTestDlg;
	pCStressTestDlg->Create(IDD_S_TEST_DIALOG, &m_ManuTab);
	pCStressTestDlg->MoveWindow(0, 25, rect.Width(), rect.Height());
	pCStressTestDlg->ShowWindow(SW_SHOW);

	pCTestDlg = new CTestDlg;
	pCTestDlg->Create(IDD_TEST_DIALOG, &m_ManuTab);
	pCTestDlg->MoveWindow(0, 25, rect.Width(), rect.Height());
	pCTestDlg->ShowWindow(SW_HIDE);

	pCServerControlDlg = new CServerControlDlg;
	pCServerControlDlg->Create(IDD_SVR_CONTROL_DIALOG, &m_ManuTab);
	pCServerControlDlg->MoveWindow(0, 25, rect.Width(), rect.Height());
	pCServerControlDlg->ShowWindow(SW_HIDE);
}

// 대화상자 시스템 메뉴 관련 함수
void CTraining4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.
void CTraining4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTraining4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTraining4Dlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CTraining4Dlg::OnTcnSelchangeManuTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_ManuTab.GetCurSel();

	switch (sel) {
	case 0:
		pCStressTestDlg->ShowWindow(SW_SHOW);
		pCTestDlg->ShowWindow(SW_HIDE);
		pCServerControlDlg->ShowWindow(SW_HIDE);
		break;

	case 1:
		pCStressTestDlg->ShowWindow(SW_HIDE);
		pCTestDlg->ShowWindow(SW_SHOW);
		pCServerControlDlg->ShowWindow(SW_HIDE);
		break;

	case 2:
		pCStressTestDlg->ShowWindow(SW_HIDE);
		pCTestDlg->ShowWindow(SW_HIDE);
		pCServerControlDlg->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}


void CAboutDlg::OnTcnSelchangeManuTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CTraining4Dlg::OnBnClickedQuit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnDestroy();
	OnOK();
}


void CTraining4Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// pCStressTestDlg;
	if (pCStressTestDlg != nullptr){
		pCStressTestDlg->OnDestroy();
		delete pCStressTestDlg;
	}
	if (pCTestDlg != nullptr)
		delete pCTestDlg;
	if (pCServerControlDlg != nullptr)
		delete pCServerControlDlg;
}
