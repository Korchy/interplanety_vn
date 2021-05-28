//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "SkyBox.h"
//---------------------------------------------------------------------------
SkyBox::SkyBox(void)
{
	// Конструктор класса
R = (float)4.9;	// Постоянный т.к модель - Box со сторонами = 10 (10/2 и 0.1 запас)
AlternativeFile = "s0\\SkyBox.x";
}
//---------------------------------------------------------------------------
SkyBox::~SkyBox(void)
{
	// Деструктор класса

}
//---------------------------------------------------------------------------
//                      РИСОВАНИЕ SkyBox НА ЭКРАН
//---------------------------------------------------------------------------
bool SkyBox::Draw()
{
	// Вывод SkyBox в сцену
pD3DDevice->SetRenderState(D3DRS_ZENABLE,false);
pD3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
pD3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);
Object3D::Draw();
pD3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_POINT);
pD3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_POINT);
pD3DDevice->SetRenderState(D3DRS_ZENABLE,true);
return true;
}
//---------------------------------------------------------------------------
//                              ОБЩИЕ ФУНКЦИИ
//---------------------------------------------------------------------------
bool SkyBox::LoadFromFile(std::string File)
{
	// Загрузка SkyBox из файла
Object3D::LoadFromFile(File);
return true;
}
//---------------------------------------------------------------------------