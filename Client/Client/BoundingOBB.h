#pragma once
//---------------------------------------------------------------------------
#ifndef BoundingOBBH
#define BoundingOBBH
//---------------------------------------------------------------------------
// Класс BoundingOBB. Bounding-объект -> OBB
//---------------------------------------------------------------------------
#include "BoundingObject.h"
//---------------------------------------------------------------------------
class BoundingOBB :
	public BoundingObject
{
private:	// Только для этого класса
	// Переменные
	// OBB описывается 7 параметрами (вектором центра Position, 3 векторами от Position к центрам трех стенок OBB и 3 длинами этих векторов):
	D3DXVECTOR3 OBBVecX;    // 3 вектора направленные в центры трех сторон OBB
	D3DXVECTOR3 OBBVecY;
	D3DXVECTOR3 OBBVecZ;
	float OBBSizeX;         // 3 длины соответствующих векторов
	float OBBSizeY;
	float OBBSizeZ;

protected:	// Для этого класса и потомков

public:		// Доступно всем
	BoundingOBB(void);
	~BoundingOBB(void);
	
	// Переменные

	// Функции
	bool CreateBy3DObject(void* Object3DVertexBuffer,int Object3DVertexCount, unsigned int Object3DVertexFVF);	// Вычисление BoundingOBB по переданному 3D-объекту

	bool CheckVisibility();	// Проверка на попадание BoundingObject в пирамиду видимости Frustum

	bool CheckIntersection(BoundingObject* BO);		// Проверка на перечечение другого BoundingObject (BO) c данным  (метод нужен, чтобы вызывать проверку не вникая в реальный тип BoundingObject)        
	bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd);	// Проверка на пересечение BoundingOBB с линией заданной двумя точками
	bool CheckIntersectionWithSphere(D3DXVECTOR3 const &PositionS, float RadiusS);				// Проверка на пересечение BoundingOBB с BoundingSphere
	bool CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX1, D3DXVECTOR3 const &OBBVecY1, D3DXVECTOR3 const &OBBVecZ1, D3DXVECTOR3 const &PositionOBB1, float OBBSizeX1, float OBBSizeY1, float OBBSizeZ1);	// Проверка на пересечение BoundingOBB с BoundingOBB

	bool UpdateTransform(D3DXMATRIX const &TransformMatrix,float SX,float SY,float SZ);		// Применить матрицу трансформации к BoundingObject, SX,SY,SZ - коэффициенты масштабирования при изменении масштаба
};
#endif