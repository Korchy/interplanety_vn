//---------------------------------------------------------------------------
#pragma hdrstop

#include "Object3DPlanet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
Object3DPlanet::Object3DPlanet(void)
{
        // Конструктор класса без параметров
        // Инициализация переменных
Bound = new BoundingSphere();
AlternativeFile = "s0\\Planet.x";
}
//---------------------------------------------------------------------------
Object3DPlanet::~Object3DPlanet()
{
        // Деструктор класса
delete Bound;
}
//---------------------------------------------------------------------------
//                                  ФУНКЦИИ
//---------------------------------------------------------------------------
bool Object3DPlanet::SetRadius(float NewBig, float NewSmall)
{
        // Установить параметры орбиты
BigRadius = NewBig;
SmallRadius = NewSmall;
if(Mesh!=NULL) {        // Переместить планету в начальную точку
        MoveInto(-BigRadius,0.0,0.0);
        UpdateTransform();
        }
return true;
}
//---------------------------------------------------------------------------
bool Object3DPlanet::SetRotateSpeed(float NewRotateSpeed)
{
        // Установить скорость собственного вращения
RotateSpeed = NewRotateSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPlanet::SetMoveSpeed(float NewMoveSpeed)
{
        // Установить скорость движения по орбите
MoveSpeed = NewMoveSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPlanet::SetName(AnsiString NewName)
{
        // Установить название
Name = NewName;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPlanet::SetFile(AnsiString NewFile)
{
        // Установить путь к файлу с моделью
File = NewFile;
return true;
}
//---------------------------------------------------------------------------
