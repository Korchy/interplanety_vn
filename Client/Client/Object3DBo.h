#pragma once
//---------------------------------------------------------------------------
#ifndef Object3DBoH
#define Object3DBoH
//---------------------------------------------------------------------------
// ����� Object3DBo. ����� ����� ���������� �������� ����������� 
// Bounding - �������. (Object3DUnit)
//---------------------------------------------------------------------------
#include "Object3D.h"
#include "BoundingObject.h"	// BoundingObject
//---------------------------------------------------------------------------
class Object3DBo :
	public Object3D
{
private:	// ������ ��� ����� ������

protected:	// ��� ����� ������ � ��������
	// ����������
	BoundingObject* Bound;	// BoundingObject
	float BoundSX;			// ������������ ��� ��������������� BoundingObject
	float BoundSY;
	float BoundSZ;

public:		// �������� ����
	Object3DBo(void);
	~Object3DBo(void);

	// ����������


	// �������
	bool LoadFromFile(std::string File);	// �������� (��������) ������� �� �����

	bool CheckVisibility();	// �������� ��������� �������

	bool Scale(float SX,float SY,float SZ);	// ��������������� ������� �� ����
	bool UpdateTransform();	// �������� ������ �.�. ��������� � ���� ���������� �������������

	bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd,float* Distance);	// �������� �� ����������� ������� ��������� ������� LineStart � LineEnd ���� �� � ����� ��������� �������, ���������� true ���� ����������� ����, false - ���, Distance - �������� �������� = ���������� �� ����� �����������
//	bool CheckIntersections(TList* AllList,TList* IntersectionList);	// �������� �� ����������� ������� ������� �� ����� ��������� ������ AllList, ���������� ������ IntersectionList � ������������� ���������

};
#endif