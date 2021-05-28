//---------------------------------------------------------------------------
#ifndef Object3DUnitH
#define Object3DUnitH
//---------------------------------------------------------------------------
#include "Object3D.h"
#include "BoundingObject.h"     // BoundingObject
//---------------------------------------------------------------------------
// Класс Object3DUnit. Абстрактный класс для создания объектов вселенной. Сюда
// входят Солнце, Планеты, Метеоры и т.д.
//---------------------------------------------------------------------------
class Object3DUnit : public Object3D
{
protected:      // Доступно здесь и для потомков
        // Переменные
        BoundingObject* Bound;
        float BoundSX;  // Коэффициенты для масштабирования BoundingObject
        float BoundSY;
        float BoundSZ;

private:        // Доступно только здесь
        // Переменные

public:         // Доступно везде
        Object3DUnit(void);     // Констуктор класса без параметров
        ~Object3DUnit();        // Деструктор класса

        // Переменные


        // Функции
        bool LoadFromFile(AnsiString File);     // Создание (загрузка) объекта из файла

        bool CheckVisibility(); // Проверка видимости объекта

        bool Scale(float SX, float SY, float SZ);       // Масштабирование объекта по осям
        bool UpdateTransform(); // Обновить объект т.е. применить к нему полученные трансформации

        bool CheckIntersections(TList* AllList, TList* IntersectionList);       // Проверка на пересечение данного объекта со всеми объектами списка AllList, возвращает список IntersectionList с пересеченными объектами

        bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance);     // Проверка на пересечение отрезка заданного точками LineStart и LineEnd хотя бы с одним полигоном объекта, возвращает true если пересечение есть, false - нет, Distance - выходной параметр = расстоянию до точки пересечения


};
//---------------------------------------------------------------------------
#endif
