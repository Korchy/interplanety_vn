//---------------------------------------------------------------------------
#pragma hdrstop

#include "Object3DBase.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
Object3DBase::Object3DBase(void)
{
        // Конструктор класса без параметров
        // Инициализация переменных
Bound = new BoundingSphere();
AlternativeFile = "s0\\Base.x";
}
//---------------------------------------------------------------------------
Object3DBase::~Object3DBase()
{
        // Деструктор класса
delete Bound;
}
//---------------------------------------------------------------------------
//                                  ФУНКЦИИ
//---------------------------------------------------------------------------
bool Object3DBase::SetRadius(float NewBig, float NewSmall)
{
        // Установить параметры орбиты
BigRadius = NewBig;
SmallRadius = NewSmall;
if(Mesh!=NULL) {        // Переместить базу в начальную точку
        MoveInto(-BigRadius,0.0,0.0);
        UpdateTransform();
        }
return true;
}
//---------------------------------------------------------------------------
bool Object3DBase::SetRotateSpeed(float NewRotateSpeed)
{
        // Установить скорость собственного вращения
RotateSpeed = NewRotateSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Object3DBase::SetMoveSpeed(float NewMoveSpeed)
{
        // Установить скорость движения по орбите
MoveSpeed = NewMoveSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Object3DBase::SetName(AnsiString NewName)
{
        // Установить название
Name = NewName;
return true;
}
//---------------------------------------------------------------------------
bool Object3DBase::SetFile(AnsiString NewFile)
{
        // Установить путь к файлу с моделью
File = NewFile;
return true;
}
//---------------------------------------------------------------------------
