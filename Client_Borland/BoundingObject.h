//---------------------------------------------------------------------------
#ifndef BoundingObjectH
#define BoundingObjectH
//---------------------------------------------------------------------------
#include "Object.h"
#include <Classes.hpp>  // Для использования стандартных С++ - типов данных (типа AnsiString)
#include "d3dx8.h"      // Подключение библиотеки D3D8X для использования ее функций
#include "FrustumPiramide.h"    // Пирамида видимости
//---------------------------------------------------------------------------
// Класс BoundingObject. Общий класс для создания Bounding-фигуры (упрощенной
// фигуры обводящей объект для упрощения вычислений с потерей точности)
//---------------------------------------------------------------------------
//class BoundingObject : public Object
class BoundingObject
{
protected:      // Доступно здесь и для потомков
        // Переменные
        D3DXVECTOR3 Position;   // Положение центра BoungingObject

private:        // Доступно только здесь
        // Переменные


public:         // Доступно везде
        BoundingObject(void);   // Констуктор класса без параметров
        ~BoundingObject();      // Деструктор класса

        // Переменные

        
        // Функции
        virtual bool CreateBy3DObject(void* Object3DVertexBuffer,int Object3DVertexCount, unsigned int Object3DVertexFVF)=0;       // Создать BoundingObject для конкретного объекта

        virtual bool CheckVisibility(FrustumPiramide* Frustum)=0;       // Проверка на попадание BoundingObject в пирамиду видимости Frustum

        virtual bool CheckIntersection(BoundingObject* BO)=0;   // Проверка на перечечение другого BoundingObject (BO) c данным (метод нужен, чтобы вызывать проверку не вникая в реальный тип BoundingObject)
        virtual bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd)=0;      // Проверка на пересечение BoundingObject с линией заданной двумя точками
        virtual bool CheckIntersectionWithSphere(D3DXVECTOR3 const &Position1, float Radius1)=0;        // Проверка на пересечение BoundingObject со сферой
        virtual bool CheckIntersectionWithOBB(D3DXVECTOR3 const &OBBVecX, D3DXVECTOR3 const &OBBVecY, D3DXVECTOR3 const &OBBVecZ, D3DXVECTOR3 const &PositionOBB, float OBBSizeX, float OBBSizeY, float OBBSizeZ)=0;    // Проверка на пересечение BoundingObject с OBB

        virtual bool UpdateTransform(D3DXMATRIX const &TransformMatrix, float SX, float SY, float SZ)=0;        // Применить матрицу трансформации к BoundingObject, SX,SY,SZ - коэффициенты масштабирования при изменении масштаба

};
//---------------------------------------------------------------------------
#endif
