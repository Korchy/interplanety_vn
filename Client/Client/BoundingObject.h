#pragma once
//---------------------------------------------------------------------------
#ifndef BoundingObjectH
#define BoundingObjectH
//---------------------------------------------------------------------------
// ����� BoundingObject. ����� ����� ��� �������� Bounding-������ (����������
// ������ ��������� ������ ��� ��������� ���������� � ������� ��������)
//---------------------------------------------------------------------------
#include "d3dx8.h"				// ����������� ���������� D3D8X ��� ������������� �� �������
#include "FrustumPiramide.h"	// �������� ���������
//---------------------------------------------------------------------------
class BoundingObject
{
private:	// ������ ��� ����� ������

protected:	// ��� ����� ������ � ��������
	// ����������
	D3DXVECTOR3 Position;		// ��������� ������ BoungingObject
	FrustumPiramide* pFrustum;	// ��������� �� �������� ��������� ��� �������� ��������� ������� �������. ���� pFrustum = NULL - �������, ��� ������ ������ �����. ���� �������� ��������� ��������� � ��������������� � ������.

public:		// �������� ����
	BoundingObject(void);
	~BoundingObject(void);

	// ����������

	// �������
	virtual bool CreateBy3DObject(void* Object3DVertexBuffer,int Object3DVertexCount,unsigned int Object3DVertexFVF)=0;		// ������� BoundingObject ��� ����������� �������

	bool SetFrustum(FrustumPiramide* Frustum);		// ��������� ��������� �� �������� ���������
	virtual bool CheckVisibility()=0;	// �������� �� ��������� BoundingObject � �������� ���������

	virtual bool CheckIntersection(BoundingObject* BO)=0;	// �������� �� ����������� ������� BoundingObject (BO) c ������ (����� �����, ����� �������� �������� �� ������ � �������� ��� BoundingObject)
	virtual bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd)=0;	// �������� �� ����������� BoundingObject � ������ �������� ����� �������
	virtual bool CheckIntersectionWithSphere(D3DXVECTOR3 const &Position1, float Radius1)=0;			// �������� �� ����������� BoundingObject �� ������
	virtual bool CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX, D3DXVECTOR3 const &OBBVecY, D3DXVECTOR3 const &OBBVecZ, D3DXVECTOR3 const &PositionOBB, float OBBSizeX, float OBBSizeY, float OBBSizeZ)=0;	// �������� �� ����������� BoundingObject � OBB

	virtual bool UpdateTransform(D3DXMATRIX const &TransformMatrix, float SX, float SY, float SZ)=0;	// ��������� ������� ������������� � BoundingObject, SX,SY,SZ - ������������ ��������������� ��� ��������� ��������
};
#endif