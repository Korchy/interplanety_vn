//---------------------------------------------------------------------------
#pragma hdrstop

#include "ObjectSkyBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
ObjectSkyBox::ObjectSkyBox(void)
{
        // ����������� ������
SkyBox = NULL;
R = 4.9;        // �.� ������ - Box �� ��������� = 10 (10/2 � 0.1 �����)
}
//---------------------------------------------------------------------------
ObjectSkyBox::~ObjectSkyBox()
{
        // ���������� ������
        // �������� �����
if(SkyBox!=NULL) delete SkyBox;
}
//---------------------------------------------------------------------------
//                      ��������� SkyBox �� �����
//---------------------------------------------------------------------------
bool ObjectSkyBox::Draw()
{
        // ����� �������� ������� � �����
if(SkyBox==NULL) return false;

pD3DDevice->SetRenderState(D3DRS_ZENABLE,false);
pD3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
pD3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);
SkyBox->Draw();
pD3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_POINT);
pD3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_POINT);
pD3DDevice->SetRenderState(D3DRS_ZENABLE,true);
return true;
}
//---------------------------------------------------------------------------
//                          ����������� SkyBox
//---------------------------------------------------------------------------
bool ObjectSkyBox::MoveInto(float X, float Y, float Z)
{
        // ����������� SkyBox � ����� � ������������ X,Y,Z � ���������� �������
        // ���������
SkyBox->MoveInto(X,Y,Z);
return true;
}
//---------------------------------------------------------------------------
bool ObjectSkyBox::UpdateTransform()
{
        // �������� SkyBox �.�. ��������� � ���� ���������� �������������
SkyBox->UpdateTransform();
return true;
}
//---------------------------------------------------------------------------
//                              ����� �������
//---------------------------------------------------------------------------
bool ObjectSkyBox::LoadFromFile(AnsiString File)
{
        // �������� ������ �� ������� �� �����
SkyBox = new Object3DSkyBox();
SkyBox->SetRenderDevice(pD3DDevice);
SkyBox->LoadFromFile(File);
SkyBox->UpdateTransform();
return true;
}
//---------------------------------------------------------------------------

