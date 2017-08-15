//modeler.cpp written by david paull.
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
// Modeler.cpp is 10% finished.

#include "stdafx.h"
#include "resource.h"
#include "render.h"
#include "globals.h"
#include <windowsx.h>
#include "Bone.h"
#if 0//either app.cpp or modeler.cpp can be active. not both.

//////////////////
//// #defines ////
//////////////////

#define MAX_LOADSTRING 100

////////////////
//// extern ////
////////////////

//pick functions, and data from Render.cpp, the rendering engine.

extern void CreatePickRay(	struct MeshLL* pMesh,aVECTOR& Pos,aVECTOR& Dir);
extern SceneLL* MenuFunction_Scene_OpenScene(char* pName);

extern struct MeshLL*	pSystem_pVectorX;
extern struct MeshLL*	pSystem_pVectorY;
extern struct MeshLL*	pSystem_pVectorZ;
extern float 			pSystem_ViewMatrix[];
extern unsigned long     System_CurrentTime;
extern int               System_PlayingMode;
extern HWND 			 System_SavehWnd;
extern POINT 			 System_ptLastCursor;

extern struct SceneLL* 	pCurrentlySelectedScene;
extern struct ObjectLL* pCurrentlySelectedObject;
extern struct BoneH* 	pCurrentlySelectedBone;

extern MENU_MODE     	CurrentMenuMode;
extern MOUSE_MODE    	CurrentMouseMode;
extern KEYBOARD_MODE 	CurrentKeyboardMode;

extern bool 			bRMouseButtonDown;

////////////////////////
//// Typedef Struct ////
////////////////////////

//////////////////////////
//// Global Variables ////
//////////////////////////

/////////////////////////////
//// Function Prototypes ////
/////////////////////////////
void Render_Modeler();
void Keyboard();
void PaintWeights();

///////////////////
//// Functions ////
///////////////////

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	Render_Init("TN Modeler");

	if(Render_Create(hInstance,nCmdShow)==false)
	{//user hit close button
		return 0;
	}

	bool done=false;											// Bool Variable To Exit Loop
	MSG msg;

	//SEARCH FOR MESSAGEHANDLER1 modeler.cpp, Render.cpp
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
			Render_Modeler();
			Render_UserInterface();
			Render_EndScene();
		}
	}

	Render_Delete();
	return 0;
}


void Render_Modeler()
{
	struct SceneLL* 	pScene  = NULL;
	struct ObjectLL* 	pObject = NULL;
	struct MeshLL* 		pMesh   = NULL;
	struct FieldLL* 	pField  = NULL;
	struct IconLL* 		pIcon  	= NULL;
    struct BoneH* 		pThis   = NULL;
    int ret = 0;						
	///////////////////////////////////////////////////////////////////////////////////////
	if(ApiType == AT_None)
	{
		return;
	}
	///////////////////////////////////////////////////////////////////////////////////////

			
	//// Keyboard, mouselook, camera
	Keyboard();

	unsigned long CurrentTime = GetElapsedMilliSeconds();

	if(System_PlayingMode == -1)
	{
		System_CurrentTime -= CurrentTime;
	}
	else if(System_PlayingMode == 1)
	{
		System_CurrentTime += CurrentTime;
	}
	
	if(pCurrentlySelectedScene)
	{
		pScene = pCurrentlySelectedScene;
		pObject = pScene->pObjectHead;
		while(pObject)
		{
			pMesh = pObject->pMeshHead;
			while(pMesh)	 
			{
				RenderMesh(pScene,pMesh);
				pMesh = pMesh->pNext;
			}
			pObject = pObject->pNext;
		}
	}

	/////////////////////////////
	//// Origina XYZ Vectors ////
	/////////////////////////////

	aVECTOR Dir;
	aVECTOR Pos;

	Pos.x = 0.0;
	Pos.y = 0.0;
	Pos.z = 0.0;
	if(1)//XYZOriginVectors
	{
		float pInverseMatrix[16];
		aVECTOR CameraPos;
		InverseMatrix(pInverseMatrix,pSystem_ViewMatrix);
		CameraPos.x = pInverseMatrix[12];
		CameraPos.y = pInverseMatrix[13];
		CameraPos.z = pInverseMatrix[14];

		Dir.x = 1.0;
		Dir.y = 0.0;
		Dir.z = 0.0;
		CreateMatrixFacingPointOnOneAxis(Pos,Dir,CameraPos,pSystem_pVectorX->pMatrix);
		Dir.x = 0.0;
		Dir.y = 1.0;
		Dir.z = 0.0;
		CreateMatrixFacingPointOnOneAxis(Pos,Dir,CameraPos,pSystem_pVectorY->pMatrix);
		Dir.x = 0.0;
		Dir.y = 0.0;
		Dir.z = 1.0;
		CreateMatrixFacingPointOnOneAxis(Pos,Dir,CameraPos,pSystem_pVectorZ->pMatrix);

		RenderMesh(NULL,pSystem_pVectorX);
		RenderMesh(NULL,pSystem_pVectorY);
		RenderMesh(NULL,pSystem_pVectorZ);

	}
}


void Keyboard()
{
	aVECTOR CameraPos;

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
		CurrentMouseMode == MOUSE_MODE_BONE ||
		CurrentMouseMode == MOUSE_MODE_WEIGHT)
	{
		if( (!bShift   && bLastShift  ) || 
			(!bControl && bLastControl))
		{
			//reset prev to avoid snap. 
			EnterMouseLookMode();
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
	POINT ptCursor;
	int HalfWidth;
	int HalfHeight;

	static float YawAngle = 0.0;
	static float PitchAngle = 0.0;
	static float RollAngle = 0.0;
	float pRollMatrix[16];





	if(System_SavehWnd!=GetFocus())
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
					HalfWidth  = (int)(CurrentWindowWidth*0.5);
					HalfHeight = (int)(CurrentWindowHeight*0.5);

					GetCursorPos( &ptCursor );
					ScreenToClient(	System_SavehWnd,&ptCursor);

					if((abs(ptCursor.x-System_ptLastCursor.x)>0) ||
					   (abs(ptCursor.y-System_ptLastCursor.y)>0))
					{
						if(bRMouseButtonDown)
						{
							RollAngle  += (float)(System_ptLastCursor.x-ptCursor.x)*0.01;
							RollAngle  *= 0.5;//smoothing
						}
						else
						{
							YawAngle   += (float)(ptCursor.x-System_ptLastCursor.x)*0.01;
							YawAngle   *= 0.5;
							
							PitchAngle += (float)(ptCursor.y-System_ptLastCursor.y)*0.01;
							PitchAngle *= 0.5;
						}
						if(YawAngle != 0.0)
						{
							Identity(pInverseView);
							InverseMatrix(pInverseView,pSystem_ViewMatrix);
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
							InverseMatrix(pSystem_ViewMatrix,pMatrix1);

						}
						if(PitchAngle != 0.0)
						{
							Identity(pInverseView);
							InverseMatrix(pInverseView,pSystem_ViewMatrix);
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
							InverseMatrix(pSystem_ViewMatrix,pMatrix1);
						}
						if(RollAngle != 0.0)
						{
							Identity(pInverseView);
							InverseMatrix(pInverseView,pSystem_ViewMatrix);
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
							InverseMatrix(pSystem_ViewMatrix,pMatrix1);
						}
						SetCursorPos(HalfWidth,HalfHeight);
 					}


					RollAngle  *= 0.5;//smoothing decay
					YawAngle   *= 0.5;
					PitchAngle *= 0.5;

					GetCursorPos( &System_ptLastCursor );
					ScreenToClient(	System_SavehWnd,&System_ptLastCursor);
				}
			}
		}
	}








	if(CurrentKeyboardMode==KEYBOARD_MODE_CAMERA)
	{
		CameraPos.x = 0;
		CameraPos.y = 0;
		CameraPos.z = 0;

		float IncPosition = 0.5;
		float IncRotation = 0.05;


		if(GetAsyncKeyState(VK_W) < 0)
		{
			CameraPos.z += IncPosition;
		}
		if(GetAsyncKeyState(VK_Z) < 0)
		{
			CameraPos.z -= IncPosition;
		}
		if(GetAsyncKeyState(VK_A) < 0)
		{
			CameraPos.x -= IncPosition;
		}
		if(GetAsyncKeyState(VK_S) < 0)
		{
			CameraPos.x += IncPosition;
		}
		if(GetAsyncKeyState(VK_E) < 0)
		{
			CameraPos.y += IncPosition;
		}
		if(GetAsyncKeyState(VK_X) < 0)
		{
			CameraPos.y -= IncPosition;
		}




		if(GetAsyncKeyState(VK_UP) < 0)
		{
			CameraPos.z += IncPosition;
		}
		if(GetAsyncKeyState(VK_DOWN) < 0)
		{
			CameraPos.z -= IncPosition;
		}
		if(GetAsyncKeyState(VK_LEFT) < 0)
		{
			CameraPos.x -= IncPosition;
		}
		if(GetAsyncKeyState(VK_RIGHT) < 0)
		{
			CameraPos.x += IncPosition;
		}
		if(GetAsyncKeyState(vk_pg_up) < 0)
		{
			CameraPos.y += IncPosition;
		}
		if(GetAsyncKeyState(vk_pg_down) < 0)
		{
			CameraPos.y -= IncPosition;
		}







		float pIdentityMatrix[16];
		float pInverseMatrix[16];
		float pFinalMatrix[16];

		if(!((CameraPos.x == 0.0)&&(CameraPos.y == 0.0)&&(CameraPos.z == 0.0)))
		{
			Identity(pIdentityMatrix);
			pIdentityMatrix[12] = CameraPos.x;
			pIdentityMatrix[13] = CameraPos.y;
			pIdentityMatrix[14] = CameraPos.z;

			InverseMatrix(pInverseMatrix,pSystem_ViewMatrix);
			MatrixMul(pFinalMatrix,pIdentityMatrix,pInverseMatrix);
			InverseMatrix(pSystem_ViewMatrix,pFinalMatrix);
		}

		float YawAngle = 0.0;
		float PitchAngle = 0.0;
		float RollAngle = 0.0;
		if(GetAsyncKeyState(VK_CONTROL) < 0)
		{
			if(GetAsyncKeyState(VK_NUMPAD4) < 0)
			{
				RollAngle = -IncRotation;
			}
			if(GetAsyncKeyState(VK_NUMPAD6) < 0)
			{
				RollAngle = IncRotation;
			}
		}
		else
		{
			if(GetAsyncKeyState(VK_NUMPAD4) < 0)
			{
				YawAngle = -IncRotation;
			}
			if(GetAsyncKeyState(VK_NUMPAD6) < 0)
			{
				YawAngle = IncRotation;
			}
		}
		if(GetAsyncKeyState(VK_NUMPAD8) < 0)
		{
			PitchAngle = IncRotation;
		}
		if(GetAsyncKeyState(VK_NUMPAD2) < 0)
		{
			PitchAngle = -IncRotation;
		}







		float pMatrix1[16];
		float pYawMatrix[16];
		float pPitchMatrix[16];
		float pRollMatrix[16];
		float pInverseView[16];

	
		if(YawAngle != 0.0)
		{

			Identity(pInverseView);
			InverseMatrix(pInverseView,pSystem_ViewMatrix);
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
			InverseMatrix(pSystem_ViewMatrix,pMatrix1);

		}
		if(PitchAngle != 0.0)
		{
			Identity(pInverseView);
			InverseMatrix(pInverseView,pSystem_ViewMatrix);
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
			InverseMatrix(pSystem_ViewMatrix,pMatrix1);
		}
		if(RollAngle != 0.0)
		{
			Identity(pInverseView);
			InverseMatrix(pInverseView,pSystem_ViewMatrix);
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
			InverseMatrix(pSystem_ViewMatrix,pMatrix1);
		}
	}
}
#endif//either app.cpp or modeler.cpp can be active. not both.

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
