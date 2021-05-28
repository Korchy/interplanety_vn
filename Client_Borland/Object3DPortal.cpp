//---------------------------------------------------------------------------
#pragma hdrstop

#include "Object3DPortal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
Object3DPortal::Object3DPortal(void)
{
        // ����������� ������ ��� ����������
        // ������������� ����������
Bound = new BoundingSphere();
AlternativeFile = "s0\\Portal.x";
}
//---------------------------------------------------------------------------
Object3DPortal::~Object3DPortal()
{
        // ���������� ������
delete Bound;
}
//---------------------------------------------------------------------------
//                                  �������
//---------------------------------------------------------------------------
bool Object3DPortal::SetRadius(float NewBig, float NewSmall)
{
        // ���������� ��������� ������
BigRadius = NewBig;
SmallRadius = NewSmall;
if(Mesh!=NULL) {        // ����������� ������ � ��������� �����
        MoveInto(-BigRadius,0.0,0.0);
        UpdateTransform();
        }
return true;
}
//---------------------------------------------------------------------------
bool Object3DPortal::SetMoveSpeed(float NewMoveSpeed)
{
        // ���������� �������� �������� �� ������
MoveSpeed = NewMoveSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPortal::SetDest(AnsiString NewDest)
{
        // ���������� ������, � ������� ����� ������
Dest = NewDest;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPortal::SetName(AnsiString NewName)
{
        // ���������� ��������
Name = NewName;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPortal::SetFile(AnsiString NewFile)
{
        // ���������� ���� � ����� � �������
File = NewFile;
return true;
}
//---------------------------------------------------------------------------
