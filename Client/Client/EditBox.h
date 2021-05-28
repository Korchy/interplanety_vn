#pragma once
//---------------------------------------------------------------------------
#ifndef EditBoxH
#define EditBoxH
//---------------------------------------------------------------------------
// Класс для ввода текста
//---------------------------------------------------------------------------
#include "Object.h"
#include "Text.h"		// Для вывода на экран текста
//---------------------------------------------------------------------------
class EditBox :
	public Object
{
private:	// Только для этого класса
	// Переменные
	Text* EditShower;	// Объект Text для вывода текста EditBox на экран
	std::string FontStyle;	// Шрифт текста
	short FontSize;			// Размер шрифта
	int Width;	// Ширина
	int Height;	// Высота
	unsigned short CursorPos;	// Положение курсора в строке текста
	unsigned short FirstVisiblePos;	// Положение первого видимого символа

protected:	// Для этого класса и потомков

public:	// Доступно всем
	EditBox(void);
	~EditBox(void);

	// Переменные
	std::string Value;	// Текст, введенный в EditBox

	// Функции
	bool SetRenderDevice(LPDIRECT3DDEVICE8 Device);	// Назначение устройства рендеринга
	bool SetRect(int Left,int Top,int Right,int Bottom);	// Установка области вывода текста
	bool Draw();	// Вывод содержимого EditBox на экран
	bool Reset();	// Сбросить объект (при переинициализации устройства рендеринга)
	bool Restore();	// Восстановить объект после того, как он был сброшен

	bool KeyPress(char Key,char Params);	// Обработка нажатия текстовых клавиш на клавиатуре
	bool SystemKeyPress(char Key,char Params);	// Обработка нажатия системных клавиш на клавиатуре
};
#endif