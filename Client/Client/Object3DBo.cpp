//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Object3DBo.h"
//---------------------------------------------------------------------------
Object3DBo::Object3DBo(void)
{
	// ����������� ������
Bound = NULL;	// BoundingObject
BoundSX = 1.0;
BoundSY = 1.0;
BoundSZ = 1.0;	// ������������ ��������������� ��� BoundingObject
}

Object3DBo::~Object3DBo(void)
{
	// ���������� ������

}
//---------------------------------------------------------------------------
//                                  �������
//---------------------------------------------------------------------------
bool Object3DBo::LoadFromFile(std::string File)
{
        // �������� ������� �� �����
bool Rez = Object3D::LoadFromFile(File);
if(Rez==false) return Rez;
        // ������� ��� ������������ ������� BoundingObject
Rez = Bound->CreateBy3DObject(WorkVertexBuffer,VertexCount,VertexFVF);
return Rez;
}
//---------------------------------------------------------------------------
//                       �������� ��������� �������
//---------------------------------------------------------------------------
bool Object3DBo::CheckVisibility()
{
	// �������� ��������� ������� �.�. ��������� ��������-�� ������ �
	// �������� ��������� Frustum ��� ���
	// ���� pFrustum = NULL - ������� ��� ������ ������ �������� � ��������
	// ���������
if(pFrustum==NULL) {
	Visible = true;
	return true;
}
	// ������� ��������� �� BoundingObject �������
bool Rez = Bound->CheckVisibility();
if(Rez==true) {
	Visible = true;
	// ���� BoundingObject ������� ����� - �������� �� ������ ������� �.�. �� ����� ���� �� ����� (���� �� �����)
	// Object3D::CheckVisibility();
}
else Visible = false;
return true;
}
//---------------------------------------------------------------------------
//                           �������� ������ �������
//---------------------------------------------------------------------------
bool Object3DBo::CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance)
{
	// �������� �� ����������� ������� � �������� �������� ������� LineStart � LineEnd
	// ������� ��������� ����������� � ��� BoundingObject
bool BoundRez = Bound->CheckIntersectionWithLine(LineStart,LineEnd);
	// ���� ���� ����������� � BoundingObject - ��������� ����������� � ����� ��������
if(BoundRez==true) {
	return Object3D::CheckIntersectionWithLine(LineStart,LineEnd,Distance);       
}
else return BoundRez;
}
//---------------------------------------------------------------------------
//              ���������� ������������ � ���������������� �������
//---------------------------------------------------------------------------
bool Object3DBo::Scale(float SX, float SY, float SZ)
{
	// ��������������� ������� �� ���� X,Y,Z �������������,
	// SX,SY,SY ��������� �� ������� ��� �������������� ������
	// ���������� ��������������� ��� BoundingObject
BoundSX *= SX;
BoundSY *= SY;
BoundSZ *= SZ;
	// �������� ������� ����������������� ������ �������
return Object3D::Scale(SX,SY,SZ);
}
//---------------------------------------------------------------------------
bool Object3DBo::UpdateTransform()
{
	// �������� ������ �.�. ��������� � ���� ���������� �������������
if(pD3DDevice==NULL) return false;
	// ��������� ������������� � BoundingObject
Bound->UpdateTransform(TransformMatrix,BoundSX,BoundSY,BoundSZ);
BoundSX = 1.0;
BoundSY = 1.0;
BoundSZ = 1.0;
	// ��������� ������������� � ������ �������
bool Rez = Object3D::UpdateTransform();
return Rez;
}
//---------------------------------------------------------------------------
//                             COLLISION DETECTION
//---------------------------------------------------------------------------
/*
bool Object3DBo::CheckIntersections(TList* AllList, TList* IntersectionList)
{
        // �������� �� ����������� ������� ������� �� ����� ��������� ������
        // AllList. ���������� true ���� ���� ���� ���� �����������, ����
        // ������� � ������ IntersectioList ������� � �������� ���� �����������.
        // IntersectionList �.�. ������ ������� � ��������� ����� ������
        // ��������.
bool Rez = false;
        // ������-�� ������ � ������� ����� ������������ ���������� ��������
try {
        if(IntersectionList->Count>0) { // ������������ ������ ������ ���� ������
//                IntersectionList = NULL;
                return Rez;
                }
        }
catch(...) {
//        IntersectionList = NULL;
        return Rez;
        }
        // ��������� ��� �����������
for(int i=0; i<AllList->Count; i++) {
        if(dynamic_cast<Object3DUnit*>((Object*)(AllList->Items[i]))!=NULL) {
                // ���� ������ �� ������ ���������� � ���� Object3DUnit - ��������� ������������
                if((Object3DUnit*)(AllList->Items[i])==this) continue;        // "��� � �����"
                // ������������ ��������� ����� ������������ BoundingObject
                bool TmpRez = ((Object3DUnit*)(AllList->Items[i]))->Bound->CheckIntersection(Bound);
                if(TmpRez==true) {
                        IntersectionList->Add(AllList->Items[i]);
                        Rez = TmpRez;
                        }
                }
//        MessageBox(NULL,(IntToStr(IntersectionList->Count)).c_str(),"",NULL);
        }
return Rez;
}
*/
//---------------------------------------------------------------------------