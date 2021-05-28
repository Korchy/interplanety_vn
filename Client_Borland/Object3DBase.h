//---------------------------------------------------------------------------
#ifndef Object3DBaseH
#define Object3DBaseH
//---------------------------------------------------------------------------
#include "Object3DUnit.h"
#include "BoundingSphere.h"
//---------------------------------------------------------------------------
// ����� Object3DBase. ������ "����" - �������, ����������� ������ ������ ��
// ������������ ����������
//---------------------------------------------------------------------------
class Object3DBase : public Object3DUnit
{
protected:      // �������� ����� � ��� ��������
        // ����������


private:        // �������� ������ �����
        // ����������
        float BigRadius;
        float SmallRadius;      // 2 �������, ���������� ������ �������� (������)
        float RotateSpeed;      // �������� �������� ������ ����������� ��� ����/��.�������
        float MoveSpeed;        // �������� �������� �� ������ ����/��.�������
        AnsiString Name;        // ��������
        AnsiString File;        // ���� � ����� � �������

public:         // �������� �����
        Object3DBase(void);     // ���������� ������ ��� ����������
        ~Object3DBase();        // ���������� ������

        // ����������

        // �������
        bool SetRadius(float NewBig, float NewSmall);   // ���������� ��������� ������
        bool SetRotateSpeed(float NewRotateSpeed);      // ���������� �������� ��������
        bool SetMoveSpeed(float NewMoveSpeed);          // ���������� �������� �������� �� ������
        bool SetName(AnsiString NewName);               // ���������� ��������
        bool SetFile(AnsiString NewFile);               // ���������� ���� � ����� � �������

};
//---------------------------------------------------------------------------
#endif
