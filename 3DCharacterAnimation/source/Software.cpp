// Software.cpp written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to

//////////////////////
//// Software.cpp ////
//////////////////////
#include "stdafx.h"
#include "resource.h"
#include "Library.h"
#include "FileAccess.h"
#include <windowsx.h>
#include <stdio.h>
#include <assert.h>

//////////////////
//// #defines ////
//////////////////

//////////////////////////
//// Global Variables ////
//////////////////////////

char gszWindowClass[128];
long RenderType = 0;
HINSTANCE hInstSoftware = NULL;// Holds The Instance Of The Software Console 
HWND	  hWndSoftware;	   // Holds Our Software Console Window Handle


HANDLE hDXImage[2];
HANDLE hDXWinImage[2];
HANDLE hDX640Image[2];
HANDLE hDX800Image[2];
HANDLE hDX1024Image[2];
HANDLE hDX1280Image[2];

HANDLE hGLImage[2];
HANDLE hGLWinImage[2];
HANDLE hGL640Image[2];
HANDLE hGL800Image[2];
HANDLE hGL1024Image[2];
HANDLE hGL1280Image[2];

extern API_TYPE Render_ApiType;//D3D or OpenGL?
extern RES_TYPE Render_ResType;//Resolution Choice

/////////////////////////////
//// Function Prototypes ////
/////////////////////////////

bool CreateSoftwareWindow(HINSTANCE hInstance,char* title, int width, int height, int bits, bool fullscreenflag,int nCmdShow);
void ChooseOpenGLorD3DWindow();
void DeleteSoftwareWindow();
LRESULT CALLBACK WndProcSW(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void RenderConsoleBackGround(HWND hWnd,HDC hdc);
void SetApipRender_ResType(long xPos,long yPos);
void BltBitmap(HDC hdc,HDC hMemDC,HANDLE hSource,long DestX,long DestY);

///////////////////
//// Functions ////
///////////////////


bool CreateSoftwareWindow(HINSTANCE hInstance,char* title, int width, int height, int bits,int nCmdShow)
{
	char szTitle[128];
	WNDCLASSEX wcex;
	
	assert(strlen(title)<128);//prevent memory stomping
	sprintf(szTitle,"%s",title);
	sprintf(gszWindowClass,"%s",title);

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProcSW;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_RENDER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);//TANZANITE TOUCH(not +1)
	wcex.lpszMenuName	= NULL;//(LPCSTR)IDC_RENDER;
	wcex.lpszClassName	= gszWindowClass;
	wcex.hIconSm		= LoadIcon(hInstance, (LPCTSTR)IDI_SMALL);

    if(!RegisterClassEx(&wcex))
    {
        return false;
    }

	hInstSoftware = hInstance;

	hWndSoftware = CreateWindow(gszWindowClass, szTitle, WS_OVERLAPPEDWINDOW,0,0,width,height,NULL, NULL, hInstance, NULL);
	if(hWndSoftware == NULL)
	{
		return false;
	}
	ShowWindow(hWndSoftware, nCmdShow);
	return true;
}



void ChooseOpenGLorD3DWindow()
{
	MSG msg;
	HACCEL hAccelTable = NULL;
	char FullName[512];

	/////////////////////////////
	//// Load DirectX Images ////
	/////////////////////////////

	sprintf(FullName,"DirectXNotSelected.bmp");AddPathToFileName(FullName);
	hDXImage[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"DirectXSelected.bmp");AddPathToFileName(FullName);
	hDXImage[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	sprintf(FullName,"dxWina.bmp");AddPathToFileName(FullName);
	hDXWinImage[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"dxWinb.bmp");AddPathToFileName(FullName);
	hDXWinImage[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	sprintf(FullName,"dx640a.bmp");AddPathToFileName(FullName);
	hDX640Image[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"dx640b.bmp");AddPathToFileName(FullName);
	hDX640Image[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	sprintf(FullName,"dx800a.bmp");AddPathToFileName(FullName);
	hDX800Image[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"dx800b.bmp");AddPathToFileName(FullName);
	hDX800Image[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	sprintf(FullName,"dx1024a.bmp");AddPathToFileName(FullName);
	hDX1024Image[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"dx1024b.bmp");AddPathToFileName(FullName);
	hDX1024Image[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	sprintf(FullName,"dx1280a.bmp");AddPathToFileName(FullName);
	hDX1280Image[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"dx1280b.bmp");AddPathToFileName(FullName);
	hDX1280Image[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	/////////////////////
	//// Load OpenGL ////
	/////////////////////
 
	sprintf(FullName,"OpenGLNotSelected.bmp");AddPathToFileName(FullName);
	hGLImage[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"OpenGLSelected.bmp");AddPathToFileName(FullName);
	hGLImage[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	sprintf(FullName,"glWina.bmp");AddPathToFileName(FullName);
	hGLWinImage[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"glWinb.bmp");AddPathToFileName(FullName);
	hGLWinImage[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	sprintf(FullName,"gl640a.bmp");AddPathToFileName(FullName);
	hGL640Image[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"gl640b.bmp");AddPathToFileName(FullName);
	hGL640Image[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	sprintf(FullName,"gl800a.bmp");AddPathToFileName(FullName);
	hGL800Image[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"gl800b.bmp");AddPathToFileName(FullName);
	hGL800Image[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	sprintf(FullName,"gl1024a.bmp");AddPathToFileName(FullName);
	hGL1024Image[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"gl1024b.bmp");AddPathToFileName(FullName);
	hGL1024Image[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

	sprintf(FullName,"gl1280a.bmp");AddPathToFileName(FullName);
	hGL1280Image[0] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
	sprintf(FullName,"gl1280b.bmp");AddPathToFileName(FullName);
	hGL1280Image[1] = LoadImage(hInstSoftware,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 

 	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}



void DeleteSoftwareWindow()
{
	int i;

	for(i=0;i<2;i++)
	{
	    DeleteObject(hDXImage[i]);
	    DeleteObject(hDXWinImage[i]);
	    DeleteObject(hDX640Image[i]);
	    DeleteObject(hDX800Image[i]);
	    DeleteObject(hDX1024Image[i]);
	    DeleteObject(hDX1280Image[i]);
	    DeleteObject(hGLImage[i]);
	    DeleteObject(hGLWinImage[i]);
	    DeleteObject(hGL640Image[i]);
	    DeleteObject(hGL800Image[i]);
	    DeleteObject(hGL1024Image[i]);
	    DeleteObject(hGL1280Image[i]);
	}

	if (!UnregisterClass(gszWindowClass,hInstSoftware))
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstSoftware=NULL;
	}

}


LRESULT CALLBACK WndProcSW(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	long xPos,yPos;
	long fwKeys;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInstSoftware, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_KEYDOWN:
			#define VK_ESC 27
			if(((int)wParam)==VK_ESC)
			{
				Render_ApiType = AT_None;
				DestroyWindow(hWndSoftware);
				break;
			}
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			RenderConsoleBackGround(hWnd,hdc);
			EndPaint(hWnd, &ps);
			break;

 		case WM_MOUSEMOVE: 
			hdc = GetDC(hWnd);
			fwKeys = wParam;        // key flags 
			xPos = LOWORD(lParam);  // horizontal position of cursor 
			yPos = HIWORD(lParam);  // vertical position of cursor 
			SetApipRender_ResType(xPos,yPos);
			RenderConsoleBackGround(hWnd,hdc);
			break;

 		case WM_LBUTTONDOWN:
			xPos = LOWORD(lParam);  // horizontal position of cursor 
			yPos = HIWORD(lParam);  // vertical position of cursor 
			SetApipRender_ResType(xPos,yPos);
			DestroyWindow(hWndSoftware);
			break;
		case WM_DESTROY://App called DestroyWindow, also after WM_CLOSE
			PostQuitMessage(0);
			break;
		case WM_CLOSE://User hit X to close window
			Render_ApiType = AT_None;
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
void RenderConsoleBackGround(HWND hWnd,HDC hdc)
{
	HDC hMemDC;
	hMemDC  = CreateCompatibleDC(NULL);

	if(Render_ApiType == AT_Direct3D)
	{
		// Draw DirectX, and OpenGL Icon
		BltBitmap(hdc,hMemDC,hDXImage[1],0,0);
		BltBitmap(hdc,hMemDC,hGLImage[0],400,0);
		///////////////////////////////////////////////
		if(Render_ResType == RT_Window)
		{
			BltBitmap(hdc,hMemDC,hDXWinImage[1],200,0);
		}
		else
		{
			BltBitmap(hdc,hMemDC,hDXWinImage[0],200,0);
		}
		///////////////////////////////////////////////
		if(Render_ResType == RT_640)
		{
			BltBitmap(hdc,hMemDC,hDX640Image[1],200,27);
		}
		else
		{
			BltBitmap(hdc,hMemDC,hDX640Image[0],200,27);
		}
		///////////////////////////////////////////////
		if(Render_ResType == RT_800)
		{
			BltBitmap(hdc,hMemDC,hDX800Image[1],200,58);
		}
		else
		{
			BltBitmap(hdc,hMemDC,hDX800Image[0],200,58);
		}
		///////////////////////////////////////////////
		if(Render_ResType == RT_1024)
		{
			BltBitmap(hdc,hMemDC,hDX1024Image[1],200,89);
		}
		else
		{
			BltBitmap(hdc,hMemDC,hDX1024Image[0],200,89);
		}
		///////////////////////////////////////////////
		if(Render_ResType == RT_1280)
		{
			BltBitmap(hdc,hMemDC,hDX1280Image[1],200,120);
		}
		else
		{
			BltBitmap(hdc,hMemDC,hDX1280Image[0],200,120);
		}
		///////////////////////////////////////////////
	}
	else if(Render_ApiType == AT_OpenGL)
	{
		// Draw DirectX, and OpenGL Icon
		BltBitmap(hdc,hMemDC,hDXImage[0],0,0);
		BltBitmap(hdc,hMemDC,hGLImage[1],400,0);
		///////////////////////////////////////////////
		if(Render_ResType == RT_Window)
		{
			BltBitmap(hdc,hMemDC,hGLWinImage[1],200,0);
		}
		else
		{
			BltBitmap(hdc,hMemDC,hGLWinImage[0],200,0);
		}
		///////////////////////////////////////////////
		if(Render_ResType == RT_640)
		{
			BltBitmap(hdc,hMemDC,hGL640Image[1],200,27);
		}
		else
		{
			BltBitmap(hdc,hMemDC,hGL640Image[0],200,27);
		}
		///////////////////////////////////////////////
		if(Render_ResType == RT_800)
		{
			BltBitmap(hdc,hMemDC,hGL800Image[1],200,58);
		}
		else
		{
			BltBitmap(hdc,hMemDC,hGL800Image[0],200,58);
		}
		///////////////////////////////////////////////
		if(Render_ResType == RT_1024)
		{
			BltBitmap(hdc,hMemDC,hGL1024Image[1],200,89);
		}
		else
		{
			BltBitmap(hdc,hMemDC,hGL1024Image[0],200,89);
		}
		///////////////////////////////////////////////
		if(Render_ResType == RT_1280)
		{
			BltBitmap(hdc,hMemDC,hGL1280Image[1],200,120);
		}
		else
		{
			BltBitmap(hdc,hMemDC,hGL1280Image[0],200,120);
		}
		///////////////////////////////////////////////
	}
	DeleteDC(hMemDC);
}


void SetApipRender_ResType(long xPos,long yPos)
{
	//  X position selects Api

	if(xPos<300)
	{
		Render_ApiType = AT_Direct3D;
	}
	else
	{
		Render_ApiType = AT_OpenGL;
	}

	//  Y position selects Resolution

	if(yPos<27)
	{
		Render_ResType = RT_Window;
	}
	else if(yPos<58)
	{
		Render_ResType = RT_640;
	}
	else if(yPos<89)
	{
		Render_ResType = RT_800;
	}
	else if(yPos<120)
	{
		Render_ResType = RT_1024;
	}
	else
	{
		Render_ResType = RT_1280;
	}
}




void BltBitmap(HDC hdc,HDC hMemDC,HANDLE hSource,long DestX,long DestY)
{
	BOOL Result; 
	HBITMAP hTempBitmap;
	BITMAP bmData;
	hTempBitmap = SelectBitmap(hMemDC,hSource);
	GetObject(hSource,sizeof(BITMAP),&bmData);
	Result = BitBlt(hdc,DestX,DestY,DestX+bmData.bmWidth,DestY+bmData.bmHeight,hMemDC,0,0,SRCCOPY); 
	SelectBitmap(hMemDC,hTempBitmap);
}