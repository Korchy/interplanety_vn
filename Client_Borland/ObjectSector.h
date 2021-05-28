//---------------------------------------------------------------------------
#ifndef ObjectSectorH
#define ObjectSectorH
//---------------------------------------------------------------------------
#include "Object.h"
#include <stdio.h>              // ��� ������-������ ������
#include "Object3DStar.h"       // ������ "������"
#include "Object3DPlanet.h"     // ������ "�������"
#include "Object3DPortal.h"     // ������ "������"
#include "Object3DBase.h"       // ������ "����"
#include "ObjectSkyBox.h"       // ������ SkyBox
//---------------------------------------------------------------------------
// ����� ObjectSector. �������� � ���� ����� ������ �������. �������� � ����
// ������, ������� � �.�. � �� ��������
//---------------------------------------------------------------------------
class ObjectSector : public Object
{
protected:      // �������� ����� � ��� ��������
        // ����������

private:        // �������� ������ �����
        // ����������
        int Version;            // ������� ������ �������
        AnsiString Name;        // �������� ������� 

public:         // �������� �����
        ObjectSector(void);     // ���������� ������ ��� ����������
        ~ObjectSector();        // ���������� ������

        // ����������
        bool Created;           // true - ������ �������� (������), false - ���

        ObjectSkyBox* SkyBox;   // �������� ����� (���)
        TList* Stars;           // ������
        TList* Planets;         // �������
        TList* Portals;         // �������
        TList* Bases;           // ����


        // �������
        bool LoadFromFile(AnsiString File);             // �������� ������ �� ������� �� �����
        bool Draw();                                    // ����� �������� ������� � �����

        bool CheckVisibility();         // �������� ��������� �������

        bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance, Object* &IntersectedObject);   // �������� �� ����������� ������� ��������� ������� LineStart � LineEnd ���� �� � ����� �� �������� �������, ���������� true ���� ����������� ����, false - ���, Distance,IntersectedObject - �������� ��������� = ���������� �� ����� ����������� � ��������� �� ������ � ������� ��������� �����������

};
//---------------------------------------------------------------------------
#endif
