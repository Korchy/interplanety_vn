//---------------------------------------------------------------------------
#ifndef BoundingSphereH
#define BoundingSphereH
//---------------------------------------------------------------------------
#include "BoundingObject.h"
//---------------------------------------------------------------------------
// ����� BoundingSphere. Bounding-������ -> �����.
//---------------------------------------------------------------------------
class BoundingSphere : public BoundingObject
{
protected:      // �������� ����� � ��� ��������
        // ����������


private:        // �������� ������ �����
        // ����������
        // ����� ����������� 2 ����������� (�������� ������ Position � ��������):
        float Radius;           // ������

public:         // �������� �����
        BoundingSphere(void);   // ���������� ������ ��� ����������
        ~BoundingSphere();      // ���������� ������

        // ����������

        // �������
        bool CreateBy3DObject(void* Object3DVertexBuffer,int Object3DVertexCount, unsigned int Object3DVertexFVF); // ���������� BoundingSphere ��� ����������� 3D-�������

        bool CheckVisibility(FrustumPiramide* Frustum); // �������� �� ��������� BoundingSphere � �������� ��������� Frustum

        bool CheckIntersection(BoundingObject* BO);     // �������� �� ����������� ������� BoundingObject (BO) c ������  (����� �����, ����� �������� �������� �� ������ � �������� ��� BoundingObject)
        bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd);        // �������� �� ����������� BoundingSphere � ������ �������� ����� �������
        bool CheckIntersectionWithSphere(D3DXVECTOR3 const &Position1, float Radius1);  // �������� �� ����������� BoundingSphere � BoundingSphere
        bool CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX, D3DXVECTOR3 const &OBBVecY, D3DXVECTOR3 const &OBBVecZ, D3DXVECTOR3 const &PositionOBB, float OBBSizeX, float OBBSizeY, float OBBSizeZ);       // �������� �� ����������� BoundingSphere � OBB

        bool UpdateTransform(D3DXMATRIX const &TransformMatrix, float SX, float SY, float SZ);  // ��������� ������� ������������� � BoundingObject, SX,SY,SZ - ������������ ��������������� ��� ��������� ��������

};
//---------------------------------------------------------------------------
#endif
