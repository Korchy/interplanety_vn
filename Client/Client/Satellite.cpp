//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Satellite.h"
//---------------------------------------------------------------------------
Satellite::Satellite(void)
{
	// Конструктор класса
Bound = new BoundingSphere();
AlternativeFile = "s0\\Planet.x";
}
//---------------------------------------------------------------------------
Satellite::~Satellite(void)
{
	// Деструктор класса
delete Bound;
}
//---------------------------------------------------------------------------
//                                  ФУНКЦИИ
//---------------------------------------------------------------------------
bool Satellite::SetRadius(float NewBig,float NewSmall)
{
	// Установить параметры орбиты
BigRadius = NewBig;
SmallRadius = NewSmall;
if(Mesh!=NULL) {	// Переместить планету в начальную точку
	MoveInto(-BigRadius,0.0,0.0);
	UpdateTransform();
}
return true;
}
//---------------------------------------------------------------------------
bool Satellite::SetRotateSpeed(float NewRotateSpeed)
{
	// Установить скорость собственного вращения
RotateSpeed = NewRotateSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Satellite::SetMoveSpeed(float NewMoveSpeed)
{
	// Установить скорость движения по орбите
MoveSpeed = NewMoveSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Satellite::SetName(std::string NewName)
{
	// Установить название
Name = NewName;
return true;
}
//---------------------------------------------------------------------------
bool Satellite::SetFile(std::string NewFile)
{
	// Установить путь к файлу с моделью
File = NewFile;
return true;
}
//---------------------------------------------------------------------------