//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Text.h"
//---------------------------------------------------------------------------
Text::Text(void)
{
	// Конструктор класса без параметров

}
//---------------------------------------------------------------------------
Text::Text(LPDIRECT3DDEVICE8 Device)
{
        // Констуктор класса (с параметром - устанавливать шрифт текста
        // по умолчанию через устройство рендеринга)
        // Инициализация переменных
Color = D3DCOLOR_XRGB(0xF8,0xD8,0x30);	// Цвет текста по умолчанию
Format = 0;								// По умолчанию - неформатированный текст
Rect.left = 0;							// Положение по умолчанию (левый верхний угол, без ограничения)
Rect.top = 0;
Rect.right = 0;
Rect.bottom = 0;
pD3DDevice = Device;
Font = NULL;                                    
SetTextFont(12,FW_NORMAL,false,false,"Arial");  // Шрифт по умолчанию (Arial, 12)
Txt = "Проект \"ВНЕЗЕМЕЛЬЕ\"";
}
//---------------------------------------------------------------------------
Text::~Text(void)
{
	// Деструктор класса
if(Font!=NULL) Font->Release();
}
//---------------------------------------------------------------------------
bool Text::Draw()
{
	// Вывод текста в сцену
if(Font==NULL) return false;	// Текст не инициализирован шрифтом
	// Выводим текст на экран через устройство рендеринга pD3DDevice
Font->Begin();
Font->DrawText(Txt.c_str(),-1,&Rect,Format,Color);
Font->End();
return true;
}
//---------------------------------------------------------------------------
bool Text::Reset()
{
	// Сбросить объект (при переинициализации устройства рендеринга)
Font->OnLostDevice();
return true;
}
//---------------------------------------------------------------------------
bool Text::Restore()
{
	// Восстанавливает объект после того, как он был сброшен
Font->OnResetDevice();
return true;
}
//---------------------------------------------------------------------------
bool Text::SetTextFont(short Size,int Bold,bool Italic,bool Underline,std::string Face)
{
	// Установление шрифта для текста
	// Удалить предыдущий шрифт
if(Font!=NULL) Font->Release();
	// Инициализировать новый шрифт
HFONT TmpFont = CreateFont(Size,0,0,0,Bold,Italic,Underline,0,RUSSIAN_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,Face.c_str());
D3DXCreateFont(pD3DDevice,TmpFont,&Font);
return true;
}
//---------------------------------------------------------------------------
bool Text::SetRect(int Left,int Top,int Right,int Bottom)
{
	// Установить область вывода текста
Rect.left = Left;
Rect.top = Top;
if(Right!=NULL) Rect.right = Right;
if(Bottom!=NULL) Rect.bottom = Bottom;
return true;
}
//---------------------------------------------------------------------------
bool Text::SetColor(D3DCOLOR NewColor)
{
	// Установить цвет текста
Color = NewColor;
return true;
}
//---------------------------------------------------------------------------
bool Text::SetFormat(DWORD Fmt)
{
	// Установить формат текста
Format = Fmt;
return true;
}
//---------------------------------------------------------------------------