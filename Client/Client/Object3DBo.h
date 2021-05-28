#pragma once
//---------------------------------------------------------------------------
#ifndef Object3DBoH
#define Object3DBoH
//---------------------------------------------------------------------------
// Класс Object3DBo. Общий класс терхмерных объектов расширенный 
// Bounding - фигурой. (Object3DUnit)
//---------------------------------------------------------------------------
#include "Object3D.h"
#include "BoundingObject.h"	// BoundingObject
//---------------------------------------------------------------------------
class Object3DBo :
	public Object3D
{
private:	// Только для этого класса

protected:	// Для этого класса и потомков
	// Переменные
	BoundingObject* Bound;	// BoundingObject
	float BoundSX;			// Коэффициенты для масштабирования BoundingObject
	float BoundSY;
	float BoundSZ;

public:		// Доступно всем
	Object3DBo(void);
	~Object3DBo(void);

	// Переменные


	// Функции
	bool LoadFromFile(std::string File);	// Создание (загрузка) объекта из файла

	bool CheckVisibility();	// Проверка видимости объекта

	bool Scale(float SX,float SY,float SZ);	// Масштабирование объекта по осям
	bool UpdateTransform();	// Обновить объект т.е. применить к нему полученные трансформации

	bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd,float* Distance);	// Проверка на пересечение отрезка заданного точками LineStart и LineEnd хотя бы с одним полигоном объекта, возвращает true если пересечение есть, false - нет, Distance - выходной параметр = расстоянию до точки пересечения
//	bool CheckIntersections(TList* AllList,TList* IntersectionList);	// Проверка на пересечение данного объекта со всеми объектами списка AllList, возвращает список IntersectionList с пересеченными объектами

};
#endif