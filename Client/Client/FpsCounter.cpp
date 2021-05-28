//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"		// Для правильной линковки
#include "FpsCounter.h"
//---------------------------------------------------------------------------
FpsCounter::FpsCounter(void)
{
	// Конструктор класса
Fps = 0;
FramesDiff = 0.0;
ShowFps = false;
FpsShower = NULL;
Frames = 0;
        // Получить частоту высокоточного счетчика
QueryPerformanceFrequency(&QPFrequency);
QueryPerformanceCounter(&QPCounter);
QPCounterLast.QuadPart = QPCounter.QuadPart;
QPCounterLast1.QuadPart = QPCounter.QuadPart;
}
//---------------------------------------------------------------------------
FpsCounter::~FpsCounter(void)
{
	// Деструктор класса
if(FpsShower!=NULL) delete FpsShower;
}
//---------------------------------------------------------------------------
bool FpsCounter::NextFrame()
{
	// Подсчет нового значения FPS
Frames++;
BOOL Rez = QueryPerformanceCounter(&QPCounter);
if(Rez==false) {
	// Если высокоточный счетчик не поддерживается аппаратно
	Fps = 0;
	FramesDiff = 0.0;
	Frames = 0;
	return true;	
}
	// Время отработки предыдущей интерации цикла (в мс.)
FramesDiff = (((double)QPCounter.QuadPart-(double)QPCounterLast1.QuadPart)/(double)QPFrequency.QuadPart)*1000.0;
QPCounterLast1.QuadPart = QPCounter.QuadPart;
	// FPS за 1 секунду
double Sek = ((double)QPCounter.QuadPart-(double)QPCounterLast.QuadPart)/(double)QPFrequency.QuadPart;
if(Sek>=1.0) {
	Fps = (unsigned short)(Frames/Sek);
	Frames = 0;
	QPCounterLast.QuadPart = QPCounter.QuadPart;
}
return true;
}
//---------------------------------------------------------------------------
bool FpsCounter::SetFpsShow(LPDIRECT3DDEVICE8 Device,int Left,int Top)
{
        // Инициализация вывода значения FPS на экран
if(FpsShower==NULL) {
	FpsShower = new Text(Device);
	FpsShower->SetTextFont(12,FW_NORMAL,false,false,"Arial");
	FpsShower->SetRect(Left,Top,0,0);
	FpsShower->SetColor(D3DCOLOR_XRGB(0xFF,0x00,0x00));
	// Выводим на экран
	ShowFps = true;
}
return true;
}
//---------------------------------------------------------------------------
bool FpsCounter::Draw()
{
	// Вывод значения FPS в сцену
	// Если указан вывод на экран
if(ShowFps==true) {
	FpsShower->Txt = "FPS: " + Common::IntToStr(Fps);
	FpsShower->Draw();
}
return true;
}
//---------------------------------------------------------------------------
bool FpsCounter::Reset()
{
	// Сбросить объект текста, который выводит FPS на экран (при
	// переинициализации устройства рендеринга)
FpsShower->Reset();
return true;
}
//---------------------------------------------------------------------------
bool FpsCounter::Restore()
{
	// Восстановить объект текста, который выводит FPS на экран после
	// сброса (при переинициализации устройства рендеринга)
FpsShower->Restore();
return true;
}
//---------------------------------------------------------------------------
unsigned short FpsCounter::GetFps()
{
	// Возвращает текущее значение FPS
return Fps;
}
//---------------------------------------------------------------------------
double FpsCounter::GetFramesDiff()
{
	// Возвращает время, за которое была обработана предыдущая интерация игрового цикла
return FramesDiff;
}
//---------------------------------------------------------------------------
float FpsCounter::GetCorrectionValue()
{
	// Возвращает текущее значение коэффициента коррекции скорости
return (float)(1.0/FramesDiff);
}
//---------------------------------------------------------------------------