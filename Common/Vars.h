#pragma once
// 3 클라이언트와 서버간 주고 받는 프로토콜 버전(GUID) 및 서버 포트 정의
extern Proud::Guid g_Version;
extern int g_ServerPort;
/*
// 1 사용자 정의 클래스(MyClass) 선언
class MyClass
{
public:
    int a;
    float b;
    double c;
};

// 2 CMessage operator <<, >> 를 정의 합니다.
namespace Proud
{
    // 호출한 RMI 함수의 내용을 문자열로 만들어 출력한다. 
    // 로그를 만들 때 유용하다.
    // Make string with information of called RMI function then print it. 
    // It is very useful when you create log.
    void AppendTextOut(Proud::String& a, const MyClass& b);

    // 메시지 버퍼로부터 커스텀 타입의 내용을 읽어온다.
    // Read information of custom type from message buffer.
    Proud::CMessage& operator >> (Proud::CMessage& a, MyClass& b);

    // 메시지 버퍼로 커스텀 타입의 내용을 넣는다.
    // Put detail of custom type by message buffer.
    Proud::CMessage& operator<<(Proud::CMessage& a, const MyClass& b);
}
*/