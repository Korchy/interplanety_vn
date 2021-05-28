//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "D:\Programms\Cpp\include\Options\Options.h"   // Для объекта "Настройки"
#include "D:\Programms\Cpp\include\Log\LogFile.h"       // Для объекта "Log-файл"
#include "Unit2.h"      // Форма активации клиента в системе
#include "d3d8.h"       // Для работы с DirectX 8.1
#include "mmsystem.h"   // Для использования ф-ции timeGetTime()
#include "math.h"       // Для использования тригонометрических функций (sin,tan и т.п.)

#include "Object3D.h"   // Трехмерные объекты
#include "ObjectText.h" // Текст

#include "ObjectSector.h"       // Объект "Сектор"
#include "Object3DStar.h"       // Объект "Звезда"
#include "Object3DContainer.h"  // Объект "Контейнер"

#include "Camera.h"     // Камера
#include "FpsCounter.h" // Счетчик FPS
//---------------------------------------------------------------------------
#define WM_START WM_USER    // Назначить сообщение WM_START, запускающее главный игровой цикл
//---------------------------------------------------------------------------
// Form1 - Класс основного окна приложения. Собственно в нем идет основное
// действие игры.
// В Form1->Tag заносится режим 0 - оконный, 1 - полноэкранный (800х600х32 60Гц)
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
private:	// User declarations
        // Переменные
        Options* Settings;              // "Настройки"
        LPDIRECT3D8 pD3D;               // Главный объект Direct3d
        LPDIRECT3DDEVICE8 pD3DDevice;   // Устройство рендеринга
        D3DPRESENT_PARAMETERS ParamStruc;      // Структура с параметрами, необходимыми для создания устройства рендеринга
        D3DXMATRIX MatWorld;            // Матрица сцены ("матрица Мира")
        D3DXMATRIX MatView;             // Матрица вида (в какой точке расположена камера и куда она направлена)
        D3DXMATRIX MatProj;             // Матрица проекции (проецирование сцены на плоскость экрана)
        D3DLIGHT8 Light1;               // 1 источник равномерного освещения (правый нижний угол сцены)
        D3DLIGHT8 Light2;               // 2 источник равномерного освещения (левый верхний угол сцены)

        unsigned int Ambient;           // Величина рассеянного освещения (гамма)
        unsigned int FonColor;          // Цвет фона
        unsigned short BackBufferCount; // Количество BackBuffer для устройства рендеринга
        int ScreenWidth;
        int ScreenHeight;               // Высота и ширина рабочей области (и соответственно BackBuffer'a)

        bool PerspectiveProj;           // Тип проецирования: true - перспективное, false - ортогональное

        Camera* Cam;                    // Камера
        float CamSpeed;                 // Скорость скроллинга камеры

        FpsCounter* Fps;                // Счетчик FPS

        bool GameCicleRun;              // Глобальная переменная true - работает главный цикл игры, false - игра стоит

        bool MouseClicked;              // Переменная для отслеживания и обработки щелчков мышкой (через переменную - чтобы не выходить из игрового цикла), true - был щелчок мыншкой, нужно его обработать
        int ClickedX;                   // X,Y координаты курсора в момент клика пользователя
        int ClickedY;
        bool MouseMoved;                // Переменная для отслеживания и обработки движения мышки (через переменную - чтобы не выходить из игрового цикла), true - курсор переместился на новое место, false - курсор остался на месте, не обрабатываем
        int MovedX;                     // X,Y координаты курсора для фиксации его перемещений
        int MovedY;

        // Функции
        bool InitDirect3D();            // Инициализация Direct3D
        bool SetDeviceParams();         // Установка дополнительных параметров устройства рендеринга
        bool DeleteDirect3D();          // Удаление Direct3D

        bool Render();                  // Вывод сцены Direct3D на экран
        bool CreateScene();             // Создание сцены (создание всех объектов, с которыми возможны взаимодействия игрока)
        bool UpdateScene();             // Обновление сцены (изменение местоположения объектов в сцене и т.п.)

        void GameCicle(TMessage &Message);      // Основной цикл игры

        Object* ProcessCursorMoving();    // Обработка перемещения курсора на новое место. Возвращает указатель на объект, если курсор встал на него
        Object* ProcessCursorClick();     // Нахождение объекта по которому пользователь кликнул мышкой. Возвращает указатель на объект

        Object* FindObjectByRecasting(int CoordX, int CoordY);  // Определяет индекс объекта в точке с координатами CoordX,CoordY используя принцип Recasting'a

        // Сообщения MESSAGES
        BEGIN_MESSAGE_MAP
                MESSAGE_HANDLER(WM_START, TMessage, GameCicle);         // Обработка сообщения WM_START
        END_MESSAGE_MAP(TForm);

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        // Переменные
        LogFile* Log;                   // Лог для записи информации, ошибок и т.п.
        TList* List3D;                  // Список всех объектов в сцене

        ObjectSector* Sector;           // Сектор

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 