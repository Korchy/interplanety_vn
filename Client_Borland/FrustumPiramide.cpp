//---------------------------------------------------------------------------
#pragma hdrstop

#include "FrustumPiramide.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
FrustumPiramide::FrustumPiramide(void)
{
        // ����������� ������

}
//---------------------------------------------------------------------------
FrustumPiramide::~FrustumPiramide()
{
        // ���������� ������

}
//---------------------------------------------------------------------------
//                              ����� �������
//---------------------------------------------------------------------------
bool FrustumPiramide::Update(D3DXMATRIX const &ProjMatrix, D3DXMATRIX const &ViewMatrix)
{
        // ����������� �������� ��������� � ������������ � ������ ������������ �
        // ������� ���������
        // �������� ������� � ���� �������� �� ������� � ����������� ������
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
        // �� �������� ������� ��������� �������� ���������
D3DXPlaneFromPoints(&Plane[0],&Vec[0],&Vec[1],&Vec[2]); // �������
D3DXPlaneFromPoints(&Plane[1],&Vec[6],&Vec[7],&Vec[5]); // �������
D3DXPlaneFromPoints(&Plane[2],&Vec[2],&Vec[6],&Vec[4]); // �����
D3DXPlaneFromPoints(&Plane[3],&Vec[7],&Vec[3],&Vec[5]); // ������
D3DXPlaneFromPoints(&Plane[4],&Vec[2],&Vec[3],&Vec[6]); // �������
D3DXPlaneFromPoints(&Plane[5],&Vec[1],&Vec[0],&Vec[4]); // ������
return true;
}
//---------------------------------------------------------------------------
//            �������� �������� �� ��������� � �������� ���������
//---------------------------------------------------------------------------
D3DXPLANE* FrustumPiramide::GetPlanes()
{
        // ���������� ��������� �� ������ � ����������� �������� ���������
return Plane;
}
//---------------------------------------------------------------------------
D3DXVECTOR3* FrustumPiramide::GetVecs()
{
        // ���������� ��������� �� ������ � ��������� �������� ���������
return Vec;
}
//---------------------------------------------------------------------------
