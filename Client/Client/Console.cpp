//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Console.h"
//---------------------------------------------------------------------------
Console::Console(void)
{
	// ����������� ������
ConsoleShower = NULL;
Visible = false;
Strings.clear();
Buffer.clear();
Buffer += ">";
FontStyle = "Courier New";	// ����� ���������� Courier New ����� ����� ���� ��������� �������� �����
FontSize = 14;				// ����� ������ 14
FontSymbolW = 7;			// ��� Courier New,14 -> �-� ������� 7�14 ��������
}
//---------------------------------------------------------------------------
Console::~Console(void)
{
	// ��������� ������
	// ������� ������ Text
if(ConsoleShower!=NULL) delete ConsoleShower;
	// ������� ��� ������ � �������
if(!Strings.empty()) {
	for(int i=0; i<(int)Strings.size(); i++) {
		if(Strings[i]!=NULL) delete Strings[i];
	}
}
}
//---------------------------------------------------------------------------
bool Console::SetRenderDevice(LPDIRECT3DDEVICE8 Device)
{
	// ���������� ���������� ����������
bool Rez = Object::SetRenderDevice(Device);
if(Rez==true) {
	ConsoleShower = new Text(Device);
	ConsoleShower->SetTextFont(FontSize,FW_NORMAL,false,false,FontStyle);
	ConsoleShower->SetRect(5,20,100,100);	// �� ��������� ������� ������� � ������� 5,20x100,100
	ConsoleShower->SetColor(D3DCOLOR_XRGB(0xFF,0x00,0x00));
	ConsoleShower->SetFormat(DT_LEFT|DT_WORDBREAK|DT_WORD_ELLIPSIS);	// ������������ �� ������ ����, ������� ������, �������� �������� �� ���� ������� ����������� ����������
	Visible = true;
	return true;
}
else return false;
}
//---------------------------------------------------------------------------
bool Console::SetRect(int Left,int Top,int Right,int Bottom)
{
	// ��������� ������� ������ ������
bool Rez = ConsoleShower->SetRect(Left,Top,Right,Bottom);
	// ��� ������, ����� �������� ������ � ��� �������
/*
std::string ShowString = "";
for(int i=(int)Strings.size()-5; i<(int)Strings.size(); i++) {
	if(i>=0) ShowString += *Strings[i];
	else ShowString += "\n";
}
	// ������ ������� ������� �����
ConsoleShower->Txt = ShowString+Buffer;
*/
return Rez;
}
//---------------------------------------------------------------------------
bool Console::Draw()
{
	// ����� ����������� ������� �� �����
if(Visible==true) {
	ConsoleShower->Draw();
}
return true;
}
//---------------------------------------------------------------------------
bool Console::Reset()
{
	// �������� ������ (����� ��� ��������� �������� ��� ������������ ���������� ����������)
ConsoleShower->Reset();
return true;
}
//---------------------------------------------------------------------------
bool Console::Restore()
{
	// �������������� ������� ����� ����, ��� �� ��� �������
ConsoleShower->Restore();
return true;
}
//---------------------------------------------------------------------------
bool Console::Add(std::string Str)
{
	// �������� � ����� ����� ������������������ ��������, ���� ���� �������
	// ������ - ��� ����������� ����������� � Strings
switch(Str[0]) {
	case VK_ESCAPE:	// ������� Esc
		break;
	case VK_BACK:	// ������� BackSpace
		if(Buffer.length()>1) Buffer.erase(Buffer.end()-1);
		break;
	// ��� ��������� �������
	default:
		Buffer += Str;
		if(Buffer[Buffer.length()-1]==VK_RETURN) {
			// ��������� ����� �� ������ � ������ �����
			std::string* TmpString = new std::string();
			*TmpString = Buffer.erase(0,1);
			Strings.push_back(TmpString);
			Buffer.clear();
			Buffer = ">";
		}
}
	// �������� ConsoleShower (������� ��������� 5 �����)
std::string ShowString = "";
for(int i=(int)Strings.size()-5; i<(int)Strings.size(); i++) {
	if(i>=0) ShowString += *Strings[i];
	else ShowString += "\n";
}
	// ������ ������� ������� �����
ConsoleShower->Txt = ShowString+Buffer;
return true;
}
//---------------------------------------------------------------------------
bool Console::KeyPress(char Key,char Params)
{
	// ��������� ������� ������ �� ����������
char TmpKey[2];
TmpKey[0] = (char)Key;
TmpKey[1] = 0;
Add(std::string(TmpKey));
switch(TmpKey[0]) {
	case VK_ESCAPE:	// ������� Esc
		ConsoleShower->SetColor(D3DCOLOR_XRGB(0xF8,0xD8,0x30));
		break;
}

return true;
}
//---------------------------------------------------------------------------