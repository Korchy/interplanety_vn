#pragma once
//---------------------------------------------------------------------------
#ifndef VnH
#define VnH
//---------------------------------------------------------------------------
// Класс Vn - основной класс приложения
//---------------------------------------------------------------------------
#include "Common.h"		// Доп. функции
#include "LogFile.h"	// Для ведения Лог-файла
#include "d3d8.h"		// Для использования DirectX 8.1
#include "d3dx8math.h"	// Для использования D3DXMATRIX
#include "mmsystem.h"	// Для использования ф-ции timeGetTime()
#include "FpsCounter.h"	// Счетчик FPS
#include "Camera.h"		// Камера
#include "Scene.h"		// Сцена
#include "Console.h"	// Консоль с текстом
#include "EditBox.h"	// EditBox
//---------------------------------------------------------------------------
class Vn
{
private:	// Только для этого класса
	// Переменные
	HWND hWnd;						// Идентификатор окна в котором выполняется приложение
	LPDIRECT3D8 pD3D;				// Главный объект Direct3d
	LPDIRECT3DDEVICE8 pD3DDevice;	// Устройство рендеринга
	D3DPRESENT_PARAMETERS ParamStruc;	// Структура с параметрами, необходимыми для создания устройства рендеринга

	D3DXMATRIX MatWorld;			// Матрица сцены ("матрица Мира")
	D3DXMATRIX MatView;				// Матрица вида (в какой точке расположена камера и куда она направлена)
	D3DXMATRIX MatProj;				// Матрица проекции (проецирование сцены на плоскость экрана)
	
	D3DLIGHT8 Light1;				// 1 источник равномерного освещения (правый нижний угол сцены)
	D3DLIGHT8 Light2;				// 2 источник равномерного освещения (левый верхний угол сцены)

	bool PerspectiveProj;			// Тип проецирования: true - перспективное, false - ортогональное

	DWORD STime;					 // Начальный и конечные моменты времени для отсчета интервала в 40 мс (25 кадр/сек)
	DWORD ETime;

	Camera* Cam;					// Камера

	FpsCounter* Fps;				// Счетчик FPS

	Scene* Scn;						// Сцена

	Console* Cons;					// Консоль
	EditBox* EditBox1;				// EditBox


	// Функции
	bool InitDirect3D();			// Инициализация Direct3D
	bool SetDeviceParams();			// Установка дополнительных параметров рендеринга
	bool DeleteDirect3D();			// Удаление Direct3D

protected:	// Для этого класса и потомков

public:		// Доступно всем
	Vn(void);		// Конструктор класса без параметров
	Vn(HWND hw);	// Конструктор с параметрами
	~Vn(void);		// Деструктор класса
	
	// Переменные
	LogFile* Log;					// Указатель на объект Log-файл
	
	bool Windowed;					// false - полноэкранный режим, true - оконный режим
	int Width;						// Ширина клиентской части окна (и соответственно BackBuffer'а)
	int Height;						// Высота клиентской части окна (и соответственно BackBuffer'а)

	unsigned short BackBufferCount;	// Количество BackBuffer'ов для устройства рендеринга
	unsigned int Ambient;			// Величина рассеянного освещения (гамма)
	unsigned int FonColor;			// Цвет фона
	unsigned short MultisamplingLevel;	// Уровень сглаживания (от 2 до 16, 0 - нет)

	// Функции
	bool Init();		// Инициализация 3D
	bool Destroy();		// Завершение работы
	bool Resize(int NewW,int NewH);		// Переинициализация 3D при изменении размеров окна
	void Interation();	// Отработка одной интерации игрового цикла
	bool Update();		// Обновление сцены
	bool Render();		// Вывод сцены на экран
	void ShowFPS();		// Показывать FPS
	void KeyPress(char Key,char Params);	// Обработка нажатия текстовых клавиш на клавиатуре
	void SystemKeyPress(char Key,char Params);	// Обработка нажатия системных клавиш на клавиатуре

};
#endif