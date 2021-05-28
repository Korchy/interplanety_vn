//---------------------------------------------------------------------------
#pragma hdrstop

#include "Object.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
Object::Object(void)
{
        // ����������� ������ ��� ����������
        // ������������� ����������
pD3DDevice = NULL;
Visible = false;
Checkable = false;
BasePath = GetCurrentDir()+"\\Data\\";
}
//---------------------------------------------------------------------------
Object::~Object()
{
        // ���������� ������
//TStringList* t = new TStringList();
//t->Add("Object");
//t->SaveToFile("END.txt");
//delete t;
}
//---------------------------------------------------------------------------
//                      �������, ����� ��� ���� ��������
//---------------------------------------------------------------------------
bool Object::SetRenderDevice(LPDIRECT3DDEVICE8 Device)
{
        // ���������� ���������� ����������
if(Device!=NULL) {
        pD3DDevice = Device;
        return true;
        }
else {
        return false;
        }
}
//---------------------------------------------------------------------------
bool Object::SetFrustum(FrustumPiramide* Frustum)
{
        // ��������� ��������� �� �������� ���������. ���� pFrustum �������
        // ����� NULL - �������, ��� �������� ��������� = ���� �����
pFrustum = Frustum;
return true;
}
//---------------------------------------------------------------------------
//        ����������� ������� (���������������� � �������-�����������)
//---------------------------------------------------------------------------
bool Object::Reset()
{
        // �������� ������ (����� ��� ��������� �������� ��� ������������
        // ���������� ����������)
return true;
}
//---------------------------------------------------------------------------
bool Object::Restore()
{
        // �������������� �������, ���� �� �� �����-���� �������� ��� ���������
return true;
}
//---------------------------------------------------------------------------
bool Object::UpdateAnimation()
{
        // ����������� �������� �������
return true;
}
//---------------------------------------------------------------------------
