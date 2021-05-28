//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Scene.h"
//---------------------------------------------------------------------------
Scene::Scene(void)
{
	// ����������� ������
SBox = NULL;
Created = false;
Version = 0;
Name = "";
Planets.clear();
Satellits.clear();
Portals.clear();
Bases.clear();
}
//---------------------------------------------------------------------------
Scene::~Scene(void)
{
	// ���������� ������
	// �������� �����
if(SBox!=NULL) delete SBox;
	// �������
if(!Planets.empty()) {
	for(int i=0; i<(int)Planets.size(); i++) {
		if(Planets[i]!=NULL) delete Planets[i];
	}
}
	// ��������
if(!Satellits.empty()) {
	for(int i=0; i<(int)Satellits.size(); i++) {
		if(Satellits[i]!=NULL) delete Satellits[i];
	}
}
	// �������
if(!Portals.empty()) {
	for(int i=0; i<(int)Portals.size(); i++) {
		if(Portals[i]!=NULL) delete Portals[i];
	}
}
	// ����
if(!Bases.empty()) {
	for(int i=0; i<(int)Bases.size(); i++) {
		if(Bases[i]!=NULL) delete Bases[i];
	}
}
}
//---------------------------------------------------------------------------
//                    ��������� �������� ����� �� �����
//---------------------------------------------------------------------------
bool Scene::Draw()
{
	// ����� �������� ������� � �����
if(Created==false) return false;
	// SkyBox
if(SBox!=NULL) SBox->Draw();
	// �������
if(!Planets.empty()) {
	for(int i=0; i<(int)Planets.size(); i++) {
		if(Planets[i]!=NULL) (Planets[i])->Draw();
	}
}
	// ��������
if(!Satellits.empty()) {
	for(int i=0; i<(int)Satellits.size(); i++) {
		if(Satellits[i]!=NULL) (Satellits[i])->Draw();
	}
}
	// �������
if(!Portals.empty()) {
	for(int i=0; i<(int)Portals.size(); i++) {
		if(Portals[i]!=NULL) (Portals[i])->Draw();
	}
}
	// ����
if(!Bases.empty()) {
	for(int i=0; i<(int)Bases.size(); i++) {
		if(Bases[i]!=NULL) (Bases[i])->Draw();
	}
}
return true;
}
//---------------------------------------------------------------------------
//                              ����� �������
//---------------------------------------------------------------------------
bool Scene::LoadFromFile(std::string File)
{
        // �������� ������ � ����� �� �����
        // �������� ������ ������� �� ���������� �������
FILE *F;
if(fopen_s(&F,(BasePath+File).c_str(),"rb")==NULL) {
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
else {
	// ���� � ������� �� ������� �� ������, ��� ��������� - ������ ������� ���� ������� �� ��������� �������
//	MessageBox(NULL,"ERRR","",NULL);

}
	// �������� ������ �� �������
rewind(F);
struct SectorHeaderStruc {	// ��������� ��������� �������
	int Version;			// ����� ������ ������� - ����� ���������, �����-�� ��������� ���� �� ������ �������
	char Name[50];			// �������
} SectorHeader;
fread(&SectorHeader,sizeof(SectorHeader),1,F);
Version = SectorHeader.Version;
Name = std::string(SectorHeader.Name);
	// �������� ������ �� ��������� �������� �������
	// ����� ������ �� ��������
struct PlanetsFatStruc {	// ��������� �������
	char Count;				// ���������� ������
} PlanetsFat;
struct SatellitsFatStruc {	// ��������� ��������
	char Count;				// ���������� ���������
} SatellitsFat;
struct PortalsFatStruc {	// ��������� �������
	char Count;				// ���������� ��������
} PortalsFat;
struct BasesFatStruc {	// ��������� ����
	char Count;			// ���������� ���
} BasesFat;
fread(&PlanetsFat,sizeof(PlanetsFat),1,F);
fread(&SatellitsFat,sizeof(SatellitsFat),1,F);
fread(&PortalsFat,sizeof(PortalsFat),1,F);
fread(&BasesFat,sizeof(BasesFat),1,F);
	// �������� ������ ��������� �� ������� �������
	// ������
if(PlanetsFat.Count>0) {
	struct PlanetStruc {	// ���������, ����������� �������
		float X;			// ���������
		float Y;
		float Z;
		float RotateSpeed;	// �������� �������� ������ ����������� ��� ����/��.�������
		char Name[50];		// ��������
		char File[250];		// ���� � ����� � �������
	} PlanetData;
	for(int i=0; i<PlanetsFat.Count; i++) {
		Planet *TmpPlanet = new Planet();
		fread(&PlanetData,sizeof(PlanetData),1,F);
		TmpPlanet->SetRenderDevice(pD3DDevice);
		TmpPlanet->SetFile(std::string(PlanetData.File));
		TmpPlanet->LoadFromFile(std::string(PlanetData.File));
		TmpPlanet->SetFrustum(pFrustum);
		TmpPlanet->UpdateTransform();
		TmpPlanet->SetPosition(PlanetData.X,PlanetData.Y,PlanetData.Z);
		TmpPlanet->SetRotateSpeed(PlanetData.RotateSpeed);
		TmpPlanet->SetName(std::string(PlanetData.Name));
		Planets.push_back(TmpPlanet);
	}
}
	// ��������
if(SatellitsFat.Count>0) {
	struct SatellitStruc {		// ���������, ����������� ���� �������
		float BigRadius;
		float SmallRadius;      // 2 �������, ���������� ������ �������� (������)
		float RotateSpeed;      // �������� �������� ������ ����������� ��� ����/��.�������
		float MoveSpeed;        // �������� �������� �� ������ ����/��.�������
		char Name[50];          // ��������
		char File[250];         // ���� � ����� � �������
	}SatelliteData;
	for(int i=0; i<SatellitsFat.Count; i++) {
		Satellite *TmpSatellite = new Satellite();
		fread(&SatelliteData,sizeof(SatelliteData),1,F);
		TmpSatellite->SetRenderDevice(pD3DDevice);
		TmpSatellite->SetFile(std::string(SatelliteData.File));
		TmpSatellite->LoadFromFile(std::string(SatelliteData.File));
		TmpSatellite->SetFrustum(pFrustum);
		TmpSatellite->UpdateTransform();
		TmpSatellite->SetRadius(SatelliteData.BigRadius,SatelliteData.SmallRadius);
		TmpSatellite->SetRotateSpeed(SatelliteData.RotateSpeed);
		TmpSatellite->SetMoveSpeed(SatelliteData.MoveSpeed);
		TmpSatellite->SetName(std::string(SatelliteData.Name));
		Satellits.push_back(TmpSatellite);
	}
}
	// �������
if(PortalsFat.Count>0) {
	struct PortalStruc {	// ���������, ����������� ������
		float BigRadius;
		float SmallRadius;	// 2 �������, ���������� ������ ������� (������)
		float MoveSpeed;	// �������� �������� �� ������ ����/��.�������
		char Name[50];		// ��������
		char Dest[10];		// ������, � ������� ����� ������
		char File[250];		// ���� � ����� � �������
	}PortalData;
	for(int i=0; i<PortalsFat.Count; i++) {
		Portal* TmpPortal = new Portal();
		fread(&PortalData,sizeof(PortalData),1,F);
		TmpPortal->SetRenderDevice(pD3DDevice);
		TmpPortal->SetFile(std::string(PortalData.File));
		TmpPortal->LoadFromFile(std::string(PortalData.File));
		TmpPortal->SetFrustum(pFrustum);
		TmpPortal->UpdateTransform();
		TmpPortal->SetRadius(PortalData.BigRadius,PortalData.SmallRadius);
		TmpPortal->SetMoveSpeed(PortalData.MoveSpeed);
		TmpPortal->SetName(std::string(PortalData.Name));
		TmpPortal->SetDest(std::string(PortalData.Dest));
		Portals.push_back(TmpPortal);
	}
}
	// ����
if(BasesFat.Count>0) {
	struct BaseStruc {		// ���������, ����������� ����
		float BigRadius;
		float SmallRadius;	// 2 �������, ���������� ������ ������� (������)
		float RotateSpeed;	// �������� �������� ������ ����������� ��� ����/��.�������
		float MoveSpeed;	// �������� �������� �� ������ ����/��.�������
		char Name[50];		// ��������
		char File[250];		// ���� � ����� � �������
	}BaseData;
	for(int i=0; i<BasesFat.Count; i++) {
		Base* TmpBase = new Base();
		fread(&BaseData,sizeof(BaseData),1,F);
		TmpBase->SetRenderDevice(pD3DDevice);
		TmpBase->SetFile(std::string(BaseData.File));
		TmpBase->LoadFromFile(std::string(BaseData.File));
		TmpBase->SetFrustum(pFrustum);
		TmpBase->UpdateTransform();
		TmpBase->SetRadius(BaseData.BigRadius,BaseData.SmallRadius);
		TmpBase->SetRotateSpeed(BaseData.RotateSpeed);
		TmpBase->SetMoveSpeed(BaseData.MoveSpeed);
		TmpBase->SetName(std::string(BaseData.Name));
		Bases.push_back(TmpBase);
	}
}
fclose(F);

SBox = new SkyBox();
SBox->SetRenderDevice(pD3DDevice);
SBox->LoadFromFile("s1\\SkyBox.x");
SBox->UpdateTransform();

Created = true;
Visible = true;
return true;
}
//---------------------------------------------------------------------------
bool Scene::CheckVisibility()
{
	// �������� ��������� �� ���� �������� ������� �.�. ���������
	// ��������-�� ������� ������� � �������� ��������� Frustum ��� ���
	// ���� pFrustum = NULL - ������� ��� ��� ������� ������ �������� � �������� ���������
if(pFrustum==NULL) {
	if(!Planets.empty()) {
		for(int i=0; i<(int)Planets.size(); i++) {
			if(Planets[i]!=NULL) (Planets[i])->Visible = true;
		}
	}
	if(!Satellits.empty()) {
		for(int i=0; i<(int)Satellits.size(); i++) {
			if(Satellits[i]!=NULL) (Satellits[i])->Visible = true;
		}
	}
	if(!Portals.empty()) {
		for(int i=0; i<(int)Portals.size(); i++) {
			if(Portals[i]!=NULL) (Portals[i])->Visible = true;
		}
	}
	if(!Bases.empty()) {
		for(int i=0; i<(int)Bases.size(); i++) {
			if(Bases[i]!=NULL) (Bases[i])->Visible = true;
		}
	}
	return true;
}
	// �� ���� �������� ������� ��������� �� ��������� (�������� � Frustum)
if(!Planets.empty()) {
	for(int i=0; i<(int)Planets.size(); i++) {
		if(Planets[i]!=NULL) (Planets[i])->CheckVisibility();
	}
}
if(!Satellits.empty()) {
	for(int i=0; i<(int)Satellits.size(); i++) {
		if(Satellits[i]!=NULL) (Satellits[i])->CheckVisibility();
	}
}
if(!Portals.empty()) {
	for(int i=0; i<(int)Portals.size(); i++) {
		if(Portals[i]!=NULL) (Portals[i])->CheckVisibility();
	}
}
if(!Bases.empty()) {
	for(int i=0; i<(int)Bases.size(); i++) {
		if(Bases[i]!=NULL) (Bases[i])->CheckVisibility();
	}
}
return true;
}
//---------------------------------------------------------------------------
bool Scene::CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance, Object* &IntersectedObject)
{
        // �������� �� ����������� ������� ��������� ������� LineStart � LineEnd
        // ���� �� � ����� �� �������� �������, ���������� true ���� �����������
        // ����, false - ���, Distance,IntersectedObject - �������� ��������� =
        // ���������� �� ����� ����������� � ��������� �� ������ � �������
        // ��������� �����������
if(Created==false) return false;
float TmpDistance = -1.0;
bool Rez = false;
	// �������� ������� �� ������
if(!Planets.empty()) {
	for(int i=0; i<(int)Planets.size(); i++) {
		if(Planets[i]!=NULL) {
			Rez = (Planets[i])->CheckIntersectionWithLine(LineStart,LineEnd,&TmpDistance);
			if(Rez==true && TmpDistance<*Distance) {
				IntersectedObject = (Object3DBo*)Planets[i];
				*Distance = TmpDistance;
			}
		}
	}
}
	// �������� �������� �� ������
if(!Satellits.empty()) {
	for(int i=0; i<(int)Satellits.size(); i++) {
		if(Satellits[i]!=NULL) {
			Rez = (Satellits[i])->CheckIntersectionWithLine(LineStart,LineEnd,&TmpDistance);
			if(Rez==true && TmpDistance<*Distance) {
				IntersectedObject = (Object3DBo*)Satellits[i];
				*Distance = TmpDistance;
			}
		}
	}
}
	// �������� ������� �� ������
if(!Portals.empty()) {
	for(int i=0; i<(int)Portals.size(); i++) {
		if(Portals[i]!=NULL) {
			Rez = (Portals[i])->CheckIntersectionWithLine(LineStart,LineEnd,&TmpDistance);
			if(Rez==true && TmpDistance<*Distance) {
				IntersectedObject = (Object3DBo*)Portals[i];
				*Distance = TmpDistance;
			}
		}
	}
}
	// �������� ���� �� ������
if(!Bases.empty()) {
	for(int i=0; i<(int)Bases.size(); i++) {
		if(Bases[i]!=NULL) {
			Rez = (Bases[i])->CheckIntersectionWithLine(LineStart,LineEnd,&TmpDistance);
			if(Rez==true && TmpDistance<*Distance) {
				IntersectedObject = (Object3DBo*)Bases[i];
				*Distance = TmpDistance;
			}
		}
	}
}
return Rez;
}
//---------------------------------------------------------------------------