//---------------------------------------------------------------------------
#pragma hdrstop

#include "BoundingSphere.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
BoundingSphere::BoundingSphere(void)
{
        // Конструктор класса без параметров
        // Инициализация переменных
Radius = 0.0;
//MessageBox(NULL,"BOUNDING SPHERE","",NULL);
}
//---------------------------------------------------------------------------
BoundingSphere::~BoundingSphere()
{
        // Деструктор класса

}
//---------------------------------------------------------------------------
//                                ОБЩИЕ ФУНКЦИИ
//---------------------------------------------------------------------------
bool BoundingSphere::CreateBy3DObject(void* Object3DVertexBuffer,int Object3DMeshVertexCount, unsigned int Object3DVertexFVF)
{
        // Вычислить параметры BoundingSphere для конкретного объекта
D3DXComputeBoundingSphere(Object3DVertexBuffer,Object3DMeshVertexCount,Object3DVertexFVF,&Position,&Radius);
//MessageBox(NULL,(FloatToStr(Radius)).c_str(),"",NULL);
return true;
}
//---------------------------------------------------------------------------
//          ПРОВЕРКА ПОПАДАНИЯ BoundibngSphere В ПИРАМИДУ ВИДИМОСТИ
//---------------------------------------------------------------------------
bool BoundingSphere::CheckVisibility(FrustumPiramide* Frustum)
{
        // Проверка на попадание BoundingSphere в пирамиду видимости Frustum
        // Указатель на массив с плоскостями пирамиды видимости
D3DXPLANE* Planes = Frustum->GetPlanes();
        // Для каждой плоскости проверяем попадание Position сферы в пирамиду видимости с учетом радиуса сферы
for(int j=0; j<=5; j++) {
        if(Planes[j].a*Position.x+Planes[j].b*Position.y+Planes[j].c*Position.z+Planes[j].d<(-Radius)) return false;
        }
return true;
}
//---------------------------------------------------------------------------
//                             COLLISION DETECTION
//---------------------------------------------------------------------------
bool BoundingSphere::CheckIntersection(BoundingObject* BO)
{
        // Проверка на перечечение другого BoundingObject (BO) c данным  (метод
        // нужен, чтобы вызывать проверку не вникая в реальный тип BoundingObject)
bool Rez = BO->CheckIntersectionWithSphere(Position,Radius);
return Rez;
}
//---------------------------------------------------------------------------
bool BoundingSphere::CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd)
{
        // Проверка на пересечение BoundingSphere с линией заданной двумя точками
D3DXVECTOR3 LineDirection = LineEnd - LineStart;
        // Проверку делаем через решение параетрического уравнения сферы и линии
float a = LineDirection.x*LineDirection.x + LineDirection.y*LineDirection.y + LineDirection.z*LineDirection.z;
float b = 2.0f*(LineDirection.x*(LineStart.x-Position.x)+LineDirection.y*(LineStart.y-Position.y)+LineDirection.z*(LineStart.z-Position.z));
float c = (LineStart.x-Position.x)*(LineStart.x-Position.x)+(LineStart.y-Position.y)*(LineStart.y-Position.y)+(LineStart.z-Position.z)*(LineStart.z-Position.z)-Radius*Radius;
float d = b*b-4.0f*a*c;
if(d<0) return false;   // нет пересечения
else return true;       // есть пересечение
}
//---------------------------------------------------------------------------
bool BoundingSphere::CheckIntersectionWithSphere(D3DXVECTOR3 const &Position1, float Radius1)
{
        // Проверка на пересечение BoundingSphere с другой BoundingSphere
        // Проверяем чтобы расстояние между центрами сфер было >= сумме их радиусов
        // Расстояние между центрами сфер
float PositionDist = sqrt((Position1.x-Position.x)*(Position1.x-Position.x)+(Position1.y-Position.y)*(Position1.y-Position.y)+(Position1.z-Position.z)*(Position1.z-Position.z));
if(PositionDist>=Radius+Radius1) return false;  // нет пересечения
else return true;                               // есть пересечение
}
//---------------------------------------------------------------------------
bool BoundingSphere::CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX, D3DXVECTOR3 const &OBBVecY, D3DXVECTOR3 const &OBBVecZ, D3DXVECTOR3 const &PositionOBB, float OBBSizeX, float OBBSizeY, float OBBSizeZ)
{
        // Проверка на пересечение BoundingSphere с OBB
        // Проверяем по проекциям на каждую локальную ось OBB - лежит ли центр
        // сферы внутри OBB, да - есть пересечение, нет - проверяем расстояние
        // от границы OBB до центра сферы и сравниваем с ее радиусом
D3DXVECTOR3 PositionSLoc = Position - PositionOBB;      // Вектор из OBBPosition в SpherePosition
D3DXVECTOR3 LocX = OBBVecX - PositionOBB;       // Вектор локальной оси OBB Х
D3DXVec3Normalize(&LocX,&LocX);
D3DXVECTOR3 LocY = OBBVecY - PositionOBB;       // Вектор локальной оси OBB Y
D3DXVec3Normalize(&LocY,&LocY);
D3DXVECTOR3 LocZ = OBBVecZ - PositionOBB;       // Вектор локальной оси OBB Z
D3DXVec3Normalize(&LocZ,&LocZ);
float PositionSX = fabs(D3DXVec3Dot(&PositionSLoc,&LocX));      // Длина проэкции PositionSLoc на локальные вектора-оси OBB
float PositionSY = fabs(D3DXVec3Dot(&PositionSLoc,&LocY));
float PositionSZ = fabs(D3DXVec3Dot(&PositionSLoc,&LocZ));
float d = 0.0;
        // Сравниваем PositionSX, SY ,SZ с размерами OBB и с радиусом сферы
if(PositionSX>OBBSizeX) d += (PositionSX - OBBSizeX)*(PositionSX - OBBSizeX);
if(PositionSY>OBBSizeY) d += (PositionSY - OBBSizeY)*(PositionSY - OBBSizeY);
if(PositionSZ>OBBSizeZ) d += (PositionSZ - OBBSizeZ)*(PositionSZ - OBBSizeZ);
if(d>(Radius*Radius)) return false;
else return true;
}
//---------------------------------------------------------------------------
//         УПРАВЛЕНИЕ ПЕРЕМЕЩЕНИЕМ И МАСШТАБИРОВАНИЕМ BoundingSphere
//---------------------------------------------------------------------------
bool BoundingSphere::UpdateTransform(D3DXMATRIX const &TransformMatrix, float SX, float SY, float SZ)
{
        // Применить матрицу трансформации к BoundingObject, SX,SY,SZ - коэффициенты
        // масштабирования при изменении масштаба
        // Изменить положение центра
D3DXVec3TransformCoord(&Position,&Position,&TransformMatrix);
        // Изменить размеры (радиус)
float MaxScale = SX;
if(MaxScale<SY) MaxScale = SY;
if(MaxScale<SZ) MaxScale = SZ;
Radius *= MaxScale;
return true;
}
//---------------------------------------------------------------------------
