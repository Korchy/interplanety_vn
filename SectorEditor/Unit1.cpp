//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
SectorHeader = NULL;
StarsFat = NULL;
PlanetsFat = NULL;
PortalsFat = NULL;
BasesFat = NULL;
Stars = NULL;
Planets = NULL;
Portals = NULL;
Bases = NULL;
PageControl->Enabled = false;
}
//---------------------------------------------------------------------------
//                              ГЛАВНОЕ МЕНЮ
//---------------------------------------------------------------------------
void __fastcall TForm1::N4Click(TObject *Sender)
{
        // Выход
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N2Click(TObject *Sender)
{
        // Открыть сектор для редактирования
if(SectorHeader!=NULL) {
        if(Application->MessageBox("Сохранить изменения?","Сохранение",MB_YESNO)==IDYES) {
                Form1->N6Click(NULL);
                }
        delete SectorHeader;
        SectorHeader = NULL;
        delete StarsFat;
        StarsFat = NULL;
        delete PlanetsFat;
        PlanetsFat = NULL;
        delete PortalsFat;
        PortalsFat = NULL;
        for(int i=0; i<Stars->Count; i++) delete (StarStruc*)Stars->Items[i];
        delete Stars;
        Stars = NULL;
        for(int i=0; i<Planets->Count; i++) delete (PlanetStruc*)Planets->Items[i];
        delete Planets;
        Planets = NULL;
        for(int i=0; i<Portals->Count; i++) delete (PortalStruc*)Portals->Items[i];
        delete Portals;
        Portals = NULL;
        for(int i=0; i<Bases->Count; i++) delete (BaseStruc*)Bases->Items[i];
        delete Bases;
        Bases = NULL;
        }
        // Создать структуры под новый сектор
SectorHeader = new SectorHeaderStruc();
StarsFat = new StarsFatStruc();
PlanetsFat = new PlanetsFatStruc();
PortalsFat = new PortalsFatStruc();
BasesFat = new BasesFatStruc();
Stars = new TList();
Planets = new TList();
Portals = new TList();
Bases = new TList();
        // Получить имя файла
if(OpenDialog->Execute()) {
        SaveDialog->FileName = OpenDialog->FileName;
        // Загрузить сектор из файла
        FILE *F;
        if((F=fopen((OpenDialog->FileName).c_str(),"rb"))==NULL) {
                ShowMessage("Невозможно открыть файл!");
                delete SectorHeader;
                SectorHeader = NULL;
                delete StarsFat;
                StarsFat = NULL;
                delete PlanetsFat;
                PlanetsFat = NULL;
                delete PortalsFat;
                PortalsFat = NULL;
                delete BasesFat;
                BasesFat = NULL;
                delete Stars;
                Stars = NULL;
                delete Planets;
                Planets = NULL;
                delete Portals;
                Portals = NULL;
                delete Bases;
                Bases = NULL;
                return;
                }
        fread(SectorHeader,sizeof(*SectorHeader),1,F);
        fread(StarsFat,sizeof(*StarsFat),1,F);
        fread(PlanetsFat,sizeof(*PlanetsFat),1,F);
        fread(PortalsFat,sizeof(*PortalsFat),1,F);
        fread(BasesFat,sizeof(*BasesFat),1,F);
        for(int i=0;i<StarsFat->Count;i++) {
                StarStruc* NewStar = new StarStruc();
                ZeroMemory(NewStar, sizeof(*NewStar));
                fread(NewStar,sizeof(*NewStar),1,F);
                int Index = Stars->Add(NewStar);
                StarsComboBox->Items->AddObject("№"+IntToStr(Index),(TObject*)(Stars->Items[Index]));
                }
        for(int i=0;i<PlanetsFat->Count;i++) {
                PlanetStruc* NewPlanet = new PlanetStruc();
                ZeroMemory(NewPlanet, sizeof(*NewPlanet));
                fread(NewPlanet,sizeof(*NewPlanet),1,F);
                int Index = Planets->Add(NewPlanet);
                PlanetsComboBox->Items->AddObject("№"+IntToStr(Index),(TObject*)(Planets->Items[Index]));
                }
        for(int i=0;i<PortalsFat->Count;i++) {
                PortalStruc* NewPortal = new PortalStruc();
                ZeroMemory(NewPortal, sizeof(*NewPortal));
                fread(NewPortal,sizeof(*NewPortal),1,F);
                int Index = Portals->Add(NewPortal);
                PortalsComboBox->Items->AddObject("№"+IntToStr(Index),(TObject*)(Portals->Items[Index]));
                }
        for(int i=0;i<BasesFat->Count;i++) {
                BaseStruc* NewBase = new BaseStruc();
                ZeroMemory(NewBase, sizeof(*NewBase));
                fread(NewBase,sizeof(*NewBase),1,F);
                int Index = Bases->Add(NewBase);
                BasesComboBox->Items->AddObject("№"+IntToStr(Index),(TObject*)(Bases->Items[Index]));
                }
        fclose(F);
        // Номер версии увеличить на 1
        SectorHeader->Version++;
        // Обновить информацию на форме
        VersionEdit->Text = IntToStr(SectorHeader->Version);
        NameEdit->Text = AnsiString(SectorHeader->Name);
        if(StarsFat->Count!=0) {
                StarsComboBox->ItemIndex = 0;
                StarsComboBox->OnClick(NULL);
                }
        if(PlanetsFat->Count!=0) {
                PlanetsComboBox->ItemIndex = 0;
                PlanetsComboBox->OnClick(NULL);
                }
        if(PortalsFat->Count!=0) {
                PortalsComboBox->ItemIndex = 0;
                PortalsComboBox->OnClick(NULL);
                }
        if(BasesFat->Count!=0) {
                BasesComboBox->ItemIndex = 0;
                BasesComboBox->OnClick(NULL);
                }
        // Разрешить работать с сектором
        PageControl->Enabled = true;
        }
else {
        delete SectorHeader;
        SectorHeader = NULL;
        delete StarsFat;
        StarsFat = NULL;
        delete PlanetsFat;
        PlanetsFat = NULL;
        delete PortalsFat;
        PortalsFat = NULL;
        delete BasesFat;
        BasesFat = NULL;
        delete Stars;
        Stars = NULL;
        delete Planets;
        Planets = NULL;
        delete Portals;
        Portals = NULL;
        delete Bases;
        Bases = NULL;
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N5Click(TObject *Sender)
{
        // Создать новый сектор
        // Если есть открытый сектор - сохранить и закрыть
if(SectorHeader!=NULL) {
        if(Application->MessageBox("Сохранить изменения?","Сохранение",MB_YESNO)==IDYES) {
                Form1->N6Click(NULL);
                }
        delete SectorHeader;
        SectorHeader = NULL;
        delete StarsFat;
        StarsFat = NULL;
        delete PlanetsFat;
        PlanetsFat = NULL;
        delete PortalsFat;
        PortalsFat = NULL;
        delete BasesFat;
        BasesFat = NULL;
        for(int i=0; i<Stars->Count; i++) delete (StarStruc*)Stars->Items[i];
        delete Stars;
        Stars = NULL;
        for(int i=0; i<Planets->Count; i++) delete (PlanetStruc*)Planets->Items[i];
        delete Planets;
        Planets = NULL;
        for(int i=0; i<Portals->Count; i++) delete (PortalStruc*)Portals->Items[i];
        delete Portals;
        Portals = NULL;
        for(int i=0; i<Bases->Count; i++) delete (BaseStruc*)Bases->Items[i];
        delete Bases;
        Bases = NULL;
        }
        // Создать структуры под новый сектор
SectorHeader = new SectorHeaderStruc();
StarsFat = new StarsFatStruc();
PlanetsFat = new PlanetsFatStruc();
PortalsFat = new PortalsFatStruc();
BasesFat = new BasesFatStruc();
Stars = new TList();
Planets = new TList();
Portals = new TList();
Bases = new TList();
        // Разрешить работать с сектором
PageControl->Enabled = true;
        // Значения по умолчанию
VersionEdit->Text = "1";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N6Click(TObject *Sender)
{
        // Сохранить
if(SectorHeader!=NULL) {
        // Переместить данные с текущих открытых закладок формы в структуры (все невидимое уже в структурах)
        SectorHeader->Version = StrToInt(VersionEdit->Text);
        strcpy(SectorHeader->Name,(NameEdit->Text).c_str());
        StarsFat->Count = StarsComboBox->Items->Count;
        PlanetsFat->Count = PlanetsComboBox->Items->Count;
        PortalsFat->Count = PortalsComboBox->Items->Count;
        BasesFat->Count = BasesComboBox->Items->Count;
        StarsComboBox->OnDropDown(NULL);
        PlanetsComboBox->OnDropDown(NULL);
        PortalsComboBox->OnDropDown(NULL);
        BasesComboBox->OnDropDown(NULL);
        // Сохранить данные в файл
        // Получить имя файла
        if(SaveDialog->FileName=="") SaveDialog->Execute();
        if(SaveDialog->FileName=="") {
                ShowMessage("Не введено имя файла, сектор не записан!");
                return;
                }
        // Удалить старый файл
        if(FileExists(SaveDialog->FileName)) DeleteFile(SaveDialog->FileName);
        // Записать новый файл
        FILE *F;
        if((F=fopen((SaveDialog->FileName).c_str(),"wb"))==NULL) {
                ShowMessage("Нельзя создать файл, сектор не записан!");
                return;
                }
        fwrite(SectorHeader,sizeof(*SectorHeader),1,F);
        fwrite(StarsFat,sizeof(*StarsFat),1,F);
        fwrite(PlanetsFat,sizeof(*PlanetsFat),1,F);
        fwrite(PortalsFat,sizeof(*PortalsFat),1,F);
        fwrite(BasesFat,sizeof(*BasesFat),1,F);
        for(int i=0;i<StarsComboBox->Items->Count;i++) {
                StarStruc* TmpStar = (StarStruc*)(StarsComboBox->Items->Objects[i]);
                fwrite(TmpStar,sizeof(*TmpStar),1,F);
                }
        for(int i=0;i<PlanetsComboBox->Items->Count;i++) {
                PlanetStruc* TmpPlanet = (PlanetStruc*)(PlanetsComboBox->Items->Objects[i]);
                fwrite(TmpPlanet,sizeof(*TmpPlanet),1,F);
                }
        for(int i=0;i<PortalsComboBox->Items->Count;i++) {
                PortalStruc* TmpPortal = (PortalStruc*)(PortalsComboBox->Items->Objects[i]);
                fwrite(TmpPortal,sizeof(*TmpPortal),1,F);
                }
        for(int i=0;i<BasesComboBox->Items->Count;i++) {
                BaseStruc* TmpBase = (BaseStruc*)(BasesComboBox->Items->Objects[i]);
                fwrite(TmpBase,sizeof(*TmpBase),1,F);
                }
        fclose(F);
        }
}
//---------------------------------------------------------------------------
//                              СОБЫТИЯ ФОРМЫ
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
        // При закрытии формы
if(SectorHeader!=NULL) {
        if(Application->MessageBox("Сохранить изменения?","Сохранение",MB_YESNO)==IDYES) {
                Form1->N6Click(NULL);
                }
        delete SectorHeader;
        SectorHeader = NULL;
        delete StarsFat;
        StarsFat = NULL;
        delete PlanetsFat;
        PlanetsFat = NULL;
        delete PortalsFat;
        PortalsFat = NULL;
        delete BasesFat;
        BasesFat = NULL;
        for(int i=0; i<Stars->Count; i++) delete (StarStruc*)Stars->Items[i];
        delete Stars;
        Stars = NULL;
        for(int i=0; i<Planets->Count; i++) delete (PlanetStruc*)Planets->Items[i];
        delete Planets;
        Planets = NULL;
        for(int i=0; i<Portals->Count; i++) delete (PortalStruc*)Portals->Items[i];
        delete Portals;
        Portals = NULL;
        for(int i=0; i<Bases->Count; i++) delete (BaseStruc*)Bases->Items[i];
        delete Bases;
        Bases = NULL;
        }
}
//---------------------------------------------------------------------------
//                                   ЗВЕЗДЫ
//---------------------------------------------------------------------------
void __fastcall TForm1::AddStarButtonClick(TObject *Sender)
{
        // Добавить звезду
StarStruc* NewStar = new StarStruc();
ZeroMemory(NewStar, sizeof(*NewStar));
int Index = Stars->Add(NewStar);
StarsComboBox->Items->AddObject("№"+IntToStr(Index),(TObject*)(Stars->Items[Index]));
StarsComboBox->OnDropDown(NULL);
StarsComboBox->ItemIndex = Index;
StarsComboBox->OnClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DelStarButtonClick(TObject *Sender)
{
        // Удалить звезду
if(StarsComboBox->ItemIndex!=-1) {
        delete (StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]);
        Stars->Delete(StarsComboBox->ItemIndex);
        StarsComboBox->ItemIndex = StarsComboBox->ItemIndex - 1;
        StarsComboBox->Items->Delete(StarsComboBox->ItemIndex+1);
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::StarsComboBoxDropDown(TObject *Sender)
{
        // Начало выбора новой звезды из списка
        // Сохранить данные в текущую структуру
if(StarsComboBox->ItemIndex!=-1) {
        //ShowMessage(IntToStr(StarsComboBox->ItemIndex));
        ((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->X = StrToFloat(StarX->Text);
        ((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->Y = StrToFloat(StarY->Text);
        ((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->Z = StrToFloat(StarZ->Text);
        ((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->RotateSpeed = StrToFloat(StarV->Text);
        strcpy(((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->Name,(StarName->Text).c_str());
        strcpy(((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->File,(StarPath->Text).c_str());
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::StarsComboBoxClick(TObject *Sender)
{
        // Выбор звезды из списка
        // Загрузить данные из новой структуры
if(StarsComboBox->ItemIndex!=-1) {
        StarX->Text = FloatToStr(((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->X);
        StarY->Text = FloatToStr(((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->Y);
        StarZ->Text = FloatToStr(((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->Z);
        StarV->Text = FloatToStr(((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->RotateSpeed);
        StarName->Text = AnsiString(((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->Name);
        StarPath->Text = AnsiString(((StarStruc*)(StarsComboBox->Items->Objects[StarsComboBox->ItemIndex]))->File);
        }
}
//---------------------------------------------------------------------------
//                                  ПЛАНЕТЫ
//---------------------------------------------------------------------------
void __fastcall TForm1::AddPlanetButtonClick(TObject *Sender)
{
        // Добавить планету
PlanetStruc* NewPlanet = new PlanetStruc();
ZeroMemory(NewPlanet, sizeof(*NewPlanet));
int Index = Planets->Add(NewPlanet);
PlanetsComboBox->Items->AddObject("№"+IntToStr(Index),(TObject*)(Planets->Items[Index]));
PlanetsComboBox->OnDropDown(NULL);
PlanetsComboBox->ItemIndex = Index;
PlanetsComboBox->OnClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DelPlanetButtonClick(TObject *Sender)
{
        // Удалить планету
if(PlanetsComboBox->ItemIndex!=-1) {
        delete (PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]);
        Planets->Delete(PlanetsComboBox->ItemIndex);
        PlanetsComboBox->ItemIndex = PlanetsComboBox->ItemIndex - 1;
        PlanetsComboBox->Items->Delete(PlanetsComboBox->ItemIndex+1);
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PlanetsComboBoxDropDown(TObject *Sender)
{
        // Начало выбора новой планеты из списка
        // Сохранить данные в текущую структуру
if(PlanetsComboBox->ItemIndex!=-1) {
        ((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->BigRadius = StrToFloat(PlanetR1->Text);
        ((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->SmallRadius = StrToFloat(PlanetR2->Text);
        ((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->RotateSpeed = StrToFloat(PlanetV->Text);
        ((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->MoveSpeed = StrToFloat(PlanetV1->Text);
        strcpy(((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->Name,(PlanetName->Text).c_str());
        strcpy(((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->File,(PlanetPath->Text).c_str());
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PlanetsComboBoxClick(TObject *Sender)
{
        // Выбор планеты из списка
        // Загрузить данные из новой структуры
if(PlanetsComboBox->ItemIndex!=-1) {
        PlanetR1->Text = FloatToStr(((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->BigRadius);
        PlanetR2->Text = FloatToStr(((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->SmallRadius);
        PlanetV->Text = FloatToStr(((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->RotateSpeed);
        PlanetV1->Text = FloatToStr(((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->MoveSpeed);
        PlanetName->Text = AnsiString(((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->Name);
        PlanetPath->Text = AnsiString(((PlanetStruc*)(PlanetsComboBox->Items->Objects[PlanetsComboBox->ItemIndex]))->File);
        }
}
//---------------------------------------------------------------------------
//                                  ПОРТАЛЫ
//---------------------------------------------------------------------------
void __fastcall TForm1::AddPortalButtonClick(TObject *Sender)
{
        // Добавить портал
PortalStruc* NewPortal = new PortalStruc();
ZeroMemory(NewPortal, sizeof(*NewPortal));
int Index = Portals->Add(NewPortal);
PortalsComboBox->Items->AddObject("№"+IntToStr(Index),(TObject*)(Portals->Items[Index]));
PortalsComboBox->OnDropDown(NULL);
PortalsComboBox->ItemIndex = Index;
PortalsComboBox->OnClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DelPortalButtonClick(TObject *Sender)
{
        // Удалить портал
if(PortalsComboBox->ItemIndex!=-1) {
        delete (PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]);
        Portals->Delete(PortalsComboBox->ItemIndex);
        PortalsComboBox->ItemIndex = PortalsComboBox->ItemIndex - 1;
        PortalsComboBox->Items->Delete(PortalsComboBox->ItemIndex+1);
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PortalsComboBoxDropDown(TObject *Sender)
{
        // Начало выбора нового портала из списка
        // Сохранить данные в текущую структуру
if(PortalsComboBox->ItemIndex!=-1) {
        ((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->BigRadius = StrToFloat(PortalR1->Text);
        ((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->SmallRadius = StrToFloat(PortalR2->Text);
        ((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->MoveSpeed = StrToFloat(PortalV1->Text);
        strcpy(((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->Name,(PortalName->Text).c_str());
        strcpy(((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->Dest,(PortalDest->Text).c_str());
        strcpy(((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->File,(PortalPath->Text).c_str());
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PortalsComboBoxClick(TObject *Sender)
{
        // Выбор портала из списка
        // Загрузить данные из новой структуры
if(PortalsComboBox->ItemIndex!=-1) {
        PortalR1->Text = FloatToStr(((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->BigRadius);
        PortalR2->Text = FloatToStr(((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->SmallRadius);
        PortalV1->Text = FloatToStr(((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->MoveSpeed);
        PortalName->Text = AnsiString(((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->Name);
        PortalDest->Text = AnsiString(((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->Dest);
        PortalPath->Text = AnsiString(((PortalStruc*)(PortalsComboBox->Items->Objects[PortalsComboBox->ItemIndex]))->File);
        }
}
//---------------------------------------------------------------------------
//                                    БАЗЫ
//---------------------------------------------------------------------------
void __fastcall TForm1::AddBaseButtonClick(TObject *Sender)
{
        // Добавить базу
BaseStruc* NewBase = new BaseStruc();
ZeroMemory(NewBase, sizeof(*NewBase));
int Index = Bases->Add(NewBase);
BasesComboBox->Items->AddObject("№"+IntToStr(Index),(TObject*)(Bases->Items[Index]));
BasesComboBox->OnDropDown(NULL);
BasesComboBox->ItemIndex = Index;
BasesComboBox->OnClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DelBaseButtonClick(TObject *Sender)
{
        // Удалить базу
if(BasesComboBox->ItemIndex!=-1) {
        delete (BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]);
        Bases->Delete(BasesComboBox->ItemIndex);
        BasesComboBox->ItemIndex = BasesComboBox->ItemIndex - 1;
        BasesComboBox->Items->Delete(BasesComboBox->ItemIndex+1);
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BasesComboBoxDropDown(TObject *Sender)
{
        // Начало выбора новой базы из списка
        // Сохранить данные в текущую структуру
if(BasesComboBox->ItemIndex!=-1) {
        ((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->BigRadius = StrToFloat(BaseR1->Text);
        ((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->SmallRadius = StrToFloat(BaseR2->Text);
        ((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->RotateSpeed = StrToFloat(BaseV->Text);
        ((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->MoveSpeed = StrToFloat(BaseV1->Text);
        strcpy(((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->Name,(BaseName->Text).c_str());
        strcpy(((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->File,(BasePath->Text).c_str());
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BasesComboBoxClick(TObject *Sender)
{
        // Выбор базы из списка
        // Загрузить данные из новой структуры
if(BasesComboBox->ItemIndex!=-1) {
        BaseR1->Text = FloatToStr(((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->BigRadius);
        BaseR2->Text = FloatToStr(((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->SmallRadius);
        BaseV->Text = FloatToStr(((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->RotateSpeed);
        BaseV1->Text = FloatToStr(((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->MoveSpeed);
        BaseName->Text = AnsiString(((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->Name);
        BasePath->Text = AnsiString(((BaseStruc*)(BasesComboBox->Items->Objects[BasesComboBox->ItemIndex]))->File);
        }
}
//---------------------------------------------------------------------------

