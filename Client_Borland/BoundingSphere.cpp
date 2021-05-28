//---------------------------------------------------------------------------
#pragma hdrstop

#include "BoundingSphere.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
BoundingSphere::BoundingSphere(void)
{
        // ����������� ������ ��� ����������
        // ������������� ����������
Radius = 0.0;
//MessageBox(NULL,"BOUNDING SPHERE","",NULL);
}
//---------------------------------------------------------------------------
BoundingSphere::~BoundingSphere()
{
        // ���������� ������

}
//---------------------------------------------------------------------------
//                                ����� �������
//---------------------------------------------------------------------------
bool BoundingSphere::CreateBy3DObject(void* Object3DVertexBuffer,int Object3DMeshVertexCount, unsigned int Object3DVertexFVF)
{
        // ��������� ��������� BoundingSphere ��� ����������� �������
D3DXComputeBoundingSphere(Object3DVertexBuffer,Object3DMeshVertexCount,Object3DVertexFVF,&Position,&Radius);
//MessageBox(NULL,(FloatToStr(Radius)).c_str(),"",NULL);
return true;
}
//---------------------------------------------------------------------------
//          �������� ��������� BoundibngSphere � �������� ���������
//---------------------------------------------------------------------------
bool BoundingSphere::CheckVisibility(FrustumPiramide* Frustum)
{
        // �������� �� ��������� BoundingSphere � �������� ��������� Frustum
        // ��������� �� ������ � ����������� �������� ���������
D3DXPLANE* Planes = Frustum->GetPlanes();
        // ��� ������ ��������� ��������� ��������� Position ����� � �������� ��������� � ������ ������� �����
for(int j=0; j<=5; j++) {
        if(Planes[j].a*Position.x+Planes[j].b*Position.y+Planes[j].c*Position.z+Planes[j].d<(-Radius)) return false;
        }
return true;
}
//---------------------------------------------------------------------------
//                             COLLISION DETECTION
//---------------------------------------------------------------------------
bool BoundingSphere::CheckIntersection(BoundingObject* BO)
{
        // �������� �� ����������� ������� BoundingObject (BO) c ������  (�����
        // �����, ����� �������� �������� �� ������ � �������� ��� BoundingObject)
bool Rez = BO->CheckIntersectionWithSphere(Position,Radius);
return Rez;
}
//---------------------------------------------------------------------------
bool BoundingSphere::CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd)
{
        // �������� �� ����������� BoundingSphere � ������ �������� ����� �������
D3DXVECTOR3 LineDirection = LineEnd - LineStart;
        // �������� ������ ����� ������� ��������������� ��������� ����� � �����
float a = LineDirection.x*LineDirection.x + LineDirection.y*LineDirection.y + LineDirection.z*LineDirection.z;
float b = 2.0f*(LineDirection.x*(LineStart.x-Position.x)+LineDirection.y*(LineStart.y-Position.y)+LineDirection.z*(LineStart.z-Position.z));
float c = (LineStart.x-Position.x)*(LineStart.x-Position.x)+(LineStart.y-Position.y)*(LineStart.y-Position.y)+(LineStart.z-Position.z)*(LineStart.z-Position.z)-Radius*Radius;
float d = b*b-4.0f*a*c;
if(d<0) return false;   // ��� �����������
else return true;       // ���� �����������
}
//---------------------------------------------------------------------------
bool BoundingSphere::CheckIntersectionWithSphere(D3DXVECTOR3 const &Position1, float Radius1)
{
        // �������� �� ����������� BoundingSphere � ������ BoundingSphere
        // ��������� ����� ���������� ����� �������� ���� ���� >= ����� �� ��������
        // ���������� ����� �������� ����
float PositionDist = sqrt((Position1.x-Position.x)*(Position1.x-Position.x)+(Position1.y-Position.y)*(Position1.y-Position.y)+(Position1.z-Position.z)*(Position1.z-Position.z));
if(PositionDist>=Radius+Radius1) return false;  // ��� �����������
else return true;                               // ���� �����������
}
//---------------------------------------------------------------------------
bool BoundingSphere::CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX, D3DXVECTOR3 const &OBBVecY, D3DXVECTOR3 const &OBBVecZ, D3DXVECTOR3 const &PositionOBB, float OBBSizeX, float OBBSizeY, float OBBSizeZ)
{
        // �������� �� ����������� BoundingSphere � OBB
        // ��������� �� ��������� �� ������ ��������� ��� OBB - ����� �� �����
        // ����� ������ OBB, �� - ���� �����������, ��� - ��������� ����������
        // �� ������� OBB �� ������ ����� � ���������� � �� ��������
D3DXVECTOR3 PositionSLoc = Position - PositionOBB;      // ������ �� OBBPosition � SpherePosition
D3DXVECTOR3 LocX = OBBVecX - PositionOBB;       // ������ ��������� ��� OBB �
D3DXVec3Normalize(&LocX,&LocX);
D3DXVECTOR3 LocY = OBBVecY - PositionOBB;       // ������ ��������� ��� OBB Y
D3DXVec3Normalize(&LocY,&LocY);
D3DXVECTOR3 LocZ = OBBVecZ - PositionOBB;       // ������ ��������� ��� OBB Z
D3DXVec3Normalize(&LocZ,&LocZ);
float PositionSX = fabs(D3DXVec3Dot(&PositionSLoc,&LocX));      // ����� �������� PositionSLoc �� ��������� �������-��� OBB
float PositionSY = fabs(D3DXVec3Dot(&PositionSLoc,&LocY));
float PositionSZ = fabs(D3DXVec3Dot(&PositionSLoc,&LocZ));
float d = 0.0;
        // ���������� PositionSX, SY ,SZ � ��������� OBB � � �������� �����
if(PositionSX>OBBSizeX) d += (PositionSX - OBBSizeX)*(PositionSX - OBBSizeX);
if(PositionSY>OBBSizeY) d += (PositionSY - OBBSizeY)*(PositionSY - OBBSizeY);
if(PositionSZ>OBBSizeZ) d += (PositionSZ - OBBSizeZ)*(PositionSZ - OBBSizeZ);
if(d>(Radius*Radius)) return false;
else return true;
}
//---------------------------------------------------------------------------
//         ���������� ������������ � ���������������� BoundingSphere
//---------------------------------------------------------------------------
bool BoundingSphere::UpdateTransform(D3DXMATRIX const &TransformMatrix, float SX, float SY, float SZ)
{
        // ��������� ������� ������������� � BoundingObject, SX,SY,SZ - ������������
        // ��������������� ��� ��������� ��������
        // �������� ��������� ������
D3DXVec3TransformCoord(&Position,&Position,&TransformMatrix);
        // �������� ������� (������)
float MaxScale = SX;
if(MaxScale<SY) MaxScale = SY;
if(MaxScale<SZ) MaxScale = SZ;
Radius *= MaxScale;
return true;
}
//---------------------------------------------------------------------------
