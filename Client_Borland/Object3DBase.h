//---------------------------------------------------------------------------
#ifndef Object3DBaseH
#define Object3DBaseH
//---------------------------------------------------------------------------
#include "Object3DUnit.h"
#include "BoundingSphere.h"
//---------------------------------------------------------------------------
// Класс Object3DBase. Объект "База" - спутник, вращающийся вокруг звезды со
// специальными свойствами
//---------------------------------------------------------------------------
class Object3DBase : public Object3DUnit
{
protected:      // Доступно здесь и для потомков
        // Переменные


private:        // Доступно только здесь
        // Переменные
        float BigRadius;
        float SmallRadius;      // 2 радиуса, описывющие орбиту спутника (эллипс)
        float RotateSpeed;      // Скорость вращения вокруг собственной оси углы/ед.времени
        float MoveSpeed;        // Скорость движения по орбите углы/ед.времени
        AnsiString Name;        // Название
        AnsiString File;        // Путь к файлу с моделью

public:         // Доступно везде
        Object3DBase(void);     // Констуктор класса без параметров
        ~Object3DBase();        // Деструктор класса

        // Переменные

        // Функции
        bool SetRadius(float NewBig, float NewSmall);   // Установить параметры орбиты
        bool SetRotateSpeed(float NewRotateSpeed);      // Установить скорость вращения
        bool SetMoveSpeed(float NewMoveSpeed);          // Установить скорость движения по орбите
        bool SetName(AnsiString NewName);               // Установить название
        bool SetFile(AnsiString NewFile);               // Установить путь к файлу с моделью

};
//---------------------------------------------------------------------------
#endif
