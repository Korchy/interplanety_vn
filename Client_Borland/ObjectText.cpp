//---------------------------------------------------------------------------
#pragma hdrstop

#include "ObjectText.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
ObjectText::ObjectText(LPDIRECT3DDEVICE8 Device)
{
        // Констуктор класса (с параметром т.к. нужно устанавливать шрифт текста
        // по умолчанию через устройство рендеринга)
        // Инициализация переменных
Color = D3DCOLOR_XRGB(0xF8,0xD8,0x30);          // Цвет текста по умолчанию
Rect.left = 55;                                // Положение по умолчанию (левый верхний угол, без ограничения)
Rect.top = 3;
Rect.right = 0;
Rect.bottom = 0;
pD3DDevice = Device;
Font = NULL;                                    
SetTextFont(12,FW_NORMAL,false,false,"Arial");  // Шрифт по умолчанию (Arial, 12)
Text = "Проект \"ВНЕЗЕМЕЛЬЕ\"";
}
//---------------------------------------------------------------------------
ObjectText::~ObjectText()
{
        // Деструктор класса
if(Font!=NULL) Font->Release();
}
//---------------------------------------------------------------------------
//                                   ФУНКЦИИ
//---------------------------------------------------------------------------
bool ObjectText::Draw()
{
        // Вывод текста в сцену
if(Font==NULL) return false;    // Текст не инициализирован шрифтом
        // Выводим текст на экран через устройство рендеринга pD3DDevice
Font->Begin();
Font->DrawTextA(Text.c_str(),-1,&Rect,0,Color);
Font->End();
return true;
}
//---------------------------------------------------------------------------
bool ObjectText::Reset()
{
        // Сбросить объект (при переинициализации устройства рендеринга)
Font->OnLostDevice();
return true;
}
//---------------------------------------------------------------------------
bool ObjectText::Restore()
{
        // Восстанавливает объект после того, как он был сброшен
Font->OnResetDevice();
return true;
}
//---------------------------------------------------------------------------
bool ObjectText::SetTextFont(short Size,int Bold,bool Italic,bool Underline,AnsiString Face)
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
bool ObjectText::SetRect(int Left, int Top, int Right, int Bottom)
{
        // Установить область вывода текста
Rect.left = Left;
Rect.top = Top;
Rect.right = Right;
Rect.bottom = Bottom;
return true;
}
//---------------------------------------------------------------------------
bool ObjectText::SetColor(D3DCOLOR NewColor)
{
        // Установить цвет текста
Color = NewColor;
return true;
}
//---------------------------------------------------------------------------
