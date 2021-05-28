//---------------------------------------------------------------------------
#pragma hdrstop

#include "ObjectSector.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
ObjectSector::ObjectSector(void)
{
        // Конструктор класса
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
        // Деструктор класса
        // Небесная сфера
if(SkyBox!=NULL) delete SkyBox;
        // Звезды
if(Stars!=NULL) {
        for(int i=0; i<Stars->Count; i++) {
                if(Stars->Items[i]!=NULL) delete (Object*)Stars->Items[i];
                }
        delete Stars;
        }
        // Планеты
if(Planets!=NULL) {
        for(int i=0; i<Planets->Count; i++) {
                if(Planets->Items[i]!=NULL) delete (Object*)Planets->Items[i];
                }
        delete Planets;
        }
        // Порталы
if(Portals!=NULL) {
        for(int i=0; i<Portals->Count; i++) {
                if(Portals->Items[i]!=NULL) delete (Object*)Portals->Items[i];
                }
        delete Portals;
        }
        // Базы
if(Bases!=NULL) {
        for(int i=0; i<Bases->Count; i++) {
                if(Bases->Items[i]!=NULL) delete (Object*)Bases->Items[i];
                }
        delete Bases;
        }
}
//---------------------------------------------------------------------------
//                    РИСОВАНИЕ ОБЪЕКТОВ СЕКТОРА НА ЭКРАН
//---------------------------------------------------------------------------
bool ObjectSector::Draw()
{
        // Вывод объектов сектора в сцену
if(Created==false) return false;
         // SkyBox
if(SkyBox!=NULL) SkyBox->Draw();
        // Звезды
if(Stars!=NULL) {
        for(int i=0; i<Stars->Count; i++) {
                if(Stars->Items[i]!=NULL) ((Object*)Stars->Items[i])->Draw();
                }
        }
        // Планеты
if(Planets!=NULL) {
        for(int i=0; i<Planets->Count; i++) {
                if(Planets->Items[i]!=NULL) ((Object*)Planets->Items[i])->Draw();
                }
        }
        // Порталы
if(Portals!=NULL) {
        for(int i=0; i<Portals->Count; i++) {
                if(Portals->Items[i]!=NULL) ((Object*)Portals->Items[i])->Draw();
                }
        }
        // Базы
if(Bases!=NULL) {
        for(int i=0; i<Bases->Count; i++) {
                if(Bases->Items[i]!=NULL) ((Object*)Bases->Items[i])->Draw();
                }
        }
return true;
}
//---------------------------------------------------------------------------
//                              ОБЩИЕ ФУНКЦИИ
//---------------------------------------------------------------------------
bool ObjectSector::LoadFromFile(AnsiString File)
{
        // Загрузка данных по сектору из файла
        // Получить версию сектора на компьютере клиента
FILE *F;
if((F=fopen((BasePath+File).c_str(),"rb"))==NULL) {
        // Файл с данными по сектору не скачан, или поврежден - заново скачать файл сектора на компьютер клиента
//        MessageBox(NULL,"ERRR","",NULL);




        }
else {
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
        // Получить данные по сектору
rewind(F);
struct SectorHeaderStruc {      // Структура заголовка сектора
        int Version;            // Номер версии сектора - чтобы проверять, нужно-ли обновлять файл на машине клиента
        char Name[50];          // Назвние
        } SectorHeader;
fread(&SectorHeader,sizeof(SectorHeader),1,F);
Version = SectorHeader.Version;
Name = AnsiString(SectorHeader.Name);
        // Получить данные по остальным объектам сектора
        // Общие данные по объектам
struct StarsFatStruc {          // Структура звезды
        char Count;             // Количество звезд
        } StarsFat;
struct PlanetsFatStruc {        // Структура планеты
        char Count;             // Количество спутников
        } PlanetsFat;
struct PortalsFatStruc {        // Структура портала
        char Count;             // Количество порталов
        } PortalsFat;
struct BasesFatStruc {          // Структура базы
        char Count;             // Количество баз
        } BasesFat;
fread(&StarsFat,sizeof(StarsFat),1,F);
fread(&PlanetsFat,sizeof(PlanetsFat),1,F);
fread(&PortalsFat,sizeof(PortalsFat),1,F);
fread(&BasesFat,sizeof(BasesFat),1,F);
        // Получаем данные конкретно по каждому объекту
        // Звезды
if(StarsFat.Count>0) {
        Stars = new TList();
        struct StarStruc {              // Структура, описывающая звезду
                float X;
                float Y;
                float Z;                // Положение
                float RotateSpeed;      // Скорость вращения вокруг собственной оси углы/ед.времени
                char Name[50];          // Название
                char File[250];         // Путь к файлу с моделью
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
        // Планеты
if(PlanetsFat.Count>0) {
        Planets = new TList();
        struct PlanetStruc {            // Структура, описывающая один спутник
                float BigRadius;
                float SmallRadius;      // 2 радиуса, описывющие орбиту спутника (эллипс)
                float RotateSpeed;      // Скорость вращения вокруг собственной оси углы/ед.времени
                float MoveSpeed;        // Скорость движения по орбите углы/ед.времени
                char Name[50];          // Название
                char File[250];         // Путь к файлу с моделью
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
        // Порталы
if(PortalsFat.Count>0) {
        Portals = new TList();
        struct PortalStruc {            // Структура, описывающая портал
                float BigRadius;
                float SmallRadius;      // 2 радиуса, описывющие орбиту портала (эллипс)
                float MoveSpeed;        // Скорость движения по орбите углы/ед.времени
                char Name[50];          // Название
                char Dest[10];          // Сектор, в который ведет портал
                char File[250];         // Путь к файлу с моделью
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
        // Базы
if(BasesFat.Count>0) {
        Bases = new TList();
        struct BaseStruc {              // Структура, описывающая базу
                float BigRadius;
                float SmallRadius;      // 2 радиуса, описывющие орбиту портала (эллипс)
                float RotateSpeed;      // Скорость вращения вокруг собственной оси углы/ед.времени
                float MoveSpeed;        // Скорость движения по орбите углы/ед.времени
                char Name[50];          // Название
                char File[250];         // Путь к файлу с моделью
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
        // Проверка видимости по всем объектам сектора т.е. проверяем
        // попадают-ли объекты сектора в пирамиду видимости Frustum или нет
        // Если pFrustum = NULL - считаем что все объекты всегда попадают в пирамиду видимости
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
        // По всем объектам сектора проверяем их видимость (попдание в Frustum)
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
        // Проверка на пересечение отрезка заданного точками LineStart и LineEnd
        // хотя бы с одним из объектов сектора, возвращает true если пересечение
        // есть, false - нет, Distance,IntersectedObject - выходные параметры =
        // расстоянию до точки пересечения и указателю на объект с которым
        // произошло пересечение
if(Created==false) return false;
float TmpDistance = -1.0;
bool Rez = false;
/*
        // Проверим Солнце
bool Rez = Sun->CheckIntersectionWithLine(LineStart,LineEnd,&TmpDistance);
if(Rez==true) {
        IntersectedObject = (Object*)Sun;
        *Distance = TmpDistance;
        }
*/
        // Проверим Звезды по списку
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
        // Проверим Планеты по списку
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
        // Проверим Порталы по списку
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
        // Проверим Базы по списку
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

