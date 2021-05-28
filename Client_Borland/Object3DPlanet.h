//---------------------------------------------------------------------------
#ifndef Object3DPlanetH
#define Object3DPlanetH
//---------------------------------------------------------------------------
#include "Object3DUnit.h"
#include "BoundingSphere.h"
//---------------------------------------------------------------------------
// Класс Object3DPlanet. Объект "Планета" - спутник, вращающийся вокруг звезды
//---------------------------------------------------------------------------
class Object3DPlanet : public Object3DUnit
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
        Object3DPlanet(void);   // Констуктор класса без параметров
        ~Object3DPlanet();      // Деструктор класса

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
