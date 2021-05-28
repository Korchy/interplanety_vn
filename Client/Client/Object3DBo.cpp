//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Object3DBo.h"
//---------------------------------------------------------------------------
Object3DBo::Object3DBo(void)
{
	// Конструктор класса
Bound = NULL;	// BoundingObject
BoundSX = 1.0;
BoundSY = 1.0;
BoundSZ = 1.0;	// Коэффициенты масштабирования для BoundingObject
}

Object3DBo::~Object3DBo(void)
{
	// Деструктор класса

}
//---------------------------------------------------------------------------
//                                  ФУНКЦИИ
//---------------------------------------------------------------------------
bool Object3DBo::LoadFromFile(std::string File)
{
        // Загрузка объекта из файла
bool Rez = Object3D::LoadFromFile(File);
if(Rez==false) return Rez;
        // Создать для загруженного объекта BoundingObject
Rez = Bound->CreateBy3DObject(WorkVertexBuffer,VertexCount,VertexFVF);
return Rez;
}
//---------------------------------------------------------------------------
//                       ПРОВЕРКИ ВИДИМОСТИ ОБЪЕКТА
//---------------------------------------------------------------------------
bool Object3DBo::CheckVisibility()
{
	// Проверка видимости объекта т.е. проверяем попадает-ли объект в
	// пирамиду видимости Frustum или нет
	// Если pFrustum = NULL - считаем что объект всегда попадает в пирамиду
	// видимости
if(pFrustum==NULL) {
	Visible = true;
	return true;
}
	// Сначала проверяем по BoundingObject объекта
bool Rez = Bound->CheckVisibility();
if(Rez==true) {
	Visible = true;
	// Если BoundingObject объекта виден - проверка по самому объекту т.к. он может быть не виден (Пока не нужно)
	// Object3D::CheckVisibility();
}
else Visible = false;
return true;
}
//---------------------------------------------------------------------------
//                           ПРОВЕРКИ ВЫБОРА ОБЪЕКТА
//---------------------------------------------------------------------------
bool Object3DBo::CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance)
{
	// Проверка на пересечение объекта с отрезком заданным точками LineStart и LineEnd
	// Сначала проверяем пересечение с его BoundingObject
bool BoundRez = Bound->CheckIntersectionWithLine(LineStart,LineEnd);
	// Если есть пересечение с BoundingObject - проверяем пересечение с самим объектом
if(BoundRez==true) {
	return Object3D::CheckIntersectionWithLine(LineStart,LineEnd,Distance);       
}
else return BoundRez;
}
//---------------------------------------------------------------------------
//              УПРАВЛЕНИЕ ПЕРЕМЕЩЕНИЕМ И МАСШТАБИРОВАНИЕМ ОБЪЕКТА
//---------------------------------------------------------------------------
bool Object3DBo::Scale(float SX, float SY, float SZ)
{
	// Масштабирование объекта по осям X,Y,Z соответсвенно,
	// SX,SY,SY указывают ВО сколько раз масштабировать объект
	// Запоминаем масштабирование для BoundingObject
BoundSX *= SX;
BoundSY *= SY;
BoundSZ *= SZ;
	// Изменяем матрицу трансформирования самого объекта
return Object3D::Scale(SX,SY,SZ);
}
//---------------------------------------------------------------------------
bool Object3DBo::UpdateTransform()
{
	// Обновить объект т.е. применить к нему полученные трансформации
if(pD3DDevice==NULL) return false;
	// Применяем трансформации к BoundingObject
Bound->UpdateTransform(TransformMatrix,BoundSX,BoundSY,BoundSZ);
BoundSX = 1.0;
BoundSY = 1.0;
BoundSZ = 1.0;
	// Применяем трансформации к самому объекту
bool Rez = Object3D::UpdateTransform();
return Rez;
}
//---------------------------------------------------------------------------
//                             COLLISION DETECTION
//---------------------------------------------------------------------------
/*
bool Object3DBo::CheckIntersections(TList* AllList, TList* IntersectionList)
{
        // Проверка на пересечение данного объекта со всеми объектами списка
        // AllList. Возвращает true если есть хоть одно пересечение, плюс
        // заносит в список IntersectioList объекты с которыми есть пересечение.
        // IntersectionList д.б. создан заранее и уничтожен после вызова
        // проверки.
bool Rez = false;
        // Создан-ли список в котором будут возвращаться результаты проверки
try {
        if(IntersectionList->Count>0) { // Возвращаемый список должен быть пустым
//                IntersectionList = NULL;
                return Rez;
                }
        }
catch(...) {
//        IntersectionList = NULL;
        return Rez;
        }
        // Проверяем все пересечения
for(int i=0; i<AllList->Count; i++) {
        if(dynamic_cast<Object3DUnit*>((Object*)(AllList->Items[i]))!=NULL) {
                // Если объект из списка приводится к типу Object3DUnit - проверяем столкновение
                if((Object3DUnit*)(AllList->Items[i])==this) continue;        // "Сам с собой"
                // Столкновения проверяем через столкновение BoundingObject
                bool TmpRez = ((Object3DUnit*)(AllList->Items[i]))->Bound->CheckIntersection(Bound);
                if(TmpRez==true) {
                        IntersectionList->Add(AllList->Items[i]);
                        Rez = TmpRez;
                        }
                }
//        MessageBox(NULL,(IntToStr(IntersectionList->Count)).c_str(),"",NULL);
        }
return Rez;
}
*/
//---------------------------------------------------------------------------