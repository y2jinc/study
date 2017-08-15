//.................................................................................................
//				.......    ..     . .......      ..     . ..... ..............					 ..
//				.  .  .   ....    .      .      ....    .   .   .  .  ..     .					 ..
//				   .     . .. .   .     .      . .. .   .   .      .   .						 ..
//				   .    .  ..  .  .    .      .  ..  .  .   .      .   ....						 ..
//				   .   ......   . .   .      ......   . .   .      .   .    					 ..
//				   .  .    ..    ..  .      .    ..    ..   .      .   .     .					 ..
//				   . .     ..     . ........     ..     . .....    .   .......					 ..
//                 Copyright (c) 2002 Tanzanite Software. All rights reserved.					 ..
//.................................................................................................
//	Render.cpp written by david paull.
// 	Copyright (c) 2002 Tanzanite Software. All rights reserved.
// 	The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// 	work in progress : For latest version goto www.tanzanite.to

//CAMERAPOSITIONSTART
//CALCULATEFINALMESHMATRIX
////////////////////
//// Render.cpp ////
////////////////////

#include "stdafx.h"
#include "resource.h"	//IDI_RENDER
#include "Library.h"
#include "FileAccess.h"
#include <math.h>		//sqrt,fabs,acos
#include <assert.h>
#include <stdio.h>		//sprintf
#include "FieldLL.h"
#include "IconLL.h"
#include "RectLL.h"
#include "SceneLL.h"
#include "ObjectLL.h"
#include "MeshLL.h"
#include "SkeletonLL.h"
#include "KeyFrameLL.h"
#include "JointLL.h"
#include "Software.h"

//////////////////
//// #defines ////
//////////////////

#define SHOW_API_SELECT

///////////////////////////////////////////
//// Rendering dll Function Prototypes ////
///////////////////////////////////////////

HWND (__stdcall* API_CreateWindow)(HINSTANCE hInstance,WNDPROC WndProc,HICON hicon,char* title,int bits,long& Width,long& Height,bool windowedflag);
void (__stdcall* API_ReSizeWindow)(long Width,long Height);
void (__stdcall* API_DeleteWindow)();
void (__stdcall* API_BeginScene  )(long ClearColor);
void (__stdcall* API_RenderMesh  )(struct MeshLL* pMesh,float* pWorld,float* pView,float* pProj,float* pOrtho);
void (__stdcall* API_EndScene    )();
void (__stdcall* API_WndProc     )(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


/////////////////////////////////////////
//////// Extern Global Variables ////////
/////////////////////////////////////////

////////////////////////////////////////////
//////// Extern Function Prototypes ////////
////////////////////////////////////////////

//////////////////////////////////////////
//////////// Global Variables ////////////
//////////////////////////////////////////

FILE* DiskFile;
char* gp_TextBuffer1 = NULL;
char* gp_TextBuffer2 = NULL;
char* gp_TextBuffer3 = NULL;

// LINKED LIST HEADS 
struct SceneLL* pRender_SceneHead = NULL;//All scenes, objects, and meshes in this LL
struct FieldLL* pRender_FieldHead = NULL;//All text is in this LL
struct IconLL* 	pRender_IconHead  = NULL;//All icons are in this LL
struct RectLL* 	pRender_RectHead  = NULL;//overlapping background rectangles, for menu

WNDPROC 		gp_App_WndProc;

long 	  		 Render_ClearColor = 0xff000000;
float 	  		pRender_IdentityMatrix[16];
float 	  		pRender_ViewMatrix[16];
float 	  		pRender_ProjMatrix[16];
float 	  		pRender_OrthoMatrix[16];
float* 			pRender_MatrixArray[256];//for Skinning code in ObjectLL.cpp
float 	  		 Render_NearPlane = 2.0;
float 	  		 Render_FarPlane  = 15000.0;
float 	  		 Render_FOV = 1.5707;
HINSTANCE 		hRender_DLL = NULL;
HWND   	  		 Render_hWnd = NULL;
bool 	  		 Render_WindowedFlag = false;//always false at start, or undefined behavour
float 	  		 Render_WindowWidth  = 1024;
float 	  		 Render_WindowHeight = 768;
API_TYPE 		 Render_ApiType = AT_Direct3D;//D3D or OpenGL?
RES_TYPE         Render_ResType = RT_Window;//Resolution Choice
RESOLUTIONDATA   Render_ResolutionData[5];


/////////////////////////////
//// Function Prototypes ////
/////////////////////////////
LRESULT CALLBACK Render_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render_ReSizeWindow();


///////////////////
//// Functions ////
///////////////////


bool Render_Create(	HINSTANCE hInstance,
					WNDPROC App_WndProc)
{
	long i;
	RECT WindowRect;

	InitDebugMsg();

	for(i=0;i<256;i++)
	{
		pRender_MatrixArray[i] = (float *)calloc(16,sizeof(float));
		Identity(pRender_MatrixArray[i]);
	}

	gp_TextBuffer1 = (char *)calloc(MAX_PATHNAME,sizeof(char));
	gp_TextBuffer2 = (char *)calloc(MAX_PATHNAME,sizeof(char));
	gp_TextBuffer3 = (char *)calloc(MAX_PATHNAME,sizeof(char));

	GetSeconds(true);

	Render_ResolutionData[RT_Window].width		= 800;
	Render_ResolutionData[RT_Window].height 	= 600;
	Render_ResolutionData[RT_Window].aspectratio= 0.75;

	Render_ResolutionData[RT_640].width			= 640.0;
	Render_ResolutionData[RT_640].height 		= 480.0;
	Render_ResolutionData[RT_640].aspectratio 	= 0.75;

	Render_ResolutionData[RT_800].width			= 800.0;
	Render_ResolutionData[RT_800].height 		= 600.0;
	Render_ResolutionData[RT_800].aspectratio 	= 0.75;

	Render_ResolutionData[RT_1024].width		= 1024.0;
	Render_ResolutionData[RT_1024].height 		= 768.0;
	Render_ResolutionData[RT_1024].aspectratio 	= 0.75;

	Render_ResolutionData[RT_1280].width		= 1280.0;
	Render_ResolutionData[RT_1280].height 		= 1024.0;
	Render_ResolutionData[RT_1280].aspectratio 	= 0.8;


	//// APIWrapper creates window with render.cpp's WndProc
	//// render.cpp's WndProc calls App.cpp's App_WndProc
	//// App.cpp's App_WndProc calls the default DefWindowProc(hwnd, uMsg, wParam, lParam);
	gp_App_WndProc = App_WndProc;

	//HACKHACK BUGBUG
	Render_ApiType = AT_Direct3D;
	//HACKHACK BUGBUG

	#ifdef SHOW_API_SELECT
	CreateSoftwareWindow(hInstance,"Choose API, and Resolution",600+8,150+31,32,SW_SHOWNORMAL);
	ChooseOpenGLorD3DWindow();
	DeleteSoftwareWindow();
	#endif

	//HACKHACK BUGBUG
	int debugger1 = 0;
	if(debugger1==1)//APISELECTHERE
	{
		Render_ApiType = AT_OpenGL;
	}
	//HACKHACK BUGBUG

	///////////////////////
	//// Create Window ////
	///////////////////////

	ShowCursor(FALSE);

	Render_WindowWidth  = Render_ResolutionData[Render_ResType].width;
	Render_WindowHeight = Render_ResolutionData[Render_ResType].height;
	
	if(Render_ResType==RT_Window)
	{
		Render_WindowedFlag = true;
	}

	//////////////////////////////////
	//// Connect To Rendering DLL ////
	//////////////////////////////////

	char pWindowName[512];
	char pDLLName[512];

	if(Render_ApiType == AT_Direct3D)
	{
		sprintf(pWindowName,"Direct3D Window");
		sprintf(pDLLName,"Direct3DDLL.dll");
		AddPathToFileName(pDLLName);
	}
	else if(Render_ApiType == AT_OpenGL)
	{
		sprintf(pWindowName,"OpenGL Window");
		sprintf(pDLLName,"OpenGLDLL.dll");
		AddPathToFileName(pDLLName);
	}
	else
	{
		return false;
	}

	hRender_DLL = LoadLibrary(pDLLName);
	if(hRender_DLL==NULL)
	{
		AppendDebugMsg("Rendering DLL not found:hRender_DLL==NULL");
		return false;
	}

	////////////////////////////////////////////////////////////////
	//// Get Function Pointers To the Rendering DLL's Functions ////
	////////////////////////////////////////////////////////////////

	*(void**)&API_CreateWindow=(void*)GetProcAddress(hRender_DLL,"API_CreateWindow");
	*(void**)&API_ReSizeWindow=(void*)GetProcAddress(hRender_DLL,"API_ReSizeWindow");
	*(void**)&API_DeleteWindow=(void*)GetProcAddress(hRender_DLL,"API_DeleteWindow");
	*(void**)&API_BeginScene  =(void*)GetProcAddress(hRender_DLL,"API_BeginScene");
	*(void**)&API_RenderMesh  =(void*)GetProcAddress(hRender_DLL,"API_RenderMesh");
	*(void**)&API_EndScene    =(void*)GetProcAddress(hRender_DLL,"API_EndScene");
	*(void**)&API_WndProc     =(void*)GetProcAddress(hRender_DLL,"API_WndProc");

	////////////////////////
	//// Initialize API ////
	////////////////////////

	HICON hicon = LoadIcon(hInstance,(LPCTSTR)IDI_RENDER);

	long TempWidth =(long)Render_WindowWidth;
	long TempHeight=(long)Render_WindowHeight;

	Render_hWnd = API_CreateWindow(
						hInstance,
						(WNDPROC)Render_WndProc,
						hicon,
						pWindowName,
						32,
						TempWidth ,
						TempHeight,
						Render_WindowedFlag);

	GetClientRect(Render_hWnd,&WindowRect);
    Render_WindowWidth  = (float)(WindowRect.right - WindowRect.left);
    Render_WindowHeight = (float)(WindowRect.bottom - WindowRect.top);

	////////////////
	//// Resize ////
	////////////////

	Render_ReSizeWindow();

	////////////////////////////////
	//// Create Identity Matrix ////
	////////////////////////////////

	Identity(pRender_IdentityMatrix);

	////////////////////////////
	//// Create View Matrix ////
	////////////////////////////
	//CAMERAPOSITIONSTART

	float pIdentityMatrix[16];
	Identity(pIdentityMatrix);
	PositionMatrix(pIdentityMatrix,0,0,-115);
	InverseMatrix(pRender_ViewMatrix,pIdentityMatrix);


	return true;
}




void Render_BeginScene()
{
	/////////////////////
	//// Begin Scene ////
	/////////////////////
	API_BeginScene(Render_ClearColor);
}

void Render_EndScene()
{
	///////////////////
	//// End Scene ////
	///////////////////
	API_EndScene();
}



//pScene can be null for Text meshes
//KEYWORD:RENDER_TRANSFORMED_VERTEXSELECTION
//CALCULATEFINALMESHMATRIX
void Render_Mesh(struct SceneLL* pScene,
				struct ObjectLL* pObject,
				struct MeshLL* pMesh)
{
	float pFinalModelMatrix[16];

	if(pObject==NULL)return;
	if(pMesh==NULL)return;
	
	MatrixMul(pFinalModelMatrix,pMesh->pMatrix,pObject->pMatrix);
	API_RenderMesh(	pMesh,
					pFinalModelMatrix,
					pRender_ViewMatrix,
					pRender_ProjMatrix,
					pRender_OrthoMatrix);
}

void Render_ReSizeWindow()
{
	CreateProjectionMatrix(	pRender_ProjMatrix,
						   	Render_FOV,//0.785398163397,
						   	Render_NearPlane,
						   	Render_FarPlane);
	CreateOrthoMatrix(pRender_OrthoMatrix);

	API_ReSizeWindow((long)Render_WindowWidth,(long)Render_WindowHeight);
}

void Render_Delete()
{
	long i;

	for(i=0;i<256;i++)
	{
		SAFE_FREE(pRender_MatrixArray[i]);
	}

	SAFE_FREE(gp_TextBuffer1);
	SAFE_FREE(gp_TextBuffer2);
	SAFE_FREE(gp_TextBuffer3);

	SceneLL_RemoveALL(pRender_SceneHead);

	API_DeleteWindow();

	FreeLibrary(hRender_DLL);

}


LRESULT CALLBACK Render_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	struct SceneLL* pScene = pRender_SceneHead;
	struct FieldLL* pField = NULL;
	FieldLL* Temp = NULL;
	RECT WindowRect;

	POINT ptCursor;
	GetCursorPos( &ptCursor );
	ScreenToClient(hwnd,&ptCursor);

	if(ptCursor.x < 0 || ptCursor.x >= Render_WindowWidth ||
	   ptCursor.y < 0 || ptCursor.y >= Render_WindowHeight)
	{
		ShowCursor(TRUE);
	}
	else
	{
		ShowCursor(FALSE);
	}



    switch (uMsg)
    {
		case WM_SIZE:
			if(Render_ResType == RT_Window)
			{
				if(wParam==SIZE_MAXIMIZED || wParam==SIZE_RESTORED)
				{
					GetClientRect(hwnd,&WindowRect);
    				Render_WindowWidth  = (float)(WindowRect.right - WindowRect.left);
    				Render_WindowHeight = (float)(WindowRect.bottom - WindowRect.top);
					Render_ReSizeWindow();
		    	    break;
				}
			}
			break;
		case WM_EXITSIZEMOVE:
			if(Render_ResType == RT_Window)
			{
				GetClientRect(hwnd,&WindowRect);
    			Render_WindowWidth  = (float)(WindowRect.right - WindowRect.left);
    			Render_WindowHeight = (float)(WindowRect.bottom - WindowRect.top);
				Render_ReSizeWindow();
			}
	        break;



		case WM_ACTIVATE:
			if(Render_WindowedFlag==false)
			{//FULLSCREEN CODE
				WORD fActive = LOWORD(wParam);           // activation flag 
				if(fActive==WA_ACTIVE)
				{
				}
				else if(fActive==WA_INACTIVE &&
						Render_ApiType == AT_OpenGL)
				{//Direct3D version doesnt lose its texture memory during alt+tab

					//OutputDebugString("Render - WA_INACTIVE\n");

					struct SceneLL* pScene  = NULL;
					pScene = pRender_SceneHead;
					while(pScene)
					{
						SceneLL_SetInVRAM(pScene,false);
						pScene = pScene->pNext;
					}

				}
			}
		break;


	    default:
	        break;
    }
	return gp_App_WndProc(hwnd, uMsg, wParam, lParam);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

