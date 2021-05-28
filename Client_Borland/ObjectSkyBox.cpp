//---------------------------------------------------------------------------
#pragma hdrstop

#include "ObjectSkyBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
ObjectSkyBox::ObjectSkyBox(void)
{
        // Конструктор класса
SkyBox = NULL;
R = 4.9;        // т.к модель - Box со сторонами = 10 (10/2 и 0.1 запас)
}
//---------------------------------------------------------------------------
ObjectSkyBox::~ObjectSkyBox()
{
        // Деструктор класса
        // Небесная сфера
if(SkyBox!=NULL) delete SkyBox;
}
//---------------------------------------------------------------------------
//                      РИСОВАНИЕ SkyBox НА ЭКРАН
//---------------------------------------------------------------------------
bool ObjectSkyBox::Draw()
{
        // Вывод объектов сектора в сцену
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
//                          ПЕРЕМЕЩЕНИЕ SkyBox
//---------------------------------------------------------------------------
bool ObjectSkyBox::MoveInto(float X, float Y, float Z)
{
        // Перемещение SkyBox в точку с координатами X,Y,Z в глобальной системе
        // координат
SkyBox->MoveInto(X,Y,Z);
return true;
}
//---------------------------------------------------------------------------
bool ObjectSkyBox::UpdateTransform()
{
        // Обновить SkyBox т.е. применить к нему полученные трансформации
SkyBox->UpdateTransform();
return true;
}
//---------------------------------------------------------------------------
//                              ОБЩИЕ ФУНКЦИИ
//---------------------------------------------------------------------------
bool ObjectSkyBox::LoadFromFile(AnsiString File)
{
        // Загрузка данных по сектору из файла
SkyBox = new Object3DSkyBox();
SkyBox->SetRenderDevice(pD3DDevice);
SkyBox->LoadFromFile(File);
SkyBox->UpdateTransform();
return true;
}
//---------------------------------------------------------------------------

