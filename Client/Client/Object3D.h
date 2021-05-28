#pragma once
//---------------------------------------------------------------------------
#ifndef Object3DH
#define Object3DH
//---------------------------------------------------------------------------
// ����� Object3D. ����� ����� ��� ���� ���������� ��������. �������� � ����
// ���� 3D ������.
//---------------------------------------------------------------------------
#include "Object.h"
#include "Common.h"	// ���. �������
//---------------------------------------------------------------------------
#define M_PI (float)3.14159265358979323846
//---------------------------------------------------------------------------
class Object3D :
	public Object
{
private:	// ������ ��� ����� ������

protected:	// ��� ����� ������ � ��������
	// ����������
	LPD3DXMESH Mesh;		// ���������� 3D-������ (�����)

	LPDIRECT3DVERTEXBUFFER8 VertexBuffer;	// ����� ����� 3D-������� (��������� �� VertexBuffer � �����-������)
	void* WorkVertexBuffer;					// ����� VertexBuffer � ��������� ������ (��� ���������� ������ � ��� - ����� �� ���������� ����-����)
	short SizeOfVertex;						// ������ ������ � ������, ����������� ��� �������� ���������� �� ����� ����� 3D-�������
	unsigned int VertexCount;				// ���������� ����� �������
	struct VertexType1 {					// ��������� ����������� ���� ����� �������. ��������� ����� �.�. ����� ������� ����� ����� ������ �������
	D3DXVECTOR3 Position;    
	};
	struct VertexType2 {
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Normal;
		D3DXVECTOR2 Texture;
	};
	struct VertexType3 {
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Normal;
	};
	struct VertexType4 {
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Texture;
	};
	enum {vsType1,vsType2,vsType3,vsType4} VertexType;	// ���������� ��� �������������� ��������� ��� �������� ����� ����� �������
	unsigned int VertexFVF;		// FVF-������ �������������� ��� �������� ����� ����� �������

	LPDIRECT3DINDEXBUFFER8 IndexBuffer;	// ����� �������� ����� 3D-������� (��������� �� IndexBuffer � �����-������)
	unsigned short* WorkIndexBuffer;	// ����� IndexBuffer � ��������� ������ (��� ���������� ������ � ���)
	unsigned int IndexCount;			// ���������� �������� �������

	DWORD MaterialsCount;		// ���-�� ���������� � 3D-�������
	D3DMATERIAL8* Materials;	// ��������� �� ������ � ����������� �������

	LPDIRECT3DTEXTURE8* Textures;		// ��������� �� ������ � ���������� �������

	// 4 ������� ��� ����������� ������������ � ������������� �������:
	// 1 ������, ����������� ��������� �������
	D3DXVECTOR3 Position;	// ���������
	// � 3 ���������� ���������, ��������������� ����������� ����������� � �������� �������
	D3DXVECTOR3 VecX;		// ����� ��� �
	D3DXVECTOR3 VecY;		// ����� ��� Y
	D3DXVECTOR3 VecZ;		// ����� ��� Z

	D3DXMATRIX TransformMatrix;	// ������� �������������, �������� (���������) � ���� ��� ��������� ���������/��������/��������������� ������� �� ���������� UpdateTransform

	std::string AlternativeFile;	// �������������� ����, ������� ����������� � LoadFromFile, ���� ���������� ����� �� ����������

public:		// �������� ����
	Object3D(void);
	~Object3D(void);

	// ����������

	// �������
	bool LoadFromFile(std::string File);	// �������� ������� �� �����
	bool Draw();	// ����� ������� � �����

	bool MoveTo(float X, float Y, float Z);		// ����������� ������� �� ���������� ���� X,Y,Z �� ������ ����������
	bool MoveInto(float X, float Y, float Z);	// ����������� ������� � ����� � ������������ X,Y,Z � ���������� ������� ���������
	bool MoveX(float X);						// ����������� ������� ����� ��� ��������� ��� X �� ������ ����������
	bool MoveY(float Y);						// ����������� ������� ����� ��� ��������� ��� Y �� ������ ����������
	bool MoveZ(float Z);						// ����������� ������� ����� ��� ��������� ��� Z �� ������ ����������
	bool RotateX(float Angle);					// ������� ������� ������ ����������� ��� � �� ���� Angle
	bool RotateY(float Angle);					// ������� ������� ������ ����������� ��� Y �� ���� Angle
	bool RotateZ(float Angle);					// ������� ������� ������ ����������� ��� Z �� ���� Angle
	bool Scale(float SX, float SY, float SZ);	// ��������������� ������� �� ����
	bool UpdateTransform();						// �������� ������ �.�. ��������� � ���� ���������� �������������

	bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance);	// �������� �� ����������� ������� ��������� ������� LineStart � LineEnd ���� �� � ����� ��������� �������, ���������� true ���� ����������� ����, false - ���, Distance - �������� �������� = ���������� �� ����� �����������

	virtual bool CheckVisibility(); // �������� ��������� �������
};
#endif