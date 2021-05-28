//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Console.h"
//---------------------------------------------------------------------------
Console::Console(void)
{
	// Конструктор класса
ConsoleShower = NULL;
Visible = false;
Strings.clear();
Buffer.clear();
Buffer += ">";
FontStyle = "Courier New";	// Берем моноширный Courier New чтобы проще было учитывать переносы строк
FontSize = 14;				// Берем размер 14
FontSymbolW = 7;			// При Courier New,14 -> р-р символа 7х14 пикселей
}
//---------------------------------------------------------------------------
Console::~Console(void)
{
	// Дестуктор класса
	// Удалить объект Text
if(ConsoleShower!=NULL) delete ConsoleShower;
	// Удалить все строки в консоли
if(!Strings.empty()) {
	for(int i=0; i<(int)Strings.size(); i++) {
		if(Strings[i]!=NULL) delete Strings[i];
	}
}
}
//---------------------------------------------------------------------------
bool Console::SetRenderDevice(LPDIRECT3DDEVICE8 Device)
{
	// Назначение устройства рендеринга
bool Rez = Object::SetRenderDevice(Device);
if(Rez==true) {
	ConsoleShower = new Text(Device);
	ConsoleShower->SetTextFont(FontSize,FW_NORMAL,false,false,FontStyle);
	ConsoleShower->SetRect(5,20,100,100);	// По умолчанию выводим консоль в область 5,20x100,100
	ConsoleShower->SetColor(D3DCOLOR_XRGB(0xFF,0x00,0x00));
	ConsoleShower->SetFormat(DT_LEFT|DT_WORDBREAK|DT_WORD_ELLIPSIS);	// Выравнивание по левому краю, перенос текста, строкчки уходящие за край консоли заканчивать троеточием
	Visible = true;
	return true;
}
else return false;
}
//---------------------------------------------------------------------------
bool Console::SetRect(int Left,int Top,int Right,int Bottom)
{
	// Установка области вывода текста
bool Rez = ConsoleShower->SetRect(Left,Top,Right,Bottom);
	// Для начала, чтобы опустить курсор в низ консоли
/*
std::string ShowString = "";
for(int i=(int)Strings.size()-5; i<(int)Strings.size(); i++) {
	if(i>=0) ShowString += *Strings[i];
	else ShowString += "\n";
}
	// Шестой строкой выводим буфер
ConsoleShower->Txt = ShowString+Buffer;
*/
return Rez;
}
//---------------------------------------------------------------------------
bool Console::Draw()
{
	// Вывод содержимого консоли на экран
if(Visible==true) {
	ConsoleShower->Draw();
}
return true;
}
//---------------------------------------------------------------------------
bool Console::Reset()
{
	// Сбросить объект (нужно для некоторых объектов при пересоздании устройства рендеринга)
ConsoleShower->Reset();
return true;
}
//---------------------------------------------------------------------------
bool Console::Restore()
{
	// Восстановление объекта после того, как он был сброшен
ConsoleShower->Restore();
return true;
}
//---------------------------------------------------------------------------
bool Console::Add(std::string Str)
{
	// Добавить в буфер новую последовательность символов, если есть перенос
	// строки - все накопленное добавляется в Strings
switch(Str[0]) {
	case VK_ESCAPE:	// Клавиша Esc
		break;
	case VK_BACK:	// Клавиша BackSpace
		if(Buffer.length()>1) Buffer.erase(Buffer.end()-1);
		break;
	// Все остальные клавиши
	default:
		Buffer += Str;
		if(Buffer[Buffer.length()-1]==VK_RETURN) {
			// Перенести текст из буфера в список строк
			std::string* TmpString = new std::string();
			*TmpString = Buffer.erase(0,1);
			Strings.push_back(TmpString);
			Buffer.clear();
			Buffer = ">";
		}
}
	// Обновить ConsoleShower (выводим последние 5 строк)
std::string ShowString = "";
for(int i=(int)Strings.size()-5; i<(int)Strings.size(); i++) {
	if(i>=0) ShowString += *Strings[i];
	else ShowString += "\n";
}
	// Шестой строкой выводим буфер
ConsoleShower->Txt = ShowString+Buffer;
return true;
}
//---------------------------------------------------------------------------
bool Console::KeyPress(char Key,char Params)
{
	// Обработка нажатия клавиш на клавиатуре
char TmpKey[2];
TmpKey[0] = (char)Key;
TmpKey[1] = 0;
Add(std::string(TmpKey));
switch(TmpKey[0]) {
	case VK_ESCAPE:	// Клавиша Esc
		ConsoleShower->SetColor(D3DCOLOR_XRGB(0xF8,0xD8,0x30));
		break;
}

return true;
}
//---------------------------------------------------------------------------