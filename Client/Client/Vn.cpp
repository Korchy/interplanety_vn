//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"		// Для правильной линковки
#include "Vn.h"
//---------------------------------------------------------------------------
Vn::Vn(void)
{
	// Конструктор класса без параметров
Log = NULL;
hWnd = NULL;
Windowed = false;	// По умолчанию - полноэкранный режим
Width = 800;
Height = 600;
pD3D = NULL;		// Главный объект Direct3D
pD3DDevice = NULL;	// Устройство рендеринга экрана
PerspectiveProj = true;	// По умолчанию - перспективная проекция
BackBufferCount = 2;	// На некоторых видеокартах не поддерживается больше 2 буферов
Ambient = 0xAAAAAAAA;	// Рассеяное освещение
FonColor = 0x00021355;	// По умолчанию фон синий
MultisamplingLevel = 0;	// По умолчанию сглаживание отключено
Cam = NULL;
Fps = NULL;
Scn = NULL;
Cons = NULL;
EditBox1 = NULL;
}
//---------------------------------------------------------------------------
Vn::Vn(HWND hw)
{
	// Конструктор класса с параметрами
Log = NULL;
hWnd = hw;
Windowed = false;	// По умолчанию - полноэкранный режим
Width = 800;
Height = 600;
pD3D = NULL;		// Главный объект Direct3D
pD3DDevice = NULL;	// Устройство рендеринга экрана
PerspectiveProj = true;
BackBufferCount = 2;	// На некоторых видеокартах не поддерживается больше 2 буферов
Ambient = 0xAAAAAAAA;	// Рассеяное освещение
FonColor = 0x00021355;	// По уомлчанию фон синий
MultisamplingLevel = 0;	// По умолчанию сглаживание отключено
Cam = NULL;
Fps = NULL;
Scn = NULL;
Cons = NULL;
EditBox1 = NULL;
}
//---------------------------------------------------------------------------
Vn::~Vn(void)
{
	// Деструктор класса
if(Cam!=NULL) delete Cam;
if(Fps!=NULL) delete Fps;
if(Scn!=NULL) delete Scn;
if(Cons!=NULL) delete Cons;
if(EditBox1!=NULL) delete EditBox1;
	// Удалить Direct3D
if(pD3D!=NULL) DeleteDirect3D();
}
//---------------------------------------------------------------------------
bool Vn::Init()
{
	// Инициализация
if(hWnd==NULL) return false;
	// Настроить матрицу мира
D3DXMatrixRotationY(&MatWorld,0.0f);	// сцена неподвижна
	// Настроить матрицу проекции
if(PerspectiveProj==true) D3DXMatrixPerspectiveFovLH(&MatProj,D3DX_PI/4,(float)Width/(float)Height,1.0f,1000.0f);	// поле зрения PI/4, масштаб выбирается в зависимости от р-ров формы, передняя граница отсечения 1, задняя - 1000 (вне границ отсечения ничего не рисуется)
else D3DXMatrixOrthoLH(&MatProj,(float)Width/4,(float)Height/4,1.0f,1000.0f);	// поле зрения PI/4, масштаб выбирается в зависимости от р-ров формы, передняя граница отсечения 1, задняя - 1000 (вне границ отсечения ничего не рисуется)
	// Инициализация Direct3D
bool Rez = InitDirect3D();
if(Rez==false) {
	if(Log!=NULL) Log->Add("Ошибка инициализации Direct3D");
	DeleteDirect3D();
	return false;
}
	// Создать камеру
Cam = new Camera();
Cam->SetRenderDevice(pD3DDevice);
Cam->UpdateWorld();     // Задаем начальное положение камеры
	// Создать счетчик FPS
Fps = new FpsCounter();
	// Создать сцену
Scn = new Scene();
//Scn->LoadFromFile("s1\\s1.vns");
// Создать консоль
Cons = new Console();
Cons->SetRenderDevice(pD3DDevice);
Cons->SetRect(5,Height-100,Width/2,Height);
Cons->Focused = true;
//Cons->Add("Введите пароль\n");
EditBox1 = new EditBox();
EditBox1->SetRenderDevice(pD3DDevice);
EditBox1->SetRect(5,20,58,35);
EditBox1->Focused = true;
EditBox1->Visible = true;
	// Инициализировать счетчик времени кадров
STime = timeGetTime();
ETime = STime;

return true;
}
//---------------------------------------------------------------------------
bool Vn::Destroy()
{
	// Завершение работы
if(Cons!=NULL) {	// Удалить консоль
	delete Cons;
	Cons = NULL;
}
if(EditBox1!=NULL) {	// Удалить EditBox1
	delete EditBox1;
	EditBox1 = NULL;
}
if(Scn!=NULL) {	// Удалить сцену
	delete Scn;
	Scn = NULL;
}
if(Cam!=NULL) {	// Удалить камеру
	delete Cam;
	Cam = NULL;
}
if(Fps!=NULL) {	// Удалить счетчик FPS
	delete Fps;
	Fps = NULL;
}
	// Удалить Direct3D
if(pD3D!=NULL) DeleteDirect3D();

return true;
}
//---------------------------------------------------------------------------
bool Vn::InitDirect3D()
{
	// Инициализация Direct3D
pD3D = Direct3DCreate8(D3D_SDK_VERSION);
if(pD3D==NULL) {
	if(Log!=NULL) Log->Add("Ошибка инициализации Direct3D\n");
	return false;
}
	// Создать устройство рендеринга
	// Получить общие параметры видеорежима
D3DDISPLAYMODE DisplayParam;
pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&DisplayParam);
	// Определить доступный Z-буффер для данной видео-карты
D3DFORMAT EnableZFormat = D3DFMT_D16;	// Начнем с простейшего и определим, есть-ли более крутой
D3DFORMAT ZFormats[] = {D3DFMT_D15S1,D3DFMT_D24X8,D3DFMT_D24S8,D3DFMT_D24X4S4,D3DFMT_D32};
for(int i=0; i<ARRAYSIZE(ZFormats); i++) {
	// Прверим все возможные форматы Z-буффера на совместимость с данной видео-картой
	if(SUCCEEDED(pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DisplayParam.Format,D3DUSAGE_DEPTHSTENCIL,D3DRTYPE_SURFACE,ZFormats[i]))) {
		if(SUCCEEDED(pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DisplayParam.Format,DisplayParam.Format,ZFormats[i]))) {
			EnableZFormat = ZFormats[i];
		}
	}
}
	// Заполнить структуры с параметрами для устройства рендеринга
ZeroMemory(&ParamStruc,sizeof(ParamStruc));
	// Общие параметры
ParamStruc.BackBufferFormat = DisplayParam.Format;	// Подключить установленные параметры видеорежима
ParamStruc.SwapEffect = D3DSWAPEFFECT_DISCARD;		// Фон сбрасывается каждый раз
ParamStruc.EnableAutoDepthStencil = true;			// Разрешить Z-буфер
ParamStruc.AutoDepthStencilFormat = EnableZFormat;
	// Для различия полноэкранного/оконного режимов
if(Windowed==true) {
	// Оконный режим
	ParamStruc.Windowed = true;
	ParamStruc.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
}
else {
	// Полноэкранный режим
	ParamStruc.BackBufferWidth = Width;
	ParamStruc.BackBufferHeight = Height;
	ParamStruc.BackBufferCount = BackBufferCount;	// Из настроек т.к. почему-то на некоторых видеокартах глючит при выходе, если больше 2
//	ParamStruc.FullScreen_RefreshRateInHz = 75;	// Почему то полноэкранный режим работает только без указания частоты
	ParamStruc.Windowed = false;					// Подключить установленные параметры видеорежима
	ParamStruc.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
	// Включить полноэкранное сглаживание (если включено - не работает BackBuffer->Lock())
if(MultisamplingLevel!=0) {
	// Проверить, поддерживается-ли нужный уровень сглаживания, если да - ставим его, если нет - ставим максимальный из поддерживаемых
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
// Пытаемся создать устройство рендеринга сначала с аппаратной поддержкой, если не работает - с программной
if(Log!=NULL) Log->Add("Размер рабочей области: "+Common::IntToStr(Width)+"x"+Common::IntToStr(Height)+"x"+Common::IntToStr(ParamStruc.FullScreen_RefreshRateInHz));
if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&ParamStruc,&pD3DDevice))) {
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&ParamStruc,&pD3DDevice))) {
		return false;        
	}    
}
	// Инициализировать базовые источники света
	// 1 источник равномерного освещения
D3DXVECTOR3 vecDir;
ZeroMemory(&Light1,sizeof(D3DLIGHT8));
Light1.Type = D3DLIGHT_DIRECTIONAL;
Light1.Diffuse.r  = 1.0f;
Light1.Diffuse.g  = 1.0f;
Light1.Diffuse.b  = 1.0f;
vecDir = D3DXVECTOR3(-1.0f,1.0f,-1.0f);
D3DXVec3Normalize((D3DXVECTOR3*)&Light1.Direction,&vecDir);
Light1.Range = 1000.0f;
	// 2 источник равномерного освещения
ZeroMemory(&Light2,sizeof(D3DLIGHT8));
Light2.Type = D3DLIGHT_DIRECTIONAL;
Light2.Diffuse.r  = 1.0f;
Light2.Diffuse.g  = 1.0f;
Light2.Diffuse.b  = 1.0f;
vecDir = D3DXVECTOR3(1.0f,-1.0f,-1.0f);
D3DXVec3Normalize((D3DXVECTOR3*)&Light2.Direction,&vecDir);
Light2.Range = 1000.0f;
	// Подключить дополнительные параметры рендеринга
SetDeviceParams();
return true;
}
//---------------------------------------------------------------------------
bool Vn::SetDeviceParams()
{
	// Установка дополнительных параметров рендеринга
	// Подключить матрицы сцены
pD3DDevice->SetTransform(D3DTS_WORLD,&MatWorld);
pD3DDevice->SetTransform(D3DTS_VIEW,&MatView);
pD3DDevice->SetTransform(D3DTS_PROJECTION,&MatProj);
	// Установить удаление тыльных сторон полигонов
//pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	// Включить источники освещения сцены
pD3DDevice->SetRenderState(D3DRS_LIGHTING,true);
pD3DDevice->SetRenderState(D3DRS_AMBIENT,Ambient);
pD3DDevice->SetLight(0,&Light1);
pD3DDevice->SetLight(1,&Light2);
pD3DDevice->LightEnable(0,true);
pD3DDevice->LightEnable(1,true);
	// Включть Z-буффер
pD3DDevice->SetRenderState(D3DRS_ZENABLE,true);
	// Включить полноэкранное сглаживание
if(MultisamplingLevel!=0) pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,true);
return true;
}
//---------------------------------------------------------------------------
bool Vn::Resize(int NewW,int NewH)
{
	// Переинициализация 3D при изменении размеров окна
if(pD3DDevice==NULL) return false;
	// Изменить размеры рабочей области окна
Width = NewW;
Height = NewH;
	// Переинициализировать мтарицу проекции с новыми р-рами
if(PerspectiveProj==true) D3DXMatrixPerspectiveFovLH(&MatProj,D3DX_PI/4,(float)Width/(float)Height,1.0f,10000.0f);
else D3DXMatrixOrthoLH(&MatProj,(float)Width/4,(float)Height/4,1.0f,1000.0f);
	// Переинициализировать сцену
if(Scn!=NULL) Scn->Reset();		// Сбросить Сцену
if(Fps->ShowFps==true) Fps->Reset();	// Сбросить счетчик FPS если он включен
if(Cons!=NULL) Cons->Reset();	// Сбросить консоль
if(EditBox1!=NULL) EditBox1->Reset();	// Сбросить консоль
	// Пересоздать устройство рендеринга из структуры с его параметрами
pD3DDevice->Reset(&ParamStruc);
SetDeviceParams();				// Заново восстановить дополнительные параметры устройства рендеринга
	// Восстановить все объекты и если нужно отмасштабировать их
Cam->UpdateWorld();     // Восстановить позицию камеры
	// Для всех объектов сцены проверить видимость в новом положении камеры
if(Scn!=NULL) Scn->CheckVisibility();
if(Cons!=NULL) {	// Восстановить консоль
	Cons->Restore();
	Cons->SetRect(5,Height-100,Width/2,Height);	// Отмасштабировать консоль на новые размеры
}
if(EditBox1!=NULL) {	// Восстановить консоль
	EditBox1->Restore();
}
if(Fps->ShowFps==true) Fps->Restore();	// Востановить счетчик FPS если он включен
	// Заново восстановить объекты сцены
if(Scn!=NULL) Scn->Restore();     // Восстановить Сцену
return true;
}
//---------------------------------------------------------------------------
bool Vn::DeleteDirect3D()
{
	// Удаление Direct3D
if(pD3D!=NULL) {
	if(pD3DDevice!=NULL) {		// Удалить устройство рендеринга
		pD3DDevice->Release();
		pD3DDevice = NULL;
	}
	pD3D->Release();			// Удалить объект Direct3D
	pD3D = NULL;
}
return true;
}
//---------------------------------------------------------------------------
void Vn::Interation()
{
	// Отработка одной интерации игрового цикла
Fps->NextFrame();	// Подсчет FPS и коэффициента коректировки скорости
ETime = timeGetTime();
if(ETime-STime>=40) {	// Прошло больше 40 мс - обработать сцену
	STime = ETime;
	// Обновить сцену
	bool Rez = Update();
	if(Rez==false) if(Log!=NULL) Log->Add("Ошибка обновления сцены\n");
	// Отобразить сцену
	Rez = Render();
	if(Rez==false) if(Log!=NULL) Log->Add("Ошибка рендеринга\n");
}
else {			// 40 мс еще не прошли - разгрузка процессора (сильно тормозит FPS, если ее убрать - FPS становится гораздо больше)
	Sleep(1);	// Чтобы процессор не загружался на 100% циклом while
}
}
//---------------------------------------------------------------------------
bool Vn::Update()
{
	// Обновление сцены
//if(Scn!=NULL) Scn->UpdateAnimation();
return true;
}
//---------------------------------------------------------------------------
bool Vn::Render()
{
	// Вывод сцены на экран
if(pD3DDevice==NULL) return false;
	// Проверить, не потеряны ли некоторые объекты сцены, если потеряны - восстановить (теряются при ALT+TAB в полноэкранном режиме)
HRESULT Rez = pD3DDevice->TestCooperativeLevel();
if(Rez==D3DERR_DEVICELOST) return false;
if(Rez==D3DERR_DEVICENOTRESET) {
	// Сбросить объекты сцены
	if(Scn!=NULL) Scn->Reset();				// Сбросить Сцену
	if(Fps->ShowFps==true) Fps->Reset();	// Сбросить счетчик FPS если он включен
	if(Cons!=NULL) Cons->Reset();			// Сбросить Консоль
	if(EditBox1!=NULL) EditBox1->Reset();			// Сбросить Консоль
	// Переинициализировать Direct3D
	pD3DDevice->Reset(&ParamStruc);			// Пересоздать устройство рендеринга из структуры с его параметрами
	SetDeviceParams();						// Заново восстановить дополнительные параметры устройства рендеринга
	// Восстановить объекты сцены после сброса
	if(Fps->ShowFps==true) Fps->Restore();	// Востановить счетчик FPS если он включен
	if(Cons!=NULL) Cons->Restore();			// Восстановить Консоль
	if(EditBox1!=NULL) EditBox1->Restore();			// Восстановить Консоль
	if(Scn!=NULL) Scn->Restore();			// Восстановить Сектор
}
	// Очистить вторичный буфер
pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,FonColor,1.0f,0);    // 1,2,5,6 параметры игнорируются, D3DCLEAR_TARGET означает что заполняем вторичный буфер цветом FonColor
	// Заполнить вторичный буфер сценой
pD3DDevice->BeginScene();	// Заблокировать вторичный буфер
	// Вывести все объекты на экран
if(Scn!=NULL) Scn->Draw();	// Сцена
	// Вывести FPS
Fps->Draw();
	// Вывести Консоль
if(Cons!=NULL) Cons->Draw();
if(EditBox1!=NULL) EditBox1->Draw();
pD3DDevice->EndScene();		// Разблокировать вторичный буфер
	// Поменять местами первичный и вторичный буфера (вывод вторичного буфера на экран)
pD3DDevice->Present(NULL,NULL,NULL,NULL);
return true;
}
//---------------------------------------------------------------------------
void Vn::ShowFPS()
{
	// Показвать FPS на экране
if(Fps!=NULL) {
	Fps->SetFpsShow(pD3DDevice,5,5);
}
}
//---------------------------------------------------------------------------
void Vn::KeyPress(char Key,char Params)
{
	// Обработка нажатия клавиш на клавиатуре
if(Scn!=NULL&&Scn->Focused==true) Scn->KeyPress(Key,Params);
if(Cons!=NULL&&Cons->Focused==true) Cons->KeyPress(Key,Params);
if(EditBox1!=NULL&&EditBox1->Focused==true) EditBox1->KeyPress(Key,Params);
}
//---------------------------------------------------------------------------
void Vn::SystemKeyPress(char Key,char Params)
{
	// Обработка нажатия клавиш на клавиатуре
if(Scn!=NULL&&Scn->Focused==true) Scn->SystemKeyPress(Key,Params);
if(Cons!=NULL&&Cons->Focused==true) Cons->SystemKeyPress(Key,Params);
if(EditBox1!=NULL&&EditBox1->Focused==true) EditBox1->SystemKeyPress(Key,Params);
}
//---------------------------------------------------------------------------