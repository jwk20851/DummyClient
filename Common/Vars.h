#pragma once
// 3 Ŭ���̾�Ʈ�� ������ �ְ� �޴� �������� ����(GUID) �� ���� ��Ʈ ����
extern Proud::Guid g_Version;
extern int g_ServerPort;
/*
// 1 ����� ���� Ŭ����(MyClass) ����
class MyClass
{
public:
    int a;
    float b;
    double c;
};

// 2 CMessage operator <<, >> �� ���� �մϴ�.
namespace Proud
{
    // ȣ���� RMI �Լ��� ������ ���ڿ��� ����� ����Ѵ�. 
    // �α׸� ���� �� �����ϴ�.
    // Make string with information of called RMI function then print it. 
    // It is very useful when you create log.
    void AppendTextOut(Proud::String& a, const MyClass& b);

    // �޽��� ���۷κ��� Ŀ���� Ÿ���� ������ �о�´�.
    // Read information of custom type from message buffer.
    Proud::CMessage& operator >> (Proud::CMessage& a, MyClass& b);

    // �޽��� ���۷� Ŀ���� Ÿ���� ������ �ִ´�.
    // Put detail of custom type by message buffer.
    Proud::CMessage& operator<<(Proud::CMessage& a, const MyClass& b);
}
*/