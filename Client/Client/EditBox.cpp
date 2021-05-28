//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "EditBox.h"
//---------------------------------------------------------------------------
EditBox::EditBox(void)
{
	// ����������� ������
Visible = false;
EditShower = NULL;
Value = "";
Width = 0;
Height = 0;
CursorPos = 0;
FirstVisiblePos = 0;
FontStyle = "Courier New";	// ����� ���������� Courier New
FontSize = 14;				// ����� ������ 14
}
//---------------------------------------------------------------------------
EditBox::~EditBox(void)
{
	// ��������� ������
	// ������� ������ Text
if(EditShower!=NULL) delete EditShower;
}
//---------------------------------------------------------------------------
bool EditBox::SetRenderDevice(LPDIRECT3DDEVICE8 Device)
{
	// ���������� ���������� ����������
bool Rez = Object::SetRenderDevice(Device);
if(Rez==true) {
	EditShower = new Text(Device);
	EditShower->SetTextFont(FontSize,FW_NORMAL,false,false,FontStyle);
	EditShower->SetRect(5,20,55,35);	// �� ��������� ������� ����� � ������� 5,20x55,35
	Width = 50;
	Height = 15;
	EditShower->SetColor(D3DCOLOR_XRGB(0xFF,0x00,0x00));
	EditShower->SetFormat(DT_LEFT|DT_SINGLELINE);	// ������������ �� ������ ����,�������� ����� �� ���������
	EditShower->Txt = Value;
	return true;
}
else return false;
}
//---------------------------------------------------------------------------
bool EditBox::SetRect(int Left,int Top,int Right,int Bottom)
{
	// ��������� ������� ������ ������
bool Rez = false;
if(EditShower!=NULL) {
	Width = Right-Left;
	Height = Bottom-Top;
	Rez = EditShower->SetRect(Left,Top,Right,Bottom);
}
return Rez;
}
//---------------------------------------------------------------------------
bool EditBox::Draw()
{
	// ����� ����������� EditBox �� �����
if(Visible==true) {
	if(EditShower!=NULL) EditShower->Draw();
}
return true;
}
//---------------------------------------------------------------------------
bool EditBox::Reset()
{
	// �������� ������ (����� ��� ��������� �������� ��� ������������ ���������� ����������)
if(EditShower!=NULL) EditShower->Reset();
return true;
}
//---------------------------------------------------------------------------
bool EditBox::Restore()
{
	// �������������� ������� ����� ����, ��� �� ��� �������
if(EditShower!=NULL) EditShower->Restore();
return true;
}
//---------------------------------------------------------------------------
bool EditBox::KeyPress(char Key,char Params)
{
	// ��������� ������� ��������� ������ �� ����������
char TmpKey[2];
TmpKey[0] = (char)Key;
TmpKey[1] = 0;
	// ��������� ��������� ������
switch(TmpKey[0]) {
	case VK_ESCAPE:	// ������� Esc
		break;
	case VK_RETURN:	// ������� Enter
		break;
	case VK_BACK:	// ������� BackSpace
		if(CursorPos!=0) {
			Value.erase(CursorPos-1,1);
			CursorPos--;
			// ��������, ���� �����, ������� �����
			SIZE TextSize;
			GetTextExtentPoint32(GetDC(NULL),(Value.substr(FirstVisiblePos,CursorPos-FirstVisiblePos)).c_str(),(int)(Value.substr(FirstVisiblePos,CursorPos-FirstVisiblePos)).length(),&TextSize);
			if(TextSize.cx<=Width/2&&FirstVisiblePos!=0) {
				FirstVisiblePos--;
				// �������� EditShower
				EditShower->Txt = Value.substr(FirstVisiblePos);
			}
		}
		break;
	// ��� ��������� �������
	default:
		// �������� ������ � �����
		Value.insert(CursorPos,std::string(TmpKey));
		CursorPos++;
		// ��������, ���� �����, ������� �����
		SIZE TextSize;
		GetTextExtentPoint32(GetDC(NULL),(Value.substr(FirstVisiblePos,CursorPos-FirstVisiblePos)).c_str(),(int)(Value.substr(FirstVisiblePos,CursorPos-FirstVisiblePos)).length(),&TextSize);
		if(Width<=TextSize.cx) {
			FirstVisiblePos++;
			// �������� EditShower
			EditShower->Txt = Value.substr(FirstVisiblePos);
		}
}
return true;
}
//---------------------------------------------------------------------------
bool EditBox::SystemKeyPress(char Key,char Params)
{
	// ��������� ������� ��������� ������ �� ����������
SIZE TextSize;
switch(Key) {
	case VK_RIGHT:
		if(CursorPos<Value.size()) {
			CursorPos++;
			GetTextExtentPoint32(GetDC(NULL),(Value.substr(FirstVisiblePos,CursorPos-FirstVisiblePos)).c_str(),(int)(Value.substr(FirstVisiblePos,CursorPos-FirstVisiblePos)).length(),&TextSize);
			if(Width<=TextSize.cx) {
				FirstVisiblePos++;
				// �������� EditShower
				EditShower->Txt = Value.substr(FirstVisiblePos);
			}
		}
		break;
	case VK_LEFT:
		if(CursorPos!=0) {
			CursorPos--;
			GetTextExtentPoint32(GetDC(NULL),(Value.substr(FirstVisiblePos,CursorPos-FirstVisiblePos)).c_str(),(int)(Value.substr(FirstVisiblePos,CursorPos-FirstVisiblePos)).length(),&TextSize);
			if(TextSize.cx==0&&FirstVisiblePos!=0) {
				FirstVisiblePos--;
				// �������� EditShower
				EditShower->Txt = Value.substr(FirstVisiblePos);
			}
		}
		break;
}
//MessageBox(NULL,(Common::IntToStr(TextSize.cx)).c_str(),"",MB_OK);
return true;
}
//---------------------------------------------------------------------------