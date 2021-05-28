#pragma once
//---------------------------------------------------------------------------
#ifndef PlanetH
#define PlanetH
//---------------------------------------------------------------------------
// ����� Planet. ������ "�������" - ����������� ���� � �������
//---------------------------------------------------------------------------
#include "Object3DBo.h"
#include "BoundingSphere.h"	// ��� �������� BoundingSphere
//---------------------------------------------------------------------------
class Planet :
	public Object3DBo
{
private:	// ������ ��� ����� ������
	// ����������
	float X;			// ���������
	float Y;
	float Z;
	float RotateSpeed;	// �������� �������� ������ ����������� ��� ����/��.�������
	std::string Name;	// ��������
	std::string File;	// ���� � ����� � �������

protected:	// ��� ����� ������ � ��������

public:		// �������� ����
	Planet(void);
	~Planet(void);

	// ����������

	// �������
	bool SetPosition(float NewX,float NewY,float NewZ);	// ���������� ��������� ������
	bool SetRotateSpeed(float NewRotateSpeed);			// ���������� �������� �������� ������
	bool SetName(std::string NewName);					// ���������� �������� �����
	bool SetFile(std::string NewFile);					// ���������� ���� � ����� � �������
};
#endif