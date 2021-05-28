//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
//                              ������� �����
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        // ��� �������� ��������� (�����������)
        // ��������� ���������
Settings = new Options();
Settings->File = "Vn.cfg";
if(Settings->Load()==true) {
        Form1->Top = StrToInt(Settings->FindByName("FormTop"));
        Form1->Left = StrToInt(Settings->FindByName("FormLeft"));
        Form1->Width = StrToInt(Settings->FindByName("FormWidth"));
        Form1->Height = StrToInt(Settings->FindByName("FormHeight"));
        switch (StrToInt(Settings->FindByName("FormStyle"))) {
                case 0: Form1->WindowState = wsNormal;
                        break;
                case 1: Form1->WindowState = wsMaximized;
                        break;
                case 2: Form1->WindowState = wsMinimized;
                        break;
                }
        Form1->Tag = StrToInt(Settings->FindByName("FullScreen"));
        BackBufferCount = StrToInt(Settings->FindByName("BackBufferCount"));
        if(Settings->FindByName("Perspective")=="0") PerspectiveProj = false;
        else PerspectiveProj = true;
        }
else {
        ShowMessage("���������� ��������� ���������!");
        Application->Terminate();
        return;
        }
        // ������� ���-���� ��� �������
Log = new LogFile();
Log->Path = GetCurrentDir();
Log->FileName = "VN_Log";
Log->Add("--------------------------------------------------");
        // ������� ����� ��������� ������� �� �������
Form2 = new TForm2(this);
if(Form2->ShowModal()==mrCancel) {
        delete Form2;
        Application->Terminate();
        return;
        }
else delete Form2;
        // ���������� �� ���������
Ambient = 0xAAAAAAAA;   // �������� ����������� ���������
FonColor = 0x00021355;  // ���� ����
CamSpeed = 0.1;         // �������� ���������� ������ 0.1 ��. �� 1 ��.
        // ��� �����
if(Form1->Tag==0) {
        // ������� �����
        Form1->BorderStyle = bsSizeable;
        ScreenWidth = Form1->ClientWidth;
        ScreenHeight = Form1->ClientHeight;
        }
else {
        // ������������� �����
        Form1->BorderStyle = bsNone;
        ScreenWidth = 800;
        ScreenHeight = 600;
        }
        // ������������� 3D
pD3D = NULL;            // ������� ������ Direct3D
pD3DDevice = NULL;      // ���������� ���������� ������
D3DXMatrixRotationY(&MatWorld,0.0f);    // ������� ������� - ����� ����������
//D3DXMatrixLookAtLH(&MatView,&D3DXVECTOR3(0.0f,0.0f,-100.0f),&D3DXVECTOR3(0.0f,0.0f,0.0f),&D3DXVECTOR3(0.0f,1.0f,0.0f)); // ������� �� ����� 0,-100,0 � ����� 0,0,0, ������� ������ ������ 0,1,0
if(PerspectiveProj==true) D3DXMatrixPerspectiveFovLH(&MatProj,D3DX_PI/4,(float)ScreenWidth/(float)ScreenHeight,1.0f,1000.0f);      // ���� ������ PI/4, ������� ���������� � ����������� �� �-��� �����, �������� ������� ��������� 1, ������ - 1000 (��� ������ ��������� ������ �� ��������)
else D3DXMatrixOrthoLH(&MatProj,(float)ScreenWidth/4,(float)ScreenHeight/4,1.0f,1000.0f);      // ���� ������ PI/4, ������� ���������� � ����������� �� �-��� �����, �������� ������� ��������� 1, ������ - 1000 (��� ������ ��������� ������ �� ��������)
bool Rez = InitDirect3D();
if(Rez==false) {
        Log->Add("������ ������������� Direct3D");
        DeleteDirect3D();
        Application->Terminate();
        return;
        }
        // ������
Cam = new Camera();
        // ������� FPS
Fps = new FpsCounter();
        // ������ ���� �������� �����
List3D = new TList();
        // ������� �����
Sector = NULL;  // ������
        // ���������� ����� �� ��������� � �� ����������
MouseClicked = false;
ClickedX = 0;
ClickedY = 0;
MouseMoved = false;
MovedX = 0;
MovedY = 0;
        // ��������� ������� ���� (����� MESSAGE, ����� ��������� ����������� ���������� �-���, � ������ ������ �����������)
GameCicleRun = true;
PostMessage(Handle,WM_START,0,0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        // ��� ������� ������� ����� - ����� ����� ��������� �������� ��
        // �������� �������� ����� � ������� ������
GameCicleRun = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
        // ��� �������� ���������
        // ������� ��� ������� � ������ ��� ���� ��������
if(List3D!=NULL) {
        for(int i=0; i<List3D->Count; i++) {
                if(List3D->Items[i]!=NULL) delete (Object*)List3D->Items[i];
                }
        delete List3D;
        }
        // ������� ������
if(Sector!=NULL) delete Sector;
        // ������� ������
if(Cam!=NULL) delete Cam;
        // ������� ������� FPS
if(Fps!=NULL) delete Fps;
        // ������� Direct3D
bool Rez = DeleteDirect3D();
if(Rez==false) Log->Add("������ ��� �������� Direct3D");
        // ��������� ���������
if(Form1->WindowState==wsNormal&&Form1->Tag==0) {       // ��������� ������ ��� �������� ������
        Settings->SetByName("FormStyle","0");
        Settings->SetByName("FormTop",IntToStr(Form1->Top));
        Settings->SetByName("FormLeft",IntToStr(Form1->Left));
        Settings->SetByName("FormWidth",IntToStr(Form1->Width));
        Settings->SetByName("FormHeight",IntToStr(Form1->Height));
        }
if(Form1->WindowState==wsMaximized) Settings->SetByName("FormStyle","1");
if(Form1->WindowState==wsMinimized) Settings->SetByName("FormStyle","2");
Settings->SetByName("FullScreen",IntToStr(Form1->Tag));
Settings->Save();
delete Settings;        // ������� ������ � �����������
         // ������� ���-����
if(Log!=NULL) delete Log;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
        // ��� ��������� �������� ����� (������ ��� �������� ������)
if(pD3DDevice!=NULL&&Form1->Tag==0) {
        // �������� ������� ������� ������� �����
        ScreenWidth = Form1->ClientWidth;
        ScreenHeight = Form1->ClientHeight;
        // �������������������� ������� �������� � ������ �-���� �����
        if(PerspectiveProj==true) D3DXMatrixPerspectiveFovLH(&MatProj,D3DX_PI/4,(float)ScreenWidth/(float)ScreenHeight,1.0f,10000.0f);
        else D3DXMatrixOrthoLH(&MatProj,(float)ScreenWidth/4,(float)ScreenHeight/4,1.0f,1000.0f);
        // �������������������� �����
        if(Sector!=NULL) Sector->Reset();       // �������� ������
        for(int i=0; i<List3D->Count; i++) {    // �������� ������� �����
                ((Object*)List3D->Items[i])->Reset();
                }
        // �������� ������� FPS ���� �� �������
        if(Fps->ShowFps==true) Fps->Reset();
        // ����������� ���������� ���������� �� ��������� � ��� �����������
        pD3DDevice->Reset(&ParamStruc);
        SetDeviceParams();      // ������ ������������ �������������� ��������� ���������� ����������
        Cam->UpdateWorld();     // ������������ ������� ������
//        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
//        for(int i=0; i<List3D->Count; i++) {
//                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
//                }
        // ����������� ������� FPS ���� �� �������
        if(Fps->ShowFps==true) Fps->Restore();
        // ������ ������������ ������� �����
        if(Sector!=NULL) Sector->Restore();     // ������������ ������
        for(int i=0; i<List3D->Count; i++) {
                ((Object*)List3D->Items[i])->Restore();
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        // ��������� ������� ������
        // ESC - ����� �� ���������
if(Key==VK_ESCAPE) {
        GameCicleRun = false;
        Close();
        }
        // F1 - �������� ����� (��� ������)
if(Key==VK_F1) {
        CreateScene();
        }
        // "Numpad *" - ������ ������ � ��������� ��������� (0,0,-100)
if(Key==VK_MULTIPLY) {
        Cam->DefaultPosition();
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Numpad +" - ����������� ������ (��� ������������� �������� ������ �� �������)
if(Key==VK_ADD&&PerspectiveProj==true) {
        Cam->Scale(true);
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Numpad -" - �������� ������ (��� ������������� �������� ������ �� �������)
if(Key==VK_SUBTRACT&&PerspectiveProj==true) {
        Cam->Scale(false);
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // " Numpad6 " - ����������� ������ �������
if((Key==VK_NUMPAD6)&&!Shift.Contains(ssCtrl)) {
        Cam->ScrollRight();
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // " Numpad4 " - ����������� ������ ������
if((Key==VK_NUMPAD4)&&!Shift.Contains(ssCtrl)) {
        Cam->ScrollLeft();
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // " Numpad8 " - ����������� ������ �����
if((Key==VK_NUMPAD8)&&!Shift.Contains(ssCtrl)) {
        Cam->ScrollUp();
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // " Numpad2 " - ����������� ������ ����
if((Key==VK_NUMPAD2)&&!Shift.Contains(ssCtrl)) {
        Cam->ScrollDown();
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Ctrl + Numpad6 " - ������� ������ ������� (��� ������������� �������� ������ �� ������������)
if((Key==VK_NUMPAD6)&&Shift.Contains(ssCtrl)&&PerspectiveProj==true) {
        Cam->RotateRight();
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Ctrl + Numpad4 " - ������� ������ ������ (��� ������������� �������� ������ �� ������������)
if((Key==VK_NUMPAD4)&&Shift.Contains(ssCtrl)&&PerspectiveProj==true) {
        Cam->RotateLeft();
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Ctrl + Numpad8 " - ������� ������ ����� (��� ������������� �������� ������ �� ������������)
if((Key==VK_NUMPAD8)&&Shift.Contains(ssCtrl)&&PerspectiveProj==true) {
        Cam->RotateUp();
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Ctrl + Numpad2 " - ������� ������ ���� (��� ������������� �������� ������ �� ������������)
if((Key==VK_NUMPAD2)&&Shift.Contains(ssCtrl)&&PerspectiveProj==true) {
        Cam->RotateDown();
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }


        // �������� ������� (��� ������)
if(Key==VK_RIGHT&&!Shift.Contains(ssAlt)) {
//        ((Object3DUnit*)(List3D->Items[2]))->MoveTo(1.0,0.0,0.0);
        ((Object3DUnit*)(List3D->Items[2]))->MoveX(1.0);
        ((Object3DUnit*)(List3D->Items[2]))->UpdateTransform();
        TList* IntList = new TList();
        IntList->Clear();
        ((Object3DUnit*)(List3D->Items[2]))->CheckIntersections(List3D,IntList);
        ((ObjectText*)List3D->Items[1])->Text = IntToStr(IntList->Count);
//        Form1->Caption = IntToStr(IntList->Count);
        delete IntList;
        }
if(Key==VK_LEFT&&!Shift.Contains(ssAlt)) {
//        ((Object3DUnit*)(List3D->Items[2]))->MoveTo(-1.0,0.0,0.0);
        ((Object3DUnit*)(List3D->Items[2]))->MoveX(-1.0);
        ((Object3DUnit*)(List3D->Items[2]))->UpdateTransform();
        TList* IntList = new TList();
        IntList->Clear();
        ((Object3DUnit*)(List3D->Items[2]))->CheckIntersections(List3D,IntList);
        ((ObjectText*)List3D->Items[1])->Text = IntToStr(IntList->Count);
//        Form1->Caption = IntToStr(IntList->Count);
        delete IntList;
        }
if(Key==VK_UP&&!Shift.Contains(ssAlt)) {
//        ((Object3DUnit*)(List3D->Items[2]))->MoveTo(0.0,1.0,0.0);
        ((Object3DUnit*)(List3D->Items[2]))->MoveY(1.0);
        ((Object3DUnit*)(List3D->Items[2]))->UpdateTransform();
        TList* IntList = new TList();
        IntList->Clear();
        ((Object3DUnit*)(List3D->Items[2]))->CheckIntersections(List3D,IntList);
        ((ObjectText*)List3D->Items[1])->Text = IntToStr(IntList->Count);
//        Form1->Caption = IntToStr(IntList->Count);
        delete IntList;
        }
if(Key==VK_DOWN&&!Shift.Contains(ssAlt)) {
//        ((Object3DUnit*)(List3D->Items[2]))->MoveTo(0.0,-1.0,0.0);
        ((Object3DUnit*)(List3D->Items[2]))->MoveY(-1.0);
        ((Object3DUnit*)(List3D->Items[2]))->UpdateTransform();
        TList* IntList = new TList();
        IntList->Clear();
        ((Object3DUnit*)(List3D->Items[2]))->CheckIntersections(List3D,IntList);
        ((ObjectText*)List3D->Items[1])->Text = IntToStr(IntList->Count);
//        Form1->Caption = IntToStr(IntList->Count);
        delete IntList;
        }
if(Key==VK_RIGHT&&Shift.Contains(ssAlt)) {
        ((Object3DUnit*)(List3D->Items[2]))->RotateY(1.0);
        ((Object3DUnit*)(List3D->Items[2]))->UpdateTransform();
        TList* IntList = new TList();
        IntList->Clear();
        ((Object3DUnit*)(List3D->Items[2]))->CheckIntersections(List3D,IntList);
        ((ObjectText*)List3D->Items[1])->Text = IntToStr(IntList->Count);
//        Form1->Caption = IntToStr(IntList->Count);
        delete IntList;
        }
if(Key==VK_LEFT&&Shift.Contains(ssAlt)) {
        ((Object3DUnit*)(List3D->Items[2]))->RotateY(-1.0);
        ((Object3DUnit*)(List3D->Items[2]))->UpdateTransform();
        TList* IntList = new TList();
        IntList->Clear();
        ((Object3DUnit*)(List3D->Items[2]))->CheckIntersections(List3D,IntList);
        ((ObjectText*)List3D->Items[1])->Text = IntToStr(IntList->Count);
//        Form1->Caption = IntToStr(IntList->Count);
        delete IntList;
        }
if(Key==VK_UP&&Shift.Contains(ssAlt)) {
        ((Object3DUnit*)(List3D->Items[2]))->RotateX(1.0);
        ((Object3DUnit*)(List3D->Items[2]))->UpdateTransform();
        TList* IntList = new TList();
        IntList->Clear();
        ((Object3DUnit*)(List3D->Items[2]))->CheckIntersections(List3D,IntList);
        ((ObjectText*)List3D->Items[1])->Text = IntToStr(IntList->Count);
//        Form1->Caption = IntToStr(IntList->Count);
        delete IntList;
        }
if(Key==VK_DOWN&&Shift.Contains(ssAlt)) {
        ((Object3DUnit*)(List3D->Items[2]))->RotateX(-1.0);
        ((Object3DUnit*)(List3D->Items[2]))->UpdateTransform();
        TList* IntList = new TList();
        IntList->Clear();
        ((Object3DUnit*)(List3D->Items[2]))->CheckIntersections(List3D,IntList);
        ((ObjectText*)List3D->Items[1])->Text = IntToStr(IntList->Count);
//        Form1->Caption = IntToStr(IntList->Count);
        delete IntList;
        }
if(Key==VK_PRIOR) {
        ((Object3DUnit*)(List3D->Items[2]))->Scale(2.0,2.0,2.0);
        ((Object3DUnit*)(List3D->Items[2]))->UpdateTransform();
        TList* IntList = new TList();
        IntList->Clear();
        ((Object3DUnit*)(List3D->Items[2]))->CheckIntersections(List3D,IntList);
        ((ObjectText*)List3D->Items[1])->Text = IntToStr(IntList->Count);
//        Form1->Caption = IntToStr(IntList->Count);
        delete IntList;
        }
if(Key==VK_NEXT) {
        ((Object3DUnit*)(List3D->Items[2]))->Scale(0.5,0.5,0.5);
        ((Object3DUnit*)(List3D->Items[2]))->UpdateTransform();
        TList* IntList = new TList();
        IntList->Clear();
        ((Object3DUnit*)(List3D->Items[2]))->CheckIntersections(List3D,IntList);
        ((ObjectText*)List3D->Items[1])->Text = IntToStr(IntList->Count);
//        Form1->Caption = IntToStr(IntList->Count);
        delete IntList;
        }
//Form1->Caption = FloatToStr(((Object3D*)(List3D->Items[2]))->Position.x)+" "+FloatToStr(((Object3D*)(List3D->Items[2]))->Position.y)+" "+FloatToStr(((Object3D*)(List3D->Items[2]))->Position.z);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
        // ��������� ����������� ������� (�����)
        // ���������� ����������, ������� ����������, ��� ����� ����������
        // �������� ����� �� ������ �� �������� �����
MovedX = X;
MovedY = Y;
MouseMoved = true;
//int aa = ProcessCursorMoving();
//if(List3D->Count>0) ((ObjectText*)List3D->Items[1])->Text = IntToStr(aa);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
        // ��������� ������� �����
        // ���������� ����������, ������� ����������, ��� ����� ����������
        // ���� ������ �� ������ �� �������� �����
ClickedX = X;
ClickedY = Y;
MouseClicked = true;
}
//---------------------------------------------------------------------------
//                              ������� Direct3D
//---------------------------------------------------------------------------
bool TForm1::InitDirect3D()
{
        // ������������� Direct3D
        // ������� ������ Direct3D
pD3D = Direct3DCreate8(D3D_SDK_VERSION);
if(pD3D==NULL) return false;
        // ������� ���������� ����������
        // �������� ����� ��������� �����������
D3DDISPLAYMODE DisplayParam;
pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&DisplayParam);
        // ���������� ��������� Z-������ ��� ������ �����-�����
D3DFORMAT EnableZFormat = D3DFMT_D16;   // ������ � ����������� � ���������, ����-�� ����� ������
D3DFORMAT ZFormats[] = {D3DFMT_D15S1,D3DFMT_D24X8,D3DFMT_D24S8,D3DFMT_D24X4S4,D3DFMT_D32};
for(int i=0; i<ARRAYSIZE(ZFormats); i++) {
        // ������� ��� ��������� ������� Z-������� �� ������������� � ������ �����-������
        if(SUCCEEDED(pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DisplayParam.Format,D3DUSAGE_DEPTHSTENCIL,D3DRTYPE_SURFACE,ZFormats[i]))) {
                if(SUCCEEDED(pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DisplayParam.Format,DisplayParam.Format,ZFormats[i]))) {
                        EnableZFormat = ZFormats[i];
                        }
                }
        }
        // ��������� ��������� � ����������� ��� ���������� ����������
ZeroMemory(&ParamStruc,sizeof(ParamStruc));
        // ����� ���������
ParamStruc.BackBufferFormat = DisplayParam.Format;      // ���������� ������������� ��������� �����������
ParamStruc.SwapEffect = D3DSWAPEFFECT_DISCARD;          // ��� ������������ ������ ���
ParamStruc.EnableAutoDepthStencil = true;               // ��������� Z-�����
ParamStruc.AutoDepthStencilFormat = EnableZFormat;
        // ��� �������� ��������������/�������� �������
if(Form1->Tag == 0) {
        // ������� �����
        ParamStruc.Windowed = true;
        ParamStruc.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
        }
else {
        // ������������� �����
        ParamStruc.BackBufferWidth = ScreenWidth;
        ParamStruc.BackBufferHeight = ScreenHeight;
        ParamStruc.BackBufferCount = BackBufferCount;           // �� �������� �.�. ������-�� �� ��������� ����������� ������ ��� ������, ���� ������ 2
        //ParamStruc.FullScreen_RefreshRateInHz = 75;           // ������ �� ������������� ����� �������� ������ ��� �������� �������
        ParamStruc.Windowed = false;                            // ���������� ������������� ��������� �����������
        ParamStruc.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        }
        // �������� ������������� ����������� (���� �������� - �� �������� BackBuffer->Lock())
if(Settings->FindByName("MultisamplingLevel")!="0") {
        // ���������, ��������������-�� ������ ������� �����������, ���� �� - ������ ���, ���� ��� - ������ ������������ �� ��������������
        D3DMULTISAMPLE_TYPE EnableMultisamplingLevel = D3DMULTISAMPLE_NONE;
        D3DMULTISAMPLE_TYPE MultisamplingLevels[] = {D3DMULTISAMPLE_2_SAMPLES,D3DMULTISAMPLE_3_SAMPLES,D3DMULTISAMPLE_4_SAMPLES,D3DMULTISAMPLE_5_SAMPLES,D3DMULTISAMPLE_6_SAMPLES,D3DMULTISAMPLE_7_SAMPLES,D3DMULTISAMPLE_8_SAMPLES,D3DMULTISAMPLE_9_SAMPLES,D3DMULTISAMPLE_10_SAMPLES,D3DMULTISAMPLE_11_SAMPLES,D3DMULTISAMPLE_12_SAMPLES,D3DMULTISAMPLE_13_SAMPLES,D3DMULTISAMPLE_14_SAMPLES,D3DMULTISAMPLE_15_SAMPLES,D3DMULTISAMPLE_16_SAMPLES};
        for(int i=0; i<=StrToInt(Settings->FindByName("MultisamplingLevel"))-2;i++) {
                if(D3D_OK==pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DisplayParam.Format,ParamStruc.Windowed,EnableMultisamplingLevel)&&D3D_OK==pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,EnableZFormat,ParamStruc.Windowed,EnableMultisamplingLevel)) {
                        EnableMultisamplingLevel = MultisamplingLevels[i];
                        }
                }
        if(EnableMultisamplingLevel!=D3DMULTISAMPLE_NONE) ParamStruc.MultiSampleType = EnableMultisamplingLevel;
        else Settings->SetByName("MultisamplingLevel","0");
        }
        // �������� ������� ���������� ���������� ������� � ���������� ����������, ���� �� �������� - � �����������
Log->Add("������ ������� �������: "+IntToStr(ScreenWidth)+"x"+IntToStr(ScreenHeight)+"x"+IntToStr(ParamStruc.FullScreen_RefreshRateInHz));
if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,Handle,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&ParamStruc,&pD3DDevice))) {
        if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,Handle,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&ParamStruc,&pD3DDevice))) {
                return false;
                }
        }
       // ���������������� ������� ��������� �����
       // 1 �������� ������������ ���������
D3DXVECTOR3 vecDir;
ZeroMemory(&Light1,sizeof(D3DLIGHT8));
Light1.Type = D3DLIGHT_DIRECTIONAL;
Light1.Diffuse.r  = 1.0f;
Light1.Diffuse.g  = 1.0f;
Light1.Diffuse.b  = 1.0f;
vecDir = D3DXVECTOR3(-1.0f,1.0f,-1.0f);
D3DXVec3Normalize((D3DXVECTOR3*)&Light1.Direction,&vecDir);
Light1.Range = 1000.0f;
        // 2 �������� ������������ ���������
ZeroMemory(&Light2,sizeof(D3DLIGHT8));
Light2.Type = D3DLIGHT_DIRECTIONAL;
Light2.Diffuse.r  = 1.0f;
Light2.Diffuse.g  = 1.0f;
Light2.Diffuse.b  = 1.0f;
vecDir = D3DXVECTOR3(1.0f,-1.0f,-1.0f);
D3DXVec3Normalize((D3DXVECTOR3*)&Light2.Direction,&vecDir);
Light2.Range = 1000.0f;
        // ���������� �������������� ��������� ���������� ����������
SetDeviceParams();
return true;
}
//---------------------------------------------------------------------------
bool TForm1::SetDeviceParams()
{
        // ��������� �������������� ���������� ���������� ����������
        // ���������� ������� �����
pD3DDevice->SetTransform(D3DTS_WORLD,&MatWorld);
pD3DDevice->SetTransform(D3DTS_VIEW,&MatView);
pD3DDevice->SetTransform(D3DTS_PROJECTION,&MatProj);
        // ���������� �������� ������� ������ ���������
//pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
        // �������� ��������� ��������� �����
pD3DDevice->SetRenderState(D3DRS_LIGHTING,true);
pD3DDevice->SetRenderState(D3DRS_AMBIENT,Ambient);
pD3DDevice->SetLight(0,&Light1);
pD3DDevice->SetLight(1,&Light2);
pD3DDevice->LightEnable(0,true);
pD3DDevice->LightEnable(1,true);
        // ������� Z-������
pD3DDevice->SetRenderState(D3DRS_ZENABLE,true);
        // �������� ������������� �����������
if(Settings->FindByName("MultisamplingLevel")!="0") {
        pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,true);
}
return true;
}
//---------------------------------------------------------------------------
bool TForm1::DeleteDirect3D()
{
        // �������� Direct3D
if(pD3D!=NULL) {
        if(pD3DDevice!=NULL) {          // ������� ���������� ����������
                pD3DDevice->Release();
                pD3DDevice = NULL;
                }
        pD3D->Release();                // ������� ������ Direct3D
        pD3D = NULL;
        }
return true;
}
//---------------------------------------------------------------------------
bool TForm1::Render()
{
        // ����� ����� Direct3D �� �����
if(pD3DDevice==NULL) return false;
        // ���������, �� �������� �� ��������� ������� �����, ���� �������� - ������������ (�������� ��� ALT+TAB � ������������� ������)
HRESULT Rez = pD3DDevice->TestCooperativeLevel();
if(Rez==D3DERR_DEVICELOST) return false;
if(Rez==D3DERR_DEVICENOTRESET) {
        // �������� ������� �����
        if(Sector!=NULL) Sector->Reset();       // �������� ������
        for(int i=0; i<List3D->Count; i++) {
                ((Object*)List3D->Items[i])->Reset();
                }
        if(Fps->ShowFps==true) Fps->Reset();    // �������� ������� FPS ���� �� �������
        // �������������������� Direct3D
        pD3DDevice->Reset(&ParamStruc);         // ����������� ���������� ���������� �� ��������� � ��� �����������
        SetDeviceParams();                      // ������ ������������ �������������� ��������� ���������� ����������
        // ������������ ������� ����� ����� ������
        if(Fps->ShowFps==true) Fps->Restore();  // ����������� ������� FPS ���� �� �������
        if(Sector!=NULL) Sector->Restore();     // ������������ ������
        for(int i=0; i<List3D->Count; i++) {
                ((Object*)List3D->Items[i])->Restore();
                }
        }
        // �������� ��������� �����
pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,FonColor,1.0f,0);    // 1,2,5,6 ��������� ������������, D3DCLEAR_TARGET �������� ��� ��������� ��������� ����� ������ FonColor
        // ��������� ��������� ����� ������
pD3DDevice->BeginScene();       // ������������� ��������� �����
        // ������� ��� 3D-������� �� ������ � ����� �� ������
if(Sector!=NULL) Sector->Draw();        // ������
for(int i=0; i<List3D->Count; i++) {
        ((Object*)List3D->Items[i])->Draw();
        }
        // ������� FPS
Fps->Draw();
//Form1->Caption = IntToStr(Fps->GetFps());
pD3DDevice->EndScene();         // �������������� ��������� �����
        // �������� ������� ��������� � ��������� ������ (����� ���������� ������ �� �����)
pD3DDevice->Present(NULL,NULL,NULL,NULL);
return true;
}
//---------------------------------------------------------------------------
//                               ����� �������
//---------------------------------------------------------------------------
void TForm1::GameCicle(TMessage &Message)
{
        // �������� ���� ����
        // ���������������� ������
Cam->SetRenderDevice(pD3DDevice);
Cam->UpdateWorld();     // ������ ��������� ��������� ������
        // ���������������� ����� �������� FPS (����� �.�. ���� ������� � ������������ - ������, ������ - ?)
if(Settings->FindByName("ShowFps")!="0") Fps->SetFpsShow(pD3DDevice,5,5);
        // �����������

        // �������� �������� �������

        // ��������� ����
//int c = 0;
DWORD stime = timeGetTime();    // ��������� � �������� ������� ������� ��� ������� ��������� � 40 �� (25 ����/���)
DWORD etime = stime;
while(GameCicleRun==true) {     // ���������� ������� ����
//c+=1;
//Form1->Caption = IntToStr(c) + " - " + IntToStr(Fps->GetFps());
//Form1->Caption = FloatToStr(Fps->GetFramesDiff());
        // ������ ����� ������������ ��������� ��������� ���� MESSAGE
        Application->ProcessMessages();
        // ������� FPS � ������������ ������������ ��������
        Fps->NextFrame();
        // ��������� �������� ������������
        // �������� �������
        if(MouseMoved==true) ProcessCursorMoving();
        // ����� ������
        if(MouseClicked==true) ProcessCursorClick();
        // �������� ������� ����� ���������� ������ - ������ �� ������
        etime = timeGetTime();
        if(etime-stime<40) {
                Sleep(1);       // ����� ��������� �� ���������� �� 100% ������ while  !!! ������ �������� FPS, ���� �� ������ - FPS ���������� ������� ������ !!!
                continue;
                }
        stime = etime;
        // ��������� ������ ����� �� �����
        // �������� �����
        bool Rez = UpdateScene();
        if(Rez==false) Log->Add("������ ���������� �����");
        // ���������� �����
        Rez = Render();
        if(Rez==false) Log->Add("������ ����������");
        }
}
//---------------------------------------------------------------------------
bool TForm1::CreateScene()
{
        // �������� ����� (������� �������� ����� ��� �� ��������, �� ������
        // ���������� �� ������� ������)
Object* Tmp;

Sector = new ObjectSector();
Sector->SetRenderDevice(pD3DDevice);
Sector->SetFrustum(Cam->Frustum);
((ObjectSector*)Sector)->LoadFromFile("s1\\s1.vns");
Cam->SetSkyBox(Sector->SkyBox);

/*
Tmp = new ObjectSector();
Tmp->SetRenderDevice(pD3DDevice);
Tmp->SetFrustum(Cam->Frustum);
((ObjectSector*)Tmp)->LoadFromFile("s1\\1.vns");
List3D->Add(Tmp);
*/
/*
Tmp = new Object3DStar();        // Object* Tmp = new Object3DContainer();
Tmp->SetRenderDevice(pD3DDevice);
((Object3D*)Tmp)->SetFrustum(Cam->Frustum);
((Object3DUnit*)Tmp)->LoadFromFile("s1\\Sun.x");
((Object3DUnit*)Tmp)->UpdateTransform();        //((Object3DUnit*)Tmp)->CheckVisibility(Cam->Frustum);
List3D->Add(Tmp);
*/

Tmp = new ObjectText(pD3DDevice);
((ObjectText*)Tmp)->SetTextFont(14,FW_NORMAL,false,false,"Times");
((ObjectText*)Tmp)->Text = "������ \"����������\"";
((ObjectText*)Tmp)->SetRect(55,3,0,0);
List3D->Add(Tmp);

Tmp = new ObjectText(pD3DDevice);
((ObjectText*)Tmp)->SetTextFont(14,FW_NORMAL,false,false,"Times");
((ObjectText*)Tmp)->Text = "������ \"����������\"";
((ObjectText*)Tmp)->SetRect(55,23,0,0);
List3D->Add(Tmp);


//for(int i=0; i<100; i++){
//Application->ProcessMessages();

//Tmp = new Object3DContainer();
//Tmp->SetRenderDevice(pD3DDevice);
//((Object3DUnit*)Tmp)->SetFrustum(Cam->Frustum);
//((Object3DUnit*)Tmp)->LoadFromFile("Bigship.x");
//((Object3DUnit*)Tmp)->MoveTo(0.0,-15.0,0.0);
//((Object3DUnit*)Tmp)->UpdateTransform();        //((Object3DUnit*)Tmp)->CheckVisibility(Cam->Frustum);
//Tmp->Checkable = true;
//List3D->Add(Tmp);

//        }     // for 100

/*
Tmp = new Object3DStar();
Tmp->SetRenderDevice(pD3DDevice);
((Object3D*)Tmp)->SetFrustum(Cam->Frustum);
((Object3DUnit*)Tmp)->LoadFromFile("s1\\SkySphere.x");
((Object3DUnit*)Tmp)->UpdateTransform();        //((Object3DUnit*)Tmp)->CheckVisibility(Cam->Frustum);
List3D->Add(Tmp);
Cam->SetSkySphere((Object3D*)Tmp);
*/

return true;
}
//---------------------------------------------------------------------------
bool TForm1::UpdateScene()
{
        // ���������� ����� (��������� �������������� �������� � ����� � �.�.)
        // �������� ��� 3D-������� �� ������ � �����
if(Sector!=NULL) Sector->UpdateAnimation();
for(int i=0; i<List3D->Count; i++) {
        ((Object*)List3D->Items[i])->UpdateAnimation();
        }
//Form1->Caption = FloatToStr(Fps->GetFramesDiff());
return true;
}
//---------------------------------------------------------------------------
Object* TForm1::ProcessCursorMoving()
{
        // ��������� ����������� ������� �� ����� �����. ���������� ������ ��
        // ������� �������� ������ (NULL - ���)
Object* Rez = NULL;
        // �������� ������ �� ������� ������ ������
Rez = FindObjectByRecasting(MovedX,MovedY);
        // �������� �� ���������
if(MovedX<25||MovedX>ScreenWidth-25||MovedY<25||MovedY>ScreenHeight-25) {
        // ������������ �������� ����������
        if(Fps->GetFramesDiff()>0) Cam->SetScrollSpeed(CamSpeed*Fps->GetFramesDiff());
        // ��������� ���������
        if(MovedX<25) Cam->ScrollLeft();
        if(MovedX>ScreenWidth-25) Cam->ScrollRight();
        if(MovedY<25) Cam->ScrollUp();
        if(MovedY>ScreenHeight-25) Cam->ScrollDown();
        Cam->UpdateWorld();
        // ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
        if(Sector!=NULL) Sector->CheckVisibility();     // ������
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        // �.�. ��� ��������� - �������������� ���������, ���� ����� �� ����� �� ������� ���������
        return Rez;
        }
        // ���� ��� �� ��������� - ��������� �������� ������� ���������
MouseMoved = false;
MovedX = 0;
MovedY = 0;
return Rez;
}
//---------------------------------------------------------------------------
Object* TForm1::ProcessCursorClick()
{
        // ���������� ������� �� �������� ������������ ������� ������.
        // ���������� ������ (NULL - ���).
        // �������� ������ �������
Object* Rez = FindObjectByRecasting(ClickedX,ClickedY);
        // ���� ������ ���������
MouseClicked = false;
ClickedX = 0;
ClickedY = 0;
return Rez;
}
//---------------------------------------------------------------------------
Object* TForm1::FindObjectByRecasting(int CoordX, int CoordY)
{
        // ���������� ������� ��������� ������ Recasting'a. NULL - ������ ��
        // ������� (���)
        // �������� ��� �� ������ � ����� � ������������ CoordX,CoordY, ��� ��������� 2 �������� � �������� �����
D3DXVECTOR3 StartPoint;
D3DXVECTOR3 EndPoint;
float FeelingLength = 200.0;    // ���������������� - �� ����� ���������� �� ������ ������� ����� ����������
if(PerspectiveProj==true) {
                // ��������� ����� - ��������� ������
        StartPoint = Cam->GetPosition();
                // �������� ����� ��������� - ��� ����� ������ �������������� Screen Space -> Viewport Space -> View Space -> World Space
        double FOV = D3DX_PI/4;
        float aspectratio = (float)ScreenWidth/(float)ScreenHeight;
                // Screen Space -> Viewport Space
        float XTmp = 1.0 - 2.0*(float)CoordX/(float)ScreenWidth;
        float YTmp = 1.0 - 2.0*(float)CoordY/(float)ScreenHeight;
                // Viewport Space -> View Space
        EndPoint.x = -XTmp*aspectratio*tan(FOV/2.0);
        EndPoint.y = YTmp*tan(FOV/2.0);
        EndPoint.z = 1.0;
        EndPoint *= FeelingLength;
                // View Space -> World Space
        D3DXMATRIX TransformMatrix;
        pD3DDevice->GetTransform(D3DTS_VIEW,(D3DMATRIX*)&TransformMatrix);
        D3DXMatrixInverse(&TransformMatrix,NULL,&TransformMatrix);
        D3DXVec3TransformCoord(&EndPoint,&EndPoint,&TransformMatrix);   // �������� EndPoint
        }
else {
                // �������� ����� ��������� - ��� ����� ������ �������������� Screen Space -> Viewport Space -> View Space -> World Space
                // Screen Space -> Viev Space �.�. ��� ����� ����������� ������������ �� �����, ����� ������ ��������� ������������� ������� ������
        EndPoint.x = (((float)CoordX/2.0 - (float)ScreenWidth/4.0)/100.0)/4.0;  // ������� � ������� �������, ������������ �� ������
        EndPoint.y = (((float)ScreenHeight/4.0 - (float)CoordY/2.0)/100.0)/4.0; // ������� � ������� �������, ������������ �� ������
        EndPoint.z = 1.0;
        EndPoint *= FeelingLength;
                // View Space -> World Space
        D3DXMATRIX TransformMatrix;
        pD3DDevice->GetTransform(D3DTS_VIEW,(D3DMATRIX*)&TransformMatrix);
        D3DXMatrixInverse(&TransformMatrix,NULL,&TransformMatrix);
        D3DXVec3TransformCoord(&EndPoint,&EndPoint,&TransformMatrix);   // �������� EndPoint
                // ��������� ����� - ��������� ������ + X � Y ���������� � EndPoint �.�. ������������ ������������� ����� �� �����
        StartPoint.z = (Cam->GetPosition()).z;
        StartPoint.x = EndPoint.x;
        StartPoint.y = EndPoint.y;
        }
        // ��������� ��� ������� �� ����������� � ����� � ������� ���������
float Distance = FeelingLength;
Object* Rez = NULL;
        // ������
float TmpDistance = -1.0;
bool TmpRez;
if(Sector!=NULL) {
        TmpRez = Sector->CheckIntersectionWithLine(StartPoint,EndPoint,&TmpDistance,Rez);
        if(TmpRez==true) Distance = TmpDistance;
        }
        // �� ���� ��������
for(int i=0; i<List3D->Count; i++) {
        // ��������� ����������� ������� � ��������
        if(((Object*)List3D->Items[i])->Checkable==true) {
                TmpDistance  = -1.0;
                TmpRez = ((Object3DUnit*)List3D->Items[i])->CheckIntersectionWithLine(StartPoint,EndPoint,&TmpDistance);
                if(TmpRez == true && TmpDistance <= Distance) { // ���� ����� ����������� ����� ��� ���� ������ - ����� ���� ������ (�� ����)
                        Distance = TmpDistance;
                        Rez = (Object*)List3D->Items[i];
                        }
                }
        }
return Rez;
}
//---------------------------------------------------------------------------

