//---------------------------------------------------------------------------
#ifndef Object3DPortalH
#define Object3DPortalH
//---------------------------------------------------------------------------
#include "Object3DUnit.h"
#include "BoundingSphere.h"
//---------------------------------------------------------------------------
// Класс Object3DPortal. Объект "Портал"
//---------------------------------------------------------------------------
class Object3DPortal : public Object3DUnit
{
protected:      // Доступно здесь и для потомков
        // Переменные


private:        // Доступно только здесь
        // Переменные
        float BigRadius;
        float SmallRadius;      // 2 радиуса, описывющие орбиту портала (эллипс)
        float MoveSpeed;        // Скорость движения по орбите углы/ед.времени
        AnsiString Dest;        // Сектор, в который ведет портал
        AnsiString Name;        // Название
        AnsiString File;        // Путь к файлу с моделью

public:         // Доступно везде
        Object3DPortal(void);   // Констуктор класса без параметров
        ~Object3DPortal();      // Деструктор класса

        // Переменные

        // Функции
        bool SetRadius(float NewBig, float NewSmall);   // Установить параметры орбиты
        bool SetMoveSpeed(float NewMoveSpeed);          // Установить скорость движения по орбите
        bool SetDest(AnsiString NewDest);               // Установить сектор, в который ведет портал
        bool SetName(AnsiString NewName);               // Установить название
        bool SetFile(AnsiString NewFile);               // Установить путь к файлу с моделью

};
//---------------------------------------------------------------------------
#endif
