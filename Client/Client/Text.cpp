//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Text.h"
//---------------------------------------------------------------------------
Text::Text(void)
{
	// ����������� ������ ��� ����������

}
//---------------------------------------------------------------------------
Text::Text(LPDIRECT3DDEVICE8 Device)
{
        // ���������� ������ (� ���������� - ������������� ����� ������
        // �� ��������� ����� ���������� ����������)
        // ������������� ����������
Color = D3DCOLOR_XRGB(0xF8,0xD8,0x30);	// ���� ������ �� ���������
Format = 0;								// �� ��������� - ����������������� �����
Rect.left = 0;							// ��������� �� ��������� (����� ������� ����, ��� �����������)
Rect.top = 0;
Rect.right = 0;
Rect.bottom = 0;
pD3DDevice = Device;
Font = NULL;                                    
SetTextFont(12,FW_NORMAL,false,false,"Arial");  // ����� �� ��������� (Arial, 12)
Txt = "������ \"����������\"";
}
//---------------------------------------------------------------------------
Text::~Text(void)
{
	// ���������� ������
if(Font!=NULL) Font->Release();
}
//---------------------------------------------------------------------------
bool Text::Draw()
{
	// ����� ������ � �����
if(Font==NULL) return false;	// ����� �� ��������������� �������
	// ������� ����� �� ����� ����� ���������� ���������� pD3DDevice
Font->Begin();
Font->DrawText(Txt.c_str(),-1,&Rect,Format,Color);
Font->End();
return true;
}
//---------------------------------------------------------------------------
bool Text::Reset()
{
	// �������� ������ (��� ����������������� ���������� ����������)
Font->OnLostDevice();
return true;
}
//---------------------------------------------------------------------------
bool Text::Restore()
{
	// ��������������� ������ ����� ����, ��� �� ��� �������
Font->OnResetDevice();
return true;
}
//---------------------------------------------------------------------------
bool Text::SetTextFont(short Size,int Bold,bool Italic,bool Underline,std::string Face)
{
	// ������������ ������ ��� ������
	// ������� ���������� �����
if(Font!=NULL) Font->Release();
	// ���������������� ����� �����
HFONT TmpFont = CreateFont(Size,0,0,0,Bold,Italic,Underline,0,RUSSIAN_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,Face.c_str());
D3DXCreateFont(pD3DDevice,TmpFont,&Font);
return true;
}
//---------------------------------------------------------------------------
bool Text::SetRect(int Left,int Top,int Right,int Bottom)
{
	// ���������� ������� ������ ������
Rect.left = Left;
Rect.top = Top;
if(Right!=NULL) Rect.right = Right;
if(Bottom!=NULL) Rect.bottom = Bottom;
return true;
}
//---------------------------------------------------------------------------
bool Text::SetColor(D3DCOLOR NewColor)
{
	// ���������� ���� ������
Color = NewColor;
return true;
}
//---------------------------------------------------------------------------
bool Text::SetFormat(DWORD Fmt)
{
	// ���������� ������ ������
Format = Fmt;
return true;
}
//---------------------------------------------------------------------------