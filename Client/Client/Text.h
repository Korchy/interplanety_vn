#pragma once
//---------------------------------------------------------------------------
#ifndef TextH
#define TextH
//---------------------------------------------------------------------------
// ????? ??? ?????? ?????? ? ??????? ??????
//---------------------------------------------------------------------------
#include "Object.h"		// ????? - ??????
#include <string>		// ??? ????????????? std::string
//---------------------------------------------------------------------------
class Text :
	public Object
{
private:	// ?????? ??? ????? ??????
	// ??????????
	D3DCOLOR Color;		// ???? ?????????? ??????
	DWORD Format;		// ?????? ?????? ??????
	LPD3DXFONT Font;	// ?????, ??????? ????????? ?????
	RECT Rect;			// ??????? ?? ?????? ???? RECT, ?????????????? ????? ??????

protected:	// ??? ????? ?????? ? ????????

public:		// ???????? ????
	Text(void);		// ??????????? ??? ??????????
	Text(LPDIRECT3DDEVICE8 Device);	// ?????????? ?????? (? ?????????? - ????? ????????????? ????? ?????? ?? ????????? ????? ?????????? ??????????)
	~Text(void);	// ??????????

	// ??????????
	std::string Txt;	// ?????????? ?????, ??????? ????? ?????????? ?? ?????

	// ???????
	bool Draw();		// ????? ?????? ? ?????
	bool Reset();		// ???????? ?????? (??? ????????????????? ?????????? ??????????)
	bool Restore();		// ???????????? ?????? ????? ????, ??? ?? ??? ???????
	bool SetTextFont(short Size,int Bold,bool Italic,bool Underline,std::string Face);	// ???????????? ?????? ??? ??????
	bool SetRect(int Left,int Top,int Right,int Bottom);								// ?????????? ??????? ?????? ??????
	bool SetColor(D3DCOLOR NewColor);													// ?????????? ???? ??????
	bool SetFormat(DWORD Fmt);															// ?????????? ?????? ??????
};
#endif