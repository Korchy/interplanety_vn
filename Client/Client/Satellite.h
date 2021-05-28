#pragma once
//---------------------------------------------------------------------------
#ifndef SatelliteH
#define SatelliteH
//---------------------------------------------------------------------------
// ����� Satellite. ������ "�������" - �������, ����������� ������ �������
//---------------------------------------------------------------------------
#include "Object3DBo.h"
#include "BoundingSphere.h"	// ��� �������� BoundingSphere
//---------------------------------------------------------------------------
class Satellite :
	public Object3DBo
{
private:	// ������ ��� ����� ������

	// ����������
	float BigRadius;	// 2 �������, ���������� ������ �������� (������)
	float SmallRadius;
	float RotateSpeed;	// �������� �������� ������ ����������� ��� ����/��.�������
	float MoveSpeed;	// �������� �������� �� ������ ����/��.�������
	std::string Name;	// ��������
	std::string File;	// ���� � ����� � �������

protected:	// ��� ����� ������ � ��������

public:		// �������� ����
	Satellite(void);
	~Satellite(void);

	// ����������

	// �������
	bool SetRadius(float NewBig,float NewSmall);	// ���������� ��������� ������
	bool SetRotateSpeed(float NewRotateSpeed);		// ���������� �������� ��������
	bool SetMoveSpeed(float NewMoveSpeed);			// ���������� �������� �������� �� ������
	bool SetName(std::string NewName);				// ���������� ��������
	bool SetFile(std::string NewFile);				// ���������� ���� � ����� � �������
};
#endif