//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
//                              СОБЫТИЯ ФОРМЫ
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        // При загрузке программы (Конструктор)
        // Загрузить настройки
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
        ShowMessage("Невозможно прочитать настройки!");
        Application->Terminate();
        return;
        }
        // Создать лог-файл для записей
Log = new LogFile();
Log->Path = GetCurrentDir();
Log->FileName = "VN_Log";
Log->Add("--------------------------------------------------");
        // Вызвать форму активации клиента на сервере
Form2 = new TForm2(this);
if(Form2->ShowModal()==mrCancel) {
        delete Form2;
        Application->Terminate();
        return;
        }
else delete Form2;
        // Переменные по умолчанию
Ambient = 0xAAAAAAAA;   // Величина рассеянного освещения
FonColor = 0x00021355;  // Цвет фона
CamSpeed = 0.1;         // Скорость скроллинга камеры 0.1 ед. за 1 мс.
        // Вид формы
if(Form1->Tag==0) {
        // Оконный режим
        Form1->BorderStyle = bsSizeable;
        ScreenWidth = Form1->ClientWidth;
        ScreenHeight = Form1->ClientHeight;
        }
else {
        // Полноэкранный режим
        Form1->BorderStyle = bsNone;
        ScreenWidth = 800;
        ScreenHeight = 600;
        }
        // Инициализация 3D
pD3D = NULL;            // Главный объект Direct3D
pD3DDevice = NULL;      // Устройство рендеринга экрана
D3DXMatrixRotationY(&MatWorld,0.0f);    // Мировая матрица - Сцена неподвижна
//D3DXMatrixLookAtLH(&MatView,&D3DXVECTOR3(0.0f,0.0f,-100.0f),&D3DXVECTOR3(0.0f,0.0f,0.0f),&D3DXVECTOR3(0.0f,1.0f,0.0f)); // смотрим из точки 0,-100,0 в точку 0,0,0, верхний вектор зрения 0,1,0
if(PerspectiveProj==true) D3DXMatrixPerspectiveFovLH(&MatProj,D3DX_PI/4,(float)ScreenWidth/(float)ScreenHeight,1.0f,1000.0f);      // поле зрения PI/4, масштаб выбирается в зависимости от р-ров формы, передняя граница отсечения 1, задняя - 1000 (вне границ отсечения ничего не рисуется)
else D3DXMatrixOrthoLH(&MatProj,(float)ScreenWidth/4,(float)ScreenHeight/4,1.0f,1000.0f);      // поле зрения PI/4, масштаб выбирается в зависимости от р-ров формы, передняя граница отсечения 1, задняя - 1000 (вне границ отсечения ничего не рисуется)
bool Rez = InitDirect3D();
if(Rez==false) {
        Log->Add("Ошибка инициализации Direct3D");
        DeleteDirect3D();
        Application->Terminate();
        return;
        }
        // Камера
Cam = new Camera();
        // Счетчик FPS
Fps = new FpsCounter();
        // Список всех объектов сцены
List3D = new TList();
        // Объекты сцены
Sector = NULL;  // Сектор
        // Изначально мышка не двигалась и не нажималась
MouseClicked = false;
ClickedX = 0;
ClickedY = 0;
MouseMoved = false;
MovedX = 0;
MovedY = 0;
        // Запускаем игровой цикл (через MESSAGE, чтобы нормально завершилась вызывающая ф-ция, в данном случае конструктор)
GameCicleRun = true;
PostMessage(Handle,WM_START,0,0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        // При попытке закрыть форму - нужно чтобы правильно выходить из
        // главного игрового цикла в оконном режиме
GameCicleRun = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
        // При закрытии программы
        // Удалить все объекты и список для этих объектов
if(List3D!=NULL) {
        for(int i=0; i<List3D->Count; i++) {
                if(List3D->Items[i]!=NULL) delete (Object*)List3D->Items[i];
                }
        delete List3D;
        }
        // Удалить Сектор
if(Sector!=NULL) delete Sector;
        // Удалить Камеру
if(Cam!=NULL) delete Cam;
        // Удалить счетчик FPS
if(Fps!=NULL) delete Fps;
        // Удалить Direct3D
bool Rez = DeleteDirect3D();
if(Rez==false) Log->Add("Ошибка при закрытии Direct3D");
        // Сохранить настройки
if(Form1->WindowState==wsNormal&&Form1->Tag==0) {       // Сохраняем только для оконного режима
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
delete Settings;        // Удалить объект с настройками
         // Закрыть Лог-файл
if(Log!=NULL) delete Log;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
        // При изменении размеров формы (только для оконного режима)
if(pD3DDevice!=NULL&&Form1->Tag==0) {
        // Изменить размеры рабочей области формы
        ScreenWidth = Form1->ClientWidth;
        ScreenHeight = Form1->ClientHeight;
        // Переинициализировать мтарицу проэкции с новыми р-рами формы
        if(PerspectiveProj==true) D3DXMatrixPerspectiveFovLH(&MatProj,D3DX_PI/4,(float)ScreenWidth/(float)ScreenHeight,1.0f,10000.0f);
        else D3DXMatrixOrthoLH(&MatProj,(float)ScreenWidth/4,(float)ScreenHeight/4,1.0f,1000.0f);
        // Переинициализировать сцену
        if(Sector!=NULL) Sector->Reset();       // Сбросить Сектор
        for(int i=0; i<List3D->Count; i++) {    // Сбросить объекты сцены
                ((Object*)List3D->Items[i])->Reset();
                }
        // Сбросить счетчик FPS если он включен
        if(Fps->ShowFps==true) Fps->Reset();
        // Пересоздать устройство рендеринга из структуры с его параметрами
        pD3DDevice->Reset(&ParamStruc);
        SetDeviceParams();      // Заново восстановить дополнительные параметры устройства рендеринга
        Cam->UpdateWorld();     // Восстановить позицию камеры
//        // Для всех объектов сцены проверить видимость в новом положении камеры
//        for(int i=0; i<List3D->Count; i++) {
//                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
//                }
        // Востановить счетчик FPS если он включен
        if(Fps->ShowFps==true) Fps->Restore();
        // Заново восстановить объекты сцены
        if(Sector!=NULL) Sector->Restore();     // Восстановить Сектор
        for(int i=0; i<List3D->Count; i++) {
                ((Object*)List3D->Items[i])->Restore();
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        // Обработка нажатия кнопок
        // ESC - выход из программы
if(Key==VK_ESCAPE) {
        GameCicleRun = false;
        Close();
        }
        // F1 - создание сцены (для тестов)
if(Key==VK_F1) {
        CreateScene();
        }
        // "Numpad *" - ставим камеру в начальное положение (0,0,-100)
if(Key==VK_MULTIPLY) {
        Cam->DefaultPosition();
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Numpad +" - приближение камеры (для ортогональной проекции камеру не двигаем)
if(Key==VK_ADD&&PerspectiveProj==true) {
        Cam->Scale(true);
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Numpad -" - удаление камеры (для ортогональной проекции камеру не двигаем)
if(Key==VK_SUBTRACT&&PerspectiveProj==true) {
        Cam->Scale(false);
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // " Numpad6 " - перемещение камеры направо
if((Key==VK_NUMPAD6)&&!Shift.Contains(ssCtrl)) {
        Cam->ScrollRight();
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // " Numpad4 " - перемещение камеры налево
if((Key==VK_NUMPAD4)&&!Shift.Contains(ssCtrl)) {
        Cam->ScrollLeft();
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // " Numpad8 " - перемещение камеры вверх
if((Key==VK_NUMPAD8)&&!Shift.Contains(ssCtrl)) {
        Cam->ScrollUp();
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // " Numpad2 " - перемещение камеры вниз
if((Key==VK_NUMPAD2)&&!Shift.Contains(ssCtrl)) {
        Cam->ScrollDown();
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Ctrl + Numpad6 " - поворот камеры направо (для ортогональной проекции камеру не поворачиваем)
if((Key==VK_NUMPAD6)&&Shift.Contains(ssCtrl)&&PerspectiveProj==true) {
        Cam->RotateRight();
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Ctrl + Numpad4 " - поворот камеры налево (для ортогональной проекции камеру не поворачиваем)
if((Key==VK_NUMPAD4)&&Shift.Contains(ssCtrl)&&PerspectiveProj==true) {
        Cam->RotateLeft();
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Ctrl + Numpad8 " - поворот камеры вверх (для ортогональной проекции камеру не поворачиваем)
if((Key==VK_NUMPAD8)&&Shift.Contains(ssCtrl)&&PerspectiveProj==true) {
        Cam->RotateUp();
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }
        // "Ctrl + Numpad2 " - поворот камеры вниз (для ортогональной проекции камеру не поворачиваем)
if((Key==VK_NUMPAD2)&&Shift.Contains(ssCtrl)&&PerspectiveProj==true) {
        Cam->RotateDown();
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        }


        // движение объекта (для тестов)
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
        // Обработка перемещения курсора (мышки)
        // Установить переменную, которая показывает, что нужно отработать
        // движения мышки не выходя из игрового цикла
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
        // Обработка щелчков мышки
        // Установить переменную, которая показывает, что нужно отработать
        // клик мышкой не выходя из игрового цикла
ClickedX = X;
ClickedY = Y;
MouseClicked = true;
}
//---------------------------------------------------------------------------
//                              ФУНКЦИИ Direct3D
//---------------------------------------------------------------------------
bool TForm1::InitDirect3D()
{
        // Инициализация Direct3D
        // Создать объект Direct3D
pD3D = Direct3DCreate8(D3D_SDK_VERSION);
if(pD3D==NULL) return false;
        // Создать устройство рендеринга
        // Получить общие параметры видеорежима
D3DDISPLAYMODE DisplayParam;
pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&DisplayParam);
        // Определить доступный Z-буффер для данной видео-карты
D3DFORMAT EnableZFormat = D3DFMT_D16;   // Начнем с простейшего и определим, есть-ли более крутой
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
ParamStruc.BackBufferFormat = DisplayParam.Format;      // Подключить установленные параметры видеорежима
ParamStruc.SwapEffect = D3DSWAPEFFECT_DISCARD;          // Фон сбрасывается каждый раз
ParamStruc.EnableAutoDepthStencil = true;               // Разрешить Z-буфер
ParamStruc.AutoDepthStencilFormat = EnableZFormat;
        // Для различия полноэкранного/оконного режимов
if(Form1->Tag == 0) {
        // Оконный режим
        ParamStruc.Windowed = true;
        ParamStruc.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
        }
else {
        // Полноэкранный режим
        ParamStruc.BackBufferWidth = ScreenWidth;
        ParamStruc.BackBufferHeight = ScreenHeight;
        ParamStruc.BackBufferCount = BackBufferCount;           // Из настроек т.к. почему-то на некоторых видеокартах глючит при выходе, если больше 2
        //ParamStruc.FullScreen_RefreshRateInHz = 75;           // Почему то полноэкранный режим работает только без указания частоты
        ParamStruc.Windowed = false;                            // Подключить установленные параметры видеорежима
        ParamStruc.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        }
        // Включить полноэкранное сглаживание (если включено - не работает BackBuffer->Lock())
if(Settings->FindByName("MultisamplingLevel")!="0") {
        // Проверить, поддерживается-ли нужный уровень сглаживания, если да - ставим его, если нет - ставим максимальный из поддерживаемых
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
        // Пытаемся создать устройство рендеринга сначала с аппаратной поддержкой, если не работает - с программной
Log->Add("Размер рабочей области: "+IntToStr(ScreenWidth)+"x"+IntToStr(ScreenHeight)+"x"+IntToStr(ParamStruc.FullScreen_RefreshRateInHz));
if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,Handle,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&ParamStruc,&pD3DDevice))) {
        if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,Handle,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&ParamStruc,&pD3DDevice))) {
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
        // Подключить дополнительные параметры устройства рендеринга
SetDeviceParams();
return true;
}
//---------------------------------------------------------------------------
bool TForm1::SetDeviceParams()
{
        // Установка дополнительных параметров устройства рендеринга
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
if(Settings->FindByName("MultisamplingLevel")!="0") {
        pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,true);
}
return true;
}
//---------------------------------------------------------------------------
bool TForm1::DeleteDirect3D()
{
        // Удаление Direct3D
if(pD3D!=NULL) {
        if(pD3DDevice!=NULL) {          // Удалить устройство рендеринга
                pD3DDevice->Release();
                pD3DDevice = NULL;
                }
        pD3D->Release();                // Удалить объект Direct3D
        pD3D = NULL;
        }
return true;
}
//---------------------------------------------------------------------------
bool TForm1::Render()
{
        // Вывод сцены Direct3D на экран
if(pD3DDevice==NULL) return false;
        // Проверить, не потеряны ли некоторые объекты сцены, если потеряны - восстановить (теряются при ALT+TAB в полноэкранном режиме)
HRESULT Rez = pD3DDevice->TestCooperativeLevel();
if(Rez==D3DERR_DEVICELOST) return false;
if(Rez==D3DERR_DEVICENOTRESET) {
        // Сбросить объекты сцены
        if(Sector!=NULL) Sector->Reset();       // Сбросить Сектор
        for(int i=0; i<List3D->Count; i++) {
                ((Object*)List3D->Items[i])->Reset();
                }
        if(Fps->ShowFps==true) Fps->Reset();    // Сбросить счетчик FPS если он включен
        // Переинициализировать Direct3D
        pD3DDevice->Reset(&ParamStruc);         // Пересоздать устройство рендеринга из структуры с его параметрами
        SetDeviceParams();                      // Заново восстановить дополнительные параметры устройства рендеринга
        // Восстановить объекты сцены после сброса
        if(Fps->ShowFps==true) Fps->Restore();  // Востановить счетчик FPS если он включен
        if(Sector!=NULL) Sector->Restore();     // Восстановить Сектор
        for(int i=0; i<List3D->Count; i++) {
                ((Object*)List3D->Items[i])->Restore();
                }
        }
        // Очистить вторичный буфер
pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,FonColor,1.0f,0);    // 1,2,5,6 параметры игнорируются, D3DCLEAR_TARGET означает что заполняем вторичный буфер цветом FonColor
        // Заполнить вторичный буфер сценой
pD3DDevice->BeginScene();       // Заблокировать вторичный буфер
        // Вывести все 3D-объекты из списка в сцену на экране
if(Sector!=NULL) Sector->Draw();        // Сектор
for(int i=0; i<List3D->Count; i++) {
        ((Object*)List3D->Items[i])->Draw();
        }
        // Вывести FPS
Fps->Draw();
//Form1->Caption = IntToStr(Fps->GetFps());
pD3DDevice->EndScene();         // Разблокировать вторичный буфер
        // Поменять местами первичный и вторичный буфера (вывод вторичного буфера на экран)
pD3DDevice->Present(NULL,NULL,NULL,NULL);
return true;
}
//---------------------------------------------------------------------------
//                               ОБЩИЕ ФУНКЦИИ
//---------------------------------------------------------------------------
void TForm1::GameCicle(TMessage &Message)
{
        // Основной цикл игры
        // Инициализировать Камеру
Cam->SetRenderDevice(pD3DDevice);
Cam->UpdateWorld();     // Задаем начальное положение камеры
        // Инициализировать показ счетчика FPS (здесь т.к. если делаешь в конструкторе - ошибка, почему - ?)
if(Settings->FindByName("ShowFps")!="0") Fps->SetFpsShow(pD3DDevice,5,5);
        // Авторизация

        // Загрузка текущего сектора

        // Запустить цикл
//int c = 0;
DWORD stime = timeGetTime();    // Начальный и конечные моменты времени для отсчета интервала в 40 мс (25 кадр/сек)
DWORD etime = stime;
while(GameCicleRun==true) {     // Собственно игровой цикл
//c+=1;
//Form1->Caption = IntToStr(c) + " - " + IntToStr(Fps->GetFps());
//Form1->Caption = FloatToStr(Fps->GetFramesDiff());
        // Внутри цикла обрабатывать системные сообщения типа MESSAGE
        Application->ProcessMessages();
        // Подсчет FPS и коэффициента коректировки скорости
        Fps->NextFrame();
        // Обработка действий пользователя
        // Движения курсора
        if(MouseMoved==true) ProcessCursorMoving();
        // Клики мышкой
        if(MouseClicked==true) ProcessCursorClick();
        // Задержка времени между обработкой кадров - ничего не делаем
        etime = timeGetTime();
        if(etime-stime<40) {
                Sleep(1);       // Чтобы процессор не загружался на 100% циклом while  !!! сильно тормозит FPS, если ее убрать - FPS становится гораздо больше !!!
                continue;
                }
        stime = etime;
        // Обработка вывода сцены на экран
        // Обновить сцену
        bool Rez = UpdateScene();
        if(Rez==false) Log->Add("Ошибка обновления сцены");
        // Отобразить сцену
        Rez = Render();
        if(Rez==false) Log->Add("Ошибка рендеринга");
        }
}
//---------------------------------------------------------------------------
bool TForm1::CreateScene()
{
        // Создание сцены (задание объектов сцены или их анимации, на основе
        // полученных от сервера данных)
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
((ObjectText*)Tmp)->Text = "Проект \"ВНЕЗЕМЕЛЬЕ\"";
((ObjectText*)Tmp)->SetRect(55,3,0,0);
List3D->Add(Tmp);

Tmp = new ObjectText(pD3DDevice);
((ObjectText*)Tmp)->SetTextFont(14,FW_NORMAL,false,false,"Times");
((ObjectText*)Tmp)->Text = "Проект \"ВНЕЗЕМЕЛЬЕ\"";
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
        // Обновление сцены (изменение местоположения объектов в сцене и т.п.)
        // Обновить все 3D-объекты из списка в сцене
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
        // Обработка перемещения курсора на новое место. Возвращает объект на
        // котором оказался курсор (NULL - фон)
Object* Rez = NULL;
        // Получить объект на который навели курсор
Rez = FindObjectByRecasting(MovedX,MovedY);
        // Проверка на скроллинг
if(MovedX<25||MovedX>ScreenWidth-25||MovedY<25||MovedY>ScreenHeight-25) {
        // Корректируем скорость скроллинга
        if(Fps->GetFramesDiff()>0) Cam->SetScrollSpeed(CamSpeed*Fps->GetFramesDiff());
        // Выполнить скроллинг
        if(MovedX<25) Cam->ScrollLeft();
        if(MovedX>ScreenWidth-25) Cam->ScrollRight();
        if(MovedY<25) Cam->ScrollUp();
        if(MovedY>ScreenHeight-25) Cam->ScrollDown();
        Cam->UpdateWorld();
        // Для всех объектов сцены проверить видимость в новом положении камеры
        if(Sector!=NULL) Sector->CheckVisibility();     // Сектор
        for(int i=0; i<List3D->Count; i++) {
                if(dynamic_cast<Object3DUnit*>((Object*)(List3D->Items[i]))!=NULL) ((Object3DUnit*)(List3D->Items[i]))->CheckVisibility();
                }
        // Т.к. это скроллинг - скроллирование повторять, пока мышка не уйдет из области прокрутки
        return Rez;
        }
        // Если это не скроллинг - обработка движения курсора закончена
MouseMoved = false;
MovedX = 0;
MovedY = 0;
return Rez;
}
//---------------------------------------------------------------------------
Object* TForm1::ProcessCursorClick()
{
        // Нахождение объекта по которому пользователь кликнул мышкой.
        // Возвращает объект (NULL - фон).
        // получить индекс объекта
Object* Rez = FindObjectByRecasting(ClickedX,ClickedY);
        // Клик мышкой обработан
MouseClicked = false;
ClickedX = 0;
ClickedY = 0;
return Rez;
}
//---------------------------------------------------------------------------
Object* TForm1::FindObjectByRecasting(int CoordX, int CoordY)
{
        // Нахождение объекта используя принцп Recasting'a. NULL - ничего не
        // выбрано (фон)
        // Получаем луч из камеры в точку с координатами CoordX,CoordY, луч описываем 2 вкторами в конечные точки
D3DXVECTOR3 StartPoint;
D3DXVECTOR3 EndPoint;
float FeelingLength = 200.0;    // Чувствительность - на каком расстоянии от камеры объекты будут выбираться
if(PerspectiveProj==true) {
                // Начальная точка - положение камеры
        StartPoint = Cam->GetPosition();
                // Конечную точку вычисляем - для этого делаем преобразование Screen Space -> Viewport Space -> View Space -> World Space
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
        D3DXVec3TransformCoord(&EndPoint,&EndPoint,&TransformMatrix);   // Получили EndPoint
        }
else {
                // Конечную точку вычисляем - для этого делаем преобразование Screen Space -> Viewport Space -> View Space -> World Space
                // Screen Space -> Viev Space т.к. все точки параллельно проэцируются на экран, нужно только учитывать установленные размеры экрана
        EndPoint.x = (((float)CoordX/2.0 - (float)ScreenWidth/4.0)/100.0)/4.0;  // Перевод в размеры области, отображаемой на экране
        EndPoint.y = (((float)ScreenHeight/4.0 - (float)CoordY/2.0)/100.0)/4.0; // Перевод в размеры области, отображаемой на экране
        EndPoint.z = 1.0;
        EndPoint *= FeelingLength;
                // View Space -> World Space
        D3DXMATRIX TransformMatrix;
        pD3DDevice->GetTransform(D3DTS_VIEW,(D3DMATRIX*)&TransformMatrix);
        D3DXMatrixInverse(&TransformMatrix,NULL,&TransformMatrix);
        D3DXVec3TransformCoord(&EndPoint,&EndPoint,&TransformMatrix);   // Получили EndPoint
                // Начальная точка - положение камеры + X и Y одинаковые с EndPoint т.к. параллельное проецирование точек на экран
        StartPoint.z = (Cam->GetPosition()).z;
        StartPoint.x = EndPoint.x;
        StartPoint.y = EndPoint.y;
        }
        // Проверяем все объекты на пересечение с лучем и находим ближайший
float Distance = FeelingLength;
Object* Rez = NULL;
        // Сектор
float TmpDistance = -1.0;
bool TmpRez;
if(Sector!=NULL) {
        TmpRez = Sector->CheckIntersectionWithLine(StartPoint,EndPoint,&TmpDistance,Rez);
        if(TmpRez==true) Distance = TmpDistance;
        }
        // По всем объектам
for(int i=0; i<List3D->Count; i++) {
        // Проверить пересечение отрезка с объектом
        if(((Object*)List3D->Items[i])->Checkable==true) {
                TmpDistance  = -1.0;
                TmpRez = ((Object3DUnit*)List3D->Items[i])->CheckIntersectionWithLine(StartPoint,EndPoint,&TmpDistance);
                if(TmpRez == true && TmpDistance <= Distance) { // Если точка пересечения ближе чем была раньше - берем этот объект (он выше)
                        Distance = TmpDistance;
                        Rez = (Object*)List3D->Items[i];
                        }
                }
        }
return Rez;
}
//---------------------------------------------------------------------------

