//---------------------------------------------------------------------------
#pragma hdrstop

#include "ObjectText.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
ObjectText::ObjectText(LPDIRECT3DDEVICE8 Device)
{
        // ���������� ������ (� ���������� �.�. ����� ������������� ����� ������
        // �� ��������� ����� ���������� ����������)
        // ������������� ����������
Color = D3DCOLOR_XRGB(0xF8,0xD8,0x30);          // ���� ������ �� ���������
Rect.left = 55;                                // ��������� �� ��������� (����� ������� ����, ��� �����������)
Rect.top = 3;
Rect.right = 0;
Rect.bottom = 0;
pD3DDevice = Device;
Font = NULL;                                    
SetTextFont(12,FW_NORMAL,false,false,"Arial");  // ����� �� ��������� (Arial, 12)
Text = "������ \"����������\"";
}
//---------------------------------------------------------------------------
ObjectText::~ObjectText()
{
        // ���������� ������
if(Font!=NULL) Font->Release();
}
//---------------------------------------------------------------------------
//                                   �������
//---------------------------------------------------------------------------
bool ObjectText::Draw()
{
        // ����� ������ � �����
if(Font==NULL) return false;    // ����� �� ��������������� �������
        // ������� ����� �� ����� ����� ���������� ���������� pD3DDevice
Font->Begin();
Font->DrawTextA(Text.c_str(),-1,&Rect,0,Color);
Font->End();
return true;
}
//---------------------------------------------------------------------------
bool ObjectText::Reset()
{
        // �������� ������ (��� ����������������� ���������� ����������)
Font->OnLostDevice();
return true;
}
//---------------------------------------------------------------------------
bool ObjectText::Restore()
{
        // ��������������� ������ ����� ����, ��� �� ��� �������
Font->OnResetDevice();
return true;
}
//---------------------------------------------------------------------------
bool ObjectText::SetTextFont(short Size,int Bold,bool Italic,bool Underline,AnsiString Face)
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
bool ObjectText::SetRect(int Left, int Top, int Right, int Bottom)
{
        // ���������� ������� ������ ������
Rect.left = Left;
Rect.top = Top;
Rect.right = Right;
Rect.bottom = Bottom;
return true;
}
//---------------------------------------------------------------------------
bool ObjectText::SetColor(D3DCOLOR NewColor)
{
        // ���������� ���� ������
Color = NewColor;
return true;
}
//---------------------------------------------------------------------------
