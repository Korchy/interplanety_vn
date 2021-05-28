//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "D:\Programms\Cpp\include\Options\Options.h"   // ��� ������� "���������"
#include "D:\Programms\Cpp\include\Log\LogFile.h"       // ��� ������� "Log-����"
#include "Unit2.h"      // ����� ��������� ������� � �������
#include "d3d8.h"       // ��� ������ � DirectX 8.1
#include "mmsystem.h"   // ��� ������������� �-��� timeGetTime()
#include "math.h"       // ��� ������������� ������������������ ������� (sin,tan � �.�.)

#include "Object3D.h"   // ���������� �������
#include "ObjectText.h" // �����

#include "ObjectSector.h"       // ������ "������"
#include "Object3DStar.h"       // ������ "������"
#include "Object3DContainer.h"  // ������ "���������"

#include "Camera.h"     // ������
#include "FpsCounter.h" // ������� FPS
//---------------------------------------------------------------------------
#define WM_START WM_USER    // ��������� ��������� WM_START, ����������� ������� ������� ����
//---------------------------------------------------------------------------
// Form1 - ����� ��������� ���� ����������. ���������� � ��� ���� ��������
// �������� ����.
// � Form1->Tag ��������� ����� 0 - �������, 1 - ������������� (800�600�32 60��)
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
        // ����������
        Options* Settings;              // "���������"
        LPDIRECT3D8 pD3D;               // ������� ������ Direct3d
        LPDIRECT3DDEVICE8 pD3DDevice;   // ���������� ����������
        D3DPRESENT_PARAMETERS ParamStruc;      // ��������� � �����������, ������������ ��� �������� ���������� ����������
        D3DXMATRIX MatWorld;            // ������� ����� ("������� ����")
        D3DXMATRIX MatView;             // ������� ���� (� ����� ����� ����������� ������ � ���� ��� ����������)
        D3DXMATRIX MatProj;             // ������� �������� (������������� ����� �� ��������� ������)
        D3DLIGHT8 Light1;               // 1 �������� ������������ ��������� (������ ������ ���� �����)
        D3DLIGHT8 Light2;               // 2 �������� ������������ ��������� (����� ������� ���� �����)

        unsigned int Ambient;           // �������� ����������� ��������� (�����)
        unsigned int FonColor;          // ���� ����
        unsigned short BackBufferCount; // ���������� BackBuffer ��� ���������� ����������
        int ScreenWidth;
        int ScreenHeight;               // ������ � ������ ������� ������� (� �������������� BackBuffer'a)

        bool PerspectiveProj;           // ��� �������������: true - �������������, false - �������������

        Camera* Cam;                    // ������
        float CamSpeed;                 // �������� ���������� ������

        FpsCounter* Fps;                // ������� FPS

        bool GameCicleRun;              // ���������� ���������� true - �������� ������� ���� ����, false - ���� �����

        bool MouseClicked;              // ���������� ��� ������������ � ��������� ������� ������ (����� ���������� - ����� �� �������� �� �������� �����), true - ��� ������ �������, ����� ��� ����������
        int ClickedX;                   // X,Y ���������� ������� � ������ ����� ������������
        int ClickedY;
        bool MouseMoved;                // ���������� ��� ������������ � ��������� �������� ����� (����� ���������� - ����� �� �������� �� �������� �����), true - ������ ������������ �� ����� �����, false - ������ ������� �� �����, �� ������������
        int MovedX;                     // X,Y ���������� ������� ��� �������� ��� �����������
        int MovedY;

        // �������
        bool InitDirect3D();            // ������������� Direct3D
        bool SetDeviceParams();         // ��������� �������������� ���������� ���������� ����������
        bool DeleteDirect3D();          // �������� Direct3D

        bool Render();                  // ����� ����� Direct3D �� �����
        bool CreateScene();             // �������� ����� (�������� ���� ��������, � �������� �������� �������������� ������)
        bool UpdateScene();             // ���������� ����� (��������� �������������� �������� � ����� � �.�.)

        void GameCicle(TMessage &Message);      // �������� ���� ����

        Object* ProcessCursorMoving();    // ��������� ����������� ������� �� ����� �����. ���������� ��������� �� ������, ���� ������ ����� �� ����
        Object* ProcessCursorClick();     // ���������� ������� �� �������� ������������ ������� ������. ���������� ��������� �� ������

        Object* FindObjectByRecasting(int CoordX, int CoordY);  // ���������� ������ ������� � ����� � ������������ CoordX,CoordY ��������� ������� Recasting'a

        // ��������� MESSAGES
        BEGIN_MESSAGE_MAP
                MESSAGE_HANDLER(WM_START, TMessage, GameCicle);         // ��������� ��������� WM_START
        END_MESSAGE_MAP(TForm);

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        // ����������
        LogFile* Log;                   // ��� ��� ������ ����������, ������ � �.�.
        TList* List3D;                  // ������ ���� �������� � �����

        ObjectSector* Sector;           // ������

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 