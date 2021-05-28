#pragma once
//---------------------------------------------------------------------------
#ifndef PlanetH
#define PlanetH
//---------------------------------------------------------------------------
// Класс Planet. Объект "Планета" - центральное тело в системе
//---------------------------------------------------------------------------
#include "Object3DBo.h"
#include "BoundingSphere.h"	// Для создания BoundingSphere
//---------------------------------------------------------------------------
class Planet :
	public Object3DBo
{
private:	// Только для этого класса
	// Переменные
	float X;			// Положение
	float Y;
	float Z;
	float RotateSpeed;	// Скорость вращения вокруг собственной оси углы/ед.времени
	std::string Name;	// Название
	std::string File;	// Путь к файлу с моделью

protected:	// Для этого класса и потомков

public:		// Доступно всем
	Planet(void);
	~Planet(void);

	// Переменные

	// Функции
	bool SetPosition(float NewX,float NewY,float NewZ);	// Установить положение звезды
	bool SetRotateSpeed(float NewRotateSpeed);			// Установить скорость вращения звезды
	bool SetName(std::string NewName);					// Установить название зведы
	bool SetFile(std::string NewFile);					// Установить путь к файлу с моделью
};
#endif