#pragma once
//---------------------------------------------------------------------------
#ifndef SceneH
#define SceneH
//---------------------------------------------------------------------------
// ����� Scene. �������� � ���� ������ ����� � ������� ��������� ��������
// (�.�. �������� � ���� ��� ������� �������)
//---------------------------------------------------------------------------
#include "Object.h"
#include <stdio.h>		// ��� ������-������ ������
#include <vector>		// ��� ������������� std::vector
#include "Planet.h"		// ������ "�������"
#include "Satellite.h"	// ������ "�������"
#include "Portal.h"		// ������ "������"
#include "Base.h"		// ������ "����"
#include "SkyBox.h"		// ������ SkyBox
//---------------------------------------------------------------------------
class Scene :
	public Object
{
private:	// ������ ��� ����� ������

	// ����������
	int Version;		// ������� ������ �������
	std::string Name;	// �������� ������� 

protected:	// ��� ����� ������ � ��������

public:		// �������� ����
	Scene(void);
	~Scene(void);

	// ����������
	bool Created;	// true - ������ �������� (������), false - ���

	SkyBox* SBox;	// �������� ����� (���)
	std::vector<Planet*> Planets;		// ������
	std::vector<Satellite*> Satellits;	// ��������
	std::vector<Portal*> Portals;		// �������
	std::vector<Base*> Bases;			// ����


	// �������
	bool LoadFromFile(std::string File);	// �������� ������ �� ������� �� �����
	bool Draw();							// ����� ����� �� �����

	bool CheckVisibility();	// �������� ��������� ����� (�.�. ������� ������� � �����)

	bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance, Object* &IntersectedObject);	// �������� �� ����������� ������� ��������� ������� LineStart � LineEnd ���� �� � ����� �� �������� �������, ���������� true ���� ����������� ����, false - ���, Distance,IntersectedObject - �������� ��������� = ���������� �� ����� ����������� � ��������� �� ������ � ������� ��������� �����������
};
#endif