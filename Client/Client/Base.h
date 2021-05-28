#pragma once
//---------------------------------------------------------------------------
#ifndef BaseH
#define BaseH
//---------------------------------------------------------------------------
// Класс Base. Объект "База" - спутник, вращающийся вокруг планеты со
// специальными свойствами
//---------------------------------------------------------------------------
#include "Object3DBo.h"
#include "BoundingSphere.h"	// Для создания BoundingSphere
//---------------------------------------------------------------------------
class Base :
	public Object3DBo
{
private:	// Только для этого класса
	// Переменные
	float BigRadius;	// 2 радиуса, описывющие орбиту спутника (эллипс)
	float SmallRadius;
	float RotateSpeed;	// Скорость вращения вокруг собственной оси углы/ед.времени
	float MoveSpeed;	// Скорость движения по орбите углы/ед.времени
	std::string Name;	// Название
	std::string File;	// Путь к файлу с моделью

protected:	// Для этого класса и потомков

public:		// Доступно всем
	Base(void);
	~Base(void);

	// Переменные

	// Функции
	bool SetRadius(float NewBig,float NewSmall);	// Установить параметры орбиты
	bool SetRotateSpeed(float NewRotateSpeed);		// Установить скорость вращения
	bool SetMoveSpeed(float NewMoveSpeed);			// Установить скорость движения по орбите
	bool SetName(std::string NewName);				// Установить название
	bool SetFile(std::string NewFile);				// Установить путь к файлу с моделью

};
#endif