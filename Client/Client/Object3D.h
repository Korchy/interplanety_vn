#pragma once
//---------------------------------------------------------------------------
#ifndef Object3DH
#define Object3DH
//---------------------------------------------------------------------------
// Класс Object3D. Общий класс для всех трехмерных объектов. Содержит в себе
// один 3D объект.
//---------------------------------------------------------------------------
#include "Object.h"
#include "Common.h"	// Доп. функции
//---------------------------------------------------------------------------
#define M_PI (float)3.14159265358979323846
//---------------------------------------------------------------------------
class Object3D :
	public Object
{
private:	// Только для этого класса

protected:	// Для этого класса и потомков
	// Переменные
	LPD3DXMESH Mesh;		// Собственно 3D-объект (сетка)

	LPDIRECT3DVERTEXBUFFER8 VertexBuffer;	// Буфер точек 3D-объекта (указатель на VertexBuffer в видео-памяти)
	void* WorkVertexBuffer;					// Копия VertexBuffer в системной памяти (для убыстрения работы с ним - чтобы не копировать туда-сюда)
	short SizeOfVertex;						// Размер памяти в байтах, необходимый для хранения информации об одной точке 3D-объекта
	unsigned int VertexCount;				// Количество точек объекта
	struct VertexType1 {					// Структура описывающая одну точку объекта. Несколько типов т.к. точки объекта могут иметь разные форматы
	D3DXVECTOR3 Position;    
	};
	struct VertexType2 {
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Normal;
		D3DXVECTOR2 Texture;
	};
	struct VertexType3 {
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Normal;
	};
	struct VertexType4 {
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Texture;
	};
	enum {vsType1,vsType2,vsType3,vsType4} VertexType;	// Конкретный тип использующейся структуры для описания одной точки объекта
	unsigned int VertexFVF;		// FVF-формат использующийся для описания одной точки объекта

	LPDIRECT3DINDEXBUFFER8 IndexBuffer;	// Буфер индексов точек 3D-объекта (указатель на IndexBuffer в видео-памяти)
	unsigned short* WorkIndexBuffer;	// Копия IndexBuffer в системной памяти (для убыстрения работы с ним)
	unsigned int IndexCount;			// Количество индексов объекта

	DWORD MaterialsCount;		// Кол-во материалов в 3D-объекте
	D3DMATERIAL8* Materials;	// Указатель на массив с материалами объекта

	LPDIRECT3DTEXTURE8* Textures;		// Указатель на массив с текстурами объекта

	// 4 вектора для обеспечения передвижения и трансформации объекта:
	// 1 вектор, описывающий положение объекта
	D3DXVECTOR3 Position;	// Положение
	// и 3 единичными векторами, обеспечивающими направления перемещения и вращение объекта
	D3DXVECTOR3 VecX;		// Вдоль оси Х
	D3DXVECTOR3 VecY;		// Вдоль оси Y
	D3DXVECTOR3 VecZ;		// Вдоль оси Z

	D3DXMATRIX TransformMatrix;	// Матрица трансформации, собирает (суммирует) в себе все изменения положения/поворота/масштабирования объекта до выполнения UpdateTransform

	std::string AlternativeFile;	// Альтернативный файл, который загружается в LoadFromFile, если указанного файла не существует

public:		// Доступно всем
	Object3D(void);
	~Object3D(void);

	// Переменные

	// Функции
	bool LoadFromFile(std::string File);	// Загрузка объекта из файла
	bool Draw();	// Вывод объекта в сцену

	bool MoveTo(float X, float Y, float Z);		// Перемещение объекта по глобальным осям X,Y,Z на нужное расстояние
	bool MoveInto(float X, float Y, float Z);	// Перемещение объекта в точку с координатами X,Y,Z в глобальной системе координат
	bool MoveX(float X);						// Перемещение объекта вдоль его локальной оси X на нужное расстояние
	bool MoveY(float Y);						// Перемещение объекта вдоль его локальной оси Y на нужное расстояние
	bool MoveZ(float Z);						// Перемещение объекта вдоль его локальной оси Z на нужное расстояние
	bool RotateX(float Angle);					// Поворот объекта вокруг собственной оси Х на угол Angle
	bool RotateY(float Angle);					// Поворот объекта вокруг собственной оси Y на угол Angle
	bool RotateZ(float Angle);					// Поворот объекта вокруг собственной оси Z на угол Angle
	bool Scale(float SX, float SY, float SZ);	// Масштабирование объекта по осям
	bool UpdateTransform();						// Обновить объект т.е. применить к нему полученные трансформации

	bool CheckIntersectionWithLine(D3DXVECTOR3 const &LineStart,D3DXVECTOR3 const &LineEnd, float* Distance);	// Проверка на пересечение отрезка заданного точками LineStart и LineEnd хотя бы с одним полигоном объекта, возвращает true если пересечение есть, false - нет, Distance - выходной параметр = расстоянию до точки пересечения

	virtual bool CheckVisibility(); // Проверка видимости объекта
};
#endif