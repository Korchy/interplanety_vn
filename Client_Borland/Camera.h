//---------------------------------------------------------------------------
#ifndef CameraH
#define CameraH
//---------------------------------------------------------------------------
#include <Classes.hpp>  // ��� ������������� ����������� �++ - ����� ������ (���� AnsiString)
#include "d3dx8.h"      // ����������� ���������� D3D8X ��� ������������� �� �������
#include "FrustumPiramide.h"    // �������� ���������
#include "ObjectSkyBox.h"       // ��� ���������� �������� SkyBox
//---------------------------------------------------------------------------
// ����� Camera. ��������� ������ ��������� ������ � �������� ������� ������
// � ����� ���������. ������� ������ ��������� ��� ����� ����������� ������ �
// ������������, ������� ��������� � ����������� �������. � ���� ����������
// ������� ������ �����. ��� ���������� �������� ��������� ����������� ������ �
// ������� ������ ����� ���������� � ����.
//---------------------------------------------------------------------------
class Camera
{

protected:
        // �������� ������ ��������
        // ����������
        LPDIRECT3DDEVICE8 pD3DDevice;   // ���������� ���������� (��� ������������ ������� ������ � ������)

private:
        // �������� ������ ����� ������
        // ����������
        // ������ �������� 6 ���������: 3 ����������� �������, ����������� ��������� ������
        D3DXVECTOR3 Position;   // ���������
	D3DXVECTOR3 Target;     // ����������� �������
	D3DXVECTOR3 Up;         // ������� ������ (����������)
        // � 3 ���������� ���������, ��������������� ����������� ��������������
        D3DXVECTOR3 VecX;       // ����� ��� �
        D3DXVECTOR3 VecY;       // ����� ��� Y
        D3DXVECTOR3 VecZ;       // ����� ��� Z

        float ScrollSpeed;      // �������� �������� ������ (��./��.) ���������� ��� FPS=1000 (1 ���� �������������� �� 1 ��.) ScrollSpeed = 0.1, ������������� ��� ������������ �������� ����� ������ ���� ������� ScrollSpeed = 0.1 * ����� ��������� �����

        D3DXMATRIX TransformMatrix;     // ������� �������������, �������� (���������) � ���� ��� ��������� ��������� ������ �� ���������� UpdateWorld

        ObjectSkyBox* SkyBox;   // ��������� �� SkyBox

        // �������
        bool MoveTo(float X, float Y, float Z); // ����������� ������ � ����� � ������������ X,Y,Z
        bool UpdateFrustum();   // ����������� ��������� �������� ��������� ��� ������

public:
        // �������� ����
        Camera(void);   // ���������� ������ ��� ����������
        ~Camera();      // ���������� ������

        // ����������
        FrustumPiramide* Frustum;       // �������� ��������� ��� ������

        // �������
        bool SetRenderDevice(LPDIRECT3DDEVICE8 Device); // ���������� ���������� ����������
        bool SetSkyBox(ObjectSkyBox* SkyBoxObject);     // ���������� ������� SkySphere (���)
        void SetScrollSpeed(float NewSpeed);    // ��������� �������� ����������

        D3DXVECTOR3 GetPosition();      // ���������� ������� ��������� ������
        bool ScrollRight();             // ����������� ������ ����� ��� � ������
        bool ScrollLeft();              // ����������� ������ ����� ��� � �����
        bool ScrollUp();                // ����������� ������ ����� ��� Y �����
        bool ScrollDown();              // ����������� ������ ����� ��� Y ����
        bool Scale(bool Increase);      // ����������� ������ ����� ��� Z, Increace==true - ��������� �������, false - ��������� �������
        bool RotateRight();             // ������� ������ ������ ��� Y �������
        bool RotateLeft();              // ������� ������ ������ ��� Y ������
        bool RotateUp();                // ������� ������ ������ ��� � �����
        bool RotateDown();              // ������� ������ ������ ��� � ����
        bool RotateRightOrtho();        // ������� ������ ������ ��� Y ������� � ������������� ��������
        bool RotateLeftOrtho();         // ������� ������ ������ ��� Y ������ � ������������� ��������
        bool RotateUpOrtho();           // ������� ������ ������ ��� � ����� � ������������� ��������
        bool RotateDownOrtho();         // ������� ������ ������ ��� � ���� � ������������� ��������
        bool UpdateWorld();             // ����������� ������� ������ � �������
        bool DefaultPosition();         // ��������� ������ � ��������� ���������

};
//---------------------------------------------------------------------------
#endif
