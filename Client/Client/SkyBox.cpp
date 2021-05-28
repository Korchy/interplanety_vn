//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "SkyBox.h"
//---------------------------------------------------------------------------
SkyBox::SkyBox(void)
{
	// ����������� ������
R = (float)4.9;	// ���������� �.� ������ - Box �� ��������� = 10 (10/2 � 0.1 �����)
AlternativeFile = "s0\\SkyBox.x";
}
//---------------------------------------------------------------------------
SkyBox::~SkyBox(void)
{
	// ���������� ������

}
//---------------------------------------------------------------------------
//                      ��������� SkyBox �� �����
//---------------------------------------------------------------------------
bool SkyBox::Draw()
{
	// ����� SkyBox � �����
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
//                              ����� �������
//---------------------------------------------------------------------------
bool SkyBox::LoadFromFile(std::string File)
{
	// �������� SkyBox �� �����
Object3D::LoadFromFile(File);
return true;
}
//---------------------------------------------------------------------------