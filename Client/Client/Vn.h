#pragma once
//---------------------------------------------------------------------------
#ifndef VnH
#define VnH
//---------------------------------------------------------------------------
// ����� Vn - �������� ����� ����������
//---------------------------------------------------------------------------
#include "Common.h"		// ���. �������
#include "LogFile.h"	// ��� ������� ���-�����
#include "d3d8.h"		// ��� ������������� DirectX 8.1
#include "d3dx8math.h"	// ��� ������������� D3DXMATRIX
#include "mmsystem.h"	// ��� ������������� �-��� timeGetTime()
#include "FpsCounter.h"	// ������� FPS
#include "Camera.h"		// ������
#include "Scene.h"		// �����
#include "Console.h"	// ������� � �������
#include "EditBox.h"	// EditBox
//---------------------------------------------------------------------------
class Vn
{
private:	// ������ ��� ����� ������
	// ����������
	HWND hWnd;						// ������������� ���� � ������� ����������� ����������
	LPDIRECT3D8 pD3D;				// ������� ������ Direct3d
	LPDIRECT3DDEVICE8 pD3DDevice;	// ���������� ����������
	D3DPRESENT_PARAMETERS ParamStruc;	// ��������� � �����������, ������������ ��� �������� ���������� ����������

	D3DXMATRIX MatWorld;			// ������� ����� ("������� ����")
	D3DXMATRIX MatView;				// ������� ���� (� ����� ����� ����������� ������ � ���� ��� ����������)
	D3DXMATRIX MatProj;				// ������� �������� (������������� ����� �� ��������� ������)
	
	D3DLIGHT8 Light1;				// 1 �������� ������������ ��������� (������ ������ ���� �����)
	D3DLIGHT8 Light2;				// 2 �������� ������������ ��������� (����� ������� ���� �����)

	bool PerspectiveProj;			// ��� �������������: true - �������������, false - �������������

	DWORD STime;					 // ��������� � �������� ������� ������� ��� ������� ��������� � 40 �� (25 ����/���)
	DWORD ETime;

	Camera* Cam;					// ������

	FpsCounter* Fps;				// ������� FPS

	Scene* Scn;						// �����

	Console* Cons;					// �������
	EditBox* EditBox1;				// EditBox


	// �������
	bool InitDirect3D();			// ������������� Direct3D
	bool SetDeviceParams();			// ��������� �������������� ���������� ����������
	bool DeleteDirect3D();			// �������� Direct3D

protected:	// ��� ����� ������ � ��������

public:		// �������� ����
	Vn(void);		// ����������� ������ ��� ����������
	Vn(HWND hw);	// ����������� � �����������
	~Vn(void);		// ���������� ������
	
	// ����������
	LogFile* Log;					// ��������� �� ������ Log-����
	
	bool Windowed;					// false - ������������� �����, true - ������� �����
	int Width;						// ������ ���������� ����� ���� (� �������������� BackBuffer'�)
	int Height;						// ������ ���������� ����� ���� (� �������������� BackBuffer'�)

	unsigned short BackBufferCount;	// ���������� BackBuffer'�� ��� ���������� ����������
	unsigned int Ambient;			// �������� ����������� ��������� (�����)
	unsigned int FonColor;			// ���� ����
	unsigned short MultisamplingLevel;	// ������� ����������� (�� 2 �� 16, 0 - ���)

	// �������
	bool Init();		// ������������� 3D
	bool Destroy();		// ���������� ������
	bool Resize(int NewW,int NewH);		// ����������������� 3D ��� ��������� �������� ����
	void Interation();	// ��������� ����� ��������� �������� �����
	bool Update();		// ���������� �����
	bool Render();		// ����� ����� �� �����
	void ShowFPS();		// ���������� FPS
	void KeyPress(char Key,char Params);	// ��������� ������� ��������� ������ �� ����������
	void SystemKeyPress(char Key,char Params);	// ��������� ������� ��������� ������ �� ����������

};
#endif