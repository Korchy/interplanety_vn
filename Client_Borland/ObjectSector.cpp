//---------------------------------------------------------------------------
#pragma hdrstop

#include "ObjectSector.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
ObjectSector::ObjectSector(void)
{
        // ����������� ������
SkyBox = NULL;
Stars = NULL;
Planets = NULL;
Portals = NULL;
Bases = NULL;
Created = false;
Version = 0;
Name = "";
}
//---------------------------------------------------------------------------
ObjectSector::~ObjectSector()
{
        // ���������� ������
        // �������� �����
if(SkyBox!=NULL) delete SkyBox;
        // ������
if(Stars!=NULL) {
        for(int i=0; i<Stars->Count; i++) {
                if(Stars->Items[i]!=NULL) delete (Object*)Stars->Items[i];
                }
        delete Stars;
        }
        // �������
if(Planets!=NULL) {
        for(int i=0; i<Planets->Count; i++) {
                if(Planets->Items[i]!=NULL) delete (Object*)Planets->Items[i];
                }
        delete Planets;
        }
        // �������
if(Portals!=NULL) {
        for(int i=0; i<Portals->Count; i++) {
                if(Portals->Items[i]!=NULL) delete (Object*)Portals->Items[i];
                }
        delete Portals;
        }
        // ����
if(Bases!=NULL) {
        for(int i=0; i<Bases->Count; i++) {
                if(Bases->Items[i]!=NULL) delete (Object*)Bases->Items[i];
                }
        delete Bases;
        }
}
//---------------------------------------------------------------------------
//                    ��������� �������� ������� �� �����
//---------------------------------------------------------------------------
bool ObjectSector::Draw()
{
        // ����� �������� ������� � �����
if(Created==false) return false;
         // SkyBox
if(SkyBox!=NULL) SkyBox->Draw();
        // ������
if(Stars!=NULL) {
        for(int i=0; i<Stars->Count; i++) {
                if(Stars->Items[i]!=NULL) ((Object*)Stars->Items[i])->Draw();
                }
        }
        // �������
if(Planets!=NULL) {
        for(int i=0; i<Planets->Count; i++) {
                if(Planets->Items[i]!=NULL) ((Object*)Planets->Items[i])->Draw();
                }
        }
        // �������
if(Portals!=NULL) {
        for(int i=0; i<Portals->Count; i++) {
                if(Portals->Items[i]!=NULL) ((Object*)Portals->Items[i])->Draw();
                }
        }
        // ����
if(Bases!=NULL) {
        for(int i=0; i<Bases->Count; i++) {
                if(Bases->Items[i]!=NULL) ((Object*)Bases->Items[i])->Draw();
                }
        }
return true;
}
//---------------------------------------------------------------------------
//                              ����� �������
//---------------------------------------------------------------------------
bool ObjectSector::LoadFromFile(AnsiString File)
{
        // �������� ������ �� ������� �� �����
        // �������� ������ ������� �� ���������� �������
FILE *F;
if((F=fopen((BasePath+File).c_str(),"rb"))==NULL) {
        // ���� � ������� �� ������� �� ������, ��� ��������� - ������ ������� ���� ������� �� ��������� �������
//        MessageBox(NULL,"ERRR","",NULL);




        }
else {
        // ��������� �� ���������-�� ������ �� �������
        // ������� ������
        rewind(F);
        int CurrentVersion;
        fread(&CurrentVersion,sizeof(CurrentVersion),1,F);
        // �������� ������ � �������
        int RealVersion;


        
        // ���� ����� - ������ ������� ���� ������� �� ��������� �������
        if(RealVersion>CurrentVersion) {





                }
        }
        // �������� ������ �� �������
rewind(F);
struct SectorHeaderStruc {      // ��������� ��������� �������
        int Version;            // ����� ������ ������� - ����� ���������, �����-�� ��������� ���� �� ������ �������
        char Name[50];          // �������
        } SectorHeader;
fread(&SectorHeader,sizeof(SectorHeader),1,F);
Version = SectorHeader.Version;
Name = AnsiString(SectorHeader.Name);
        // �������� ������ �� ��������� �������� �������
        // ����� ������ �� ��������
struct StarsFatStruc {          // ��������� ������
        char Count;             // ���������� �����
        } StarsFat;
struct PlanetsFatStruc {        // ��������� �������
        char Count;             // ���������� ���������
        } PlanetsFat;
struct PortalsFatStruc {        // ��������� �������
        char Count;             // ���������� ��������
        } PortalsFat;
struct BasesFatStruc {          // ��������� ����
        char Count;             // ���������� ���
        } BasesFat;
fread(&StarsFat,sizeof(StarsFat),1,F);
fread(&PlanetsFat,sizeof(PlanetsFat),1,F);
fread(&PortalsFat,sizeof(PortalsFat),1,F);
fread(&BasesFat,sizeof(BasesFat),1,F);
        // �������� ������ ��������� �� ������� �������
        // ������
if(StarsFat.Count>0) {
        Stars = new TList();
        struct StarStruc {              // ���������, ����������� ������
                float X;
                float Y;
                float Z;                // ���������
                float RotateSpeed;      // �������� �������� ������ ����������� ��� ����/��.�������
                char Name[50];          // ��������
                char File[250];         // ���� � ����� � �������
                } StarData;
        for(int i=0; i<StarsFat.Count; i++) {
                Object3DStar *TmpStar = new Object3DStar();
                fread(&StarData,sizeof(StarData),1,F);
                TmpStar->SetRenderDevice(pD3DDevice);
                TmpStar->SetFile(AnsiString(StarData.File));
                TmpStar->LoadFromFile(AnsiString(StarData.File));
                TmpStar->SetFrustum(pFrustum);
                TmpStar->UpdateTransform();
                TmpStar->SetPosition(StarData.X,StarData.Y,StarData.Z);
                TmpStar->SetRotateSpeed(StarData.RotateSpeed);
                TmpStar->SetName(AnsiString(StarData.Name));
                Stars->Add(TmpStar);
                }
        }
        // �������
if(PlanetsFat.Count>0) {
        Planets = new TList();
        struct PlanetStruc {            // ���������, ����������� ���� �������
                float BigRadius;
                float SmallRadius;      // 2 �������, ���������� ������ �������� (������)
                float RotateSpeed;      // �������� �������� ������ ����������� ��� ����/��.�������
                float MoveSpeed;        // �������� �������� �� ������ ����/��.�������
                char Name[50];          // ��������
                char File[250];         // ���� � ����� � �������
                }PlanetData;
        for(int i=0; i<PlanetsFat.Count; i++) {
                Object3DPlanet *TmpPlanet = new Object3DPlanet();
                fread(&PlanetData,sizeof(PlanetData),1,F);
                TmpPlanet->SetRenderDevice(pD3DDevice);
                TmpPlanet->SetFile(AnsiString(PlanetData.File));
                TmpPlanet->LoadFromFile(AnsiString(PlanetData.File));
                TmpPlanet->SetFrustum(pFrustum);
                TmpPlanet->UpdateTransform();
                TmpPlanet->SetRadius(PlanetData.BigRadius,PlanetData.SmallRadius);
                TmpPlanet->SetRotateSpeed(PlanetData.RotateSpeed);
                TmpPlanet->SetMoveSpeed(PlanetData.MoveSpeed);
                TmpPlanet->SetName(AnsiString(PlanetData.Name));
                Planets->Add(TmpPlanet);
                }
        }
        // �������
if(PortalsFat.Count>0) {
        Portals = new TList();
        struct PortalStruc {            // ���������, ����������� ������
                float BigRadius;
                float SmallRadius;      // 2 �������, ���������� ������ ������� (������)
                float MoveSpeed;        // �������� �������� �� ������ ����/��.�������
                char Name[50];          // ��������
                char Dest[10];          // ������, � ������� ����� ������
                char File[250];         // ���� � ����� � �������
                }PortalData;
        for(int i=0; i<PortalsFat.Count; i++) {
                Object3DPortal *TmpPortal = new Object3DPortal();
                fread(&PortalData,sizeof(PortalData),1,F);
                TmpPortal->SetRenderDevice(pD3DDevice);
                TmpPortal->SetFile(AnsiString(PortalData.File));
                TmpPortal->LoadFromFile(AnsiString(PortalData.File));
                TmpPortal->SetFrustum(pFrustum);
                TmpPortal->UpdateTransform();
                TmpPortal->SetRadius(PortalData.BigRadius,PortalData.SmallRadius);
                TmpPortal->SetMoveSpeed(PortalData.MoveSpeed);
                TmpPortal->SetName(AnsiString(PortalData.Name));
                TmpPortal->SetDest(AnsiString(PortalData.Dest));
                Portals->Add(TmpPortal);
                }
        }
        // ����
if(BasesFat.Count>0) {
        Bases = new TList();
        struct BaseStruc {              // ���������, ����������� ����
                float BigRadius;
                float SmallRadius;      // 2 �������, ���������� ������ ������� (������)
                float RotateSpeed;      // �������� �������� ������ ����������� ��� ����/��.�������
                float MoveSpeed;        // �������� �������� �� ������ ����/��.�������
                char Name[50];          // ��������
                char File[250];         // ���� � ����� � �������
                }BaseData;
        for(int i=0; i<BasesFat.Count; i++) {
                Object3DBase *TmpBase = new Object3DBase();
                fread(&BaseData,sizeof(BaseData),1,F);
                TmpBase->SetRenderDevice(pD3DDevice);
                TmpBase->SetFile(AnsiString(BaseData.File));
                TmpBase->LoadFromFile(AnsiString(BaseData.File));
                TmpBase->SetFrustum(pFrustum);
                TmpBase->UpdateTransform();
                TmpBase->SetRadius(BaseData.BigRadius,BaseData.SmallRadius);
                TmpBase->SetRotateSpeed(BaseData.RotateSpeed);
                TmpBase->SetMoveSpeed(BaseData.MoveSpeed);
                TmpBase->SetName(AnsiString(BaseData.Name));
                Bases->Add(TmpBase);
                }
        }
fclose(F);

SkyBox = new ObjectSkyBox();
SkyBox->SetRenderDevice(pD3DDevice);
SkyBox->LoadFromFile("s1\\SkyBox.x");
SkyBox->UpdateTransform();

Created = true;
Visible = true;
return true;
}
//---------------------------------------------------------------------------
bool ObjectSector::CheckVisibility()
{
        // �������� ��������� �� ���� �������� ������� �.�. ���������
        // ��������-�� ������� ������� � �������� ��������� Frustum ��� ���
        // ���� pFrustum = NULL - ������� ��� ��� ������� ������ �������� � �������� ���������
if(pFrustum==NULL) {
        if(Stars!=NULL) {
                for(int i=0; i<Stars->Count; i++) {
                        if(Stars->Items[i]!=NULL) ((Object*)Stars->Items[i])->Visible = true;
                        }
                }
        if(Planets!=NULL) {
                for(int i=0; i<Planets->Count; i++) {
                        if(Planets->Items[i]!=NULL) ((Object*)Planets->Items[i])->Visible = true;
                        }
                }
        if(Portals!=NULL) {
                for(int i=0; i<Portals->Count; i++) {
                        if(Portals->Items[i]!=NULL) ((Object*)Portals->Items[i])->Visible = true;
                        }
                }
        if(Bases!=NULL) {
                for(int i=0; i<Bases->Count; i++) {
                        if(Bases->Items[i]!=NULL) ((Object*)Bases->Items[i])->Visible = true;
                        }
                }
        return true;
        }
        // �� ���� �������� ������� ��������� �� ��������� (�������� � Frustum)
if(Stars!=NULL) {
        for(int i=0; i<Stars->Count; i++) {
//                if(Stars->Items[i]!=NULL) ((Object*)Stars->Items[i])->Visible = true;
                if(Stars->Items[i]!=NULL) ((Object3D*)Stars->Items[i])->CheckVisibility();
                }
        }
if(Planets!=NULL) {
        for(int i=0; i<Planets->Count; i++) {
                if(Planets->Items[i]!=NULL) ((Object3D*)Planets->Items[i])->CheckVisibility();
                }
        }
if(Portals!=NULL) {
        for(int i=0; i<Portals->Count; i++) {
                if(Portals->Items[i]!=NULL) ((Object3D*)Portals->Items[i])->CheckVisibility();
                }
        }
if(Bases!=NULL) {
        for(int i=0; i<Bases->Count; i++) {
                if(Bases->Items[i]!=NULL) ((Object3D*)Bases->Items[i])->CheckVisibility();
                }
        }
return true;
}
//---------------------------------------------------------------------------
bool ObjectSector::CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance, Object* &IntersectedObject)
{
        // �������� �� ����������� ������� ��������� ������� LineStart � LineEnd
        // ���� �� � ����� �� �������� �������, ���������� true ���� �����������
        // ����, false - ���, Distance,IntersectedObject - �������� ��������� =
        // ���������� �� ����� ����������� � ��������� �� ������ � �������
        // ��������� �����������
if(Created==false) return false;
float TmpDistance = -1.0;
bool Rez = false;
/*
        // �������� ������
bool Rez = Sun->CheckIntersectionWithLine(LineStart,LineEnd,&TmpDistance);
if(Rez==true) {
        IntersectedObject = (Object*)Sun;
        *Distance = TmpDistance;
        }
*/
        // �������� ������ �� ������
if(Stars!=NULL) {
        for(int i=0; i<Stars->Count; i++) {
                if(Stars->Items[i]!=NULL) {
                        Rez = ((Object3DUnit*)Stars->Items[i])->CheckIntersectionWithLine(LineStart,LineEnd,&TmpDistance);
                        if(Rez==true && TmpDistance<*Distance) {
                                IntersectedObject = (Object3DUnit*)Stars->Items[i];
                                *Distance = TmpDistance;
                                }
                        }
                }
        }
        // �������� ������� �� ������
if(Planets!=NULL) {
        for(int i=0; i<Planets->Count; i++) {
                if(Planets->Items[i]!=NULL) {
                        Rez = ((Object3DUnit*)Planets->Items[i])->CheckIntersectionWithLine(LineStart,LineEnd,&TmpDistance);
                        if(Rez==true && TmpDistance<*Distance) {
                                IntersectedObject = (Object3DUnit*)Planets->Items[i];
                                *Distance = TmpDistance;
                                }
                        }
                }
        }
        // �������� ������� �� ������
if(Portals!=NULL) {
        for(int i=0; i<Portals->Count; i++) {
                if(Portals->Items[i]!=NULL) {
                        Rez = ((Object3DUnit*)Portals->Items[i])->CheckIntersectionWithLine(LineStart,LineEnd,&TmpDistance);
                        if(Rez==true && TmpDistance<*Distance) {
                                IntersectedObject = (Object3DUnit*)Portals->Items[i];
                                *Distance = TmpDistance;
                                }
                        }
                }
        }
        // �������� ���� �� ������
if(Bases!=NULL) {
        for(int i=0; i<Bases->Count; i++) {
                if(Bases->Items[i]!=NULL) {
                        Rez = ((Object3DUnit*)Bases->Items[i])->CheckIntersectionWithLine(LineStart,LineEnd,&TmpDistance);
                        if(Rez==true && TmpDistance<*Distance) {
                                IntersectedObject = (Object3DUnit*)Bases->Items[i];
                                *Distance = TmpDistance;
                                }
                        }
                }
        }
return Rez;
}
//---------------------------------------------------------------------------

