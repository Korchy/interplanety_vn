//---------------------------------------------------------------------------
#ifndef Object3DPortalH
#define Object3DPortalH
//---------------------------------------------------------------------------
#include "Object3DUnit.h"
#include "BoundingSphere.h"
//---------------------------------------------------------------------------
// ����� Object3DPortal. ������ "������"
//---------------------------------------------------------------------------
class Object3DPortal : public Object3DUnit
{
protected:      // �������� ����� � ��� ��������
        // ����������


private:        // �������� ������ �����
        // ����������
        float BigRadius;
        float SmallRadius;      // 2 �������, ���������� ������ ������� (������)
        float MoveSpeed;        // �������� �������� �� ������ ����/��.�������
        AnsiString Dest;        // ������, � ������� ����� ������
        AnsiString Name;        // ��������
        AnsiString File;        // ���� � ����� � �������

public:         // �������� �����
        Object3DPortal(void);   // ���������� ������ ��� ����������
        ~Object3DPortal();      // ���������� ������

        // ����������

        // �������
        bool SetRadius(float NewBig, float NewSmall);   // ���������� ��������� ������
        bool SetMoveSpeed(float NewMoveSpeed);          // ���������� �������� �������� �� ������
        bool SetDest(AnsiString NewDest);               // ���������� ������, � ������� ����� ������
        bool SetName(AnsiString NewName);               // ���������� ��������
        bool SetFile(AnsiString NewFile);               // ���������� ���� � ����� � �������

};
//---------------------------------------------------------------------------
#endif
