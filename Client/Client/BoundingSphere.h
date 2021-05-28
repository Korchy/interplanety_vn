#pragma once
//---------------------------------------------------------------------------
#ifndef BoundingSphereH
#define BoundingSphereH
//---------------------------------------------------------------------------
// Класс BoundingSphere. Bounding-объект -> Сфера
//---------------------------------------------------------------------------
#include "BoundingObject.h"
//---------------------------------------------------------------------------
class BoundingSphere :
	public BoundingObject
{
private:	// Только для этого класса
	// Переменные
	// Сфера описывается 2 параметрами (вектором центра Position и радиусом):
	float Radius;	// Радиус

protected:	// Для этого класса и потомков

public:		// Доступно всем
	BoundingSphere(void);
	~BoundingSphere(void);
	// Переменные

	// Функции
	bool CreateBy3DObject(void* Object3DVertexBuffer,int Object3DVertexCount, unsigned int Object3DVertexFVF);	// Вычисление BoundingSphere для переданного 3D-объекта

	bool CheckVisibility();	// Проверка на попадание BoundingSphere в пирамиду видимости Frustum

	bool CheckIntersection(BoundingObject* BO);	// Проверка на перечечение другого BoundingObject (BO) c данным  (метод нужен, чтобы вызывать проверку не вникая в реальный тип BoundingObject)
	bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd);	// Проверка на пересечение BoundingSphere с линией заданной двумя точками
	bool CheckIntersectionWithSphere(D3DXVECTOR3 const &Position1, float Radius1);	// Проверка на пересечение BoundingSphere с BoundingSphere
	bool CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX, D3DXVECTOR3 const &OBBVecY, D3DXVECTOR3 const &OBBVecZ, D3DXVECTOR3 const &PositionOBB, float OBBSizeX, float OBBSizeY, float OBBSizeZ);	// Проверка на пересечение BoundingSphere с OBB

	bool UpdateTransform(D3DXMATRIX const &TransformMatrix,float SX,float SY,float SZ);	// Применить матрицу трансформации к BoundingObject, SX,SY,SZ - коэффициенты масштабирования при изменении масштаба
};
#endif