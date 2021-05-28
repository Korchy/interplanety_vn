//---------------------------------------------------------------------------
#ifndef Object3DPlanetH
#define Object3DPlanetH
//---------------------------------------------------------------------------
#include "Object3DUnit.h"
#include "BoundingSphere.h"
//---------------------------------------------------------------------------
// ����� Object3DPlanet. ������ "�������" - �������, ����������� ������ ������
//---------------------------------------------------------------------------
class Object3DPlanet : public Object3DUnit
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
        Object3DPlanet(void);   // ���������� ������ ��� ����������
        ~Object3DPlanet();      // ���������� ������

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
