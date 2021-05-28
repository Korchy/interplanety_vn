#pragma once
//---------------------------------------------------------------------------
#ifndef SceneH
#define SceneH
//---------------------------------------------------------------------------
// Класс Scene. Содержит в себе полную сцену с набором различных объектов
// (т.е. содержит в себе все объекты сектора)
//---------------------------------------------------------------------------
#include "Object.h"
#include <stdio.h>		// Для чтения-записи файлов
#include <vector>		// Для использования std::vector
#include "Planet.h"		// Объект "Планета"
#include "Satellite.h"	// Объект "Спутник"
#include "Portal.h"		// Объект "Портал"
#include "Base.h"		// Объект "База"
#include "SkyBox.h"		// Объект SkyBox
//---------------------------------------------------------------------------
class Scene :
	public Object
{
private:	// Только для этого класса

	// Переменные
	int Version;		// Текущая версия сектора
	std::string Name;	// Название сектора 

protected:	// Для этого класса и потомков

public:		// Доступно всем
	Scene(void);
	~Scene(void);

	// Переменные
	bool Created;	// true - сектор загружен (создан), false - нет

	SkyBox* SBox;	// Небесная сфера (фон)
	std::vector<Planet*> Planets;		// Звезды
	std::vector<Satellite*> Satellits;	// Спутники
	std::vector<Portal*> Portals;		// Порталы
	std::vector<Base*> Bases;			// Базы


	// Функции
	bool LoadFromFile(std::string File);	// Загрузка данных по сектору из файла
	bool Draw();							// Вывод сцены на экран

	bool CheckVisibility();	// Проверка видимости сцены (т.е. каждого объекта в сцене)

	bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance, Object* &IntersectedObject);	// Проверка на пересечение отрезка заданного точками LineStart и LineEnd хотя бы с одним из объектов сектора, возвращает true если пересечение есть, false - нет, Distance,IntersectedObject - выходные параметры = расстоянию до точки пересечения и указателю на объект с которым произошло пересечение
};
#endif