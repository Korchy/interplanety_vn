//---------------------------------------------------------------------------
#ifndef Object3DUnitH
#define Object3DUnitH
//---------------------------------------------------------------------------
#include "Object3D.h"
#include "BoundingObject.h"     // BoundingObject
//---------------------------------------------------------------------------
// ����� Object3DUnit. ����������� ����� ��� �������� �������� ���������. ����
// ������ ������, �������, ������� � �.�.
//---------------------------------------------------------------------------
class Object3DUnit : public Object3D
{
protected:      // �������� ����� � ��� ��������
        // ����������
        BoundingObject* Bound;
        float BoundSX;  // ������������ ��� ��������������� BoundingObject
        float BoundSY;
        float BoundSZ;

private:        // �������� ������ �����
        // ����������

public:         // �������� �����
        Object3DUnit(void);     // ���������� ������ ��� ����������
        ~Object3DUnit();        // ���������� ������

        // ����������


        // �������
        bool LoadFromFile(AnsiString File);     // �������� (��������) ������� �� �����

        bool CheckVisibility(); // �������� ��������� �������

        bool Scale(float SX, float SY, float SZ);       // ��������������� ������� �� ����
        bool UpdateTransform(); // �������� ������ �.�. ��������� � ���� ���������� �������������

        bool CheckIntersections(TList* AllList, TList* IntersectionList);       // �������� �� ����������� ������� ������� �� ����� ��������� ������ AllList, ���������� ������ IntersectionList � ������������� ���������

        bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance);     // �������� �� ����������� ������� ��������� ������� LineStart � LineEnd ���� �� � ����� ��������� �������, ���������� true ���� ����������� ����, false - ���, Distance - �������� �������� = ���������� �� ����� �����������


};
//---------------------------------------------------------------------------
#endif
