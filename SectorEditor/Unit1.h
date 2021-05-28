//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <stdio.h>      // Для чтения-записи файлов
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *N4;
        TMenuItem *N5;
        TToolBar *ToolBar;
        TToolButton *ToolButton1;
        TMenuItem *N6;
        TBitBtn *NewButton;
        TBitBtn *OpenButton;
        TBitBtn *SaveButton;
        TToolButton *ToolButton2;
        TPageControl *PageControl;
        TTabSheet *CommonPage;
        TTabSheet *StarsPage;
        TTabSheet *PlanetsPage;
        TTabSheet *PoratlsPage;
        TLabel *Label1;
        TEdit *VersionEdit;
        TLabel *Label2;
        TEdit *NameEdit;
        TComboBox *StarsComboBox;
        TBitBtn *AddStarButton;
        TBitBtn *DelStarButton;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *StarX;
        TEdit *StarY;
        TEdit *StarZ;
        TLabel *Label7;
        TEdit *StarV;
        TLabel *Label8;
        TEdit *StarName;
        TEdit *StarPath;
        TLabel *Label9;
        TComboBox *PlanetsComboBox;
        TBitBtn *AddPlanetButton;
        TBitBtn *DelPlanetButton;
        TLabel *Label10;
        TEdit *PlanetName;
        TLabel *Label11;
        TLabel *Label12;
        TEdit *PlanetR1;
        TLabel *Label13;
        TEdit *PlanetR2;
        TLabel *Label14;
        TEdit *PlanetV;
        TLabel *Label15;
        TEdit *PlanetPath;
        TLabel *Label16;
        TEdit *PlanetV1;
        TComboBox *PortalsComboBox;
        TBitBtn *AddPortalButton;
        TBitBtn *DelPortalButton;
        TLabel *Label17;
        TEdit *PortalName;
        TLabel *Label18;
        TLabel *Label19;
        TEdit *PortalR1;
        TLabel *Label20;
        TEdit *PortalR2;
        TLabel *Label21;
        TEdit *PortalV1;
        TLabel *Label22;
        TEdit *PortalPath;
        TLabel *Label23;
        TEdit *PortalDest;
        TOpenDialog *OpenDialog;
        TSaveDialog *SaveDialog;
        TTabSheet *BasesPage;
        TComboBox *BasesComboBox;
        TBitBtn *AddBaseButton;
        TBitBtn *DelBaseButton;
        TLabel *Label24;
        TEdit *BaseName;
        TLabel *Label25;
        TLabel *Label26;
        TEdit *BaseR1;
        TLabel *Label27;
        TEdit *BaseR2;
        TLabel *Label28;
        TEdit *BaseV;
        TLabel *Label29;
        TEdit *BaseV1;
        TLabel *Label30;
        TEdit *BasePath;
        void __fastcall N4Click(TObject *Sender);
        void __fastcall N2Click(TObject *Sender);
        void __fastcall N5Click(TObject *Sender);
        void __fastcall N6Click(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall AddStarButtonClick(TObject *Sender);
        void __fastcall DelStarButtonClick(TObject *Sender);
        void __fastcall AddPlanetButtonClick(TObject *Sender);
        void __fastcall DelPlanetButtonClick(TObject *Sender);
        void __fastcall AddPortalButtonClick(TObject *Sender);
        void __fastcall DelPortalButtonClick(TObject *Sender);
        void __fastcall StarsComboBoxClick(TObject *Sender);
        void __fastcall StarsComboBoxDropDown(TObject *Sender);
        void __fastcall PlanetsComboBoxDropDown(TObject *Sender);
        void __fastcall PlanetsComboBoxClick(TObject *Sender);
        void __fastcall PortalsComboBoxDropDown(TObject *Sender);
        void __fastcall PortalsComboBoxClick(TObject *Sender);
        void __fastcall AddBaseButtonClick(TObject *Sender);
        void __fastcall DelBaseButtonClick(TObject *Sender);
        void __fastcall BasesComboBoxClick(TObject *Sender);
        void __fastcall BasesComboBoxDropDown(TObject *Sender);
private:	// User declarations
public:		// User declarations
        // Переменные
        struct SectorHeaderStruc {      // Структура заголовка сектора
                int Version;            // Номер версии сектора - чтобы проверять, нужно-ли обновлять файл на машине клиента
                char Name[50];          // Назвние
                } *SectorHeader;

        struct StarsFatStruc {          // Структура указаний на структуры, описывающие звезды в секторе
                char Count;             // Количество звезд
                } *StarsFat;
        struct PlanetsFatStruc {        // Структура указаний на структуры, описывающие спутники в секторе
                char Count;             // Количество спутников
                } *PlanetsFat;
        struct PortalsFatStruc {        // Структура указаний на структуры, описывающие порталы в секторе
                char Count;             // Количество порталов
                } *PortalsFat;
        struct BasesFatStruc {          // Структура указаний на структуры, описывающие базы в секторе
                char Count;             // Количество баз
                } *BasesFat;

        struct StarStruc {              // Структура, описывающая звезду
                float X;
                float Y;
                float Z;                // Положение
                float RotateSpeed;      // Скорость вращения вокруг собственной оси углы/ед.времени
                char Name[50];          // Название
                char File[250];         // Путь к файлу с моделью
                };
        TList* Stars;                   // Список указателей на структуры StarStruc

        struct PlanetStruc {            // Структура, описывающая один спутник
                float BigRadius;
                float SmallRadius;      // 2 радиуса, описывющие орбиту спутника (эллипс)
                float RotateSpeed;      // Скорость вращения вокруг собственной оси углы/ед.времени
                float MoveSpeed;        // Скорость движения по орбите углы/ед.времени
                char Name[50];          // Название
                char File[250];         // Путь к файлу с моделью
                };
        TList* Planets;                 // Список указателей на структуры PlanetStruc

        struct PortalStruc {            // Структура, описывающая портал
                float BigRadius;
                float SmallRadius;      // 2 радиуса, описывющие орбиту портала (эллипс)
                float MoveSpeed;        // Скорость движения по орбите углы/ед.времени
                char Name[50];          // Название
                char Dest[10];          // Сектор, в который ведет портал
                char File[250];         // Путь к файлу с моделью
                };
        TList* Portals;                 //  Список указателей на структуры PortalStruc

        struct BaseStruc {              // Структура, описывающая базу
                float BigRadius;
                float SmallRadius;      // 2 радиуса, описывющие орбиту портала (эллипс)
                float RotateSpeed;      // Скорость вращения вокруг собственной оси углы/ед.времени
                float MoveSpeed;        // Скорость движения по орбите углы/ед.времени
                char Name[50];          // Название
                char File[250];         // Путь к файлу с моделью
                };
        TList* Bases;                   //  Список указателей на структуры BaseStruc

        // Функции

        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
