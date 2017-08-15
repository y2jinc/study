//app.cpp written by david paull.
//could/should be animator.cpp
// Copyright (c) 2002 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to

#include "stdafx.h"
#include "resource.h"
#include <math.h>	//sqrt
#include <stdio.h>	//sprintf
#include <assert.h>
#include "Library.h"
#include "FileAccess.h"
#include "Render.h"
#include "AppMenu.h"
#include "AppTools.h"
#include "FieldLL.h"
#include "IconLL.h"
#include "ObjectLL.h"
#include "MeshLL.h"
#include "KeyFrameLL.h"
#include "JointLL.h"

//////////////////
//// #defines ////
//////////////////

#define SHOW_DATAVIEW

/////////////////////////////////////////
//////// Extern Global Variables ////////
/////////////////////////////////////////
extern HWND  Render_hWnd;
extern API_TYPE Render_ApiType;
extern FontObject 		 AppTools_Fonts[];
extern struct FieldLL*  pRender_FieldHead;
extern struct IconLL* 	pRender_IconHead;
extern struct MeshLL*  	pAppTools_MousePointer;
extern struct MeshLL*  	pAppTools_VertexHandles;
extern struct SceneLL* 	pAppTools_Scene;
extern struct ObjectLL*	pAppTools_Object;
extern struct MeshLL*  	pAppTools_PickSphere;
extern struct MeshLL*  	pAppTools_PickPlane;
extern struct MeshLL*  	pAppTools_XYZ;
extern struct MeshLL*  	pAppTools_XYZSphere;
extern struct MeshLL*  	pAppTools_UVN;
extern struct MeshLL*  	pAppTools_Vector;
extern struct MeshLL*	pAppTools_VectorX;
extern struct MeshLL*	pAppTools_VectorY;
extern struct MeshLL*	pAppTools_VectorZ;

extern float Render_WindowWidth;
extern float Render_WindowHeight;
extern float pRender_ViewMatrix[];
extern float pRender_ProjMatrix[];
extern float pRender_IdentityMatrix[];
extern float pRender_OrthoMatrix[];
extern float IconWidth;
extern float IconScale;
extern float TempHearchyX;
extern float TempHearchyY;
extern long Render_ClearColor;

extern bool bCurrentlyLoadingSystemFiles;

////////////////////////////////////////////
//////// Extern Function Prototypes ////////
////////////////////////////////////////////

extern void (__stdcall* API_RenderMesh  )(struct MeshLL* pMesh,float* pWorld,float* pView,float* pProj,float* pOrtho);

//////////////////////////////////////////
//////////// Global Variables ////////////
//////////////////////////////////////////

unsigned long 		App_DataViewFlags = APP_SHOWJOINTS|APP_SHOWBONES;

struct FieldLL*		pSelectedField = NULL;
struct AppItem		 CurrentlySelectedApp;//only 1 App allowed, so use physical memory.
struct AppItem*		pCurrentlySelectedApp       	= &CurrentlySelectedApp;
struct SceneLL* 	pCurrentlySelectedScene       	= NULL;
struct ObjectLL* 	pCurrentlySelectedObject      	= NULL;
struct MeshLL* 		pCurrentlySelectedMesh        	= NULL;
aTEXTURE* 			pCurrentlySelectedTexture     	= NULL;
struct SkeletonLL*  pCurrentlySelectedSkeleton    	= NULL;
struct KeyFrameLL*  pCurrentlySelectedKeyFrame    	= NULL;
struct JointLL* 	pCurrentlySelectedJoint        	= NULL;
float*		 		pCurrentlySelectedMatrix      	= NULL;
bool				bCurrentlyEditingWeights 	  	= false;//uses to init, and restore flags for edit weight mode
MENU_MODE     		CurrentMenuMode     = MENU_MODE_ICONBARMAINMENU;
MOUSE_MODE    		CurrentMouseMode    = MOUSE_MODE_MENU;
KEYBOARD_MODE 		CurrentKeyboardMode = KEYBOARD_MODE_MENU;

unsigned long* 		pCurrentlySelectedFlags 	= NULL;
unsigned long  		iCurrentlySelectedFlag  	= 0;
bool           		bCurrentlySelectedClear 	= false;
bool 		   		bCurrentlyRotatingObject	= false;
bool 		   		bCurrentlyRotatingMesh  	= false;
bool 		   		bCurrentlyRotatingJoint  	= false;
bool 		   		bCurrentlyMovingObject  	= false;
bool 		   		bCurrentlyMovingMesh    	= false;
bool 		   		bCurrentlyMovingJoint    	= false;
bool 		   		bCurrentlyMovingVerts   	= false;


float* 				pApp_AbsoluteMeshMatrix = NULL;//pMesh->pMat*pObj->pMat
float* 				pApp_TempMatrix = NULL;
float 				 App_CameraPositionSpeed = 3.0;
float 				 App_CameraRotationSpeed = 0.05;
float 	   			 App_PickSphereRadius 	= 5.0;
unsigned long   	 App_CurrentTime = 0;//ms
unsigned long   	 App_CurrentAnim = 0;//ms
int             	 App_PlayingMode = 0;//-1,0,1 reverse, stop, play
POINT 				 App_ptLastCursor;
POINT 				MouseMoveVertLastMousePos;
bool 				bLMouseButtonDown = false;
bool 				bMMouseButtonDown = false;
bool 				bRMouseButtonDown = false;
bool 				bCtrlKeyDown = false;
bool 				bShiftKeyDown = false;
aDOUBLE_VECTOR		CurrentMouseSpherePickVector          = {0.0, 0.0, 0.0};
aDOUBLE_VECTOR		PreviousMouseSpherePickVector         = {0.0, 0.0, 0.0};


float CenterLineYAdjustment = 21.0;
float App_BoneScale   = 1.0;
float App_JointScale  = 0.5;
float App_SprayRadius = 1.0;
unsigned long App_RenderFlags = APP_SHOWJOINTS|APP_SHOWBONES;
unsigned long LastCursorPos = 0;//the last known cursor position.

POINT ptApp_SelectRectStart;
POINT ptApp_SelectRectStop;
POINT CurrentCursorPos;
POINT LastSelectedPoint;
POINT CurrentMousePosition;


/////////////////////////////////////
//////// Function Prototypes ////////
/////////////////////////////////////
void App_ExitEditWeightMode();
void App_EnterEditWeightMode();
void App_EnterMouseLookMode();
LRESULT CALLBACK App_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void App_Draw_Animator();
void App_Draw_Skeleton(struct ObjectLL* pObject,
					struct SkeletonLL* pSkeleton,
					unsigned long anim_num,
					unsigned long time);
void App_Draw_UserInterface();


void App_handle_Keyboard();
void App_PaintWeights();
void App_CalculateBoneMatrix(	float* pMatrix,
							aVECTOR Pos,
							aVECTOR LookAtPos);

void App_UpdatePickSphere(struct ObjectLL* pObject);
void App_UpdatePickSphere(struct ObjectLL* pObject,struct MeshLL* pMesh);
void App_UpdatePickSphere(struct ObjectLL* pObject,struct JointLL* pJoint);

int  App_Draw_FieldText(FieldLL* pField,FontObject* pFont);
int  App_Draw_FieldCursor(FieldLL* pField,FontObject* pFont);
int  App_Draw_FieldBackground(FieldLL* pField,FontObject* pFont);
int  App_Draw_FieldIcons(FontObject* pFont);
void App_handle_WM_KEYDOWN(WPARAM wParam, LPARAM lParam);
void App_handle_WM_LBUTTONDOWN(HWND hwnd,WPARAM wParam,LPARAM lParam);
void App_handle_WM_RBUTTONDOWN();
void App_handle_WM_MOUSEMOVE(HWND hwnd,WPARAM wParam,LPARAM lParam);

bool App_Create(HINSTANCE hInstance)
{
	long i;
	char pName1[32];
	char pName2[32];
	char pName3[32];
	char pName4[32];
	char pName5[32];
	unsigned long OrthoAlphaTextureFlags = 	
		RENDER_ORTHOGRAPHIC|
		RENDER_TRANSLUCENT_TEXTURE|
		RENDER_VISIBLE|
		RENDER_TEXTURED;
	unsigned long NormalTextureMappedObjectFlags = 	
		RENDER_VISIBLE|
		RENDER_TEXTURED|
		RENDER_ZBUFFERREAD|
		RENDER_ZBUFFERWRITE|
		RENDER_LIGHTING|
		RENDER_SELECTABLE|
		RENDER_BILINEARFILTER;
	unsigned long VertexAlphaFlags = 	
		RENDER_VISIBLE|
		RENDER_TEXTURED|
		RENDER_LIGHTING|
		RENDER_BILINEARFILTER|
		RENDER_SELECTABLE|
		RENDER_TRANSLUCENT_VERTEX;
	unsigned long BoneFlags = 	
		RENDER_VISIBLE|
		RENDER_TEXTURED|
		RENDER_ZBUFFERREAD|
		RENDER_ZBUFFERWRITE|
		RENDER_BILINEARFILTER|
		//RENDER_LIGHTING|
		RENDER_SELECTABLE;

	if(!Render_Create(hInstance,App_WndProc))
	{
		return false;
	}

	if(!AppMenu_Create())
	{
		return false;
	}

	pApp_AbsoluteMeshMatrix = (float *)calloc(16,sizeof(float));
	pApp_TempMatrix = (float *)calloc(16,sizeof(float));


	//Set Current Application Name
	sprintf(pCurrentlySelectedApp->pName,"TechNature Editor");

	//////////////////////////////
	//// Load System Geometry ////
	//////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	bCurrentlyLoadingSystemFiles = true;//supress warning "cant alter system"////
	/////////////////////////////////////////////////////////////////////////////

	pAppTools_Scene  = MenuFunction_Scene_NewScene("System");
	pAppTools_Object = MenuFunction_Object_NewObject("System");
	/////////////////////////////////////////////////////////////////////////////
	pAppTools_PickSphere = MenuFunction_Mesh_OpenMesh("matrix.txt");
	pAppTools_PickSphere->RenderFlags = VertexAlphaFlags;
	/////////////////////////////////////////////////////////////////////////////
	pAppTools_PickPlane = MenuFunction_Mesh_OpenMesh("pickplane.txt");
	pAppTools_PickPlane->RenderFlags = VertexAlphaFlags;
	pAppTools_PickPlane->pMatrix[0]  = 20.0;//start x scale
	pAppTools_PickPlane->pMatrix[10] = 20.0;//start z scale
	//pAppTools_PickPlane->pMatrix[13] = -120.0;//start y height of plane
	
	/////////////////////////////////////////////////////////////////////////////
	pAppTools_XYZ = MenuFunction_Mesh_OpenMesh("xyz.txt");
	pAppTools_XYZ->RenderFlags = NormalTextureMappedObjectFlags;
	/////////////////////////////////////////////////////////////////////////////
	pAppTools_XYZSphere = MenuFunction_Mesh_OpenMesh("uvnsphere.txt");
	pAppTools_XYZSphere->RenderFlags = VertexAlphaFlags;
	/////////////////////////////////////////////////////////////////////////////
	pAppTools_UVN = MenuFunction_Mesh_OpenMesh("uvn.txt");
	pAppTools_UVN->RenderFlags = NormalTextureMappedObjectFlags;
	/////////////////////////////////////////////////////////////////////////////
	//pAppTools_Vector = MenuFunction_Mesh_OpenMesh("vector.txt");
	//pAppTools_Vector->RenderFlags = BoneFlags;

	pAppTools_Vector = MeshLL_CreateNode("Bone",MAX_VERTICES,MAX_INDICES,VertexAlphaFlags); 	
	MeshLL_ReInsert(pAppTools_Vector,pAppTools_Object->pMeshHead);
	CreateBoneGeometry(pAppTools_Vector);
	pAppTools_Vector->SrcBlend = BLEND_SRCCOLOR;
	pAppTools_Vector->DstBlend = BLEND_INVSRCCOLOR;
	pAppTools_Vector->RenderFlags = BoneFlags;

	bCurrentlyLoadingSystemFiles = false;//done using menu functions
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////
	//// Dynamically Created Geometry ////
	//////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	pAppTools_VectorX = MeshLL_CreateNode("Origin X Vector",	MAX_VERTICES,MAX_INDICES,VertexAlphaFlags); 	
	MeshLL_ReInsert(pAppTools_VectorX,pAppTools_Object->pMeshHead);
	CreateVectorGeometry(pAppTools_VectorX,0);
	pAppTools_VectorX->SrcBlend = BLEND_SRCCOLOR;
	pAppTools_VectorX->DstBlend = BLEND_INVSRCCOLOR;
	//////////////////////////////////////////////////////////////////////////////////////////////
	pAppTools_VectorY = MeshLL_CreateNode("Origin Y Vector",	MAX_VERTICES,MAX_INDICES,VertexAlphaFlags); 	
	MeshLL_ReInsert(pAppTools_VectorY,pAppTools_Object->pMeshHead);
	CreateVectorGeometry(pAppTools_VectorY,1);
	pAppTools_VectorY->SrcBlend = BLEND_SRCCOLOR;
	pAppTools_VectorY->DstBlend = BLEND_INVSRCCOLOR;
	//////////////////////////////////////////////////////////////////////////////////////////////
	pAppTools_VectorZ = MeshLL_CreateNode("Origin Z Vector",	MAX_VERTICES,MAX_INDICES,VertexAlphaFlags); 	
	MeshLL_ReInsert(pAppTools_VectorZ,pAppTools_Object->pMeshHead);
	CreateVectorGeometry(pAppTools_VectorZ,2);
	pAppTools_VectorZ->SrcBlend = BLEND_SRCCOLOR;
	pAppTools_VectorZ->DstBlend = BLEND_INVSRCCOLOR;
	//////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////
	//Init Font :: "Georgia","Times New Roman","Arial Black","Agency FB","Century Gothic","Arial" //
	////////////////////////////////////////////////////////////////////////////////////////////////


	for(i=0;i<2;i++)
	{
		if(i==0)
		{
			sprintf(AppTools_Fonts[i].pName,"Times New Roman");
		}
		if(i==1)
		{
			sprintf(AppTools_Fonts[i].pName,"Arial");
		}
		sprintf(pName1,"font %d : background",i);//32 chars max!
		sprintf(pName2,"font %d : font1",i);
		sprintf(pName3,"font %d : font2",i);
		sprintf(pName4,"font %d : icons",i);
		sprintf(pName5,"font %d : overlay",i);

		AppTools_Fonts[i].pBackground = MeshLL_CreateNode(pName1,MAX_VERTICES,MAX_INDICES,OrthoAlphaTextureFlags);
		MeshLL_ReInsert(AppTools_Fonts[i].pBackground,pAppTools_Object->pMeshHead);
		AppTools_Fonts[i].pBackground->SrcBlend = BLEND_SRCCOLOR;
		AppTools_Fonts[i].pBackground->DstBlend = BLEND_INVSRCCOLOR;

		AppTools_Fonts[i].pText1      = MeshLL_CreateNode(pName2,MAX_VERTICES,MAX_INDICES,OrthoAlphaTextureFlags);
		MeshLL_ReInsert(AppTools_Fonts[i].pText1,pAppTools_Object->pMeshHead);
		AppTools_Fonts[i].pText1->SrcBlend = BLEND_SRCCOLOR;
		AppTools_Fonts[i].pText1->DstBlend = BLEND_INVSRCCOLOR;

		AppTools_Fonts[i].pText2      = MeshLL_CreateNode(pName3,MAX_VERTICES,MAX_INDICES,OrthoAlphaTextureFlags);
		MeshLL_ReInsert(AppTools_Fonts[i].pText2,pAppTools_Object->pMeshHead);
		AppTools_Fonts[i].pText2->SrcBlend = BLEND_SRCCOLOR;
		AppTools_Fonts[i].pText2->DstBlend = BLEND_INVSRCCOLOR;

		AppTools_Fonts[i].pIcons      = MeshLL_CreateNode(pName4,MAX_VERTICES,MAX_INDICES,OrthoAlphaTextureFlags|RENDER_ZBUFFERREAD|RENDER_ZBUFFERWRITE);
		MeshLL_ReInsert(AppTools_Fonts[i].pIcons,pAppTools_Object->pMeshHead);
		AppTools_Fonts[i].pIcons->SrcBlend = BLEND_SRCCOLOR;
		AppTools_Fonts[i].pIcons->DstBlend = BLEND_INVSRCCOLOR;

		AppTools_Fonts[i].pOverlay    = MeshLL_CreateNode(pName5,MAX_VERTICES,MAX_INDICES,OrthoAlphaTextureFlags);
		MeshLL_ReInsert(AppTools_Fonts[i].pOverlay,pAppTools_Object->pMeshHead);
		AppTools_Fonts[i].pOverlay->SrcBlend = BLEND_SRCCOLOR;
		AppTools_Fonts[i].pOverlay->DstBlend = BLEND_INVSRCCOLOR;
	}
	//the smaller text needs bilinear filter
	AppTools_Fonts[1].pText1->RenderFlags |= RENDER_BILINEARFILTER;
	AppTools_Fonts[1].pText2->RenderFlags |= RENDER_BILINEARFILTER;


	///////////////////////////////////
	//// Create Mouse Pointer Mesh ////
	///////////////////////////////////

	pAppTools_MousePointer = MeshLL_CreateNode("mouse pointer",   MAX_VERTICES,MAX_INDICES,RENDER_ORTHOGRAPHIC|RENDER_TRANSLUCENT_TEXTURE|RENDER_VISIBLE|RENDER_TEXTURED);
	MeshLL_ReInsert(pAppTools_MousePointer,pAppTools_Object->pMeshHead);
	pAppTools_MousePointer->SrcBlend = BLEND_SRCCOLOR;
	pAppTools_MousePointer->DstBlend = BLEND_INVSRCCOLOR;
	LoadTexture("arrow.bmp",NULL,pAppTools_MousePointer->Texture,TEXTURE_FLAG_4444);


	////////////////////////////////////
	//// Create Vertex Handles Mesh ////
	////////////////////////////////////

	pAppTools_VertexHandles = MeshLL_CreateNode("vertex handles",   MAX_VERTICES,MAX_INDICES,RENDER_ORTHOGRAPHIC|RENDER_TRANSLUCENT_TEXTURE|RENDER_VISIBLE|RENDER_TEXTURED);
	MeshLL_ReInsert(pAppTools_VertexHandles,pAppTools_Object->pMeshHead);
	pAppTools_VertexHandles->SrcBlend = BLEND_SRCCOLOR;
	pAppTools_VertexHandles->DstBlend = BLEND_INVSRCCOLOR;
	LoadTexture("vertex.bmp",NULL,pAppTools_VertexHandles->Texture,TEXTURE_FLAG_4444);




	return true;
}

void App_Delete()
{
	SAFE_FREE(pApp_AbsoluteMeshMatrix);
	SAFE_FREE(pApp_TempMatrix);

	AppMenu_Delete();
	Render_Delete();
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	if(!App_Create(hInstance))
	{
		return 0;
	}

	/////////////////////
	//// Create Font ////
	/////////////////////
	
	CreateFontTexture(&AppTools_Fonts[0]);
	CreateFontTexture(&AppTools_Fonts[1]);

	//refresh menu for first viewing.
	RefreshSubordinates(pCurrentlySelectedScene);

	bool done=false;											// Bool Variable To Exit Loop
	MSG msg;

	while(!done)												// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))				// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)							// Have We Received A Quit Message?
			{
				done=true;										// If So done=TRUE
			}
			else												// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);							// Dispatch The Message
			}
		}
		else													// If There Are No Messages
		{//idle time
			Render_BeginScene();
				App_handle_Keyboard();
				App_Draw_Animator();
				App_Draw_UserInterface();
			Render_EndScene();
		}
	}

	App_Delete();

	return 0;
}




LRESULT CALLBACK App_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
		case WM_CHAR:
			if(pSelectedField)
			{//FieldLL handles keydown here
				FieldLL_WM_CHAR(pSelectedField,wParam,lParam);
			}
			break;

		case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
	        break;

		case WM_LBUTTONDOWN://LEFT MOUSE BTTN
			//get position of cursor when left mouse button is pressed.
		    GetCursorPos(&LastSelectedPoint);
		    ScreenToClient(Render_hWnd,&LastSelectedPoint);

		    GetCursorPos(&CurrentMousePosition);
		    ScreenToClient(Render_hWnd,&CurrentMousePosition);

			//bLMouseButtonDown = (bool)(wParam & MK_LBUTTON);
			//bMMouseButtonDown = (bool)(wParam & MK_MBUTTON);
			//bRMouseButtonDown = (bool)(wParam & MK_RBUTTON);

			App_handle_WM_LBUTTONDOWN(hwnd,wParam,lParam);

        	break;

		case WM_LBUTTONUP://RIGHT MOUSE BTTN
			//bLMouseButtonDown = (bool)(wParam & MK_LBUTTON);
			//bMMouseButtonDown = (bool)(wParam & MK_MBUTTON);
			//bRMouseButtonDown = (bool)(wParam & MK_RBUTTON);
	        break;

		case WM_MOUSEMOVE:
			App_handle_WM_MOUSEMOVE(hwnd,wParam,lParam);
        	break;

		case WM_RBUTTONDOWN://RIGHT MOUSE BTTN
			//bLMouseButtonDown = wParam & MK_LBUTTON;
			//bMMouseButtonDown = wParam & MK_MBUTTON;
			//bRMouseButtonDown = wParam & MK_RBUTTON;
			App_handle_WM_RBUTTONDOWN();
	        break;

		case WM_RBUTTONUP://RIGHT MOUSE BTTN
			//bLMouseButtonDown = wParam & MK_LBUTTON;
			//bMMouseButtonDown = wParam & MK_MBUTTON;
			//bRMouseButtonDown = wParam & MK_RBUTTON;
	        break;

		case WM_KEYDOWN:
			App_handle_WM_KEYDOWN(wParam,lParam);
			break;

	    default:
	        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



void App_Draw_Skeleton(struct ObjectLL* pObject,
					struct SkeletonLL* pSkeleton,
					unsigned long anim_num,
					unsigned long time)
{
	aVECTOR Pos,LookAtPos;
	struct JointLL*    	pJoint		= NULL;
	struct KeyFrameLL* 	pPrev 	   	= NULL;
	struct KeyFrameLL* 	pThis 	   	= NULL;
	struct KeyFrameLL* 	pNext 	   	= NULL;
	bool bRestoreColor = false;
	if(pSkeleton==NULL)return;


	////////////////////////////////////
	//// Render Actor Matrix Offset ////
	////////////////////////////////////
/*
	if(App_RenderFlags&APP_SHOWBONES)
	{
		LookAtPos.x = pObject->pMatrix[12];
		LookAtPos.y = pObject->pMatrix[13];
		LookAtPos.z = pObject->pMatrix[14];
		Pos.x = 0.0;
		Pos.y = 0.0;
		Pos.z = 0.0;
		App_CalculateBoneMatrix(pAppTools_Vector->pMatrix,Pos,LookAtPos);
   		MeshLL_ColorizeWhite(pAppTools_Vector);
		Render_Mesh(NULL,pAppTools_Object,pAppTools_Vector);//note im using pAppTools_Object, for no transform
		//restore to blue
   	 //	MeshLL_ColorizeBlue(pAppTools_Vector);
	}
*/

	////////////////////////////////////////////
	//// Render Skeletons Joints, and Bones ////
	////////////////////////////////////////////
	if(KeyFrameLL_Get(pSkeleton->pKeyFrameHead,pPrev,pThis,pNext,anim_num,time))
	{
		pJoint = pThis->pJointHead;
		while(pJoint)
		{
			if(App_RenderFlags&APP_SHOWJOINTS)
			{
				///////////////
				////  XYZ  ////
				///////////////

    			if(pAppTools_XYZ)
				{
    				pAppTools_XYZ->pMatrix[12] = pJoint->pAbsoluteMatrix[12];
    				pAppTools_XYZ->pMatrix[13] = pJoint->pAbsoluteMatrix[13];
    				pAppTools_XYZ->pMatrix[14] = pJoint->pAbsoluteMatrix[14];
				}

				/////////////////////
				////  XYZSphere  ////
				///////////////////3///UT Value

    			if(pAppTools_XYZSphere)
				{
    				pAppTools_XYZSphere->pMatrix[12] = pJoint->pAbsoluteMatrix[12];
    				pAppTools_XYZSphere->pMatrix[13] = pJoint->pAbsoluteMatrix[13];
    				pAppTools_XYZSphere->pMatrix[14] = pJoint->pAbsoluteMatrix[14];
				}

				///////////////
				////  UVN  ////
				///////////////

				float pScaleMat[16];

    			if(pAppTools_UVN)
				{
					Scale(pScaleMat,App_JointScale);
					MatrixMul(pAppTools_UVN->pMatrix,pScaleMat,pJoint->pAbsoluteMatrix);
					Render_Mesh(NULL,pObject,pAppTools_UVN);
				}


    			if(pAppTools_XYZSphere)
				{
					Scale(pAppTools_XYZSphere->pMatrix,App_JointScale);
    				pAppTools_XYZSphere->pMatrix[12] = pJoint->pAbsoluteMatrix[12];
    				pAppTools_XYZSphere->pMatrix[13] = pJoint->pAbsoluteMatrix[13];
    				pAppTools_XYZSphere->pMatrix[14] = pJoint->pAbsoluteMatrix[14];
					Render_Mesh(NULL,pObject,pAppTools_XYZSphere);
				}


/*	
    			if(pAppTools_UVN)
				{
					memcpy(	pAppTools_UVN->pMatrix,
							pJoint->pAbsoluteMatrix,
							16*sizeof(float));
				}
*/	
				/////////////////////////////
				//// Render Joint Marker ////
				/////////////////////////////
//				Render_Mesh(NULL,pObject,pAppTools_XYZ);
//				Render_Mesh(NULL,pObject,pAppTools_XYZSphere);
//				Render_Mesh(NULL,pObject,pAppTools_UVN);

			}

			if(App_RenderFlags&APP_SHOWBONES)
			{
				/////////////
				////  P  ////
				/////////////


				LookAtPos.x = pJoint->pAbsoluteMatrix[12];
				LookAtPos.y = pJoint->pAbsoluteMatrix[13];
				LookAtPos.z = pJoint->pAbsoluteMatrix[14];

				if(strcmp(pJoint->pInfoItem->pParentName,"null")==0)
				{
					Pos.x = 0.0;
					Pos.y = 0.0;
					Pos.z = 0.0;
				}
				else
				{
					Pos.x = pJoint->pParent->pAbsoluteMatrix[12];
					Pos.y = pJoint->pParent->pAbsoluteMatrix[13];
					Pos.z = pJoint->pParent->pAbsoluteMatrix[14];

					App_CalculateBoneMatrix(pAppTools_Vector->pMatrix,Pos,LookAtPos);
					

		   			MeshLL_ColorizeBlue(pAppTools_Vector);

					if(pCurrentlySelectedJoint)
					{
						if(strcmp(pJoint->pInfoItem->pName,pCurrentlySelectedJoint->pInfoItem->pName)==0)
						{	
							MeshLL_ColorizeWhite(pAppTools_Vector);
						}
					}

					if(pCurrentlySelectedJoint)
					{
						if(strcmp(pJoint->pInfoItem->pParentName,pCurrentlySelectedJoint->pInfoItem->pName)==0)
						{	
							MeshLL_ColorizeGreen(pAppTools_Vector);
						}
					}

					///////////////////////////////////
					/// Render Joint Offset "bone" ////
					///////////////////////////////////

					Render_Mesh(NULL,pObject,pAppTools_Vector);

				}
			}
			pJoint = pJoint->pNext;
		}
	}
}
//not mesh function because of actor matrix
void App_Draw_VertexHandles(struct ObjectLL* pObject)
{
	VERTEX_XYZNRGBUV DstV;
	VERTEX_XYZNRGBUV SrcV;
	VERTEX_XYZNRGBUV Vert[4];
	VERTEX_XYZNRGBUV* pWorldVerts = NULL;
	VERTEX_XYZNRGBUV* pLocalVerts = NULL;
	VERTEX_XYZNRGBUV* pSEV_WorldVerts = NULL;
	WORD*             pSEV_Indices = NULL;
	struct MeshLL* pMesh = NULL;
	unsigned char*		pVertexFlags = NULL;//selected, visible
	float Scale = 5.0;//halfscale
	long Diffuse = 0xffffffff;
	long i;	
	float pTempMatrix[16];
	float pCameraMatrix[16];
	float pFinalModelMatrix[16];
	float u1;
	float u2;
	float v1;
	float v2;
	bool bInView = false;

	SrcV.x = 0.0;
	SrcV.y = 0.0;
	SrcV.z = 0.0;

	pAppTools_VertexHandles->num_verts = 0;
	pAppTools_VertexHandles->num_tris  = 0;


	pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		//CALCULATEFINALMESHMATRIX
		if(0)//pMesh->RenderFlags & RENDER_TRANSFORMED)
		{
			memcpy(	pFinalModelMatrix,
		  			pObject->pMatrix,
					//pRender_IdentityMatrix,
					16*sizeof(float));
		}
		else
		{
			MatrixMul(pFinalModelMatrix,pMesh->pMatrix,pObject->pMatrix);
		}

		MatrixMul4x4(pTempMatrix,pFinalModelMatrix,pRender_ViewMatrix);
		MatrixMul4x4(pCameraMatrix,pTempMatrix,pRender_ProjMatrix);

		pLocalVerts = pMesh->pLocalVerts;//src
		pWorldVerts = pMesh->pWorldVerts;//src
		pVertexFlags = pMesh->pVertexFlags;

		pSEV_WorldVerts = pAppTools_VertexHandles->pWorldVerts;//dst
		pSEV_Indices    = pAppTools_VertexHandles->pIndices;
		
		//////////////////////////////////
		//// DRAW SELECTION RECTANGLE ////
		//////////////////////////////////
		
		if( (GetAsyncKeyState(VK_SHIFT  ) < 0) &&
			(GetAsyncKeyState(VK_LBUTTON) < 0 || GetAsyncKeyState(VK_RBUTTON) < 0))
		{//shift down + lmb, or rmb down
			POINT ptUL;
			POINT ptLR;
			if(ptApp_SelectRectStart.y<ptApp_SelectRectStop.y)
			{
				ptUL.y = ptApp_SelectRectStart.y;
				ptLR.y = ptApp_SelectRectStop.y;
			}
			else
			{
				ptUL.y = ptApp_SelectRectStop.y;
				ptLR.y = ptApp_SelectRectStart.y;
			}
			//find left, and right
			if(ptApp_SelectRectStart.x<ptApp_SelectRectStop.x)
			{
				ptUL.x = ptApp_SelectRectStart.x;
				ptLR.x = ptApp_SelectRectStop.x;
			}
			else
			{
				ptUL.x = ptApp_SelectRectStop.x;
				ptLR.x = ptApp_SelectRectStart.x;
			}
			//guarantee that the rect spans at least 1 X pixel
			if(ptUL.x==ptLR.x)
			{
				ptLR.x = ptUL.x+1;
			}
			//guarantee that the rect spans at least 1 Y pixel
			if(ptUL.y==ptLR.y)
			{
				ptLR.y = ptUL.y+1;
			}

			Vert[0].x =  ptUL.x-0.5;
			Vert[0].y =  ptUL.y-0.5;
			Vert[0].z =  1.0;

			Vert[1].x =  ptLR.x-0.5;
			Vert[1].y =  ptUL.y-0.5;
			Vert[1].z =  1.0;

			Vert[2].x =  ptUL.x-0.5;
			Vert[2].y =  ptLR.y-0.5;
			Vert[2].z =  1.0;

			Vert[3].x =  ptLR.x-0.5;
			Vert[3].y =  ptLR.y-0.5;
			Vert[3].z =  1.0;

			u1 = 23.0/32.0;
			u2 = 23.0/32.0;
			v1 =  4.0/32.0;
			v2 =  4.0/32.0;

			//Purge if necessary
			if(	(pAppTools_VertexHandles->num_verts +4) >= MAX_VERTICES ||
				(pAppTools_VertexHandles->num_tris*3+6) >= MAX_INDICES)
			{
				Render_Mesh(NULL,pAppTools_Object,pAppTools_VertexHandles);
				pAppTools_VertexHandles->num_verts = 0;
				pAppTools_VertexHandles->num_tris  = 0;
			}

			FillXYZNRGBUV(&pSEV_WorldVerts[pAppTools_VertexHandles->num_verts+0],Vert[0].x,Vert[0].y,Vert[0].z,0.0,0.0,1.0,Diffuse,u1,v1);
			FillXYZNRGBUV(&pSEV_WorldVerts[pAppTools_VertexHandles->num_verts+1],Vert[1].x,Vert[1].y,Vert[1].z,0.0,0.0,1.0,Diffuse,u2,v1);
			FillXYZNRGBUV(&pSEV_WorldVerts[pAppTools_VertexHandles->num_verts+2],Vert[2].x,Vert[2].y,Vert[2].z,0.0,0.0,1.0,Diffuse,u1,v2);
			FillXYZNRGBUV(&pSEV_WorldVerts[pAppTools_VertexHandles->num_verts+3],Vert[3].x,Vert[3].y,Vert[3].z,0.0,0.0,1.0,Diffuse,u2,v2);
			
			//triangles are described in counter clock wise order
			pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+0 ] = pAppTools_VertexHandles->num_verts+0;
			pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+1 ] = pAppTools_VertexHandles->num_verts+2;
			pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+2 ] = pAppTools_VertexHandles->num_verts+1;

			pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+3 ] = pAppTools_VertexHandles->num_verts+1;
			pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+4 ] = pAppTools_VertexHandles->num_verts+2;
			pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+5 ] = pAppTools_VertexHandles->num_verts+3;
			
			//must inc verts after indices section
			pAppTools_VertexHandles->num_verts += 4;
			pAppTools_VertexHandles->num_tris += 2;
		}

		Mesh_FlagVerticesAsVisible(pObject->pMatrix,pMesh);//,pSystem_VisibleVertices);

		for(i=0;i<pMesh->num_verts;i++)
		{
			bInView = TransformVertex(&DstV,&pLocalVerts[i],pCameraMatrix,Render_WindowWidth,Render_WindowHeight);
			if(bInView)//this handle is in visible onscreen so render
			{

				Vert[0].x =  -Scale+DstV.x-0.5;
				Vert[0].y =  -Scale+DstV.y-0.5;
				Vert[0].z =   1.0;

				Vert[1].x =   Scale+DstV.x-0.5;
				Vert[1].y =  -Scale+DstV.y-0.5;
				Vert[1].z =   1.0;

				Vert[2].x =  -Scale+DstV.x-0.5;
				Vert[2].y =   Scale+DstV.y-0.5;
				Vert[2].z =   1.0;

				Vert[3].x =   Scale+DstV.x-0.5;
				Vert[3].y =   Scale+DstV.y-0.5;
				Vert[3].z =   1.0;

				if(pVertexFlags[i] & VERTEX_VISIBLE)
				{
					u1 =  0.0/32.0;
					u2 = 10.0/32.0;
				}
				else//not visible
				{
					u1 = 10.0/32.0;
					u2 = 20.0/32.0;
				}

				if(pVertexFlags[i] & VERTEX_SELECTED)
				{
					v1 =  0.0/32.0;
					v2 = 10.0/32.0;
				}
				else//not selected
				{
					v1 = 10.0/32.0;
					v2 = 20.0/32.0;
				}

				//Purge if necessary
				if(	(pAppTools_VertexHandles->num_verts +4) >= MAX_VERTICES ||
					(pAppTools_VertexHandles->num_tris*3+6) >= MAX_INDICES)
				{
					Render_Mesh(NULL,pAppTools_Object,pAppTools_VertexHandles);
					pAppTools_VertexHandles->num_verts = 0;
					pAppTools_VertexHandles->num_tris  = 0;
				}
				
				FillXYZNRGBUV(&pSEV_WorldVerts[pAppTools_VertexHandles->num_verts+0],Vert[0].x,Vert[0].y,Vert[0].z,0.0,0.0,1.0,Diffuse,u1,v1);
				FillXYZNRGBUV(&pSEV_WorldVerts[pAppTools_VertexHandles->num_verts+1],Vert[1].x,Vert[1].y,Vert[1].z,0.0,0.0,1.0,Diffuse,u2,v1);
				FillXYZNRGBUV(&pSEV_WorldVerts[pAppTools_VertexHandles->num_verts+2],Vert[2].x,Vert[2].y,Vert[2].z,0.0,0.0,1.0,Diffuse,u1,v2);
				FillXYZNRGBUV(&pSEV_WorldVerts[pAppTools_VertexHandles->num_verts+3],Vert[3].x,Vert[3].y,Vert[3].z,0.0,0.0,1.0,Diffuse,u2,v2);
				

				//triangles are described in counter clock wise order
				pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+0 ] = pAppTools_VertexHandles->num_verts+0;
				pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+1 ] = pAppTools_VertexHandles->num_verts+2;
				pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+2 ] = pAppTools_VertexHandles->num_verts+1;

				pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+3 ] = pAppTools_VertexHandles->num_verts+1;
				pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+4 ] = pAppTools_VertexHandles->num_verts+2;
				pSEV_Indices[(pAppTools_VertexHandles->num_tris*3)+5 ] = pAppTools_VertexHandles->num_verts+3;
				

				//must inc verts after indices section
				pAppTools_VertexHandles->num_verts += 4;
				pAppTools_VertexHandles->num_tris += 2;
			}
			else
			{
				printf("s");
			}
			
		}
	 	pMesh = pMesh->pNext;
	}

}







void App_Draw_UserInterface()
{
	struct SceneLL* 	pScene  = NULL;
	struct ObjectLL* 	pObject = NULL;
	struct MeshLL* 		pMesh   = NULL;
	struct FieldLL* 	pField  = NULL;
	struct IconLL* 		pIcon  	= NULL;
    struct BoneH* 		pThis   = NULL;
    int ret = 0;						
	
	////////////////////////////////////////////////////////
	//// Render UserInterface : Text, and Mouse Pointer ////
	////////////////////////////////////////////////////////
	//Main Menu Text is recreated every frame, so save cursor pos, and restore it afterwards

	if(pSelectedField)
	{
		LastCursorPos = pSelectedField->cursorpos;//the last known cursor position. Hack
	}
	//bBeginScene = true;//actually render.
	UpdateMainMenu(false);
	//bBeginScene = false;//cures drawprim error from no begin scene.
	if(pSelectedField)
	{
		pSelectedField->cursorpos = LastCursorPos;
	}

	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////

	pField = pRender_FieldHead;
	while(pField)
	{
		if(pField->flags & TEXT_FLAG_EDITABLE)
		{
			App_Draw_FieldBackground(pField,&AppTools_Fonts[0]);
		}
		pField = pField->pNext;
	}

	
	//render all icons


	App_Draw_FieldIcons(&AppTools_Fonts[0]);//ALL ICONS
	IconLL_RemoveALL(pRender_IconHead);


	#ifdef SHOW_DATAVIEW
	pField = pRender_FieldHead;
	while(pField)
	{
		//App_Draw_FieldText(pField,&AppTools_Fonts[0]);
		App_Draw_FieldText(pField,pField->pFont);
		pField = pField->pNext;
	}

	if(pSelectedField)
	{
		App_Draw_FieldCursor(pSelectedField,&AppTools_Fonts[0]);
	}
	#endif//SHOW_DATAVIEW

	//if(	CurrentMenuMode==MENU_MODE_ICONBARMAINMENU)
	if(CurrentMouseMode==MOUSE_MODE_MENU)
	{
		RefreshPointer2(pAppTools_MousePointer);
		Render_Mesh(NULL,pAppTools_Object,pAppTools_MousePointer);
	}
	else if(CurrentMouseMode==MOUSE_MODE_OBJECT ||
			CurrentMouseMode==MOUSE_MODE_MESH ||
			CurrentMouseMode==MOUSE_MODE_JOINT ||
			CurrentMouseMode==MOUSE_MODE_WEIGHT ||
			CurrentMouseMode==MOUSE_MODE_VERT)
	{			   
		if(	GetAsyncKeyState(VK_SHIFT) < 0 ||
			GetAsyncKeyState(VK_CONTROL) < 0)
		{
			RefreshPointer2(pAppTools_MousePointer);
			Render_Mesh(NULL,pAppTools_Object,pAppTools_MousePointer);
		}
	}
	

	//// Render Pick Sphere
	if(	CurrentMouseMode==MOUSE_MODE_OBJECT ||
		CurrentMouseMode==MOUSE_MODE_MESH ||
		CurrentMouseMode==MOUSE_MODE_JOINT)
	{
		if(	GetAsyncKeyState(VK_SHIFT) < 0 ||
			GetAsyncKeyState(VK_CONTROL) < 0)
		{
			//// Render Matrix's Manipulation Sphere
			Render_Mesh(NULL,pAppTools_Object,pAppTools_PickSphere);
		}
	}
}



void App_Draw_Animator()
{
	struct SceneLL* 	pScene  	= NULL;
	struct ObjectLL* 	pObject 	= NULL;
	struct SkeletonLL* 	pSkeleton  	= NULL;
	struct MeshLL* 		pMesh   	= NULL;
	struct FieldLL* 	pField  	= NULL;
	struct IconLL* 		pIcon  		= NULL;
	struct KeyFrameLL* 	pPrev 	   	= NULL;
	struct KeyFrameLL* 	pThis 	   	= NULL;
	struct KeyFrameLL* 	pNext 	   	= NULL;
	struct KeyFrameLL* 	pKeyFrame	= NULL;
	bool bValid_pPrev,bValid_pNext;
	unsigned long playtime;
	
    int ret = 0;						
	///////////////////////////////////////////////////////////////////////////////////////
	if(Render_ApiType == AT_None)
	{
		return;
	}
	///////////////////////////////////////////////////////////////////////////////////////

	unsigned long CurrentTime = GetElapsedMilliSeconds();

	if(App_PlayingMode == -1)
	{
		App_CurrentTime -= CurrentTime;
	}
	else if(App_PlayingMode == 1)
	{
		App_CurrentTime += CurrentTime;
	}

	///////////////////////////
	//// Render Pick Plane ////
	///////////////////////////

	Render_Mesh(NULL,pAppTools_Object,pAppTools_PickPlane);

	/////////////////////////////////
	//// Render Edit Weight Mode ////
	/////////////////////////////////

	if(pCurrentlySelectedObject)//dont swap
	{
		if(CurrentMouseMode == MOUSE_MODE_WEIGHT)
		{
			App_PaintWeights();
		}
	}
	

	//////////////////////////////////////////////////////////////////
	//// Render The Current Scenes Objects, Skeletons, and Meshes ////
	//////////////////////////////////////////////////////////////////

	if(pCurrentlySelectedScene)
	{
		pScene = pCurrentlySelectedScene;
		pObject = pScene->pObjectHead;
		while(pObject)
		{//loop through all Objects
			pSkeleton = pObject->pSkeletonHead;
			while(pSkeleton)
			{//loop through all Skeletons
				if(pSkeleton->looptime==0 || App_PlayingMode == 0)
				{//no looping, or not animating
					playtime = App_CurrentTime;
				}
				else
				{//looping
					playtime = (App_CurrentTime-pSkeleton->starttime)%pSkeleton->looptime;
				}

				if(KeyFrameLL_Get(pSkeleton->pKeyFrameHead,pPrev,pKeyFrame,pNext,pSkeleton->playing_anim_num,playtime))
				{//found a keyframe that matches current time exactly.
					KeyFrame_Copy(pKeyFrame,pSkeleton->pCurrPose);
				}
				else
				{
					bValid_pPrev = (pPrev && pPrev->anim_num == pSkeleton->playing_anim_num);
					bValid_pNext = (pNext && pNext->anim_num == pSkeleton->playing_anim_num && pNext->time<=KEYFRAME_MAXFRAME);
					///////////////////////////////////
					////////   pPrev &&  pNext ////////
					///////////////////////////////////
					if(bValid_pPrev && bValid_pNext)
					{
						KeyFrame_Slerp(	pSkeleton->pCurrPose,pPrev,pNext,playtime);
					}
					///////////////////////////////////
					////////  !pPrev &&  pNext ////////
					///////////////////////////////////
					if(!bValid_pPrev && bValid_pNext)
					{
						KeyFrame_Copy(pNext,pSkeleton->pCurrPose);
					}
					///////////////////////////////////
					////////   pPrev && !pNext ////////
					///////////////////////////////////
					if(bValid_pPrev && !bValid_pNext)
					{
						KeyFrame_Copy(pPrev,pSkeleton->pCurrPose);
					}
					
					///////////////////////////////////
					////////  !pPrev && !pNext ////////
					///////////////////////////////////
					if(!bValid_pPrev && !bValid_pNext)
					{
						KeyFrame_Copy(pSkeleton->pRestPose,pSkeleton->pCurrPose);
					}
				}


				//////////////////////////////////////////
				//// Reconnect Current Poses Skeleton ////
				//////////////////////////////////////////

				KeyFrame_TransformJoints_RelativeToAbsolute(pSkeleton->pCurrPose,pRender_IdentityMatrix);
				pSkeleton = pSkeleton->pNext;
			}

			///////////////////////////////////////
			//// Deform Mesh To Match Skeleton ////
			///////////////////////////////////////

			ObjectLL_DeformMeshesUsingSkeleton(pObject);

			//////////////////////////////////////////////
			//// Colorize Mesh Based On Joint Weights ////
			//////////////////////////////////////////////
			if(	CurrentMouseMode == MOUSE_MODE_WEIGHT &&
				pCurrentlySelectedJoint)
			{
				ObjectLL_ColorizeUsingBoneWeights(pObject,pCurrentlySelectedJoint->UniqueID,0,255,0);
			}


			/////////////////////////
			//// Render Skeleton ////
			/////////////////////////

			App_Draw_Skeleton(	pObject,
							  	pObject->pSkeletonHead,
								KEYFRAME_ANIMNUM0,
								KEYFRAME_CURRPOSE);

			///////////////////////////
			//// Render All Meshes ////
			///////////////////////////
			
			pMesh = pObject->pMeshHead;
			while(pMesh)	 
			{
				if(pMesh==pAppTools_MousePointer)
				{//hack? if browsing system objects, update pointer before drawing it.
					RefreshPointer2(pMesh);
				}

				Render_Mesh(pScene,pObject,pMesh);
				pMesh = pMesh->pNext;
			}
			pObject = pObject->pNext;
		}
	}

	////////////////////////////
	//// Origin XYZ Vectors ////
	////////////////////////////

	aVECTOR Dir;
	aVECTOR Pos;

	Pos.x = 0.0;
	Pos.y = 0.0;
	Pos.z = 0.0;
	if(1)//XYZOriginVectors
	{
		float pInverseMatrix[16];
		aVECTOR CameraPos;
		InverseMatrix(pInverseMatrix,pRender_ViewMatrix);
		CameraPos.x = pInverseMatrix[12];
		CameraPos.y = pInverseMatrix[13];
		CameraPos.z = pInverseMatrix[14];

		Dir.x = 100.0;
		Dir.y = 0.0;
		Dir.z = 0.0;
		CreateMatrixFacingPointOnOneAxis(Pos,Dir,CameraPos,pAppTools_VectorX->pMatrix);
		Dir.x = 0.0;
		Dir.y = 100.0;
		Dir.z = 0.0;
		CreateMatrixFacingPointOnOneAxis(Pos,Dir,CameraPos,pAppTools_VectorY->pMatrix);
		Dir.x = 0.0;
		Dir.y = 0.0;
		Dir.z = 100.0;
		CreateMatrixFacingPointOnOneAxis(Pos,Dir,CameraPos,pAppTools_VectorZ->pMatrix);

		Render_Mesh(NULL,pAppTools_Object,pAppTools_VectorX);
		Render_Mesh(NULL,pAppTools_Object,pAppTools_VectorY);
		Render_Mesh(NULL,pAppTools_Object,pAppTools_VectorZ);

	}

	//////////////////////////////////////////////////
	//// Vertex Mode - Show Particles On Vertices ////
	//////////////////////////////////////////////////

	if(	CurrentMouseMode == MOUSE_MODE_VERT ||
		CurrentMouseMode == MOUSE_MODE_WEIGHT)
	{
		if(	pCurrentlySelectedObject && 
			!bstrcmp(pCurrentlySelectedObject->pName,"System"))
		{
			App_Draw_VertexHandles(pCurrentlySelectedObject);
			Render_Mesh(NULL,pAppTools_Object,pAppTools_VertexHandles);
		}
	}
}


int  App_Draw_FieldText(FieldLL* pField,FontObject* pFont)
{
    long Diffuse = pField->textcolor;
	fRECT CharRect;
	fRECT Texture;
	fRECT Clip;
	
	fRECT Screen;
	float t;
	long i,j;//,k;
	char Ascii = 0;
	char* pText = pField->pText;
	long length = strlen(pField->pText);
	float PosX = 0.0;

	VERTEX_XYZNRGBUV* pSrcV = NULL;//local space text chars. 32 units wideXYZ, 32 texels across
	VERTEX_XYZNRGBUV pCamV[4];//camera space. cam at 0,0,0 text on near plane
	VERTEX_XYZNRGBUV* pDstV = NULL;
	long cDstV = 0;

	Screen.left   = 0;
	Screen.right  = Render_WindowWidth;
	Screen.top    = 0;
	Screen.bottom = Render_WindowHeight;



	assert(length<=FIELD_MAX_CHAR);


	if(pField == pSelectedField)
	{
		FieldKeepCursorInView(pField,pFont);
	}	
	for(i=0;i<length;i++)
	{
		Ascii = pText[i];

		if(Ascii>=32 && Ascii<=126)
		{//Valid Character
			pSrcV = &pFont->ptlV[(Ascii*4)];

			//KERNING PART 1
			if(Ascii==ASCII_SP)
			{//space
				if(pField->flags&TEXT_FLAG_KERNING)
				{
					PosX -= pFont->pCharRectXY[SPACE_CHAR].left*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
			else
			{//space
				if(pField->flags&TEXT_FLAG_KERNING)
				{
					PosX -= (pFont->pCharRectXY[Ascii].left)*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
			
			if(Ascii>=33 && Ascii<=63)
			{//!->? use font texture 0
				pDstV = &pFont->pText1->pWorldVerts[(pFont->pText1->num_verts)];
				cDstV = pFont->pText1->num_verts;
			}
			else if(Ascii>63)
			{//use font texture 1
				pDstV = &pFont->pText2->pWorldVerts[(pFont->pText2->num_verts)];
				cDstV = pFont->pText2->num_verts;
			}
			else if(Ascii==ASCII_SP)
			{
				goto LABEL_NO_CHAR_ADDED;
			}

			///////////////////////////////////////////////////
			//// Calculate Screen Space Clipping Rect      ////
			//// Assums No Camera Rotation, or Translation ////
			///////////////////////////////////////////////////

			//position text for menus : PART 1/2
			Clip.left   = pField->Screen.left  ;// + pField->screenx;  
			Clip.right  = pField->Screen.right ;// + pField->screenx; 
			Clip.top    = pField->Screen.top   ;// + pField->screeny;   
			Clip.bottom = pField->Screen.bottom;// + pField->screeny;

			///////////////////////////////////////////////
			//// Calculate Screen Space Character Rect ////
			///////////////////////////////////////////////

			memcpy(pCamV,pSrcV,4*sizeof(VERTEX_XYZNRGBUV));
			//xyz&uv of pDstV will be overwritten later
			memcpy(pDstV,pSrcV,4*sizeof(VERTEX_XYZNRGBUV));

			for(j=0;j<4;j++)
			{
				//scale text
				pCamV[j].x *= pField->fontscale;
				pCamV[j].y *= pField->fontscale;
				//position text 
				pCamV[j].x += pField->Screen.left;
				pCamV[j].y += pField->Screen.top;
				//center text vertically
				//text is centered on Y axis. ie the height ranges from -16 to +16.
				pCamV[j].y += CenterLineYAdjustment;
				//add kerning position
				pCamV[j].x += PosX;
				//scroll text.
				pCamV[j].x -= pField->xpos;
				pCamV[j].y -= pField->ypos;//used to clip menu correctly. see FieldLL.cpps InsertMenuItem2
			}


			CharRect.left   = pCamV[0].x;
			CharRect.right  = pCamV[3].x;
			CharRect.top    = pCamV[0].y;
			CharRect.bottom = pCamV[3].y;

			if(pField->flags&TEXT_FLAG_VERTICALSCALE)
			{//Scale Hieght to fit Vertically Inside Field Clip Rect
				CharRect.top    = Clip.top;   
				CharRect.bottom = Clip.bottom;
			}
			
			///// CLIP to field background ////////////////
			fRECT ClippedChar;
			fRECT Final;

			ClippedChar = ClipRectToRect(CharRect,Screen);
			Final = ClipRectToRect(ClippedChar,Clip);
			if((Final.left   == 0.0) &&
			   (Final.right  == 0.0) &&
			   (Final.top    == 0.0) &&
			   (Final.bottom == 0.0))
			{
				goto LABEL_NO_CHAR_ADDED;
			}
			else
			{
				Texture.left   = pDstV[0].tu;
				Texture.right  = pDstV[1].tu;
				Texture.top    = pDstV[0].tv;
				Texture.bottom = pDstV[2].tv;
				//Adjust UV's before XY
				if(Final.left != CharRect.left)
				{
					t = (Final.left-CharRect.left)/(CharRect.right-CharRect.left);
					pDstV[0].tu = t*(Texture.right-Texture.left)+Texture.left;
					pDstV[2].tu = pDstV[0].tu;
				}
				if(Final.right != CharRect.right)
				{
					t = (Final.right-CharRect.left)/(CharRect.right-CharRect.left);
					pDstV[1].tu = t*(Texture.right-Texture.left)+Texture.left;
					pDstV[3].tu = pDstV[1].tu;
				}
				if(!(pField->flags&TEXT_FLAG_VERTICALSCALE))
				{//clip UV's
					if(Final.top != CharRect.top)
					{
						t = (CharRect.top-Final.top)/(CharRect.top-CharRect.bottom);
						pDstV[0].tv = t*(Texture.bottom-Texture.top)+Texture.top;
						pDstV[1].tv = pDstV[0].tv;
					}
					if(Final.bottom != CharRect.bottom)
					{
						t = (CharRect.top-Final.bottom)/(CharRect.top-CharRect.bottom);
						pDstV[2].tv = t*(Texture.bottom-Texture.top)+Texture.top;
						pDstV[3].tv = pDstV[2].tv;
					}
				}

				CharRect.left   = Final.left;
				CharRect.right  = Final.right;
				CharRect.top    = Final.top;
				CharRect.bottom = Final.bottom;
			}
			/////END CLIP////////////////

			pDstV[0].x = CharRect.left - 0.5; 
			pDstV[0].y = CharRect.top  - 0.5;
			pDstV[0].z = 1.0;

			pDstV[1].x = CharRect.right  - 0.5; 
			pDstV[1].y = CharRect.top    - 0.5;
			pDstV[1].z = 1.0;

			pDstV[2].x = CharRect.left   - 0.5; 
			pDstV[2].y = CharRect.bottom - 0.5;
			pDstV[2].z = 1.0;

			pDstV[3].x = CharRect.right  - 0.5; 
			pDstV[3].y = CharRect.bottom - 0.5;
			pDstV[3].z = 1.0;


			pDstV[0].diffuse = Diffuse;
			pDstV[1].diffuse = Diffuse;
			pDstV[2].diffuse = Diffuse;
			pDstV[3].diffuse = Diffuse;

			if(Ascii>=33 && Ascii<=63)
			{//!->? use font texture 0
				pFont->pText1->num_verts += 4;
				pFont->pText1->num_tris += 2;
			}
			else if(Ascii>63)
			{//use font texture 1
				pFont->pText2->num_verts += 4;
				pFont->pText2->num_tris += 2;
			}

			LABEL_NO_CHAR_ADDED:;
			///////////////////////////////////////////////////////////////////
			//KERNING PART 2
			if(Ascii==ASCII_SP)
			{//space
				if(pField->flags&&TEXT_FLAG_KERNING)
				{
					PosX += pFont->pCharRectXY[SPACE_CHAR].right*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
			else
			{
				if(pField->flags&&TEXT_FLAG_KERNING)
				{
					PosX += pFont->pCharRectXY[Ascii].right*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
		}
	}

	if(pFont->pText1->num_tris>0)
	{
		API_RenderMesh(pFont->pText1,pFont->pText1->pMatrix,pRender_ViewMatrix,pRender_ProjMatrix,pRender_OrthoMatrix);
		pFont->pText1->num_verts = 0;
		pFont->pText1->num_tris = 0;
	}
	if(pFont->pText2->num_tris>0)
	{
		API_RenderMesh(pFont->pText2,pFont->pText2->pMatrix,pRender_ViewMatrix,pRender_ProjMatrix,pRender_OrthoMatrix);
		pFont->pText2->num_verts = 0;
		pFont->pText2->num_tris = 0;
	}
	return -1;
}
int  App_Draw_FieldCursor(FieldLL* pField,FontObject* pFont)
{
	fRECT CharRect;
	long i,j;
	char Ascii = 0;
	char* pText = &pField->pText[0];
	long length = strlen(pText);	
	float PosX = 0.0;
	assert(length<=FIELD_MAX_CHAR);

	fRECT Clip;

	VERTEX_XYZNRGBUV* pSrcV = NULL;//local space text chars. 32 units wideXYZ, 32 texels across
	VERTEX_XYZNRGBUV pCamV[4];//camera space. cam at 0,0,0 text on near plane
	VERTEX_XYZNRGBUV* pDstV = NULL;
	long cDstV = 0;

	if(length==pField->cursorpos)
	{//cursor is past last char in text string.
		length++;//take one more step to include the cursor.
	}
	for(i=0;i<length;i++)
	{
		Ascii = pText[i];
		if(Ascii==0)
		{//cursor is past last char in text string.
			Ascii=ASCII_SP;//tell cursor to emulate space char
		}
		if(Ascii>=32 && Ascii<=126)
		{//Valid Character
			pSrcV = &pFont->ptlV[(Ascii*4)];
			if(Ascii==ASCII_SP)
			{//space
				pSrcV = &pFont->ptlV[(SPACE_CHAR*4)];//@
			}
	
			if(i==pField->cursorpos)
			{//render cursor!
				//KERNING PART 1
				if(Ascii==ASCII_SP)
				{//space
					if(pField->flags&TEXT_FLAG_KERNING)
					{
						PosX -= pFont->pCharRectXY[SPACE_CHAR].left*pField->fontscale;
					}
					else
					{
						PosX += (pFont->CharWidth/2)*pField->fontscale;
					}
				}
				else
				{//normal char
					if(pField->flags&TEXT_FLAG_KERNING)
					{
						PosX -= pFont->pCharRectXY[Ascii].left*pField->fontscale;
					}
					else
					{
						PosX += (pFont->CharWidth/2)*pField->fontscale;
					}
				}

				
				pDstV = &pFont->pIcons->pWorldVerts[(pFont->pIcons->num_verts)];
				cDstV = pFont->pIcons->num_verts;
				///////////////////////////////////////////////////
				//// Calculate Screen Space Clipping Rect      ////
				//// Assums No Camera Rotation, or Translation ////
				///////////////////////////////////////////////////
				Clip = pField->Screen;

				///////////////////////////////////////////////
				//// Calculate Screen Space Character Rect ////
				///////////////////////////////////////////////

				memcpy(pCamV,pSrcV,4*sizeof(VERTEX_XYZNRGBUV));
				//xyz&uv of pDstV will be overwritten later
				memcpy(pDstV,pSrcV,4*sizeof(VERTEX_XYZNRGBUV));


				for(j=0;j<4;j++)
				{
					//scale text
					pCamV[j].x *= pField->fontscale;
					pCamV[j].y *= pField->fontscale;
					//position text 
					pCamV[j].x += pField->Screen.left;
					pCamV[j].y += pField->Screen.top;
					//center text vertically
					//text is centered on Y axis. ie the height ranges from -16 to +16.
					pCamV[j].y += CenterLineYAdjustment;
					//add kerning position
					pCamV[j].x += PosX;
					//scroll text.
					pCamV[j].x -= pField->xpos;
					pCamV[j].y -= pField->ypos;
				}

				CharRect.left   = pCamV[0].x;
				CharRect.right  = pCamV[3].x;
				CharRect.top    = pCamV[0].y;
				CharRect.bottom = pCamV[3].y;

				if(pField->flags&TEXT_FLAG_VERTICALSCALE)
				{//Scale Hieght to fit Vertically Inside Field Clip Rect
					CharRect.top    = Clip.top;   
					CharRect.bottom = Clip.bottom;
				}

			
				///// CLIP to field background ////////////////
				fRECT Final;
				Final = ClipRectToRect(CharRect,Clip);
				if((Final.left   == 0.0) &&
				   (Final.right  == 0.0) &&
				   (Final.top    == 0.0) &&
				   (Final.bottom == 0.0))
				{
					goto LABEL_NO_CHAR_ADDED3;
				}
				else
				{
					CharRect.left   = Final.left  ;
					CharRect.right  = Final.right ;
					CharRect.top    = Final.top   ;
					CharRect.bottom = Final.bottom;
				}
				/////END CLIP////////////////

				//// Attach to camera in world space.

				pDstV[0].x = CharRect.left; 
				pDstV[0].y = CharRect.top;
				pDstV[0].z = 1.0;

				pDstV[1].x = CharRect.right; 
				pDstV[1].y = CharRect.top;
				pDstV[1].z = 1.0;

				pDstV[2].x = CharRect.left; 
				pDstV[2].y = CharRect.bottom;
				pDstV[2].z = 1.0;

				pDstV[3].x = CharRect.right; 
				pDstV[3].y = CharRect.bottom;
				pDstV[3].z = 1.0;

				//CURSORUVCOORDS
				for(int k=0;k<4;k++)
				{//cursor UV from icons.bmp
					pDstV[k].tu = 286.0/512.0;
					pDstV[k].tv = 64.0/512.0;
				}




				//always use font texture 0 for cursor
				pFont->pIcons->num_verts += 4;
				pFont->pIcons->num_tris += 2;
			}
			else//not a selected character
			{//Keep searching for selected character 
				//KERNING PART 1
				if(Ascii==ASCII_SP)
				{//space
					if(pField->flags&TEXT_FLAG_KERNING)
					{
						PosX -= pFont->pCharRectXY[SPACE_CHAR].left*pField->fontscale;
					}
					else
					{
						PosX += (pFont->CharWidth/2)*pField->fontscale;
					}
				}
				else
				{//space
					if(pField->flags&TEXT_FLAG_KERNING)
					{
						PosX -= pFont->pCharRectXY[Ascii].left*pField->fontscale;
					}
					else
					{
						PosX += (pFont->CharWidth/2)*pField->fontscale;
					}
				}
			}

			//KERNING PART 2
			if(Ascii==ASCII_SP)
			{//space
				if(pField->flags&&TEXT_FLAG_KERNING)
				{
					PosX += pFont->pCharRectXY[SPACE_CHAR].right*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
			else
			{
				if(pField->flags&&TEXT_FLAG_KERNING)
				{
					PosX += pFont->pCharRectXY[Ascii].right*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
		}
	}

	LABEL_NO_CHAR_ADDED3:;


	unsigned long OriginalSrcBlend = pFont->pIcons->SrcBlend;
	unsigned long OriginalDstBlend = pFont->pIcons->DstBlend;

	pFont->pIcons->SrcBlend = BLEND_ONE;
	pFont->pIcons->DstBlend = BLEND_ONE;
//	pFont->pIcons->SrcBlend = BLEND_SRCCOLOR;
//	pFont->pIcons->DstBlend = BLEND_INVSRCCOLOR;
//	pFont->pIcons->DstBlend = BLEND_DESTCOLOR;
//
//	AppTools_Fonts[0].pBackground->SrcBlend = BLEND_ZERO;
//	AppTools_Fonts[0].pBackground->DstBlend = BLEND_ZERO;

	if(pFont->pIcons->num_tris>0)
	{
		API_RenderMesh(pFont->pIcons,pFont->pIcons->pMatrix,pRender_ViewMatrix,pRender_ProjMatrix,pRender_OrthoMatrix);
		pFont->pIcons->num_verts = 0;
		pFont->pIcons->num_tris = 0;
	}

	pFont->pIcons->SrcBlend = OriginalSrcBlend;
	pFont->pIcons->DstBlend = OriginalDstBlend;

	return -1;
}




int  App_Draw_FieldBackground(FieldLL* pField,FontObject* pFont)
{
	fRECT Texture;
	char Ascii = 0;
	char* pText = &pField->pText[0];
	long length = strlen(pText);	
	float PosX = 0.0;
	float PosY = 0.0;
	assert(length<=FIELD_MAX_CHAR);

	VERTEX_XYZNRGBUV* pSrcV = NULL;//local space text chars. 32 units wideXYZ, 32 texels across
	VERTEX_XYZNRGBUV* pDstV = NULL;
	long cDstV = 0;
	long Diffuse = 0xffffffff;
	long Specular = 0x00000000;
	float rhw = 1.0;

	///////////////////////////////////////////////////
	//// Calculate Rect For Field Background Frame ////
	///////////////////////////////////////////////////
	Texture.left   = 307.0/512.0;//270//EDITABLEFIELD BG COLOR HERE~
	Texture.right  = 307.0/512.0;
	Texture.top    = 127.5/512.0;//118
	Texture.bottom = 127.5/512.0;

	pDstV = &pFont->pIcons->pWorldVerts[(pFont->pIcons->num_verts)];
	cDstV = pFont->pIcons->num_verts;

	FillXYZNRGBUV(&pDstV[0],pField->Screen.left -0.5,pField->Screen.top   -0.5,1.0,  0.0,0.0,1.0   ,Diffuse,Texture.left, Texture.top);
	FillXYZNRGBUV(&pDstV[1],pField->Screen.right-0.5,pField->Screen.top   -0.5,1.0,  0.0,0.0,1.0   ,Diffuse,Texture.right,Texture.top);
	FillXYZNRGBUV(&pDstV[2],pField->Screen.left -0.5,pField->Screen.bottom-0.5,1.0,  0.0,0.0,1.0   ,Diffuse,Texture.left, Texture.bottom);
	FillXYZNRGBUV(&pDstV[3],pField->Screen.right-0.5,pField->Screen.bottom-0.5,1.0,  0.0,0.0,1.0   ,Diffuse,Texture.right,Texture.bottom);

	pFont->pIcons->num_verts += 4;
	pFont->pIcons->num_tris += 2;

	if(pFont->pIcons->num_tris>0)
	{
		API_RenderMesh(pFont->pIcons,pFont->pIcons->pMatrix,pRender_ViewMatrix,pRender_ProjMatrix,pRender_OrthoMatrix);
		pFont->pIcons->num_verts = 0;
		pFont->pIcons->num_tris = 0;
	}
	return -1;
}











int  App_Draw_FieldIcons(FontObject* pFont)
{
	struct IconLL* 		pIcon  	= NULL;
	VERTEX_XYZNRGBUV* pDstV = NULL;
	
	long cDstV = 0;
	long index = 0;
	long Diffuse = 0xffffffff;
	long Specular = 0x00000000;
	float rhw = 1.0;
	fRECT ScreenRect;
	fRECT TextureRect;

	pDstV = &pFont->pIcons->pWorldVerts[(pFont->pIcons->num_verts)];
	cDstV = pFont->pIcons->num_verts;


	//render background using zbuffer to clip overlapping rects
	SetBit(pFont->pIcons->RenderFlags,RENDER_ZBUFFERREAD);
	SetBit(pFont->pIcons->RenderFlags,RENDER_ZBUFFERWRITE);

	pIcon = pRender_IconHead;
	while(pIcon && pIcon->SortKey==0)
	{
		ScreenRect  = pIcon->Screen;
		TextureRect = pIcon->Texture;
		FillXYZNRGBUV(&pDstV[index+0],ScreenRect.left -0.5,ScreenRect.top   -0.5, 1.0,   0.0,0.0,1.0  ,Diffuse,TextureRect.left, TextureRect.top   );
		FillXYZNRGBUV(&pDstV[index+1],ScreenRect.right-0.5,ScreenRect.top   -0.5, 1.0,   0.0,0.0,1.0  ,Diffuse,TextureRect.right,TextureRect.top   );
		FillXYZNRGBUV(&pDstV[index+2],ScreenRect.left -0.5,ScreenRect.bottom-0.5, 1.0,   0.0,0.0,1.0  ,Diffuse,TextureRect.left, TextureRect.bottom);
		FillXYZNRGBUV(&pDstV[index+3],ScreenRect.right-0.5,ScreenRect.bottom-0.5, 1.0,   0.0,0.0,1.0  ,Diffuse,TextureRect.right,TextureRect.bottom);
		index+=4;
		pFont->pIcons->num_verts += 4;
		pFont->pIcons->num_tris += 2;
		pIcon = pIcon->pNext;
	}

	if(pFont->pIcons->num_tris>0)
	{
		API_RenderMesh(pFont->pIcons,pFont->pIcons->pMatrix,pRender_ViewMatrix,pRender_ProjMatrix,pRender_OrthoMatrix);
		pFont->pIcons->num_verts = 0;
		pFont->pIcons->num_tris = 0;
	}

	
	index = 0;

	//render remaining icons. turn zbuffer off
	ClearBit(pFont->pIcons->RenderFlags,RENDER_ZBUFFERREAD);
	ClearBit(pFont->pIcons->RenderFlags,RENDER_ZBUFFERWRITE);

	while(pIcon)
	{
		ScreenRect  = pIcon->Screen;
		TextureRect = pIcon->Texture;
		FillXYZNRGBUV(&pDstV[index+0],ScreenRect.left -0.5,ScreenRect.top   -0.5, 1.0,   0.0,0.0,1.0  ,Diffuse,TextureRect.left, TextureRect.top   );
		FillXYZNRGBUV(&pDstV[index+1],ScreenRect.right-0.5,ScreenRect.top   -0.5, 1.0,   0.0,0.0,1.0  ,Diffuse,TextureRect.right,TextureRect.top   );
		FillXYZNRGBUV(&pDstV[index+2],ScreenRect.left -0.5,ScreenRect.bottom-0.5, 1.0,   0.0,0.0,1.0  ,Diffuse,TextureRect.left, TextureRect.bottom);
		FillXYZNRGBUV(&pDstV[index+3],ScreenRect.right-0.5,ScreenRect.bottom-0.5, 1.0,   0.0,0.0,1.0  ,Diffuse,TextureRect.right,TextureRect.bottom);
		index+=4;
		pFont->pIcons->num_verts += 4;
		pFont->pIcons->num_tris += 2;
		pIcon = pIcon->pNext;
	}

	if(pFont->pIcons->num_tris>0)
	{
		API_RenderMesh(pFont->pIcons,pFont->pIcons->pMatrix,pRender_ViewMatrix,pRender_ProjMatrix,pRender_OrthoMatrix);
		pFont->pIcons->num_verts = 0;
		pFont->pIcons->num_tris = 0;
	}



	return -1;
}







void App_handle_Keyboard()
{
	aVECTOR CameraPos;
	#ifdef LOG_FUNCTION_NAMES
	 if(DISK)
	 { 	DiskFile=fopen("log.log","a");
	 	fprintf(DiskFile,"KeyBoard()\n");
	 	fclose(DiskFile);						
	 }
	#endif//LOG_FUNCTION_NAMES

	static bool bShift = false;
	static bool bLastShift;
	static bool bControl = false;
	static bool bLastControl;

	bLastShift = bShift;
	bShift = (GetAsyncKeyState(VK_SHIFT) < 0);

	bLastControl = bControl;
	bControl = (GetAsyncKeyState(VK_CONTROL) < 0);


	//AVOID SNAP AFTER ROTATE/MOVE HERE
	if(	CurrentMouseMode == MOUSE_MODE_OBJECT ||
		CurrentMouseMode == MOUSE_MODE_MESH ||
		CurrentMouseMode == MOUSE_MODE_VERT ||
		CurrentMouseMode == MOUSE_MODE_TRI ||
		CurrentMouseMode == MOUSE_MODE_JOINT ||
		CurrentMouseMode == MOUSE_MODE_WEIGHT)
	{
		if( (!bShift   && bLastShift  ) || 
			(!bControl && bLastControl))
		{
			//reset prev to avoid snap. 
			App_EnterMouseLookMode();
		}
	}

	//MOUSELOOK HERE
	//_11, _12, _13, _14
	//_21, _22, _23, _24
	//_31, _32, _33, _34
	//_41, _42, _43, _44
	//
	//0  1  2  3 
	//4  5  6  7
	//8  9  10 11
	//12 13 14 15
	//MOUSELOOK HERE
	//MOUSELOOK HERE
	//MOUSELOOK HERE

	float pMatrix1[16];

	float pYawMatrix[16];
	float pInverseView[16];
	float pPitchMatrix[16];

	float pRollMatrix[16];
	float pIdentityMatrix[16];
	float pInverseMatrix[16];
	float pFinalMatrix[16];

	float epsilon = 0.0001;

	POINT ptCursor;
	int HalfWidth;
	int HalfHeight;

	static float YawAngle = 0.0;
	static float PitchAngle = 0.0;
	static float RollAngle = 0.0;

	if(Render_hWnd!=GetFocus())
	{//if we lose focus, verify that we are not in mouselook mode. 
		//mouselook forces pointer to center of screen even if minimized!(previously)
		CurrentMenuMode     = MENU_MODE_ICONBARMAINMENU;
		CurrentMouseMode    = MOUSE_MODE_MENU;
		CurrentKeyboardMode = KEYBOARD_MODE_MENU;
	}

	if(CurrentMenuMode!=MENU_MODE_ICONBARMAINMENU)
	{
		if(CurrentMouseMode!=MOUSE_MODE_MENU)
		{
			if(!bShift)
			{
				if(!bControl)
				{
					HalfWidth  = (int)(Render_WindowWidth*0.5);
					HalfHeight = (int)(Render_WindowHeight*0.5);

					GetCursorPos( &ptCursor );
					ScreenToClient(	Render_hWnd,&ptCursor);

					if((abs(ptCursor.x-App_ptLastCursor.x)>0) ||
					   (abs(ptCursor.y-App_ptLastCursor.y)>0))
					{
						if(bRMouseButtonDown)
						{
							RollAngle  += (float)(App_ptLastCursor.x-ptCursor.x)*0.01;
							RollAngle  *= 0.5;//smoothing
						}
						else
						{
							YawAngle   += (float)(ptCursor.x-App_ptLastCursor.x)*0.01;
							YawAngle   *= 0.5;
							
							PitchAngle += (float)(ptCursor.y-App_ptLastCursor.y)*0.01;
							PitchAngle *= 0.5;
						}
						
						if(YawAngle != 0.0)
						{
							Identity(pInverseView);
							InverseMatrix(pInverseView,pRender_ViewMatrix);
							CameraPos.x = pInverseView[12];
							CameraPos.y = pInverseView[13];
							CameraPos.z = pInverseView[14];
							AxisRotationMatrix(	pYawMatrix,
												pInverseView[4],
												pInverseView[5],
												pInverseView[6],
												YawAngle);
							MatrixMul(pMatrix1,pInverseView,pYawMatrix);
							pMatrix1[12] = CameraPos.x;
							pMatrix1[13] = CameraPos.y;
							pMatrix1[14] = CameraPos.z;
							InverseMatrix(pRender_ViewMatrix,pMatrix1);

						}
						if(PitchAngle != 0.0)
						{
							Identity(pInverseView);
							InverseMatrix(pInverseView,pRender_ViewMatrix);
							CameraPos.x = pInverseView[12];
							CameraPos.y = pInverseView[13];
							CameraPos.z = pInverseView[14];
							AxisRotationMatrix(	pPitchMatrix,
												pInverseView[0],//UT Value
												pInverseView[1],
												pInverseView[2],
												PitchAngle);
							MatrixMul(pMatrix1,pInverseView,pPitchMatrix);
							pMatrix1[12] = CameraPos.x;
							pMatrix1[13] = CameraPos.y;
							pMatrix1[14] = CameraPos.z;
							InverseMatrix(pRender_ViewMatrix,pMatrix1);
						}
						if(RollAngle != 0.0)
						{
							Identity(pInverseView);
							InverseMatrix(pInverseView,pRender_ViewMatrix);
							CameraPos.x = pInverseView[12];
							CameraPos.y = pInverseView[13];
							CameraPos.z = pInverseView[14];
							AxisRotationMatrix(	pRollMatrix,
												pInverseView[8],
												pInverseView[9],
												pInverseView[10],
												RollAngle);
							MatrixMul(pMatrix1,pInverseView,pRollMatrix);
							pMatrix1[12] = CameraPos.x;
							pMatrix1[13] = CameraPos.y;
							pMatrix1[14] = CameraPos.z;
							InverseMatrix(pRender_ViewMatrix,pMatrix1);
						}

						SetCursorPos(HalfWidth,HalfHeight);
 					}
					GetCursorPos( &App_ptLastCursor );
					ScreenToClient(	Render_hWnd,&App_ptLastCursor);
				}
			}
			RollAngle  *= 0.5;//smoothing decay
			YawAngle   *= 0.5;
			PitchAngle *= 0.5;
			if(fabs(RollAngle)<epsilon)RollAngle=0.0;
			if(fabs(YawAngle)<epsilon)YawAngle=0.0;
			if(fabs(PitchAngle)<epsilon)PitchAngle=0.0;
		}
	}

	if(CurrentKeyboardMode==KEYBOARD_MODE_CAMERA)
	{
		CameraPos.x = 0;
		CameraPos.y = 0;
		CameraPos.z = 0;

		if(GetAsyncKeyState(VK_W) < 0)
		{
			CameraPos.z += App_CameraPositionSpeed*0.1;
		}
		if(GetAsyncKeyState(VK_Z) < 0)
		{
			CameraPos.z -= App_CameraPositionSpeed*0.1;
		}

		if(GetAsyncKeyState(VK_A) < 0)
		{
			CameraPos.x -= App_CameraPositionSpeed*0.1;
		}
		if(GetAsyncKeyState(VK_S) < 0)
		{
			CameraPos.x += App_CameraPositionSpeed*0.1;
		}
		if(GetAsyncKeyState(VK_E) < 0)
		{
			CameraPos.y += App_CameraPositionSpeed*0.1;
		}
		if(GetAsyncKeyState(VK_X) < 0)
		{
			CameraPos.y -= App_CameraPositionSpeed*0.1;
		}

		if(GetAsyncKeyState(VK_UP) < 0)
		{
			CameraPos.z += App_CameraPositionSpeed;
		}
		if(GetAsyncKeyState(VK_DOWN) < 0)
		{
			CameraPos.z -= App_CameraPositionSpeed;
		}
		if(GetAsyncKeyState(VK_LEFT) < 0)
		{
			CameraPos.x -= App_CameraPositionSpeed;
		}
		if(GetAsyncKeyState(VK_RIGHT) < 0)
		{
			CameraPos.x += App_CameraPositionSpeed;
		}
		if(GetAsyncKeyState(vk_pg_up) < 0)
		{
			CameraPos.y += App_CameraPositionSpeed;
		}
		if(GetAsyncKeyState(vk_pg_down) < 0)
		{
			CameraPos.y -= App_CameraPositionSpeed;
		}

		if(!((CameraPos.x == 0.0)&&(CameraPos.y == 0.0)&&(CameraPos.z == 0.0)))
		{
			Identity(pIdentityMatrix);
			pIdentityMatrix[12] = CameraPos.x;
			pIdentityMatrix[13] = CameraPos.y;
			pIdentityMatrix[14] = CameraPos.z;

			InverseMatrix(pInverseMatrix,pRender_ViewMatrix);
			MatrixMul(pFinalMatrix,pIdentityMatrix,pInverseMatrix);
			InverseMatrix(pRender_ViewMatrix,pFinalMatrix);
		}

		if(GetAsyncKeyState(VK_CONTROL) < 0)
		{
			if(GetAsyncKeyState(VK_NUMPAD4) < 0)
			{
				RollAngle = -App_CameraRotationSpeed;
			}
			if(GetAsyncKeyState(VK_NUMPAD6) < 0)
			{
				RollAngle = App_CameraRotationSpeed;
			}
		}
		else
		{
			if(GetAsyncKeyState(VK_NUMPAD4) < 0)
			{
				YawAngle = -App_CameraRotationSpeed;
			}
			if(GetAsyncKeyState(VK_NUMPAD6) < 0)
			{
				YawAngle = App_CameraRotationSpeed;
			}
		}
		if(GetAsyncKeyState(VK_NUMPAD8) < 0)
		{
			PitchAngle = App_CameraRotationSpeed;
		}
		if(GetAsyncKeyState(VK_NUMPAD2) < 0)
		{
			PitchAngle = -App_CameraRotationSpeed;
		}

		if(YawAngle != 0.0)
		{
			Identity(pInverseView);
			InverseMatrix(pInverseView,pRender_ViewMatrix);
			CameraPos.x = pInverseView[12];
			CameraPos.y = pInverseView[13];
			CameraPos.z = pInverseView[14];
			AxisRotationMatrix(	pYawMatrix,
								pInverseView[4],
								pInverseView[5],
								pInverseView[6],
								YawAngle);
			MatrixMul(pMatrix1,pInverseView,pYawMatrix);
			pMatrix1[12] = CameraPos.x;
			pMatrix1[13] = CameraPos.y;
			pMatrix1[14] = CameraPos.z;
			InverseMatrix(pRender_ViewMatrix,pMatrix1);

		}

		if(PitchAngle != 0.0)
		{
			Identity(pInverseView);
			InverseMatrix(pInverseView,pRender_ViewMatrix);
			CameraPos.x = pInverseView[12];
			CameraPos.y = pInverseView[13];
			CameraPos.z = pInverseView[14];
			AxisRotationMatrix(	pPitchMatrix,
								pInverseView[0],//UT Value
								pInverseView[1],
								pInverseView[2],
								PitchAngle);
			MatrixMul(pMatrix1,pInverseView,pPitchMatrix);
			//MatrixMul(pMatrix1,pPitchMatrix,pInverseView);
			pMatrix1[12] = CameraPos.x;
			pMatrix1[13] = CameraPos.y;
			pMatrix1[14] = CameraPos.z;
			InverseMatrix(pRender_ViewMatrix,pMatrix1);
		}

		if(RollAngle != 0.0)
		{
			Identity(pInverseView);
			InverseMatrix(pInverseView,pRender_ViewMatrix);
			CameraPos.x = pInverseView[12];
			CameraPos.y = pInverseView[13];
			CameraPos.z = pInverseView[14];
			AxisRotationMatrix(	pRollMatrix,
								pInverseView[8],
								pInverseView[9],
								pInverseView[10],
								RollAngle);
			MatrixMul(pMatrix1,pInverseView,pRollMatrix);
			pMatrix1[12] = CameraPos.x;
			pMatrix1[13] = CameraPos.y;
			pMatrix1[14] = CameraPos.z;
			InverseMatrix(pRender_ViewMatrix,pMatrix1);
		}
	}
}


void App_handle_WM_KEYDOWN(WPARAM wParam, LPARAM lParam)
{
	int Key = (int) wParam;    // virtual-key code 
	long PrevKeyState = (lParam>>30)&1;

	////////////////////////////////
	////////////////////////////////
	////////////////////////////////
	//// ONLY ICON BAR KEYDOWNS ////
	////////////////////////////////
	////////////////////////////////
	////////////////////////////////

	if(	CurrentMenuMode==MENU_MODE_ICONBAR ||
		CurrentMenuMode==MENU_MODE_ICONBARMAINMENU)
	{//MENU
		if(Key==VK_ESC)
		{
			if(bCurrentlyEditingWeights)App_ExitEditWeightMode();
			if(CurrentMenuMode==MENU_MODE_ICONBARMAINMENU)
			{//colapse main menu.
				App_EnterMouseLookMode();
				CurrentMenuMode  = MENU_MODE_ICONBAR;
				CurrentMouseMode = MOUSE_MODE_OBJECT;
				CurrentKeyboardMode = KEYBOARD_MODE_CAMERA;
				return;
			}
			else if(App_PlayingMode != 0)
			{//stop time
		        App_PlayingMode = 0;
			}
			else if(App_CurrentTime != 0)
			{//stop time
				App_CurrentTime = 0;
			}
			else
			{//then quit
				PostQuitMessage(0);
				return;
			}
		}
		if(Key == VK_TAB)
		{
			if(bCurrentlyEditingWeights)App_ExitEditWeightMode();
			if(CurrentMenuMode==MENU_MODE_ICONBARMAINMENU)
			{//ENTER EDIT JOINT MODE WHEN LEAVING MAINMENU MODE
				App_EnterMouseLookMode();
				//SetCursorPos((int)(Render_WindowWidth*0.5),(int)(Render_WindowHeight*0.5));
				CurrentMenuMode  	= MENU_MODE_ICONBAR;
				CurrentMouseMode    = MOUSE_MODE_JOINT;
				CurrentKeyboardMode = KEYBOARD_MODE_CAMERA;

		        //App_PlayingMode = 0;
				//App_CurrentTime = 0;

				//MenuFunction_Skeleton_KeyFrame_SelectFirst();
				//App_CurrentTime = (pCurrentlySelectedKeyFrame) ? (pCurrentlySelectedKeyFrame->time) : (0);
				return;
			}
			else
			{//ENTER MAIN MENU MODE
				CurrentMenuMode     = MENU_MODE_ICONBARMAINMENU;
				CurrentMouseMode    = MOUSE_MODE_MENU;
				CurrentKeyboardMode = KEYBOARD_MODE_MENU;
				return;
			}
		}
	}//End if(CurrentMenuMode==MENU_MODE_APPLICATION)

	
	//pre load mouse sphere with current Obj, or mesh	
	//MSPRIME
	if(Key==VK_SHIFT || Key==VK_CONTROL)
	{
		if(PrevKeyState==0)
		{//first time user presses shift. we only want to do this once.
			if(	CurrentMouseMode==MOUSE_MODE_OBJECT &&
				pCurrentlySelectedObject && 
				pAppTools_PickSphere)
			{
				//memcpy(	pAppTools_PickSphere->pMatrix,
				//		pCurrentlySelectedObject->pMatrix,
				//		16*sizeof(float));
				App_UpdatePickSphere(pCurrentlySelectedObject);
			}
			if(	CurrentMouseMode==MOUSE_MODE_MESH &&
				pCurrentlySelectedObject && 
				pCurrentlySelectedMesh && 
				pAppTools_PickSphere)
			{
				//memcpy(	pAppTools_PickSphere->pMatrix,
				//		pCurrentlySelectedMesh->pMatrix,
				//		16*sizeof(float));
				App_UpdatePickSphere(pCurrentlySelectedObject,pCurrentlySelectedMesh);
			}
			if(	CurrentMouseMode==MOUSE_MODE_JOINT &&
				pCurrentlySelectedObject && 
				pCurrentlySelectedJoint && 
				pAppTools_PickSphere)
			{
				//memcpy(	pAppTools_PickSphere->pMatrix,
				//		pCurrentlySelectedMesh->pMatrix,
				//		16*sizeof(float));
				App_UpdatePickSphere(pCurrentlySelectedObject,pCurrentlySelectedJoint);
			}
		}
	}
	
	if(Key==VK_F1)
	{
		if(bCurrentlyEditingWeights)App_ExitEditWeightMode();
		App_EnterMouseLookMode();

		CurrentMenuMode     = MENU_MODE_ICONBAR;
		CurrentMouseMode    = MOUSE_MODE_OBJECT;
		CurrentKeyboardMode = KEYBOARD_MODE_CAMERA;

        App_PlayingMode = 0;
		App_CurrentTime = 0;
		return;
	}
	else if(Key==VK_F2)
	{
		if(bCurrentlyEditingWeights)App_ExitEditWeightMode();
		App_EnterMouseLookMode();
		CurrentMenuMode     = MENU_MODE_ICONBAR;
		CurrentMouseMode    = MOUSE_MODE_MESH;
		CurrentKeyboardMode = KEYBOARD_MODE_CAMERA;

        App_PlayingMode = 0;
		App_CurrentTime = 0;
		return;
	}
	else if(Key==VK_F3)
	{
		if(bCurrentlyEditingWeights)App_ExitEditWeightMode();
		App_EnterMouseLookMode();
		CurrentMenuMode     = MENU_MODE_ICONBAR;
		CurrentMouseMode    = MOUSE_MODE_JOINT;
		CurrentKeyboardMode = KEYBOARD_MODE_CAMERA;

        App_PlayingMode = 0;
		App_CurrentTime = 0;

		MenuFunction_Skeleton_KeyFrame_SelectFirst();
		App_CurrentTime = (pCurrentlySelectedKeyFrame) ? (pCurrentlySelectedKeyFrame->time) : (0);

		return;
	}
	else if(Key==VK_F4)
	{
		if(bCurrentlyEditingWeights==false)
		{//Init Edit Weight Mode
			App_EnterEditWeightMode();
		}

		App_EnterMouseLookMode();
		CurrentMenuMode     = MENU_MODE_ICONBAR;
		CurrentMouseMode    = MOUSE_MODE_WEIGHT;
		CurrentKeyboardMode = KEYBOARD_MODE_CAMERA;
		//App_CurrentTime  = KEYFRAME_RESTPOSE;
        App_PlayingMode  = 0;
		return;
	}
	else if(Key==VK_F5)
	{
		if(bCurrentlyEditingWeights)App_ExitEditWeightMode();

		App_EnterMouseLookMode();
		CurrentMenuMode     = MENU_MODE_ICONBAR;
		CurrentMouseMode    = MOUSE_MODE_VERT;
		CurrentKeyboardMode = KEYBOARD_MODE_CAMERA;
		App_CurrentTime  = KEYFRAME_RESTPOSE;
        App_PlayingMode  = 0;
		return;
	}
	else if(Key==VK_F6)
	{
		if(bCurrentlyEditingWeights)App_ExitEditWeightMode();

		App_EnterMouseLookMode();
		CurrentMenuMode     = MENU_MODE_ICONBAR;
		CurrentMouseMode    = MOUSE_MODE_TRI;
		CurrentKeyboardMode = KEYBOARD_MODE_CAMERA;
		return;
	}
	else if(Key==VK_F7)//select next
	{
		if(CurrentMouseMode==MOUSE_MODE_OBJECT)
		{
		 	MenuFunction_Object_SelectNext();
		}
		if(CurrentMouseMode==MOUSE_MODE_MESH)
		{
		 	MenuFunction_Mesh_SelectNext();
		}
		if(CurrentMouseMode==MOUSE_MODE_JOINT)
		{
			MenuFunction_Skeleton_Joint_SelectNext();
		}
		if(CurrentMouseMode==MOUSE_MODE_WEIGHT)
		{
			MenuFunction_Skeleton_Joint_SelectNext();
		}
		if(CurrentMouseMode==MOUSE_MODE_VERT)
		{
		 	MenuFunction_Mesh_SelectNext();
		}
		return;
	}
	else if(Key==VK_F12)//select next
	{
		char pTempText[256];
		sprintf(pTempText,"obj-%s",pCurrentlySelectedObject->pName);
		OutputTextToConsole(pTempText,0xffffffff);
		sprintf(pTempText,"ske-%s",pCurrentlySelectedSkeleton->pName);
		OutputTextToConsole(pTempText,0xffffffff);
		sprintf(pTempText,"tim-%d",pCurrentlySelectedKeyFrame->time);
		OutputTextToConsole(pTempText,0xffffffff);
		sprintf(pTempText,"jon-%s",pCurrentlySelectedJoint->pInfoItem->pName);
		OutputTextToConsole(pTempText,0xffffffff);
	}

	if(	CurrentMouseMode==MOUSE_MODE_JOINT &&
		pCurrentlySelectedObject &&
		pCurrentlySelectedObject->pSkeletonHead)
	{
		if(Key==VK_1)
		{
            App_PlayingMode = 0;
			MenuFunction_Skeleton_KeyFrame_SelectFirst();
			App_CurrentTime = (pCurrentlySelectedKeyFrame) ? pCurrentlySelectedKeyFrame->time : 0;
			//App_CurrentTime = KEYFRAME_MINFRAME;
		}
		else if(Key==VK_2)
		{
			MenuFunction_Skeleton_KeyFrame_SelectPrev();
			App_CurrentTime = (pCurrentlySelectedKeyFrame) ? pCurrentlySelectedKeyFrame->time : 0;
		}
		else if(Key==VK_3)
		{
			//Call timer to init.
			GetElapsedMilliSeconds();
            App_PlayingMode = -1;
		}
		else if(Key==VK_4)
		{
			if(App_PlayingMode == 0)
			{//hitting stop twice sets counter to 0
				App_CurrentTime = 0;
			}
            App_PlayingMode = 0;
		}
		else if(Key==VK_5)
		{
			//Call timer to init.
			GetElapsedMilliSeconds();
            App_PlayingMode = 1;

			//// LOOPANIMATION
			if(pCurrentlySelectedSkeleton)
				pCurrentlySelectedSkeleton->starttime = App_CurrentTime;
		}
		else if(Key==VK_6)//next keyframe
		{
			MenuFunction_Skeleton_KeyFrame_SelectNext();
			App_CurrentTime = (pCurrentlySelectedKeyFrame) ? pCurrentlySelectedKeyFrame->time : 0;
			//HACKHACK
			//App_CurrentTime += 100;
		}
		else if(Key==VK_7)
		{
            App_PlayingMode = 0;
			MenuFunction_Skeleton_KeyFrame_SelectLast();
			App_CurrentTime = (pCurrentlySelectedKeyFrame) ? pCurrentlySelectedKeyFrame->time : 0;
			//App_CurrentTime = KEYFRAME_MAXFRAME;
		}
		else if(Key==VK_8)
		{
            App_PlayingMode = 0;
			MenuFunction_Skeleton_KeyFrame_SelectRestPose();
			App_CurrentTime = (pCurrentlySelectedKeyFrame) ? pCurrentlySelectedKeyFrame->time : 0;
		}
	}


	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////
	//// ONLY MAIN MENU KEYDOWNS ////
	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////

	//check for menu also??? BUGBUG
	if(CurrentMenuMode==MENU_MODE_ICONBARMAINMENU)
	{
		if(pSelectedField)
		{//FieldLL handles keydown here
			if(Key==VK_ESC)
			{
				pSelectedField = NULL;
				//Set LastSelectedPoint to an absurd number so that no field can be selected using the last point.
				LastSelectedPoint.x = MAX_LONG;
				LastSelectedPoint.y = MAX_LONG;
			}
			else
			{
				FieldLL_WM_KEYDOWN(pSelectedField,wParam,lParam);
			}
		}
		else
		{//render handles keydown here			
			if(Key==VK_ESC || Key == VK_TAB)
			{//ENTER EDIT OBJECT MODE TO LEAVE MAINMENU MODE
				App_EnterMouseLookMode();
				//SetCursorPos((int)(Render_WindowWidth*0.5),(int)(Render_WindowHeight*0.5));
				CurrentMenuMode  = MENU_MODE_ICONBAR;
				CurrentMouseMode = MOUSE_MODE_OBJECT;
				CurrentKeyboardMode = KEYBOARD_MODE_CAMERA;
				return;
			}
			if(Key == VK_DOWN)
			{
				TempHearchyY -= IconWidth;
				LastSelectedPoint.y -= IconWidth;
			}
			if(Key == VK_UP)
			{
				if(TempHearchyY<0.0)
				{
					TempHearchyY += IconWidth;
					LastSelectedPoint.y += IconWidth;
				}
			}
			if(Key == VK_RIGHT)
			{
				TempHearchyX -= IconWidth;
				LastSelectedPoint.x -= IconWidth;
			}
			if(Key == VK_LEFT)
			{
				if(TempHearchyX<0.0)
				{
					TempHearchyX += IconWidth;
					LastSelectedPoint.x += IconWidth;
				}
			}
			if(Key == vk_pg_down)
			{
				TempHearchyY -= IconWidth*((float)((int)(Render_WindowHeight/IconWidth)));
				LastSelectedPoint.y -= IconWidth*((float)((int)(Render_WindowHeight/IconWidth)));
			}
			if(Key == vk_pg_up)
			{
				if(TempHearchyY<0.0)
				{
					TempHearchyY += IconWidth*((float)((int)(Render_WindowHeight/IconWidth)));
					LastSelectedPoint.y += IconWidth*((float)((int)(Render_WindowHeight/IconWidth)));
					if(TempHearchyY>0.0)
					{
						LastSelectedPoint.y -= TempHearchyY;
						TempHearchyY = 0.0;
					}
				}
			}
			if(Key == VK_HOME)
			{
			 	TempHearchyX = 0.0;
			 	TempHearchyY = 0.0;
			}
		}
	}//End if(CurrentMenuMode==MENU_MODE_APPLICATION)
}









void App_handle_WM_LBUTTONDOWN(HWND hwnd,WPARAM wParam,LPARAM lParam)
{
	struct SceneLL* pScene = NULL;
	struct ObjectLL* pObject = NULL;
	struct MeshLL* pMesh = NULL;
	KeyFrameLL* pCurr_KeyFrame = NULL;
	KeyFrameLL* pPrev_KeyFrame = NULL;
	long xPos = LOWORD(lParam);
	long yPos = HIWORD(lParam);

	if(	CurrentMenuMode==MENU_MODE_ICONBAR ||
		CurrentMenuMode==MENU_MODE_ICONBARMAINMENU)
	{
//		Pick();
		UpdateMainMenu(true);
		//UpdateSceneHiearchyDebug();
		if(pSelectedField)
		{
		}
		else if(pCurrentlySelectedFlags)
		{//flip flags value
			if(bCurrentlySelectedClear)
			{//clear contents because there can only be one selection for SrcBlend type data
				*pCurrentlySelectedFlags = 0;
			}
			SwapBit(*pCurrentlySelectedFlags,iCurrentlySelectedFlag);

			pCurrentlySelectedFlags = NULL;//we have fliped the bit, so clear pointer.
		}
	}

	//(Use Actor Matrix) must be enabled to edit this matrix
	if(CurrentMouseMode == MOUSE_MODE_OBJECT)
	{
		if(	pCurrentlySelectedObject &&
			App_PlayingMode == 0)
		{
			if(GetAsyncKeyState(VK_SHIFT) < 0)
			{
				bCurrentlyRotatingObject = false;
				if(PickThisMatrix(pCurrentlySelectedObject->pMatrix))//is mouse actually over control sphere?
				{
					bCurrentlyRotatingObject = true;
					memcpy(	pAppTools_PickSphere->pMatrix,
							pCurrentlySelectedObject->pMatrix,
							16*sizeof(float));
			
					//reset prev to avoid snap. 
					PreviousMouseSpherePickVector.x = CurrentMouseSpherePickVector.x;
					PreviousMouseSpherePickVector.y = CurrentMouseSpherePickVector.y;
					PreviousMouseSpherePickVector.z = CurrentMouseSpherePickVector.z;
				}
			}
			if(GetAsyncKeyState(VK_CONTROL) < 0)
			{
				bCurrentlyMovingObject = false;
				if(PickThisMatrix(pCurrentlySelectedObject->pMatrix))//is mouse actually over control sphere?
				{
					bCurrentlyMovingObject = true;
					memcpy(	pAppTools_PickSphere->pMatrix,
							pCurrentlySelectedObject->pMatrix,
							16*sizeof(float));

					//reset prev to avoid snap. 
					PreviousMouseSpherePickVector.x = CurrentMouseSpherePickVector.x;
					PreviousMouseSpherePickVector.y = CurrentMouseSpherePickVector.y;
					PreviousMouseSpherePickVector.z = CurrentMouseSpherePickVector.z;
				}
			}
		}
	}

	if(CurrentMouseMode == MOUSE_MODE_MESH)
	{
		if( pCurrentlySelectedObject &&
			pCurrentlySelectedMesh &&
			(!(pCurrentlySelectedMesh->RenderFlags & RENDER_TRANSFORMED)) &&
			App_PlayingMode == 0)
		{//cant edit matrix if TRANSFORMED. skeleton deforms mesh in this mode.
			if(GetAsyncKeyState(VK_SHIFT) < 0)
			{
				bCurrentlyRotatingMesh = false;

				//substitute MeshAbsoluteMatrix.
				//// Search for ABSOLUTEMESHMATRIXCALLOUT
				//// PickThisMatrix() actually modifies the matrix you give it.
				//// Sometimes a Mesh is also transformed by the parent Objects->pMatrix
				//// thus 2 matrices are in the mix, and only one needs rotating.
				//// the global pApp_AbsoluteMeshMatrix matrix will hold pMsh->pMat*pObj->pMat
				//// pMesh->pMat will be rstores in the Render_handle_WM_MOUSEMOVE function

				MatrixMul(	pApp_AbsoluteMeshMatrix,
							pCurrentlySelectedMesh->pMatrix,
							pCurrentlySelectedObject->pMatrix);

				if(PickThisMatrix(pApp_AbsoluteMeshMatrix))//is mouse actually over control sphere?
				{
					bCurrentlyRotatingMesh = true;
					memcpy(	pAppTools_PickSphere->pMatrix,
							pApp_AbsoluteMeshMatrix,
							16*sizeof(float));
					//reset prev to avoid snap. 
					PreviousMouseSpherePickVector.x = CurrentMouseSpherePickVector.x;
					PreviousMouseSpherePickVector.y = CurrentMouseSpherePickVector.y;
					PreviousMouseSpherePickVector.z = CurrentMouseSpherePickVector.z;
				}							
			}
			if(GetAsyncKeyState(VK_CONTROL) < 0)
			{
				bCurrentlyMovingMesh = false;
				//substitute MeshAbsoluteMatrix.
				//substitute MeshAbsoluteMatrix.
				//// Search for ABSOLUTEMESHMATRIXCALLOUT
				//// PickThisMatrix() actually modifies the matrix you give it.
				//// Sometimes a Mesh is also transformed by the parent Objects->pMatrix
				//// thus 2 matrices are in the mix, and only one needs rotating.
				//// the global pApp_AbsoluteMeshMatrix matrix will hold pMsh->pMat*pObj->pMat
				//// pMesh->pMat will be rstores in the Render_handle_WM_MOUSEMOVE function

				MatrixMul(	pApp_AbsoluteMeshMatrix,
							pCurrentlySelectedMesh->pMatrix,
							pCurrentlySelectedObject->pMatrix);
				
				if(PickThisMatrix(pApp_AbsoluteMeshMatrix))//is mouse actually over control sphere?
				{
					bCurrentlyMovingMesh = true;
					memcpy(	pAppTools_PickSphere->pMatrix,
							pApp_AbsoluteMeshMatrix,
							16*sizeof(float));
					//reset prev to avoid snap. 
					PreviousMouseSpherePickVector.x = CurrentMouseSpherePickVector.x;
					PreviousMouseSpherePickVector.y = CurrentMouseSpherePickVector.y;
					PreviousMouseSpherePickVector.z = CurrentMouseSpherePickVector.z;
				}
			}
		}
	}


	if(CurrentMouseMode == MOUSE_MODE_JOINT)
	{
		if(	pCurrentlySelectedObject &&
			pCurrentlySelectedObject->pSkeletonHead &&
			pCurrentlySelectedKeyFrame &&
			pCurrentlySelectedKeyFrame->time == App_CurrentTime &&
			pCurrentlySelectedJoint &&
			App_PlayingMode == 0)
		{
			if(GetAsyncKeyState(VK_SHIFT) < 0)
			{
				bCurrentlyRotatingJoint = false;

				//substitute MeshAbsoluteMatrix.
				//// Search for ABSOLUTEMESHMATRIXCALLOUT
				//// PickThisMatrix() actually modifies the matrix you give it.
				//// Sometimes a Mesh is also transformed by the parent Objects->pMatrix
				//// thus 2 matrices are in the mix, and only one needs rotating.
				//// the global pApp_AbsoluteMeshMatrix matrix will hold pMsh->pMat*pObj->pMat
				//// pMesh->pMat will be rstores in the Render_handle_WM_MOUSEMOVE function

				MatrixMul(	pApp_AbsoluteMeshMatrix,
							pCurrentlySelectedJoint->pAbsoluteMatrix,
							pCurrentlySelectedObject->pMatrix);
				
				if(PickThisMatrix(pApp_AbsoluteMeshMatrix))//is mouse actually over control sphere?
				//if(PickThisMatrix(pCurrentlySelectedJoint->pAbsoluteMatrix))//is mouse actually over control sphere?
				//if(PickThisMatrix(pCurrentlySelectedJoint->pRelativeMatrix))//is mouse actually over control sphere?
				{
					bCurrentlyRotatingJoint = true;
					//reset prev to avoid snap. 

					memcpy(	pAppTools_PickSphere->pMatrix,
							pApp_AbsoluteMeshMatrix,
							//pCurrentlySelectedJoint->pAbsoluteMatrix,
							16*sizeof(float));

					PreviousMouseSpherePickVector.x = CurrentMouseSpherePickVector.x;
					PreviousMouseSpherePickVector.y = CurrentMouseSpherePickVector.y;
					PreviousMouseSpherePickVector.z = CurrentMouseSpherePickVector.z;
				}
			}
			if(GetAsyncKeyState(VK_CONTROL) < 0)
			{
				bCurrentlyMovingJoint = false;


				//substitute MeshAbsoluteMatrix.
				//// Search for ABSOLUTEMESHMATRIXCALLOUT
				//// PickThisMatrix() actually modifies the matrix you give it.
				//// Sometimes a Mesh is also transformed by the parent Objects->pMatrix
				//// thus 2 matrices are in the mix, and only one needs rotating.
				//// the global pApp_AbsoluteMeshMatrix matrix will hold pMsh->pMat*pObj->pMat
				//// pMesh->pMat will be rstores in the Render_handle_WM_MOUSEMOVE function

				MatrixMul(	pApp_AbsoluteMeshMatrix,
							pCurrentlySelectedJoint->pAbsoluteMatrix,
							pCurrentlySelectedObject->pMatrix);

				if(PickThisMatrix(pApp_AbsoluteMeshMatrix))//is mouse actually over control sphere?
				{
					bCurrentlyMovingJoint = true;
				
					memcpy(	pAppTools_PickSphere->pMatrix,
							pApp_AbsoluteMeshMatrix,
							//pCurrentlySelectedJoint->pAbsoluteMatrix,
							16*sizeof(float));

					//reset prev to avoid snap. 
					PreviousMouseSpherePickVector.x = CurrentMouseSpherePickVector.x;
					PreviousMouseSpherePickVector.y = CurrentMouseSpherePickVector.y;
					PreviousMouseSpherePickVector.z = CurrentMouseSpherePickVector.z;
				}
			}
		}
	}
	if(CurrentMouseMode == MOUSE_MODE_WEIGHT)
	{
		if(	pCurrentlySelectedObject && 
			pCurrentlySelectedObject->pSkeletonHead && 
			pCurrentlySelectedMesh)
		{
			if(GetAsyncKeyState(VK_SHIFT) < 0)
			{//user is selecting vertices
			    GetCursorPos(&ptApp_SelectRectStart);
			    ScreenToClient(Render_hWnd,&ptApp_SelectRectStart);
			    
			    GetCursorPos(&ptApp_SelectRectStop);
			    ScreenToClient(Render_hWnd,&ptApp_SelectRectStop);

				//FlagVerticesAsSelected(pCurrentlySelectedObject,pCurrentlySelectedMesh,pSystem_SelectedVertices,ptApp_SelectRectStart,ptApp_SelectRectStop);
				Object_FlagVerticesAsSelected(pCurrentlySelectedObject,ptApp_SelectRectStart,ptApp_SelectRectStop);
			}
		}
	}

	if(CurrentMouseMode == MOUSE_MODE_VERT)
	{
		if(	pCurrentlySelectedObject && 
			pCurrentlySelectedMesh)
		{
			if(GetAsyncKeyState(VK_SHIFT) < 0)
			{//user is selecting vertices
			    GetCursorPos(&ptApp_SelectRectStart);
			    ScreenToClient(Render_hWnd,&ptApp_SelectRectStart);
			    
			    GetCursorPos(&ptApp_SelectRectStop);
			    ScreenToClient(Render_hWnd,&ptApp_SelectRectStop);

				//FlagVerticesAsSelected(pCurrentlySelectedObject,pCurrentlySelectedMesh,pApp_SelectedVertices,ptApp_SelectRectStart,ptApp_SelectRectStop);
				Object_FlagVerticesAsSelected(pCurrentlySelectedObject,ptApp_SelectRectStart,ptApp_SelectRectStop);
			}
			if(GetAsyncKeyState(VK_CONTROL) < 0)
			{//user is moving vertices
			   	bCurrentlyMovingVerts = false;
				if(1)//inbounds)
				{
					MouseMoveVertLastMousePos.x = xPos;
					MouseMoveVertLastMousePos.y = yPos;
					bCurrentlyMovingVerts = true;
				}
			}	
		}
	}
}


void App_handle_WM_RBUTTONDOWN()
{
	//EditBoneWeights : Subtract Selected Mode 
	if(CurrentMouseMode == MOUSE_MODE_WEIGHT)
	{
		if(	pCurrentlySelectedObject && 
			pCurrentlySelectedObject->pSkeletonHead && 
			pCurrentlySelectedMesh)
		{
			if(GetAsyncKeyState(VK_SHIFT) < 0)
			{//user is selecting vertices
			    GetCursorPos(&ptApp_SelectRectStart);
			    ScreenToClient(Render_hWnd,&ptApp_SelectRectStart);
			    
			    GetCursorPos(&ptApp_SelectRectStop);
			    ScreenToClient(Render_hWnd,&ptApp_SelectRectStop);

				//FlagVerticesAsSelected(pCurrentlySelectedObject,pCurrentlySelectedMesh,pApp_SelectedVertices,ptApp_SelectRectStart,ptApp_SelectRectStop);
				Object_FlagVerticesAsSelected(pCurrentlySelectedObject,ptApp_SelectRectStart,ptApp_SelectRectStop);
			}
		}
	}
}

void App_handle_WM_MOUSEMOVE(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

	bLMouseButtonDown = (bool)(wParam & MK_LBUTTON);
	bMMouseButtonDown = (bool)(wParam & MK_MBUTTON);
	bRMouseButtonDown = (bool)(wParam & MK_RBUTTON);
	bCtrlKeyDown      = (bool)(wParam & MK_CONTROL);
	bShiftKeyDown     = (bool)(wParam & MK_SHIFT);
	long xPos = LOWORD(lParam);
	long yPos = HIWORD(lParam);

	KeyFrameLL* pCurr_KeyFrame = NULL;
	KeyFrameLL* pPrev_KeyFrame = NULL;


	//for MainMenu code in FieldLL.cpp
    GetCursorPos(&CurrentMousePosition);
    ScreenToClient(Render_hWnd,&CurrentMousePosition);


	if(	CurrentMenuMode==MENU_MODE_ICONBAR ||
		CurrentMenuMode==MENU_MODE_ICONBARMAINMENU)
	{
		if(bCtrlKeyDown)//control camera
		{
		}
	}

	//////////////////////////////////
	if(CurrentMouseMode == MOUSE_MODE_OBJECT)
	{
		if( pCurrentlySelectedObject &&
			App_PlayingMode == 0)
		{
			if(bCurrentlyRotatingObject && bShiftKeyDown)
			{
				if(bLMouseButtonDown)
				{
					Mouse_Rotate_CurrentMatrix(pCurrentlySelectedObject->pMatrix);
				}
			}
			if(bCurrentlyMovingObject &&bCtrlKeyDown)
			{
				if(bLMouseButtonDown)
				{
					Mouse_Move_CurrentMatrix(pCurrentlySelectedObject->pMatrix);
				}
			}
		}
	}
	else if(CurrentMouseMode == MOUSE_MODE_MESH)
	{
		if( pCurrentlySelectedObject &&
			pCurrentlySelectedMesh &&
			(!(pCurrentlySelectedMesh->RenderFlags & RENDER_TRANSFORMED)) &&
			App_PlayingMode == 0)
		{
			if(bCurrentlyRotatingMesh && bShiftKeyDown)
			{
				//App_UpdatePickSphere(pCurrentlySelectedMesh);
				if(bLMouseButtonDown)
				{
					Mouse_Rotate_CurrentMatrix(pApp_AbsoluteMeshMatrix);

					//// Restore pMesh Matrix. Search for ABSOLUTEMESHMATRIXCALLOUT
					float pInverseActorMatrix[16];
					InverseMatrix(pInverseActorMatrix,pCurrentlySelectedObject->pMatrix);
					
					MatrixMul(	pCurrentlySelectedMesh->pMatrix,
								pApp_AbsoluteMeshMatrix,
								pInverseActorMatrix);
				}
			}
			if(bCurrentlyMovingMesh && bCtrlKeyDown)
			{
				if(bLMouseButtonDown)
				{
					Mouse_Move_CurrentMatrix(pApp_AbsoluteMeshMatrix);

					//// Restore pMesh Matrix. Search for ABSOLUTEMESHMATRIXCALLOUT
					float pInverseActorMatrix[16];
					InverseMatrix(pInverseActorMatrix,pCurrentlySelectedObject->pMatrix);
					
					MatrixMul(	pCurrentlySelectedMesh->pMatrix,
								pApp_AbsoluteMeshMatrix,
								pInverseActorMatrix);
				}
			}
		}
	}
	else if(CurrentMouseMode == MOUSE_MODE_JOINT)
	{
		if(	pCurrentlySelectedObject && 
			pCurrentlySelectedObject->pSkeletonHead && 
			pCurrentlySelectedJoint && 
			App_PlayingMode == 0)
		{
			if(bLMouseButtonDown)
			{
				if(bShiftKeyDown && bCurrentlyRotatingJoint)
				{
					Mouse_Rotate_CurrentMatrix(pApp_AbsoluteMeshMatrix);
				}
				if(bCtrlKeyDown && bCurrentlyMovingJoint)
				{
					Mouse_Move_CurrentMatrix(pApp_AbsoluteMeshMatrix);
				}

				//// Restore pMesh Matrix. Search for ABSOLUTEMESHMATRIXCALLOUT
				float pInverseActorMatrix[16];
				InverseMatrix(pInverseActorMatrix,pCurrentlySelectedObject->pMatrix);
				
				MatrixMul(	pCurrentlySelectedJoint->pAbsoluteMatrix,
							pApp_AbsoluteMeshMatrix,
							pInverseActorMatrix);



				if(bShiftKeyDown || bCtrlKeyDown)
				{
					//// Mouse has moved, absolute joint has changed. Update relative matrices 
					//Joint_TransformJoint_AbsoluteToRelative(pCurrentlySelectedJoint,pCurrentlySelectedObject->pMatrix);
					//KeyFrame_TransformJoints_RelativeToAbsolute(pCurrentlySelectedKeyFrame,pCurrentlySelectedObject->pMatrix);

					Joint_TransformJoint_AbsoluteToRelative(pCurrentlySelectedJoint,pRender_IdentityMatrix);
					KeyFrame_TransformJoints_RelativeToAbsolute(pCurrentlySelectedKeyFrame,pRender_IdentityMatrix);




				}
			}
		}
	}
	else if(CurrentMouseMode == MOUSE_MODE_WEIGHT)
	{
		if(	pCurrentlySelectedObject && 
			pCurrentlySelectedObject->pSkeletonHead && 
			(bLMouseButtonDown || bRMouseButtonDown))
		{
			if(	bShiftKeyDown)
			{//user is selecting
			    GetCursorPos(&ptApp_SelectRectStop);
			    ScreenToClient(Render_hWnd,&ptApp_SelectRectStop);
				//FlagVerticesAsSelected(pCurrentlySelectedObject,pCurrentlySelectedMesh,pApp_SelectedVertices,ptApp_SelectRectStart,ptApp_SelectRectStop);
				Object_FlagVerticesAsSelected(pCurrentlySelectedObject,ptApp_SelectRectStart,ptApp_SelectRectStop);
			}
		}
	}
	else if(CurrentMouseMode == MOUSE_MODE_VERT)
	{
		if(	pCurrentlySelectedObject && 
			pCurrentlySelectedMesh &&
			bLMouseButtonDown)
		{
			if(	bShiftKeyDown)
			{//user is selecting
			    GetCursorPos(&ptApp_SelectRectStop);
			    ScreenToClient(Render_hWnd,&ptApp_SelectRectStop);
				//FlagVerticesAsSelected(pCurrentlySelectedObject,pCurrentlySelectedMesh,pApp_SelectedVertices,ptApp_SelectRectStart,ptApp_SelectRectStop);
				Object_FlagVerticesAsSelected(pCurrentlySelectedObject,ptApp_SelectRectStart,ptApp_SelectRectStop);
			}
			else if(	bCtrlKeyDown &&
						bCurrentlyMovingVerts)
			{//user is moving verts
				POINT MouseDiff;
				MouseDiff.x = (xPos-MouseMoveVertLastMousePos.x);
				MouseDiff.y = (MouseMoveVertLastMousePos.y-yPos);
				Object_Mouse_Move_SelectedVertices(pCurrentlySelectedObject,MouseDiff);
				MouseMoveVertLastMousePos.x = xPos;
				MouseMoveVertLastMousePos.y = yPos;
			}
		}
	}
}

void App_PaintWeights()
{
	PERVERTEX_JOINT_DATA* pPVBData = NULL;
	POINT ptCursor;
	struct SceneLL*  pScene  = NULL;
	struct ObjectLL* pObject = NULL;
	struct MeshLL* 	 pMesh   = NULL;
	VERTEX_XYZNRGBUV* pLocalVerts;
	VERTEX_XYZNRGBUV* pWorldVerts;
	unsigned char*	  pVertexFlags = NULL;//selected, visible
	WORD* pIndices;
	aVECTOR Point;
	aVECTOR Pos;
	aVECTOR Dir;
	float Length;
	float MinDistance;
	float Distance;
	long i;
	long JointIndex0;
	long JointIndex1;
	bool bIntersection = false;
	unsigned long CurrJointID;

	if(pCurrentlySelectedObject==NULL)return;
	if(pCurrentlySelectedJoint==NULL)return;


	if(	GetAsyncKeyState(VK_SHIFT) < 0 ||
		GetAsyncKeyState(VK_CONTROL) < 0)
	{
		CurrJointID = pCurrentlySelectedJoint->UniqueID;
		pMesh = pCurrentlySelectedObject->pMeshHead;
		while(pMesh)	 
		{
			pLocalVerts  = pMesh->pLocalVerts;
			pWorldVerts  = pMesh->pWorldVerts;
			pVertexFlags = pMesh->pVertexFlags;
			pIndices     = pMesh->pIndices;
			pPVBData     = pMesh->pPerVertexJointData;

			//get mouse position
		    GetCursorPos(&ptCursor);
		    ScreenToClient(Render_hWnd,&ptCursor);

			//create 3d pick ray
			CreatePickRay(pCurrentlySelectedObject,pMesh,ptCursor,Pos,Dir);
			MacroNormalize(Length,Dir.x,Dir.y,Dir.z);

			if(pMesh->RenderFlags & RENDER_VISIBLE)
			{

				MinDistance = 10000.0;
				for(i=0;i<pMesh->num_verts;i++)
				{
					Point.x = pLocalVerts[i].x;
					Point.y = pLocalVerts[i].y;
					Point.z = pLocalVerts[i].z;
					Distance = Distance_PointToVector(Pos,Dir,Point);


					if(Distance<MinDistance)
					{
						MinDistance = Distance;
						//sprintf(pGlobalConsoleText2,"MinDistance[   ] : %f",MinDistance);
					}

					if(Distance<App_SprayRadius || pVertexFlags[i] & VERTEX_SELECTED)
					{
															   
						JointIndex0 = pPVBData[i].index[0];
						JointIndex1 = pPVBData[i].index[1];

						////////////////////////
						//// Spray Addition ////
						////////////////////////
						if(GetAsyncKeyState(VK_LBUTTON) < 0)
						{//[+] Spray Add
							if(JointIndex0 == CurrJointID)
							{//CurrJointID is already added to this vert in j0. increment j0 weight.
								if(JointIndex1 == -1)
								{//this is only joint attached to vert. set j0 weight to 1.0
									pPVBData[i].weight[0] = 1.0;
								}
								else
								{//j0, and j1 exist. increment j0 weight.
									pPVBData[i].weight[0] += 0.02;
								}								
							}
							else if(JointIndex1 == CurrJointID)
							{//CurrJointID is already added to this vert in j1. increment j1 weight.
								if(JointIndex0 == -1)
								{//this is only joint attached to vert. set j1 weight to 1.0
									pPVBData[i].weight[1] = 1.0;
								}
								else
								{//j0, and j2 exist. increment j1 weight.
									pPVBData[i].weight[1] += 0.02;
								}								
							}
							else if(JointIndex0 == -1)
							{//CurrJointID needs to be added to this vert.  slot j0 is available
								pPVBData[i].index[0]  = CurrJointID;
								pPVBData[i].weight[0] = 0.02;
							}
							else if(JointIndex1 == -1)
							{//CurrJointID needs to be added to this vert.  slot j1 is available
								pPVBData[i].index[1]  = CurrJointID;
								pPVBData[i].weight[1] = 0.02;
							}
							else
							{//CurrJointID needs to be added to this vert.  no slot available. override weakest weight
								if(pPVBData[i].weight[0] < pPVBData[i].weight[1])
								{//j0 is weakest, so overwrite with CurrJointID
									pPVBData[i].index[0]  = CurrJointID;
									pPVBData[i].weight[0] = 0.02;
								}
								else
								{//j1 is weakest, so overwrite with CurrJointID
									pPVBData[i].index[1]  = CurrJointID;
									pPVBData[i].weight[1] = 0.02;
								}
							}
						}
						////////////////////////
						//// Spray Subtract ////
						////////////////////////
						else if(GetAsyncKeyState(VK_RBUTTON) < 0)
						{//[+] Spray Subtract
							if(JointIndex0 == CurrJointID)
							{//CurrJointID is already added to this vert in j0. decrease, or remove j0 weight.
								if(JointIndex1 == -1)
								{//this is only joint attached to vert. weights are all(1.0) or nothing, so remove. set j0 -1, and weight to 0.0
									pPVBData[i].index[0]  =  -1;
									pPVBData[i].weight[0] = 0.0;
								}
								else
								{//j0, and j1 exist. decrease j0 weight.
									pPVBData[i].weight[0] -= 0.02;
									if(pPVBData[i].weight[0]<=0.0)
									{//oh well, we erased it
										pPVBData[i].index[0]  =  -1;
										pPVBData[i].weight[0] = 0.0;
									}
								}								
							}
							else if(JointIndex1 == CurrJointID)
							{//CurrJointID is already added to this vert in j1. increment j1 weight.
								if(JointIndex0 = -1)
								{//this is only joint attached to vert. weights are all(1.0) or nothing, so remove. set j0 -1, and weight to 0.0
									pPVBData[i].index[1]  =  -1;
									pPVBData[i].weight[1] = 0.0;
								}
								else
								{//j0, and j1 exist. decrease j0 weight.
									pPVBData[i].weight[1] -= 0.02;
									if(pPVBData[i].weight[1]<=0.0)
									{//oh well, we erased it
										pPVBData[i].index[1]  =  -1;
										pPVBData[i].weight[1] = 0.0;
									}
								}								
							}
						}

						MacroNormalizeWeights(Length,
										pPVBData[i].weight[0],
										pPVBData[i].weight[1],
										pPVBData[i].weight[2],
										pPVBData[i].weight[3]);

					}
				}//End for i<num_verts
			}//End if not NOPICK
			pMesh = pMesh->pNext;
		}//End while(pMesh)
	}//end if shift, or ctrl down
}















void App_CalculateBoneMatrix(float* pMatrix,aVECTOR Pos,aVECTOR LookAtPos)
{
	aVECTOR Up,Offset;
	float Length;
	float pLookAtMatrix[16];
	float pScaleMat[16];

	Offset.x = LookAtPos.x-Pos.x;
	Offset.y = LookAtPos.y-Pos.y;
	Offset.z = LookAtPos.z-Pos.z;
	MacroMagnitude(Length,Offset.x,Offset.y,Offset.z);
	Scale(pScaleMat,(Length/10.0));//save length to scale vector


	pScaleMat[0 ] = App_BoneScale;
	pScaleMat[5 ] = App_BoneScale;
	pScaleMat[10] = (Length/10.0);

	//finish normalization
	Offset.x /= Length;
	Offset.y /= Length;
	Offset.z /= Length;

	Up.x = 0;
	Up.y = 1;
	Up.z = 0;
	//if offset is parallel to up, choose new up.
	MacroDot(Length,Offset.x,Offset.y,Offset.z,Up.x,Up.y,Up.z);
	if(Length<-0.5 || Length>0.5)
	{
		Up.x = 1;
		Up.y = 0;
		Up.z = 0;
	}

	CreateLookAtMatrix(Pos,Up,LookAtPos,pLookAtMatrix);
	MatrixMul(	pAppTools_Vector->pMatrix,
				pScaleMat,
				pLookAtMatrix);

}





void App_EnterMouseLookMode()
{
	SetCursorPos((int)(Render_WindowWidth*0.5),(int)(Render_WindowHeight*0.5));
	GetCursorPos(&App_ptLastCursor);
	ScreenToClient(Render_hWnd,&App_ptLastCursor);
}

void App_EnterEditWeightMode()
{
	if(bCurrentlyEditingWeights==false)
	{
        App_PlayingMode = 0;
		App_CurrentTime = KEYFRAME_RESTPOSE;

		if(pCurrentlySelectedObject)
		{
			Render_ClearColor = 0xff444444;

			//we want to force the keyframe to be RP
			//selecting App_CurrentTime = KEYFRAME_RESTPOSE
			//is not sufficient. you must set pCurrentlySelectedKeyFrame to RP
			RefreshSubordinates(pCurrentlySelectedObject);//reset everything below pObject
			if(pCurrentlySelectedSkeleton)
			{
				pCurrentlySelectedKeyFrame = pCurrentlySelectedSkeleton->pRestPose;
				RefreshSubordinates(pCurrentlySelectedKeyFrame);
			}

			ObjectLL_AllMeshs_BackupRenderFlags(pCurrentlySelectedObject);

			ObjectLL_AllMeshs_ClearBit(pCurrentlySelectedObject,RENDER_TEXTURED);
			ObjectLL_AllMeshs_ClearBit(pCurrentlySelectedObject,RENDER_LIGHTING);
			ObjectLL_AllMeshs_ClearBit(pCurrentlySelectedObject,RENDER_TRANSLUCENT_VERTEX);
			ObjectLL_AllMeshs_ClearBit(pCurrentlySelectedObject,RENDER_TRANSLUCENT_TEXTURE);
			ObjectLL_AllMeshs_ClearBit(pCurrentlySelectedObject,RENDER_WIREFRAME);
			//ObjectLL_AllMeshs_SetBit(pCurrentlySelectedObject,RENDER_TRANSFORMED);
			ObjectLL_AllMeshs_SetBit(pCurrentlySelectedObject,RENDER_WIREFRAME);
			bCurrentlyEditingWeights = true;
		}
	}
}

void App_ExitEditWeightMode()
{
	if(bCurrentlyEditingWeights)
	{
		Render_ClearColor = 0xff000000;
		ObjectLL_AllMeshs_RestoreRenderFlags(pCurrentlySelectedObject);
		App_CurrentTime = 0;
		bCurrentlyEditingWeights = false;
	}
}
void App_UpdatePickSphere(struct ObjectLL* pObject)
{
	float pFinal[16];
	float pScale[16];
	float DeltaX,DeltaY,DeltaZ,MaxDelta;
	float ObjectMinX,ObjectMinY,ObjectMinZ;
	float ObjectMaxX,ObjectMaxY,ObjectMaxZ;

	if(pObject==NULL)return;

	//// Determine Radius 

	ObjectLL_GetMinMaxXYZ(pObject,
						ObjectMinX,ObjectMinY,ObjectMinZ,
						ObjectMaxX,ObjectMaxY,ObjectMaxZ);

	DeltaX = ObjectMaxX-ObjectMinX;
	DeltaY = ObjectMaxY-ObjectMinY;
	DeltaZ = ObjectMaxZ-ObjectMinZ;
	MaxDelta = MaxValueOutOf3(DeltaX,DeltaY,DeltaZ);
	App_PickSphereRadius = MaxDelta*0.4;//specialspecial
	if(App_PickSphereRadius<6.0)App_PickSphereRadius = 6.0;

	//// Scale Pick Sphere Matrix By Radius 
	
	Scale(pScale,App_PickSphereRadius);
	memcpy(	pAppTools_PickSphere->pMatrix,
			pObject->pMatrix,
			16*sizeof(float));
	MatrixMul(pFinal,pAppTools_PickSphere->pMatrix,pScale);
	memcpy(pAppTools_PickSphere->pMatrix,pFinal,16*sizeof(float));

	//// Position Pick Sphere on pObject

	pAppTools_PickSphere->pMatrix[12] = pObject->pMatrix[12];
	pAppTools_PickSphere->pMatrix[13] = pObject->pMatrix[13];
	pAppTools_PickSphere->pMatrix[14] = pObject->pMatrix[14];

}

void App_UpdatePickSphere(struct ObjectLL* pObject,struct MeshLL* pMesh)
{
	float pFinal[16];
	float pScale[16];
	float DeltaX,DeltaY,DeltaZ,MaxDelta;
	float MeshMinX,MeshMinY,MeshMinZ;
	float MeshMaxX,MeshMaxY,MeshMaxZ;

	if(pObject==NULL)return;
	if(pMesh==NULL)return;

	MeshLL_GetMinMaxXYZ(pMesh,
						MeshMinX,MeshMinY,MeshMinZ,
						MeshMaxX,MeshMaxY,MeshMaxZ);

	DeltaX = MeshMaxX-MeshMinX;
	DeltaY = MeshMaxY-MeshMinY;
	DeltaZ = MeshMaxZ-MeshMinZ;
	MaxDelta = MaxValueOutOf3(DeltaX,DeltaY,DeltaZ);
	App_PickSphereRadius = MaxDelta*0.4;//specialspecial
	if(App_PickSphereRadius<6.0)App_PickSphereRadius = 6.0;

	//// Scale Pick Sphere Matrix By Radius 
	

	//CALCULATEFINALMESHMATRIX
	if(0)//pMesh->RenderFlags & RENDER_TRANSFORMED)
	{
		memcpy(	pApp_AbsoluteMeshMatrix,
				pObject->pMatrix,
				//pRender_IdentityMatrix,
				16*sizeof(float));
	}
	else
	{
		MatrixMul(	pApp_AbsoluteMeshMatrix,
					pMesh->pMatrix,
					pObject->pMatrix);
	}

	Scale(pScale,App_PickSphereRadius);
	memcpy(	pAppTools_PickSphere->pMatrix,
			pApp_AbsoluteMeshMatrix,
			//pMesh->pMatrix,
			16*sizeof(float));
	MatrixMul(pFinal,pAppTools_PickSphere->pMatrix,pScale);
	memcpy(pAppTools_PickSphere->pMatrix,pFinal,16*sizeof(float));

	//// Position Pick Sphere on pMesh

	pAppTools_PickSphere->pMatrix[12] = pApp_AbsoluteMeshMatrix[12];
	pAppTools_PickSphere->pMatrix[13] = pApp_AbsoluteMeshMatrix[13];
	pAppTools_PickSphere->pMatrix[14] = pApp_AbsoluteMeshMatrix[14];

}




void App_UpdatePickSphere(struct ObjectLL* pObject,struct JointLL* pJoint)
{
	float pFinal[16];
	float pScale[16];
	float Length;

	if(pObject==NULL)return;
	if(pJoint==NULL)return;

	MacroMagnitude(Length,	pJoint->pRelativeMatrix[12],
							pJoint->pRelativeMatrix[13],
							pJoint->pRelativeMatrix[14]);

	App_PickSphereRadius = Length*0.3;
	if(App_PickSphereRadius<6.0)App_PickSphereRadius = 6.0;

	MatrixMul(	pApp_AbsoluteMeshMatrix,
				pJoint->pAbsoluteMatrix,
				pObject->pMatrix);
	Scale(pScale,App_PickSphereRadius);
	memcpy(	pAppTools_PickSphere->pMatrix,
			pApp_AbsoluteMeshMatrix,
			16*sizeof(float));
	MatrixMul(pFinal,pAppTools_PickSphere->pMatrix,pScale);
	memcpy(pAppTools_PickSphere->pMatrix,pFinal,16*sizeof(float));

	//// Position Pick Sphere on pMesh
	pAppTools_PickSphere->pMatrix[12] = pApp_AbsoluteMeshMatrix[12];
	pAppTools_PickSphere->pMatrix[13] = pApp_AbsoluteMeshMatrix[13];
	pAppTools_PickSphere->pMatrix[14] = pApp_AbsoluteMeshMatrix[14];
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
