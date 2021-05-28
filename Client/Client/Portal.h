#pragma once
//---------------------------------------------------------------------------
#ifndef PortalH
#define PortalH
//---------------------------------------------------------------------------
// ����� Portal. ������ "������"
//---------------------------------------------------------------------------
#include "Object3DBo.h"
#include "BoundingSphere.h"	// ��� �������� BoundingSphere
//---------------------------------------------------------------------------
class Portal :
	public Object3DBo
{
private:	// ������ ��� ����� ������
	// ����������
	float BigRadius;	// 2 �������, ���������� ������ ������� (������)
	float SmallRadius;
	float MoveSpeed;	// �������� �������� �� ������ ����/��.�������
	std::string Dest;	// ������, � ������� ����� ������
	std::string Name;	// ��������
	std::string File;	// ���� � ����� � �������

protected:	// ��� ����� ������ � ��������

public:		// �������� ����
	Portal(void);
	~Portal(void);

	// ����������

	// �������
	bool SetRadius(float NewBig,float NewSmall);	// ���������� ��������� ������
	bool SetMoveSpeed(float NewMoveSpeed);			// ���������� �������� �������� �� ������
	bool SetDest(std::string NewDest);				// ���������� ������, � ������� ����� ������
	bool SetName(std::string NewName);				// ���������� ��������
	bool SetFile(std::string NewFile);				// ���������� ���� � ����� � �������

};
#endif