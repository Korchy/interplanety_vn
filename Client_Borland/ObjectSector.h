//---------------------------------------------------------------------------
#ifndef ObjectSectorH
#define ObjectSectorH
//---------------------------------------------------------------------------
#include "Object.h"
#include <stdio.h>              // Для чтения-записи файлов
#include "Object3DStar.h"       // Объект "Звезда"
#include "Object3DPlanet.h"     // Объект "Планета"
#include "Object3DPortal.h"     // Объект "Портал"
#include "Object3DBase.h"       // Объект "База"
#include "ObjectSkyBox.h"       // Объект SkyBox
//---------------------------------------------------------------------------
// Класс ObjectSector. Содержит в себе карту одного сектора. Включает в себя
// звезду, планеты и т.п. и их свойства
//---------------------------------------------------------------------------
class ObjectSector : public Object
{
protected:      // Доступно здесь и для потомков
        // Переменные

private:        // Доступно только здесь
        // Переменные
        int Version;            // Текущая версия сектора
        AnsiString Name;        // Название сектора 

public:         // Доступно везде
        ObjectSector(void);     // Констуктор класса без параметров
        ~ObjectSector();        // Деструктор класса

        // Переменные
        bool Created;           // true - сектор загружен (создан), false - нет

        ObjectSkyBox* SkyBox;   // Небесная сфера (фон)
        TList* Stars;           // Звезды
        TList* Planets;         // Планеты
        TList* Portals;         // Порталы
        TList* Bases;           // Базы


        // Функции
        bool LoadFromFile(AnsiString File);             // Загрузка данных по сектору из файла
        bool Draw();                                    // Вывод объектов сектора в сцену

        bool CheckVisibility();         // Проверка видимости объекта

        bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance, Object* &IntersectedObject);   // Проверка на пересечение отрезка заданного точками LineStart и LineEnd хотя бы с одним из объектов сектора, возвращает true если пересечение есть, false - нет, Distance,IntersectedObject - выходные параметры = расстоянию до точки пересечения и указателю на объект с которым произошло пересечение

};
//---------------------------------------------------------------------------
#endif
