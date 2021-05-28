//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Scene.h"
//---------------------------------------------------------------------------
Scene::Scene(void)
{
	// Конструктор класса
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
	// Деструктор класса
	// Небесная сфера
if(SBox!=NULL) delete SBox;
	// Планеты
if(!Planets.empty()) {
	for(int i=0; i<(int)Planets.size(); i++) {
		if(Planets[i]!=NULL) delete Planets[i];
	}
}
	// Спутники
if(!Satellits.empty()) {
	for(int i=0; i<(int)Satellits.size(); i++) {
		if(Satellits[i]!=NULL) delete Satellits[i];
	}
}
	// Порталы
if(!Portals.empty()) {
	for(int i=0; i<(int)Portals.size(); i++) {
		if(Portals[i]!=NULL) delete Portals[i];
	}
}
	// Базы
if(!Bases.empty()) {
	for(int i=0; i<(int)Bases.size(); i++) {
		if(Bases[i]!=NULL) delete Bases[i];
	}
}
}
//---------------------------------------------------------------------------
//                    РИСОВАНИЕ ОБЪЕКТОВ СЦЕНЫ НА ЭКРАН
//---------------------------------------------------------------------------
bool Scene::Draw()
{
	// Вывод объектов сектора в сцену
if(Created==false) return false;
	// SkyBox
if(SBox!=NULL) SBox->Draw();
	// Планеты
if(!Planets.empty()) {
	for(int i=0; i<(int)Planets.size(); i++) {
		if(Planets[i]!=NULL) (Planets[i])->Draw();
	}
}
	// Спутники
if(!Satellits.empty()) {
	for(int i=0; i<(int)Satellits.size(); i++) {
		if(Satellits[i]!=NULL) (Satellits[i])->Draw();
	}
}
	// Порталы
if(!Portals.empty()) {
	for(int i=0; i<(int)Portals.size(); i++) {
		if(Portals[i]!=NULL) (Portals[i])->Draw();
	}
}
	// Базы
if(!Bases.empty()) {
	for(int i=0; i<(int)Bases.size(); i++) {
		if(Bases[i]!=NULL) (Bases[i])->Draw();
	}
}
return true;
}
//---------------------------------------------------------------------------
//                              ОБЩИЕ ФУНКЦИИ
//---------------------------------------------------------------------------
bool Scene::LoadFromFile(std::string File)
{
        // Загрузка данных в сцену из файла
        // Получить версию сектора на компьютере клиента
FILE *F;
if(fopen_s(&F,(BasePath+File).c_str(),"rb")==NULL) {
	// Проверить не обновился-ли сектор на сервере
	// Текущая версия
	rewind(F);
	int CurrentVersion;
	fread(&CurrentVersion,sizeof(CurrentVersion),1,F);
	// Получить версию с сервера
	int RealVersion;


	// Если нужно - заново скачать файл сектора на компьютер клиента
	if(RealVersion>CurrentVersion) {


	}
}
else {
	// Файл с данными по сектору не скачан, или поврежден - заново скачать файл сектора на компьютер клиента
//	MessageBox(NULL,"ERRR","",NULL);

}
	// Получить данные по сектору
rewind(F);
struct SectorHeaderStruc {	// Структура заголовка сектора
	int Version;			// Номер версии сектора - чтобы проверять, нужно-ли обновлять файл на машине клиента
	char Name[50];			// Назвние
} SectorHeader;
fread(&SectorHeader,sizeof(SectorHeader),1,F);
Version = SectorHeader.Version;
Name = std::string(SectorHeader.Name);
	// Получить данные по остальным объектам сектора
	// Общие данные по объектам
struct PlanetsFatStruc {	// Структура планеты
	char Count;				// Количество планет
} PlanetsFat;
struct SatellitsFatStruc {	// Структура спутника
	char Count;				// Количество спутников
} SatellitsFat;
struct PortalsFatStruc {	// Структура портала
	char Count;				// Количество порталов
} PortalsFat;
struct BasesFatStruc {	// Структура базы
	char Count;			// Количество баз
} BasesFat;
fread(&PlanetsFat,sizeof(PlanetsFat),1,F);
fread(&SatellitsFat,sizeof(SatellitsFat),1,F);
fread(&PortalsFat,sizeof(PortalsFat),1,F);
fread(&BasesFat,sizeof(BasesFat),1,F);
	// Получаем данные конкретно по каждому объекту
	// Звезды
if(PlanetsFat.Count>0) {
	struct PlanetStruc {	// Структура, описывающая планету
		float X;			// Положение
		float Y;
		float Z;
		float RotateSpeed;	// Скорость вращения вокруг собственной оси углы/ед.времени
		char Name[50];		// Название
		char File[250];		// Путь к файлу с моделью
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
	// Спутники
if(SatellitsFat.Count>0) {
	struct SatellitStruc {		// Структура, описывающая один спутник
		float BigRadius;
		float SmallRadius;      // 2 радиуса, описывющие орбиту спутника (эллипс)
		float RotateSpeed;      // Скорость вращения вокруг собственной оси углы/ед.времени
		float MoveSpeed;        // Скорость движения по орбите углы/ед.времени
		char Name[50];          // Название
		char File[250];         // Путь к файлу с моделью
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
	// Порталы
if(PortalsFat.Count>0) {
	struct PortalStruc {	// Структура, описывающая портал
		float BigRadius;
		float SmallRadius;	// 2 радиуса, описывющие орбиту портала (эллипс)
		float MoveSpeed;	// Скорость движения по орбите углы/ед.времени
		char Name[50];		// Название
		char Dest[10];		// Сектор, в который ведет портал
		char File[250];		// Путь к файлу с моделью
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
	// Базы
if(BasesFat.Count>0) {
	struct BaseStruc {		// Структура, описывающая базу
		float BigRadius;
		float SmallRadius;	// 2 радиуса, описывющие орбиту портала (эллипс)
		float RotateSpeed;	// Скорость вращения вокруг собственной оси углы/ед.времени
		float MoveSpeed;	// Скорость движения по орбите углы/ед.времени
		char Name[50];		// Название
		char File[250];		// Путь к файлу с моделью
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
	// Проверка видимости по всем объектам сектора т.е. проверяем
	// попадают-ли объекты сектора в пирамиду видимости Frustum или нет
	// Если pFrustum = NULL - считаем что все объекты всегда попадают в пирамиду видимости
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
	// По всем объектам сектора проверяем их видимость (попдание в Frustum)
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
        // Проверка на пересечение отрезка заданного точками LineStart и LineEnd
        // хотя бы с одним из объектов сектора, возвращает true если пересечение
        // есть, false - нет, Distance,IntersectedObject - выходные параметры =
        // расстоянию до точки пересечения и указателю на объект с которым
        // произошло пересечение
if(Created==false) return false;
float TmpDistance = -1.0;
bool Rez = false;
	// Проверим Планеты по списку
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
	// Проверим Спутники по списку
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
	// Проверим Порталы по списку
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
	// Проверим Базы по списку
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