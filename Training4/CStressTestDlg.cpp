// CStressTestDlg.cpp: 구현 파일
//
#include "pch.h"
#include "Training4.h"
#include "afxdialogex.h"
#include "CStressTestDlg.h"
#include "language.h"

#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <locale>
#include <io.h>
#include <unordered_map>
#include <chrono>
#include <random>

#include "../../../../ProudNet/include/ProudNetClient.h"


using namespace std;
using namespace Proud;

#include "../Common/Vars.h"
#include "../Common/C2S_common.cpp"
#include "../Common/C2S_proxy.h"
#include "../Common/C2S_proxy.cpp"

#include "../Common/S2C_common.cpp"
#include "../Common/S2C_stub.cpp"
#include "../Common/S2C_stub.h"

CriticalSection g_critSec;
volatile bool keepWorkerThread;

C2S::Proxy g_C2SProxy;

CStressTestDlg* s_ClientDlg{};

class S2CStub : public S2C::Stub
{
public:
	DECRMI_S2C_recvString;
};

S2CStub g_S2CStub;

DEFRMI_S2C_recvString(S2CStub)
{
	CriticalSectionLock lock(g_critSec, true);
	std::string str = StringT2A(txt).GetString();
	printf("Client %d: %s\n", num, txt);

	/*
	str = "Client " + std::to_string(num) + ": " + str + "\r\n";
	const char* buf = str.c_str();
	int chrSize = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, NULL);
	wchar_t* pWchr = new WCHAR[chrSize];
	MultiByteToWideChar(CP_ACP, 0, buf, strlen(buf) + 1, pWchr, chrSize);

	s_ClientDlg->m_msgList.InsertString(-1, buf);
	delete[] pWchr;
	*/
	//std::wstring wstr = txt;
	std::wstring wstr = L"Client " + std::to_wstring(num) + L": " + txt.c_str();

	s_ClientDlg->m_msgList.InsertString(-1, wstr.c_str());

	return true;
}

// CStressTestDlg 대화 상자
IMPLEMENT_DYNAMIC(CStressTestDlg, CDialogEx)

CStressTestDlg::CStressTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_S_TEST_DIALOG, pParent)
	//, fileName(L"../Msg/msg.txt")
{
	s_ClientDlg = this;
}

// 해당 함수가 이미 본문에 있다는 오류로 주석 처리
CStressTestDlg::~CStressTestDlg()
{
	for (auto dmClient : vec_dmClients)
		delete dmClient;
}

void CStressTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_S_LIST_MSG, m_msgList);
	DDX_Control(pDX, IDC_S_LIST_LOG, m_logList);
	DDX_Control(pDX, IDC_NOW_USER, m_nowUser);
	DDX_Control(pDX, IDC_SCENARIO_FILE, m_ScenarioCombo);
}

BEGIN_MESSAGE_MAP(CStressTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TEST_START, &CStressTestDlg::OnBnClickedTestStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TEST_STOP, &CStressTestDlg::OnBnClickedTestStop)
	ON_WM_DESTROY()
	//ON_BN_CLICKED(IDC_LOAD_FILE, &CStressTestDlg::OnBnClickedLoadFile)
END_MESSAGE_MAP()

/* // 배열에 저장된 문자열을 이용하여 클라이언트 메세지 송신 시 사용
void CStressTestDlg::ClientSendMsg(uint64_t userNum) {
	int sendtype = rand() % (dmClient->m_pattern.sendType.size());
	int msgIdx = rand() % 10;

	std::string sendMsg = "";
	switch (sendtype) {
	case 0:
		sendMsg = kor[msgIdx];
		break;
	case 1:
		sendMsg = eng[msgIdx];
		break;
	case 2:
		sendMsg = num[msgIdx];
		break;
	case 3:
		sendMsg = mix[msgIdx];
		break;
	}
	
	//g_C2SProxy.Chat(HostID_Server, RmiContext::ReliableSend, std::string(CT2CA(fileContents[msgIdx])), userNum);

	// 파일 내 모든 메세지(msg.txt) 송신 시
	//fileName.open(filePath);
	//if(fileName.is_open()){
	//	std::wstring line;
	//	while (std::getline(fileName, line)) {
	//		printf("%s\n", StringT2A(line).GetString());
	//		g_C2SProxy.Chat(HostID_Server, RmiContext::ReliableSend, line, userNum);
	//	}
	//	fileName.close();
	//}
	//else {
	//	m_logList.InsertString(-1, L"파일을 열지 못했습니다.");
	//}
}
*/

static std::string client_time_stamp() {
	const auto now = std::chrono::system_clock::now();		// 현재 시간을 얻은 뒤 time_t 타입(1970년 1월 1일이후부터 현재까지 흐른 초)으로 변환
	time_t tm_now = std::chrono::system_clock::to_time_t(now);

	struct tm tstruct;	// time_t 값을 년-월-일-시-분-초로 환산
	localtime_s(&tstruct, &tm_now);		// windows 환경에서 사용되는 함수

	auto duration = now.time_since_epoch();		// 에포크 이후 경과 시간을 나타내는 지속 시간을 가져오는 함수
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;	// 밀리초만 필요하기 때문에 밀리초로 변환 후 1000으로 나눈 나머지 값

	char temp[128];
	snprintf(temp, sizeof(temp), "%02d%02d%02d_%02d:%02d:%02d.%03ld",
		tstruct.tm_year - 100, tstruct.tm_mon + 1, tstruct.tm_mday,
		tstruct.tm_hour, tstruct.tm_min, tstruct.tm_sec, millis.count());	// .count(): millis 변수가 나타내는 std::chrono::duration 객체의 값인 시간 간격을 반환

	return std::string(temp);
}

void DummyClient::DmClientSendMsg() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> sendTypeDistribution(0, m_pattern.sendType.size() - 1);
	int sendtype = m_pattern.sendType[sendTypeDistribution(gen)];

	std::uniform_int_distribution<int> msgIdxDistribution(0, 9);
	int msgIdx = msgIdxDistribution(gen);

	/*int sendtype = rand() % (m_pattern.sendType.size());
	int msgIdx = rand() % 10;*/

	std::string sendMsg = "";
	switch (sendtype) {
	case 0:
		sendMsg = kor[msgIdx];
		break;
	case 1:
		sendMsg = eng[msgIdx];
		break;
	case 2:
		sendMsg = num[msgIdx];
		break;
	case 3:
		sendMsg = mix[msgIdx];
		break;
	}

	std::string nowTime = client_time_stamp();

	g_C2SProxy.Send(HostID_Server, RmiContext::ReliableSend, sendMsg, clientIdx, nowTime);

	std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 1500));

	// 재귀 호출
	//if(this.dlg.getThreadStatus)
	if(keepWorkerThread)
		DmClientSendMsg();
}

void CStressTestDlg::ConnectProudNet() {
	//m_client = CNetClient::Create();

	//m_client 대신 vec_clients[vec_clients.size() - 1] 를 사용하여도 동작가능. log창 번호 출력은 size최대일 경우로만 출력하여 idx 자체는 수정 불가
	//vec_clients.push_back(m_client);	//이후 FrameMove를 위해 벡터에 넣음
	dmClient = new DummyClient();
	vec_dmClients.push_back(dmClient);
	

	for(auto idx : scTypes){
		dmClient->m_pattern.sendType.push_back(idx);
	}

	dmClient->m_client->OnJoinServerComplete =
		[&](ErrorInfo* info, const ByteArray& replyFromServer)
		{
			CriticalSectionLock lock(g_critSec, true);
			if (info->m_errorType == ErrorType_Ok)
			{
				if (dmClient->m_client != nullptr) {
					std::string str = "Succeed to connect server. Allocated hostID= " + std::to_string(++userIdx);
					dmClient->clientIdx = userIdx;
					//std::string str = "Succeed to connect server. Allocated hostID= " + std::to_string(vec_clients[vec_clients.size() - 1]->GetLocalHostID());

					const char* buf = str.c_str();
					int chrSize = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, NULL);
					wchar_t* pWchr = new WCHAR[chrSize];
					MultiByteToWideChar(CP_ACP, 0, buf, strlen(buf) + 1, pWchr, chrSize);
					m_logList.InsertString(-1, pWchr);
					delete[] pWchr;

					g_C2SProxy.Chat(HostID_Server,                  // send destination
						RmiContext::ReliableSend,                   // how to send
						_PNT("Hello ProudNet~!!!\n"), userIdx);		// user defined parameters
					
					//ClientSendMsg(userIdx);

					dmClient->dmThread = std::thread([&](){
						dmClient->DmClientSendMsg();
					});
					dmClient->dmThread.detach();
				}
				else {
					std::string str = "m_client nullptr 확인\n";
					const char* buf = str.c_str();
					int chrSize = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, NULL);
					wchar_t* pWchr = new WCHAR[chrSize];
					MultiByteToWideChar(CP_ACP, 0, buf, strlen(buf) + 1, pWchr, chrSize);

					m_msgList.InsertString(-1, pWchr);
					delete[] pWchr;
				}
			}
			else
			{
				std::cout << "Failed to connect to server.\n";
			}
		};
	dmClient->m_client->OnLeaveServer = [&](ErrorInfo* errorInfo) {
		CriticalSectionLock lock(g_critSec, true);
		printf("OnLeaveServer. %s  \n", StringT2A(errorInfo->m_comment).GetString());
	};
	dmClient->m_client->OnError = [](ErrorInfo* errorInfo) {
		printf("OnError : %s\n", StringT2A(errorInfo->ToString()).GetString());
		};
	dmClient->m_client->OnWarning = [](ErrorInfo* errorInfo) {
		printf("OnWarning : %s\n", StringT2A(errorInfo->ToString()).GetString());
		};
	dmClient->m_client->OnInformation = [](ErrorInfo* errorInfo) {
		printf("OnInformation : %s\n", StringT2A(errorInfo->ToString()).GetString());
		};
	dmClient->m_client->OnException = [](const Exception& e) {
		printf("OnException : %s\n", (const char*)e.what());
		};

	// NetClient에 RmiStub, RmiProxy Attach 코드 작성하기
	dmClient->m_client->AttachProxy(&g_C2SProxy);    // Client-to-server => 
	dmClient->m_client->AttachStub(&g_S2CStub);        // server-to-client <=

	// 서버 Connection Param 코드 작성
	CNetConnectionParam cp;
	cp.m_protocolVersion = g_Version;    // 서버와 맞춰야 하는 프로토콜 버전
	cp.m_serverIP = _PNT("localhost");    // local에서 direct p2p를 테스트할경우 localhost대신 직접 ip를 넣어야 홀펀칭이 제대로 동작합니다
	cp.m_serverPort = g_ServerPort;

	dmClient->m_client->Connect(cp);

	return;
}

// CStressTestDlg 메시지 처리기
void CStressTestDlg::OnBnClickedTestStart()
{
	keepWorkerThread = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int cnt = GetDlgItemInt(IDC_USER_NUM);
	if (cnt <= 0){
		m_logList.InsertString(-1, L"접속 인원 수를 확인해주세요.");
		return;
	}

	if (!SelectScenario())
		return;

	//if (!SaveFileContents())
	//	return;
	
	if (!SaveScenarioType())
		return;

	//PreSendMsg();

	maxIdx += cnt;
}

/* // 이전 접속 인원들 우선 메세지 송신
void CStressTestDlg::PreSendMsg() {
	for (int i = 1; i <= maxIdx; ++i)
		ClientSendMsg(i);
}
*/

void CStressTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 1:
		CheckUserAccess();
		break;
	case 2:
		if (nowIdx < maxIdx) {
			ConnectProudNet();
			Sleep(50);
			vec_dmClients[nowIdx]->m_client->FrameMove();
			//CheckUserAccess();
			++nowIdx;
			ChangePrintUserNum();
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

inline void TextBoxSetText(CEdit* edit, std::string inStr)
{
	edit->SetWindowTextW((CString)inStr.c_str());
}

void CStressTestDlg::ChangePrintUserNum() {
	char tmp[10]{};
	snprintf(tmp, sizeof(tmp), "%d", nowIdx);
	TextBoxSetText(&m_nowUser, tmp);
}

void CStressTestDlg::CheckUserAccess() {
	for (auto cl : vec_dmClients)
		cl->m_client->FrameMove();
}

void CStressTestDlg::OnBnClickedTestStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	keepWorkerThread = false;
	//for (auto cl : vec_dmClients){
	//	cl->dmThread.join();
	//}

	std::string cmd = "Stop";
	g_C2SProxy.Quit(HostID_Server, RmiContext::ReliableSend, cmd);

	for (auto cl : vec_dmClients)
		cl->m_client->Disconnect();

	maxIdx = 0;
	nowIdx = 0;
	userIdx = 0;
	SetDlgItemInt(IDC_NOW_USER, nowIdx);
	SetDlgItemInt(IDC_USER_NUM, nowIdx);
	m_msgList.ResetContent();
	m_logList.ResetContent();

	filePath = "";
}

BOOL CStressTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetTimer(1, 100, 0);
	SetTimer(2, 100, 0);

	LoadScenarioFiles();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CStressTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(1);
	KillTimer(2);
}

void CStressTestDlg::LoadScenarioFiles() {
	// 다이얼로그 생성 시 Scenario 폴더 내 모든 txt 파일 불러오기
	string fPath = "../Scenario/*.txt";

	struct _finddata_t fd;
	intptr_t handle;
	if ((handle = _findfirst(fPath.c_str(), &fd)) == -1L){			// 파일이 없을경우 발생시킬 이벤트
		m_logList.InsertString(-1, L"No file in directory!");
		return;
	}

	do{
		//std::wstring wstr(fd.name, fd.name + sizeof(fd.name) - 1);	//char[] to wstring
		CString fileFullName(fd.name);
		fileNames.push_back(fileFullName);

		// cstring 문자열에 포함된 확장자(."txt") 제거
		CString substring = _T(".txt");
		fileFullName.Replace(substring, _T(""));

		// wstr 문자열에 포함된 확장자(".txt") 제거
		//std::wstring target = L".txt";
		//size_t pos = wstr.find(target);
		//if (pos != std::wstring::npos) {
		//	wstr.erase(pos, target.length());
		//}
		
		m_ScenarioCombo.SetWindowTextW(_T("Select Scenerio"));
		m_ScenarioCombo.AddString(fileFullName);
	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);
}

bool CStressTestDlg::SelectScenario() {
	// 시나리오 파일 선택 확인
	int scIdx = m_ScenarioCombo.GetCurSel();
	if (scIdx < 0) {
		m_logList.InsertString(-1, L"Select Scenario File");
		return false;
	}

	// 파일 경로 확인 및 utf-8 인코딩
	filePath = L"../Scenario/" + fileNames[scIdx];
	if (!filePath.IsEmpty()) {
		m_logList.InsertString(-1, fileNames[scIdx] + L" 파일 확인");
		fileName.imbue(std::locale("ko_KR.UTF-8"));
	}
	else {
		m_logList.InsertString(-1, L"파일을 찾지 못하였습니다.");
		return false;
	}

	return true;
}

bool CStressTestDlg::SaveScenarioType() {
	// 시나리오 타입, 타입 벡터에 저장
	fileName.open(filePath);
	if (fileName.is_open()) {
		std::unordered_map<std::string, ScenarioType> scenarioMap = {
			{"sendOnlyKOR", KOR},
			{"sendOnlyENG", ENG},
			{"sendOnlyNUM", NUM},
			{"sendOnlyMIX", MIX}
		};

		scTypes.clear();
		std::string line;	//getline 사용을 위해 wstring 선언
		while (std::getline(fileName, line)) {
			std::string subStr = line.substr(0, 2);
			if (subStr == "//")
				continue;
			
			ScenarioType scType{};
			auto it = scenarioMap.find(line);
			if (it != scenarioMap.end()) {
				scType = it->second;
			}
			else {
				m_logList.InsertString(-1, L"시나리오 타입을 찾지 못하였습니다");
				return false;
			}

			switch (scType) {
			case KOR:
				scTypes.push_back(0);
				break;
			case ENG:
				scTypes.push_back(1);
				break;
			case NUM:
				scTypes.push_back(2);
				break;
			case MIX:
				scTypes.push_back(3);
				break;
			}
		}
		fileName.close();
		m_logList.InsertString(-1, L"시나리오 타입 저장 완료");
	}
	else {
		m_logList.InsertString(-1, L"파일을 열지 못했습니다.");
		return false;
	}
	return true;
}


/*// 파일의 전체 내용을 읽어 fileContents 벡터에 저장하여 출력 시 사용
bool CStressTestDlg::SaveFileContents() {
	wfileName.open(filePath);
	if (wfileName.is_open()) {
		std::wstring line;	//getline 사용을 위해 wstring 선언
		while (std::getline(wfileName, line)) {
			fileContents.push_back(line.c_str());
		}
		wfileName.close();
	}
	else {
		m_logList.InsertString(-1, L"파일을 열지 못했습니다.");
		return false;
	}
	return true;
}
*/

/*// 파일을 불러온 뒤 UTF-8 인코딩
void CStressTestDlg::OnBnClickedLoadFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	filePath = L"../msg.txt";
	if (!filePath.IsEmpty()) {		//msg.txt 고정 메세지 출력 및 파일 utf-8 인코딩
		m_logList.InsertString(-1, L"msg.txt 파일 확인 완료.");
		fileName.imbue(std::locale("ko_KR.UTF-8"));
	}
	else {
		m_logList.InsertString(-1, L"파일을 찾지 못하였습니다.");
	}
}
*/