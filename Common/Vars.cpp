#include "pch.h"

#include "ProudNetClient.h"
#include "Vars.h"

// 3 Ŭ���̾�Ʈ�� ������ �ְ� �޴� �������� ����(GUID) �� ���� ��Ʈ ����
Proud::PNGUID guid = { 0x3ae33249, 0xecc6, 0x4980,{ 0xbc, 0x5d, 0x7b, 0xa, 0x99, 0x9c, 0x7, 0x39 } };

Proud::Guid g_Version = Proud::Guid(guid);
int g_ServerPort = 33334;
/*
// 2 CMessage operator <<, >> �� ���� �մϴ�.
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