//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"		// ��� ���������� ��������
#include "FpsCounter.h"
//---------------------------------------------------------------------------
FpsCounter::FpsCounter(void)
{
	// ����������� ������
Fps = 0;
FramesDiff = 0.0;
ShowFps = false;
FpsShower = NULL;
Frames = 0;
        // �������� ������� ������������� ��������
QueryPerformanceFrequency(&QPFrequency);
QueryPerformanceCounter(&QPCounter);
QPCounterLast.QuadPart = QPCounter.QuadPart;
QPCounterLast1.QuadPart = QPCounter.QuadPart;
}
//---------------------------------------------------------------------------
FpsCounter::~FpsCounter(void)
{
	// ���������� ������
if(FpsShower!=NULL) delete FpsShower;
}
//---------------------------------------------------------------------------
bool FpsCounter::NextFrame()
{
	// ������� ������ �������� FPS
Frames++;
BOOL Rez = QueryPerformanceCounter(&QPCounter);
if(Rez==false) {
	// ���� ������������ ������� �� �������������� ���������
	Fps = 0;
	FramesDiff = 0.0;
	Frames = 0;
	return true;	
}
	// ����� ��������� ���������� ��������� ����� (� ��.)
FramesDiff = (((double)QPCounter.QuadPart-(double)QPCounterLast1.QuadPart)/(double)QPFrequency.QuadPart)*1000.0;
QPCounterLast1.QuadPart = QPCounter.QuadPart;
	// FPS �� 1 �������
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
        // ������������� ������ �������� FPS �� �����
if(FpsShower==NULL) {
	FpsShower = new Text(Device);
	FpsShower->SetTextFont(12,FW_NORMAL,false,false,"Arial");
	FpsShower->SetRect(Left,Top,0,0);
	FpsShower->SetColor(D3DCOLOR_XRGB(0xFF,0x00,0x00));
	// ������� �� �����
	ShowFps = true;
}
return true;
}
//---------------------------------------------------------------------------
bool FpsCounter::Draw()
{
	// ����� �������� FPS � �����
	// ���� ������ ����� �� �����
if(ShowFps==true) {
	FpsShower->Txt = "FPS: " + Common::IntToStr(Fps);
	FpsShower->Draw();
}
return true;
}
//---------------------------------------------------------------------------
bool FpsCounter::Reset()
{
	// �������� ������ ������, ������� ������� FPS �� ����� (���
	// ����������������� ���������� ����������)
FpsShower->Reset();
return true;
}
//---------------------------------------------------------------------------
bool FpsCounter::Restore()
{
	// ������������ ������ ������, ������� ������� FPS �� ����� �����
	// ������ (��� ����������������� ���������� ����������)
FpsShower->Restore();
return true;
}
//---------------------------------------------------------------------------
unsigned short FpsCounter::GetFps()
{
	// ���������� ������� �������� FPS
return Fps;
}
//---------------------------------------------------------------------------
double FpsCounter::GetFramesDiff()
{
	// ���������� �����, �� ������� ���� ���������� ���������� ��������� �������� �����
return FramesDiff;
}
//---------------------------------------------------------------------------
float FpsCounter::GetCorrectionValue()
{
	// ���������� ������� �������� ������������ ��������� ��������
return (float)(1.0/FramesDiff);
}
//---------------------------------------------------------------------------