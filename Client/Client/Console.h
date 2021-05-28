#pragma once
//---------------------------------------------------------------------------
#ifndef ConsoleH
#define ConsoleH
//---------------------------------------------------------------------------
// ����� ������� - ������ ��� ������ ���������� ��������
//---------------------------------------------------------------------------
#include "Object.h"
#include <string>		// ��� ������������� std::string
#include <vector>		// ��� ������������� std::vector
#include "Text.h"		// ��� ������ �� ����� ������
//---------------------------------------------------------------------------
class Console :
	public Object
{
private:	// ������ ��� ����� ������
	// ����������
	Text* ConsoleShower;	// ������ Text ��� ������ ������ ������� �� �����
	std::string FontStyle;	// ����� ������ ������� (����� ���������� Courier ����� ����� ���� ��������� �������� �����)
	short FontSize;			// ������ ������
	char FontSymbolW;		// ������ ������ ������� (����� ��������� �������� �����)
	
	std::vector<std::string*> Strings;	// ������ ����� ������ �������
	std::string Buffer;					// ����� � ������� ��������� ������� �� ��������� �� � Strings

protected:	// ��� ����� ������ � ��������

public:		// �������� ����
	Console(void);
	~Console(void);

	// �������
	bool SetRenderDevice(LPDIRECT3DDEVICE8 Device);	// ���������� ���������� ����������
	bool SetRect(int Left,int Top,int Right,int Bottom);	// ��������� ������� ������ ������
	bool Draw();	// ����� ����������� ������� �� �����
	bool Reset();	// �������� ������ (��� ����������������� ���������� ����������)
	bool Restore();	// ������������ ������ ����� ����, ��� �� ��� �������

	bool Add(std::string Str);	// �������� � ����� ����� ������������������ ��������, ���� ���� ������� ������ - ��� ����������� ����������� � Strings
	bool KeyPress(char Key,char Params);	// ��������� ������� ������ �� ����������
};
#endif