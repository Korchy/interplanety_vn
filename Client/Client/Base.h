#pragma once
//---------------------------------------------------------------------------
#ifndef BaseH
#define BaseH
//---------------------------------------------------------------------------
// ����� Base. ������ "����" - �������, ����������� ������ ������� ��
// ������������ ����������
//---------------------------------------------------------------------------
#include "Object3DBo.h"
#include "BoundingSphere.h"	// ��� �������� BoundingSphere
//---------------------------------------------------------------------------
class Base :
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
	Base(void);
	~Base(void);

	// ����������

	// �������
	bool SetRadius(float NewBig,float NewSmall);	// ���������� ��������� ������
	bool SetRotateSpeed(float NewRotateSpeed);		// ���������� �������� ��������
	bool SetMoveSpeed(float NewMoveSpeed);			// ���������� �������� �������� �� ������
	bool SetName(std::string NewName);				// ���������� ��������
	bool SetFile(std::string NewFile);				// ���������� ���� � ����� � �������

};
#endif