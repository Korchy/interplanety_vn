#pragma once
//---------------------------------------------------------------------------
#ifndef PortalH
#define PortalH
//---------------------------------------------------------------------------
// Класс Portal. Объект "Портал"
//---------------------------------------------------------------------------
#include "Object3DBo.h"
#include "BoundingSphere.h"	// Для создания BoundingSphere
//---------------------------------------------------------------------------
class Portal :
	public Object3DBo
{
private:	// Только для этого класса
	// Переменные
	float BigRadius;	// 2 радиуса, описывющие орбиту портала (эллипс)
	float SmallRadius;
	float MoveSpeed;	// Скорость движения по орбите углы/ед.времени
	std::string Dest;	// Сектор, в который ведет портал
	std::string Name;	// Название
	std::string File;	// Путь к файлу с моделью

protected:	// Для этого класса и потомков

public:		// Доступно всем
	Portal(void);
	~Portal(void);

	// Переменные

	// Функции
	bool SetRadius(float NewBig,float NewSmall);	// Установить параметры орбиты
	bool SetMoveSpeed(float NewMoveSpeed);			// Установить скорость движения по орбите
	bool SetDest(std::string NewDest);				// Установить сектор, в который ведет портал
	bool SetName(std::string NewName);				// Установить название
	bool SetFile(std::string NewFile);				// Установить путь к файлу с моделью

};
#endif