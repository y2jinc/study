//Direct3D.cpp written by david paull.
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
//////////////////////
//// Direct3d.cpp ////
//////////////////////
#include "stdafx.h"
//#include "resource.h"
#include "..\library.h"//remove this?
#include "Direct3DTextureLL.h"
#include <windowsx.h>
#include <stdio.h>
#include <d3d8.h>
//#include <Dxerr8.h>//DXGetErrorString8 requires 
#include <assert.h>


//#define POINTSAMPLE
#include <D3DX8.h>

//////////////////
//// #defines ////
//////////////////

#define D3DFVF_FATVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)


/////////////////////////
//// TypeDef Structs ////
/////////////////////////

void Identity(float *pMat);
VOID D3DUtil_SetRotateXMatrix( D3DMATRIX& mat, FLOAT fRads );
VOID D3DUtil_SetRotateYMatrix( D3DMATRIX& mat, FLOAT fRads );
VOID D3DUtil_SetRotateZMatrix( D3DMATRIX& mat, FLOAT fRads );
HRESULT D3DMath_MatrixInvert( D3DMATRIX& q, D3DMATRIX& a );
void __stdcall Direct3D_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//////////////////////////
//// Global Variables ////
//////////////////////////
//D3DXMATRIX View,Proj;

WNDPROC gp_Render_WndProc;

D3DXMATRIX    m_matObj1;
#define NumberOfErrorCodes 21
long ErrorNum[NumberOfErrorCodes];
char DebugText[512];

char gszWindowClass[128];



float 		Direct3D_WindowWidth;
float 		Direct3D_WindowHeight;
bool  		Direct3D_windowedflag = false;
HINSTANCE 	Direct3D_hInst = NULL;
HWND	  	Direct3D_hWnd;// Holds Our Direct3D Console Window Handle

//long d3d_BackBufferWidth  = 0;
//long d3d_BackBufferHeight = 0;




bool d3d_BeginScene = false;//indicates if begin scene succeded. If not, dont render


struct Direct3DTextureLL* pDirect3DTextureHead = NULL;
LPDIRECT3DDEVICE8       pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER8 m_pwVB = NULL;//World Vertex Buffer XYZNUV
LPDIRECT3DINDEXBUFFER8  m_pIB = NULL;//Indices
//HINSTANCE hInstDirect3D = NULL;// Holds The Instance Of The Direct3D Console 
//HWND	  hWndDirect3D;	   // Holds Our Direct3D Console Window Handle
float CameraDistance = 10.0;



/////////////////////////////
//// Function Prototypes ////
/////////////////////////////
//void CreateOrthoMatrixD3D(float* pMat);
D3D_ERROR Render3dMeshD3D(struct MeshLL* pMesh);
bool RestoreVertexBuffers();

bool LoadTextureIntoVRAMD3D(aTEXTURE& Texture);

void ConvertVeiwMatrixToOpenGL(float* pMat);
void LookAtD3D(float* pMat,aVECTOR* pPos,aVECTOR* pLookAt);

void AxisRotationMatrix(float *pMat,float Ax,float Ay,float Az,float Angle);

void CreateProjectionMatrixD3D(D3DMATRIX& mat,float fov,float nearplane);
HWND CreateDirect3DWindow(HINSTANCE hInstance,char* title, int width, int height, int bits, bool windowed,int nCmdShow);
void UpdateDirect3DWindow();
void DeleteDirect3DWindow();

bool CreateRenderD3D(long Width, long Height);
bool RenderTestObject();

void FreeVertexBuffers();

char* GetError(HRESULT Error);
void MakeError();
VOID D3DUtil_InitMaterial( 	D3DMATERIAL8& mtrl, 
							FLOAT dr, 
							FLOAT dg, 
							FLOAT db,
                           	FLOAT da,
							FLOAT ar, 
							FLOAT ag, 
							FLOAT ab,
                           	FLOAT aa );


///////////////////
//// Functions ////
///////////////////

typedef struct _MYVERTEX
{
    FLOAT     x;
    FLOAT     y;
    FLOAT     z;
    FLOAT     rhw;
    D3DCOLOR  color;
} MYVERTEX;


//HWND CreateDirect3DWindow(HINSTANCE hInstance,WNDPROC WndProc,char* title,int bits,RES_TYPE ResType,int nCmdShow)
//eraseme
void __stdcall API_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//nothing
}

HWND __stdcall API_CreateWindow(	
					HINSTANCE hInstance,
					WNDPROC WndProc,
					HICON hicon,
					char* title,
					int bits,
					long& Width,
					long& Height,
					bool windowedflag)
{
	RECT WindowRect;
	char szTitleD3D[128];

	MakeError();

	assert(strlen(title)<128);//prevent memory stomping
	sprintf(szTitleD3D,"%s",title);
	sprintf(gszWindowClass,"%s",title);

	gp_Render_WndProc = WndProc;

	Direct3D_hInst 			= hInstance;
	Direct3D_windowedflag 	= windowedflag;
	Direct3D_WindowWidth  	= Width;;
	Direct3D_WindowHeight 	= Height;


	//windowed mode
	DWORD m_dwWindowStyle = WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|
	                          WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_VISIBLE;


	if(!Direct3D_windowedflag)
	{//fullscreen
		m_dwWindowStyle = WS_POPUP|WS_SYSMENU|WS_VISIBLE;
	}

	WNDCLASS wndClass = { 	0, 
							(WNDPROC)Direct3D_WndProc, 
							0, 
							0, 
							hInstance,
							hicon,
							//note:cant seem to load icon in dll on win98ME
							//LoadIcon( hInstance,"small.ico"),
							//(HICON)LoadImage(hInstance,"Render.ico",IMAGE_ICON,32,32,NULL),
							//LoadIcon( hInstance, (LPCTSTR)IDI_WINLOGO),
							LoadCursor( NULL, IDC_ARROW ),
							(HBRUSH)(COLOR_WINDOW),//GetStockObject(WHITE_BRUSH),
							NULL, 
							gszWindowClass};

	RegisterClass( &wndClass );
	DWORD Error = GetLastError();
	Direct3D_hWnd = CreateWindow(
					gszWindowClass, 
					szTitleD3D, 
					m_dwWindowStyle,
					0,
					0,
					Direct3D_WindowWidth,
					Direct3D_WindowHeight,
					NULL, 
					NULL, 
					hInstance, 
					NULL);
	Error = GetLastError();


    if(Direct3D_hWnd == NULL)
    {
        return false;
    }

	GetClientRect(Direct3D_hWnd,&WindowRect);
    Width  = (float)(WindowRect.right - WindowRect.left);
    Height = (float)(WindowRect.bottom - WindowRect.top);

	CreateRenderD3D(Width,Height);	
    UpdateWindow(Direct3D_hWnd);

	return Direct3D_hWnd;
}

void __stdcall API_ReSizeWindow(long Width,long Height)
{
	HRESULT hr = 0;
	D3DPRESENT_PARAMETERS d3dpp;
    HDC hdc = GetDC(Direct3D_hWnd);

	if(pd3dDevice == NULL)
	{//this gets called too early at startup.
		return;
	}

	FreeVertexBuffers();

	memset(&d3dpp,0,sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.Windowed         = Direct3D_windowedflag;
	d3dpp.hDeviceWindow    = Direct3D_hWnd;
	d3dpp.BackBufferWidth  = Width;//Direct3D_WindowWidth;
	d3dpp.BackBufferHeight = Height;//Direct3D_WindowHeight;
	d3dpp.BackBufferFormat = GetDeviceCaps(hdc, BITSPIXEL) == 16 ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount  = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
    ReleaseDC(Direct3D_hWnd, hdc);

	if(pd3dDevice && FAILED(hr = pd3dDevice->Reset(&d3dpp)))
	{
		GetError(hr);
		OutputDebugString(DebugText);
	    return;
	}
	RestoreVertexBuffers();
}

void __stdcall API_DeleteWindow()
{
    DestroyWindow(Direct3D_hWnd);
    SAFE_RELEASE( m_pwVB );
    SAFE_RELEASE( m_pIB );
	Direct3DTextureLL_RemoveALL(pDirect3DTextureHead);
    SAFE_RELEASE( pd3dDevice );
	if (!UnregisterClass(gszWindowClass,Direct3D_hInst))
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		Direct3D_hInst=NULL;
	}
}
void __stdcall API_BeginScene(long ClearColor)
{
	HRESULT hr = 0;

	/////////////////////////////////////////////////
	//// Test D3D To See If It Is Safe To Render ////
	/////////////////////////////////////////////////

    if(FAILED(hr = pd3dDevice->TestCooperativeLevel()))
    {
        if(hr == D3DERR_DEVICELOST)
		{
			//This happens during fullscreen, and alt+tab
			//  or if ctrl+alt+del is pressed
			//  or if display settings are changed from start->settings->display
            return;//D3DERR_NOERROR
		}
        if(hr == D3DERR_DEVICENOTRESET)
        {   
			FreeVertexBuffers();

			HDC hdc = GetDC(Direct3D_hWnd);
			if(hdc == NULL)
			{
			    PostQuitMessage(-1);
			    return;
			}

			// The Following Code Solves Problem When User Changes 
			//		Windows From 32bit To 16bit Causing Blank Screen  
			D3DPRESENT_PARAMETERS d3dpp;
			memset(&d3dpp,0,sizeof(D3DPRESENT_PARAMETERS));
    		d3dpp.Windowed         = Direct3D_windowedflag;
    		d3dpp.hDeviceWindow    = Direct3D_hWnd;
    		d3dpp.BackBufferWidth  = Direct3D_WindowWidth;
    		d3dpp.BackBufferHeight = Direct3D_WindowHeight;
			d3dpp.BackBufferFormat = GetDeviceCaps(hdc, BITSPIXEL) == 16 ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;
			d3dpp.BackBufferCount  = 1;
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

			if(FAILED(hr = pd3dDevice->Reset(&d3dpp)))
			{
	            return;
			}
			RestoreVertexBuffers();
        }
		else
		{
			assert(0 && "UpdateRenderD3D : UnknownError1");
		}
    }
    if(SUCCEEDED(pd3dDevice->BeginScene()))
    {
		////////////////////////
		//// Clear Surfaces ////
		////////////////////////
		//pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0f, 0);
		pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,ClearColor, 1.0f, 0);

	    hr = pd3dDevice->SetStreamSource( 0, m_pwVB, sizeof(VERTEX_XYZNRGBUV) );
	    if(FAILED(hr))return;
	    hr = pd3dDevice->SetIndices( m_pIB, 0 );
	    if(FAILED(hr))return;

		d3d_BeginScene = true;//indicates if begin scene succeded. If not, dont render
	}
}

void __stdcall API_RenderMesh(
					struct MeshLL* pMesh,
					float* pWorld,
					float* pView,
					float* pProj,
					float* pOrtho)
{
	D3DMATRIX IdentityMatrix;
	if(!d3d_BeginScene)
	{
		//assert(0 && "must call begin scene before rendering tris");
		return;
	}

/*
	//no scenes, so use system ortho matrix
	if(pScene==NULL)
	{
		if(pMesh->RenderFlags & RENDER_ORTHOGRAPHIC)
		{
			Identity((float*)&IdentityMatrix);
		    pd3dDevice->SetTransform( D3DTS_PROJECTION, (const struct _D3DMATRIX *)d3dOrthoMatrix);
		    pd3dDevice->SetTransform( D3DTS_VIEW,       (const struct _D3DMATRIX *)&IdentityMatrix);
		}
		else
		{
			assert(0);//3d with no scene????
		}
	}
	else
	{
		/////////////////////////////////////////////////////////////////////////////
		if(pMesh->RenderFlags & RENDER_ORTHOGRAPHIC)
		{
			Identity((float*)&IdentityMatrix);
		    pd3dDevice->SetTransform( D3DTS_PROJECTION, (const struct _D3DMATRIX *)pScene->pOrtho );
		    pd3dDevice->SetTransform( D3DTS_VIEW,       (const struct _D3DMATRIX *)&IdentityMatrix );
		}
		else//perspective projection
		{
		    pd3dDevice->SetTransform( D3DTS_PROJECTION, (const struct _D3DMATRIX *)pScene->pProj );
		    pd3dDevice->SetTransform( D3DTS_VIEW,       (const struct _D3DMATRIX *)pScene->pView );
		}
	}
*/
	if(pMesh->RenderFlags & RENDER_ORTHOGRAPHIC)
	{
		Identity((float*)&IdentityMatrix);
	    pd3dDevice->SetTransform( D3DTS_PROJECTION, (const struct _D3DMATRIX *)pOrtho );
	    pd3dDevice->SetTransform( D3DTS_VIEW,       (const struct _D3DMATRIX *)&IdentityMatrix );
	}
	else//perspective projection
	{
	    pd3dDevice->SetTransform( D3DTS_PROJECTION, (const struct _D3DMATRIX *)pProj );
	    pd3dDevice->SetTransform( D3DTS_VIEW,       (const struct _D3DMATRIX *)pView );
	}

	pd3dDevice->SetTransform( D3DTS_WORLD,(D3DXMATRIX*)pWorld);

	Render3dMeshD3D(pMesh);

	/////////////////////////////////////////////////////////////////////////////
}

void __stdcall API_EndScene()
{
	HRESULT hr = 0;
    pd3dDevice->EndScene();
	d3d_BeginScene = false;
    hr = pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void __stdcall Direct3D_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	gp_Render_WndProc(hwnd,uMsg,wParam,lParam);
}

bool CreateRenderD3D(long Width, long Height)
{
	HRESULT hr;

	IDirect3D8 *pIDirect3D8 = Direct3DCreate8(D3D_SDK_VERSION);//DX80120
    if (pIDirect3D8 == NULL)
    {
        return false;
    }

    HDC hdc = GetDC(Direct3D_hWnd);
    if(hdc == NULL)
    {
        return false;
    }

	D3DPRESENT_PARAMETERS d3dpp;
    memset(&d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));
    d3dpp.Windowed         = Direct3D_windowedflag;
    d3dpp.hDeviceWindow    = Direct3D_hWnd;
    d3dpp.BackBufferWidth  = Direct3D_WindowWidth;
    d3dpp.BackBufferHeight = Direct3D_WindowHeight;

    Direct3D_WindowWidth  = Width;//for restore
    Direct3D_WindowHeight = Height;

    d3dpp.BackBufferFormat = GetDeviceCaps(hdc, BITSPIXEL) == 16 ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount  = 1;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;


    hr = pIDirect3D8->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,Direct3D_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice);
    if(FAILED(hr))
    {
		GetError(hr);
		OutputDebugString(DebugText);
        return false;
    }

    pIDirect3D8->Release();//we dont need this any more

    ReleaseDC(Direct3D_hWnd, hdc);





    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

    pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
    pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);

    pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
//    pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0x00ffffff );
    pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0xffffffff);
    // Set the transform matrices


	///////////////////////////////////////////////

//    pd3dDevice->SetTexture( 0, pTexture );
    pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
    pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
    pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
    pd3dDevice->SetRenderState( D3DRS_ALPHAREF,         0x80 );
//    pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
//    pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_LESS );
    pd3dDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
    pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CW );
    pd3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE ); //!!!!!
    pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
    pd3dDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
    pd3dDevice->SetRenderState( D3DRS_EDGEANTIALIAS,    FALSE );
    pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
    pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND,      FALSE );
    pd3dDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
    pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
    pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );






//	ReSizeDirect3D();
	return true;
}


void DeleteRenderD3D()
{
}

bool VerifyRenderD3D()
{
    IDirect3D8 *pIDirect3D8 = Direct3DCreate8(D3D_SDK_VERSION);
    if (pIDirect3D8 == NULL)
    {
        return false;
    }
    pIDirect3D8->Release();
	return true;
}







//D3D_ERROR RenderMeshD3D(struct SceneLL* pScene,

D3D_ERROR Render3dMeshD3D(struct MeshLL* pMesh)
{
	WORD* pVB_TriList = NULL;
	VERTEX_XYZNRGBUV* pVB_Vertices = NULL;
	VERTEX_XYZNRGBUV* pRenderVerts = NULL;
	static float inc = 1.2345;
	HRESULT hr = 0;
    D3DMATERIAL8 mtrl;

	if(pMesh->num_tris<1)return D3DERR_NOERROR;

	//TESTING
    pd3dDevice->SetRenderState( D3DRS_ZFUNC,  D3DCMP_LESS );


	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	pd3dDevice->SetRenderState( D3DRS_CLIPPING, TRUE );//??
	/////////////////////////////////////////////////////////////////////////////
	if(!(pMesh->RenderFlags & RENDER_VISIBLE))
	{
		return D3DERR_NOERROR;
	}	
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	// ALPHA STATE //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_TRANSPARENT_TEXTURE)
	{
        pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
        pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	    pd3dDevice->SetRenderState(D3DRS_ALPHAREF,128 );
//       	pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
       	pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);
	}
	/////////////////////////////////////////////////////////////////////////////
	else if(pMesh->RenderFlags & RENDER_TRANSLUCENT_TEXTURE ||
			pMesh->RenderFlags & RENDER_TRANSLUCENT_VERTEX)
	{
        pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
        pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE); 
	}
	else
	{
        pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	}

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	// SOURCE BLEND MODE
	if(pMesh->SrcBlend==BLEND_ZERO        	)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
	if(pMesh->SrcBlend==BLEND_ONE		   	)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	if(pMesh->SrcBlend==BLEND_SRCCOLOR	   	)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
	if(pMesh->SrcBlend==BLEND_INVSRCCOLOR	)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCCOLOR);
	if(pMesh->SrcBlend==BLEND_SRCALPHA		)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	if(pMesh->SrcBlend==BLEND_INVSRCALPHA	)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCALPHA);
	if(pMesh->SrcBlend==BLEND_DESTALPHA		)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTALPHA);
	if(pMesh->SrcBlend==BLEND_INVDESTALPHA	)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVDESTALPHA);
	if(pMesh->SrcBlend==BLEND_DESTCOLOR   	)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
	if(pMesh->SrcBlend==BLEND_INVDESTCOLOR	)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVDESTCOLOR);
	if(pMesh->SrcBlend==BLEND_SRCALPHASAT 	)pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHASAT);

	// DESTINATION BLEND MODE
	if(pMesh->DstBlend==BLEND_ZERO        	)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	if(pMesh->DstBlend==BLEND_ONE		   	)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	if(pMesh->DstBlend==BLEND_SRCCOLOR	   	)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCCOLOR);
	if(pMesh->DstBlend==BLEND_INVSRCCOLOR	)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
	if(pMesh->DstBlend==BLEND_SRCALPHA		)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCALPHA);
	if(pMesh->DstBlend==BLEND_INVSRCALPHA	)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	if(pMesh->DstBlend==BLEND_DESTALPHA		)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTALPHA);
	if(pMesh->DstBlend==BLEND_INVDESTALPHA	)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVDESTALPHA);
	if(pMesh->DstBlend==BLEND_DESTCOLOR   	)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTCOLOR);
	if(pMesh->DstBlend==BLEND_INVDESTCOLOR	)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVDESTCOLOR);
	if(pMesh->DstBlend==BLEND_SRCALPHASAT 	)pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCALPHASAT);

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_WIREFRAME)
	{
        //pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,FALSE);
		pd3dDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	}
	else
	{
        pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	}

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_ZBUFFERREAD)
	{
		pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	}
	else
	{
		pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	}
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_ZBUFFERWRITE)
	{
		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	}
	else
	{
		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	}
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_BILINEARFILTER)
	{
    	pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	    pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	}
	else
	{
    	pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	    pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	}
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_TRANSPARENT_TEXTURE)
	{//colorvertex disables bitmap zbuffer, so turn this off
	    pd3dDevice->SetRenderState( D3DRS_COLORVERTEX ,FALSE);
		pd3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);
	}
	else
	{//always colorize vertex if not transparent
	    pd3dDevice->SetRenderState( D3DRS_COLORVERTEX ,TRUE);
		pd3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_COLOR1);
	}
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_LIGHTING)
	{
	    pd3dDevice->SetRenderState( D3DRS_LIGHTING ,TRUE);
	}
	else
	{
	    pd3dDevice->SetRenderState( D3DRS_LIGHTING ,FALSE);
	}
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	//KEYWORD:RENDER_TRANSFORMED_VERTEXSELECTION
	if(	pMesh->RenderFlags & RENDER_TRANSFORMED ||
		pMesh->RenderFlags & RENDER_ORTHOGRAPHIC)
	{//meshes vertices are already transformed. use World Vertices, and Identity Matrix
		pRenderVerts = pMesh->pWorldVerts;
	}
	else
	{//meshes vertices not transformed. use Local Vertices, and LocalToWorld Matrix
		pRenderVerts = pMesh->pLocalVerts;
	}

	/////////////////////////////////////////////////////////////////////////////
	D3DUtil_InitMaterial( mtrl, 1.0,0.0,0.0,1.0, 1.0,1.0,1.0,1.0);
	pd3dDevice->SetMaterial( &mtrl );
	/////////////////////////////////////////////////////////////////////////////
	if(!pMesh->Texture.InVRAM)
	{
		LoadTextureIntoVRAMD3D(pMesh->Texture);
	}
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_TEXTURED)
	{
	    pd3dDevice->SetTexture( 0, (struct IDirect3DBaseTexture8 *)pMesh->Texture.pAPITEXTURE);
	}
	else
	{
    	pd3dDevice->SetTexture(0,NULL);
	}
	/////////////////////////////////////////////////////////////////////////////

    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );

    pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
//    pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_BLENDDIFFUSEALPHA);
    pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

    pd3dDevice->SetRenderState( D3DRS_CULLMODE,D3DCULL_CW );

//	pd3dDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_COLOR1);
//	pd3dDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE,D3DMCS_MATERIAL);



   	pd3dDevice->SetVertexShader( D3DFVF_FATVERTEX );

	/////////////////////////////////////
	//// Fill Vertex Buffer Vertices ////
	/////////////////////////////////////

	if(SUCCEEDED(hr = m_pwVB->Lock(0,pMesh->num_verts*sizeof(VERTEX_XYZNRGBUV),(BYTE**)&pVB_Vertices,0)))
	{
		memcpy(pVB_Vertices,pRenderVerts,pMesh->num_verts*sizeof(VERTEX_XYZNRGBUV));
  		hr = m_pwVB->Unlock();
	}
	else return D3DERR_NOERROR;//fail
   	
  	hr = pd3dDevice->SetStreamSource( 0, m_pwVB, sizeof(VERTEX_XYZNRGBUV) );
    if(FAILED(hr))return D3DERR_NOERROR;

	////////////////////////////////////
	//// Fill Vertex Buffer Indices ////
	////////////////////////////////////

	if(SUCCEEDED(hr = m_pIB->Lock(0,3*pMesh->num_tris*sizeof(WORD),(BYTE**)&pVB_TriList,0)))
	{
		memcpy(pVB_TriList,pMesh->pIndices,3*pMesh->num_tris*sizeof(WORD));
		hr = m_pIB->Unlock();
	}
	else return D3DERR_NOERROR;//fail

	//////////////////////////
	//// Render Trianlges ////
	//////////////////////////

	hr = pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,pMesh->num_verts,0,pMesh->num_tris);
	if(FAILED(hr))
	{
		//sprintf(DebugText,"%s",DXGetErrorString8(hr));
		return D3DERR_NOERROR;//dont cry
	}
	return D3DERR_NOERROR;
}




void CheckCaps()
{
//  Is DX on this system?
//	windowed mode? voodoo2!
//  resolutions supported?
}
void FreeVertexBuffers()
{
    SAFE_RELEASE( m_pwVB );
//    SAFE_RELEASE( m_psVB );
    SAFE_RELEASE( m_pIB );
}


D3DLIGHT8 light;
D3DLIGHT8 light2;
bool RestoreVertexBuffers()
{
	HRESULT hr;




	//// Lights ////
	//// Lights ////
    memset(&light,0,sizeof(D3DLIGHT8));
    light.Type        = D3DLIGHT_POINT;
//    light.Type        = D3DLIGHT_DIRECTIONAL;

    light.Ambient.r   =  0.3;//ambient controlled here, and D3DRS_AMBIENT
    light.Ambient.g   =  0.3;
    light.Ambient.b   =  0.3;

    light.Diffuse.r   =  1.0;
    light.Diffuse.g   =  1.0;
    light.Diffuse.b   =  1.0;
	light.Direction.x =  0.0;
	light.Direction.y =  -1.0;
	light.Direction.z = 0.0;
    light.Position.x  = -20.0;
    light.Position.y  = -20.0;
    light.Position.z  =  50.0;
    light.Range       = 1000.0f;
    light.Attenuation0 = 1.0f;
    pd3dDevice->SetLight( 0, &light );
    pd3dDevice->LightEnable( 0, TRUE );
    pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	//// Lights ////
	//// Lights ////
/*
    memset(&light2,0,sizeof(D3DLIGHT8));
    light2.Type        = D3DLIGHT_POINT;
//    light.Type        = D3DLIGHT_DIRECTIONAL;
    light2.Diffuse.r   =  1.0;
    light2.Diffuse.g   =  0.0;
    light2.Diffuse.b   =  0.0;
	light2.Direction.x =  0.0;
	light2.Direction.y =  -1.0;
	light2.Direction.z =  0.0;
    light2.Position.x  = -5.0;
    light2.Position.y  = -10.0;
    light2.Position.z  =  5.0;
    light2.Range       = 1000.0f;
    light2.Attenuation0 = 1.0f;
    pd3dDevice->SetLight( 0, &light2 );
*/





//    pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
//    pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0xffffffff );
    pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0xff333333);
//    pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0xffc8c8c8);
//    pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0x0000ffff );
//    pd3dDevice->SetRenderState( D3DRS_LIGHTING ,FALSE);
//    pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0x00ffffff );
	long size = MAX_VERTICES*sizeof(VERTEX_XYZNRGBUV);
    if(FAILED(hr = pd3dDevice->CreateVertexBuffer( MAX_VERTICES*sizeof(VERTEX_XYZNRGBUV),
                                                       D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
                                                       D3DPOOL_DEFAULT, &m_pwVB ) ) )
    {
    	return false;
    }
/*
    if(FAILED(hr = pd3dDevice->CreateVertexBuffer( MAX_VERTICES*sizeof(VERTEX_XYZNRGBUV),
                                                       D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
                                                       D3DPOOL_DEFAULT, &m_psVB ) ) )
    {
    	return false;
    }
*/
	if(FAILED(hr = pd3dDevice->CreateIndexBuffer( MAX_INDICES *sizeof(WORD),
                                            D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
                                            D3DPOOL_DEFAULT, &m_pIB ) ) )
    {
    	return false;
    }
	return true;
}







VOID D3DUtil_InitMaterial( 	D3DMATERIAL8& mtrl, 
							FLOAT dr, 
							FLOAT dg, 
							FLOAT db,
                           	FLOAT da,
							FLOAT ar, 
							FLOAT ag, 
							FLOAT ab,
                           	FLOAT aa )
{
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
    mtrl.Diffuse.r = dr;
    mtrl.Diffuse.g = dg;
    mtrl.Diffuse.b = db;
    mtrl.Diffuse.a = da;

	mtrl.Ambient.r = ar;
	mtrl.Ambient.g = ag;
	mtrl.Ambient.b = ab;
	mtrl.Ambient.a = aa;
}


char *ErrorText[] = 
{
	"D3DERR_WRONGTEXTUREFORMAT",
	"D3DERR_UNSUPPORTEDCOLOROPERATION ",
	"D3DERR_UNSUPPORTEDCOLORARG		  ",
	"D3DERR_UNSUPPORTEDALPHAOPERATION ",
	"D3DERR_UNSUPPORTEDALPHAARG		  ",
	"D3DERR_TOOMANYOPERATIONS		  ",
	"D3DERR_CONFLICTINGTEXTUREFILTER  ",
	"D3DERR_UNSUPPORTEDFACTORVALUE	  ",
	"D3DERR_CONFLICTINGRENDERSTATE	  ",
	"D3DERR_UNSUPPORTEDTEXTUREFILTER  ",
	"D3DERR_CONFLICTINGTEXTUREPALETTE ",
	"D3DERR_DRIVERINTERNALERROR		  ",
	"D3DERR_NOTFOUND				  ",
	"D3DERR_MOREDATA				  ",
	"D3DERR_DEVICELOST				  ",
	"D3DERR_DEVICENOTRESET			  ",
	"D3DERR_NOTAVAILABLE			  ",
	"D3DERR_OUTOFVIDEOMEMORY		  ",
	"D3DERR_INVALIDDEVICE			  ",
	"D3DERR_INVALIDCALL",
	"D3DERR_DRIVERINVALIDCALL"
};




void MakeError()
{ 
	ErrorNum[0 ] = D3DERR_WRONGTEXTUREFORMAT;
	ErrorNum[1 ] = D3DERR_UNSUPPORTEDCOLOROPERATION;
	ErrorNum[2 ] = D3DERR_UNSUPPORTEDCOLORARG		;
	ErrorNum[3 ] = D3DERR_UNSUPPORTEDALPHAOPERATION	 ;
	ErrorNum[4 ] = D3DERR_UNSUPPORTEDALPHAARG		  ;
	ErrorNum[5 ] = D3DERR_TOOMANYOPERATIONS			   ;
	ErrorNum[6 ] = D3DERR_CONFLICTINGTEXTUREFILTER		;
	ErrorNum[7 ] = D3DERR_UNSUPPORTEDFACTORVALUE ;
	ErrorNum[8 ] = D3DERR_CONFLICTINGRENDERSTATE  ;
	ErrorNum[9 ] = D3DERR_UNSUPPORTEDTEXTUREFILTER ;
	ErrorNum[10] = D3DERR_CONFLICTINGTEXTUREPALETTE	;
	ErrorNum[11] = D3DERR_DRIVERINTERNALERROR		 ;
	ErrorNum[12] = D3DERR_NOTFOUND			 ;
	ErrorNum[13] = D3DERR_MOREDATA			  ;
	ErrorNum[14] = D3DERR_DEVICELOST		   ;
	ErrorNum[15] = D3DERR_DEVICENOTRESET		;
	ErrorNum[16] = D3DERR_NOTAVAILABLE			 ;
	ErrorNum[17] = D3DERR_OUTOFVIDEOMEMORY		  ;
	ErrorNum[18] = D3DERR_INVALIDDEVICE			   ;
	ErrorNum[19] = D3DERR_INVALIDCALL				;
	ErrorNum[20] = D3DERR_DRIVERINVALIDCALL			 ;
}
char* GetError(HRESULT Error)
{ 
	long i;
	if(Error==0)
	{
		sprintf(DebugText,";) TANZANITE : No Error\n");
	 	return DebugText;
	}
	for(i=0;i<NumberOfErrorCodes;i++)
	{
		if(Error == ErrorNum[i])
		{
			sprintf(DebugText,";) TANZANITE : %s\n",ErrorText[i]);
			return DebugText;
		}
	}
	return NULL;
}


bool LoadTextureIntoVRAMD3D(aTEXTURE& Texture)
{
	long Pitch;
	long dstIndex,srcIndex,x,y;
	//long Alp;
	HRESULT hr;
	struct Direct3DTextureLL* pNewDirect3DTexture = NULL;

	if(Texture.pChar==NULL)return false;

	pNewDirect3DTexture = Direct3DTextureLL_Insert(0,Texture.UniqueID,pDirect3DTextureHead);
//Direct3DTextureLL_Insert();

    hr = pd3dDevice->CreateTexture( Texture.Width, Texture.Height, 1,
                                      0, D3DFMT_A8R8G8B8,
                                      D3DPOOL_MANAGED, &pNewDirect3DTexture->pD3DTexture8 );
    if(FAILED(hr))return false;

    D3DLOCKED_RECT d3dlr;
    pNewDirect3DTexture->pD3DTexture8->LockRect( 0, &d3dlr, 0, 0 );
    WORD* pDst16 = (WORD*)d3dlr.pBits;
    unsigned char* pDst = (unsigned char*)d3dlr.pBits;
    unsigned char* pSrc = Texture.pChar;

	Pitch = d3dlr.Pitch;
    for(y=0;y<Texture.Height;y++)
    {
        for(x=0;x<Texture.Width;x++)
        {
			srcIndex = (y*Texture.Width*3) + (x*3);
			dstIndex = (y*Pitch) + (x*4);
			pDst[dstIndex+0] = pSrc[srcIndex+0];//Blue
			pDst[dstIndex+1] = pSrc[srcIndex+1];//Green
			pDst[dstIndex+2] = pSrc[srcIndex+2];//Red
			pDst[dstIndex+3] = (pSrc[srcIndex+0]+pSrc[srcIndex+1]+pSrc[srcIndex+2])/3;//pSrc[srcIndex+3];//alph
//			pDst[dstIndex+3] = (char)(RandZeroToOne*255);
/*
			pDst[dstIndex+3] = 255;
			/////////RGBA
			Alp = pSrc[srcIndex+0]+pSrc[srcIndex+1]+pSrc[srcIndex+2];
//			if(Alp>255)Alp=255;
//			pDst[dstIndex+3] = Alp;
			pDst[dstIndex+3] = 0;
			if(Alp>0)
				pDst[dstIndex+3] = 255;
			/////////
*/
		}
	}
    pNewDirect3DTexture->pD3DTexture8->UnlockRect(0);

	// now write to mesh's texture to notify the engine that its texture is now in VRAM
    Texture.InVRAM = true;
    Texture.pAPITEXTURE = (unsigned char*)pNewDirect3DTexture->pD3DTexture8;
	return true;
}



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

























VOID D3DUtil_SetRotateXMatrix( D3DMATRIX& mat, FLOAT fRads )
{
    mat._22 =  (FLOAT)cos( fRads );
    mat._23 =  (FLOAT)sin( fRads );
    mat._32 = -(FLOAT)sin( fRads );
    mat._33 =  (FLOAT)cos( fRads );
}
VOID D3DUtil_SetRotateYMatrix( D3DMATRIX& mat, FLOAT fRads )
{
    mat._11 =  (FLOAT)cos( fRads );
    mat._13 = -(FLOAT)sin( fRads );
    mat._31 =  (FLOAT)sin( fRads );
    mat._33 =  (FLOAT)cos( fRads );
}
VOID D3DUtil_SetRotateZMatrix( D3DMATRIX& mat, FLOAT fRads )
{
    mat._11  =  (FLOAT)cos( fRads );
    mat._12  =  (FLOAT)sin( fRads );
    mat._21  = -(FLOAT)sin( fRads );
    mat._22  =  (FLOAT)cos( fRads );
}


HRESULT D3DMath_MatrixInvert( D3DMATRIX& q, D3DMATRIX& a )
{
    if( fabs(a._44 - 1.0f) > .001f)
        return E_INVALIDARG;
    if( fabs(a._14) > .001f || fabs(a._24) > .001f || fabs(a._34) > .001f )
        return E_INVALIDARG;

    FLOAT fDetInv = 1.0f / ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
                             a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
                             a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

    q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
    q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
    q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
    q._14 = 0.0f;

    q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
    q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
    q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
    q._24 = 0.0f;

    q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
    q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
    q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
    q._34 = 0.0f;

    q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
    q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
    q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
    q._44 = 1.0f;

    return S_OK;
}
void Identity(float *pMat)
{
	memset(pMat,0,16*sizeof(float));
	pMat[0 ] = 1.0;
	pMat[5 ] = 1.0;
	pMat[10] = 1.0;
	pMat[15] = 1.0;
}



















