//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "Object3D.h"
//---------------------------------------------------------------------------
Object3D::Object3D(void)
{
	// Конструктор класса
	// 3D-объект
Mesh = NULL;
SizeOfVertex = 0;

VertexBuffer = NULL;
WorkVertexBuffer = NULL;
VertexCount = 0;

IndexBuffer = NULL;
WorkIndexBuffer = NULL;
IndexCount = 0;

Materials = NULL;
MaterialsCount = 0;
Textures = NULL;

Position = D3DXVECTOR3(0.0f,0.0f,0.0f);
VecX = D3DXVECTOR3(1.0f,0.0f,0.0f);
VecY = D3DXVECTOR3(0.0f,1.0f,0.0f);
VecZ = D3DXVECTOR3(0.0f,0.0f,-1.0f);
	// Матрица трансформации объекта по началу "единичная" (нет изменений)
D3DXMatrixIdentity(&TransformMatrix);
	// Указатель на пирамиду видимости
pFrustum = NULL;
}
//---------------------------------------------------------------------------
Object3D::~Object3D(void)
{
	// Деструктор класса
	// Буфера точек
if(VertexBuffer!=NULL) VertexBuffer->Release();
if(WorkVertexBuffer!=NULL) delete [] WorkVertexBuffer;
	// Буфера индексов
if(IndexBuffer!=NULL) IndexBuffer->Release();
if(WorkIndexBuffer!=NULL) delete [] WorkIndexBuffer;
	// Буфер материалов
if(Materials!=NULL) delete [] Materials;
	// Буфер текстур
if(Textures!=NULL) {
	for(DWORD i=0; i<MaterialsCount; i++ ) {
		if(Textures[i]!=NULL) Textures[i]->Release();        
	}
}
delete [] Textures;
	// 3D-Объект
if(Mesh!=NULL) Mesh->Release();
}
//---------------------------------------------------------------------------
//                     ФУНКЦИИ ОБЩИЕ ДЛЯ ВСЕХ ОБЪЕКТОВ
//---------------------------------------------------------------------------
bool Object3D::LoadFromFile(std::string File)
{
	// Загрузка объекта из файла
if(Common::FileExists((BasePath+File).c_str())==false) File = AlternativeFile;
LPD3DXBUFFER MatBuffer;	// Временный буфер для сохранения материалов объекта
D3DXLoadMeshFromX((LPSTR)(BasePath+File).c_str(),D3DXMESH_SYSTEMMEM,pD3DDevice,NULL,&MatBuffer,&MaterialsCount,&Mesh);
	// Оптимизация загруженного объекта (для ускорения его рисования на экран)
Mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT,NULL,NULL,NULL,NULL);	// Первый параметр указывает, что оптимизируем Mesh для лучшего его вывода через DrawSubset
	// Определить формат одной точки для загруженного объекта
if(Mesh->GetFVF()&D3DFVF_XYZ) {
	SizeOfVertex += sizeof(float)*3;
	VertexType = vsType1;
	VertexFVF = D3DFVF_XYZ;    
}
if(Mesh->GetFVF()&D3DFVF_NORMAL) {
	SizeOfVertex += sizeof(float)*3;
	VertexType = vsType3;
	VertexFVF = D3DFVF_XYZ|D3DFVF_NORMAL;
}
if(Mesh->GetFVF()&D3DFVF_TEX1) {
	SizeOfVertex += sizeof(float)*2;
	if(VertexType==vsType3) {
		VertexType = vsType2;
		VertexFVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
	}
	else {
		VertexType = vsType4;
		VertexFVF = D3DFVF_XYZ|D3DFVF_TEX1;
	}
}
	// Определить буфер точек и буфер индексов точек
Mesh->GetVertexBuffer(&VertexBuffer);
Mesh->GetIndexBuffer(&IndexBuffer);
VertexCount = Mesh->GetNumVertices();	// Кол-во точек
IndexCount = Mesh->GetNumFaces()*3;		// Кол-во индексов = кол-во полигонов * 3
	// Сделать копии буферов индексов и точек в системной памяти для ускорения работы с ними (чтобы меньше копировать из виде-памяти в системную и обратно)
	// Создаем копию буфера точек для работы с ним
switch (VertexType) {
	case(vsType1):
		WorkVertexBuffer = new VertexType1[VertexCount];
		break;
	case(vsType2):
		WorkVertexBuffer = new VertexType2[VertexCount];
		break;
	case(vsType3):
		WorkVertexBuffer = new VertexType3[VertexCount];
		break;
	case(vsType4):
		WorkVertexBuffer = new VertexType4[VertexCount];
		break;
}
void* WorkVertexBufferTmp;	// Указатель на буфер точек в видео-памяти, нужен для перебрасывания его в буфер точек в системной памяти
VertexBuffer->Lock(0,0,(BYTE**)&WorkVertexBufferTmp,0);
memcpy(WorkVertexBuffer,WorkVertexBufferTmp,VertexCount*SizeOfVertex);
VertexBuffer->Unlock();
	// Создаем копию буфера индексов для работы с ним (каждый полигон объекта описывается 3 индексами, полигоны размещаются последовательно в буфере начиная с первого)
WorkIndexBuffer = new unsigned short[IndexCount];
void* WorkIndexBufferTmp;	// Указатель на буфер индексов в видео-памяти, нужен для перебрасывания его в буфер индексов в системной памяти
IndexBuffer->Lock(0,0,(BYTE**)&WorkIndexBufferTmp,0);
memcpy(WorkIndexBuffer,WorkIndexBufferTmp,IndexCount*sizeof(unsigned short));
IndexBuffer->Unlock();
	// Определить материал и текстуры
D3DXMATERIAL* TmpMaterials = (D3DXMATERIAL*)MatBuffer->GetBufferPointer();	// Указатель на материалы загруженного объекта
Materials = new D3DMATERIAL8[MaterialsCount];
Textures = new LPDIRECT3DTEXTURE8[MaterialsCount];
for(DWORD i=0; i<MaterialsCount; i++) {
	Materials[i] = TmpMaterials[i].MatD3D;
	Materials[i].Ambient = Materials[i].Diffuse;
	if(FAILED(D3DXCreateTextureFromFile(pD3DDevice,(Common::GetPath(BasePath+File)+std::string(TmpMaterials[i].pTextureFilename)).c_str(),&Textures[i]))) Textures[i] = NULL;
}
MatBuffer->Release();
return true;
}
//---------------------------------------------------------------------------
//                       ПРОВЕРКИ ВИДИМОСТИ ОБЪЕКТА
//---------------------------------------------------------------------------
bool Object3D::CheckVisibility()
{
	// Проверка видимости объекта т.е. проверяем попадает-ли объект в
	// пирамиду видимости Frustum или нет
	// Если pFrustum = NULL - считаем что объект всегда попадает в пирамиду
	// видимости
if(pFrustum==NULL) {
	Visible = true;
	return true;
}
	// Проверяем каждую точку объекта на попадание в 6 плоскостей пирамиды видимости
D3DXPLANE* Planes = pFrustum->GetPlanes();	// Указатель на массив с плоскостями пирамиды видимости
for(unsigned int i=0; i<VertexCount; i++) {
	// Получим точку
	D3DXVECTOR3 v;
	// Приводим void* к типу формата точек (иначе неправильно работает массив)
	switch(VertexType) {
		case(vsType1):
			VertexType1* Tmp1;
			Tmp1 = (VertexType1*)WorkVertexBuffer;
			v = Tmp1[i].Position;
			break;
		case(vsType2):
			VertexType2* Tmp2;
			Tmp2 = (VertexType2*)WorkVertexBuffer;
			v = Tmp2[i].Position;
			break;
		case(vsType3):
			VertexType3* Tmp3;
			Tmp3 = (VertexType3*)WorkVertexBuffer;
			v = Tmp3[i].Position;
			break;
		case(vsType4):
			VertexType4* Tmp4;
			Tmp4 = (VertexType4*)WorkVertexBuffer;
			v = Tmp4[i].Position;
			break;	
	}
	// Проверим, попадает-ли точка в 6 плоскостей пирамиды видимости
	bool TmpRez = true;
	for(int j=0; j<=5; j++) {
		if(Planes[j].a*v.x+Planes[j].b*v.y+Planes[j].c*v.z+Planes[j].d<0.0) TmpRez = false;            
	}
	// Если попадает - виден весь объект
	if(TmpRez==true) {
		Visible = true;
		return true;
	}    
}
Visible = false;
return true;
}
//---------------------------------------------------------------------------
//                           ПРОВЕРКИ ВЫБОРА ОБЪЕКТА
//---------------------------------------------------------------------------
bool Object3D::CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance)
{
	// Проверка на пересечение отрезка заданного точками LineStart и LineEnd
	// хотя бы с одним полигоном объекта, возвращает true если пересечение
	// есть, false - нет, Distance - выходной параметр = расстоянию до точки
	// пересечения
bool Rez = false;	// true - пересечение есть, false - нет
	// Получить вектор направления отрезка
D3DXVECTOR3 LineDirection;
LineDirection = LineEnd - LineStart;
	// Проверить на пересечение переданного отрезка с текущим объектом
D3DXIntersect(Mesh,&LineStart,&LineDirection,(int*)&Rez,NULL,NULL,NULL,Distance,NULL,NULL);
if(Rez==false) *Distance = -1.0;
return Rez;
}
//---------------------------------------------------------------------------
//                         РИСОВАНИЕ ОБЪЕКТА НА ЭКРАН
//---------------------------------------------------------------------------
bool Object3D::Draw()
{
	// Вывод 3D-объекта в сцену
	// Если объект не видим - его не рисуем
if(Visible==false) return true;
	// Обрабатываем Mesh для вывода
pD3DDevice->SetVertexShader(VertexFVF);
pD3DDevice->SetStreamSource(0,VertexBuffer,SizeOfVertex);
pD3DDevice->SetIndices(IndexBuffer,0);
for(DWORD i=0; i<MaterialsCount; i++) {
	pD3DDevice->SetMaterial(&Materials[i]);
	pD3DDevice->SetTexture(0,Textures[i]);
	Mesh->DrawSubset(i);
}
return true;
}
//---------------------------------------------------------------------------
//              УПРАВЛЕНИЕ ПЕРЕМЕЩЕНИЕМ И МАСШТАБИРОВАНИЕМ ОБЪЕКТА
//---------------------------------------------------------------------------
bool Object3D::MoveTo(float X, float Y, float Z)
{
	// Перемещение объекта вдоль глобальных осей X,Y,Z на соответствующую
	// величину
D3DXMATRIX Move;
D3DXMatrixIdentity(&Move);
D3DXMatrixTranslation(&Move,X,Y,Z);
TransformMatrix *= Move;
return true;
}
//---------------------------------------------------------------------------
bool Object3D::MoveInto(float X, float Y, float Z)
{
	// Перемещение объекта в точку с координатами X,Y,Z в глобальной системе
	// координат
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&D3DXVECTOR3(X,Y,Z),&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::MoveX(float X)
{
	// Перемещение объекта вдоль его локальной оси X на нужное расстояние
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecX,&Position);
MoveVector *= X;
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::MoveY(float Y)
{
	// Перемещение объекта вдоль его локальной оси Y на нужное расстояние
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecY,&Position);
MoveVector *= Y;
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::MoveZ(float Z)
{
	// Перемещение объекта вдоль его локальной оси Z на нужное расстояние
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecZ,&Position);
MoveVector *= Z;
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::RotateX(float Angel)
{
	// Поворот объекта вокруг вектора VecХ на угол Angle. Чтобы объект
	// поворачивался вокруг своей оси, а не центра координат - поворачиваем
	// в 3 этапа: создаем матрицу переноса в центр координат, создаем
	// матрицу поворота, создаем матрицу переноса обратно в текущую позицию
	// Перенести в 0,0,0
D3DXVECTOR3 Vec000;	// Вектор указывающий в 0,0,0 в мировой системе координат
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector;	// Vec000 перобразованный в систему координат объекта
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
	// Поворачиваем объект
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationX(&Rot,Angel*M_PI/180);	// на Angel градусов (180 гр. = PI рад.)
TransformMatrix *= Rot;
	// Вернуть объект обратно
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::RotateY(float Angel)
{
	// Поворот объекта вокруг вектора VecY на угол Angle. Чтобы объект
	// поворачивался вокруг своей оси, а не центра координат - поворачиваем
	// в 3 этапа: создаем матрицу переноса в центр координат, создаем
	// матрицу поворота, создаем матрицу переноса обратно в текущую позицию
	// Перенести в 0,0,0
D3DXVECTOR3 Vec000;	// Вектор указывающий в 0,0,0 в мировой системе координат
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector;	// Vec000 перобразованный в систему координат объекта
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
	// Поворачиваем объект
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationY(&Rot,Angel*M_PI/180);	// на Angel градусов (180 гр. = PI рад.)
TransformMatrix *= Rot;
	// Вернуть объект обратно
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::RotateZ(float Angel)
{
	// Поворот объекта вокруг вектора VecZ на угол Angle. Чтобы объект
	// поворачивался вокруг своей оси, а не центра координат - поворачиваем
	// в 3 этапа: создаем матрицу переноса в центр координат, создаем
	// матрицу поворота, создаем матрицу переноса обратно в текущую позицию
	// Перенести в 0,0,0
D3DXVECTOR3 Vec000;	// Вектор указывающий в 0,0,0 в мировой системе координат
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector;	// Vec000 перобразованный в систему координат объекта
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
	// Поворачиваем объект
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationZ(&Rot,Angel*M_PI/180);	// на Angel градусов (180 гр. = PI рад.)
TransformMatrix *= Rot;
	// Вернуть объект обратно
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Object3D::Scale(float SX, float SY, float SZ)
{
	// Масштабирование объекта по осям X,Y,Z соответсвенно,
	// SX,SY,SY указывают ВО сколько раз масштабировать объект
D3DXMATRIX Scale;
D3DXMatrixIdentity(&Scale);
D3DXMatrixScaling(&Scale,SX,SY,SZ);
TransformMatrix *= Scale;
return true;
}
//---------------------------------------------------------------------------
bool Object3D::UpdateTransform()
{
	// Обновить объект т.е. применить к нему полученные трансформации
if(pD3DDevice==NULL) return false;
	// Изменить вектора, описывающие объект в соответствии с полученной матрицей трансформации
D3DXVec3TransformCoord(&Position,&Position,&TransformMatrix);
D3DXVec3TransformCoord(&VecX,&VecX,&TransformMatrix);
D3DXVec3TransformCoord(&VecY,&VecY,&TransformMatrix);
D3DXVec3TransformCoord(&VecZ,&VecZ,&TransformMatrix);
	// Изменить VertexBuffer объекта в соответствии с полученной матрицей трансформации
	// Применить матрицу трансформации к WorkVertexBuffer
for(unsigned int i=0; i<VertexCount; i++) {
	// Т.к. форматы точек разные - тарансформация зависит от формата
	switch(VertexType) {
		case(vsType1):
			VertexType1* Tmp1;	// Приводим void* к типу формата точек (иначе неправильно работает массив)
			Tmp1 = (VertexType1*)WorkVertexBuffer;
			D3DXVec3TransformCoord(&Tmp1[i].Position,&Tmp1[i].Position,&TransformMatrix);
			break;
		case(vsType2):
			VertexType2* Tmp2;	// Приводим void* к типу формата точек (иначе неправильно работает массив)
			Tmp2 = (VertexType2*)WorkVertexBuffer;
			D3DXVec3TransformCoord(&Tmp2[i].Position,&Tmp2[i].Position,&TransformMatrix);
			D3DXVec3TransformNormal(&Tmp2[i].Normal,&Tmp2[i].Normal,&TransformMatrix);
			D3DXVec3Normalize(&Tmp2[i].Normal,&Tmp2[i].Normal);
			break;
		case(vsType3):
			VertexType3* Tmp3;	// Приводим void* к типу формата точек (иначе неправильно работает массив)
			Tmp3 = (VertexType3*)WorkVertexBuffer;
			D3DXVec3TransformCoord(&Tmp3[i].Position,&Tmp3[i].Position,&TransformMatrix);
			D3DXVec3TransformNormal(&Tmp3[i].Normal,&Tmp3[i].Normal,&TransformMatrix);
			D3DXVec3Normalize(&Tmp3[i].Normal,&Tmp3[i].Normal);
			break;
		case(vsType4):
			VertexType4* Tmp4;	// Приводим void* к типу формата точек (иначе неправильно работает массив)
			Tmp4 = (VertexType4*)WorkVertexBuffer;
			D3DXVec3TransformCoord(&Tmp4[i].Position,&Tmp4[i].Position,&TransformMatrix);
			break;
	}
}
	// Сбросить обновленный WorkVertexBuffer в VertexBuffer
void* WorkVertexBufferTmp;	// Временный буфер точек
VertexBuffer->Lock(0,0,(BYTE**)&WorkVertexBufferTmp,0);
memcpy(WorkVertexBufferTmp,WorkVertexBuffer,VertexCount*SizeOfVertex);
VertexBuffer->Unlock();
	// Очистить матрицу трансформации
D3DXMatrixIdentity(&TransformMatrix);
	// Проверить объект на попадание в пирамиду видимости
CheckVisibility();
return true;
}
//---------------------------------------------------------------------------