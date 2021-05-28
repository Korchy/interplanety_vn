//---------------------------------------------------------------------------
#ifndef BoundingSphereH
#define BoundingSphereH
//---------------------------------------------------------------------------
#include "BoundingObject.h"
//---------------------------------------------------------------------------
// Класс BoundingSphere. Bounding-объект -> Сфера.
//---------------------------------------------------------------------------
class BoundingSphere : public BoundingObject
{
protected:      // Доступно здесь и для потомков
        // Переменные


private:        // Доступно только здесь
        // Переменные
        // Сфера описывается 2 параметрами (вектором центра Position и радиусом):
        float Radius;           // Радиус

public:         // Доступно везде
        BoundingSphere(void);   // Констуктор класса без параметров
        ~BoundingSphere();      // Деструктор класса

        // Переменные

        // Функции
        bool CreateBy3DObject(void* Object3DVertexBuffer,int Object3DVertexCount, unsigned int Object3DVertexFVF); // Вычисление BoundingSphere для переданного 3D-объекта

        bool CheckVisibility(FrustumPiramide* Frustum); // Проверка на попадание BoundingSphere в пирамиду видимости Frustum

        bool CheckIntersection(BoundingObject* BO);     // Проверка на перечечение другого BoundingObject (BO) c данным  (метод нужен, чтобы вызывать проверку не вникая в реальный тип BoundingObject)
        bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd);        // Проверка на пересечение BoundingSphere с линией заданной двумя точками
        bool CheckIntersectionWithSphere(D3DXVECTOR3 const &Position1, float Radius1);  // Проверка на пересечение BoundingSphere с BoundingSphere
        bool CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX, D3DXVECTOR3 const &OBBVecY, D3DXVECTOR3 const &OBBVecZ, D3DXVECTOR3 const &PositionOBB, float OBBSizeX, float OBBSizeY, float OBBSizeZ);       // Проверка на пересечение BoundingSphere с OBB

        bool UpdateTransform(D3DXMATRIX const &TransformMatrix, float SX, float SY, float SZ);  // Применить матрицу трансформации к BoundingObject, SX,SY,SZ - коэффициенты масштабирования при изменении масштаба

};
//---------------------------------------------------------------------------
#endif
