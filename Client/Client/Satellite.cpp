//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Satellite.h"
//---------------------------------------------------------------------------
Satellite::Satellite(void)
{
	// ����������� ������
Bound = new BoundingSphere();
AlternativeFile = "s0\\Planet.x";
}
//---------------------------------------------------------------------------
Satellite::~Satellite(void)
{
	// ���������� ������
delete Bound;
}
//---------------------------------------------------------------------------
//                                  �������
//---------------------------------------------------------------------------
bool Satellite::SetRadius(float NewBig,float NewSmall)
{
	// ���������� ��������� ������
BigRadius = NewBig;
SmallRadius = NewSmall;
if(Mesh!=NULL) {	// ����������� ������� � ��������� �����
	MoveInto(-BigRadius,0.0,0.0);
	UpdateTransform();
}
return true;
}
//---------------------------------------------------------------------------
bool Satellite::SetRotateSpeed(float NewRotateSpeed)
{
	// ���������� �������� ������������ ��������
RotateSpeed = NewRotateSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Satellite::SetMoveSpeed(float NewMoveSpeed)
{
	// ���������� �������� �������� �� ������
MoveSpeed = NewMoveSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Satellite::SetName(std::string NewName)
{
	// ���������� ��������
Name = NewName;
return true;
}
//---------------------------------------------------------------------------
bool Satellite::SetFile(std::string NewFile)
{
	// ���������� ���� � ����� � �������
File = NewFile;
return true;
}
//---------------------------------------------------------------------------