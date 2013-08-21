#pragma once
#include <windows.h>


//-------------------------------------------------------------------------------------------------
#ifndef IMPORT
#define IMPORT __declspec(dllimport)
#endif


//-------------------------------------------------------------------------------------------------
IMPORT void Startup();
IMPORT void Shutdown();


//-------------------------------------------------------------------------------------------------
class IMPORT CImage
{
public:
	CImage();
	~CImage();

	bool ReadImage(char *filename, bool c);
	bool ReadImage(void* buf, int len, bool c);
	bool ReadImage(HBITMAP hbmp, bool c);

	bool WriteImage(char *filename);

	void DrawImage(HDC hdc, int x, int y, int w, int h, DWORD argb=0xffffffff);
	
	void CreateImage(int w, int h, bool c); // width, height, color
	void DeleteImage();

	bool HasData(){ return m_Data!=NULL; }
	BYTE*GetData(){ return m_Data; }
	bool IsColor(){ return m_Color; }
	int  GetWidth(){ return m_Width; }
	int  GetHeight(){ return m_Height; }

protected:
	void ReadData(void* ptr, bool c);
	void WriteData(void* ptr);

	bool  m_Color;
	int   m_Width, m_Height;
	BYTE *m_Data;
};