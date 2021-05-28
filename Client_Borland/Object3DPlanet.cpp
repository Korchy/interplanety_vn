//---------------------------------------------------------------------------
#pragma hdrstop

#include "Object3DPlanet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
Object3DPlanet::Object3DPlanet(void)
{
        // ����������� ������ ��� ����������
        // ������������� ����������
Bound = new BoundingSphere();
AlternativeFile = "s0\\Planet.x";
}
//---------------------------------------------------------------------------
Object3DPlanet::~Object3DPlanet()
{
        // ���������� ������
delete Bound;
}
//---------------------------------------------------------------------------
//                                  �������
//---------------------------------------------------------------------------
bool Object3DPlanet::SetRadius(float NewBig, float NewSmall)
{
        // ���������� ��������� ������
BigRadius = NewBig;
SmallRadius = NewSmall;
if(Mesh!=NULL) {        // ����������� ������� � ��������� �����
        MoveInto(-BigRadius,0.0,0.0);
        UpdateTransform();
        }
return true;
}
//---------------------------------------------------------------------------
bool Object3DPlanet::SetRotateSpeed(float NewRotateSpeed)
{
        // ���������� �������� ������������ ��������
RotateSpeed = NewRotateSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPlanet::SetMoveSpeed(float NewMoveSpeed)
{
        // ���������� �������� �������� �� ������
MoveSpeed = NewMoveSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPlanet::SetName(AnsiString NewName)
{
        // ���������� ��������
Name = NewName;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPlanet::SetFile(AnsiString NewFile)
{
        // ���������� ���� � ����� � �������
File = NewFile;
return true;
}
//---------------------------------------------------------------------------
