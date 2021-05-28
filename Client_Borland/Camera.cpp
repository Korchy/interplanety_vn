//---------------------------------------------------------------------------
#pragma hdrstop

#include "Camera.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
Camera::Camera(void)
{
        // Конструктор класса без параметров
pD3DDevice = NULL;
        // Поместить камеру в начальное положение
DefaultPosition();
        // Матрица трансформации по началу "единичная" (нет изменений)
D3DXMatrixIdentity(&TransformMatrix);
        // Скорость движения камеры
ScrollSpeed = 1.0;      // Приемлено 0.1 ед. за 1 мс. 
        // Пирамида видимости
Frustum = NULL;
        // Указатель на SkySphere (Фон)
SkyBox = NULL;
}
//---------------------------------------------------------------------------
Camera::~Camera()
{
        // Деструктор класса
if(Frustum!=NULL) delete Frustum;       // Удалить пирамиду видимости
}
//---------------------------------------------------------------------------
//                              ОБЩИЕ ФУНКЦИИ
//---------------------------------------------------------------------------
bool Camera::SetRenderDevice(LPDIRECT3DDEVICE8 Device)
{
        // Назначается устройство рендеринга
if(Device!=NULL) {
        pD3DDevice = Device;
        Frustum = new FrustumPiramide();        // Создать Пирамиду видимости
        return true;
        }
else {
        return false;
        }
}
//---------------------------------------------------------------------------
bool Camera::SetSkyBox(ObjectSkyBox* SkyBoxObject)
{
        // Назначение объекта SkySphere (фон)
SkyBox = SkyBoxObject;
if(SkyBox!=NULL) {      // Притянуть центр SkySphere к Position камеры
//        SkyBox->MoveInto(Position.x,Position.y,Position.z);
        D3DXVECTOR3 MoveSkyBoxVec;
//        MoveSkyBoxVec = VecZ - Position;
//        MoveSkyBoxVec = Position+D3DXVECTOR3(0.0f,0.0f,1.0f)*SkyBox->R;
        MoveSkyBoxVec = Position+(VecZ - Position)*SkyBox->R;
        SkyBox->MoveInto(MoveSkyBoxVec.x,MoveSkyBoxVec.y,MoveSkyBoxVec.z);
        SkyBox->UpdateTransform();
        }
return true;
}
//---------------------------------------------------------------------------
void Camera::SetScrollSpeed(float NewSpeed)
{
        // Установка скорости скроллирования для камеры
        // Имеет смысл устанавливать равной 1/FPS
ScrollSpeed = NewSpeed;
}
//---------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetPosition()
{
        // Возвращает текущее положение камеры
return Position;
}
//---------------------------------------------------------------------------
bool Camera::ScrollLeft()
{
        // Перемешение камеры вдоль оси Х налево
        // Создать временный вектор, описывающий перемещение из текущей точки в нужную точку
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecX,&Position);
MoveVector *= -ScrollSpeed;
        // Переместить камеру в новую точку согласно полученному вектору
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
//MoveTo(-ScrollSpeed,0,0);        // В (локальной) системе координат камеры
return true;
}
//---------------------------------------------------------------------------
bool Camera::ScrollRight()
{
        // Перемешение камеры вдоль оси Х направо
        // Создать временный вектор, описывающий перемещение из текущей точки в нужную точку
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecX,&Position);
MoveVector *= ScrollSpeed;
        // Переместить камеру в новую точку согласно полученному вектору
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
//MoveTo(ScrollSpeed,0,0);        // В (локальной) системе координат камеры
return true;
}
//---------------------------------------------------------------------------
bool Camera::ScrollUp()
{
        // Перемешение камеры вдоль оси Y вверх
        // Создать временный вектор, описывающий перемещение из текущей точки в нужную точку
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecY,&Position);
MoveVector *= ScrollSpeed;
        // Переместить камеру в новую точку согласно полученному вектору
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
//MoveTo(1,0,0);        // В (локальной) системе координат камеры
return true;
}
//---------------------------------------------------------------------------
bool Camera::ScrollDown()
{
        // Перемешение камеры вдоль оси Y вниз
        // Создать временный вектор, описывающий перемещение из текущей точки в нужную точку
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecY,&Position);
MoveVector *= -ScrollSpeed;
        // Переместить камеру в новую точку согласно полученному вектору
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
//MoveTo(1,0,0);        // В (локальной) системе координат камеры
return true;
}
//---------------------------------------------------------------------------
bool Camera::Scale(bool Increase)
{
        // Перемешение камеры вдоль оси Z
        // Создать временный вектор, описывающий перемещение из текущей точки в нужную точку
D3DXVECTOR3 MoveVector;
D3DXVec3Subtract(&MoveVector,&VecZ,&Position);
if(Increase==true) MoveVector *= ScrollSpeed;
else MoveVector *= -ScrollSpeed;
        // Переместить камеру в новую точку согласно полученному вектору
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
//MoveTo(1,0,0);        // В (локальной) системе координат камеры
return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateRight()
{
        // Поворот камеры вокруг оси Y направо. Чтобы она поворачивалась вокруг
        // своей оси, а не центра координат - поворачиваем в 3 этапа: создаем
        // матрицу переноса в центр координат, создаем матрицу поворота,
        // создаем матрицу переноса обратно в текущую позицию
        // Перенести в 0,0,0
D3DXVECTOR3 Vec000;     // Вектор указывающий в 0,0,0 в мировой системе координат
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector; // Vec000 перобразованный в систему координат камеры
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
        // Поворачиваем камеру
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationY(&Rot,M_PI/180);      // на 1 градус (180 гр. = PI рад.)
TransformMatrix *= Rot;
        // Вернуть камеру обратно
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateLeft()
{
        // Поворот камеры вокруг оси Y налево. Чтобы она поворачивалась вокруг
        // своей оси, а не центра координат - поворачиваем в 3 этапа: создаем
        // матрицу переноса в центр координат, создаем матрицу поворота,
        // создаем матрицу переноса обратно в текущую позицию
        // Перенести в 0,0,0
D3DXVECTOR3 Vec000;     // Вектор указывающий в 0,0,0 в мировой системе координат
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector; // Vec000 перобразованный в систему координат камеры
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
        // Поворачиваем камеру
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationY(&Rot,-M_PI/180);    // на 1 градус (180 гр. = PI рад.)
TransformMatrix *= Rot;
        // Вернуть камеру обратно
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateUp()
{
        // Поворот камеры вокруг оси Х вверх. Чтобы она поворачивалась вокруг
        // своей оси, а не центра координат - поворачиваем в 3 этапа: создаем
        // матрицу переноса в центр координат, создаем матрицу поворота,
        // создаем матрицу переноса обратно в текущую позицию
        // Перенести в 0,0,0
D3DXVECTOR3 Vec000;     // Вектор указывающий в 0,0,0 в мировой системе координат
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector; // Vec000 перобразованный в систему координат камеры
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
        // Поворачиваем камеру
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationX(&Rot,-M_PI/180);    // на 1 градус (180 гр. = PI рад.)
TransformMatrix *= Rot;
        // Вернуть камеру обратно
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateDown()
{
        // Поворот камеры вокруг оси Х вниз. Чтобы она поворачивалась вокруг
        // своей оси, а не центра координат - поворачиваем в 3 этапа: создаем
        // матрицу переноса в центр координат, создаем матрицу поворота,
        // создаем матрицу переноса обратно в текущую позицию
        // Перенести в 0,0,0
D3DXVECTOR3 Vec000;     // Вектор указывающий в 0,0,0 в мировой системе координат
Vec000 = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 MoveVector; // Vec000 перобразованный в систему координат камеры
D3DXVec3Subtract(&MoveVector,&Vec000,&Position);
MoveTo(MoveVector.x,MoveVector.y,MoveVector.z);
        // Поворачиваем камеру
D3DXMATRIX Rot;
D3DXMatrixIdentity(&Rot);
D3DXMatrixRotationX(&Rot,M_PI/180);     // на 1 градус (180 гр. = PI рад.)
TransformMatrix *= Rot;
        // Вернуть камеру обратно
MoveTo(Position.x,Position.y,Position.z);
return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateRightOrtho()
{
        // Поворот камеры вокруг оси Y направо для ортогональной проекции
        // По логике нужно вместе с поворотом отодвигать камеру назад, чтобы
        // объекты не резались плоскостью камеры

        // Для ортогональной пока поворота нет смысла делать

return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateLeftOrtho()
{
        // Поворот камеры вокруг оси Y налево для ортогональной проекции
        // По логике нужно вместе с поворотом отодвигать камеру назад, чтобы
        // объекты не резались плоскостью камеры

return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateUpOrtho()
{
        // Поворот камеры вокруг оси Х вверх для ортогональной проекции
        // По логике нужно вместе с поворотом отодвигать камеру назад, чтобы
        // объекты не резались плоскостью камеры

return true;
}
//---------------------------------------------------------------------------
bool Camera::RotateDownOrtho()
{
        // Поворот камеры вокруг оси Х вниз для ортогональной проекции
        // По логике нужно вместе с поворотом отодвигать камеру назад, чтобы
        // объекты не резались плоскостью камеры

return true;
}
//---------------------------------------------------------------------------
bool Camera::UpdateWorld()
{
if(pD3DDevice==NULL) return false;
        // Переместить мировую камеру к текущей
        // Изменить вектора, описывающие камеру в соответствии с полученной матрицей трансформации
D3DXVec3TransformCoord(&Position,&Position,&TransformMatrix);
D3DXVec3TransformCoord(&Target,&Target,&TransformMatrix);
D3DXVec3TransformNormal(&Up,&Up,&TransformMatrix);
D3DXVec3TransformCoord(&VecX,&VecX,&TransformMatrix);
D3DXVec3TransformCoord(&VecY,&VecY,&TransformMatrix);
D3DXVec3TransformCoord(&VecZ,&VecZ,&TransformMatrix);
        // Очистить матрицу трансформации
D3DXMatrixIdentity(&TransformMatrix);
        // Сбросить в NewPosition новые значения трех векторов, задающих новое положение камеры
D3DXMATRIX NewPosition;
D3DXMatrixLookAtLH(&NewPosition,&Position,&Target,&Up);
        // Применить NewPosition к мировой камере
pD3DDevice->SetTransform(D3DTS_VIEW,&NewPosition);
        // Обновить пирамиду видимости для камеры
D3DXMATRIX ProjMatrix;  // Получить Проекционную матрицу
pD3DDevice->GetTransform(D3DTS_PROJECTION,(D3DMATRIX*)&ProjMatrix);
Frustum->Update(ProjMatrix,NewPosition);        // Видовая матрица = NewPosition
        // Обновить положение SkyBox
if(SkyBox!=NULL) {
//        SkyBox->MoveInto(Position.x,Position.y,Position.z);
        D3DXVECTOR3 MoveSkyBoxVec;
//        MoveSkyBoxVec = Position+D3DXVECTOR3(0.0f,0.0f,1.0f)*SkyBox->R;
        MoveSkyBoxVec = Position+(VecZ - Position)*SkyBox->R;
        SkyBox->MoveInto(MoveSkyBoxVec.x,MoveSkyBoxVec.y,MoveSkyBoxVec.z);
        SkyBox->UpdateTransform();
        }
return true;
}
//---------------------------------------------------------------------------
bool Camera::DefaultPosition()
{
        // Вернуть камеру в начальное положение
        // Начальное положение камеры
Position = D3DXVECTOR3(0.0f,0.0f,-100.0f);
Target = D3DXVECTOR3(0.0f,0.0f,0.0f);
Up = D3DXVECTOR3(0.0f,1.0f,0.0f);
VecX = D3DXVECTOR3(1.0f,0.0f,-100.0f);
VecY = D3DXVECTOR3(0.0f,1.0f,-100.0f);
VecZ = D3DXVECTOR3(0.0f,0.0f,-99.0f);
return true;
}
//---------------------------------------------------------------------------
//                            ВНУТРЕННИЕ ФУНКЦИИ
//---------------------------------------------------------------------------
bool Camera::MoveTo(float X, float Y, float Z)
{
        // Перемещение камеры в точку с координатами X,Y,Z в глобальной
        // системе координат
D3DXMATRIX Move;
D3DXMatrixIdentity(&Move);
D3DXMatrixTranslation(&Move,X,Y,Z);
TransformMatrix *= Move;
return true;
}
//---------------------------------------------------------------------------

