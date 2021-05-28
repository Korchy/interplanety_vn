//---------------------------------------------------------------------------
#ifndef BoundingOBBH
#define BoundingOBBH
//---------------------------------------------------------------------------
#include "BoundingObject.h"
//---------------------------------------------------------------------------
// ����� BoundingOBB. Bounding-������ -> OBB
//---------------------------------------------------------------------------
class BoundingOBB : public BoundingObject
{
protected:      // �������� ����� � ��� ��������
        // ����������


private:        // �������� ������ �����
        // ����������
        // OBB ����������� 7 ����������� (�������� ������ Position, 3 ��������� �� Position � ������� ���� ������ OBB � 3 ������� ���� ��������):
        D3DXVECTOR3 OBBVecX;    // 3 ������� ������������ � ������ ���� ������ OBB
        D3DXVECTOR3 OBBVecY;
        D3DXVECTOR3 OBBVecZ;
        float OBBSizeX;         // 3 ����� ��������������� ��������
        float OBBSizeY;
        float OBBSizeZ;

public:         // �������� �����
        BoundingOBB(void);      // ���������� ������ ��� ����������
        ~BoundingOBB();         // ���������� ������

        // ����������

        // �������
        bool CreateBy3DObject(void* Object3DVertexBuffer,int Object3DVertexCount, unsigned int Object3DVertexFVF); // ���������� BoundingOBB �� ����������� 3D-�������

        bool CheckVisibility(FrustumPiramide* Frustum); // �������� �� ��������� BoundingObject � �������� ��������� Frustum

        bool CheckIntersection(BoundingObject* BO);     // �������� �� ����������� ������� BoundingObject (BO) c ������  (����� �����, ����� �������� �������� �� ������ � �������� ��� BoundingObject)        
        bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd);        // �������� �� ����������� BoundingOBB � ������ �������� ����� �������
        bool CheckIntersectionWithSphere(D3DXVECTOR3 const &PositionS, float RadiusS);  // �������� �� ����������� BoundingOBB � BoundingSphere
        bool CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX1, D3DXVECTOR3 const &OBBVecY1, D3DXVECTOR3 const &OBBVecZ1, D3DXVECTOR3 const &PositionOBB1, float OBBSizeX1, float OBBSizeY1, float OBBSizeZ1);       // �������� �� ����������� BoundingOBB � BoundingOBB

        bool UpdateTransform(D3DXMATRIX const &TransformMatrix, float SX, float SY, float SZ);  // ��������� ������� ������������� � BoundingObject, SX,SY,SZ - ������������ ��������������� ��� ��������� ��������

};
//---------------------------------------------------------------------------
#endif
