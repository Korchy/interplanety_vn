//---------------------------------------------------------------------------
#pragma hdrstop

#include "Object3DPortal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
Object3DPortal::Object3DPortal(void)
{
        // Конструктор класса без параметров
        // Инициализация переменных
Bound = new BoundingSphere();
AlternativeFile = "s0\\Portal.x";
}
//---------------------------------------------------------------------------
Object3DPortal::~Object3DPortal()
{
        // Деструктор класса
delete Bound;
}
//---------------------------------------------------------------------------
//                                  ФУНКЦИИ
//---------------------------------------------------------------------------
bool Object3DPortal::SetRadius(float NewBig, float NewSmall)
{
        // Установить параметры орбиты
BigRadius = NewBig;
SmallRadius = NewSmall;
if(Mesh!=NULL) {        // Переместить портал в начальную точку
        MoveInto(-BigRadius,0.0,0.0);
        UpdateTransform();
        }
return true;
}
//---------------------------------------------------------------------------
bool Object3DPortal::SetMoveSpeed(float NewMoveSpeed)
{
        // Установить скорость движения по орбите
MoveSpeed = NewMoveSpeed;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPortal::SetDest(AnsiString NewDest)
{
        // Установить сектор, в который ведет портал
Dest = NewDest;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPortal::SetName(AnsiString NewName)
{
        // Установить название
Name = NewName;
return true;
}
//---------------------------------------------------------------------------
bool Object3DPortal::SetFile(AnsiString NewFile)
{
        // Установить путь к файлу с моделью
File = NewFile;
return true;
}
//---------------------------------------------------------------------------
