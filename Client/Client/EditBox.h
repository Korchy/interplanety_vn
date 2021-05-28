#pragma once
//---------------------------------------------------------------------------
#ifndef EditBoxH
#define EditBoxH
//---------------------------------------------------------------------------
// ����� ��� ����� ������
//---------------------------------------------------------------------------
#include "Object.h"
#include "Text.h"		// ��� ������ �� ����� ������
//---------------------------------------------------------------------------
class EditBox :
	public Object
{
private:	// ������ ��� ����� ������
	// ����������
	Text* EditShower;	// ������ Text ��� ������ ������ EditBox �� �����
	std::string FontStyle;	// ����� ������
	short FontSize;			// ������ ������
	int Width;	// ������
	int Height;	// ������
	unsigned short CursorPos;	// ��������� ������� � ������ ������
	unsigned short FirstVisiblePos;	// ��������� ������� �������� �������

protected:	// ��� ����� ������ � ��������

public:	// �������� ����
	EditBox(void);
	~EditBox(void);

	// ����������
	std::string Value;	// �����, ��������� � EditBox

	// �������
	bool SetRenderDevice(LPDIRECT3DDEVICE8 Device);	// ���������� ���������� ����������
	bool SetRect(int Left,int Top,int Right,int Bottom);	// ��������� ������� ������ ������
	bool Draw();	// ����� ����������� EditBox �� �����
	bool Reset();	// �������� ������ (��� ����������������� ���������� ����������)
	bool Restore();	// ������������ ������ ����� ����, ��� �� ��� �������

	bool KeyPress(char Key,char Params);	// ��������� ������� ��������� ������ �� ����������
	bool SystemKeyPress(char Key,char Params);	// ��������� ������� ��������� ������ �� ����������
};
#endif