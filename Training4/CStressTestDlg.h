#pragma once
#include "afxdialogex.h"
#include <vector>
#include <fstream>
#include <thread>

#include "../../../../ProudNet/include/ProudNetClient.h"

using namespace std;
using namespace Proud;

struct sPattern
{
	vector<int>	sendType{};
};

class DummyClient{
public:
	CNetClient* m_client;
	DummyClient() { m_client = CNetClient::Create(); }
	~DummyClient() {}

	sPattern m_pattern;
	int clientIdx{};
	std::thread dmThread;

	void DmClientSendMsg();
};

// CStressTestDlg 대화 상자

class CStressTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStressTestDlg)

public:
	CStressTestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	virtual ~CStressTestDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_S_TEST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void ConnectProudNet();
	void CheckUserAccess();
	void ChangePrintUserNum();
	//void ClientSendMsg(uint64_t userNum);
	//void PreSendMsg();
	void LoadScenarioFiles();
	bool SelectScenario();
	//bool SaveFileContents();
	bool SaveScenarioType();


	//std::vector<CNetClient*> vec_clients;
	//CNetClient* m_client;
	std::vector<DummyClient*> vec_dmClients;
	DummyClient* dmClient;
	uint64_t userIdx{};		//통신 시 유저 인덱스
	uint64_t nowIdx{};		//현재 접속 유저 수
	uint64_t maxIdx{};		//최대(목표) 유저 수

	CString filePath{};
	std::wifstream wfileName{};	//UTF-8 파일 사용 시 사용
	std::ifstream fileName{};
	CString nowFilePath{};
	vector<CString> fileNames{};
	vector<CString> fileContents{};
	vector<int> scTypes{};

	afx_msg void OnBnClickedTestStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	CListBox m_msgList;
	CListBox m_logList;
	CEdit m_nowUser{};

	afx_msg void OnBnClickedTestStop();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//afx_msg void OnBnClickedLoadFile();
	CComboBox m_ScenarioCombo;
};

enum ScenarioType {
	KOR,
	ENG,
	NUM,
	MIX
};