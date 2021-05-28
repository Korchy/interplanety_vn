//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "BoundingOBB.h"
//---------------------------------------------------------------------------
BoundingOBB::BoundingOBB(void)
{
	// Конструктор класса
OBBSizeX = 0.0;
OBBSizeY = 0.0;
OBBSizeZ = 0.0;
OBBVecX = D3DXVECTOR3(0.0f,0.0f,0.0f);
OBBVecY = D3DXVECTOR3(0.0f,0.0f,0.0f);
OBBVecZ = D3DXVECTOR3(0.0f,0.0f,0.0f);
}
//---------------------------------------------------------------------------
BoundingOBB::~BoundingOBB(void)
{
	// Деструктор класса

}
//---------------------------------------------------------------------------
bool BoundingOBB::CreateBy3DObject(void* Object3DVertexBuffer,int Object3DMeshVertexCount, unsigned int Object3DVertexFVF)
{
	// Вычислить параметры OBB для конкретного объекта
D3DXVECTOR3 OBBMax;
D3DXVECTOR3 OBBMin;
D3DXComputeBoundingBox(Object3DVertexBuffer,Object3DMeshVertexCount,Object3DVertexFVF,&OBBMin,&OBBMax);
OBBSizeX = (OBBMax.x - OBBMin.x)/2.0f;
OBBSizeY = (OBBMax.y - OBBMin.y)/2.0f;
OBBSizeZ = (OBBMax.z - OBBMin.z)/2.0f;
Position = OBBMin + D3DXVECTOR3(OBBSizeX,OBBSizeY,OBBSizeZ);
OBBVecX = OBBMax + (D3DXVECTOR3(OBBMax.x,OBBMin.y,OBBMin.z) - OBBMax)/2.0f;
OBBVecY = OBBMax + (D3DXVECTOR3(OBBMin.x,OBBMax.y,OBBMin.z) - OBBMax)/2.0f;
OBBVecZ = OBBMax + (D3DXVECTOR3(OBBMin.x,OBBMin.y,OBBMax.z) - OBBMax)/2.0f;
//MessageBox(NULL,(FloatToStr(Position.x)+"  "+FloatToStr(Position.y)+"  "+FloatToStr(Position.z)).c_str(),"",NULL);
return true;
}
//---------------------------------------------------------------------------
//            ПРОВЕРКА ПОПАДАНИЯ BoundibngOBB В ПИРАМИДУ ВИДИМОСТИ
//---------------------------------------------------------------------------
bool BoundingOBB::CheckVisibility()
{
	// Проверка на попадание BoundingOBB в пирамиду видимости pFrustum
if(pFrustum==NULL) return true;
	// Указатель на массив с плоскостями пирамиды видимости
D3DXPLANE* Planes = pFrustum->GetPlanes();
	// Найдем 8 крайних точек OBB
D3DXVECTOR3 LocX = OBBVecX - Position;	// Вектор вдоль локальной оси OBB Х
D3DXVECTOR3 LocY = OBBVecY - Position;	// Вектор вдоль локальной оси OBB Y
D3DXVECTOR3 LocZ = OBBVecZ - Position;	// Вектор вдоль локальной оси OBB Z
D3DXVECTOR3 OBB[9];
OBB[0] = Position+(LocY-LocX-LocZ);		// 1 вершина OBB
OBB[1] = Position+(LocY+LocX-LocZ);
OBB[2] = Position+(-LocY+LocX-LocZ);
OBB[3] = Position+(-LocY-LocX-LocZ);
OBB[4] = Position+(LocY-LocX+LocZ);
OBB[5] = Position+(LocY+LocX+LocZ);
OBB[6] = Position+(-LocY+LocX+LocZ);
OBB[7] = Position+(-LocX-LocY+LocZ);	// 8 вершина OBB
OBB[8] = Position;						// Position проверка нужна чтобы предусмотреть случай, когда часть вершин OBB лежит слева от левой плоскости, а другая часть справа
	// По каждой крайней точке OBB проверяем, попадает-ли она в область видимости камеры - если попадает хоть одна -> обрабатываем
for(int i=0; i<=8; i++) {
	bool Rez = true;
	for(int j=0; j<=5; j++) {
		if(Planes[j].a*OBB[i].x+Planes[j].b*OBB[i].y+Planes[j].c*OBB[i].z+Planes[j].d<0.0) Rez = false;
	}
	if(Rez==true) return true;
}
return false;
}
//---------------------------------------------------------------------------
//                             COLLISION DETECTION
//---------------------------------------------------------------------------
bool BoundingOBB::CheckIntersection(BoundingObject* BO)
{
//MessageBox(NULL,"OBB","",NULL);
	// Проверка на перечечение другого BoundingObject (BO) c данным  (метод
	// нужен, чтобы вызывать проверку не вникая в реальный тип BoundingObject)
bool Rez = BO->CheckIntersectionWithOBB(OBBVecX,OBBVecY,OBBVecZ,Position,OBBSizeX,OBBSizeY,OBBSizeZ);
return Rez;
}
//---------------------------------------------------------------------------
bool BoundingOBB::CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd)
{
//MessageBox(NULL,"Line","",NULL);
	// Проверка на пересечение OBB с линией заданной двумя точками
	// Считаем линию вырожденным OBB у которого 2 из 3х размеров = 0 и
	// проверяем также как OBB - OBB
D3DXVECTOR3 LineDirection = LineEnd - LineStart;
D3DXVECTOR3 LinePosition = LineStart + LineDirection/2.0;	// Вектор в середину линии
	// 1/2 длины линии
float LineLength2 = (sqrt(LineDirection.x*LineDirection.x+LineDirection.y*LineDirection.y+LineDirection.z*LineDirection.z))/(float)2.0;
	// Проверяем проекции линии на оси OBB, и их
	// проекции на векторные произведения их осей
D3DXVECTOR3 v = LinePosition - Position;
D3DXVECTOR3 LocX = OBBVecX - Position;	// Вектор локальной оси OBB Х
D3DXVec3Normalize(&LocX,&LocX);
D3DXVECTOR3 LocY = OBBVecY - Position;	// Вектор локальной оси OBB Y
D3DXVec3Normalize(&LocY,&LocY);
D3DXVECTOR3 LocZ = OBBVecZ - Position;	// Вектор локальной оси OBB Z
D3DXVec3Normalize(&LocZ,&LocZ);
D3DXVECTOR3 LocL = LineEnd - LinePosition;	// Локальный вектор линии вдоль ее оси
D3DXVec3Normalize(&LocL,&LocL);
float T[3];
T[0] = D3DXVec3Dot(&v,&LocX);	// Длина проекции v на локальные вектора-оси OBB
T[1] = D3DXVec3Dot(&v,&LocY);
T[2] = D3DXVec3Dot(&v,&LocZ);
float TM[3];	// Проекции локальных осей OBB на вектор линии
TM[0] = D3DXVec3Dot(&LocX,&LocL);
TM[1] = D3DXVec3Dot(&LocY,&LocL);
TM[2] = D3DXVec3Dot(&LocZ,&LocL);
	// Проверка проецированием на оси OBB
if(fabs(T[0])>OBBSizeX+LineLength2*fabs(TM[0])) return false;
if(fabs(T[1])>OBBSizeY+LineLength2*fabs(TM[1])) return false;
if(fabs(T[2])>OBBSizeZ+LineLength2*fabs(TM[2])) return false;
	// Проверка проецированием на ось линии
if(fabs(T[0]*TM[0]+T[1]*TM[1]+T[2]*TM[2])>(OBBSizeX*fabs(TM[0])+OBBSizeY*fabs(TM[1])+OBBSizeZ*fabs(TM[2]+LineLength2))) return false;
	// Проверка проецированием на 3 векторных произведения осей OBB и линии
	// OBBVecX x LocL
if(fabs(T[2]*TM[1]-T[1]*TM[2])>OBBSizeY*fabs(TM[2])+OBBSizeZ*fabs(TM[1])) return false;
	// OBBVecY x LocL
if(fabs(T[0]*TM[2]-T[2]*TM[0])>OBBSizeX*fabs(TM[2])+OBBSizeZ*fabs(TM[0])) return false;
	// OBBVecZ x LocL
if(fabs(T[1]*TM[0]-T[0]*TM[1])>OBBSizeX*fabs(TM[1])+OBBSizeY*fabs(TM[0])) return false;
	// Если все проекции пересекаются - есть пересечение
return true;
}
//---------------------------------------------------------------------------
bool BoundingOBB::CheckIntersectionWithSphere(D3DXVECTOR3 const &PositionS, float RadiusS)
{
//MessageBox(NULL,"Sphere","",NULL);
	// Проверка на пересечение OBB c BoundingSphere
	// Проверяем по проекциям на каждую локальную ось OBB - лежит ли центр
	// сферы внутри OBB, да - есть пересечение, нет - проверяем расстояние
	// от границы OBB до центра сферы и сравниваем с ее радиусом
D3DXVECTOR3 PositionSLoc = PositionS - Position;	// Вектор из OBBPosition в SpherePosition
D3DXVECTOR3 LocX = OBBVecX - Position;	// Вектор локальной оси OBB Х
D3DXVec3Normalize(&LocX,&LocX);
D3DXVECTOR3 LocY = OBBVecY - Position;	// Вектор локальной оси OBB Y
D3DXVec3Normalize(&LocY,&LocY);
D3DXVECTOR3 LocZ = OBBVecZ - Position;	// Вектор локальной оси OBB Z
D3DXVec3Normalize(&LocZ,&LocZ);
float PositionSX = fabs(D3DXVec3Dot(&PositionSLoc,&LocX));	// Длина проэкции PositionSLoc на локальные вектора-оси OBB
float PositionSY = fabs(D3DXVec3Dot(&PositionSLoc,&LocY));
float PositionSZ = fabs(D3DXVec3Dot(&PositionSLoc,&LocZ));
float d = 0.0;
	// Сравниваем PositionSX, SY ,SZ с размерами OBB и с радиусом сферы
if(PositionSX>OBBSizeX) d += (PositionSX - OBBSizeX)*(PositionSX - OBBSizeX);
if(PositionSY>OBBSizeY) d += (PositionSY - OBBSizeY)*(PositionSY - OBBSizeY);
if(PositionSZ>OBBSizeZ) d += (PositionSZ - OBBSizeZ)*(PositionSZ - OBBSizeZ);
if(d>(RadiusS*RadiusS)) return false;
else return true;
}
//---------------------------------------------------------------------------
bool BoundingOBB::CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX1, D3DXVECTOR3 const &OBBVecY1, D3DXVECTOR3 const &OBBVecZ1, D3DXVECTOR3 const &PositionOBB1, float OBBSizeX1, float OBBSizeY1, float OBBSizeZ1)
{
//MessageBox(NULL,"OBB","",NULL);
	// Проверка на пересечение OBB с OBB
	// Проверяем проекции одного OBB на оси другого и наоборот, и их
	// проекции на векторные произведения их осей
D3DXVECTOR3 v = PositionOBB1 - Position;
D3DXVECTOR3 LocX = OBBVecX - Position;	// Вектор локальной оси OBB Х
D3DXVec3Normalize(&LocX,&LocX);
D3DXVECTOR3 LocY = OBBVecY - Position;	// Вектор локальной оси OBB Y
D3DXVec3Normalize(&LocY,&LocY);
D3DXVECTOR3 LocZ = OBBVecZ - Position;	// Вектор локальной оси OBB Z
D3DXVec3Normalize(&LocZ,&LocZ);
D3DXVECTOR3 LocX1 = OBBVecX1 - PositionOBB1;	// Вектор локальной оси OBB1 Х
D3DXVec3Normalize(&LocX1,&LocX1);
D3DXVECTOR3 LocY1 = OBBVecY1 - PositionOBB1;	// Вектор локальной оси OBB1 Y
D3DXVec3Normalize(&LocY1,&LocY1);
D3DXVECTOR3 LocZ1 = OBBVecZ1 - PositionOBB1;	// Вектор локальной оси OBB1 Z
D3DXVec3Normalize(&LocZ1,&LocZ1);
float T[3];
T[0] = D3DXVec3Dot(&v,&LocX);	// Длина проекции v на локальные вектора-оси OBB
T[1] = D3DXVec3Dot(&v,&LocY);
T[2] = D3DXVec3Dot(&v,&LocZ);
float TM[3][3];		// Матрица трансформации одного OBB в систему другого
TM[0][0] = D3DXVec3Dot(&LocX,&LocX1);
TM[0][1] = D3DXVec3Dot(&LocX,&LocY1);
TM[0][2] = D3DXVec3Dot(&LocX,&LocZ1);
TM[1][0] = D3DXVec3Dot(&LocY,&LocX1);
TM[1][1] = D3DXVec3Dot(&LocY,&LocY1);
TM[1][2] = D3DXVec3Dot(&LocY,&LocZ1);
TM[2][0] = D3DXVec3Dot(&LocZ,&LocX1);
TM[2][1] = D3DXVec3Dot(&LocZ,&LocY1);
TM[2][2] = D3DXVec3Dot(&LocZ,&LocZ1);
	// Проверка проецированием на оси OBB
if(fabs(T[0])>OBBSizeX+OBBSizeX1*fabs(TM[0][0])+OBBSizeY1*fabs(TM[0][1])+OBBSizeZ1*fabs(TM[0][2])) return false;
if(fabs(T[1])>OBBSizeY+OBBSizeX1*fabs(TM[1][0])+OBBSizeY1*fabs(TM[1][1])+OBBSizeZ1*fabs(TM[1][2])) return false;
if(fabs(T[2])>OBBSizeZ+OBBSizeX1*fabs(TM[2][0])+OBBSizeY1*fabs(TM[2][1])+OBBSizeZ1*fabs(TM[2][2])) return false;
	// Проверка проецированием на оси OBB1
if(fabs(T[0]*TM[0][0]+T[1]*TM[1][0]+T[2]*TM[2][0])>(OBBSizeX*fabs(TM[0][0])+OBBSizeY*fabs(TM[1][0])+OBBSizeZ*fabs(TM[2][0]+OBBSizeX1))) return false;
if(fabs(T[0]*TM[0][1]+T[1]*TM[1][1]+T[2]*TM[2][1])>(OBBSizeX*fabs(TM[0][1])+OBBSizeY*fabs(TM[1][1])+OBBSizeZ*fabs(TM[2][1]+OBBSizeY1))) return false;
if(fabs(T[0]*TM[0][2]+T[1]*TM[1][2]+T[2]*TM[2][2])>(OBBSizeX*fabs(TM[0][2])+OBBSizeY*fabs(TM[1][2])+OBBSizeZ*fabs(TM[2][2]+OBBSizeZ1))) return false;
	// Проверка проецированием на 9 векторных произведений осей OBB и OBB1
	// OBBVecX x OBBVecX1
if(fabs(T[2]*TM[1][0]-T[1]*TM[2][0])>OBBSizeY*fabs(TM[2][0])+OBBSizeZ*fabs(TM[1][0])+OBBSizeY1*fabs(TM[0][2])+OBBSizeZ1*fabs(TM[0][1])) return false;
	// OBBVecX x OBBVecY1
if(fabs(T[2]*TM[1][1]-T[1]*TM[2][1])>OBBSizeY*fabs(TM[2][1])+OBBSizeZ*fabs(TM[1][1])+OBBSizeX1*fabs(TM[0][2])+OBBSizeZ1*fabs(TM[0][0])) return false;
	// OBBVecX x OBBVecZ1
if(fabs(T[2]*TM[1][2]-T[1]*TM[2][2])>OBBSizeY*fabs(TM[2][2])+OBBSizeZ*fabs(TM[1][2])+OBBSizeX1*fabs(TM[0][1])+OBBSizeY1*fabs(TM[0][0])) return false;
	// OBBVecY x OBBVecX1
if(fabs(T[0]*TM[2][0]-T[2]*TM[0][0])>OBBSizeX*fabs(TM[2][0])+OBBSizeZ*fabs(TM[0][0])+OBBSizeY1*fabs(TM[1][2])+OBBSizeZ1*fabs(TM[1][1])) return false;
	// OBBVecY x OBBVecY1
if(fabs(T[0]*TM[2][1]-T[2]*TM[0][1])>OBBSizeX*fabs(TM[2][1])+OBBSizeZ*fabs(TM[0][1])+OBBSizeX1*fabs(TM[1][2])+OBBSizeZ1*fabs(TM[1][0])) return false;
	// OBBVecY x OBBVecZ1
if(fabs(T[0]*TM[2][2]-T[2]*TM[0][2])>OBBSizeX*fabs(TM[2][2])+OBBSizeZ*fabs(TM[0][2])+OBBSizeX1*fabs(TM[1][1])+OBBSizeY1*fabs(TM[1][0])) return false;
	// OBBVecZ x OBBVecX1
if(fabs(T[1]*TM[0][0]-T[0]*TM[1][0])>OBBSizeX*fabs(TM[1][0])+OBBSizeY*fabs(TM[0][0])+OBBSizeY1*fabs(TM[2][2])+OBBSizeZ1*fabs(TM[2][1])) return false;
	// OBBVecZ x OBBVecY1
if(fabs(T[1]*TM[0][1]-T[0]*TM[1][1])>OBBSizeX*fabs(TM[1][1])+OBBSizeY*fabs(TM[0][1])+OBBSizeX1*fabs(TM[2][2])+OBBSizeZ1*fabs(TM[2][0])) return false;
	// OBBVecZ x OBBVecZ1
if(fabs(T[1]*TM[0][2]-T[0]*TM[1][2])>OBBSizeX*fabs(TM[1][2])+OBBSizeY*fabs(TM[0][2])+OBBSizeX1*fabs(TM[2][1])+OBBSizeY1*fabs(TM[2][0])) return false;
	// Если все проекции пересекаются - есть пересечение
return true;
}
//---------------------------------------------------------------------------
//         УПРАВЛЕНИЕ ПЕРЕМЕЩЕНИЕМ И МАСШТАБИРОВАНИЕМ BoundingOBB
//---------------------------------------------------------------------------
bool BoundingOBB::UpdateTransform(D3DXMATRIX const &TransformMatrix, float SX, float SY, float SZ)
{
	// Применить матрицу трансформации к BoundingObject, SX,SY,SZ - коэффициенты
	// масштабирования при изменении масштаба
	// Изменить положение центра, и векторов OBB
D3DXVec3TransformCoord(&Position,&Position,&TransformMatrix);
D3DXVec3TransformCoord(&OBBVecX,&OBBVecX,&TransformMatrix);
D3DXVec3TransformCoord(&OBBVecY,&OBBVecY,&TransformMatrix);
D3DXVec3TransformCoord(&OBBVecZ,&OBBVecZ,&TransformMatrix);
	// Изменить размеры
if(SX!=1.0||SY!=1.0||SZ!=1.0) {
	OBBSizeX *= SX;
	OBBSizeY *= SY;
	OBBSizeZ *= SZ;    
}
return true;
}
//---------------------------------------------------------------------------