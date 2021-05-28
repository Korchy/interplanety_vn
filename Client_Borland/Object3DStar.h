//---------------------------------------------------------------------------
#ifndef Object3DStarH
#define Object3DStarH
//---------------------------------------------------------------------------
#include "Object3DUnit.h"
#include "BoundingSphere.h"
//---------------------------------------------------------------------------
// ����� Object3DStar. ������ "������" - ����������� ���� � �������
//---------------------------------------------------------------------------
class Object3DStar : public Object3DUnit
{
protected:      // �������� ����� � ��� ��������
        // ����������


private:        // �������� ������ �����
        // ����������
         float X;
         float Y;
         float Z;                // ���������
         float RotateSpeed;      // �������� �������� ������ ����������� ��� ����/��.�������
         AnsiString Name;        // ��������
         AnsiString File;        // ���� � ����� � �������

public:         // �������� �����
        Object3DStar(void);      // ���������� ������ ��� ����������
        ~Object3DStar();         // ���������� ������

        // ����������

        // �������
        bool SetPosition(float NewX,float NewY,float NewZ);     // ���������� ��������� ������
        bool SetRotateSpeed(float NewRotateSpeed);              // ���������� �������� �������� ������
        bool SetName(AnsiString NewName);                       // ���������� �������� �����
        bool SetFile(AnsiString NewFile);                       // ���������� ���� � ����� � �������
};
//---------------------------------------------------------------------------
#endif
