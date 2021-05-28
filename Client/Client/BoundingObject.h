#pragma once
//---------------------------------------------------------------------------
#ifndef BoundingObjectH
#define BoundingObjectH
//---------------------------------------------------------------------------
// Класс BoundingObject. Общий класс для создания Bounding-фигуры (упрощенной
// фигуры обводящей объект для упрощения вычислений с потерей точности)
//---------------------------------------------------------------------------
#include "d3dx8.h"				// Подключение библиотеки D3D8X для использования ее функций
#include "FrustumPiramide.h"	// Пирамида видимости
//---------------------------------------------------------------------------
class BoundingObject
{
private:	// Только для этого класса

protected:	// Для этого класса и потомков
	// Переменные
	D3DXVECTOR3 Position;		// Положение центра BoungingObject
	FrustumPiramide* pFrustum;	// Указатель на пирамиду видимости для проверки видимости объекта камерой. Если pFrustum = NULL - считаем, что объект всегда виден. Сама пирамида видимости создается и пересчитывается в камере.

public:		// Доступно всем
	BoundingObject(void);
	~BoundingObject(void);

	// Переменные

	// Функции
	virtual bool CreateBy3DObject(void* Object3DVertexBuffer,int Object3DVertexCount,unsigned int Object3DVertexFVF)=0;		// Создать BoundingObject для конкретного объекта

	bool SetFrustum(FrustumPiramide* Frustum);		// Установка указателя на пирамиду видимости
	virtual bool CheckVisibility()=0;	// Проверка на попадание BoundingObject в пирамиду видимости

	virtual bool CheckIntersection(BoundingObject* BO)=0;	// Проверка на перечечение другого BoundingObject (BO) c данным (метод нужен, чтобы вызывать проверку не вникая в реальный тип BoundingObject)
	virtual bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd)=0;	// Проверка на пересечение BoundingObject с линией заданной двумя точками
	virtual bool CheckIntersectionWithSphere(D3DXVECTOR3 const &Position1, float Radius1)=0;			// Проверка на пересечение BoundingObject со сферой
	virtual bool CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX, D3DXVECTOR3 const &OBBVecY, D3DXVECTOR3 const &OBBVecZ, D3DXVECTOR3 const &PositionOBB, float OBBSizeX, float OBBSizeY, float OBBSizeZ)=0;	// Проверка на пересечение BoundingObject с OBB

	virtual bool UpdateTransform(D3DXMATRIX const &TransformMatrix, float SX, float SY, float SZ)=0;	// Применить матрицу трансформации к BoundingObject, SX,SY,SZ - коэффициенты масштабирования при изменении масштаба
};
#endif