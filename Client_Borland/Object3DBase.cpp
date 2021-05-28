//---------------------------------------------------------------------------
#pragma hdrstop

#include "Object3DBase.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
Object3DBase::Object3DBase(void)
{
        // ����������� ������ ��� ����������
        // ������������� ����������
Bound = new BoundingSphere();
AlternativeFile = "s0\\Base.x";
}
//---------------------------------------------------------------------------
Object3DBase::~Object3DBase()
{
        // ���������� ������
delete Bound;
}
//---------------------------------------------------------------------------
//                                  �������
//---------------------------------------------------------------------------
bool Object3DBase::SetRadius(float NewBig, float NewSmall)
{
        // ���������� ��������� ������
BigRadius = NewBig;
SmallRadius = NewSmall;
if(Mesh!=NULL) {        // ����������� ���� � ��������� �����
        MoveInto(-BigRadius,0.0,0.0);
        UpdateTransform();
        }
return true;
}
//---------------------------------------------------------------------------
bool Object3DBase::SetRotateSpeed(float NewRotateSpeed)
{
        // ���������� �������� ������������ ��������
RotateSpeed = NewRotateSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Object3DBase::SetMoveSpeed(float NewMoveSpeed)
{
        // ���������� �������� �������� �� ������
MoveSpeed = NewMoveSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Object3DBase::SetName(AnsiString NewName)
{
        // ���������� ��������
Name = NewName;
return true;
}
//---------------------------------------------------------------------------
bool Object3DBase::SetFile(AnsiString NewFile)
{
        // ���������� ���� � ����� � �������
File = NewFile;
return true;
}
//---------------------------------------------------------------------------
