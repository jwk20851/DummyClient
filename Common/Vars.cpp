#include "pch.h"

#include "ProudNetClient.h"
#include "Vars.h"

// 3 클라이언트와 서버간 주고 받는 프로토콜 버전(GUID) 및 서버 포트 정의
Proud::PNGUID guid = { 0x3ae33249, 0xecc6, 0x4980,{ 0xbc, 0x5d, 0x7b, 0xa, 0x99, 0x9c, 0x7, 0x39 } };

Proud::Guid g_Version = Proud::Guid(guid);
int g_ServerPort = 33334;
/*
// 2 CMessage operator <<, >> 를 정의 합니다.
namespace Proud
{
    void AppendTextOut(String& a, const MyClass& b)
    {
        String f;
        f.Format(_PNT("MyClass.a : %d, MyClass.b : %.f, MyClass : %lf\n"), b.a, b.b, b.c);
        a += f;
    }

    CMessage& operator >> (CMessage& a, MyClass& b)
    {
        a >> b.a;
        a >> b.b;
        a >> b.c;
        return a;
    }

    CMessage& operator<<(CMessage& a, const MyClass& b)
    {
        a << b.a;
        a << b.b;
        a << b.c;
        return a;
    }
}
*/