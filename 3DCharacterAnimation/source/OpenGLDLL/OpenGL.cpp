// OpenGL.cpp written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to




////////////////////
//// OpenGL.cpp ////
////////////////////

#include "stdafx.h"
#include "..\Library.h"//jump up a dir to get access to globals.h
#include "OpenGLTextureLL.h"
#include <windowsx.h>
#include <math.h>//sqrt
#include <stdio.h>//sprintf
#include <stdlib.h>//calloc,free
#include <assert.h>


//#define PRECISION_MATTERS
//////////////////
//// #defines ////
//////////////////
extern FILE* DiskFile;
////////////////////////
//// Typedef Struct ////
////////////////////////
typedef struct _OGLVERTEX {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
	GLfloat tu;
	GLfloat tv;
} OGLVERTEX;
//////////////////////////
//// Global Variables ////
//////////////////////////
bool apiactive = true;
char gszWindowClass[128];

#ifdef PRECISION_MATTERS
	VERTEX_XYZNRGBUV* pNewVerts = NULL;
#endif//PRECISION_MATTERS

float openglOrthoMatrix[16];//local copy of ortho
long pARGBtoABGR[MAX_VERTICES];

WNDPROC gp_Render_WndProc;

float 		OpenGL_WindowWidth;
float 		OpenGL_WindowHeight;
bool  		OpenGL_windowedflag = false;
HINSTANCE 	OpenGL_hInst = NULL;
HWND	  	OpenGL_hWnd;
HDC		   	OpenGL_hDC=NULL;
HGLRC		OpenGL_hRC=NULL;
//bool			active=TRUE;

//HWND	  hWndOpenGL;

struct OpenGLTextureLL* pOpenGLTextureHead = NULL;

//GLuint pIndices[100];
float CameraDistanceGL = 10.0;
GLfloat xspeed = 0.2;
GLfloat yspeed = 0.2;
//GLuint	texture[3];

//bool			keys[256];
DEVMODE	dmSaved;
DEVMODE dmSelected;
GLfloat	xrot;				// X Rotation
GLfloat	yrot;				// Y Rotation
//							  RED	GRN	  BLU



//The ambient term is simply the ambient color of the light scaled by the ambient material property
GLfloat LightAmbient[]=		{ 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{ -20.0f, -20.0f, 50.0f, 1.0f };


/////////////////////////////
//// Function Prototypes ////
/////////////////////////////
//void CreateOrthoMatrixOpenGL(float* pMat);
void __stdcall OpenGL_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void ExtractFrustum();
void Identity(float *pMat);
void ConvertViewMatrixToOpenGL(float* pMat);
void ConvertProjMatrixToOpenGL(float* pMat);
void ConvertOrthoMatrixToOpenGL(float* pMat);


void DeleteOpenGLWindow();
void CreateProjectionMatrixOGL(GLfloat *pmat,float fov,float nearplane);
long LoadImage(char *pFileName);
bool CreateOpenGL(long Width,long Height);
void DeleteOpenGL();
void ReSizeOpenGL();
bool InitOpenGL();
void Render3dMeshOpenGL(struct MeshLL* pMesh);
bool LoadTextureIntoVRAMOpenGL(aTEXTURE* pTexture);

///////////////////
//// Functions ////
///////////////////
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
	char szTitleOpenGL[128];

	assert(strlen(title)<128);//prevent memory stomping
	sprintf(szTitleOpenGL,"%s",title);
	sprintf(gszWindowClass,"%s",title);

	gp_Render_WndProc = WndProc;

	OpenGL_hInst 		 = hInstance;
	OpenGL_windowedflag  = windowedflag;
	OpenGL_WindowWidth   = Width;
	OpenGL_WindowHeight  = Height;

	//windowed mode
	DWORD m_dwWindowStyle = WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|
	                          WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_VISIBLE;


	if(!OpenGL_windowedflag)
	{//fullscreen
		m_dwWindowStyle = WS_POPUP|WS_SYSMENU|WS_VISIBLE;
	}
	
	WNDCLASS wndClass = { 	0, 
							(WNDPROC)OpenGL_WndProc, 
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
	OpenGL_hWnd = CreateWindow(
					gszWindowClass, 
					szTitleOpenGL, 
					m_dwWindowStyle,
					0,
					0,
					OpenGL_WindowWidth,
					OpenGL_WindowHeight,
					NULL, 
					NULL, 
					hInstance, 
					NULL);
	Error = GetLastError();


    if(OpenGL_hWnd == NULL)
    {
        return false;
    }

	GetClientRect(OpenGL_hWnd,&WindowRect);
    Width  = (float)(WindowRect.right - WindowRect.left);
    Height = (float)(WindowRect.bottom - WindowRect.top);

	CreateOpenGL(Width,Height);
    UpdateWindow(OpenGL_hWnd);

	return OpenGL_hWnd;
}


void __stdcall API_ReSizeWindow(long Width,long Height)
{
	glViewport(0,0,Width,Height);
//	CreateOrthoMatrixOpenGL(openglOrthoMatrix);
}

void __stdcall API_DeleteWindow()
{
	DeleteOpenGL();
	OpenGLTextureLL_RemoveALL(pOpenGLTextureHead);

	if (OpenGL_hWnd && !DestroyWindow(OpenGL_hWnd))
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		OpenGL_hWnd=NULL;
	}

	if (!UnregisterClass(gszWindowClass,OpenGL_hInst))
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		OpenGL_hInst=NULL;
	}
}

void __stdcall API_BeginScene(long ClearColor)
{
	//Must reset mask values, or zbuffer wont clear
	glColorMask(1,1,1,1);			
	glDepthMask(1);

	//convert to OpenGL Specific
	float Alp = (float)GetAlp8888(ClearColor)/255.0;
	float Red = (float)GetRed8888(ClearColor)/255.0;
	float Grn = (float)GetGrn8888(ClearColor)/255.0;
	float Blu = (float)GetBlu8888(ClearColor)/255.0;

	glClearColor(Red,Grn,Blu,Alp);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}


void __stdcall API_RenderMesh(
					struct MeshLL* pMesh,
					float* pWorld,
					float* pView,
					float* pProj,
					float* pOrtho)
{
	float pLocalProj[16];
	float pLocalView[16];
	float pLocalOrtho[16];

	if(pMesh->RenderFlags & RENDER_ORTHOGRAPHIC)
	{
		glMatrixMode(GL_PROJECTION);
		memcpy(pLocalOrtho,pOrtho,16*sizeof(float));
		ConvertOrthoMatrixToOpenGL(pLocalOrtho);
		glLoadMatrixf(pLocalOrtho);
		//glScalef(1.0,1.0,-1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else//perspective projection
	{
		glMatrixMode(GL_PROJECTION);
		memcpy(pLocalProj,pProj,16*sizeof(float));
		ConvertProjMatrixToOpenGL(pLocalProj);
		glLoadMatrixf(pLocalProj);

		glMatrixMode(GL_MODELVIEW);
		memcpy(pLocalView,pView,16*sizeof(float));
		ConvertViewMatrixToOpenGL(pLocalView);
		glLoadMatrixf(pLocalView);
	}



	glPushMatrix();
	glMultMatrixf(pWorld);
	Render3dMeshOpenGL(pMesh);
	glPopMatrix();
}

void __stdcall API_EndScene()
{
	if(apiactive)
	{
		SwapBuffers(OpenGL_hDC);
	}
}
//This function allows us to alt tab to, and away from fullscreen
//void Handle_WM_ACTIVATE(WPARAM wParam, LPARAM lParam)
void __stdcall OpenGL_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
		case WM_ACTIVATE:
			if(OpenGL_windowedflag==false)
			{//FULLSCREEN CODE
				WORD fActive = LOWORD(wParam);           // activation flag 
				if(fActive==WA_ACTIVE && apiactive==false)
				{
					OutputDebugString("OpenGL - WA_ACTIVE\n");
//					CreateOpenGL(OpenGL_WindowWidth,OpenGL_WindowHeight);
				    UpdateWindow(OpenGL_hWnd);
				}
				else if(fActive==WA_INACTIVE && apiactive==true)
				{
					OutputDebugString("OpenGL - WA_INACTIVE\n");
//					DeleteOpenGL();
					OpenGLTextureLL_RemoveALL(pOpenGLTextureHead);
				}
			}
/*
			if (!HIWORD(wParam))
			{
				active=TRUE;
			}
			else
			{
				active=FALSE;
			}
*/
		break;
	}
	gp_Render_WndProc(hwnd,uMsg,wParam,lParam);
}


bool CreateOpenGL(long Width,long Height)
{
	float Scale = 1.0;

#ifdef PRECISION_MATTERS
	SAFE_FREE(pNewVerts);
	pNewVerts = (VERTEX_XYZNRGBUV *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
#endif//PRECISION_MATTERS

	assert(sizeof(float)==sizeof(GLfloat));//Render assums this with matricies

	if(OpenGL_windowedflag==false)
	{//FULLSCREEN CODE
		DEVMODE dmSelected;
		memset(&dmSelected,0,sizeof(dmSelected));
		dmSelected.dmSize=sizeof(dmSelected);
		dmSelected.dmPelsWidth	= Width;
		dmSelected.dmPelsHeight	= Height;
		dmSelected.dmBitsPerPel	= 32;/////eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
		dmSelected.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		if(ChangeDisplaySettings(&dmSelected,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
			return false;
		}
	}

	GLuint		PixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,//bits,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	
	if (!(OpenGL_hDC=GetDC(OpenGL_hWnd)))
	{
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (!(PixelFormat=ChoosePixelFormat(OpenGL_hDC,&pfd)))
	{
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(!SetPixelFormat(OpenGL_hDC,PixelFormat,&pfd))
	{
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (!(OpenGL_hRC=wglCreateContext(OpenGL_hDC)))
	{
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(!wglMakeCurrent(OpenGL_hDC,OpenGL_hRC))
	{
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

//	ReSizeOpenGL();


	// LOAD IMAGES AFTER THIS POINT
//	LoadImage("image1");
//	LoadImage("vector");




	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
//	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
//	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

//	float pColor[4] = {0.1,0.1,0.1,0.1};
	float pColor[4] = {1.0,1.0,1.0,1.0};
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,pColor);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One

	glColor4f(1.0f, 1.0f, 1.0f, 1.0);					// Full Brightness.  50% Alpha
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency

	apiactive = true;
	return true;
}
void DeleteOpenGL()
{
#ifdef PRECISION_MATTERS
	SAFE_FREE(pNewVerts);
#endif//PRECISION_MATTERS

	if(OpenGL_windowedflag==false)
	{//FULLSCREEN CODE
		ChangeDisplaySettings(&dmSaved,CDS_RESET);
		ShowCursor(TRUE);
	}

	if (OpenGL_hRC)
	{
		if (!wglMakeCurrent(NULL,NULL))
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(OpenGL_hRC))
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		OpenGL_hRC=NULL;
	}

	if (OpenGL_hDC && !ReleaseDC(OpenGL_hWnd,OpenGL_hDC))
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		OpenGL_hDC=NULL;
	}
	apiactive = false;
}

//void RenderMeshOpenGL(struct SceneLL* pScene,struct MeshLL* pMesh)

void Render3dMeshOpenGL(struct MeshLL* pMesh)
{
	long i,iRed,iGrn,iBlu,iAlp;
	long Offset = sizeof(VERTEX_XYZNRGBUV);//distance to next vertex
	VERTEX_XYZNRGBUV* pRenderVerts = NULL;
	GLenum SrcBlend,DstBlend;
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->num_verts==0 || pMesh->num_tris==0)
	{
		//Attempting to render using 0 verts or tris
		//has proven to disturb the next valid render, so leave now.
		return;
	}
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_VISIBLE)
	{
	}
	else
	{
		return;
	}	
	/////////////////////////////////////////////////////////////////////////////
	if(1)//assume solid
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glDisable(GL_BLEND);
	}
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glDisable(GL_BLEND);
	}

	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_TRANSLUCENT_TEXTURE ||
	   pMesh->RenderFlags & RENDER_TRANSLUCENT_VERTEX)
	{
		glEnable(GL_BLEND);
	}
	/////////////////////////////////////////////////////////////////////////////
	// SOURCE BLEND MODE
	if(pMesh->SrcBlend==BLEND_ZERO        	)SrcBlend = GL_ZERO;
	if(pMesh->SrcBlend==BLEND_ONE		   	)SrcBlend = GL_ONE;
	if(pMesh->SrcBlend==BLEND_SRCCOLOR	   	)SrcBlend = GL_SRC_COLOR;
	if(pMesh->SrcBlend==BLEND_INVSRCCOLOR	)SrcBlend = GL_ONE_MINUS_SRC_COLOR;
	if(pMesh->SrcBlend==BLEND_SRCALPHA		)SrcBlend = GL_SRC_ALPHA;
	if(pMesh->SrcBlend==BLEND_INVSRCALPHA	)SrcBlend = GL_ONE_MINUS_SRC_ALPHA;
	if(pMesh->SrcBlend==BLEND_DESTALPHA		)SrcBlend = GL_DST_ALPHA;
	if(pMesh->SrcBlend==BLEND_INVDESTALPHA	)SrcBlend = GL_ONE_MINUS_DST_ALPHA;
	if(pMesh->SrcBlend==BLEND_DESTCOLOR   	)SrcBlend = GL_DST_COLOR;
	if(pMesh->SrcBlend==BLEND_INVDESTCOLOR	)SrcBlend = GL_ONE_MINUS_DST_COLOR;
	if(pMesh->SrcBlend==BLEND_SRCALPHASAT 	)SrcBlend = GL_SRC_ALPHA_SATURATE;

	// DESTINATION BLEND MODE
	if(pMesh->DstBlend==BLEND_ZERO        	)DstBlend = GL_ZERO;
	if(pMesh->DstBlend==BLEND_ONE		   	)DstBlend = GL_ONE;
	if(pMesh->DstBlend==BLEND_SRCCOLOR	   	)DstBlend = GL_SRC_COLOR;
	if(pMesh->DstBlend==BLEND_INVSRCCOLOR	)DstBlend = GL_ONE_MINUS_SRC_COLOR;
	if(pMesh->DstBlend==BLEND_SRCALPHA		)DstBlend = GL_SRC_ALPHA;
	if(pMesh->DstBlend==BLEND_INVSRCALPHA	)DstBlend = GL_ONE_MINUS_SRC_ALPHA;
	if(pMesh->DstBlend==BLEND_DESTALPHA		)DstBlend = GL_DST_ALPHA;
	if(pMesh->DstBlend==BLEND_INVDESTALPHA	)DstBlend = GL_ONE_MINUS_DST_ALPHA;
	if(pMesh->DstBlend==BLEND_DESTCOLOR   	)DstBlend = GL_DST_COLOR;
	if(pMesh->DstBlend==BLEND_INVDESTCOLOR	)DstBlend = GL_ONE_MINUS_DST_COLOR;
	if(pMesh->DstBlend==BLEND_SRCALPHASAT 	)DstBlend = GL_SRC_ALPHA_SATURATE;

	glBlendFunc(SrcBlend,DstBlend);

	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_ZBUFFERREAD)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	/////////////////////////////////////////////////////////////////////////////
	if(pMesh->RenderFlags & RENDER_ZBUFFERWRITE)
	{
		glDepthMask(TRUE);
	}
	else
	{
		glDepthMask(FALSE);
	}



	/////////////////////////////////////////////////////////////////////////////

	if(pMesh->RenderFlags & RENDER_LIGHTING)
	{
		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
		glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
		glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
		glEnable(GL_LIGHT1);								// Enable Light One
		glEnable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0);					// Full Brightness.  50% Alpha
	}
	else
	{
		glDisable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0);					// Full Brightness.  50% Alpha
	}





	/////////////////////////////////////////////////////////////////////////////

	if(apiactive)
	{
		if(!pMesh->Texture.InVRAM)
		{
			LoadTextureIntoVRAMOpenGL(&pMesh->Texture);
		}

		glBindTexture(GL_TEXTURE_2D,pMesh->Texture.OpenGLTextureID);

		/////////////////////////////////////////////////////////////////////////////
		//must be done after glBindTexture
		if(pMesh->RenderFlags & RENDER_BILINEARFILTER)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		}
		/////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////
		if(	pMesh->RenderFlags & RENDER_TRANSFORMED ||
			pMesh->RenderFlags & RENDER_ORTHOGRAPHIC)
		{//meshes vertices are already transformed. use World Vertices, and Identity Matrix
			//float pIdentityMatrix[16];
			//Identity(pIdentityMatrix);

			pRenderVerts = pMesh->pWorldVerts;
		    //glPushMatrix();
		    //glMultMatrixf(pIdentityMatrix);
		}
		else
		{//meshes vertices not transformed. use Local Vertices, and LocalToWorld Matrix
			pRenderVerts = pMesh->pLocalVerts;
		    //glPushMatrix();
		    //glMultMatrixf(pMesh->pMatrix);
		}

		if(pMesh->RenderFlags & RENDER_TEXTURED)
		{
			glEnable(GL_TEXTURE_2D);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}


		//clear all clients states. they may be set up for a different vertex format.
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_EDGE_FLAG_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		//DATACONVERSION FROM Render.cpp to OpenGL
		//ACK must convert Render.cpp's ARGB format into OpenGL's ABGR format.
		for(i=0;i<pMesh->num_verts;i++)
		{
			iAlp =  pRenderVerts[i].diffuse>>24;
			iBlu = (pRenderVerts[i].diffuse>>16)&0xff;
			iGrn = (pRenderVerts[i].diffuse>>8 )&0xff;
			iRed = (pRenderVerts[i].diffuse     )&0xff;
			pARGBtoABGR[i] = PACKRGBA(iRed,iGrn,iBlu,iAlp);
		}


		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		
		glVertexPointer(3,GL_FLOAT,Offset,&(pRenderVerts[0].x  ));


#ifdef PRECISION_MATTERS
		long max = 0;
		for(i=0;i<(pMesh->num_tris*3);i++)
		{
			if(pMesh->pIndices[i]>max)
			{
				max = pMesh->pIndices[i];
			}
		}
		memcpy(pNewVerts,pRenderVerts,(max+1)*sizeof(VERTEX_XYZNRGBUV));
		
		for(i=0;i<(max+1);i++)
		{
			pNewVerts[i].y += 0.5;
		}
		glVertexPointer(3,GL_FLOAT,Offset,&(pNewVerts[0].x  ));
#endif//PRECISION_MATTERS



		glNormalPointer(GL_FLOAT,Offset,&(pRenderVerts[0].nx));
 		glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(long),pARGBtoABGR);
		glTexCoordPointer(2,GL_FLOAT,Offset,&(pRenderVerts[0].tu ));

		
		//DEBUG
		//DEBUG
		for(i=0;i<(pMesh->num_tris*3);i++)
		{
			if(pMesh->pIndices[i]<0 || pMesh->pIndices[i]>=MAX_VERTICES)
			{
				printf("Mesh Indices[%d] = %d",i,pMesh->pIndices[i]);
				assert(0 && "more than max");
			}
		}
		//DEBUG
		//DEBUG

    	glDrawElements(	GL_TRIANGLES, 
    					pMesh->num_tris*3,
    					GL_UNSIGNED_SHORT,
    					pMesh->pIndices);
	
		//Restore model matrix
	    //glPopMatrix();
	}
}



//what if it is already in there? dynamic textures...
bool LoadTextureIntoVRAMOpenGL(aTEXTURE* pTexture)
{
	struct OpenGLTextureLL* pNewOpenGLTexture = NULL;

	if(pTexture->pChar==NULL)return false;

	char pText[256];
	sprintf(pText,"uploading %s\n",pTexture->pName);
	OutputDebugString(pText);

	pNewOpenGLTexture = OpenGLTextureLL_Insert(0,pTexture->UniqueID,pOpenGLTextureHead);

	// now write to mesh's texture
	glGenTextures(1, &pTexture->OpenGLTextureID);					// Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_2D, pTexture->OpenGLTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,pTexture->Width,pTexture->Height, 0,GL_BGR_EXT,GL_UNSIGNED_BYTE,pTexture->pChar);

	//now in VRAM
    pTexture->InVRAM = true;
	return true;
}

bool VerifyRenderOpenGL()
{
/*
ToDo::   	LOAD OPENGL32.dll
    HINSTANCE hOPENGL32DLL = LoadLibrary( "opengl32.dll" );
    if(hOPENGL32DLL == NULL)
    {
        return false;
    }
    FreeLibrary(hOPENGL32DLL);
*/
	return true;
}









void ConvertProjMatrixToOpenGL(float* pMat)
{
 	pMat[0]  = -pMat[0];
 	pMat[10] = -pMat[10];
 	pMat[11] = -pMat[11];//see d3d8help
//If your application needs to use a negative value in _34 (11) this location, it should scale the entire projection matrix by -1, instead.
}

void ConvertOrthoMatrixToOpenGL(float* pMat)
{
	//this eliminates texture pimples that appear on opengl text
	//this might be a driver bug
	//at 800x600, pMat[0] = 0.0025, and pimples appear on text
	//	setting pMat[0] to = 0.0020, and then 
	//	setting pMat[0] to = 0.0025 cures the problem.
	//	that sounds like a driver bug to me. :)
 	pMat[0] += 0.000001;

}

void ConvertViewMatrixToOpenGL(float* pMat)
{
	aVECTOR Pos,NewPos;

	//// Get Actual Camera World Position ////

	Pos.x = -pMat[12];
	Pos.y = -pMat[13];
	Pos.z = -pMat[14];

	MacroDot(NewPos.x,pMat[0],pMat[1],pMat[2 ],Pos.x,Pos.y,Pos.z);
	MacroDot(NewPos.y,pMat[4],pMat[5],pMat[6 ],Pos.x,Pos.y,Pos.z);
	MacroDot(NewPos.z,pMat[8],pMat[9],pMat[10],Pos.x,Pos.y,Pos.z);

	//// Convert From Lefthand to GL's Righthand coord system ////

 	pMat[0]  = -pMat[0];
 	pMat[2]  = -pMat[2];

	pMat[4]  = -pMat[4]; 
	pMat[6]  = -pMat[6]; 

	pMat[8]  = -pMat[8]; 
	pMat[10] = -pMat[10];

	MacroDot(pMat[12],pMat[0],pMat[4],pMat[8 ],NewPos.x,NewPos.y,NewPos.z);
	MacroDot(pMat[13],pMat[1],pMat[5],pMat[9 ],NewPos.x,NewPos.y,NewPos.z);
	MacroDot(pMat[14],pMat[2],pMat[6],pMat[10],NewPos.x,NewPos.y,NewPos.z);

	pMat[12] = -pMat[12];
	pMat[13] = -pMat[13];
	pMat[14] = -pMat[14];
	pMat[15] = 1.0;

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

typedef struct _PLANE {
	float x;
	float y;
	float z;
	float d;
}PLANE;


// OpenGL.cpp
void ExtractFrustum()
{
	PLANE Frustum[6];
	float proj[16];
	float modl[16];
	float clip[16];
	float t;

   /* Get the current PROJECTION matrix from OpenGL */
   glGetFloatv( GL_PROJECTION_MATRIX, proj );

   /* Get the current MODELVIEW matrix from OpenGL */
   glGetFloatv( GL_MODELVIEW_MATRIX, modl );

   /* Combine the two matrices (multiply projection by modelview) */
   clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
   clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
   clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
   clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

   clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
   clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
   clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
   clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

   clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
   clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
   clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
   clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

   clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
   clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
   clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
   clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

   /* Extract the numbers for the RIGHT plane */
   Frustum[0].x = clip[ 3] - clip[ 0];
   Frustum[0].y = clip[ 7] - clip[ 4];
   Frustum[0].z = clip[11] - clip[ 8];
   Frustum[0].d = clip[15] - clip[12];

   /* Normalize the result */
/*
	float Length;
	MacroNormalize(Length,Frustum[0].x,Frustum[0].y,Frustum[0].z);

	
	
	
   Frustum[0].d = ((clip[12] * Frustum[0].x) + 
   				   (clip[13] * Frustum[0].y) + 
   				   (clip[14] * Frustum[0].z));
*/


   t = 1.0/(sqrt((Frustum[0].x * Frustum[0].x) + 
   				 (Frustum[0].y * Frustum[0].y) + 
   				 (Frustum[0].z * Frustum[0].z)));
   Frustum[0].x *= t;
   Frustum[0].y *= t;
   Frustum[0].z *= t;
   Frustum[0].d *= t;


   /* Extract the numbers for the LEFT plane */
   Frustum[1].x = clip[ 3] + clip[ 0];
   Frustum[1].y = clip[ 7] + clip[ 4];
   Frustum[1].z = clip[11] + clip[ 8];
   Frustum[1].d = clip[15] + clip[12];

   /* Normalize the result */
   t = 1.0/(sqrt((Frustum[1].x * Frustum[1].x) + 
   				 (Frustum[1].y * Frustum[1].y) + 
   				 (Frustum[1].z * Frustum[1].z)));
   Frustum[1].x *= t;
   Frustum[1].y *= t;
   Frustum[1].z *= t;
   Frustum[1].d *= t;

   /* Extract the BOTTOM plane */
   Frustum[2].x = clip[ 3] + clip[ 1];
   Frustum[2].y = clip[ 7] + clip[ 5];
   Frustum[2].z = clip[11] + clip[ 9];
   Frustum[2].d = clip[15] + clip[13];

   /* Normalize the result */
   t = 1.0/(sqrt((Frustum[2].x * Frustum[2].x) + 
   				 (Frustum[2].y * Frustum[2].y) + 
   				 (Frustum[2].z * Frustum[2].z)));
   Frustum[2].x *= t;
   Frustum[2].y *= t;
   Frustum[2].z *= t;
   Frustum[2].d *= t;

   /* Extract the TOP plane */
   Frustum[3].x = clip[ 3] - clip[ 1];
   Frustum[3].y = clip[ 7] - clip[ 5];
   Frustum[3].z = clip[11] - clip[ 9];
   Frustum[3].d = clip[15] - clip[13];

   /* Normalize the result */
   t = 1.0/(sqrt((Frustum[3].x * Frustum[3].x) + 
   				 (Frustum[3].y * Frustum[3].y) + 
   				 (Frustum[3].z * Frustum[3].z)));
   Frustum[3].x *= t;
   Frustum[3].y *= t;
   Frustum[3].z *= t;
   Frustum[3].d *= t;

   /* Extract the FAR plane */
   Frustum[4].x = clip[ 3] - clip[ 2];
   Frustum[4].y = clip[ 7] - clip[ 6];
   Frustum[4].z = clip[11] - clip[10];
   Frustum[4].d = clip[15] - clip[14];

   /* Normalize the result */
   t = 1.0/(sqrt((Frustum[4].x * Frustum[4].x) + 
   				 (Frustum[4].y * Frustum[4].y) + 
   				 (Frustum[4].z * Frustum[4].z)));
   Frustum[4].x *= t;
   Frustum[4].y *= t;
   Frustum[4].z *= t;
   Frustum[4].d *= t;

   /* Extract the NEAR plane */
   Frustum[5].x = clip[ 3] + clip[ 2];
   Frustum[5].y = clip[ 7] + clip[ 6];
   Frustum[5].z = clip[11] + clip[10];
   Frustum[5].d = clip[15] + clip[14];

   /* Normalize the result */
   t = 1.0/(sqrt((Frustum[5].x * Frustum[5].x) + 
   				 (Frustum[5].y * Frustum[5].y) + 
   				 (Frustum[5].z * Frustum[5].z)));
   Frustum[5].x *= t;
   Frustum[5].y *= t;
   Frustum[5].z *= t;
   Frustum[5].d /= clip[10];

//   float temp = (clip[15] + clip[14])/(clip[15] + clip[14]);

}

void Identity(float *pMat)
{
	memset(pMat,0,16*sizeof(float));
	pMat[0 ] = 1.0;
	pMat[5 ] = 1.0;
	pMat[10] = 1.0;
	pMat[15] = 1.0;
}
/*
void CreateOrthoMatrixOpenGL(float* pMat)
{
 	pMat[0]  =  2.0/CurrentWindowWidth;
 	pMat[2]  =  0.0;
 	pMat[3]  =  0.0;
	pMat[4]  =  0.0;
 	pMat[5]  = -2.0/CurrentWindowHeight;
	pMat[6]  =  0.0;
 	pMat[7]  =  0.0;
	pMat[8]  =  0.0;
	pMat[9]  =  0.0;
	pMat[10] =  1.0/(1000.0-0.0);
 	pMat[11] =  0.0;
	pMat[12] = -1.0;
	pMat[13] =  1.0;
	pMat[14] =  0.0;
	pMat[15] =  1.0;
}
*/



































