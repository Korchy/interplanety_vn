//---------------------------------------------------------------------------
#pragma hdrstop

#include "Camera.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
Camera::Camera(void)
{
        // ����������� ������ ��� ����������
pD3DDevice = NULL;
        // ��������� ������ � ��������� ���������
DefaultPosition();
        // ������� ������������� �� ������ "���������" (��� ���������)
D3DXMatrixIdentity(&TransformMatrix);
        // �������� �������� ������
ScrollSpeed = 1.0;      // ��������� 0.1 ��. �� 1 ��. 
        // �������� ���������
Frustum = NULL;
        // ��������� �� SkySphere (���)
SkyBox = NULL;
}
//---------------------------------------------------------------------------
Camera::~Camera()
{
        // ���������� ������
if(Frustum!=NULL) delete Frustum;       // ������� �������� ���������
}
//---------------------------------------------------------------------------
//                              ����� �������
//---------------------------------------------------------------------------
bool Camera::SetRenderDevice(LPDIRECT3DDEVICE8 Device)
{
        // ����������� ���������� ����������
if(Device!=NULL) {
        pD3DDevice = Device;
        Frustum = new FrustumPiramide();        // ������� �������� ���������
        return true;
        }
else {
        return false;
        }
}
//---------------------------------------------------------------------------
bool Camera::SetSkyBox(ObjectSkyBox* SkyBoxObject)
{
        // ���������� ������� SkySphere (���)
SkyBox = SkyBoxObject;
if(SkyBox!=NULL) {      // ��������� ����� SkySphere � Position ������
//        SkyBox->MoveInto(Position.x,Position.y,Position.z);
        D3DXVECTOR3 MoveSkyBoxVec;
//        MoveSkyBoxVec = VecZ - Position;
//        MoveSkyBoxVec = Position+D3DXVECTOR3(0.0f,0.0f,1.0f)*SkyBox->R;
        MoveSkyBoxVec = Position+(VecZ - Position)*SkyBox->R;
        SkyBox->MoveInto(MoveSkyBoxVec.x,MoveSkyBoxVec.y,MoveSkyBoxVec.z);
        SkyBox->UpdateTransform();
        }
return true;
}
//---------------------------------------------------------------------------
void Camera::SetScrollSpeed(float NewSpeed)
{
        // ��������� �������� �������������� ��� ������
        // ����� ����� ������������� ������ 1/FPS
ScrollSpeed = NewSpeed;
}
//---------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetPosition()
{
        // ���������� ������� ��������� ������
return Position;
}
//---------------------------------------------------------------------------
bool Camera::ScrollLeft()
{
        // ����������� ������ ����� ��� � ������
        // ������� ��������� ������, ����������� ����������� �� ������� ����� � ������ �����
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecX,&Position);
MoveVector *= -ScrollSpeed;
        // ����������� ������ � ����� ����� �������� ����������� �������
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
//MoveTo(-ScrollSpeed,0,0);        // � (���������) ������� ��������� ������
return true;
}
//---------------------------------------------------------------------------
bool Camera::ScrollRight()
{
        // ����������� ������ ����� ��� � �������
        // ������� ��������� ������, ����������� ����������� �� ������� ����� � ������ �����
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecX,&Position);
MoveVector *= ScrollSpeed;
        // ����������� ������ � ����� ����� �������� ����������� �������
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
//MoveTo(ScrollSpeed,0,0);        // � (���������) ������� ��������� ������
return true;
}
//---------------------------------------------------------------------------
bool Camera::ScrollUp()
{
        // ����������� ������ ����� ��� Y �����
        // ������� ��������� ������, ����������� ����������� �� ������� ����� � ������ �����
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecY,&Position);
MoveVector *= ScrollSpeed;
        // ����������� ������ � ����� ����� �������� ����������� �������
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
//MoveTo(1,0,0);        // � (���������) ������� ��������� ������
return true;
}
//---------------------------------------------------------------------------
bool Camera::ScrollDown()
{
        // ����������� ������ ����� ��� Y ����
        // ������� ��������� ������, ����������� ����������� �� ������� ����� � ������ �����
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecY,&Position);
MoveVector *= -ScrollSpeed;
        // ����������� ������ � ����� ����� �������� ����������� �������
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
//MoveTo(1,0,0);        // � (���������) ������� ��������� ������
return true;
}
//---------------------------------------------------------------------------
bool Camera::Scale(bool Increase)
{
        // ����������� ������ ����� ��� Z
        // ������� ��������� ������, ����������� ����������� �� ������� ����� � ������ �����
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecZ,&Position);
if(Increase==true) MoveVector *= ScrollSpeed;
else MoveVector *= -ScrollSpeed;
        // ����������� ������ � ����� ����� �������� ����������� �������
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
//MoveTo(1,0,0);        // � (���������) ������� ��������� ������
return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateRight()
{
        // ������� ������ ������ ��� Y �������. ����� ��� �������������� ������
        // ����� ���, � �� ������ ��������� - ������������ � 3 �����: �������
        // ������� �������� � ����� ���������, ������� ������� ��������,
        // ������� ������� �������� ������� � ������� �������
        // ��������� � 0,0,0
D3DXVECTOR3 Vec000;     // ������ ����������� � 0,0,0 � ������� ������� ���������
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector; // Vec000 ��������������� � ������� ��������� ������
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
        // ������������ ������
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationY(&Rot,M_PI/180);      // �� 1 ������ (180 ��. = PI ���.)
TransformMatrix *= Rot;
        // ������� ������ �������
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateLeft()
{
        // ������� ������ ������ ��� Y ������. ����� ��� �������������� ������
        // ����� ���, � �� ������ ��������� - ������������ � 3 �����: �������
        // ������� �������� � ����� ���������, ������� ������� ��������,
        // ������� ������� �������� ������� � ������� �������
        // ��������� � 0,0,0
D3DXVECTOR3 Vec000;     // ������ ����������� � 0,0,0 � ������� ������� ���������
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector; // Vec000 ��������������� � ������� ��������� ������
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
        // ������������ ������
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationY(&Rot,-M_PI/180);    // �� 1 ������ (180 ��. = PI ���.)
TransformMatrix *= Rot;
        // ������� ������ �������
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateUp()
{
        // ������� ������ ������ ��� � �����. ����� ��� �������������� ������
        // ����� ���, � �� ������ ��������� - ������������ � 3 �����: �������
        // ������� �������� � ����� ���������, ������� ������� ��������,
        // ������� ������� �������� ������� � ������� �������
        // ��������� � 0,0,0
D3DXVECTOR3 Vec000;     // ������ ����������� � 0,0,0 � ������� ������� ���������
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector; // Vec000 ��������������� � ������� ��������� ������
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
        // ������������ ������
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationX(&Rot,-M_PI/180);    // �� 1 ������ (180 ��. = PI ���.)
TransformMatrix *= Rot;
        // ������� ������ �������
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateDown()
{
        // ������� ������ ������ ��� � ����. ����� ��� �������������� ������
        // ����� ���, � �� ������ ��������� - ������������ � 3 �����: �������
        // ������� �������� � ����� ���������, ������� ������� ��������,
        // ������� ������� �������� ������� � ������� �������
        // ��������� � 0,0,0
D3DXVECTOR3 Vec000;     // ������ ����������� � 0,0,0 � ������� ������� ���������
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector; // Vec000 ��������������� � ������� ��������� ������
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
        // ������������ ������
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationX(&Rot,M_PI/180);     // �� 1 ������ (180 ��. = PI ���.)
TransformMatrix *= Rot;
        // ������� ������ �������
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateRightOrtho()
{
        // ������� ������ ������ ��� Y ������� ��� ������������� ��������
        // �� ������ ����� ������ � ��������� ���������� ������ �����, �����
        // ������� �� �������� ���������� ������

        // ��� ������������� ���� �������� ��� ������ ������

return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateLeftOrtho()
{
        // ������� ������ ������ ��� Y ������ ��� ������������� ��������
        // �� ������ ����� ������ � ��������� ���������� ������ �����, �����
        // ������� �� �������� ���������� ������

return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateUpOrtho()
{
        // ������� ������ ������ ��� � ����� ��� ������������� ��������
        // �� ������ ����� ������ � ��������� ���������� ������ �����, �����
        // ������� �� �������� ���������� ������

return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateDownOrtho()
{
        // ������� ������ ������ ��� � ���� ��� ������������� ��������
        // �� ������ ����� ������ � ��������� ���������� ������ �����, �����
        // ������� �� �������� ���������� ������

return true;
}
//---------------------------------------------------------------------------
bool Camera::UpdateWorld()
{
if(pD3DDevice==NULL) return false;
        // ����������� ������� ������ � �������
        // �������� �������, ����������� ������ � ������������ � ���������� �������� �������������
D3DXVec3TransformCoord(&Position,&Position,&TransformMatrix);
D3DXVec3TransformCoord(&Target,&Target,&TransformMatrix);
D3DXVec3TransformNormal(&Up,&Up,&TransformMatrix);
D3DXVec3TransformCoord(&VecX,&VecX,&TransformMatrix);
D3DXVec3TransformCoord(&VecY,&VecY,&TransformMatrix);
D3DXVec3TransformCoord(&VecZ,&VecZ,&TransformMatrix);
        // �������� ������� �������������
D3DXMatrixIdentity(&TransformMatrix);
        // �������� � NewPosition ����� �������� ���� ��������, �������� ����� ��������� ������
D3DXMATRIX NewPosition;
D3DXMatrixLookAtLH(&NewPosition,&Position,&Target,&Up);
        // ��������� NewPosition � ������� ������
pD3DDevice->SetTransform(D3DTS_VIEW,&NewPosition);
        // �������� �������� ��������� ��� ������
D3DXMATRIX ProjMatrix;  // �������� ������������ �������
pD3DDevice->GetTransform(D3DTS_PROJECTION,(D3DMATRIX*)&ProjMatrix);
Frustum->Update(ProjMatrix,NewPosition);        // ������� ������� = NewPosition
        // �������� ��������� SkyBox
if(SkyBox!=NULL) {
//        SkyBox->MoveInto(Position.x,Position.y,Position.z);
        D3DXVECTOR3 MoveSkyBoxVec;
//        MoveSkyBoxVec = Position+D3DXVECTOR3(0.0f,0.0f,1.0f)*SkyBox->R;
        MoveSkyBoxVec = Position+(VecZ - Position)*SkyBox->R;
        SkyBox->MoveInto(MoveSkyBoxVec.x,MoveSkyBoxVec.y,MoveSkyBoxVec.z);
        SkyBox->UpdateTransform();
        }
return true;
}
//---------------------------------------------------------------------------
bool Camera::DefaultPosition()
{
        // ������� ������ � ��������� ���������
        // ��������� ��������� ������
Position = D3DXVECTOR3(0.0f,0.0f,-100.0f);
Target = D3DXVECTOR3(0.0f,0.0f,0.0f);
Up = D3DXVECTOR3(0.0f,1.0f,0.0f);
VecX = D3DXVECTOR3(1.0f,0.0f,-100.0f);
VecY = D3DXVECTOR3(0.0f,1.0f,-100.0f);
VecZ = D3DXVECTOR3(0.0f,0.0f,-99.0f);
return true;
}
//---------------------------------------------------------------------------
//                            ���������� �������
//---------------------------------------------------------------------------
bool Camera::MoveTo(float X, float Y, float Z)
{
        // ����������� ������ � ����� � ������������ X,Y,Z � ����������
        // ������� ���������
D3DXMATRIX Move;
D3DXMatrixIdentity(&Move);
D3DXMatrixTranslation(&Move,X,Y,Z);
TransformMatrix *= Move;
return true;
}
//---------------------------------------------------------------------------

