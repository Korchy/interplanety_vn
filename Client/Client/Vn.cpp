//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"		// ��� ���������� ��������
#include "Vn.h"
//---------------------------------------------------------------------------
Vn::Vn(void)
{
	// ����������� ������ ��� ����������
Log = NULL;
hWnd = NULL;
Windowed = false;	// �� ��������� - ������������� �����
Width = 800;
Height = 600;
pD3D = NULL;		// ������� ������ Direct3D
pD3DDevice = NULL;	// ���������� ���������� ������
PerspectiveProj = true;	// �� ��������� - ������������� ��������
BackBufferCount = 2;	// �� ��������� ����������� �� �������������� ������ 2 �������
Ambient = 0xAAAAAAAA;	// ��������� ���������
FonColor = 0x00021355;	// �� ��������� ��� �����
MultisamplingLevel = 0;	// �� ��������� ����������� ���������
Cam = NULL;
Fps = NULL;
Scn = NULL;
Cons = NULL;
EditBox1 = NULL;
}
//---------------------------------------------------------------------------
Vn::Vn(HWND hw)
{
	// ����������� ������ � �����������
Log = NULL;
hWnd = hw;
Windowed = false;	// �� ��������� - ������������� �����
Width = 800;
Height = 600;
pD3D = NULL;		// ������� ������ Direct3D
pD3DDevice = NULL;	// ���������� ���������� ������
PerspectiveProj = true;
BackBufferCount = 2;	// �� ��������� ����������� �� �������������� ������ 2 �������
Ambient = 0xAAAAAAAA;	// ��������� ���������
FonColor = 0x00021355;	// �� ��������� ��� �����
MultisamplingLevel = 0;	// �� ��������� ����������� ���������
Cam = NULL;
Fps = NULL;
Scn = NULL;
Cons = NULL;
EditBox1 = NULL;
}
//---------------------------------------------------------------------------
Vn::~Vn(void)
{
	// ���������� ������
if(Cam!=NULL) delete Cam;
if(Fps!=NULL) delete Fps;
if(Scn!=NULL) delete Scn;
if(Cons!=NULL) delete Cons;
if(EditBox1!=NULL) delete EditBox1;
	// ������� Direct3D
if(pD3D!=NULL) DeleteDirect3D();
}
//---------------------------------------------------------------------------
bool Vn::Init()
{
	// �������������
if(hWnd==NULL) return false;
	// ��������� ������� ����
D3DXMatrixRotationY(&MatWorld,0.0f);	// ����� ����������
	// ��������� ������� ��������
if(PerspectiveProj==true) D3DXMatrixPerspectiveFovLH(&MatProj,D3DX_PI/4,(float)Width/(float)Height,1.0f,1000.0f);	// ���� ������ PI/4, ������� ���������� � ����������� �� �-��� �����, �������� ������� ��������� 1, ������ - 1000 (��� ������ ��������� ������ �� ��������)
else D3DXMatrixOrthoLH(&MatProj,(float)Width/4,(float)Height/4,1.0f,1000.0f);	// ���� ������ PI/4, ������� ���������� � ����������� �� �-��� �����, �������� ������� ��������� 1, ������ - 1000 (��� ������ ��������� ������ �� ��������)
	// ������������� Direct3D
bool Rez = InitDirect3D();
if(Rez==false) {
	if(Log!=NULL) Log->Add("������ ������������� Direct3D");
	DeleteDirect3D();
	return false;
}
	// ������� ������
Cam = new Camera();
Cam->SetRenderDevice(pD3DDevice);
Cam->UpdateWorld();     // ������ ��������� ��������� ������
	// ������� ������� FPS
Fps = new FpsCounter();
	// ������� �����
Scn = new Scene();
//Scn->LoadFromFile("s1\\s1.vns");
// ������� �������
Cons = new Console();
Cons->SetRenderDevice(pD3DDevice);
Cons->SetRect(5,Height-100,Width/2,Height);
Cons->Focused = true;
//Cons->Add("������� ������\n");
EditBox1 = new EditBox();
EditBox1->SetRenderDevice(pD3DDevice);
EditBox1->SetRect(5,20,58,35);
EditBox1->Focused = true;
EditBox1->Visible = true;
	// ���������������� ������� ������� ������
STime = timeGetTime();
ETime = STime;

return true;
}
//---------------------------------------------------------------------------
bool Vn::Destroy()
{
	// ���������� ������
if(Cons!=NULL) {	// ������� �������
	delete Cons;
	Cons = NULL;
}
if(EditBox1!=NULL) {	// ������� EditBox1
	delete EditBox1;
	EditBox1 = NULL;
}
if(Scn!=NULL) {	// ������� �����
	delete Scn;
	Scn = NULL;
}
if(Cam!=NULL) {	// ������� ������
	delete Cam;
	Cam = NULL;
}
if(Fps!=NULL) {	// ������� ������� FPS
	delete Fps;
	Fps = NULL;
}
	// ������� Direct3D
if(pD3D!=NULL) DeleteDirect3D();

return true;
}
//---------------------------------------------------------------------------
bool Vn::InitDirect3D()
{
	// ������������� Direct3D
pD3D = Direct3DCreate8(D3D_SDK_VERSION);
if(pD3D==NULL) {
	if(Log!=NULL) Log->Add("������ ������������� Direct3D\n");
	return false;
}
	// ������� ���������� ����������
	// �������� ����� ��������� �����������
D3DDISPLAYMODE DisplayParam;
pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&DisplayParam);
	// ���������� ��������� Z-������ ��� ������ �����-�����
D3DFORMAT EnableZFormat = D3DFMT_D16;	// ������ � ����������� � ���������, ����-�� ����� ������
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
ParamStruc.BackBufferFormat = DisplayParam.Format;	// ���������� ������������� ��������� �����������
ParamStruc.SwapEffect = D3DSWAPEFFECT_DISCARD;		// ��� ������������ ������ ���
ParamStruc.EnableAutoDepthStencil = true;			// ��������� Z-�����
ParamStruc.AutoDepthStencilFormat = EnableZFormat;
	// ��� �������� ��������������/�������� �������
if(Windowed==true) {
	// ������� �����
	ParamStruc.Windowed = true;
	ParamStruc.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
}
else {
	// ������������� �����
	ParamStruc.BackBufferWidth = Width;
	ParamStruc.BackBufferHeight = Height;
	ParamStruc.BackBufferCount = BackBufferCount;	// �� �������� �.�. ������-�� �� ��������� ����������� ������ ��� ������, ���� ������ 2
//	ParamStruc.FullScreen_RefreshRateInHz = 75;	// ������ �� ������������� ����� �������� ������ ��� �������� �������
	ParamStruc.Windowed = false;					// ���������� ������������� ��������� �����������
	ParamStruc.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
	// �������� ������������� ����������� (���� �������� - �� �������� BackBuffer->Lock())
if(MultisamplingLevel!=0) {
	// ���������, ��������������-�� ������ ������� �����������, ���� �� - ������ ���, ���� ��� - ������ ������������ �� ��������������
	D3DMULTISAMPLE_TYPE EnableMultisamplingLevel = D3DMULTISAMPLE_NONE;
	D3DMULTISAMPLE_TYPE MultisamplingLevels[] = {D3DMULTISAMPLE_2_SAMPLES,D3DMULTISAMPLE_3_SAMPLES,D3DMULTISAMPLE_4_SAMPLES,D3DMULTISAMPLE_5_SAMPLES,D3DMULTISAMPLE_6_SAMPLES,D3DMULTISAMPLE_7_SAMPLES,D3DMULTISAMPLE_8_SAMPLES,D3DMULTISAMPLE_9_SAMPLES,D3DMULTISAMPLE_10_SAMPLES,D3DMULTISAMPLE_11_SAMPLES,D3DMULTISAMPLE_12_SAMPLES,D3DMULTISAMPLE_13_SAMPLES,D3DMULTISAMPLE_14_SAMPLES,D3DMULTISAMPLE_15_SAMPLES,D3DMULTISAMPLE_16_SAMPLES};
	for(int i=0; i<=MultisamplingLevel-2;i++) {
		if(D3D_OK==pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DisplayParam.Format,ParamStruc.Windowed,EnableMultisamplingLevel)&&D3D_OK==pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,EnableZFormat,ParamStruc.Windowed,EnableMultisamplingLevel)) {
			EnableMultisamplingLevel = MultisamplingLevels[i];            
		}         
	}
	if(EnableMultisamplingLevel!=D3DMULTISAMPLE_NONE) ParamStruc.MultiSampleType = EnableMultisamplingLevel;
	else MultisamplingLevel = 0;    
}
// �������� ������� ���������� ���������� ������� � ���������� ����������, ���� �� �������� - � �����������
if(Log!=NULL) Log->Add("������ ������� �������: "+Common::IntToStr(Width)+"x"+Common::IntToStr(Height)+"x"+Common::IntToStr(ParamStruc.FullScreen_RefreshRateInHz));
if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&ParamStruc,&pD3DDevice))) {
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&ParamStruc,&pD3DDevice))) {
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
	// ���������� �������������� ��������� ����������
SetDeviceParams();
return true;
}
//---------------------------------------------------------------------------
bool Vn::SetDeviceParams()
{
	// ��������� �������������� ���������� ����������
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
if(MultisamplingLevel!=0) pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,true);
return true;
}
//---------------------------------------------------------------------------
bool Vn::Resize(int NewW,int NewH)
{
	// ����������������� 3D ��� ��������� �������� ����
if(pD3DDevice==NULL) return false;
	// �������� ������� ������� ������� ����
Width = NewW;
Height = NewH;
	// �������������������� ������� �������� � ������ �-����
if(PerspectiveProj==true) D3DXMatrixPerspectiveFovLH(&MatProj,D3DX_PI/4,(float)Width/(float)Height,1.0f,10000.0f);
else D3DXMatrixOrthoLH(&MatProj,(float)Width/4,(float)Height/4,1.0f,1000.0f);
	// �������������������� �����
if(Scn!=NULL) Scn->Reset();		// �������� �����
if(Fps->ShowFps==true) Fps->Reset();	// �������� ������� FPS ���� �� �������
if(Cons!=NULL) Cons->Reset();	// �������� �������
if(EditBox1!=NULL) EditBox1->Reset();	// �������� �������
	// ����������� ���������� ���������� �� ��������� � ��� �����������
pD3DDevice->Reset(&ParamStruc);
SetDeviceParams();				// ������ ������������ �������������� ��������� ���������� ����������
	// ������������ ��� ������� � ���� ����� ���������������� ��
Cam->UpdateWorld();     // ������������ ������� ������
	// ��� ���� �������� ����� ��������� ��������� � ����� ��������� ������
if(Scn!=NULL) Scn->CheckVisibility();
if(Cons!=NULL) {	// ������������ �������
	Cons->Restore();
	Cons->SetRect(5,Height-100,Width/2,Height);	// ���������������� ������� �� ����� �������
}
if(EditBox1!=NULL) {	// ������������ �������
	EditBox1->Restore();
}
if(Fps->ShowFps==true) Fps->Restore();	// ����������� ������� FPS ���� �� �������
	// ������ ������������ ������� �����
if(Scn!=NULL) Scn->Restore();     // ������������ �����
return true;
}
//---------------------------------------------------------------------------
bool Vn::DeleteDirect3D()
{
	// �������� Direct3D
if(pD3D!=NULL) {
	if(pD3DDevice!=NULL) {		// ������� ���������� ����������
		pD3DDevice->Release();
		pD3DDevice = NULL;
	}
	pD3D->Release();			// ������� ������ Direct3D
	pD3D = NULL;
}
return true;
}
//---------------------------------------------------------------------------
void Vn::Interation()
{
	// ��������� ����� ��������� �������� �����
Fps->NextFrame();	// ������� FPS � ������������ ������������ ��������
ETime = timeGetTime();
if(ETime-STime>=40) {	// ������ ������ 40 �� - ���������� �����
	STime = ETime;
	// �������� �����
	bool Rez = Update();
	if(Rez==false) if(Log!=NULL) Log->Add("������ ���������� �����\n");
	// ���������� �����
	Rez = Render();
	if(Rez==false) if(Log!=NULL) Log->Add("������ ����������\n");
}
else {			// 40 �� ��� �� ������ - ��������� ���������� (������ �������� FPS, ���� �� ������ - FPS ���������� ������� ������)
	Sleep(1);	// ����� ��������� �� ���������� �� 100% ������ while
}
}
//---------------------------------------------------------------------------
bool Vn::Update()
{
	// ���������� �����
//if(Scn!=NULL) Scn->UpdateAnimation();
return true;
}
//---------------------------------------------------------------------------
bool Vn::Render()
{
	// ����� ����� �� �����
if(pD3DDevice==NULL) return false;
	// ���������, �� �������� �� ��������� ������� �����, ���� �������� - ������������ (�������� ��� ALT+TAB � ������������� ������)
HRESULT Rez = pD3DDevice->TestCooperativeLevel();
if(Rez==D3DERR_DEVICELOST) return false;
if(Rez==D3DERR_DEVICENOTRESET) {
	// �������� ������� �����
	if(Scn!=NULL) Scn->Reset();				// �������� �����
	if(Fps->ShowFps==true) Fps->Reset();	// �������� ������� FPS ���� �� �������
	if(Cons!=NULL) Cons->Reset();			// �������� �������
	if(EditBox1!=NULL) EditBox1->Reset();			// �������� �������
	// �������������������� Direct3D
	pD3DDevice->Reset(&ParamStruc);			// ����������� ���������� ���������� �� ��������� � ��� �����������
	SetDeviceParams();						// ������ ������������ �������������� ��������� ���������� ����������
	// ������������ ������� ����� ����� ������
	if(Fps->ShowFps==true) Fps->Restore();	// ����������� ������� FPS ���� �� �������
	if(Cons!=NULL) Cons->Restore();			// ������������ �������
	if(EditBox1!=NULL) EditBox1->Restore();			// ������������ �������
	if(Scn!=NULL) Scn->Restore();			// ������������ ������
}
	// �������� ��������� �����
pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,FonColor,1.0f,0);    // 1,2,5,6 ��������� ������������, D3DCLEAR_TARGET �������� ��� ��������� ��������� ����� ������ FonColor
	// ��������� ��������� ����� ������
pD3DDevice->BeginScene();	// ������������� ��������� �����
	// ������� ��� ������� �� �����
if(Scn!=NULL) Scn->Draw();	// �����
	// ������� FPS
Fps->Draw();
	// ������� �������
if(Cons!=NULL) Cons->Draw();
if(EditBox1!=NULL) EditBox1->Draw();
pD3DDevice->EndScene();		// �������������� ��������� �����
	// �������� ������� ��������� � ��������� ������ (����� ���������� ������ �� �����)
pD3DDevice->Present(NULL,NULL,NULL,NULL);
return true;
}
//---------------------------------------------------------------------------
void Vn::ShowFPS()
{
	// ��������� FPS �� ������
if(Fps!=NULL) {
	Fps->SetFpsShow(pD3DDevice,5,5);
}
}
//---------------------------------------------------------------------------
void Vn::KeyPress(char Key,char Params)
{
	// ��������� ������� ������ �� ����������
if(Scn!=NULL&&Scn->Focused==true) Scn->KeyPress(Key,Params);
if(Cons!=NULL&&Cons->Focused==true) Cons->KeyPress(Key,Params);
if(EditBox1!=NULL&&EditBox1->Focused==true) EditBox1->KeyPress(Key,Params);
}
//---------------------------------------------------------------------------
void Vn::SystemKeyPress(char Key,char Params)
{
	// ��������� ������� ������ �� ����������
if(Scn!=NULL&&Scn->Focused==true) Scn->SystemKeyPress(Key,Params);
if(Cons!=NULL&&Cons->Focused==true) Cons->SystemKeyPress(Key,Params);
if(EditBox1!=NULL&&EditBox1->Focused==true) EditBox1->SystemKeyPress(Key,Params);
}
//---------------------------------------------------------------------------