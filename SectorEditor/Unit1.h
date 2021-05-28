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
#include <stdio.h>      // ��� ������-������ ������
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
        // ����������
        struct SectorHeaderStruc {      // ��������� ��������� �������
                int Version;            // ����� ������ ������� - ����� ���������, �����-�� ��������� ���� �� ������ �������
                char Name[50];          // �������
                } *SectorHeader;

        struct StarsFatStruc {          // ��������� �������� �� ���������, ����������� ������ � �������
                char Count;             // ���������� �����
                } *StarsFat;
        struct PlanetsFatStruc {        // ��������� �������� �� ���������, ����������� �������� � �������
                char Count;             // ���������� ���������
                } *PlanetsFat;
        struct PortalsFatStruc {        // ��������� �������� �� ���������, ����������� ������� � �������
                char Count;             // ���������� ��������
                } *PortalsFat;
        struct BasesFatStruc {          // ��������� �������� �� ���������, ����������� ���� � �������
                char Count;             // ���������� ���
                } *BasesFat;

        struct StarStruc {              // ���������, ����������� ������
                float X;
                float Y;
                float Z;                // ���������
                float RotateSpeed;      // �������� �������� ������ ����������� ��� ����/��.�������
                char Name[50];          // ��������
                char File[250];         // ���� � ����� � �������
                };
        TList* Stars;                   // ������ ���������� �� ��������� StarStruc

        struct PlanetStruc {            // ���������, ����������� ���� �������
                float BigRadius;
                float SmallRadius;      // 2 �������, ���������� ������ �������� (������)
                float RotateSpeed;      // �������� �������� ������ ����������� ��� ����/��.�������
                float MoveSpeed;        // �������� �������� �� ������ ����/��.�������
                char Name[50];          // ��������
                char File[250];         // ���� � ����� � �������
                };
        TList* Planets;                 // ������ ���������� �� ��������� PlanetStruc

        struct PortalStruc {            // ���������, ����������� ������
                float BigRadius;
                float SmallRadius;      // 2 �������, ���������� ������ ������� (������)
                float MoveSpeed;        // �������� �������� �� ������ ����/��.�������
                char Name[50];          // ��������
                char Dest[10];          // ������, � ������� ����� ������
                char File[250];         // ���� � ����� � �������
                };
        TList* Portals;                 //  ������ ���������� �� ��������� PortalStruc

        struct BaseStruc {              // ���������, ����������� ����
                float BigRadius;
                float SmallRadius;      // 2 �������, ���������� ������ ������� (������)
                float RotateSpeed;      // �������� �������� ������ ����������� ��� ����/��.�������
                float MoveSpeed;        // �������� �������� �� ������ ����/��.�������
                char Name[50];          // ��������
                char File[250];         // ���� � ����� � �������
                };
        TList* Bases;                   //  ������ ���������� �� ��������� BaseStruc

        // �������

        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
