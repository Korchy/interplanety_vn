//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Object3D.h"
//---------------------------------------------------------------------------
Object3D::Object3D(void)
{
	// ����������� ������
	// 3D-������
Mesh = NULL;
SizeOfVertex = 0;

VertexBuffer = NULL;
WorkVertexBuffer = NULL;
VertexCount = 0;

IndexBuffer = NULL;
WorkIndexBuffer = NULL;
IndexCount = 0;

Materials = NULL;
MaterialsCount = 0;
Textures = NULL;

Position = D3DXVECTOR3(0.0f,0.0f,0.0f);
VecX = D3DXVECTOR3(1.0f,0.0f,0.0f);
VecY = D3DXVECTOR3(0.0f,1.0f,0.0f);
VecZ = D3DXVECTOR3(0.0f,0.0f,-1.0f);
	// ������� ������������� ������� �� ������ "���������" (��� ���������)
D3DXMatrixIdentity(&TransformMatrix);
	// ��������� �� �������� ���������
pFrustum = NULL;
}
//---------------------------------------------------------------------------
Object3D::~Object3D(void)
{
	// ���������� ������
	// ������ �����
if(VertexBuffer!=NULL) VertexBuffer->Release();
if(WorkVertexBuffer!=NULL) delete [] WorkVertexBuffer;
	// ������ ��������
if(IndexBuffer!=NULL) IndexBuffer->Release();
if(WorkIndexBuffer!=NULL) delete [] WorkIndexBuffer;
	// ����� ����������
if(Materials!=NULL) delete [] Materials;
	// ����� �������
if(Textures!=NULL) {
	for(DWORD i=0; i<MaterialsCount; i++ ) {
		if(Textures[i]!=NULL) Textures[i]->Release();        
	}
}
delete [] Textures;
	// 3D-������
if(Mesh!=NULL) Mesh->Release();
}
//---------------------------------------------------------------------------
//                     ������� ����� ��� ���� ��������
//---------------------------------------------------------------------------
bool Object3D::LoadFromFile(std::string File)
{
	// �������� ������� �� �����
if(Common::FileExists((BasePath+File).c_str())==false) File = AlternativeFile;
LPD3DXBUFFER MatBuffer;	// ��������� ����� ��� ���������� ���������� �������
D3DXLoadMeshFromX((LPSTR)(BasePath+File).c_str(),D3DXMESH_SYSTEMMEM,pD3DDevice,NULL,&MatBuffer,&MaterialsCount,&Mesh);
	// ����������� ������������ ������� (��� ��������� ��� ��������� �� �����)
Mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT,NULL,NULL,NULL,NULL);	// ������ �������� ���������, ��� ������������ Mesh ��� ������� ��� ������ ����� DrawSubset
	// ���������� ������ ����� ����� ��� ������������ �������
if(Mesh->GetFVF()&D3DFVF_XYZ) {
	SizeOfVertex += sizeof(float)*3;
	VertexType = vsType1;
	VertexFVF = D3DFVF_XYZ;    
}
if(Mesh->GetFVF()&D3DFVF_NORMAL) {
	SizeOfVertex += sizeof(float)*3;
	VertexType = vsType3;
	VertexFVF = D3DFVF_XYZ|D3DFVF_NORMAL;
}
if(Mesh->GetFVF()&D3DFVF_TEX1) {
	SizeOfVertex += sizeof(float)*2;
	if(VertexType==vsType3) {
		VertexType = vsType2;
		VertexFVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
	}
	else {
		VertexType = vsType4;
		VertexFVF = D3DFVF_XYZ|D3DFVF_TEX1;
	}
}
	// ���������� ����� ����� � ����� �������� �����
Mesh->GetVertexBuffer(&VertexBuffer);
Mesh->GetIndexBuffer(&IndexBuffer);
VertexCount = Mesh->GetNumVertices();	// ���-�� �����
IndexCount = Mesh->GetNumFaces()*3;		// ���-�� �������� = ���-�� ��������� * 3
	// ������� ����� ������� �������� � ����� � ��������� ������ ��� ��������� ������ � ���� (����� ������ ���������� �� ����-������ � ��������� � �������)
	// ������� ����� ������ ����� ��� ������ � ���
switch (VertexType) {
	case(vsType1):
		WorkVertexBuffer = new VertexType1[VertexCount];
		break;
	case(vsType2):
		WorkVertexBuffer = new VertexType2[VertexCount];
		break;
	case(vsType3):
		WorkVertexBuffer = new VertexType3[VertexCount];
		break;
	case(vsType4):
		WorkVertexBuffer = new VertexType4[VertexCount];
		break;
}
void* WorkVertexBufferTmp;	// ��������� �� ����� ����� � �����-������, ����� ��� �������������� ��� � ����� ����� � ��������� ������
VertexBuffer->Lock(0,0,(BYTE**)&WorkVertexBufferTmp,0);
memcpy(WorkVertexBuffer,WorkVertexBufferTmp,VertexCount*SizeOfVertex);
VertexBuffer->Unlock();
	// ������� ����� ������ �������� ��� ������ � ��� (������ ������� ������� ����������� 3 ���������, �������� ����������� ��������������� � ������ ������� � �������)
WorkIndexBuffer = new unsigned short[IndexCount];
void* WorkIndexBufferTmp;	// ��������� �� ����� �������� � �����-������, ����� ��� �������������� ��� � ����� �������� � ��������� ������
IndexBuffer->Lock(0,0,(BYTE**)&WorkIndexBufferTmp,0);
memcpy(WorkIndexBuffer,WorkIndexBufferTmp,IndexCount*sizeof(unsigned short));
IndexBuffer->Unlock();
	// ���������� �������� � ��������
D3DXMATERIAL* TmpMaterials = (D3DXMATERIAL*)MatBuffer->GetBufferPointer();	// ��������� �� ��������� ������������ �������
Materials = new D3DMATERIAL8[MaterialsCount];
Textures = new LPDIRECT3DTEXTURE8[MaterialsCount];
for(DWORD i=0; i<MaterialsCount; i++) {
	Materials[i] = TmpMaterials[i].MatD3D;
	Materials[i].Ambient = Materials[i].Diffuse;
	if(FAILED(D3DXCreateTextureFromFile(pD3DDevice,(Common::GetPath(BasePath+File)+std::string(TmpMaterials[i].pTextureFilename)).c_str(),&Textures[i]))) Textures[i] = NULL;
}
MatBuffer->Release();
return true;
}
//---------------------------------------------------------------------------
//                       �������� ��������� �������
//---------------------------------------------------------------------------
bool Object3D::CheckVisibility()
{
	// �������� ��������� ������� �.�. ��������� ��������-�� ������ �
	// �������� ��������� Frustum ��� ���
	// ���� pFrustum = NULL - ������� ��� ������ ������ �������� � ��������
	// ���������
if(pFrustum==NULL) {
	Visible = true;
	return true;
}
	// ��������� ������ ����� ������� �� ��������� � 6 ���������� �������� ���������
D3DXPLANE* Planes = pFrustum->GetPlanes();	// ��������� �� ������ � ����������� �������� ���������
for(unsigned int i=0; i<VertexCount; i++) {
	// ������� �����
	D3DXVECTOR3 v;
	// �������� void* � ���� ������� ����� (����� ����������� �������� ������)
	switch(VertexType) {
		case(vsType1):
			VertexType1* Tmp1;
			Tmp1 = (VertexType1*)WorkVertexBuffer;
			v = Tmp1[i].Position;
			break;
		case(vsType2):
			VertexType2* Tmp2;
			Tmp2 = (VertexType2*)WorkVertexBuffer;
			v = Tmp2[i].Position;
			break;
		case(vsType3):
			VertexType3* Tmp3;
			Tmp3 = (VertexType3*)WorkVertexBuffer;
			v = Tmp3[i].Position;
			break;
		case(vsType4):
			VertexType4* Tmp4;
			Tmp4 = (VertexType4*)WorkVertexBuffer;
			v = Tmp4[i].Position;
			break;	
	}
	// ��������, ��������-�� ����� � 6 ���������� �������� ���������
	bool TmpRez = true;
	for(int j=0; j<=5; j++) {
		if(Planes[j].a*v.x+Planes[j].b*v.y+Planes[j].c*v.z+Planes[j].d<0.0) TmpRez = false;            
	}
	// ���� �������� - ����� ���� ������
	if(TmpRez==true) {
		Visible = true;
		return true;
	}    
}
Visible = false;
return true;
}
//---------------------------------------------------------------------------
//                           �������� ������ �������
//---------------------------------------------------------------------------
bool Object3D::CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance)
{
	// �������� �� ����������� ������� ��������� ������� LineStart � LineEnd
	// ���� �� � ����� ��������� �������, ���������� true ���� �����������
	// ����, false - ���, Distance - �������� �������� = ���������� �� �����
	// �����������
bool Rez = false;	// true - ����������� ����, false - ���
	// �������� ������ ����������� �������
D3DXVECTOR3 LineDirection;
LineDirection = LineEnd - LineStart;
	// ��������� �� ����������� ����������� ������� � ������� ��������
D3DXIntersect(Mesh,&LineStart,&LineDirection,(int*)&Rez,NULL,NULL,NULL,Distance,NULL,NULL);
if(Rez==false) *Distance = -1.0;
return Rez;
}
//---------------------------------------------------------------------------
//                         ��������� ������� �� �����
//---------------------------------------------------------------------------
bool Object3D::Draw()
{
	// ����� 3D-������� � �����
	// ���� ������ �� ����� - ��� �� ������
if(Visible==false) return true;
	// ������������ Mesh ��� ������
pD3DDevice->SetVertexShader(VertexFVF);
pD3DDevice->SetStreamSource(0,VertexBuffer,SizeOfVertex);
pD3DDevice->SetIndices(IndexBuffer,0);
for(DWORD i=0; i<MaterialsCount; i++) {
	pD3DDevice->SetMaterial(&Materials[i]);
	pD3DDevice->SetTexture(0,Textures[i]);
	Mesh->DrawSubset(i);
}
return true;
}
//---------------------------------------------------------------------------
//              ���������� ������������ � ���������������� �������
//---------------------------------------------------------------------------
bool Object3D::MoveTo(float X, float Y, float Z)
{
	// ����������� ������� ����� ���������� ���� X,Y,Z �� ���������������
	// ��������
D3DXMATRIX Move;
D3DXMatrixIdentity(&Move);
D3DXMatrixTranslation(&Move,X,Y,Z);
TransformMatrix *= Move;
return true;
}
//---------------------------------------------------------------------------
bool Object3D::MoveInto(float X, float Y, float Z)
{
	// ����������� ������� � ����� � ������������ X,Y,Z � ���������� �������
	// ���������
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&D3DXVECTOR3(X,Y,Z),&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::MoveX(float X)
{
	// ����������� ������� ����� ��� ��������� ��� X �� ������ ����������
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecX,&Position);
MoveVector *= X;
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::MoveY(float Y)
{
	// ����������� ������� ����� ��� ��������� ��� Y �� ������ ����������
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecY,&Position);
MoveVector *= Y;
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::MoveZ(float Z)
{
	// ����������� ������� ����� ��� ��������� ��� Z �� ������ ����������
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecZ,&Position);
MoveVector *= Z;
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::RotateX(float Angel)
{
	// ������� ������� ������ ������� Vec� �� ���� Angle. ����� ������
	// ������������� ������ ����� ���, � �� ������ ��������� - ������������
	// � 3 �����: ������� ������� �������� � ����� ���������, �������
	// ������� ��������, ������� ������� �������� ������� � ������� �������
	// ��������� � 0,0,0
D3DXVECTOR3 Vec000;	// ������ ����������� � 0,0,0 � ������� ������� ���������
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector;	// Vec000 ��������������� � ������� ��������� �������
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
	// ������������ ������
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationX(&Rot,Angel*M_PI/180);	// �� Angel �������� (180 ��. = PI ���.)
TransformMatrix *= Rot;
	// ������� ������ �������
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::RotateY(float Angel)
{
	// ������� ������� ������ ������� VecY �� ���� Angle. ����� ������
	// ������������� ������ ����� ���, � �� ������ ��������� - ������������
	// � 3 �����: ������� ������� �������� � ����� ���������, �������
	// ������� ��������, ������� ������� �������� ������� � ������� �������
	// ��������� � 0,0,0
D3DXVECTOR3 Vec000;	// ������ ����������� � 0,0,0 � ������� ������� ���������
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector;	// Vec000 ��������������� � ������� ��������� �������
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
	// ������������ ������
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationY(&Rot,Angel*M_PI/180);	// �� Angel �������� (180 ��. = PI ���.)
TransformMatrix *= Rot;
	// ������� ������ �������
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::RotateZ(float Angel)
{
	// ������� ������� ������ ������� VecZ �� ���� Angle. ����� ������
	// ������������� ������ ����� ���, � �� ������ ��������� - ������������
	// � 3 �����: ������� ������� �������� � ����� ���������, �������
	// ������� ��������, ������� ������� �������� ������� � ������� �������
	// ��������� � 0,0,0
D3DXVECTOR3 Vec000;	// ������ ����������� � 0,0,0 � ������� ������� ���������
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector;	// Vec000 ��������������� � ������� ��������� �������
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
	// ������������ ������
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationZ(&Rot,Angel*M_PI/180);	// �� Angel �������� (180 ��. = PI ���.)
TransformMatrix *= Rot;
	// ������� ������ �������
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::Scale(float SX, float SY, float SZ)
{
	// ��������������� ������� �� ���� X,Y,Z �������������,
	// SX,SY,SY ��������� �� ������� ��� �������������� ������
D3DXMATRIX Scale;
D3DXMatrixIdentity(&Scale);
D3DXMatrixScaling(&Scale,SX,SY,SZ);
TransformMatrix *= Scale;
return true;
}
//---------------------------------------------------------------------------
bool Object3D::UpdateTransform()
{
	// �������� ������ �.�. ��������� � ���� ���������� �������������
if(pD3DDevice==NULL) return false;
	// �������� �������, ����������� ������ � ������������ � ���������� �������� �������������
D3DXVec3TransformCoord(&Position,&Position,&TransformMatrix);
D3DXVec3TransformCoord(&VecX,&VecX,&TransformMatrix);
D3DXVec3TransformCoord(&VecY,&VecY,&TransformMatrix);
D3DXVec3TransformCoord(&VecZ,&VecZ,&TransformMatrix);
	// �������� VertexBuffer ������� � ������������ � ���������� �������� �������������
	// ��������� ������� ������������� � WorkVertexBuffer
for(unsigned int i=0; i<VertexCount; i++) {
	// �.�. ������� ����� ������ - �������������� ������� �� �������
	switch(VertexType) {
		case(vsType1):
			VertexType1* Tmp1;	// �������� void* � ���� ������� ����� (����� ����������� �������� ������)
			Tmp1 = (VertexType1*)WorkVertexBuffer;
			D3DXVec3TransformCoord(&Tmp1[i].Position,&Tmp1[i].Position,&TransformMatrix);
			break;
		case(vsType2):
			VertexType2* Tmp2;	// �������� void* � ���� ������� ����� (����� ����������� �������� ������)
			Tmp2 = (VertexType2*)WorkVertexBuffer;
			D3DXVec3TransformCoord(&Tmp2[i].Position,&Tmp2[i].Position,&TransformMatrix);
			D3DXVec3TransformNormal(&Tmp2[i].Normal,&Tmp2[i].Normal,&TransformMatrix);
			D3DXVec3Normalize(&Tmp2[i].Normal,&Tmp2[i].Normal);
			break;
		case(vsType3):
			VertexType3* Tmp3;	// �������� void* � ���� ������� ����� (����� ����������� �������� ������)
			Tmp3 = (VertexType3*)WorkVertexBuffer;
			D3DXVec3TransformCoord(&Tmp3[i].Position,&Tmp3[i].Position,&TransformMatrix);
			D3DXVec3TransformNormal(&Tmp3[i].Normal,&Tmp3[i].Normal,&TransformMatrix);
			D3DXVec3Normalize(&Tmp3[i].Normal,&Tmp3[i].Normal);
			break;
		case(vsType4):
			VertexType4* Tmp4;	// �������� void* � ���� ������� ����� (����� ����������� �������� ������)
			Tmp4 = (VertexType4*)WorkVertexBuffer;
			D3DXVec3TransformCoord(&Tmp4[i].Position,&Tmp4[i].Position,&TransformMatrix);
			break;
	}
}
	// �������� ����������� WorkVertexBuffer � VertexBuffer
void* WorkVertexBufferTmp;	// ��������� ����� �����
VertexBuffer->Lock(0,0,(BYTE**)&WorkVertexBufferTmp,0);
memcpy(WorkVertexBufferTmp,WorkVertexBuffer,VertexCount*SizeOfVertex);
VertexBuffer->Unlock();
	// �������� ������� �������������
D3DXMatrixIdentity(&TransformMatrix);
	// ��������� ������ �� ��������� � �������� ���������
CheckVisibility();
return true;
}
//---------------------------------------------------------------------------