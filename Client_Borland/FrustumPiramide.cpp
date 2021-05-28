//---------------------------------------------------------------------------
#pragma hdrstop

#include "FrustumPiramide.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
FrustumPiramide::FrustumPiramide(void)
{
        // Конструктор класса

}
//---------------------------------------------------------------------------
FrustumPiramide::~FrustumPiramide()
{
        // Деструктор класса

}
//---------------------------------------------------------------------------
//                              ОБЩИЕ ФУНКЦИИ
//---------------------------------------------------------------------------
bool FrustumPiramide::Update(D3DXMATRIX const &ProjMatrix, D3DXMATRIX const &ViewMatrix)
{
        // Пересчитать пирамиду видимости в соответствии с новыми Проекционной и
        // Видовой матрицами
        // Получить вектора в углы пирамиды из Видовой и Прекционной матриц
D3DXMATRIX PVMatrix;
D3DXMatrixMultiply(&PVMatrix,&ViewMatrix,&ProjMatrix);
D3DXMatrixInverse(&PVMatrix,NULL,&PVMatrix);
Vec[0] = D3DXVECTOR3(-1.0f,-1.0f,0.0f); // xyz
Vec[1] = D3DXVECTOR3(1.0f,-1.0f,0.0f);  // Xyz
Vec[2] = D3DXVECTOR3(-1.0f,1.0f,0.0f);  // xYz
Vec[3] = D3DXVECTOR3(1.0f,1.0f,0.0f);   // XYz
Vec[4] = D3DXVECTOR3(-1.0f,-1.0f,1.0f); // xyZ
Vec[5] = D3DXVECTOR3(1.0f,-1.0f,1.0f);  // XyZ
Vec[6] = D3DXVECTOR3(-1.0f,1.0f,1.0f);  // xYZ
Vec[7] = D3DXVECTOR3(1.0f,1.0f,1.0f);   // XYZ
for(int i=0; i<8; i++) {
        D3DXVec3TransformCoord(&Vec[i],&Vec[i],&PVMatrix);
        }
        // Из векторов получим плоскости пирамиды видимости
D3DXPlaneFromPoints(&Plane[0],&Vec[0],&Vec[1],&Vec[2]); // Ближняя
D3DXPlaneFromPoints(&Plane[1],&Vec[6],&Vec[7],&Vec[5]); // Дальняя
D3DXPlaneFromPoints(&Plane[2],&Vec[2],&Vec[6],&Vec[4]); // Левая
D3DXPlaneFromPoints(&Plane[3],&Vec[7],&Vec[3],&Vec[5]); // Правая
D3DXPlaneFromPoints(&Plane[4],&Vec[2],&Vec[3],&Vec[6]); // Верхняя
D3DXPlaneFromPoints(&Plane[5],&Vec[1],&Vec[0],&Vec[4]); // Нижняя
return true;
}
//---------------------------------------------------------------------------
//            ПРОВЕРКА ОБЪЕКТОВ НА ПОПАДАНИЕ В ПИРАМИДУ ВИДИМОСТИ
//---------------------------------------------------------------------------
D3DXPLANE* FrustumPiramide::GetPlanes()
{
        // Возвращает указатель на массив с плоскостями пирамиды видимости
return Plane;
}
//---------------------------------------------------------------------------
D3DXVECTOR3* FrustumPiramide::GetVecs()
{
        // Возвращает указатель на массив с векторами пирамиды видимости
return Vec;
}
//---------------------------------------------------------------------------
