//---------------------------------------------------------------------------
#ifndef Object3DStarH
#define Object3DStarH
//---------------------------------------------------------------------------
#include "Object3DUnit.h"
#include "BoundingSphere.h"
//---------------------------------------------------------------------------
// Класс Object3DStar. Объект "Звезда" - центральное тело в системе
//---------------------------------------------------------------------------
class Object3DStar : public Object3DUnit
{
protected:      // Доступно здесь и для потомков
        // Переменные


private:        // Доступно только здесь
        // Переменные
         float X;
         float Y;
         float Z;                // Положение
         float RotateSpeed;      // Скорость вращения вокруг собственной оси углы/ед.времени
         AnsiString Name;        // Название
         AnsiString File;        // Путь к файлу с моделью

public:         // Доступно везде
        Object3DStar(void);      // Констуктор класса без параметров
        ~Object3DStar();         // Деструктор класса

        // Переменные

        // Функции
        bool SetPosition(float NewX,float NewY,float NewZ);     // Установить положение звезды
        bool SetRotateSpeed(float NewRotateSpeed);              // Установить скорость вращения звезды
        bool SetName(AnsiString NewName);                       // Установить название зведы
        bool SetFile(AnsiString NewFile);                       // Установить путь к файлу с моделью
};
//---------------------------------------------------------------------------
#endif
