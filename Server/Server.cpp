#include <iostream>
#include <fstream>
#include <locale>
#include <chrono>
#include <vector>

#include "../../../../ProudNet/include/ProudNetServer.h"
//#include "../Training4/CStressTestDlg.h"

using namespace std;
using namespace Proud;

#include "../Common/Vars.h"
#include "../Common/C2S_common.cpp"
#include "../Common/S2C_common.cpp"
#include "../Common/C2S_stub.h"
#include "../Common/S2C_proxy.h"
#include "../Common/C2S_stub.cpp"
#include "../Common/S2C_proxy.cpp"

class C2SStub : public C2S::Stub
{
private:
    CNetServer* m_srv;
public:
    C2SStub(CNetServer* srv) : m_srv(srv) {}
    DECRMI_C2S_Chat;
    DECRMI_C2S_Send;
    DECRMI_C2S_Quit;
};

S2C::Proxy g_S2CProxy;
vector<vector<String>> clientMsg;

DEFRMI_C2S_Chat(C2SStub)       //C to S 연결 성공 시
{
    std::string recvString = str;
    const char* tmp = recvString.c_str();
    if (strcmp(tmp, "Hello ProudNet~!!!\n") == 0)
        printf("[Server] client[%d] connected : %s", num, StringT2A(str).GetString());
    else {
        if (clientMsg.size() < num) {
            clientMsg.push_back({str});
        }
        else {
            clientMsg[num-1].push_back(str);
        }
        printf("From Client %d: %s\n", num, StringT2A(str).GetString());
        
        g_S2CProxy.recvString(remote, RmiContext::ReliableSend, str, num);
        /*  1명의 송신 후 각 클라이언트마다 메세지를 출력할 때 사용
        HostID targetList[100];
        int max = m_srv->GetClientHostIDs(targetList, 100);
        for (int i = 0; i < max; i++)
            cout << targetList[i] << " " << max << "\n";
        cout << "\n";
        */
    }
    return true;
}

static std::string server_time_stamp() {
    const auto now = std::chrono::system_clock::now();
    time_t tm_now = std::chrono::system_clock::to_time_t(now);
    struct tm tstruct;

#if defined(_WIN32)
    localtime_s(&tstruct, &tm_now);
#endif

    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

    char temp[128];
    snprintf(temp, sizeof(temp), "%02d%02d%02d_%02d:%02d:%02d.%03ld",
        tstruct.tm_year - 100, tstruct.tm_mon + 1, tstruct.tm_mday,
        tstruct.tm_hour, tstruct.tm_min, tstruct.tm_sec, millis.count());

    return std::string(temp);
}

DEFRMI_C2S_Send(C2SStub)       //C to S 연결 성공 시
{
    // 송신자 데이터 우선 저장
    std::string who = "_me: ";
    std::string saveStr = StringT2A(sendTime).GetString() + who + StringT2A(str).GetString();
    if (num > clientMsg.size()) {
        clientMsg.push_back({ saveStr });
    }
    else {
        clientMsg[num - 1].push_back(saveStr);
    }

    int nowUserCnt = m_srv->GetClientCount();
    printf("From Client %d: %s\n", num, StringT2A(str).GetString());
    g_S2CProxy.recvString(remote, RmiContext::ReliableSend, str, num);

    who = "_from Client" + to_string(num) + ": ";
    saveStr = server_time_stamp() + who + StringT2A(str).GetString();
    for (int i = 0; i < clientMsg.size(); ++i) {
        if (i == num - 1)   // 송신자일 경우
            continue;

        clientMsg[i].push_back(saveStr);
    }

    return true;
}

static std::string get_time_stamp() {
    const auto now = std::chrono::system_clock::now();
    time_t tm_now = std::chrono::system_clock::to_time_t(now);
    struct tm tstruct;

#if defined(_WIN32)
    localtime_s(&tstruct, &tm_now);
#endif

    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

    char temp[128];
    snprintf(temp, sizeof(temp), "%02d%02d%02d_%02d%02d_%02d.%03ld",
        tstruct.tm_year - 100, tstruct.tm_mon + 1, tstruct.tm_mday,
        tstruct.tm_hour, tstruct.tm_min, tstruct.tm_sec, millis.count());

    return std::string(temp);
}

DEFRMI_C2S_Quit(C2SStub)       //C to S 연결 성공 시
{
    //파일 저장
    std::string cmd = StringT2A(dlg).GetString();
    if(cmd == "Stop"){
        printf("Client Stoped.\n");

        std::string saveFileName = "";
        for (int i = 0; i < clientMsg.size(); ++i) {
            saveFileName = "../ClientData/" + get_time_stamp() + "_client[" + to_string(i+1) + "].txt";
            std::ofstream outputFile(saveFileName);
            for (auto msg : clientMsg[i]) {
                if (outputFile.is_open()) {
                    outputFile << StringT2A(msg).GetString() << "\n";
                }
                else {
                    cout << "File Open Error" << endl;
                    return 1;
                }
            }
            outputFile.close();
        }
    }
    //else if (cmd == "Stop") {
        clientMsg.clear();
    //}

    return true;
}

int main()
{
    CNetServer* srv = CNetServer::Create();
    srv->OnClientJoin = [](CNetClientInfo* clientInfo) {
        printf("Client %d connected.\n", clientInfo->m_HostID);
        };
    srv->OnClientLeave = [](CNetClientInfo* clientInfo, ErrorInfo* errorInfo, const ByteArray& comment) {
        printf("Client %d disconnected.\n", clientInfo->m_HostID);
        };
    srv->OnError = [](ErrorInfo* errorInfo) {
        printf("OnError : %s\n", StringT2A(errorInfo->ToString()).GetString());
        };
    srv->OnWarning = [](ErrorInfo* errorInfo) {
        printf("OnWarning : %s\n", StringT2A(errorInfo->ToString()).GetString());
        };
    srv->OnInformation = [](ErrorInfo* errorInfo) {
        printf("OnInformation : %s\n", StringT2A(errorInfo->ToString()).GetString());
        };
    srv->OnException = [](const Exception& e) {
        printf("OnInformation : %s\n", e.what());
        };

    C2SStub g_C2SStub(srv);
    srv->AttachStub(&g_C2SStub);
    srv->AttachProxy(&g_S2CProxy);

    CStartServerParameter p1;           //서버 파라미터 생성
    p1.m_protocolVersion = g_Version;   //프로토콜 버전
    p1.m_tcpPorts.Add(g_ServerPort);    //포트 번호

    try
    {
        srv->Start(p1);
        puts("Server Startㅡㅡㅡ");
        puts("q: Quit.");
        puts("c: Console Clear.");
        puts("t: Current Time.");
    }
    catch (Exception& e)
    {
        cout << "Server start failed: " << e.what() << endl;
        return 0;
    }

    string userInput;
    while (1)
    {
        cin >> userInput;
        if (userInput == "q") {
            srv->DetachStub(&g_C2SStub);
            srv->DetachProxy(&g_S2CProxy);
            
            srv->Stop();
            
            break;
        }
        else if (userInput == "c") {
            system("cls");
            puts("Server Startㅡㅡㅡ");
            puts("q: Quit.");
            puts("c: Console Clear.");
            puts("t: Current Time.");
        }
        else if (userInput == "t") {
            // 현재 시간 출력
            cout << get_time_stamp() << endl;
        }

        printf("현재 접속 클라이언트 수: %d\n", srv->GetClientCount());
        // CPU 점유를 100% 차지하지 않게 하기 위함
        // To prevent 100% occupation rate of CPU
        Sleep(100);
        puts("sleep 100 complete");
    }

    return 0;
}