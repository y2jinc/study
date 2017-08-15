////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// AppMenu.cpp written by david paull. 
// Copyright (c) 2002 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// BOOKMARKS : 
// searchfor:BEVELEDGE
// searchfor MAINMENUMAINCALLER
// SEPERATESKELETON
// ICONBARTEXTCALLOUT
// CALL_FUNCTION_WHEN_FLAG_IS_TOGGLED
////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Library.h"
#include "FileAccess.h"
#include "FieldLL.h"
#include "IconLL.h"
#include "RectLL.h"
#include "AppMenu.h"
#include "SceneLL.h"
#include "ObjectLL.h"
#include "MeshLL.h"
#include "SkeletonLL.h"
#include "KeyFrameLL.h"
#include "JointLL.h"
#include <assert.h>
#include <stdio.h>
#define DISK 0

//#define TechNaturePrivate

#define NO_FUNCTION 0
#define NewScene 1

/////////////////////////////////////
//////////// Extern Data ////////////
//////////// Extern Data ////////////
/////////////////////////////////////
extern char* gp_TextBuffer1;
extern char* gp_TextBuffer2;
extern char* gp_TextBuffer3;

extern float App_BoneScale;
extern float App_JointScale;
extern float App_SprayRadius;
extern unsigned long App_DataViewFlags;
extern unsigned long App_RenderFlags;

extern float App_PickSphereRadius;
extern float App_CameraPositionSpeed;
extern float App_CameraRotationSpeed;

extern char pGlobalConsoleText1[];
extern char pGlobalConsoleText2[];

extern unsigned long App_CurrentTime;//ms
extern int System_PlayingMode;//-1,0,1 reverse, stop, play

extern float pRender_ViewMatrix[];
extern float pRender_ProjMatrix[];
extern float pRender_OrthoMatrix[];
extern float pRender_IdentityMatrix[];

extern struct SceneLL* 	pAppTools_Scene;
extern struct ObjectLL*	pAppTools_Object;

extern float Render_WindowWidth;
extern float Render_WindowHeight;

extern POINT LastSelectedPoint;

extern HWND SavehWnd;
extern struct FieldLL* pRender_FieldHead;
extern POINT CurrentMousePosition;
extern FontObject AppTools_Fonts[];

extern MENU_MODE  			CurrentMenuMode;
extern MOUSE_MODE 			CurrentMouseMode;
extern KEYBOARD_MODE 		CurrentKeyboardMode;

extern struct AppItem*		pCurrentlySelectedApp;
extern struct SceneLL* 		pCurrentlySelectedScene;
extern struct ObjectLL* 	pCurrentlySelectedObject;
extern struct MeshLL* 		pCurrentlySelectedMesh;

extern struct SkeletonLL*   pCurrentlySelectedSkeleton;
extern struct KeyFrameLL*	pCurrentlySelectedKeyFrame;
extern struct JointLL* 		pCurrentlySelectedJoint;

extern unsigned long CurrentTime;

extern float IconWidth;

extern float TempHearchyX;
extern float TempHearchyY;


extern unsigned long* pCurrentlySelectedFlags;
extern unsigned long  iCurrentlySelectedFlag;
extern bool           bCurrentlySelectedClear;


extern struct FieldLL* pSelectedField;
extern struct IconLL* pRender_IconHead;
extern struct RectLL* pRender_RectHead;
extern struct SceneLL* pRender_SceneHead;

extern FILE* DiskFile;


////////////////////////////////////////////////////
//////////// Extern Function Prototypes ////////////
//////////// Extern Function Prototypes ////////////
////////////////////////////////////////////////////

extern bool Load_txt(char* pFileName,struct MeshLL* pMesh);
extern bool Load_txt(char* pFileName,struct ObjectLL* pObject,float fps, bool bSmoothGroups);



/////////////////////////////////////
//////////// Global Data ////////////
//////////// Global Data ////////////
/////////////////////////////////////
float IconWidth = 32+4;
float IconScale = (32.0/32.0);
float TempHearchyX = 0.0;
float TempHearchyY = 0.0;


float	MainMenuDataWidth            = 0.35;
float	MainMenuDataHeight           = 0.08;

// main menu flags.
unsigned long 	  	bExpand_MainMenu_App	   	= false;
unsigned long 	  	bExpand_MainMenu_Scene	   	= false;
unsigned long 	  	bExpand_MainMenu_Object	   	= false;
unsigned long 	  	bExpand_MainMenu_Mesh	   	= false;
unsigned long 	  	bExpand_MainMenu_Texture 	= false;
unsigned long 	  	bExpand_MainMenu_Skeleton  	= false;
unsigned long 	  	bExpand_MainMenu_KeyFrame  	= false;
unsigned long 	  	bExpand_MainMenu_Joint	   	= false;

unsigned long 	  	bShow_ViewMatrix 			= false;
unsigned long 	  	bShow_ProjMatrix 			= false;
unsigned long 	  	bShow_OrthoMatrix			= false;
unsigned long 	  	bShow_NewPlanet				= false;

long TextColor_Matrix 	    = PACKRGBA(181,181,181,255);
long TextColor_Editable     = PACKRGBA(220,220,220,255);
long TextColor_NotEditable  = PACKRGBA(175,175,175,255);
long TextColor_ConsoleFadeTo= PACKRGBA(50,50,50,0);

long TextColor_ConsoleSuccess = PACKRGBA( 64,255, 64,0);
long TextColor_ConsoleError   = PACKRGBA(255,200,200,0);
long TextColor_ConsoleNormal  = PACKRGBA(255,255,255,0);


long TextColor_App 	    	= PACKRGBA(254,255,196,255);
long TextColor_Scene 		= PACKRGBA(229,255,196,255);
long TextColor_Object 		= PACKRGBA(204,255,205,255);
long TextColor_Mesh   		= PACKRGBA(196,253,226,255);
long TextColor_Skeleton		= PACKRGBA(196,246,252,255);
long TextColor_KeyFrame		= PACKRGBA(206,198,255,255);
long TextColor_Joint   		= PACKRGBA(232,196,255,255);

long TextColor_Vertices		= PACKRGBA(196,219,255,255);
long TextColor_Triangles	= PACKRGBA(206,198,255,255);

long MouseOverTextColor	= PACKRGBA(255,128,128,255);

long SelectedTextColor_App  		= PACKRGBA(215,255,196,255);
long SelectedTextColor_Scene 		= PACKRGBA(215,255,196,255);
long SelectedTextColor_Object 		= PACKRGBA(215,255,196,255);
long SelectedTextColor_Mesh   	   	= PACKRGBA(215,255,196,255);
long SelectedTextColor_Texture     	= PACKRGBA(215,255,196,255);
long SelectedTextColor_Skeleton    	= PACKRGBA(215,255,196,255);
long SelectedTextColor_KeyFrame    	= PACKRGBA(215,255,196,255);
long SelectedTextColor_Joint       	= PACKRGBA(215,255,196,255);

LineOfConsoleText pConsoleText[MAX_LINES_CONSOLE_TEXT];
long              pConsoleTextColorIndex[MAX_LINES_CONSOLE_TEXT];
long              cConsoleText = 0;//0,1,2,3,0,1,2,3
float 				DataViewMinLeft;
float 				DataViewMaxRight;


long MainMenu_FunctionTextColor = PACKRGBA(215,215,255,255);

char quote = 34;


char*  pNewSceneName = NULL;
char* pOpenSceneName = NULL;
char* pSaveSceneName = NULL;

char*  pNewObjectName = NULL;
char* pOpenObjectName = NULL;
char* pSaveObjectName = NULL;

char*  pNewMeshName = NULL;
char* pOpenMeshName = NULL;
char* pSaveMeshName = NULL;

char*  pNewSkeletonParentName = NULL;
char*  pNewSkeletonName       = NULL;
char* pOpenSkeletonName       = NULL;
char* pSaveSkeletonName       = NULL;

char*  pNewJointParentName = NULL;
char*  pNewJointName       = NULL;
char* pOpenJointName       = NULL;
char* pSaveJointName       = NULL;

char* pTempText 			= NULL;


unsigned long NewKeyFrame_time = 0;
unsigned long NewKeyFrame_anim_num = 0;

unsigned long CopyKeyFrame_From_anim_num = 0;
unsigned long CopyKeyFrame_From_time = 0;
unsigned long CopyKeyFrame_To_anim_num = 0;
unsigned long CopyKeyFrame_To_time = 0;


unsigned short NewJointAngle = 0;//argument for NewChildJoint(0-360)
float          NewJointLength = 1.0;//argument for NewChildJoint(0-inf)

fRECT* pIconTextureRects = NULL;

int num_console_lines = 3;//2 console, 1 icon bar(top)

long StructRenderSceneHeadOpen = true;//true/false

bool bWM_LBUTTONDOWN = false;

bool bCurrentlyLoadingSystemFiles = false;



char *SceneLLMatrix[] =
{
	"fch",//SortKey
	"fch",//pView
	"fch",//pProj
	"fgh",//pObjectHead
};

char *MatrixIcons[] =
{
	"c",//SortKey
	"c",//pView
	"c",//pProj
	"g",//pObjectHead
};
/////////////////////////////////////////
//////////////// OBJECTS ////////////////
/////////////////////////////////////////
char *ObjectLLTextVersion[] =
{
	"6",//num of elements in this list
	"",//pName. just output name, not pName = name
	"SortKey = ",
	"World Matrix",	//pMatrix
	"Meshes",
	"Skeleton",
	"Use Object Matrix = ",//bUseObjectMatrix
};
char *ObjectLLConnectionTypes[] =
{
	"5",//num of elements in this list
	"fca",//pName. just output name, not pName = name
	"ffch",	//SortKey
	"ffce",	//pMatrix
	"ffce",	//pMeshHead
	"ffch",	//bUseObjectMatrix
};
char *ObjectLLMatrix[] =
{
	"c",//SortKey
	"c",//pView
	"c",//pProj
	"g",//pMeshHead
};

char *MeshLLConnectionTypes[] =
{
	"11",//num of elements in this list
	"fffca",	//pName. just output name, not pName = name
	"ffffch",	//SortKey
	"ffffce",	//pMatrix
	"ffffce",	//pVertices
	"ffffce",	//ptlVertices
	"ffffce",	//ptlVertices
	"ffffch",	//num_verts
	"ffffch",	//num_tris
	"ffffce",	//SrcBlend
	"ffffce",	//DstBlend
	"ffffce",	//RenderFlags
};
char *MeshLLMatrix[] =
{
	"c",//SortKey
	"c",//pView
	"c",//pProj
	"g",//pObjectHead
};
///////////////////////////////////////////
char *MeshListIcons[] =
{
	"c",//Start
	"g",//End
};
///////////////////////////////////////////

char *AppRenderFlagsText[] =
{
	"2",//num of elements in this list.
	"Show Joints",
	"Show Bones",
};

char *ObjectRenderFlagsText[] =
{
	"3",//num of elements in this list.
	"Meshs : Wireframe",
	"Meshs : Alpha Vertex",
	"Meshs : Textured",

};
char *MeshRenderFlagsText[] =
{
	"13",//num of elements in this list.
	"Orthographic",
	"Wireframe",
	"Selectable",
	"Z Buffer Read",
	"Z Buffer Write",
	"Bilinear Filter",
	"Translucent Vertex",
	"Translucent Texture",
	"Transparent Texture",
	"Lighting",
	"Textured",
	"Transformed",
	"Visible",

};

char *BlendModeText[] =
{
	"11",//num of elements in this list.
	"Zero",
	"One",
	"SrcColor",
	"InvSrcColor",
	"SrcAlpha",
	"InvSrcAlpha",
	"DstAlpha",
	"InvDstAlpha",
	"DstColor",
	"InvDstColor",
	"SrcAlphaSat",
};


/////////////////////////////////////////////
//////////// Function Prototypes ////////////
//////////// Function Prototypes ////////////
/////////////////////////////////////////////
void GetNamePath(char* pName,char* pExt);

char* GetConsoleText(int line_num);
long GetConsoleTextColor(int line_num);
void OutputTextToConsole(char *pText,long Color);
long WrapQueueIndex(long index);
long BlendColor(long Color1,long Color2,float t);
void ConvertRectLLintoIconLL(	struct RectLL* pInitialLL,
								struct IconLL*& pIconHeadIn);

void RenderMainMenu_FunctionsText_App(float& PosX, float& PosY);
void RenderMainMenu_FunctionsText_Scene(float& PosX, float& PosY);
void RenderMainMenu_FunctionsText_Object(float& PosX, float& PosY);
void RenderMainMenu_FunctionsText_Mesh(float& PosX, float& PosY);
void RenderMainMenu_FunctionsText_Texture(float& PosX,float& PosY);
void RenderMainMenu_FunctionsText_Skeleton(float& PosX, float& PosY);
void RenderMainMenu_FunctionsText_KeyFrame(float& PosX, float& PosY);
void RenderMainMenu_FunctionsText_Joint(float& PosX, float& PosY);

void RenderMainMenu_InstancesText_App(float& PosX, float& PosY);
void RenderMainMenu_InstancesText_Scene(struct SceneLL* pScene, float& PosX, float& PosY);
void RenderMainMenu_InstancesText_Object(struct ObjectLL* pObject, float& PosX, float& PosY);
void RenderMainMenu_InstancesText_Mesh(struct MeshLL* pMesh, float& PosX, float& PosY);
void RenderMainMenu_InstancesText_Texture(aTEXTURE* pTexture, float& PosX, float& PosY);
void RenderMainMenu_InstancesText_Skeleton(struct SkeletonLL* pSkeleton, float& PosX, float& PosY);
void RenderMainMenu_InstancesText_KeyFrame(struct KeyFrameLL* pKeyFrame, float& PosX, float& PosY);
void RenderMainMenu_InstancesText_Joint(struct JointLL* pJoint, float& PosX, float& PosY);


void RenderMainMenu_FunctionBackground_App(float& PosX,float& PosY);
void RenderMainMenu_FunctionBackground_Scene(float& PosX,float& PosY);
void RenderMainMenu_FunctionBackground_Object(float& PosX,float& PosY);
void RenderMainMenu_FunctionBackground_Mesh(float& PosX,float& PosY);
void RenderMainMenu_FunctionBackground_Texture(float& PosX,float& PosY);
void RenderMainMenu_FunctionBackground_Skeleton(float& PosX,float& PosY);
void RenderMainMenu_FunctionBackground_KeyFrame(float& PosX,float& PosY);
void RenderMainMenu_FunctionBackground_Joint(float& PosX,float& PosY);

void RenderMainMenu_InstancesBackground_Scene(struct SceneLL* pScene, float& PosX, float& PosY);
void RenderMainMenu_InstancesBackground_Object(struct ObjectLL* pObject, float& PosX, float& PosY);
void RenderMainMenu_InstancesBackground_Mesh(struct MeshLL* pMesh, float& PosX, float& PosY);

#ifdef TechNaturePrivate
void Planet_RenderMenu_Background(float& PosX,float& PosY);
void Planet_RenderMenu_Text(float& PosX,float& PosY);
#endif	

struct FieldLL* InsertHalfHeightMenuItemTop(char* pText,float PosX,float PosY,long Color);
struct FieldLL* InsertHalfHeightMenuItemBottom(char* pText,float PosX,float PosY,long Color);

//ICON FUNCTIONS

void RenderMainMenuBackGround();
void RenderMainMenuText();
struct FieldLL* ClipInsertMenuItem(char* pText,float PosX,float PosY,long Color);
struct FieldLL* InsertMenuItem2(char* pText,float PosX,float PosY,long Color);
struct FieldLL* InsertMenuItem(char* pText,float PosX,float PosY,long Color);
struct FieldLL* InsertEditableMenuItem(
				char* pText,
				float PosX,
				float PosY,
				float Width,//width in pixels of editable field.
				long Color,
				float* pfloat,
				unsigned long* plong,
				unsigned short* pword,
				char* pchar);
struct FieldLL* InsertEditableMenuItem2(
				char* pText,
				float PosX,
				float PosY,
				float Width,//width in pixels of editable field.
				long Color,
				float* pfloat,
				unsigned long* plong,
				unsigned short* pword,
				char* pchar);
void RenderMainMenu_RenderFlagsText(	
				unsigned long ViewFlags, 
				unsigned long flag, 
				char* pItems[],
				long TextColor, 
				float SaveX,
				float& PosX, 
				float& PosY);






void RefreshSubordinates(struct SceneLL* pScene)
{
	memset(pSaveSceneName,0,512);
	sprintf(pSaveSceneName,"%s",pScene->pName);

	//assign CurrentlySelected
	pCurrentlySelectedScene = pScene;
	//if(pScene==NULL)return;

	//select first object
	pCurrentlySelectedObject = (pScene) ? (pScene->pObjectHead) : (NULL);
	RefreshSubordinates(pCurrentlySelectedObject);
}

void RefreshSubordinates(struct ObjectLL* pObject)
{
	memset( pSaveObjectName,0,512);
	sprintf(pSaveObjectName,"%s",pObject->pName);


	//assign CurrentlySelected
	pCurrentlySelectedObject = pObject;
	//if(pObject==NULL)return;

	//select first mesh
	pCurrentlySelectedMesh = (pObject) ? (pObject->pMeshHead) : (NULL);
	RefreshSubordinates(pCurrentlySelectedMesh);


	//select first skeleton
	pCurrentlySelectedSkeleton = (pObject) ? (pObject->pSkeletonHead) : (NULL);
	RefreshSubordinates(pCurrentlySelectedSkeleton);

}
void RefreshSubordinates(struct MeshLL* pMesh)
{
	memset( pSaveMeshName,0,512);
	sprintf(pSaveMeshName,"%s",pMesh->pName);

	//assign CurrentlySelected
	pCurrentlySelectedMesh = pMesh;
	//if(pMesh==NULL)return;

	
}
void RefreshSubordinates(struct SkeletonLL* pSkeleton)
{
	memset( pSaveSkeletonName,0,512);
	sprintf(pSaveSkeletonName,"%s",pSkeleton->pName);

	//assign CurrentlySelected
	pCurrentlySelectedSkeleton = pSkeleton;
	//if(pSkeleton==NULL)return;

	//select first KeyFrame
	//pCurrentlySelectedKeyFrame = (pSkeleton) ? (pSkeleton->pKeyFrameHead) : (NULL);
	//we dont want CurrFrame to defaut as fisrt since you cant add joints to it
	pCurrentlySelectedKeyFrame = (pSkeleton && pSkeleton->pRestPose) ? (pSkeleton->pRestPose) : (NULL);
	RefreshSubordinates(pCurrentlySelectedKeyFrame);

}
void RefreshSubordinates(struct KeyFrameLL* pKeyFrame)
{
	//assign CurrentlySelected
	pCurrentlySelectedKeyFrame = pKeyFrame;
	//if(pKeyFrame==NULL)return;

	//select first Joint
	pCurrentlySelectedJoint = (pKeyFrame) ? (pKeyFrame->pJointHead) : (NULL);
	RefreshSubordinates(pCurrentlySelectedJoint);
}
void RefreshSubordinates(struct JointLL* pJoint)
{
	//assign CurrentlySelected
	pCurrentlySelectedJoint = pJoint;
	//if(pJoint==NULL)return;
}

void InsertIconIntoIconLL(		float& PosX,
								float& PosY,
								char* pIcons)
{
	long i,j,index,Length;
	float PreStep = 0.0;
	fRECT Screen = {0,0,0,0};
	fRECT Texture[16];
	struct IconLL* pIcon = NULL;

	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			index = j*4+i;
			Texture[index].left    = (float)(i+0)*(64.0/256.0);
			Texture[index].right   = (float)(i+1)*(64.0/256.0);
			Texture[index].top     = (float)(j+0)*(64.0/256.0);
			Texture[index].bottom  = (float)(j+1)*(64.0/256.0);
		}
	}

	Length = strlen(pIcons);
	for(i=0;i<Length;i++)
	{
		Screen.left   = PosX;
		Screen.right  = PosX+IconWidth;
		Screen.top    = (PosY-PreStep);
		Screen.bottom = (PosY-PreStep)+IconWidth;

		pIcon = IconLL_Insert(	"Data View Icon",
								1,//0 is reserved for object coloring
								Screen,
//								pIconTextureRects[(int)(pIcons[i]-97)],
								pIconTextureRects[(int)(((unsigned char)pIcons[i])-97)],
								//Texture[(int)(pIcons[i]-97)],
								0,
								pRender_IconHead);
		PosX += IconWidth;
	}
}
//everything past skeleton is offset from perfect IconWidth height spacing.

void InsertIconIntoIconLL2(		float& PosX,
								float& PosY,
								char* pIcons)
{
	long i,j,index=0,Length;
	float PreStep = 0.0;
	fRECT Screen = {0,0,0,0};
	fRECT Texture[24];
	struct IconLL* pIcon = NULL;
	float StepY = IconWidth;
	float posy = 0.0;
	float RestoreValue = 0.0;

	for(j=0;j<3;j++)
	{
		for(i=0;i<8;i++)
		{
			Texture[index].left    = (float)(i+0)*(64.0/512.0);
			Texture[index].right   = (float)(i+1)*(64.0/512.0);
			Texture[index].top     = (float)(j+0)*(64.0/512.0);
			Texture[index].bottom  = (float)(j+1)*(64.0/512.0);
			index++;
		}
	}

	Length = strlen(pIcons);
	for(i=0;i<Length;i++)
	{
		Screen.left   = PosX;
		Screen.right  = PosX+IconWidth;
		Screen.top    = (PosY-PreStep);
		Screen.bottom = (PosY-PreStep)+IconWidth;

		//save this value, we may overwrite it to account for clipping/
		RestoreValue = Texture[(int)(((unsigned char)pIcons[i])-97)].top;

		if(Screen.top<(StepY*num_console_lines))
		{
			posy = (StepY*num_console_lines)-Screen.top;
			Screen.top = (StepY*num_console_lines);
			Texture[(int)(((unsigned char)pIcons[i])-97)].top += (posy/StepY)*(64.0/512.0);
		}



		pIcon = IconLL_Insert(	"Data View Icon",
								1,//0 is reserved for object coloring
								Screen,
//								pIconTextureRects[(int)(pIcons[i]-97)],
								//pIconTextureRects[(int)(((unsigned char)pIcons[i])-97)],
								Texture[(int)((unsigned char)pIcons[i]-97)],
								0,
								pRender_IconHead);

		//restore UVs for next pass
		Texture[(int)(((unsigned char)pIcons[i])-97)].top = RestoreValue;

		PosX += IconWidth;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
void RenderMatrix(	float* pMat,
					float NewPosX,
					float& PosY)
{
	fRECT Screen = {0,0,0,0};
	long i,j,index;
	float ElementPosX;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	float MatrixElementWidth = 0.0;
	char pFieldText[256];
//	float StepY = 36.0/Render_WindowHeight;
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;

	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			index = j*4+i;					
			ElementPosX = NewPosX+//start here
						 ((MatrixElementWidth)*i);//add offset for the columns
						 //((MatrixElementWidth/Render_WindowWidth)*i);//add offset for the columns
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{
				sprintf(pFieldText,"%.4f",pMat[index]);

				//pField = InsertMenuItem2(pFieldText,ElementPosX,PosY,TextColor_Matrix);

				pField = InsertEditableMenuItem2(pFieldText,ElementPosX,PosY,320.0,TextColor_Matrix,
							&pMat[index],			//pfloat
							NULL,			//plong
							NULL,			//pword
							NULL);	//pchar









				//Width = CalcWidthField(pField,&AppTools_Fonts[0]);
				/*
				pField = FieldLL_Insert(0,"Debug Value",pFieldText,
							ElementPosX,PosY,
							(120.0/Render_WindowWidth),(46.0/Render_WindowWidth),
							Screen,
							1.0,0.0,0.0,TextColor_Matrix,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE|TEXT_FLAG_EDITABLE,
							0,
							&pMat[index],	//pfloat
							NULL,			//plong
							NULL,			//pword
							NULL,			//pchar
							&AppTools_Fonts[0],
							pRender_FieldHead);
				*/
				MatrixElementWidth = 120;

				pField->Screen.left   = ElementPosX;
				pField->Screen.right  = ElementPosX+MatrixElementWidth;
//				pField->Screen.top    = (PosY-PreStep);
//				pField->Screen.bottom = (PosY-PreStep)+IconWidth;
				PickDataViewField(LastSelectedPoint,pField);//->Screen,long* pVisibilityBit)
				if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
			}
		}
		PosY+=IconWidth;
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////
//void Render3dVertices(	VERTEX_XYZNRGBUV *pWorldVerts,
void Render3dVertices(	VERTEX_XYZNRGBUV *pLocalVerts,
						unsigned long num_verts,
						float NewPosX,
						float& PosY)
{
	fRECT Screen = {0,0,0,0};
	long i,j,index;
	float ElementPosX;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	float MatrixElementWidth = 0.0;
	char pFieldText[256];
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);
	float* pV = (float*)pLocalVerts;
//	float* pV = (float*)pWorldVerts;
	long* pL = (long*)pLocalVerts;
	float StepY = IconWidth;

	if(pLocalVerts==NULL)return;

	for(j=0;j<num_verts;j++)
	{
		for(i=0;i<9;i++)
		{
			index = j*9+i;					
			ElementPosX = NewPosX+((MatrixElementWidth)*i);//add offset for the columns
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{



				sprintf(pFieldText,"%.3f",pV[index]);
				if(i==6)
				{
					sprintf(pFieldText,"0x%X",pL[index]);
				}

				pField = FieldLL_Insert(0,"Debug Value",pFieldText,
							ElementPosX,PosY,
							(120.0/Render_WindowWidth),(46.0/Render_WindowWidth),
							Screen,
							1.0,0.0,0.0,TextColor_Vertices,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE|TEXT_FLAG_EDITABLE,
							0,
							&pV[index],	//pfloat
							NULL,		//plong
							NULL,		//pword
							NULL,		//pchar
							&AppTools_Fonts[0],
							pRender_FieldHead);

				MatrixElementWidth = 120;
				if(i==6)MatrixElementWidth = 150;

				pField->Screen.left   = ElementPosX;
				pField->Screen.right  = ElementPosX+MatrixElementWidth;
				pField->Screen.top    = (PosY-PreStep);
				pField->Screen.bottom = (PosY-PreStep)+IconWidth;
				PickDataViewField(LastSelectedPoint,pField);//->Screen,long* pVisibilityBit)
				if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
			}
		}
		PosY+=IconWidth;
	}
}
void RenderIndices(	WORD* pIndices,
					unsigned long num_tris,
					float NewPosX,
					float& PosY)
{
	fRECT Screen = {0,0,0,0};
	long i,j,index;
	float ElementPosX;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	float MatrixElementWidth = 0.0;
	char pFieldText[256];
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;

	for(j=0;j<num_tris;j++)
	{
		for(i=0;i<3;i++)
		{
			index = j*3+i;					
			ElementPosX = NewPosX+((MatrixElementWidth)*i);//add offset for the columns
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{
				sprintf(pFieldText,"%d",pIndices[index]);
				pField = FieldLL_Insert(0,"Debug Value",pFieldText,
							ElementPosX,PosY,
							(120.0/Render_WindowWidth),(46.0/Render_WindowWidth),
							Screen,
							1.0,0.0,0.0,TextColor_Triangles,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE|TEXT_FLAG_EDITABLE,
							0,
							NULL,				//pfloat
							NULL,				//plong
							&pIndices[index],	//pword
							NULL,				//pchar
							&AppTools_Fonts[0],
							pRender_FieldHead);
				MatrixElementWidth = 120;
				pField->Screen.left   = ElementPosX;
				pField->Screen.right  = ElementPosX+MatrixElementWidth;
				pField->Screen.top    = (PosY-PreStep);
				pField->Screen.bottom = (PosY-PreStep)+IconWidth;
				PickDataViewField(LastSelectedPoint,pField);//->Screen,long* pVisibilityBit)
				if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
			}
		}
		PosY+=IconWidth;
	}
}




//when mouse down occours, the vars set here are used

void PickDataView(	POINT MousePos,
					long MinY,
					long MaxY,
					unsigned long* pFlags,//pointer to this objects flags var
					unsigned long  iFlag,//flag that was clicked
					bool bClear)//bool tells wether or not to clear all flags, or |= flags
{

	if(MousePos.y >= MinY)
	{
		if(MousePos.y <= MaxY)
		{
			pCurrentlySelectedFlags = pFlags;
			iCurrentlySelectedFlag  = iFlag;
			bCurrentlySelectedClear = bClear;
		}
	}
}
bool PickDataView_IsCursorOnThisLine(POINT MousePos,long MinY,long MaxY)
{
	if(MousePos.y >= MinY)
	{
		if(MousePos.y <= MaxY)
		{
			return true;
		}
	}
	return false;
}

bool PickDataView_IsCursorClickingFunction(POINT MousePos,long MinX,long MaxX,long MinY,long MaxY)
{
	if(MousePos.x >= MinX)
	{
		if(MousePos.x <= MaxX)
		{
			if(MousePos.y >= MinY)
			{
				if(MousePos.y <= MaxY)
				{
					return true;
				}
			}
		}
	}
	return false;
}
void PickDataViewField(POINT MousePos,FieldLL* pField)
{
	if(!IsBitSet(pField->flags,TEXT_FLAG_EDITABLE))
	{
		return;//not edatable, so not selectable
	}

	if(MousePos.y >= pField->Screen.top)
	{
		if(MousePos.y <= pField->Screen.bottom)
		{
			if(MousePos.x >= pField->Screen.left)
			{
				if(MousePos.x <= pField->Screen.right)
				{
					//pCurrentSelectedField = pField;
					pSelectedField = pField;
				}
			}
		}
	}
}


#if 0
/*
//a b c d 
//e f g h 
//i j k l
//m n o p
void MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU()
void MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU()
void MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU_MENU()
*/
#endif



//ICONBARTEXTCALLOUT
void RenderIconBarCurrentObjectText()
{
	struct SceneLL* pScene = NULL;
	struct FieldLL* pField = NULL;
	float SaveX,SaveY;
	float PosX,PosY;
	float ClipTop;
	float ClipBottom;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
	float Width;
	fRECT Screen = {0,0,0,0};

	PosX  = 0;//IconWidth;
	PosY  = 0;//TempHearchyY;

	SaveX = PosX;
	SaveY = PosY;

	char pTextLine1[256];
	char pTextLine2[256];

	memset(pTextLine1,0,256*sizeof(char));
	memset(pTextLine2,0,256*sizeof(char));

	if(CurrentMouseMode==MOUSE_MODE_MENU)
	{
	}
	////////////////////////
	//////// OBJECT ////////
	////////////////////////
	if(CurrentMouseMode==MOUSE_MODE_OBJECT)
	{
		if(pCurrentlySelectedObject)
		   	sprintf(pTextLine1," current object : %s",pCurrentlySelectedObject->pName);
		else
			sprintf(pTextLine1," current object : (null)");

		if(pCurrentlySelectedMesh)
			sprintf(pTextLine2," current mesh : %s",pCurrentlySelectedMesh->pName);
		else
			sprintf(pTextLine2," current mesh : (null)");
	}
	//////////////////////
	//////// MESH ////////
	//////////////////////
	if(	CurrentMouseMode==MOUSE_MODE_MESH)
	{
		if(	pCurrentlySelectedMesh &&
			pCurrentlySelectedMesh->RenderFlags & RENDER_TRANSFORMED)
		{//cant rotate
			if(pCurrentlySelectedMesh)
				sprintf(pTextLine1," current mesh : %s",pCurrentlySelectedMesh->pName);
			else
				sprintf(pTextLine1," current mesh : (null)");

		 	sprintf(pTextLine2," this Mesh's [Transformed] flag is set. Can't rotate or move.");
		}
		else
		{//normal
			if(pCurrentlySelectedObject)
				sprintf(pTextLine1," current object : %s",pCurrentlySelectedObject->pName);
			else
				sprintf(pTextLine1," current object : (null)");

			if(pCurrentlySelectedMesh)
				sprintf(pTextLine2," current mesh : %s",pCurrentlySelectedMesh->pName);
			else
				sprintf(pTextLine2," current mesh : (null)");
		}
	}
	///////////////////////
	//////// JOINT ////////
	///////////////////////
	if(CurrentMouseMode==MOUSE_MODE_JOINT)
	{
		if(pCurrentlySelectedObject && pCurrentlySelectedObject->pSkeletonHead)
		{
			if(pCurrentlySelectedJoint)
				sprintf(pTextLine1," current Joint : %s",pCurrentlySelectedJoint->pInfoItem->pName);
			else
				sprintf(pTextLine2," current Joint : (null)");

			sprintf(pTextLine2," current time : %u",App_CurrentTime);
	
			if(App_CurrentTime==KEYFRAME_RESTPOSE)
				sprintf(pTextLine2," current time : RestPose");

			if(App_CurrentTime==KEYFRAME_CURRPOSE)
				sprintf(pTextLine2," current time : CurrPose");
		}
		else
		{
			if(pCurrentlySelectedObject)
				sprintf(pTextLine1," current object : %s",pCurrentlySelectedObject->pName);
			else
				sprintf(pTextLine1," current object : (null)");

		 	sprintf(pTextLine2," this Object doesn't have a Skeleton. Can't edit Joints.");
		}
	}
	////////////////////////
	//////// WEIGHT ////////
	////////////////////////
	if(CurrentMouseMode==MOUSE_MODE_WEIGHT)
	{
		if(pCurrentlySelectedObject && pCurrentlySelectedObject->pSkeletonHead)
		{
			if(pCurrentlySelectedJoint)
				sprintf(pTextLine1," current Joint  : %s : %d",pCurrentlySelectedJoint->pInfoItem->pName,pCurrentlySelectedJoint->UniqueID);

			if(pCurrentlySelectedMesh)
				sprintf(pTextLine2," current mesh  : %s",pCurrentlySelectedMesh->pName);
		}
		else
		{
			if(pCurrentlySelectedObject)
				sprintf(pTextLine1," current object : %s",pCurrentlySelectedObject->pName);
			else
				sprintf(pTextLine1," current object : (null)");

		 	sprintf(pTextLine2," this Object doesn't have a Skeleton. Can't edit Weights.");
		}
	}

	//////////////////////
	//////// VERT ////////
	//////////////////////

	if(	CurrentMouseMode==MOUSE_MODE_VERT)
	{
		if(pCurrentlySelectedObject)
			sprintf(pTextLine1," current object : %s",pCurrentlySelectedObject->pName);
		if(pCurrentlySelectedMesh)
			sprintf(pTextLine2," current mesh  : %s",pCurrentlySelectedMesh->pName);
	}
	////////////////////////
	//// Console Line 0 ////
	////////////////////////
	PosX       = SaveX+(12*IconWidth);
	
	





	//// OVERRIDE : IF VIEWING SYSTEM SCENE

	if(	pCurrentlySelectedObject && 
		bstrcmp(pCurrentlySelectedObject->pName,"System"))
	{
		sprintf(pTextLine1," you are currently viewing the system Objects.");
		sprintf(pTextLine2," create/change Scenes to add/view your Objects.");
	}

	////////////////////////////////////////
	//// Extra Offset If Too Many Icons ////
	////////////////////////////////////////
	// DOWN BELOW ALSO !! ! ! !
	// DOWN BELOW ALSO !! ! ! !

	if(CurrentMouseMode==MOUSE_MODE_OBJECT)
	{//cant rotate, so move text over. no lmb, rmb buttons
	 //also extra room is needed, for warning message
		//PosX = SaveX+(10*IconWidth);
	}					
	if(CurrentMouseMode==MOUSE_MODE_MESH)
	{//cant rotate, so move text over. no lmb, rmb buttons
	 //also extra room is needed, for warning message
		if(	 pCurrentlySelectedMesh &&
		     pCurrentlySelectedMesh->RenderFlags & RENDER_TRANSFORMED)
		{
			PosX = SaveX+(10*IconWidth);
		}
	}					
	if(CurrentMouseMode==MOUSE_MODE_JOINT)
	{//more icons. vcr control buttons
		if(pCurrentlySelectedObject && pCurrentlySelectedObject->pSkeletonHead)
		{
			PosX = SaveX+(20*IconWidth);
		}
		else
		{
			PosX = SaveX+(10*IconWidth);
		}
	}

	if(CurrentMouseMode==MOUSE_MODE_WEIGHT)
	{//more icons. select one, select many, lmb rmb
		if(pCurrentlySelectedObject && pCurrentlySelectedObject->pSkeletonHead)
		{
			PosX = SaveX+(14*IconWidth);
		}
		else
		{
			PosX = SaveX+(10*IconWidth);
		}
	}

	////////////////////////////////////////

	
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>0)
	{
		pField = FieldLL_Insert(
					SCENE_TYPE,
					"Console Message",
					pTextLine1,
					PosX,
					PosY,
					MainMenuDataWidth,
					MainMenuDataHeight,
					Screen,
					0.5,
					0.0,0.0,
					TextColor_Scene,
					TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
			 		0,NULL,NULL,NULL,NULL,
					&AppTools_Fonts[1],
					pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[1]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+(Width);
		pField->Screen.top    = (PosY-PreStep)-9;
		pField->Screen.bottom = (PosY-PreStep)+(IconWidth)-9;
		//if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}

	////////////////////////
	//// Console Line 0 ////
	////////////////////////
	PosX       = SaveX+(12*IconWidth);

	////////////////////////////////////////
	//// Extra Offset If Too Many Icons ////
	////////////////////////////////////////
	// UP ABOVE ALSO !! ! ! !
	// UP ABOVE ALSO !! ! ! !

	if(CurrentMouseMode==MOUSE_MODE_OBJECT)
	{//cant rotate, so move text over. no lmb, rmb buttons
	 //also extra room is needed, for warning message
	 //	PosX = SaveX+(10*IconWidth);
	}					
	if(CurrentMouseMode==MOUSE_MODE_MESH)
	{//cant rotate, so move text over. no lmb, rmb buttons
	 //also extra room is needed, for warning message
		if(	 pCurrentlySelectedMesh &&
		     pCurrentlySelectedMesh->RenderFlags & RENDER_TRANSFORMED)
		{
			PosX = SaveX+(10*IconWidth);
		}
	}					

	if(CurrentMouseMode==MOUSE_MODE_JOINT)
	{//more icons. vcr control buttons
		if(pCurrentlySelectedObject && pCurrentlySelectedObject->pSkeletonHead)
		{
			PosX = SaveX+(20*IconWidth);
		}
		else
		{
			PosX = SaveX+(10*IconWidth);
		}
	}

	if(CurrentMouseMode==MOUSE_MODE_WEIGHT)
	{//more icons. select one, select many, lmb rmb
		if(pCurrentlySelectedObject && pCurrentlySelectedObject->pSkeletonHead)
		{
			PosX = SaveX+(14*IconWidth);
		}
		else
		{
			PosX = SaveX+(10*IconWidth);
		}
	}

	////////////////////////////////////////
	
	
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>0)
	{
		pField = FieldLL_Insert(
					SCENE_TYPE,
					"Console Message",
					pTextLine2,
					PosX,
					PosY,
					MainMenuDataWidth,
					MainMenuDataHeight,
					Screen,
					0.5,
					0.0,0.0,
					TextColor_Scene,
					TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
			 		0,NULL,NULL,NULL,NULL,
					&AppTools_Fonts[1],
					pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[1]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+(Width);
		pField->Screen.top    = (PosY-PreStep)+9;
		pField->Screen.bottom = (PosY-PreStep)+(IconWidth)+9;
		//if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosY      += StepY;


}






//sleepless
void RenderMainMenuText()
{
	struct SceneLL* pScene = NULL;
	struct FieldLL* pField = NULL;
	float SaveX,SaveY;
	float PosX,PosY;
	float ClipTop;
	float ClipBottom;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
	float Width;
	fRECT Screen = {0,0,0,0};

	PosX  = 0;//IconWidth;
	PosY  = StepY;//TempHearchyY;

	SaveX = PosX;
	SaveY = PosY;


	////////////////////////
	//// Console Line 1 ////
	////////////////////////
	PosX       = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>0)
	{
		pField = FieldLL_Insert(
					SCENE_TYPE,
					"Console Message",
					GetConsoleText(WrapQueueIndex(cConsoleText+0)),
					//" console > error. no parent selected. select Animation, then select NewKeyframe",
					PosX,
					PosY,
					MainMenuDataWidth,
					MainMenuDataHeight,
					Screen,
					0.5,
					0.0,0.0,
					GetConsoleTextColor(WrapQueueIndex(cConsoleText+0)),
//					TextColor_Scene,
					TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
			 		0,NULL,NULL,NULL,NULL,
					&AppTools_Fonts[1],
					pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[1]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+(Width);
		pField->Screen.top    = (PosY-PreStep)-9;
		pField->Screen.bottom = (PosY-PreStep)+(IconWidth)-9;
		//if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}

	////////////////////////
	//// Console Line 2 ////
	////////////////////////
	PosX       = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>0)
	{
		pField = FieldLL_Insert(
					SCENE_TYPE,
					"Console Message",
					GetConsoleText(WrapQueueIndex(cConsoleText+1)),
					//" console > error. filename does not exist.",
					PosX,
					PosY,
					MainMenuDataWidth,
					MainMenuDataHeight,
					Screen,
					0.5,
					0.0,0.0,
					GetConsoleTextColor(WrapQueueIndex(cConsoleText+1)),
					//TextColor_Scene,
					TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
			 		0,NULL,NULL,NULL,NULL,
					&AppTools_Fonts[1],
					pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[1]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+(Width);
		pField->Screen.top    = (PosY-PreStep)+9;
		pField->Screen.bottom = (PosY-PreStep)+(IconWidth)+9;
		//if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosY      += StepY;

	////////////////////////
	//// Console Line 3 ////
	////////////////////////
	PosX       = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>0)
	{
		pField = FieldLL_Insert(
					SCENE_TYPE,
					"Console Message",
					GetConsoleText(WrapQueueIndex(cConsoleText+2)),
					//" console > error. filename does not exist.",
					PosX,
					PosY,
					MainMenuDataWidth,
					MainMenuDataHeight,
					Screen,
					0.5,
					0.0,0.0,
					GetConsoleTextColor(WrapQueueIndex(cConsoleText+2)),
					//TextColor_Scene,
					TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
			 		0,NULL,NULL,NULL,NULL,
					&AppTools_Fonts[1],
					pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[1]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+(Width);
		pField->Screen.top    = (PosY-PreStep)-9;
		pField->Screen.bottom = (PosY-PreStep)+(IconWidth)-9;
		//if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;

	}


	////////////////////////
	//// Console Line 4 ////
	////////////////////////
	PosX       = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>0)
	{
		pField = FieldLL_Insert(
					SCENE_TYPE,
					"Console Message",
					GetConsoleText(WrapQueueIndex(cConsoleText+3)),
					//" console > error. filename does not exist.",
					PosX,
					PosY,
					MainMenuDataWidth,
					MainMenuDataHeight,
					Screen,
					0.5,
					0.0,0.0,
					GetConsoleTextColor(WrapQueueIndex(cConsoleText+3)),
					//TextColor_Scene,
					TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
			 		0,NULL,NULL,NULL,NULL,
					&AppTools_Fonts[1],
					pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[1]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+(Width);
		pField->Screen.top    = (PosY-PreStep)+9;
		pField->Screen.bottom = (PosY-PreStep)+(IconWidth)+9;
		//if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}

	PosY  += StepY;

	///////////////////////////////////////////
	//// "Application Name : "TN Animator" ////
	///////////////////////////////////////////

	PosX  = TempHearchyX;//ADD LEFT RIGHT OFFSET HERE
	PosY  = TempHearchyY+(StepY*3);
	SaveX = PosX;
	SaveY = PosY;

	//////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////
	//// INSTANCES : Application ////
	/////////////////////////////////
	//searchfor MAINMENUMAINCALLER
	//PosX = SaveX+IconWidth;
	PosX = SaveX+IconWidth*5;
	RenderMainMenu_FunctionsText_App(PosX,PosY);

	PosX = SaveX+IconWidth*5;
	RenderMainMenu_FunctionsText_Scene(PosX,PosY);

	PosX = SaveX+IconWidth*5;
	RenderMainMenu_FunctionsText_Object(PosX,PosY);

	PosX = SaveX+IconWidth*5;
	RenderMainMenu_FunctionsText_Mesh(PosX,PosY);

	//SEPERATESKELETON TO SHOW ITS PART OF OBJ
	PosY += 12;

	PosX = SaveX+IconWidth*5;
	RenderMainMenu_FunctionsText_Skeleton(PosX,PosY);

	PosX = SaveX+IconWidth*5;
	RenderMainMenu_FunctionsText_KeyFrame(PosX,PosY);

	PosX = SaveX+IconWidth*5;
	RenderMainMenu_FunctionsText_Joint(PosX,PosY);

}


void RenderIconBar()
{
	float PosX,PosY;
	float SaveX,SaveY;
	float StepY = IconWidth;
	char pIcons[256];

	PosX  = 0;//TempHearchyX;
	PosY  = 0;//TempHearchyY;
	SaveX = PosX;
	SaveY = PosY;

	//to understand this function, 
	//See "icons.bmp"
	//See "icons.bmp"
	//See "icons.bmp"
	// SEARCHFORICONBAR
	// SEARCHFORICONBAR
	// SEARCHFORICONBAR

	//sprintf(pIcons,"%c%c%c%c%c%c%c%c",147,121,123,124,125,126,127,133);

	memset(pIcons,0,256*sizeof(char));

	if(CurrentMouseMode==MOUSE_MODE_MENU)
	{
		sprintf(pIcons,"%c%c%c%c%c%c%c%c",121,130,123,124,125,126,127,128);
	}
	if(CurrentMouseMode==MOUSE_MODE_OBJECT)
	{
		char MouseLookChar = (char)139;
		char RotateChar    = (char)137;
		char MoveChar      = (char)138;
	
		if(GetAsyncKeyState(VK_SHIFT) < 0)
		{
			MouseLookChar = (char)136;
			RotateChar    = (char)140;
		}
		if(GetAsyncKeyState(VK_CONTROL) < 0)
		{
			MouseLookChar = (char)136;
			MoveChar    = (char)141;
		}
		sprintf(pIcons,"%c%c%c%c%c%c%c%c%c%c%c%c",121,122,131,124,125,126,127,128,129,MouseLookChar,RotateChar,MoveChar);    
	}
	if(CurrentMouseMode==MOUSE_MODE_MESH)
	{
		char MouseLookChar = (char)153;
		char RotateChar    = (char)151;
		char MoveChar      = (char)152;
	
		if(GetAsyncKeyState(VK_SHIFT) < 0)
		{
			MouseLookChar = (char)150;
			RotateChar    = (char)154;
		}
		if(GetAsyncKeyState(VK_CONTROL) < 0)
		{
			MouseLookChar = (char)150;
			MoveChar    = (char)155;
		}
		if(	pCurrentlySelectedMesh &&
			pCurrentlySelectedMesh->RenderFlags & RENDER_TRANSFORMED)
		{//cant rotate
			sprintf(pIcons,"%c%c%c%c%c%c%c%c%c%c",121,122,123,132,125,126,127,128,129,MouseLookChar);    
		}
		else
		{//normal
			sprintf(pIcons,"%c%c%c%c%c%c%c%c%c%c%c%c",121,122,123,132,125,126,127,128,129,MouseLookChar,RotateChar,MoveChar);    
		}
	}
	if(CurrentMouseMode==MOUSE_MODE_JOINT)
	{
		char MouseLookChar = (char)177;
		char RotateChar    = (char)164;
		char MoveChar      = (char)165;
		char RestPoseChar  = (char)166;
	
		if(GetAsyncKeyState(VK_SHIFT) < 0)
		{
			MouseLookChar = (char)163;
			RotateChar    = (char)178;
		}
		if(GetAsyncKeyState(VK_CONTROL) < 0)
		{
			MouseLookChar = (char)163;
			MoveChar      = (char)179;
		}


		if(pCurrentlySelectedObject && pCurrentlySelectedObject->pSkeletonHead)
		{
			//              L*M*EoEmEbEwEvEtSn
			//                                MlRbMb
			//                                      << <RpS P > >> 
			//                                                    Rp
			sprintf(pIcons,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
							121,122,123,124,133,126,127,128,129,
							MouseLookChar,RotateChar,MoveChar,
							167,168,169,170,171,172,173,
							RestPoseChar);
		}
		else
		{
			sprintf(pIcons,"%c%c%c%c%c%c%c%c%c%c",
							121,122,123,124,133,126,127,128,129,
							MouseLookChar);
		}


	}
	if(CurrentMouseMode==MOUSE_MODE_WEIGHT)
	{
		char MouseLookChar = (char)177;
		char PaintManyChar = (char)142;
		char PaintOneChar  = (char)143;
		if(GetAsyncKeyState(VK_SHIFT) < 0)
		{
			MouseLookChar = (char)163;
			PaintManyChar = (char)144;
		}
		if(GetAsyncKeyState(VK_CONTROL) < 0)
		{
			MouseLookChar = (char)163;
			PaintOneChar  = (char)145;
		}

	
		if(pCurrentlySelectedObject && pCurrentlySelectedObject->pSkeletonHead)
		{
			if(	GetAsyncKeyState(VK_SHIFT) < 0 ||
				GetAsyncKeyState(VK_CONTROL) < 0)
			{//show spray weights lmb[ADD] or rmb[SUB] icons
				//              L*M*EoEmEbEwEvEtSn
				//                                MlRbMb
				//                                      M+M-
				sprintf(pIcons,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
								121,122,123,124,125,134,127,128,129,
								MouseLookChar,PaintManyChar,PaintOneChar,174,175);
			}
			else
			{//dont show spray weights lmb[ADD] or rmb[SUB] icons
				//              L*M*EoEmEbEwEvEtSn
				//                                MlRbMb
				sprintf(pIcons,"%c%c%c%c%c%c%c%c%c%c%c%c",
								121,122,123,124,125,134,127,128,129,
								MouseLookChar,PaintManyChar,PaintOneChar);
			}
		}
		else
		{
			//              L*M*EoEmEbEwEvEtSn
			//                                Ml
			sprintf(pIcons,"%c%c%c%c%c%c%c%c%c%c",
							121,122,123,124,125,134,127,128,129,
							MouseLookChar);
		}

	}								   
	if(CurrentMouseMode==MOUSE_MODE_VERT)
	{
		//              L*M*EoEmEbEwEvEtSn
		sprintf(pIcons,"%c%c%c%c%c%c%c%c%c",
						121,122,123,124,125,126,146,128,129);
	}
	if(CurrentMouseMode==MOUSE_MODE_TRI)
	{
		//              L*M*EoEmEbEwEvEtSn
		sprintf(pIcons,"%c%c%c%c%c%c%c%c%c",
						121,122,123,124,125,126,127,147,129);
	}


	//note : It doesnt matter that the above 
	//  unsigned char numbers are converted into 
	//    signed char numbers by sprintf.
	InsertIconIntoIconLL(PosX,PosY,pIcons);
}
//sleepless
void RenderMainMenuBackGround()
{
	struct RectLL* pRect = NULL;
	float PosX,PosY;
	float AppPosY;
	float ScenePosY;
	float ObjectPosY;
	float MeshPosY;
	float SkeletonPosY;
	float KeyFramePosY;
	float JointPosY;


	float SaveX,SaveY;
	float StepY = IconWidth;
	fRECT Screen2;

	//"Console" Backdrop
	Screen2.left    = 0;
	Screen2.right   = Render_WindowWidth;
	Screen2.top     = 0;
	Screen2.bottom  = IconWidth*3;
	pRect = RectLL_Insert(	0,//0console
							0,
							Screen2.left,  
							Screen2.right, 
							Screen2.top,   
							Screen2.bottom,
							pRender_RectHead);
	////////////////////////////////////////////////

	PosX  = TempHearchyX;//ADD LEFT RIGHT OFFSET HERE
	PosY  = TempHearchyY+(StepY*3);
	SaveX = PosX;
	SaveY = PosY;


	//PANE0

	float StartY = PosY;
	//clip to console window
	if(StartY<(StepY*num_console_lines))
	{
		StartY = (StepY*num_console_lines);
	}

	//searchfor MAINMENUMAINCALLER

	PosX = SaveX;
	AppPosY = PosY;
	RenderMainMenu_FunctionBackground_App(PosX,PosY);
	
	PosX = SaveX;
	ScenePosY = PosY;
	RenderMainMenu_FunctionBackground_Scene(PosX,PosY);

	PosX = SaveX;
	ObjectPosY = PosY;
	RenderMainMenu_FunctionBackground_Object(PosX,PosY);

	PosX = SaveX;
	MeshPosY = PosY;
	RenderMainMenu_FunctionBackground_Mesh(PosX,PosY);


	//SEPERATESKELETON TO SHOW ITS PART OF OBJ
	PosY += 12;
 	
	PosX = SaveX;
	SkeletonPosY = PosY;
	RenderMainMenu_FunctionBackground_Skeleton(PosX,PosY);

	PosX = SaveX;
	KeyFramePosY = PosY;
	RenderMainMenu_FunctionBackground_KeyFrame(PosX,PosY);

	PosX = SaveX;
	JointPosY = PosY;
	RenderMainMenu_FunctionBackground_Joint(PosX,PosY);

	PosX = SaveX;
	
	//searchfor:BEVELEDGE

	/////////////
	//// APP ////
	/////////////

	Screen2.left    = PosX+(IconWidth*0);
	Screen2.right   = Render_WindowWidth;
	Screen2.top     = AppPosY+1;
	Screen2.bottom  = PosY+2;
	if(Screen2.top<(StepY*num_console_lines))Screen2.top = (StepY*num_console_lines);
	if(Screen2.bottom>Screen2.top)
	{
		pRect = RectLL_Insert(1,1,Screen2.left,Screen2.right,Screen2.top,Screen2.bottom,pRender_RectHead);
	}


	///////////////
	//// SCENE ////
	///////////////

	Screen2.top     = ScenePosY;
	Screen2.bottom  = PosY;
	if(Screen2.top<(StepY*num_console_lines))Screen2.top = (StepY*num_console_lines);
	if(Screen2.bottom>Screen2.top)
	{
		pRect = RectLL_Insert(2,2,Screen2.left,Screen2.right,Screen2.top,Screen2.bottom,pRender_RectHead);
	}


	////////////////
	//// OBJECT ////
	////////////////

	Screen2.top     = ObjectPosY;
	Screen2.bottom  = PosY;
	if(Screen2.top<(StepY*num_console_lines))Screen2.top = (StepY*num_console_lines);
	if(Screen2.bottom>Screen2.top)
	{
		pRect = RectLL_Insert(3,3,Screen2.left,Screen2.right,Screen2.top,Screen2.bottom,pRender_RectHead);
	}

	//////////////
	//// MESH ////
	//////////////

	Screen2.top     = MeshPosY;
	Screen2.bottom  = SkeletonPosY-9;

	if(Screen2.top<(StepY*num_console_lines))Screen2.top = (StepY*num_console_lines);
	if(Screen2.bottom>Screen2.top)
	{
		pRect = RectLL_Insert(4,4,Screen2.left,Screen2.right,Screen2.top,Screen2.bottom,pRender_RectHead);
	}


	//////////////////
	//// SKELETON ////
	//////////////////

	Screen2.top     = SkeletonPosY-1;//+4;//+1;//MENUHACKHACK create seam between mesh, and skeleton which are on same level, but seperate.
	Screen2.bottom  = PosY;
	if(Screen2.top<(StepY*num_console_lines))Screen2.top = (StepY*num_console_lines);
	if(Screen2.bottom>Screen2.top)
	{
		pRect = RectLL_Insert(9,9,Screen2.left,Screen2.right,Screen2.top,Screen2.bottom,pRender_RectHead);
	}

	//////////////////
	//// KEYFRAME ////
	//////////////////

	Screen2.top     = KeyFramePosY+1;
	Screen2.bottom  = PosY;
	if(Screen2.top<(StepY*num_console_lines))Screen2.top = (StepY*num_console_lines);
	if(Screen2.bottom>Screen2.top)
	{
		pRect = RectLL_Insert(10,10,Screen2.left,Screen2.right,Screen2.top,Screen2.bottom,pRender_RectHead);
	}

	///////////////
	//// JOINT ////
	///////////////

	Screen2.top     = JointPosY+1;
	Screen2.bottom  = PosY;
	if(Screen2.top<(StepY*num_console_lines))Screen2.top = (StepY*num_console_lines);
	if(Screen2.bottom>Screen2.top)
	{
		pRect = RectLL_Insert(11,11,Screen2.left,Screen2.right,Screen2.top,Screen2.bottom,pRender_RectHead);
	}

}




//we know :
//last point with lmouse click
//current mouse position
//abcdef
void UpdateMainMenu(bool left_mouse_button_down)
{
	//remove text fields that are re-created every frame
	//the Hiearchy can be expanded, and collapsed, so it can be totally different the next frame.
	FieldLL_RemoveVolatile(pRender_FieldHead);
	IconLL_RemoveALL(pRender_IconHead);//eeeee
	RectLL_RemoveALL(pRender_RectHead);

	pSelectedField = NULL;
	pCurrentlySelectedFlags = NULL;
//	CurrentNumFields=0;
//	CurrentNumIcons = 0;
	DataViewMinLeft = FLT_MAX;
	DataViewMaxRight = 0;
	bWM_LBUTTONDOWN = left_mouse_button_down;

	RenderIconBar();


	if(	CurrentMouseMode==MOUSE_MODE_OBJECT ||
		CurrentMouseMode==MOUSE_MODE_MESH ||
		CurrentMouseMode==MOUSE_MODE_JOINT ||
		CurrentMouseMode==MOUSE_MODE_VERT ||
		CurrentMouseMode==MOUSE_MODE_WEIGHT)
	{
		RenderIconBarCurrentObjectText();
	}
	if(CurrentMouseMode==MOUSE_MODE_MENU)
	{
		RenderMainMenuBackGround();
		RenderMainMenuText();
	}

	//convert rects to Icons for rendering
	ConvertRectLLintoIconLL(pRender_RectHead,pRender_IconHead);
	RectLL_RemoveALL(pRender_RectHead);

}
	









//Clips line to valid main menu window
//console is above main menu window
struct FieldLL* ClipInsertMenuItem(char* pText,float PosX,float PosY,long Color)
{
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float ClipTop;
	float ClipBottom;
	float StepY = IconWidth;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		return InsertMenuItem(pText,PosX,PosY,Color);
	}
	return NULL;
}





struct FieldLL* InsertHalfHeightMenuItemTop(char* pText,float PosX,float PosY,long Color)
{
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float ClipTop;
	float ClipBottom;
	float StepY = IconWidth;

	fRECT Screen = {0,0,0,0};
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	float Width;
	float posy = 0.0;//not used


//	pField = InsertMenuItem2(pText,PosX,PosY-9,Color);



	ClipTop    = (PosY-PreStep);//top to mid
	ClipBottom = (PosY-PreStep)+IconWidth/2;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		if(ClipTop<(StepY*num_console_lines))
		{
			posy = (StepY*num_console_lines)-ClipTop + 9;
			ClipTop = (StepY*num_console_lines);
		}
		else
		{
			posy = 0;
			ClipTop    = (PosY-PreStep)-9;
		}

		pField = FieldLL_Insert(
					SCENE_TYPE,
					"Console Message",
					pText,
					//" console > error. no parent selected. select Animation, then select NewKeyframe",
					PosX,
					PosY,
					MainMenuDataWidth,
					MainMenuDataHeight,
					Screen,
					0.5,
					0.0,posy,
					Color,
					TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
			 		0,NULL,NULL,NULL,NULL,
					&AppTools_Fonts[1],
					pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[1]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+(Width);
//		pField->Screen.top    = (PosY-PreStep)-9;
		pField->Screen.top    = ClipTop;
		pField->Screen.bottom = (PosY-PreStep)+(IconWidth)-9;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;

		return pField;
	}
	return NULL;
}
struct FieldLL* InsertHalfHeightMenuItemBottom(char* pText,float PosX,float PosY,long Color)
{
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float ClipTop;
	float ClipBottom;
	float StepY = IconWidth;
	fRECT Screen = {0,0,0,0};
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	float Width;
	float posy = 0.0;

	ClipTop    = (PosY-PreStep)+IconWidth/2;//mid to bottom
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		if(ClipTop<(StepY*num_console_lines))
		{
			posy = (StepY*num_console_lines)-ClipTop +9;
			ClipTop = (StepY*num_console_lines);
		}
		else
		{
			posy = 0.0;
			ClipTop    = (PosY-PreStep)+9;
		}

		pField = FieldLL_Insert(
					SCENE_TYPE,
					"Console Message",
					pText,
					//" console > error. no parent selected. select Animation, then select NewKeyframe",
					PosX,
					PosY,
					MainMenuDataWidth,
					MainMenuDataHeight,
					Screen,
					0.5,
					0.0,posy,
					Color,
					TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
			 		0,NULL,NULL,NULL,NULL,
					&AppTools_Fonts[1],
					pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[1]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+(Width);
		pField->Screen.top    = ClipTop;
		//pField->Screen.top    = (PosY-PreStep)+9;
		pField->Screen.bottom = (PosY-PreStep)+(IconWidth)+9;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
		return pField;
	}
	return NULL;
}


struct FieldLL* InsertMenuItem(char* pText,float PosX,float PosY,long Color)
{
	struct FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float ClipTop;
	float ClipBottom;
	float Width;
	fRECT Screen = {0,0,0,0};

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>0)
	{
		pField = FieldLL_Insert(SCENE_TYPE,"Debug Value",pText,PosX,PosY,
						MainMenuDataWidth,MainMenuDataHeight,Screen,
						1.0,0.0,0.0,Color,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
				 		0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
		return pField;
	}
	return NULL;
}








//Skeleton, Keyframe, and Joint are offset Y due to beveled color panes.
//most text is either visible, or not, however their text will be clipped in half
struct FieldLL* InsertMenuItem2(char* pText,float PosX,float PosY,long Color)
{
	struct FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float ClipTop;
	float ClipBottom;
	float Width;
	fRECT Screen = {0,0,0,0};
	float StepY = IconWidth;
	float posy = 0.0;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		if(ClipTop<(StepY*num_console_lines))
		{
			posy = (StepY*num_console_lines)-ClipTop;
			ClipTop = (StepY*num_console_lines);
		}

		pField = FieldLL_Insert(SCENE_TYPE,"Debug Value",pText,PosX,PosY,
						MainMenuDataWidth,MainMenuDataHeight,Screen,
						1.0,0.0,posy,Color,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
				 		0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = ClipTop;
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
		return pField;
	}
	return NULL;
}

struct FieldLL* InsertEditableMenuItem(
				char* pText,
				float PosX,
				float PosY,
				float Width,//width in pixels of editable field.
				long Color,
				float* pfloat,
				unsigned long* plong,
				unsigned short* pword,
				char* pchar)

{
	struct FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float ClipTop;
	float ClipBottom;
	float StepY = IconWidth;
//	float Width;
	fRECT Screen = {0,0,0,0};

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
 		ClipBottom>(StepY*num_console_lines))
	{
		//NIPNIP
		pField = FieldLL_Insert(SCENE_TYPE,"Debug Value",pText,PosX,PosY,
//						MainMenuDataWidth,MainMenuDataHeight,Screen,
						220,36,Screen,
						1.0,0.0,0.0,Color,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE|TEXT_FLAG_EDITABLE,
						0,pfloat,plong,pword,pchar,
						&AppTools_Fonts[0],
						pRender_FieldHead);
//						1.0,0.0,TextColor_Editable,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE|TEXT_FLAG_EDITABLE,0,pfloat,pword,pchar,pRender_FieldHead);

//abcdefgh
//		Width = 220;//CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep)+1;
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		PickDataViewField(LastSelectedPoint,pField);
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
		return pField;
	}
	return NULL;
}



//new clips skeleton which is offset from regular IconWidth increments
//now clips to console for any Y, not just IconWitdth increments
struct FieldLL* InsertEditableMenuItem2(
				char* pText,
				float PosX,
				float PosY,
				float Width,//width in pixels of editable field.
				long Color,
				float* pfloat,
				unsigned long* plong,
				unsigned short* pword,
				char* pchar)

{
	struct FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float ClipTop;
	float ClipBottom;
	float StepY = IconWidth;
	float posy = 0.0;
//	float Width;
	fRECT Screen = {0,0,0,0};

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
 		ClipBottom>(StepY*num_console_lines))
	{
		//NIPNIP

		if(ClipTop<(StepY*num_console_lines))
		{
			posy = (StepY*num_console_lines)-ClipTop;
			ClipTop = (StepY*num_console_lines);
		}


		pField = FieldLL_Insert(SCENE_TYPE,"Debug Value",pText,PosX,PosY,
//						MainMenuDataWidth,MainMenuDataHeight,Screen,
						220,36,Screen,
						1.0,0.0,posy,Color,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE|TEXT_FLAG_EDITABLE,
						0,pfloat,plong,pword,pchar,
						&AppTools_Fonts[0],
						pRender_FieldHead);
//						1.0,0.0,TextColor_Editable,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE|TEXT_FLAG_EDITABLE,0,pfloat,pword,pchar,pRender_FieldHead);

//abcdefgh
//		Width = 220;//CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = ClipTop+1;
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		PickDataViewField(LastSelectedPoint,pField);
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
		return pField;
	}
	return NULL;
}




void RenderMainMenu_RenderFlagsText(	
				unsigned long ViewFlags, 
				unsigned long flag, 
				char* pItems[],
				long TextColor, 
				float SaveX,
				float& PosX, 
				float& PosY)
{
	long i;
	float Width = 0.0;
	fRECT Screen = {0,0,0,0};
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.


	//////////////////////
	//// Output Flags ////
	//////////////////////

	PosX = SaveX+IconWidth*2;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(OBJECT_TYPE,"Debug Name",
						"flags",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
				 		0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;

	if(IsBitSet(ViewFlags,flag))//expand/contract
	{
		for(i=0;i<(atoi(pItems[0]));i++)
		{
			PosX = SaveX+(IconWidth*3);
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{
				pField = FieldLL_Insert(OBJECT_TYPE,"Debug Name",
								pItems[i+1],
								PosX,PosY,
								MainMenuDataWidth,
								MainMenuDataHeight,Screen,1.0,0.0,0.0,
								TextColor,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						 		0,NULL,NULL,NULL,NULL,
								&AppTools_Fonts[0],
								pRender_FieldHead);
				Width = CalcWidthField(pField,&AppTools_Fonts[0]);
				pField->Screen.left   = PosX;
				pField->Screen.right  = PosX+Width;
				pField->Screen.top    = (PosY-PreStep);
				pField->Screen.bottom = (PosY-PreStep)+IconWidth;
				if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
				if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
			}
			PosX = SaveX;
			PosY+=IconWidth;
		}
	}
}




//Copy RectLL data into IconLL data
//Rects define the "smoked glass" behind menu
// 	originaly, the rects were clipped against eachother to form 1 layer
//	now, the z buffer is used to get a single layer from overlapped rects.

//void NoClipNoOverlapRectLLaddToIconLL(struct RectLL* pInitialLL,
void ConvertRectLLintoIconLL(	struct RectLL* pInitialLL,
								struct IconLL*& pIconHeadIn)
{
	int Zero = 0;
	int One  = 1;
	fRECT Screen = {0,0,0,0};
	fRECT ColorFadeRects[100];
	struct RectLL* pInitialRect	 = NULL;

	struct RectLL* pShadowedLL	 = NULL;
	struct RectLL* pShadowerRect = NULL;
	struct RectLL* pShadowedRect = NULL;

	struct RectLL* pVisibleLL	 = NULL;

	struct RectLL* pRect	 = NULL;

	struct IconLL* pIcon = NULL;
	struct RectLL* pRemoveMe = NULL;

	int i;
	int num_items = 12;
	float a = 64;
	float b = 128;
	float currenty;
	for(i=0;i<num_items;i++)
	{
		currenty = (float)(a+i)+0.5;
		ColorFadeRects[i].left   = ((256+8.0)/512.0);
		ColorFadeRects[i].right  = ((256+8.0)/512.0);
		ColorFadeRects[i].top    = (currenty /512.0);
		ColorFadeRects[i].bottom = (currenty /512.0);
	}

	///////////////////////////////////
	///////////////////////////////////
	/*
	if(DISK)DiskFile=fopen("debug.log","w");
	if(DISK)fprintf(DiskFile,"Tanzanite Software\n");
	pInitialRect = pInitialLL;
	while(pInitialRect)
	{
		if(DISK)fprintf(DiskFile,"Initial[%d] %d %d %d %d\n",pInitialRect->SortKey,(int)pInitialRect->left,(int)pInitialRect->right,(int)pInitialRect->top,(int)pInitialRect->bottom);
		pInitialRect = pInitialRect->pNext;
	}
	*/
	///////////////////////////////////
	///////////////////////////////////



	//limit right side//abcdef
	pInitialRect = pInitialLL;


	while(pInitialRect)
	{
		if(1)//pInitialRect->right>DataViewMaxRight)
		{ 
			//BEVELEDGE
			//			pInitialRect->right = DataViewMaxRight + (2-pInitialRect->SortKey)*4;
			//applicaation +12     0
			//scene        +8	   1
			//object       +4	   2
			//mesh         +0	   3
			//                     4
			//console              5

			//console	   	0	   	0
			//App		   	1	   	+15
			//Scene		   	2	   	+12
			//Object	   	3	   	+9
			//Mesh		   	4	   	+6
			//Skeleton	   	9	   	+6
			//KeyFrame	   	0	   	+3
			//Joint		   	1	   	+3

			int BevelScale = 6;
			
			//TOP is not beveled

			//LEFT BEVEL			 
			if(pInitialRect->SortKey==1 )pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*5);
			if(pInitialRect->SortKey==2 )pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*4);
			if(pInitialRect->SortKey==3 )pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*3);
			if(pInitialRect->SortKey==4 )pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*2);
			if(pInitialRect->SortKey==5 )pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*1);
			if(pInitialRect->SortKey==6 )pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*1);
			if(pInitialRect->SortKey==7 )pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*1);
			if(pInitialRect->SortKey==8 )pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*1);
			if(pInitialRect->SortKey==9 )pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*2);
			if(pInitialRect->SortKey==10)pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*1);
			if(pInitialRect->SortKey==11)pInitialRect->left = DataViewMinLeft - (IconWidth*6) - (BevelScale*0);


			//RIGHT BEVEL			 
			if(pInitialRect->SortKey==0 )pInitialRect->right = Render_WindowWidth;
			if(pInitialRect->SortKey==1 )pInitialRect->right = DataViewMaxRight + (BevelScale*5);
			if(pInitialRect->SortKey==2 )pInitialRect->right = DataViewMaxRight + (BevelScale*4);
			if(pInitialRect->SortKey==3 )pInitialRect->right = DataViewMaxRight + (BevelScale*3);
			if(pInitialRect->SortKey==4 )pInitialRect->right = DataViewMaxRight + (BevelScale*2);
			if(pInitialRect->SortKey==5 )pInitialRect->right = DataViewMaxRight + (BevelScale*1);
			if(pInitialRect->SortKey==6 )pInitialRect->right = DataViewMaxRight + (BevelScale*1);
			if(pInitialRect->SortKey==7 )pInitialRect->right = DataViewMaxRight + (BevelScale*1);
			if(pInitialRect->SortKey==8 )pInitialRect->right = DataViewMaxRight + (BevelScale*1);
			if(pInitialRect->SortKey==9 )pInitialRect->right = DataViewMaxRight + (BevelScale*2);
			if(pInitialRect->SortKey==10)pInitialRect->right = DataViewMaxRight + (BevelScale*1);
			if(pInitialRect->SortKey==11)pInitialRect->right = DataViewMaxRight + (BevelScale*0);
																				

			//RIGHT BEVEL			 
			if(pInitialRect->SortKey==1 )pInitialRect->bottom += (BevelScale*5);
			if(pInitialRect->SortKey==2 )pInitialRect->bottom += (BevelScale*4);
			if(pInitialRect->SortKey==3 )pInitialRect->bottom += (BevelScale*3);
			if(pInitialRect->SortKey==9 )pInitialRect->bottom += (BevelScale*2);
			if(pInitialRect->SortKey==10)pInitialRect->bottom += (BevelScale*1);
/*
			if(pInitialRect->SortKey==4 )pInitialRect->bottom += (BevelScale*2);
			if(pInitialRect->SortKey==5 )pInitialRect->bottom += (BevelScale*1);
			if(pInitialRect->SortKey==6 )pInitialRect->bottom += (BevelScale*1);
			if(pInitialRect->SortKey==7 )pInitialRect->bottom += (BevelScale*1);
			if(pInitialRect->SortKey==8 )pInitialRect->bottom += (BevelScale*1);
			if(pInitialRect->SortKey==9 )pInitialRect->bottom += (BevelScale*2);
			if(pInitialRect->SortKey==10)pInitialRect->bottom += (BevelScale*1);
			if(pInitialRect->SortKey==11)pInitialRect->bottom += (BevelScale*1);
*/

		}
		pInitialRect = pInitialRect->pNext;
	}



	pRect = pInitialLL;
	while(pRect)
	{
			Screen.left   = pRect->left;
		Screen.right  = pRect->right;
		Screen.top    = pRect->top;
		Screen.bottom = pRect->bottom;
		if(DISK)fprintf(DiskFile," Screen[%d] %d %d %d %d\n",pInitialRect->SortKey,(int)Screen.left,(int)Screen.right,(int)Screen.top,(int)Screen.bottom);
		pIcon = IconLL_Insert(	"Clipped Rect",
								0,
								Screen,
								ColorFadeRects[pRect->ColorIndex],
								RENDER_VISIBLE,
								pIconHeadIn);//ICON!!!
		pRect = pRect->pNext;
	}
	if(DISK)fclose(DiskFile);						
}



//void SubtractRectFromRectLL(struct RectLL* pRect,struct RectLL* pSrcHead,struct RectLL* pOutsideHead)
void ClipRectX(	struct RectLL* pRect,
				float x,
				int dir,
				struct RectLL*& pVisibleLLHead)
{

	if(dir>0)
	{
		if(x<=pRect->left)
		{//total out. should have already been removed
			return;
		}
		else if(x>=pRect->right)
		{//no change
			return;
		}
		else
		{
			//right of plane is visible
			RectLL_Insert(	pRect->SortKey,
									0,
							x,  
							pRect->right, 
							pRect->top,   
							pRect->bottom,
							pVisibleLLHead);
			//adjust shaded rect
			pRect->right = x;
		}
	}
	else
	{
		if(x<=pRect->left)
		{//no change
			return;
		}
		else if(x>=pRect->right)
		{//total out. should have already been removed
			return;
		}
		else
		{
			//left of plane is visible
			RectLL_Insert(	pRect->SortKey,
									0,
							pRect->left, 
							x,  
							pRect->top,   
							pRect->bottom,
							pVisibleLLHead);
			//adjust shaded rect
			pRect->left = x;
		}
	}
}
void ClipRectY(	struct RectLL* pRect,
				float y,
				int dir,
				struct RectLL*& pVisibleLLHead)
{
	if(dir>0)
	{
		if(y<=pRect->top)
		{//total out. should have already been removed
			return;
		}
		else if(y>=pRect->bottom)
		{//no change
			return;
		}
		else
		{
			//right of plane is visible
			RectLL_Insert(	pRect->SortKey,
									0,
							pRect->left, 
							pRect->right, 
							y,   
							pRect->bottom,
							pVisibleLLHead);
			//adjust shaded rect
			pRect->bottom = y;
		}
	}
	else
	{
		if(y<=pRect->top)
		{//total out. should have already been removed
			return;
		}
		else if(y>=pRect->bottom)
		{//no change
			return;
		}
		else
		{
			//left of plane is visible
			RectLL_Insert(	pRect->SortKey,
									0,
							pRect->left, 
							pRect->right, 
							pRect->top,   
							y,
							pVisibleLLHead);
			//adjust shaded rect
			pRect->top = y;
		}
	}
}


















/*
void a()/////////////////////////////////////////////////
void a()//// RENDER MAINMENU BG, AND TEXT FUNCTIONS /////
void a()/////////////////////////////////////////////////
void a()//////////////////////////////
void a()//// InstancesBackground /////
void a()//////////////////////////////
*/
void RenderMainMenu_InstancesBackground_App(struct AppItem* pApp,float& PosX, float& PosY)
{
	struct ObjectLL* pObject = NULL;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
	char pIcons[256];
	long i;

	SaveX = PosX;
	SaveY = PosY;

	/////////////////////
	//// View Matrix ////
	/////////////////////

	PosX = SaveX;
	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bShow_ViewMatrix,1,false);
	if(bShow_ViewMatrix)
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;

		for(i=0;i<4;i++)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
					ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnfn%s",MatrixIcons[i]);
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;
		}
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;
	}
	/////////////////////
	//// Proj Matrix ////
	/////////////////////

	PosX = SaveX;
	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bShow_ProjMatrix,1,false);
	if(bShow_ProjMatrix)
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;

		for(i=0;i<4;i++)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
					ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnfn%s",MatrixIcons[i]);
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;
		}
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;
	}

	//////////////////////
	//// Ortho Matrix ////
	//////////////////////

	PosX = SaveX;
	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bShow_OrthoMatrix,1,false);
	if(bShow_OrthoMatrix)
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;

		for(i=0;i<4;i++)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
					ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnfn%s",MatrixIcons[i]);
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;
		}
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;
	}








	#ifdef TechNaturePrivate
	////////////////////
	//// New Planet ////
	////////////////////

	PosX = SaveX;
	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bShow_NewPlanet,1,false);
	if(bShow_NewPlanet)
	{
		Planet_RenderMenu_Background(PosX,PosY);
		/*
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;

		for(i=0;i<4;i++)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
					ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnnn%s",MatrixIcons[i]);
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;
		}
		*/
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;
	}

	#endif	



	//////////////////////////////
	//// Output App_BoneScale ////
	//////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");//LastItemReplace c with g
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;

	///////////////////////////////
	//// Output App_JointScale ////
	///////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;

	////////////////////////////////
	//// Output App_SprayRadius ////
	////////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;

	//////////////////////////////////////////
	//// Output App_PickSphereRadius ////
	//////////////////////////////////////////
	/*
	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnghh");//LastItemReplace c with g
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;
	*/
	////////////////////////////////////////////
	//// Output App_CameraPositionSpeed ////
	////////////////////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");//LastItemReplace c with g
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;
	////////////////////////////////////////////
	//// Output App_CameraRotationSpeed ////
	////////////////////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");//LastItemReplace c with g
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;



	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Flag Picking ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

	PosX = SaveX;

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&App_DataViewFlags,APP_DATAVIEW_SHOW_FLAGS,false);
	if(IsBitSet(App_DataViewFlags,APP_DATAVIEW_SHOW_FLAGS))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnghd");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////SHOW_JOINTS
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&App_RenderFlags,APP_SHOWJOINTS,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(App_RenderFlags,APP_SHOWJOINTS))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////SHOW_BONES
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&App_RenderFlags,APP_SHOWBONES,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(App_RenderFlags,APP_SHOWBONES))
				sprintf(pIcons,"nnnnnngk");
			else
				sprintf(pIcons,"nnnnnngl");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnghe");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}
	PosX = SaveX;

}

void RenderMainMenu_InstancesBackground_Scene(struct SceneLL* pScene, float& PosX, float& PosY)
{
	struct ObjectLL* pObject = NULL;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
	char pIcons[256];

	if(pScene==NULL)return;

	SaveX = PosX;
	SaveY = PosY;

	////////////////////////////
	//// Output Scene pName ////
	////////////////////////////

//	PickDataView2(LastSelectedPoint,PosY,PosY+IconWidth,pScene,NULL,NULL,&pScene->SceneDataViewFlags,SCENE_DATAVIEW_SHOW_THIS_SCENE,false);

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL(PosX,PosY,pIcons);
	}

	//////////////////////////////
	//// Output Scene SortKey ////
	//////////////////////////////

	PosY+=IconWidth;
	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnghh");//LastItemReplace c with g
		InsertIconIntoIconLL(PosX,PosY,pIcons);
	}

	PosY+=IconWidth;
	PosX = SaveX;
/*
	////////////////////////////
	//// Output Scene pView ////
	////////////////////////////
	//////////////////////////////////
	//// Output Scene pObjectHead ////
	//////////////////////////////////

	PosX = SaveX;
	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pScene->SceneDataViewFlags,SCENE_DATAVIEW_SHOW_OBJECTHEAD,false);
	if(IsBitSet(pScene->SceneDataViewFlags,SCENE_DATAVIEW_SHOW_OBJECTHEAD))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
//			sprintf(pIcons,"cd");
//			InsertIconIntoIconLL(PosX,PosY,pIcons);
//			sprintf(pIcons,"%scd",pIcons2);
			sprintf(pIcons,"ffcd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}

		PosY+=IconWidth;

		PosX = SaveX;
		RenderMainMenu_FunctionBackground_Object(PosX,PosY);
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			//sprintf(pIcons,"%sge",pIcons2);
			sprintf(pIcons,"ffge");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;
	}
	PosX = SaveX;
*/
}






void RenderMainMenu_InstancesBackground_Object(struct ObjectLL* pObject, float& PosX, float& PosY)
{
	char pIcons[16];
	char* stopstring = NULL;
	long i,index;
	float StartPosX = PosX;
	float StartPosY	= PosY;
	float MatrixElementWidth = 0.0;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	float StepY = IconWidth;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
//	fRECT TextureBlank;
//	fRECT Texture[16];
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);

	if(pObject==NULL)return;

/*
	
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			index = j*4+i;
			Texture[index].left    = (float)(i+0)*(64.0/256.0);
			Texture[index].right   = (float)(i+1)*(64.0/256.0);
			Texture[index].top     = (float)(j+0)*(64.0/256.0);
			Texture[index].bottom  = (float)(j+1)*(64.0/256.0);
		}
	}

	TextureBlank.left   = 0.0/256.0;
	TextureBlank.right  = 1.0/256.0;
	TextureBlank.top    = 0.0/256.0;
	TextureBlank.bottom = 1.0/256.0;
*/

	SaveX = PosX;
	SaveY = PosY;

	/////////////////////////////
	//// Output Object pName ////
	/////////////////////////////
/*
//	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_THIS_OBJECT,false);
	PickDataView2(LastSelectedPoint,PosY,PosY+IconWidth,NULL,pObject,NULL,&pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_THIS_OBJECT,false);
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		if(IsBitSet(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_THIS_OBJECT))
		{
			sprintf(pIcons,"fffcd");

			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		else
		{
			sprintf(pIcons,"fffce");
			InsertIconIntoIconLL(PosX,PosY,pIcons);//a!!!
		}
	}
	PosY+=IconWidth;
	PosX = SaveX;

	if(!IsBitSet(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_THIS_OBJECT))
	{
		return;
	}
*/
	////////////////////////////
	//// Output Scene pName ////
	////////////////////////////

//	PickDataView2(LastSelectedPoint,PosY,PosY+IconWidth,pScene,NULL,NULL,&pScene->SceneDataViewFlags,SCENE_DATAVIEW_SHOW_THIS_SCENE,false);

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;
	PosX = SaveX;
	///////////////////////////////
	//// Output Object SortKey ////
	///////////////////////////////

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;
	PosX = SaveX;

	///////////////////////////////
	//// Output Object pMatrix ////
	///////////////////////////////

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_MATRIX,false);
	if(IsBitSet(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_MATRIX))
	{//if matrix is expanded so that elements are visible
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;

		for(i=0;i<4;i++)
		{//4 rows
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnfn%s",ObjectLLMatrix[i]);
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;
		}
	}
	else
	{//output name of matrix only. 
//			Length = strlen(ObjectLLConnectionTypes[index]);
//			sprintf(pIcons,"%s",ObjectLLConnectionTypes[index]);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");//NOT _> LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;
	}
/*
	/////////////////////////////////
	//// Output Object pMeshHead ////
	/////////////////////////////////

	index = 4;
	PosX = SaveX;
	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_MESHHEAD,false);
	if(IsBitSet(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_MESHHEAD))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnncd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;
		RenderMainMenu_FunctionBackground_Mesh(PosX,PosY);
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnce");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;
	}
	PosX = SaveX;


	//////////////////////////////////
	//// Output Objects pSkeleton ////
	//////////////////////////////////

	index = 4;
	PosX = SaveX;
	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_SKELETON,false);
	if(IsBitSet(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_SKELETON))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnncd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;
		RenderMainMenu_FunctionBackground_Skeleton(PosX,PosY);
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnce");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosY+=IconWidth;
		PosX = SaveX;
	}
	PosX = SaveX;
*/







	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Flag Picking ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

	index = 11;

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_FLAGS,false);
	if(IsBitSet(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_FLAGS))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnghd");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////// Object Flags : OBJECT_MESHES_WIREFRAME ///////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// CALL_FUNCTION_WHEN_FLAG_IS_TOGGLED
		static bool bLastOM_WIREFRAME = false;
		bool        bCurrOM_WIREFRAME = pObject->RenderFlags & OBJECT_MESHES_WIREFRAME;
		if(bCurrOM_WIREFRAME != bLastOM_WIREFRAME)
		{//user toggled the flag
			if(bCurrOM_WIREFRAME)	ObjectLL_SetAllMeshs_Flags(pObject,RENDER_WIREFRAME);
			else 			   	ObjectLL_ClearAllMeshs_Flags(pObject,RENDER_WIREFRAME);
			bLastOM_WIREFRAME = bCurrOM_WIREFRAME;
		}

		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pObject->RenderFlags,OBJECT_MESHES_WIREFRAME,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pObject->RenderFlags,OBJECT_MESHES_WIREFRAME))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////// Object Flags : OBJECT_MESHES_ALPHAVERTEX /////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// CALL_FUNCTION_WHEN_FLAG_IS_TOGGLED
		static bool bLastOM_ALPHAVERTEX = false;
		bool        bCurrOM_ALPHAVERTEX = pObject->RenderFlags & OBJECT_MESHES_ALPHAVERTEX;
		if(bCurrOM_ALPHAVERTEX != bLastOM_ALPHAVERTEX)
		{//user toggled the flag
			if(bCurrOM_ALPHAVERTEX)	ObjectLL_SetAllMeshs_Flags(pObject,RENDER_TRANSLUCENT_VERTEX);
			else 			   		ObjectLL_ClearAllMeshs_Flags(pObject,RENDER_TRANSLUCENT_VERTEX);
			bLastOM_ALPHAVERTEX = bCurrOM_ALPHAVERTEX;
		}

		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pObject->RenderFlags,OBJECT_MESHES_ALPHAVERTEX,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pObject->RenderFlags,OBJECT_MESHES_ALPHAVERTEX))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////// Object Flags : OBJECT_MESHES_ALPHAVERTEX /////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// CALL_FUNCTION_WHEN_FLAG_IS_TOGGLED
		static bool bLastOM_TEXTURED = false;
		bool        bCurrOM_TEXTURED = pObject->RenderFlags & OBJECT_MESHES_TEXTURED;
		if(bCurrOM_TEXTURED != bLastOM_TEXTURED)
		{//user toggled the flag
			if(bCurrOM_TEXTURED)	ObjectLL_SetAllMeshs_Flags(pObject,RENDER_TEXTURED);
			else 			   		ObjectLL_ClearAllMeshs_Flags(pObject,RENDER_TEXTURED);
			bLastOM_TEXTURED = bCurrOM_TEXTURED;
		}

		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pObject->RenderFlags,OBJECT_MESHES_TEXTURED,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pObject->RenderFlags,OBJECT_MESHES_TEXTURED))
				sprintf(pIcons,"nnnnnngk");
			else
				sprintf(pIcons,"nnnnnngl");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnghe");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}
	PosX = SaveX;

}




void RenderMainMenu_InstancesBackground_Mesh(struct MeshLL* pMesh, float& PosX, float& PosY)
{
	char pIcons[16];
	char* stopstring = NULL;
	long i,index;
//	float StepY = 36.0/Render_WindowHeight;
	float StartPosX = PosX;
	float StartPosY	= PosY;
	float MatrixElementWidth = 0.0;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	float StepY = IconWidth;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
//	fRECT TextureBlank;
//	fRECT Texture[16];
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);

	if(pMesh==NULL)return;

/*	
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			index = j*4+i;
			Texture[index].left    = (float)(i+0)*(64.0/256.0);
			Texture[index].right   = (float)(i+1)*(64.0/256.0);
			Texture[index].top     = (float)(j+0)*(64.0/256.0);
			Texture[index].bottom  = (float)(j+1)*(64.0/256.0);
		}
	}

	TextureBlank.left   = 0.0/256.0;
	TextureBlank.right  = 1.0/256.0;
	TextureBlank.top    = 0.0/256.0;
	TextureBlank.bottom = 1.0/256.0;
*/
	SaveX = PosX;
	SaveY = PosY;

	////////////////////////////////////
	////////////////////////////////////
	//////// Output Mesh pName /////////
	////////////////////////////////////
	////////////////////////////////////
/*
	//PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_THIS_MESH,false);
	PickDataView2(LastSelectedPoint,PosY,PosY+IconWidth,NULL,NULL,pMesh,&pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_THIS_MESH,false);
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_THIS_MESH))
		{
			sprintf(pIcons,"fffffcd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		else
		{
			sprintf(pIcons,"fffffce");
			InsertIconIntoIconLL(PosX,PosY,pIcons);//a!!!
		}
	}
	PosX = SaveX;
	PosY+=IconWidth;

	if(!IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_THIS_MESH))
	{
		return;
	}
*/

	////////////////////////////
	//// Output Mesh pName ////
	////////////////////////////

//	PickDataView2(LastSelectedPoint,PosY,PosY+IconWidth,pMesh,NULL,NULL,&pMesh->MeshDataViewFlags,Mesh_DATAVIEW_SHOW_THIS_Mesh,false);

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;
	PosX = SaveX;
	/////////////////////////////////////
	/////////////////////////////////////
	//////// Output Mesh SortKey ////////
	/////////////////////////////////////
	/////////////////////////////////////

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL(PosX,PosY,pIcons);
	}
	PosX = SaveX;
	PosY+=IconWidth;

	////////////////////////////////////
	////////////////////////////////////
	//////// Output Mesh Matrix ////////
	////////////////////////////////////
	////////////////////////////////////

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_MATRIX,false);
	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_MATRIX))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;

		for(i=0;i<4;i++)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnfn%s",MeshLLMatrix[i]);
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosX = SaveX;
			PosY+=IconWidth;
		}
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}

	///////////////////////////////////////////
	///////////////////////////////////////////
	//////// Output Mesh Local Vertices////////
	//////// pLocalVerts               ////////
	///////////////////////////////////////////
	///////////////////////////////////////////

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_LOCALVERTICES,false);
	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_LOCALVERTICES))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;

		for(i=0;i<pMesh->num_verts;i++)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				index = 0;
				if(i==(pMesh->num_verts-1))
				{
					index = 1;
				}
				sprintf(pIcons,"nnnnfn%s",MeshListIcons[index]);
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosX = SaveX;
			PosY+=IconWidth;
		}
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}

	///////////////////////////////////////////
	///////////////////////////////////////////
	//////// Output Mesh World Vertices////////
	//////// pWorldVerts               ////////
	///////////////////////////////////////////
	///////////////////////////////////////////

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_WORLDVERTICES,false);
	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_WORLDVERTICES))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;

		for(i=0;i<pMesh->num_verts;i++)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				index = 0;
				if(i==(pMesh->num_verts-1))
				{
					index = 1;
				}
				sprintf(pIcons,"nnnnfn%s",MeshListIcons[index]);
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosX = SaveX;
			PosY+=IconWidth;
		}
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}

	/////////////////////////////////////
	/////////////////////////////////////
	//////// Output Mesh Indices ////////
	//////// pIndices       ////////
	/////////////////////////////////////
	/////////////////////////////////////

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_3dINDICES,false);
	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_3dINDICES))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;

		for(i=0;i<pMesh->num_tris;i++)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				index = 0;
				if(i==(pMesh->num_tris-1))
				{
					index = 1;
				}
				sprintf(pIcons,"nnnnfn%s",MeshListIcons[index]);
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosX = SaveX;
			PosY+=IconWidth;
		}
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}

	/////////////////////////////////
	//// Output Mesh num_verts ////
	/////////////////////////////////

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL(PosX,PosY,pIcons);
	}
	PosX = SaveX;
	PosY+=IconWidth;


	/////////////////////////////////
	//// Output Mesh num_tris  ////
	/////////////////////////////////

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL(PosX,PosY,pIcons);
	}
	PosX = SaveX;
	PosY+=IconWidth;


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////// Output SrcBlend /////////////////////////////////////////////////////////////////
	//////////////// Output SrcBlend /////////////////////////////////////////////////////////////////
	//////////////// Output SrcBlend /////////////////////////////////////////////////////////////////
	//////////////// Flag Picking ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

	index = 9;

	PosX = SaveX;

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_SRCBLEND,false);
	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_SRCBLEND))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_ZERO,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_ZERO))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_ONE,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_ONE))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_SRCCOLOR,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_SRCCOLOR))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_INVSRCCOLOR,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_INVSRCCOLOR))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_SRCALPHA,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_SRCALPHA))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_INVSRCALPHA,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_INVSRCALPHA))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_DESTALPHA,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_DESTALPHA))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_INVDESTALPHA,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_INVDESTALPHA))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_DESTCOLOR,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_DESTCOLOR))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_INVDESTCOLOR,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_INVDESTCOLOR))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->SrcBlend,BLEND_SRCALPHASAT,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->SrcBlend,BLEND_SRCALPHASAT))
				sprintf(pIcons,"nnnnfngk");//LastItemReplace c with g
			else
				sprintf(pIcons,"nnnnfngl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////// Output DstBlend /////////////////////////////////////////////////////////////////
	//////////////// Output DstBlend /////////////////////////////////////////////////////////////////
	//////////////// Output DstBlend /////////////////////////////////////////////////////////////////
	//////////////// Flag Picking ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

	index = 10;

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_DSTBLEND,false);
	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_DSTBLEND))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_ZERO,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_ZERO))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_ONE,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_ONE))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_SRCCOLOR,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_SRCCOLOR))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_INVSRCCOLOR,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_INVSRCCOLOR))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_SRCALPHA,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_SRCALPHA))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_INVSRCALPHA,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_INVSRCALPHA))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_DESTALPHA,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_DESTALPHA))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_INVDESTALPHA,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_INVDESTALPHA))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_DESTCOLOR,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_DESTCOLOR))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_INVDESTCOLOR,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_INVDESTCOLOR))
				sprintf(pIcons,"nnnnfnck");
			else
				sprintf(pIcons,"nnnnfncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->DstBlend,BLEND_SRCALPHASAT,true);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->DstBlend,BLEND_SRCALPHASAT))
				sprintf(pIcons,"nnnnfngk");//LastItemReplace c with g
			else
				sprintf(pIcons,"nnnnfngl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}



/*
				sprintf(pIcons,"nnnnfnck");
				sprintf(pIcons,"nnnnfncl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfncl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");
				sprintf(pIcons,"nnnnfchk");
				sprintf(pIcons,"nnnnfchl");

*/

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Flag Picking ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

	index = 11;

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_FLAGS,false);
	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_FLAGS))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnghd");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_ORTHOGRAPHIC,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_ORTHOGRAPHIC))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_WIREFRAME,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_WIREFRAME))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_SELECTABLE,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_SELECTABLE))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_ZBUFFERREAD,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_ZBUFFERREAD))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_ZBUFFERWRITE,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_ZBUFFERWRITE))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_BILINEARFILTER,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_BILINEARFILTER))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_TRANSLUCENT_VERTEX,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_TRANSLUCENT_VERTEX))
				sprintf(pIcons,"nnnnnnck");
			else				  
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_TRANSLUCENT_TEXTURE,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_TRANSLUCENT_TEXTURE))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_TRANSPARENT_TEXTURE,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_TRANSPARENT_TEXTURE))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_LIGHTING,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_LIGHTING))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_TEXTURED,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_TEXTURED))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;

		/////////////////////////////////////////////
		/////////////////////////////////////////////

		static bool bLastRENDER_TRANSFORMED = pMesh->RenderFlags&RENDER_TRANSFORMED;//first
		bool        bCurrRENDER_TRANSFORMED = pMesh->RenderFlags&RENDER_TRANSFORMED;
		if(bCurrRENDER_TRANSFORMED != bLastRENDER_TRANSFORMED)
		{//user toggled the flag
			if(bCurrRENDER_TRANSFORMED && !bLastRENDER_TRANSFORMED)
			{//now we are transformed, then we wernt
				//provide localtoworld, in case that isnt provided.	
				//RENDER_TRANSFORMED flag implies that the data is transformed fromloacltoworld by user provided routine, ObjectLL_DeformMeshesUsingSkeleton
				//TESTING 
				MeshLL_CopyLocalToWorld(pMesh);
				Identity(pMesh->pMatrix);//too obtrusive, and damaging. it is up to user to ensure pMesh->pMat is identity for TRANSFORMED meshes
			}
			bLastRENDER_TRANSFORMED = bCurrRENDER_TRANSFORMED;
		}

		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_TRANSFORMED,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_TRANSFORMED))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;

		/////////////////////////////////////////////
		/////////////////////////////////////////////
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pMesh->RenderFlags,RENDER_VISIBLE,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pMesh->RenderFlags,RENDER_VISIBLE))
				sprintf(pIcons,"nnnnnngk");//LastItemReplace c with g
			else
				sprintf(pIcons,"nnnnnngl");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnghe");
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}
	PosX = SaveX;
}
/*

				sprintf(pIcons,"nnnnngk");
			else
				sprintf(pIcons,"nnnnngl");
				sprintf(pIcons,"nnnnngk");
			else
				sprintf(pIcons,"nnnnngl");
				sprintf(pIcons,"nnnnngk");
			else
				sprintf(pIcons,"nnnnngl");
				sprintf(pIcons,"nnnnngk");
			else
				sprintf(pIcons,"nnnnngl");
				sprintf(pIcons,"nnnnngk");
			else
				sprintf(pIcons,"nnnnngl");
				sprintf(pIcons,"nnnnngk");
			else
				sprintf(pIcons,"nnnnngl");
				sprintf(pIcons,"nnnnngk");
			else
				sprintf(pIcons,"nnnnngl");
				sprintf(pIcons,"nnnnngk");
			else
				sprintf(pIcons,"nnnnngl");
				sprintf(pIcons,"nnnnngk");
			else
				sprintf(pIcons,"nnnnngl");
				sprintf(pIcons,"nnnnngk");
			else
				sprintf(pIcons,"nnnnngl");

*/






void RenderMainMenu_InstancesBackground_Skeleton(struct SkeletonLL* pSkeleton, float& PosX, float& PosY)
{
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
	char pIcons[256];

	if(pSkeleton==NULL)return;

	SaveX = PosX;
	SaveY = PosY;

	///////////////////////////////
	//// Output Skeleton pName ////
	///////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;

	/////////////////////////////////////
	//// Output Skeleton pParentName ////
	/////////////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;

	//////////////////////////////////////////
	//// Output Skeleton pParentJointName ////
	//////////////////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;

	//////////////////////////////////////////
	//// Output Skeleton playing_anim_num ////
	//////////////////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");//LastItemReplace c with g
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;


	//////////////////////////////////
	//// Output Skeleton looptime ////
	//////////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnghh");//LastItemReplace c with g
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Output Flags ////////////////////////////////////////////////////////////////////
	//////////////// Flag Picking ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
//as it turns out, there is no need for skeleton flags yet. 
//setting looptime to zero effectivly disables looping, so a flag isnt necessary
/*
	PosX = SaveX;

	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pSkeleton->SkeletonDataViewFlags,SKELETON_DATAVIEW_SHOW_FLAGS,false);
	if(IsBitSet(pSkeleton->SkeletonDataViewFlags,SKELETON_DATAVIEW_SHOW_FLAGS))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
		/////////////////////////////////////////////
		/////////////////////////////////////////////loop anim
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pSkeleton->RenderFlags,SKELETON_LOOPANIM,false);
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(IsBitSet(pSkeleton->RenderFlags,SKELETON_LOOPANIM))
				sprintf(pIcons,"nnnnnnck");
			else
				sprintf(pIcons,"nnnnnncl");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnghe");//LastItemReplace c with g
			InsertIconIntoIconLL(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}
	PosX = SaveX;
*/
}


void RenderMainMenu_InstancesBackground_KeyFrame(struct KeyFrameLL* pKeyFrame, float& PosX, float& PosY)
{
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
	char pIcons[256];

	if(pKeyFrame==NULL)return;

	SaveX = PosX;
	SaveY = PosY;

	//////////////////////////////////
	//// Output KeyFrame anim_num ////
	//////////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;

	//////////////////////////////
	//// Output KeyFrame time ////
	//////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnghh");//LastItemReplace c with g
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;
}


void RenderMainMenu_InstancesBackground_Joint(struct JointLL* pJoint, float& PosX, float& PosY)
{
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
	char pIcons[256];
	long i;

	if(pJoint==NULL)return;

	SaveX = PosX;
	SaveY = PosY;

	////////////////////////////
	//// Output Joint pName ////
	////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;

	//////////////////////////////////
	//// Output Joint pParentName ////
	//////////////////////////////////

	PosX = SaveX;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		sprintf(pIcons,"nnnnchh");//LastItemReplace c with g
		InsertIconIntoIconLL2(PosX,PosY,pIcons);
	}
	PosY+=IconWidth;


	//////////////////////////////////////////////
	//////////////////////////////////////////////
	//////// Output Joint Relative Matrix ////////
	//////////////////////////////////////////////
	//////////////////////////////////////////////

	PosX = SaveX;
	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_RELATIVE_MATRIX,false);
	if(IsBitSet(pJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_RELATIVE_MATRIX))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnchd");
			InsertIconIntoIconLL2(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;

		for(i=0;i<4;i++)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnfn%s",MeshLLMatrix[i]);
				InsertIconIntoIconLL2(PosX,PosY,pIcons);
			}
			PosX = SaveX;
			PosY+=IconWidth;
		}
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnche");
			InsertIconIntoIconLL2(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}



	//////////////////////////////////////////////
	//////////////////////////////////////////////
	//////// Output Joint Absolute Matrix ////////
	//////////////////////////////////////////////
	//////////////////////////////////////////////

	PosX = SaveX;
	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&pJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_ABSOLUTE_MATRIX,false);
	if(IsBitSet(pJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_ABSOLUTE_MATRIX))
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnghd");
			InsertIconIntoIconLL2(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;

		for(i=0;i<4;i++)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnnn%s",MeshLLMatrix[i]);
				InsertIconIntoIconLL2(PosX,PosY,pIcons);
			}
			PosX = SaveX;
			PosY+=IconWidth;
		}
	}
	else
	{
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			sprintf(pIcons,"nnnnghe");
			InsertIconIntoIconLL2(PosX,PosY,pIcons);
		}
		PosX = SaveX;
		PosY+=IconWidth;
	}
}

/*
void a()////////////////////////
void a()//// InstancesText /////
void a()////////////////////////
*/

void RenderMainMenu_InstancesText_App(struct AppItem* pApp,float& PosX,float& PosY)
{
//	char pFieldText[256];
	char* stopstring = NULL;
	long i,j,index;
//	float StepY = 36.0/Render_WindowHeight;
	float StepY = IconWidth;
	float Width = 0.0;
	float StartPosX = PosX;
	float StartPosY	= PosY;
	float MatrixElementWidth = 0.0;
	float SaveX;
	float SaveY;
//	float ClipTop;
//	float ClipBottom;
	char pFieldText[256];
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	FieldLL* pField2 = NULL;
	fRECT Screen = {0,0,0,0};
	fRECT TextureBlank;
	fRECT Texture[16];
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);

	if(pApp==NULL)return;
	
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			index = j*4+i;
			Texture[index].left    = (float)(i+0)*(64.0/256.0);
			Texture[index].right   = (float)(i+1)*(64.0/256.0);
			Texture[index].top     = (float)(j+0)*(64.0/256.0);
			Texture[index].bottom  = (float)(j+1)*(64.0/256.0);
		}
	}

	TextureBlank.left   = 0.0/256.0;
	TextureBlank.right  = 1.0/256.0;
	TextureBlank.top    = 0.0/256.0;
	TextureBlank.bottom = 1.0/256.0;

	SaveX = PosX;
	SaveY = PosY;



	/////////////////////
	//// View Matrix ////
	/////////////////////
	PosX = SaveX+IconWidth*2;
	pField = ClipInsertMenuItem("View Matrix",PosX,PosY,TextColor_App);
	PosY  += StepY;
	if(IsBitSet(bShow_ViewMatrix,1))
	{
		PosX = SaveX+(IconWidth*2);
		RenderMatrix(pRender_ViewMatrix,PosX,PosY);
		PosX = SaveX;
	}
	/////////////////////
	//// Proj Matrix ////
	/////////////////////
	PosX = SaveX+IconWidth*2;
	pField = ClipInsertMenuItem("Proj Matrix",PosX,PosY,TextColor_App);
	PosY  += StepY;
	if(IsBitSet(bShow_ProjMatrix,1))
	{
		PosX = SaveX+(IconWidth*2);
		RenderMatrix(pRender_ProjMatrix,PosX,PosY);
		PosX = SaveX;
	}
	//////////////////////
	//// Ortho Matrix ////
	//////////////////////
	PosX = SaveX+IconWidth*2;
	pField = ClipInsertMenuItem("Ortho Matrix",PosX,PosY,TextColor_App);
	PosY  += StepY;
	if(IsBitSet(bShow_OrthoMatrix,1))
	{
		PosX = SaveX+(IconWidth*2);
		RenderMatrix(pRender_OrthoMatrix,PosX,PosY);
		PosX = SaveX;
	}




	#ifdef TechNaturePrivate
	////////////////////
	//// New Planet ////
	////////////////////
	PosX = SaveX+IconWidth*2;
	pField = ClipInsertMenuItem("New Planet",PosX,PosY,TextColor_App);
	PosY  += StepY;
	if(IsBitSet(bShow_NewPlanet,1))
	{
		PosX = SaveX+(IconWidth*2);
		Planet_RenderMenu_Text(PosX,PosY);
		PosX = SaveX;
	}
	#endif//TechNaturePrivate









	//////////////////////////////////////////////////////////////////////////////////

	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"Visual Bone Scale : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_App);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);
	PosX += Width;

	sprintf(pFieldText,"%f",App_BoneScale);
	pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,150.0,TextColor_Editable,
					(float*)&App_BoneScale,
					NULL,
					NULL,
					NULL);
	PosY+=StepY;

	//////////////////////////////////////////////////////////////////////////////////

	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"Visual Joint Scale : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_App);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);
	PosX += Width;

	sprintf(pFieldText,"%f",App_JointScale);
	pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,150.0,TextColor_Editable,
					(float*)&App_JointScale,
					NULL,
					NULL,
					NULL);
	PosY+=StepY;

	//////////////////////////////////////////////////////////////////////////////////


	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"	Weight Spray Radius : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_App);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);
	PosX += Width;

	sprintf(pFieldText,"%f",App_SprayRadius);
	pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,150.0,TextColor_Editable,
					(float*)&App_SprayRadius,			  
					NULL,
					NULL,
					NULL);
	PosY+=StepY;
	//////////////////////////////////////////////////////////////////////////////////

/*
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"Mouse Sphere Radius : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_App);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);
	PosX += Width;

	sprintf(pFieldText,"%f",App_PickSphereRadius);
	pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,150.0,TextColor_Editable,
					(float*)&App_PickSphereRadius,
					NULL,
					NULL,
					NULL);
	PosY+=StepY;
*/

	//////////////////////////////////////////////////////////////////////////////////

	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"Camera Position Speed : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_App);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);
	PosX += Width;

	sprintf(pFieldText,"%f",App_CameraPositionSpeed);
	pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,150.0,TextColor_Editable,
					(float*)&App_CameraPositionSpeed,
					NULL,
					NULL,
					NULL);
	PosY+=StepY;


	//////////////////////////////////////////////////////////////////////////////////

	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"Camera Rotation Speed : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_App);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);
	PosX += Width;

	sprintf(pFieldText,"%f",App_CameraRotationSpeed);
	pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,150.0,TextColor_Editable,
					(float*)&App_CameraRotationSpeed,
					NULL,
					NULL,
					NULL);
	PosY+=StepY;


	///////////////
	//// Flags ////
	///////////////

	RenderMainMenu_RenderFlagsText(App_DataViewFlags,APP_DATAVIEW_SHOW_FLAGS,AppRenderFlagsText,TextColor_App,SaveX,PosX,PosY);

}

void RenderMainMenu_InstancesText_Scene(struct SceneLL* pScene,float& PosX,float& PosY)
{
	char pFieldText[256];
	char* stopstring = NULL;
	long i,j,Length,index;
//	float StepY = 36.0/Render_WindowHeight;
	float StepY = IconWidth;
	float Width = 0.0;
	float StartPosX = PosX;
	float StartPosY	= PosY;
	float MatrixElementWidth = 0.0;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	FieldLL* pField2 = NULL;
	fRECT Screen = {0,0,0,0};
	fRECT TextureBlank;
	fRECT Texture[16];
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	char *SceneLLConnectionTypes[] =
	{
		"5", //num of elements in this list
		"a", //pName. just output name, not pName = name
		"ch",//SortKey
		"ce",//pView
		"ce",//pProj
		"ce",//pObjectHead
	};

	char *SceneLLTextVersion[] =
	{
		"5",//num of elements in this list
		"",			  //pName. just output name, not pName = name
		"SortKey = ", //SortKey
		"View Matrix",//pView
		"Proj Matrix",//pProj
		"Objects",	  //pObjectHead
	};

	if(pScene==NULL)return;
	
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			index = j*4+i;
			Texture[index].left    = (float)(i+0)*(64.0/256.0);
			Texture[index].right   = (float)(i+1)*(64.0/256.0);
			Texture[index].top     = (float)(j+0)*(64.0/256.0);
			Texture[index].bottom  = (float)(j+1)*(64.0/256.0);
		}
	}

	TextureBlank.left   = 0.0/256.0;
	TextureBlank.right  = 1.0/256.0;
	TextureBlank.top    = 0.0/256.0;
	TextureBlank.bottom = 1.0/256.0;

	SaveX = PosX;
	SaveY = PosY;

	////////////////////////////
	//// Output Scene pName ////
	////////////////////////////

	index = 1;
	Length = strlen(SceneLLConnectionTypes[index]);

	PosX = SaveX+(IconWidth*2);

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	sprintf(pTempText,"pName : ");
	pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Scene);

//	PosX = SaveX+(IconWidth*7);
//	pField = ClipInsertMenuItem(pTempText,PosX,PosY,SelectedTextColor_Scene);


/*
	sprintf(pTempText,"%s",pScene->pName);
	if(pCurrentlySelectedScene && strcmpi(pScene->pName,pCurrentlySelectedScene->pName))
	{//not currently selected, so normal color
		pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Scene);
	}
	else
	{//currently selected, so use selected color
		pField = ClipInsertMenuItem(pTempText,PosX,PosY,SelectedTextColor_Scene);
	}

*/

	PosX = SaveX+(IconWidth*6);
	pField2 = InsertEditableMenuItem(pScene->pName,PosX,PosY,320.0,TextColor_Editable,
				NULL,			//pfloat
				NULL,			//plong
				NULL,			//pword
				pScene->pName);	//pchar



	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	PosY+=StepY;
	PosX = SaveX;

/*
	//// Continue If Scene Is Expanded
	if(!IsBitSet(pScene->SceneDataViewFlags,SCENE_DATAVIEW_SHOW_THIS_SCENE))
	{
		return;
	}
	if(pScene!=pCurrentlySelectedScene)
	{//only currentlyselected scene can actually be expanded
		return;
	}
*/
  
	/////////////////////////////////////
	/////////////////////////////////////
	//////// Output Scene SortKey ///////
	/////////////////////////////////////
	/////////////////////////////////////


	index = 2;
	PosX = SaveX+(IconWidth*2);

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//"sortkey ="
		pField = FieldLL_Insert(SCENE_TYPE,"Debug Name",
						SceneLLTextVersion[index],
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Scene,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
				 		0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX += Width;

	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{//"= actual value"
		sprintf(pFieldText,"%u",pScene->SortKey);
		pField = FieldLL_Insert(SCENE_TYPE,"Debug Value",pFieldText,PosX,PosY,
						MainMenuDataWidth,MainMenuDataHeight,Screen,
						1.0,0.0,0.0,TextColor_Scene,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
				 		0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}

	PosY+=StepY;
	PosX = SaveX;


	////////////////////////////////////
	////////////////////////////////////
	////////////////////////////////////
	//////// Output Scene pView ////////
	////////////////////////////////////
	////////////////////////////////////
	////////////////////////////////////
	//////////////////////////////////
	//// Output Scene pObjectHead ////
	//////////////////////////////////
/*
	// Object : 
	PosX = SaveX+IconWidth*2;
	sprintf(pTempText,"Objects : ");
	pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Scene);

	PosX = SaveX;
	PosY+=StepY;

	//DONT CLIP THIS. CLIPPING IS HANDLED IN RenderObjectStructureText
	if(IsBitSet(pScene->SceneDataViewFlags,SCENE_DATAVIEW_SHOW_OBJECTHEAD))
	{
		PosX   = SaveX+(IconWidth*2);
		RenderMainMenu_FunctionsText_Object(PosX,PosY);
	}
*/
}
//		struct SceneLL* pScene,
void RenderMainMenu_InstancesText_Object(struct ObjectLL* pObject,float& PosX,float& PosY)
{
	char pFieldText[256];
	char* stopstring = NULL;
	long index;
	float Width = 0.0;
	float StartPosX = PosX;
	float StartPosY	= PosY;
	float MatrixElementWidth = 0.0;
	float SaveX;
	float SaveY;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	FieldLL* pField2 = NULL;//temp pointer. fields are added to linked list during insert.
	fRECT Screen = {0,0,0,0};
//	fRECT TextureBlank;
//	fRECT Texture[16];
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
	
	if(pObject==NULL)return;
/*
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			index = j*4+i;
			Texture[index].left    = (float)(i+0)*(64.0/256.0);
			Texture[index].right   = (float)(i+1)*(64.0/256.0);
			Texture[index].top     = (float)(j+0)*(64.0/256.0);
			Texture[index].bottom  = (float)(j+1)*(64.0/256.0);
		}
	}

	TextureBlank.left   = 0.0/256.0;
	TextureBlank.right  = 1.0/256.0;
	TextureBlank.top    = 0.0/256.0;
	TextureBlank.bottom = 1.0/256.0;
*/
	SaveX = PosX;
	SaveY = PosY;

	/////////////////////////////
	//// Output Object pName ////
	/////////////////////////////

	index = 1;
	PosX = SaveX+IconWidth*2;

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
//	sprintf(pTempText,"Object Name : ");
//	pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Object);

	sprintf(pTempText,"pName : ");
	pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Object);
/*

	PosX = SaveX+(IconWidth*7);

	sprintf(pTempText,"%s",pObject->pName);
	if(pCurrentlySelectedObject && strcmpi(pObject->pName,pCurrentlySelectedObject->pName))
	{//not currently selected, so normal color
		pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Object);
	}
	else
	{//currently selected, so use selected color
		pField = ClipInsertMenuItem(pTempText,PosX,PosY,SelectedTextColor_Object);
	}
	PosY+=IconWidth;
	PosX = SaveX;
*/
	PosX = SaveX+(IconWidth*6);
	pField2 = InsertEditableMenuItem(pObject->pName,PosX,PosY,320.0,TextColor_Editable,
				NULL,			//pfloat
				NULL,			//plong
				NULL,			//pword
				pObject->pName);	//pchar

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	PosY+=StepY;
	PosX = SaveX;
/*

	//// Continue, only If Object Is Expanded
	if(!IsBitSet(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_THIS_OBJECT))
	{
		return;
	}

*/
	///////////////////////////////
	//// Output Object SortKey ////
	///////////////////////////////

	index = 2;

	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(OBJECT_TYPE,"Debug Name",
						ObjectLLTextVersion[index],
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Object,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
				 		0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX += Width;


	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pFieldText,"%u",pObject->SortKey);
		pField = InsertEditableMenuItem(pFieldText,PosX,PosY,50.0,TextColor_Editable,
					NULL,
					NULL,
					(unsigned short*)&pObject->SortKey,
					NULL);

		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosY+=IconWidth;
	PosX = SaveX;


	///////////////////////////////
	//// Output Object pMatrix ////
	///////////////////////////////

	index = 3;
//  	Length = strlen(ObjectLLConnectionTypes[index]);
//	PosX += (Length*IconWidth);

	PosX = SaveX+IconWidth*2;


	//Output the name of this variable pView/
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(OBJECT_TYPE,"Debug Name",
						ObjectLLTextVersion[index],
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Object,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
				 		0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;

//	if(pObject->bShowMatrix)
	if(IsBitSet(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_MATRIX))
	{
		PosX = SaveX+(IconWidth*2);
		RenderMatrix(pObject->pMatrix,PosX,PosY);
		PosX = SaveX;
	}

/*
	///////////////////////////////////
	//// Output Object pMeshHead ////
	///////////////////////////////////
	PosX = SaveX+IconWidth;

	// Meshes : 
	PosX = SaveX+IconWidth;
	sprintf(pTempText,"Meshes");
	pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Object);


	PosX = SaveX;
	PosY+=IconWidth;

	if(IsBitSet(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_MESHHEAD))
	{
		PosX   = SaveX+IconWidth;
		RenderMainMenu_FunctionsText_Mesh(PosX,PosY);
	}






	/////////////////////////////////
	//// Output Object pSkeleton ////
	/////////////////////////////////

	// Skeleton : 
	PosX = SaveX+IconWidth;
	sprintf(pTempText,"Skeleton");
	pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Object);


	PosX = SaveX;
	PosY+=IconWidth;
	if(IsBitSet(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_SKELETON))
	{
		PosX   = SaveX+IconWidth;
		RenderMainMenu_FunctionsText_Skeleton(PosX,PosY);
	}
*/








	//////////////////////
	//// Output Flags ////
	//////////////////////

	RenderMainMenu_RenderFlagsText(pObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_FLAGS,ObjectRenderFlagsText,TextColor_Object,SaveX,PosX,PosY);

}


void RenderMainMenu_InstancesText_Mesh(struct MeshLL* pMesh,float& PosX,float& PosY)
{
	char pFieldText[256];
	char* stopstring = NULL;
	long i,index;
//	float StepY = 36.0/Render_WindowHeight;
//	float StepY = 36.0;
	float Width = 0.0;
	float StartPosX = PosX;
	float StartPosY	= PosY;
	float MatrixElementWidth = 0.0;
	float SaveX;
	float SaveY;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	struct FieldLL* pField2 = NULL;
	fRECT Screen = {0,0,0,0};
//	fRECT TextureBlank;
//	fRECT Texture[16];

	float StepY = IconWidth;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float ClipTop;
	float ClipBottom;
	
	if(pMesh==NULL)return;
/*
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			index = j*4+i;
			Texture[index].left    = (float)(i+0)*(64.0/256.0);
			Texture[index].right   = (float)(i+1)*(64.0/256.0);
			Texture[index].top     = (float)(j+0)*(64.0/256.0);
			Texture[index].bottom  = (float)(j+1)*(64.0/256.0);
		}
	}

	TextureBlank.left   = 0.0/256.0;
	TextureBlank.right  = 1.0/256.0;
	TextureBlank.top    = 0.0/256.0;
	TextureBlank.bottom = 1.0/256.0;
*/
	SaveX = PosX;
	SaveY = PosY;


	PosX = SaveX+IconWidth*2;
	sprintf(pTempText,"pName : ");
	pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Mesh);

	PosX = SaveX+(IconWidth*6);
	pField2 = InsertEditableMenuItem(pMesh->pName,PosX,PosY,320.0,TextColor_Editable,
				NULL,			//pfloat
				NULL,			//plong
				NULL,			//pword
				pMesh->pName);	//pchar

/*
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	sprintf(pTempText,"Mesh Name : ");
	pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Mesh);

	PosX = SaveX+(IconWidth*8);

	sprintf(pTempText,"%s",pMesh->pName);
	if(pCurrentlySelectedMesh && strcmpi(pMesh->pName,pCurrentlySelectedMesh->pName))
	{//not currently selected, so normal color
		pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Mesh);
	}
	else
	{//currently selected, so use selected color
		pField = ClipInsertMenuItem(pTempText,PosX,PosY,SelectedTextColor_Mesh);
	}
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
*/

	PosY+=IconWidth;
	PosX = SaveX;

	//// Continue If Scene Is Expanded
//	if(!pMesh->bShowThisMesh)
/*
	if(!IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_THIS_MESH))
	{
		return;
	}
*/
	///////////////////////////////
	//// Output Mesh SortKey ////
	///////////////////////////////

	index = 2;
//	Length = strlen(MeshLLConnectionTypes[index]);
//	PosX += (Length*IconWidth);
	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*3))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"SortKey = ",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX += Width;


	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pFieldText,"%u",pMesh->SortKey);

		pField = InsertEditableMenuItem(pFieldText,PosX,PosY,50.0,TextColor_Editable,
					NULL,
					NULL,
					(unsigned short*)&pMesh->SortKey,
					NULL);




		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;


	///////////////////////////////
	//// Output Mesh pMatrix ////
	///////////////////////////////

	index = 3;
//  	Length = strlen(MeshLLConnectionTypes[index]);
//	PosX += (Length*IconWidth);

	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"World Matrix",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;

//	if(pMesh->bShowMatrix)
	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_MATRIX))
	{
		PosX = SaveX+(IconWidth*2);
		RenderMatrix(pMesh->pMatrix,PosX,PosY);
		PosX = SaveX;
	}

	///////////////////////////////////////////
	///////////////////////////////////////////
	//////// Output Mesh pLocalVertices////////
	//////// pLocalVerts               ////////
	///////////////////////////////////////////
	///////////////////////////////////////////

	index = 4;
	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"Local Vertices",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;

	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_LOCALVERTICES))
	{
		PosX = SaveX+(IconWidth*2);
		Render3dVertices(pMesh->pLocalVerts,pMesh->num_verts,PosX,PosY);
		PosX = SaveX;
	}

	///////////////////////////////////////////
	///////////////////////////////////////////
	//////// Output Mesh pWorldVertices////////
	//////// pWorldVerts               ////////
	///////////////////////////////////////////
	///////////////////////////////////////////

	index = 4;
	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"World Vertices",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;

	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_WORLDVERTICES))
	{
		PosX = SaveX+(IconWidth*2);
		Render3dVertices(pMesh->pWorldVerts,pMesh->num_verts,PosX,PosY);
		PosX = SaveX;
	}



	/////////////////////////////////////
	/////////////////////////////////////
	//////// Output Mesh Indices ////////
	//////// pIndices       ////////
	/////////////////////////////////////
	/////////////////////////////////////

	index = 6;
//  	Length = strlen(MeshLLConnectionTypes[index]);
//	PosX += (Length*IconWidth);
	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"Indices",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;

	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_3dINDICES))
	{
		PosX = SaveX+(IconWidth*2);
		RenderIndices(pMesh->pIndices,pMesh->num_tris,PosX,PosY);
		PosX = SaveX;
	}

	/////////////////////////////////
	//// Output Mesh num_verts ////
	/////////////////////////////////

	index = 7;
	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"num_verts ",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX += Width;


	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pFieldText,"%u",pMesh->num_verts);
		pField = FieldLL_Insert(MESH_TYPE,"Debug Value",pFieldText,PosX,PosY,
						MainMenuDataWidth,MainMenuDataHeight,Screen,
						1.0,0.0,0.0,TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;


	/////////////////////////////////
	//// Output Mesh num_tris  ////
	/////////////////////////////////

	index = 8;
//	Length = strlen(MeshLLConnectionTypes[index]);
//	PosX += (Length*IconWidth);
	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"num_tris ",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX += Width;


	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pFieldText,"%u",pMesh->num_tris);
		pField = FieldLL_Insert(MESH_TYPE,"Debug Value",pFieldText,PosX,PosY,
						MainMenuDataWidth,MainMenuDataHeight,Screen,
						1.0,0.0,0.0,TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;



	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	//////// Output SrcBlend ////////////////////////////////
	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////

	index = 9;
	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"Src Blend",
						//MeshLLTextVersion[index],
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;

//	if(pMesh->bShowFlags)
	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_SRCBLEND))
	{

		for(i=0;i<(atoi(BlendModeText[0]));i++)
		{
	//	  	Length = strlen("dddddce");
			PosX = SaveX+(IconWidth*3);
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{
				pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
								BlendModeText[i+1],
								PosX,PosY,
								MainMenuDataWidth,
								MainMenuDataHeight,Screen,1.0,0.0,0.0,
								TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
								0,NULL,NULL,NULL,NULL,
								&AppTools_Fonts[0],
								pRender_FieldHead);
				Width = CalcWidthField(pField,&AppTools_Fonts[0]);
				pField->Screen.left   = PosX;
				pField->Screen.right  = PosX+Width;
				pField->Screen.top    = (PosY-PreStep);
				pField->Screen.bottom = (PosY-PreStep)+IconWidth;
				if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
				if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
			}
			PosX = SaveX;
			PosY+=IconWidth;
		}
	}

	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	//////// Output DstBlend ////////////////////////////////
	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////

	index = 9;
//  	Length = strlen(MeshLLConnectionTypes[index]);
//	PosX += (Length*IconWidth);
	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"Dest Blend",
						//MeshLLTextVersion[index],
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
				 		0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;

//	if(pMesh->bShowFlags)
	if(IsBitSet(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_DSTBLEND))
	{

		for(i=0;i<(atoi(BlendModeText[0]));i++)
		{
		  	//Length = strlen("dddddce");
			//PosX += (Length*IconWidth);
			PosX = SaveX+(IconWidth*3);
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{
				pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
								BlendModeText[i+1],
								PosX,PosY,
								MainMenuDataWidth,
								MainMenuDataHeight,Screen,1.0,0.0,0.0,
								TextColor_Mesh,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
				 				0,NULL,NULL,NULL,NULL,
								&AppTools_Fonts[0],
								pRender_FieldHead);
				Width = CalcWidthField(pField,&AppTools_Fonts[0]);
				pField->Screen.left   = PosX;
				pField->Screen.right  = PosX+Width;
				pField->Screen.top    = (PosY-PreStep);
				pField->Screen.bottom = (PosY-PreStep)+IconWidth;
				if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
				if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
			}
			PosX = SaveX;
			PosY+=IconWidth;
		}
	}
	//////////////////////
	//// Output Flags ////
	//////////////////////

	
	RenderMainMenu_RenderFlagsText(pMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_FLAGS,MeshRenderFlagsText,TextColor_Mesh,SaveX,PosX,PosY);

}
void RenderMainMenu_InstancesText_Skeleton(struct SkeletonLL* pSkeleton,float& PosX,float& PosY)
{
	char pFieldText[256];
	char* stopstring = NULL;
//	float StepY = 36.0/Render_WindowHeight;
	float StepY = IconWidth;
	float Width = 0.0;
	float StartPosX = PosX;
	float StartPosY	= PosY;
	float MatrixElementWidth = 0.0;
	float SaveX;
	float SaveY;
//	float ClipTop;
//	float ClipBottom;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	FieldLL* pField2 = NULL;
	fRECT Screen = {0,0,0,0};
//	fRECT TextureBlank;
//	fRECT Texture[16];
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);

	if(pSkeleton==NULL)return;

	SaveX = PosX;
	SaveY = PosY;

	////////////////////////////
	//// Output Skeleton pName ////
	////////////////////////////



	/////////////////////////////////////////////////////////////////////
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"pName : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Skeleton);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX += Width;
	pField2 = InsertEditableMenuItem2(pSkeleton->pName,PosX,PosY,320.0,TextColor_Editable,
				NULL,			//pfloat
				NULL,			//plong
				NULL,			//pword
				pSkeleton->pName);	//pchar

	PosY+=StepY;
	/////////////////////////////////////////////////////////////////////
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"pParentName : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Skeleton);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX += Width;
	pField2 = InsertEditableMenuItem2(pSkeleton->pParentName,PosX,PosY,320.0,TextColor_Editable,
				NULL,			//pfloat
				NULL,			//plong
				NULL,			//pword
				pSkeleton->pParentName);	//pchar

	PosY+=StepY;
	/////////////////////////////////////////////////////////////////////
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"pParentJointName : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Skeleton);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX += Width;
	pField2 = InsertEditableMenuItem2(pSkeleton->pParentJointName,PosX,PosY,320.0,TextColor_Editable,
				NULL,			//pfloat
				NULL,			//plong
				NULL,			//pword
				pSkeleton->pParentJointName);	//pchar

	PosY+=StepY;

	///////////////////////////////////////////
	//// Output Skeleton playing_anim_num ////
	///////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"playing_anim_num : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Skeleton);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX += Width;
	sprintf(pFieldText,"%u",pSkeleton->playing_anim_num);
	pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,100.0,TextColor_Editable,
				NULL,
				(unsigned long*)&pSkeleton->playing_anim_num,
				NULL,
				NULL);

	PosY+=StepY;

	/////////////////////////////////////////////////////////////////////
	//unsigned long looptime;//there has to be a looptime KF, and it is == to the 0 keyframe
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"looptime : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Skeleton);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX += Width;
	sprintf(pFieldText,"%u",pSkeleton->looptime);
	pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,100.0,TextColor_Editable,
				NULL,
				(unsigned long*)&pSkeleton->looptime,
				NULL,
				NULL);

	PosY+=StepY;

	/////////////////////////////////////////////////////////////////////

//setting lloptime to 0 turns off looping, so no flags	
//	RenderMainMenu_RenderFlagsText(pSkeleton->SkeletonDataViewFlags,SKELETON_DATAVIEW_SHOW_FLAGS,SkeletonRenderFlagsText,TextColor_Skeleton,SaveX,PosX,PosY);


}

void RenderMainMenu_InstancesText_KeyFrame(struct KeyFrameLL* pKeyFrame,float& PosX,float& PosY)
{
	char pFieldText[256];
	char* stopstring = NULL;
//	float StepY = 36.0/Render_WindowHeight;
	float StepY = IconWidth;
	float Width = 0.0;
	float StartPosX = PosX;
	float StartPosY	= PosY;
	float MatrixElementWidth = 0.0;
	float SaveX;
	float SaveY;
//	float ClipTop;
//	float ClipBottom;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	FieldLL* pField2 = NULL;
	fRECT Screen = {0,0,0,0};
//	fRECT TextureBlank;
//	fRECT Texture[16];
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);

	if(pKeyFrame==NULL)return;

	SaveX = PosX;
	SaveY = PosY;

	//////////////////////////////////
	//// Output KeyFrame anim_num ////
	//////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"anim_num : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_KeyFrame);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX += Width;

	sprintf(pFieldText,"%u",pKeyFrame->anim_num);

	if(0)//pKeyFrame->time<=KEYFRAME_MAXFRAME)
	{
		pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,50.0,TextColor_Editable,
					NULL,
					(unsigned long*)&pKeyFrame->anim_num,
					NULL,
					NULL);
	}
	else
	{
		pField = InsertMenuItem2(pFieldText,PosX,PosY,TextColor_NotEditable);
	}

	PosY+=StepY;
	/////////////////////////////////////////////////////////////////////
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"time : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_KeyFrame);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX += Width;

	sprintf(pFieldText,"%u",pKeyFrame->time);

	if(0)//pKeyFrame->time<=KEYFRAME_MAXFRAME)
	{
		pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,50.0,TextColor_Editable,
					NULL,
					(unsigned long*)&pKeyFrame->time,
					NULL,
					NULL);
	}
	else
	{
		pField = InsertMenuItem2(pFieldText,PosX,PosY,TextColor_NotEditable);
	}

	PosY+=StepY;




/*

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*3))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"anim_num : ",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_KeyFrame,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX += Width;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pFieldText,"%u",pKeyFrame->anim_num);
		//editing the animnum is currently disabled. need error checking to verify it DNE first
		if(0)//pKeyFrame->time<=KEYFRAME_MAXFRAME)
		{
			pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,50.0,TextColor_Editable,
						NULL,
						(unsigned long*)&pKeyFrame->anim_num,
						NULL,
						NULL);
		}
		else
		{
			pField = InsertMenuItem2(pFieldText,PosX,PosY,TextColor_NotEditable);
		}

		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosY+=IconWidth;
	PosX = SaveX;

	//////////////////////////////
	//// Output KeyFrame time ////
	//////////////////////////////

	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*3))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"time : ",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_KeyFrame,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX += Width;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pFieldText,"%u",pKeyFrame->time);
		//editing the time is currently disabled. need error checking to verify it DNE first
		if(0)//pKeyFrame->time<=KEYFRAME_MAXFRAME)
		{
			pField = InsertEditableMenuItem2(pFieldText,PosX,PosY,50.0,TextColor_Editable,
						NULL,
						(unsigned long*)&pKeyFrame->time,
						NULL,
						NULL);
		}
		else
		{
			pField = InsertMenuItem2(pFieldText,PosX,PosY,TextColor_NotEditable);
		}

		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosY+=IconWidth;
	PosX = SaveX;
*/
}

void RenderMainMenu_InstancesText_Joint(struct JointLL* pJoint,float& PosX,float& PosY)
{
//	char pFieldText[256];
	char* stopstring = NULL;
//	long i,j,Length;//,index;
//	float StepY = 36.0/Render_WindowHeight;
	float StepY = IconWidth;
	float Width = 0.0;
	float StartPosX = PosX;
	float StartPosY	= PosY;
	float MatrixElementWidth = 0.0;
	float SaveX;
	float SaveY;
//	float ClipTop;
//	float ClipBottom;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	FieldLL* pField2 = NULL;
	fRECT Screen = {0,0,0,0};
//	fRECT TextureBlank;
//	fRECT Texture[16];
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);

	if(pJoint==NULL)return;
	if(pJoint->pInfoItem==NULL)return;

	SaveX = PosX;
	SaveY = PosY;

	/////////////////////////////////////////////////////////////////////
	////////////////////////////
	//// Output Joint pName ////
	////////////////////////////
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"pName : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Joint);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX += Width;

	pField2 = InsertMenuItem2(pJoint->pInfoItem->pName,PosX,PosY,TextColor_Joint);
	/* 
	//editing the name is currently disabled, because if you rename joint2 to be joint1, then the real joint1 is lost forever.
	pField2 = InsertEditableMenuItem(pJoint->pInfoItem->pName,PosX,PosY,320.0,TextColor_Editable,
				NULL,			//pfloat
				NULL,			//plong
				NULL,			//pword
				pJoint->pInfoItem->pName);	//pchar
	*/
	PosY+=StepY;
	/////////////////////////////////////////////////////////////////////
	//////////////////////////////////
	//// Output Joint pParentName ////
	//////////////////////////////////
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"pParentName : ");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Joint);
	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX += Width;

	pField2 = InsertMenuItem2(pJoint->pInfoItem->pParentName,PosX,PosY,TextColor_Joint);
	/* 
	//editing the name is currently disabled, because if you rename joint2 to be joint1, then the real joint1 is lost forever.
	pField2 = InsertEditableMenuItem(pJoint->pInfoItem->pParentName,PosX,PosY,320.0,TextColor_Editable,
				NULL,			//pfloat
				NULL,			//plong
				NULL,			//pword
				pJoint->pInfoItem->pParentName);	//pchar
	*/

	PosY+=StepY;








	//////////////////////////////////////////////
	//////////////////////////////////////////////
	//////// Output Joint Relative Matrix ////////
	//////////////////////////////////////////////
	//////////////////////////////////////////////
/*
	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"Relative Matrix",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Joint,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;


*/



	/////////////////////////////////////////////////////////////////////
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"Relative Matrix");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Joint);
//	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX = SaveX;
	PosY+=IconWidth;

	if(IsBitSet(pJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_RELATIVE_MATRIX))
	{
		PosX = SaveX+(IconWidth*2);
		RenderMatrix(pJoint->pRelativeMatrix,PosX,PosY);
		PosX = SaveX;
	}

	//////////////////////////////////////////////
	//////////////////////////////////////////////
	//////// Output Joint Absolute Matrix ////////
	//////////////////////////////////////////////
	//////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	PosX = SaveX+(IconWidth*2);
	sprintf(pTempText,"Absolute Matrix");
	pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Joint);
//	Width = CalcWidthField(pField,&AppTools_Fonts[0]);

	PosX = SaveX;
	PosY+=IconWidth;

	if(IsBitSet(pJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_ABSOLUTE_MATRIX))
	{
		PosX = SaveX+(IconWidth*2);
		RenderMatrix(pJoint->pAbsoluteMatrix,PosX,PosY);
		PosX = SaveX;
	}

/*
	PosX = SaveX+IconWidth*2;

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		pField = FieldLL_Insert(MESH_TYPE,"Debug Name",
						"Absolute Matrix",
						PosX,PosY,
						MainMenuDataWidth,
						MainMenuDataHeight,Screen,1.0,0.0,0.0,
						TextColor_Joint,TEXT_FLAG_KERNING|TEXT_FLAG_VOLATILE,
						0,NULL,NULL,NULL,NULL,
						&AppTools_Fonts[0],
						pRender_FieldHead);
		Width = CalcWidthField(pField,&AppTools_Fonts[0]);
		pField->Screen.left   = PosX;
		pField->Screen.right  = PosX+Width;
		pField->Screen.top    = (PosY-PreStep);
		pField->Screen.bottom = (PosY-PreStep)+IconWidth;
		if(pField->Screen.left <DataViewMinLeft )DataViewMinLeft =pField->Screen.left;
		if(pField->Screen.right>DataViewMaxRight)DataViewMaxRight=pField->Screen.right;
	}
	PosX = SaveX;
	PosY+=IconWidth;

	if(IsBitSet(pJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_ABSOLUTE_MATRIX))
	{
		PosX = SaveX+(IconWidth*2);
		RenderMatrix(pJoint->pAbsoluteMatrix,PosX,PosY);
		PosX = SaveX;
	}
*/



}
/*
void a()//////////////////////////////
void a()//// FunctionsBackground /////
void a()//////////////////////////////
*/
void RenderMainMenu_FunctionBackground_App(	float& PosX,float& PosY)
{
//	char pIcons[16];
	struct RectLL* pRect = NULL;
	SceneLL* pScene = NULL;
//	long i;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;//temp pointer. fields are added to linked list during insert.
	float StepY = IconWidth;

	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	SaveX = PosX;
	SaveY = PosY;
	/////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	//// "        [+-]          Aplication Name : "TN Animator" ////
	////////////////////////////////////////////////////////////////

//	PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,(unsigned long*)&bExpand_MainMenu_App,1,false);

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
 		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		//toggle ""[+]App : "TN Animator" ""
		//NEW LOCATION
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_App,1,false);

		if(bExpand_MainMenu_App)
		{
			InsertIconIntoIconLL(PosX,PosY,"nnuvovw");//"[ ][ ][-][ ][ ]App
		}
		else
		{
			InsertIconIntoIconLL(PosX,PosY,"nnuvpvw");//"[ ][ ][+][ ][ ]App
		}
	}
	PosX = SaveX;
	PosY+=IconWidth;

	if(bExpand_MainMenu_App)
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////
		//// FUNCTIONS : ////
		/////////////////////

		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// INSTANCES : Single APP ////
		////////////////////////////////

		PosX = SaveX;
  		RenderMainMenu_InstancesBackground_App(pCurrentlySelectedApp,PosX,PosY);

/*
		//Search for : ADDAPPDATAHERE:
		/////////////////////
		//// View Matrix ////
		/////////////////////

		PosX = SaveX;
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bShow_ViewMatrix,1,false);
		if(bShow_ViewMatrix)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
 				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnncd");
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;

			for(i=0;i<4;i++)
			{
				ClipTop    = (PosY-PreStep);
				ClipBottom = (PosY-PreStep)+IconWidth;
				if(	ClipTop<Render_WindowHeight &&
 					ClipBottom>(StepY*num_console_lines))
				{//If onscreen
					sprintf(pIcons,"nnnn%s",SceneLLMatrix[i]);
					InsertIconIntoIconLL(PosX,PosY,pIcons);
				}
				PosY+=IconWidth;
				PosX = SaveX;
			}
		}
		else
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
 				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnce");
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;
		}
		/////////////////////
		//// Proj Matrix ////
		/////////////////////

		PosX = SaveX;
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bShow_ProjMatrix,1,false);
		if(bShow_ProjMatrix)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
 				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnncd");
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;

			for(i=0;i<4;i++)
			{
				ClipTop    = (PosY-PreStep);
				ClipBottom = (PosY-PreStep)+IconWidth;
				if(	ClipTop<Render_WindowHeight &&
 					ClipBottom>(StepY*num_console_lines))
				{//If onscreen
					sprintf(pIcons,"nnnn%s",SceneLLMatrix[i]);
					InsertIconIntoIconLL(PosX,PosY,pIcons);
				}
				PosY+=IconWidth;
				PosX = SaveX;
			}
		}
		else
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
 				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnce");
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;
		}

		//////////////////////
		//// Ortho Matrix ////
		//////////////////////

		PosX = SaveX;
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bShow_OrthoMatrix,1,false);
		if(bShow_OrthoMatrix)
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
 				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnncd");
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;

			for(i=0;i<4;i++)
			{
				ClipTop    = (PosY-PreStep);
				ClipBottom = (PosY-PreStep)+IconWidth;
				if(	ClipTop<Render_WindowHeight &&
 					ClipBottom>(StepY*num_console_lines))
				{//If onscreen
					sprintf(pIcons,"nnnn%s",SceneLLMatrix[i]);
					InsertIconIntoIconLL(PosX,PosY,pIcons);
				}
				PosY+=IconWidth;
				PosX = SaveX;
			}
		}
		else
		{
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
 				ClipBottom>(StepY*num_console_lines))
			{//If onscreen
				sprintf(pIcons,"nnnnce");
				InsertIconIntoIconLL(PosX,PosY,pIcons);
			}
			PosY+=IconWidth;
			PosX = SaveX;
		}

*/









		/////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////
		//// LL Of Scenes ////
		//////////////////////
/*
		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
 			ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			if(bExpand_MainMenu_Scenes)
			{
				InsertIconIntoIconLL(PosX,PosY,"nnnncd");//"[-]Scenes
			}
			else
			{
				InsertIconIntoIconLL(PosX,PosY,"nnnnce");//"[+]Scenes
			}
		}
		PosY+=IconWidth;
		
		//// If Scenes Is Expanded render
		PosX = SaveX;
		if(bExpand_MainMenu_Scenes)
		{
			RenderMainMenu_FunctionBackground_Scene(PosX,PosY);
		}
*/

	}//End if(bExpand_MainMenu_App)
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////
	//// "[+-]ViewData" ////
	////////////////////////
}




void RenderMainMenu_FunctionBackground_Scene(float& PosX,float& PosY)
{
	SceneLL* pScene = NULL;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
//	fRECT Screen2;
 	struct RectLL*  pRect = NULL;
	struct SceneLL* pPrev = NULL;
	struct SceneLL* pThis = NULL;
	struct SceneLL* pNext = NULL;

	SaveX = PosX;
	SaveY = PosY;


	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	//// "[|<] [<] [+] [>] [>|] Scene : "SceneName"  ////
	/////////////////////////////////////////////////////

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
 		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
		//toggle ""[+]Scene : "scene1"
//		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Scene,1,false);
		
		if(bWM_LBUTTONDOWN)
		{
			if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*2,SaveX+IconWidth*3,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("First Button Pressed",TextColor_ConsoleError);
				MenuFunction_Scene_SelectFirst();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*3,SaveX+IconWidth*4,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Previous Button Pressed",TextColor_ConsoleError);
				MenuFunction_Scene_SelectPrev();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*4,SaveX+IconWidth*5,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Expand Button Pressed",TextColor_ConsoleError);
				bExpand_MainMenu_Scene^=1;//flip
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*5,SaveX+IconWidth*6,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Next Button Pressed",TextColor_ConsoleError);
				MenuFunction_Scene_SelectNext();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*6,SaveX+IconWidth*7,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Last Button Pressed",TextColor_ConsoleError);
				MenuFunction_Scene_SelectLast();
			}
			else 
			{
				sprintf(pTempText,"Scene : %c%s%c",quote,pCurrentlySelectedScene->pName,quote);
				float SelectableWidth = CalcWidthString(pTempText,&AppTools_Fonts[0],true);
				if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*7,SaveX+IconWidth*7+SelectableWidth,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
				{
					//OutputTextToConsole("Expand ExtraButton Pressed",TextColor_ConsoleError);
					bExpand_MainMenu_Scene^=1;//flip
				}
				//PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Scene,1,false);
			}
		}

		//if(parent==null), turn of the ability to expand/browse child
		//for scene, the parent(app) is never null. Scene functions are always accessable
		if(0)
		{
			//never null parent for scene. keep structure for comparison sake 
		}
		else
		{
			if(pRender_SceneHead)
			{//there are objects to display
				if(bExpand_MainMenu_Scene)
				{
					InsertIconIntoIconLL(PosX,PosY,"nnqrost");//"[|<] [<] [-] [>] [>|]Scene
				}
				else
				{
					InsertIconIntoIconLL(PosX,PosY,"nnqrpst");//"[|<] [<] [+] [>] [>|]Scene
				}
			}
			else
			{
				if(bExpand_MainMenu_Scene)
				{
					InsertIconIntoIconLL(PosX,PosY,"nnuvovw");//"[ ][ ][-][ ][ ]Scene
				}
				else
				{
					InsertIconIntoIconLL(PosX,PosY,"nnuvpvw");//"[ ][ ][+][ ][ ]Scene
				}
			}
		}
	}
	PosX = SaveX;
	PosY+=IconWidth;

	if(bExpand_MainMenu_Scene)
	{

		/////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////
		//// FUNCTION : NewScene() ////
		///////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : OpenScene() ////
		////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : SaveScene() ////
		////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////
		//// FUNCTION : CloseScene() ////
		/////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////
		//// INSTANCES : Scenes ////
		////////////////////////////

		PosX = SaveX;
  		RenderMainMenu_InstancesBackground_Scene(pCurrentlySelectedScene,PosX,PosY);
/*
		pScene = pRender_SceneHead;
		while(pScene)
		{
			float StartY = PosY;
			//clip to console window
			if(StartY<(StepY*num_console_lines))
			{
				StartY = (StepY*num_console_lines);
			}
			RenderMainMenu_InstancesBackground_Scene(pScene,PosX,PosY);
			Screen2.left    = PosX+(IconWidth*2);
			Screen2.right   = Render_WindowWidth;
			Screen2.top     = StartY+1;
			Screen2.bottom  = PosY-1;
			if(Screen2.bottom>Screen2.top)
			{
				pRect = RectLL_Insert(	1,//00application,0=scene,1=object,2=mesh,3=matrixJoint,4=popupmenu
										6,
										Screen2.left,  
										Screen2.right, 
										Screen2.top,   
										Screen2.bottom,
										pRender_RectHead);
			}
			pScene = pScene->pNext;
		}
*/
	}
}



void RenderMainMenu_FunctionBackground_Object(float& PosX,float& PosY)
{
	ObjectLL* pObject = NULL;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
//	fRECT Screen2;
 	struct RectLL* pRect = NULL;

	SaveX = PosX;
	SaveY = PosY;

	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	//// "[|<] [<] [+] [>] [>|] Object : "ObjectName"  ////
	///////////////////////////////////////////////////////

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
 		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
//		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Object,1,false);
		if(bWM_LBUTTONDOWN)
		{
			if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*2,SaveX+IconWidth*3,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("First Button Pressed",TextColor_ConsoleError);
				MenuFunction_Object_SelectFirst();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*3,SaveX+IconWidth*4,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Previous Button Pressed",TextColor_ConsoleError);
				MenuFunction_Object_SelectPrev();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*4,SaveX+IconWidth*5,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Expand Button Pressed",TextColor_ConsoleError);
				bExpand_MainMenu_Object^=1;//flip
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*5,SaveX+IconWidth*6,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Next Button Pressed",TextColor_ConsoleError);
				MenuFunction_Object_SelectNext();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*6,SaveX+IconWidth*7,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Last Button Pressed",TextColor_ConsoleError);
				MenuFunction_Object_SelectLast();
			}
			else 
			{
				sprintf(pTempText,"Object : %c%s%c",quote,pCurrentlySelectedObject->pName,quote);
				float SelectableWidth = CalcWidthString(pTempText,&AppTools_Fonts[0],true);
				if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*7,SaveX+IconWidth*7+SelectableWidth,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
				{
					//OutputTextToConsole("Expand ExtraButton Pressed",TextColor_ConsoleError);
					bExpand_MainMenu_Object^=1;//flip
				}
				//PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Object,1,false);
			}
		}

		//if(parent==null), turn of the ability to expand/browse child
		if(pCurrentlySelectedScene==NULL)
		{
			InsertIconIntoIconLL(PosX,PosY,"nnuvvvw");//"[ ][ ][ ][ ][ ]Object
			bExpand_MainMenu_Object = false;
			//see if(parent==null) below
		}
		else
		{
			if(	pCurrentlySelectedScene &&
				pCurrentlySelectedScene->pObjectHead)
			{//there are objects to display
				if(bExpand_MainMenu_Object)
				{
					InsertIconIntoIconLL(PosX,PosY,"nnqrost");//"[|<] [<] [-] [>] [>|]Object
				}
				else
				{
					InsertIconIntoIconLL(PosX,PosY,"nnqrpst");//"[|<] [<] [+] [>] [>|]Object
				}
			}
			else
			{//dont show browse buttons. 
				if(bExpand_MainMenu_Object)
				{
					InsertIconIntoIconLL(PosX,PosY,"nnuvovw");//"[ ][ ][-][ ][ ]Object
				}
				else
				{
					InsertIconIntoIconLL(PosX,PosY,"nnuvpvw");//"[ ][ ][+][ ][ ]Object
				}
			}
		}
	}
	PosX = SaveX;
	PosY+=IconWidth;

	//if(parent==null), leave now
	if(pCurrentlySelectedScene==NULL)
	{
		return;	
	}

	if(bExpand_MainMenu_Object)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : NewObject() ////
		////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////
		//// FUNCTION : OpenObject() ////
		/////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : SaveScene() ////
		////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////
		//// FUNCTION : CloseObject() ////
		//////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////
		//// FUNCTION : InitBoneWeights() ////
		//////////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////
		//// FUNCTION : ReSortObjects() ////
		////////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////
		//// INSTANCES : Objects ////
		/////////////////////////////

		PosX = SaveX;
  		RenderMainMenu_InstancesBackground_Object(pCurrentlySelectedObject,PosX,PosY);
/*
		PosX = SaveX;
		pObject = pCurrentlySelectedScene->pObjectHead;
		while(pObject)
		{
			float StartY = PosY;
			//clip to console window
			if(StartY<(StepY*num_console_lines))
			{
				StartY = (StepY*num_console_lines);
			}
			RenderMainMenu_InstancesBackground_Object(pObject,PosX,PosY);
			Screen2.left    = PosX+(IconWidth*4);
			Screen2.right   = Render_WindowWidth;
			Screen2.top     = StartY+1;
			Screen2.bottom  = PosY-1;
			if(Screen2.bottom>Screen2.top)
			{
				pRect = RectLL_Insert(	2,//00application,0=scene,1=object,2=mesh,3=matrixJoint,4=popupmenu
										9,
										Screen2.left,  
										Screen2.right, 
										Screen2.top,   
										Screen2.bottom,
										pRender_RectHead);
			}
			pObject = pObject->pNext;
		}
*/
	}
}

void RenderMainMenu_FunctionBackground_Mesh(float& PosX,float& PosY)
{
	MeshLL* pMesh = NULL;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
//	fRECT Screen2;
 	struct RectLL* pRect = NULL;

	SaveX = PosX;
	SaveY = PosY;

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	//// "[|<] [<] [+] [>] [>|] Mesh : "MeshName"  ////
	/////////////////////////////////////////////////////

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
 		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
//		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Mesh,1,false);
		if(bWM_LBUTTONDOWN)
		{
			if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*2,SaveX+IconWidth*3,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("First Button Pressed",TextColor_ConsoleError);
				MenuFunction_Mesh_SelectFirst();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*3,SaveX+IconWidth*4,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Previous Button Pressed",TextColor_ConsoleError);
				MenuFunction_Mesh_SelectPrev();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*4,SaveX+IconWidth*5,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Expand Button Pressed",TextColor_ConsoleError);
				bExpand_MainMenu_Mesh^=1;//flip
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*5,SaveX+IconWidth*6,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Next Button Pressed",TextColor_ConsoleError);
				MenuFunction_Mesh_SelectNext();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*6,SaveX+IconWidth*7,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Last Button Pressed",TextColor_ConsoleError);
				MenuFunction_Mesh_SelectLast();
			}
			else 
			{
				sprintf(pTempText,"Mesh : %c%s%c",quote,pCurrentlySelectedMesh->pName,quote);
				float SelectableWidth = CalcWidthString(pTempText,&AppTools_Fonts[0],true);
				if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*7,SaveX+IconWidth*7+SelectableWidth,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
				{
					//OutputTextToConsole("Expand ExtraButton Pressed",TextColor_ConsoleError);
					bExpand_MainMenu_Mesh^=1;//flip
				}
				//PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Mesh,1,false);
			}
		}

		//if(parent==null), turn of the ability to expand/browse child
		if(pCurrentlySelectedObject==NULL)
		{
			InsertIconIntoIconLL(PosX,PosY,"nnuvvvw");//"[ ][ ][ ][ ][ ]Mesh
			bExpand_MainMenu_Mesh = false;
			//see if(parent==null) below
		}
		else
		{
			if(	pCurrentlySelectedObject &&
				pCurrentlySelectedObject->pMeshHead)
			{//there are objects to display
				if(bExpand_MainMenu_Mesh)
				{
					InsertIconIntoIconLL(PosX,PosY,"nnqrost");//"[|<] [<] [-] [>] [>|]Mesh
				}
				else
				{
					InsertIconIntoIconLL(PosX,PosY,"nnqrpst");//"[|<] [<] [+] [>] [>|]Mesh
				}
			}
			else
			{//dont show browse buttons. 
				if(bExpand_MainMenu_Mesh)
				{
					InsertIconIntoIconLL(PosX,PosY,"nnuvovw");//"[ ][ ][-][ ][ ]Mesh
				}
				else
				{
					InsertIconIntoIconLL(PosX,PosY,"nnuvpvw");//"[ ][ ][+][ ][ ]Mesh
				}
			}
		}
	}
	PosX = SaveX;
	PosY+=IconWidth;

	//if(parent==null), leave now
	if(pCurrentlySelectedObject==NULL)
	{
		return;	
	}

	if(bExpand_MainMenu_Mesh)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : NewMesh() ////
		////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////
		//// FUNCTION : OpenMesh() ////
		/////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : SaveScene() ////
		////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////
		//// FUNCTION : CloseMesh() ////
		//////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////
		//// FUNCTION : ReSortMeshes() ////
		///////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////
		//// INSTANCES : Meshs ////
		/////////////////////////////

		PosX = SaveX;
		RenderMainMenu_InstancesBackground_Mesh(pCurrentlySelectedMesh,PosX,PosY);

	}
/*
	PosX = SaveX;
	pMesh = pCurrentlySelectedObject->pMeshHead;
	while(pMesh)
	{
		float StartY = PosY;
		//clip to console window
		if(StartY<(StepY*num_console_lines))
		{
			StartY = (StepY*num_console_lines);
		}
		RenderMainMenu_InstancesBackground_Mesh(pMesh,PosX,PosY);
		Screen2.left    = PosX+(IconWidth*6);
		Screen2.right   = Render_WindowWidth;
		Screen2.top     = StartY+1;
		Screen2.bottom  = PosY-1;
		if(Screen2.bottom>Screen2.top)
		{
			pRect = RectLL_Insert(	3,//00application,0=scene,1=object,2=mesh,3=matrixJoint,4=popupmenu
									11,
									Screen2.left,  
									Screen2.right, 
									Screen2.top,   
									Screen2.bottom,
									pRender_RectHead);
		}
		pMesh = pMesh->pNext;
	}
*/
}


void RenderMainMenu_FunctionBackground_Skeleton(float& PosX,float& PosY)
{
	MeshLL* pMesh = NULL;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
 	struct RectLL* pRect = NULL;

	SaveX = PosX;
	SaveY = PosY;

	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	//// "[|<] [<] [+] [>] [>|] Skeleton : "SkeletonName"  ////
	///////////////////////////////////////////////////////////

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
 		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
//		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Skeleton,1,false);
		if(bWM_LBUTTONDOWN)
		{
			if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*2,SaveX+IconWidth*3,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("First Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_SelectFirst();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*3,SaveX+IconWidth*4,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Previous Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_SelectPrev();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*4,SaveX+IconWidth*5,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Expand Button Pressed",TextColor_ConsoleError);
				bExpand_MainMenu_Skeleton^=1;//flip
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*5,SaveX+IconWidth*6,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Next Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_SelectNext();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*6,SaveX+IconWidth*7,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Last Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_SelectLast();
			}
			else 
			{
				sprintf(pTempText,"Skeleton : %c%s%c",quote,pCurrentlySelectedSkeleton->pName,quote);
				float SelectableWidth = CalcWidthString(pTempText,&AppTools_Fonts[0],true);
				if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*7,SaveX+IconWidth*7+SelectableWidth,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
				{
					//OutputTextToConsole("Expand ExtraButton Pressed",TextColor_ConsoleError);
					bExpand_MainMenu_Skeleton^=1;//flip
				}
				//PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Skeleton,1,false);
			}
		}

		//if(parent==null), turn of the ability to expand/browse child
		if(pCurrentlySelectedObject==NULL)
		{
			InsertIconIntoIconLL2(PosX,PosY,"nnuvvvw");//"[ ][ ][ ][ ][ ]Skeleton"
			bExpand_MainMenu_Skeleton = false;
			//see if(parent==null) below
		}
		else
		{
			if(	pCurrentlySelectedObject &&
				pCurrentlySelectedObject->pSkeletonHead)
			{//there are objects to display
				if(bExpand_MainMenu_Skeleton)
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnqrost");//"[|<] [<] [-] [>] [>|]Skeleton"
				}
				else
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnqrpst");//"[|<] [<] [+] [>] [>|]Skeleton"
				}
			}
			else
			{//dont show browse buttons. 
				if(bExpand_MainMenu_Skeleton)
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnuvovw");//"[ ][ ][-][ ][ ]Skeleton"
				}
				else
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnuvpvw");//"[ ][ ][+][ ][ ]Skeleton"
				}
			}
		}
	}
	PosX = SaveX;
	PosY+=IconWidth;

	//if(parent==null), leave now
	if(pCurrentlySelectedObject==NULL)
	{
		return;	
	}

	if(bExpand_MainMenu_Skeleton)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////
		//// FUNCTION : NewSkeleton() ////
		//////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL2(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////
		//// FUNCTION : OpenSkeleton() ////
		///////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL2(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;


		/////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////
		//// FUNCTION : SaveSkeleton() ////
		///////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL2(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////
		//// FUNCTION : CloseSkeleton() ////
		////////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL2(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////
		//// INSTANCES : Objects ////
		/////////////////////////////

		PosX = SaveX;
  		RenderMainMenu_InstancesBackground_Skeleton(pCurrentlySelectedSkeleton,PosX,PosY);

	}
}

void RenderMainMenu_FunctionBackground_KeyFrame(float& PosX,float& PosY)
{
	MeshLL* pMesh = NULL;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
 	struct RectLL* pRect = NULL;

	SaveX = PosX;
	SaveY = PosY;

	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	//// "[|<] [<] [+] [>] [>|] KeyFrame : "KeyFrameName"  ////
	///////////////////////////////////////////////////////////

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
 		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
//		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_KeyFrame,1,false);
		if(bWM_LBUTTONDOWN)
		{
			if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*2,SaveX+IconWidth*3,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("First Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_KeyFrame_SelectFirst();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*3,SaveX+IconWidth*4,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Previous Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_KeyFrame_SelectPrev();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*4,SaveX+IconWidth*5,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Expand Button Pressed",TextColor_ConsoleError);
				bExpand_MainMenu_KeyFrame^=1;//flip
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*5,SaveX+IconWidth*6,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Next Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_KeyFrame_SelectNext();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*6,SaveX+IconWidth*7,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Last Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_KeyFrame_SelectLast();
			}
			else 
			{
				//sprintf(pTempText,"KeyFrame : %c%s%c",quote,pCurrentlySelectedKeyFrame->pName,quote);
				if(pCurrentlySelectedKeyFrame)
				{
					sprintf(pTempText,"KeyFrame : %u:%u",pCurrentlySelectedKeyFrame->anim_num,pCurrentlySelectedKeyFrame->time);
				}
				else
				{
					sprintf(pTempText,"KeyFrame : %c(null)%c",quote,quote);
				}
				float SelectableWidth = CalcWidthString(pTempText,&AppTools_Fonts[0],true);
				if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*7,SaveX+IconWidth*7+SelectableWidth,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
				{
					//OutputTextToConsole("Expand ExtraButton Pressed",TextColor_ConsoleError);
					bExpand_MainMenu_KeyFrame^=1;//flip
				}
				//PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_KeyFrame,1,false);
			}
		}

		//if(parent==null), turn of the ability to expand/browse child
		if(pCurrentlySelectedSkeleton==NULL)
		{
			InsertIconIntoIconLL2(PosX,PosY,"nnuvvvw");//"[ ][ ][ ][ ][ ]KeyFrame"
			bExpand_MainMenu_KeyFrame = false;
			//see if(parent==null) below
		}
		else
		{
			if(	pCurrentlySelectedSkeleton &&
				pCurrentlySelectedSkeleton->pKeyFrameHead)
			{//there are objects to display
				if(bExpand_MainMenu_KeyFrame)
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnqrost");//"[|<] [<] [-] [>] [>|]KeyFrame
				}
				else
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnqrpst");//"[|<] [<] [+] [>] [>|]KeyFrame"
				}
			}
			else
			{//dont show browse buttons. 
				if(bExpand_MainMenu_KeyFrame)
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnuvovw");//"[ ][ ][-][ ][ ]KeyFrame"
				}
				else
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnuvpvw");//"[ ][ ][+][ ][ ]KeyFrame"
				}
			}
		}
	}
	PosX = SaveX;
	PosY+=IconWidth;

	//if(parent==null), leave now
	if(pCurrentlySelectedSkeleton==NULL)
	{
		return;	
	}

	if(bExpand_MainMenu_KeyFrame)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////
		//// FUNCTION : NewKeyFrame() ////
		//////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL2(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////
		//// FUNCTION : CopyKeyFrame() ////
		///////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL2(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////
		//// FUNCTION : CloseKeyFrame() ////
		////////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL2(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////
		//// INSTANCES : Objects ////
		/////////////////////////////

		PosX = SaveX;
  		RenderMainMenu_InstancesBackground_KeyFrame(pCurrentlySelectedKeyFrame,PosX,PosY);

	}
}

void RenderMainMenu_FunctionBackground_Joint(float& PosX,float& PosY)
{
	MeshLL* pMesh = NULL;
	float SaveX;
	float SaveY;
	float ClipTop;
	float ClipBottom;
	FieldLL* pField = NULL;
	float PreStep = 0.0;//(MainMenuDataHeight*0.5);
	float StepY = IconWidth;
 	struct RectLL* pRect = NULL;

	SaveX = PosX;
	SaveY = PosY;

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	//// "[|<] [<] [+] [>] [>|] Joint : "JointName"  ////
	/////////////////////////////////////////////////////

	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
 		ClipBottom>(StepY*num_console_lines))
	{//If onscreen
//		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Joint,1,false);
		if(bWM_LBUTTONDOWN)
		{
			if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*2,SaveX+IconWidth*3,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("First Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_Joint_SelectFirst();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*3,SaveX+IconWidth*4,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Previous Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_Joint_SelectPrev();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*4,SaveX+IconWidth*5,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Expand Button Pressed",TextColor_ConsoleError);
				bExpand_MainMenu_Joint^=1;//flip
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*5,SaveX+IconWidth*6,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Next Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_Joint_SelectNext();
			}
			else if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*6,SaveX+IconWidth*7,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
			{
				//OutputTextToConsole("Last Button Pressed",TextColor_ConsoleError);
				MenuFunction_Skeleton_Joint_SelectLast();
			}
			else 
			{
				if(pCurrentlySelectedJoint && pCurrentlySelectedJoint->pInfoItem)
				{
					sprintf(pTempText,"Joint : %c%s%c",quote,pCurrentlySelectedJoint->pInfoItem->pName,quote);
				}
				else
				{
					sprintf(pTempText,"Joint : %c(null)%c",quote,quote);
				}

				float SelectableWidth = CalcWidthString(pTempText,&AppTools_Fonts[0],true);
				if(PickDataView_IsCursorClickingFunction(CurrentMousePosition,SaveX+IconWidth*7,SaveX+IconWidth*7+SelectableWidth,PosY,PosY+IconWidth))//cmp,left,right,top,bottom
				{
					//OutputTextToConsole("Expand ExtraButton Pressed",TextColor_ConsoleError);
					bExpand_MainMenu_Joint^=1;//flip
				}
				//PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Joint,1,false);
			}
		}

		//if(parent==null), turn of the ability to expand/browse child
		if(pCurrentlySelectedKeyFrame==NULL)
		{
			InsertIconIntoIconLL2(PosX,PosY,"nnuvvvw");//"[ ][ ][ ][ ][ ]Joint"
			bExpand_MainMenu_Joint = false;
			//see if(parent==null) below
		}
		else
		{
			if(	pCurrentlySelectedKeyFrame &&
				pCurrentlySelectedKeyFrame->pJointHead)
			{//there are objects to display
				if(bExpand_MainMenu_Joint)
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnqrost");//"[|<] [<] [-] [>] [>|]Joint"
				}
				else
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnqrpst");//"[|<] [<] [+] [>] [>|]Joint"
				}
			}
			else
			{//dont show browse buttons. 
				if(bExpand_MainMenu_Joint)
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnuvovw");//"[ ][ ][-][ ][ ]Joint"
				}
				else
				{
					InsertIconIntoIconLL2(PosX,PosY,"nnuvpvw");//"[ ][ ][+][ ][ ]Joint"
				}
			}
		}
	}
	PosX = SaveX;
	PosY+=IconWidth;

	//if(parent==null), leave now
	if(pCurrentlySelectedKeyFrame==NULL)
	{
		return;	
	}

	if(bExpand_MainMenu_Joint)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////
		//// FUNCTION : NewJoint() ////
		//////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL2(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////
		//// FUNCTION : CloseJoint() ////
		////////////////////////////////////

		PosX = SaveX;
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
		{//If onscreen
			InsertIconIntoIconLL2(PosX,PosY,"nnnnchj");
		}
		PosY+=IconWidth;

		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////
		//// INSTANCES : Objects ////
		/////////////////////////////

		PosX = SaveX;
  		RenderMainMenu_InstancesBackground_Joint(pCurrentlySelectedJoint,PosX,PosY);

	}
}
/*
void a()////////////////////////
void a()//// FunctionsText /////
void a()////////////////////////
*/


void RenderMainMenu_FunctionsText_App(float& PosX,float& PosY)
{
	unsigned long NewTime = 0;
	SceneLL* pScene = NULL;
	struct FieldLL* pField = NULL;
	struct FieldLL* pField2 = NULL;
	float StepY = IconWidth;
	float SaveX = PosX;
	struct RectLL* pRect = NULL;
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);


	PosX       = SaveX+IconWidth*2;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pTempText,"App : %cTechNature Animator%c",quote,quote);
		pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_App);
	}
	PosY  += StepY;


	if(bExpand_MainMenu_App)
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////
		//// FUNCTIONS : ////
		/////////////////////

		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// INSTANCES : Single APP ////
		////////////////////////////////

		PosX = SaveX;
		RenderMainMenu_InstancesText_App(pCurrentlySelectedApp,PosX,PosY);



/*
		//Search for : ADDAPPDATAHERE:

		/////////////////////
		//// View Matrix ////
		/////////////////////
		PosX = SaveX+IconWidth;
		pField = ClipInsertMenuItem("View Matrix",PosX,PosY,TextColor_App);
		PosY  += StepY;
		if(IsBitSet(bShow_ViewMatrix,1))
		{
			PosX = SaveX+(IconWidth*2);
			RenderMatrix(pRender_ViewMatrix,PosX,PosY);
			PosX = SaveX;
		}
		/////////////////////
		//// Proj Matrix ////
		/////////////////////
		PosX = SaveX+IconWidth;
		pField = ClipInsertMenuItem("Proj Matrix",PosX,PosY,TextColor_App);
		PosY  += StepY;
		if(IsBitSet(bShow_ProjMatrix,1))
		{
			PosX = SaveX+(IconWidth*2);
			RenderMatrix(pRender_ProjMatrix,PosX,PosY);
			PosX = SaveX;
		}
		//////////////////////
		//// Ortho Matrix ////
		//////////////////////
		PosX = SaveX+IconWidth;
		pField = ClipInsertMenuItem("Ortho Matrix",PosX,PosY,TextColor_App);
		PosY  += StepY;
		if(IsBitSet(bShow_OrthoMatrix,1))
		{
			PosX = SaveX+(IconWidth*2);
			RenderMatrix(pRender_OrthoMatrix,PosX,PosY);
			PosX = SaveX;
		}
		//sprintf(pTempText,"Scenes : %s",pCurrentlySelectedScene->pName);
*/
/*
		// Scenes : 
		PosX = SaveX+IconWidth;
		sprintf(pTempText,"Scenes");
		pField = ClipInsertMenuItem(pTempText,PosX,PosY,TextColor_Scene);

	
		PickDataView(LastSelectedPoint,PosY,PosY+IconWidth,&bExpand_MainMenu_Scenes,1,false);
//		PosX = SaveX+IconWidth;
		PosY  += StepY;

		//toggle [+]Scenes : "current scene"
		PosX = SaveX+IconWidth;
		if(bExpand_MainMenu_Scenes)
		{
			RenderMainMenu_FunctionsText_Scene(PosX,PosY);
		}
*/
	}//End if(bExpand_MainMenu_App)
}



void RenderMainMenu_FunctionsText_Scene(float& PosX,float& PosY)
{
	struct SceneLL* pScene = NULL;
	struct SceneLL* pOpenScene = NULL;
	struct FieldLL* pField = NULL;
	struct FieldLL* pField2 = NULL;
	float StepY = IconWidth;
	float SaveX = PosX;
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);


	PosX       = SaveX+IconWidth*2;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pTempText,"Scene : %c%s%c",quote,pCurrentlySelectedScene->pName,quote);
		pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Scene);
	}
	PosY  += StepY;

	if(bExpand_MainMenu_Scene)
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////
		//// FUNCTION : NewScene() ////
		///////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX = SaveX+IconWidth*2;
			sprintf(pTempText,"NewScene(                     )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Scene);
		
			PosX    = SaveX+IconWidth*7;
			pField2 = InsertEditableMenuItem(pNewSceneName,PosX,PosY,320.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pNewSceneName);	//pchar
			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					if(strlen(pNewSceneName)==0)
					{//check for no name
						OutputTextToConsole("error. enter a name for the new scene.",TextColor_ConsoleError);
						goto Label_Scene_NewSceneFail;
					}
					else
					{

						MenuFunction_Scene_NewScene(pNewSceneName);
					}

					Label_Scene_NewSceneFail:;
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : OpenScene() ////
		////////////////////////////////
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX = SaveX+IconWidth*2;
			sprintf(pTempText,"OpenScene(                     )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Scene);
		
			PosX    = SaveX+IconWidth*7;
			pField2 = InsertEditableMenuItem(pOpenSceneName,PosX,PosY,320.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pOpenSceneName);	//pchar

			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					pOpenScene = MenuFunction_Scene_OpenScene(pOpenSceneName);
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : SaveScene() ////
		////////////////////////////////
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"SaveScene(                      )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Scene);

			PosX    = SaveX+IconWidth*7;
			if(pCurrentlySelectedScene)
			{
				pField2 = InsertEditableMenuItem(pSaveSceneName,PosX,PosY,320.0,TextColor_Editable,
							NULL,			//pfloat
							NULL,			//plong
							NULL,			//pword
							pSaveSceneName);//pchar
			}

			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Scene_SaveScene(pSaveSceneName);
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////
		//// FUNCTION : CloseScene() ////
		/////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"CloseScene(                      )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Scene);


			PosX    = SaveX+IconWidth*7;
			sprintf(pTempText,"%s",pCurrentlySelectedScene->pName);
			pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_NotEditable);


			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Scene_CloseScene();
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////
		//// INSTANCES : Scenes ////
		////////////////////////////
		PosX = SaveX;
		RenderMainMenu_InstancesText_Scene(pCurrentlySelectedScene,PosX,PosY);
	}
/*

	PosX = SaveX;
	pScene = pRender_SceneHead;
	while(pScene)
	{
		PosX = SaveX;
		RenderMainMenu_InstancesText_Scene(pScene,PosX,PosY);
		pScene = pScene->pNext;
	}

*/
}

void RenderMainMenu_FunctionsText_Object(float& PosX,float& PosY)
{
	struct ObjectLL* pObject = NULL;
	struct ObjectLL* pOpenObject = NULL;
	struct FieldLL* pField = NULL;
	struct FieldLL* pField2 = NULL;
	float StepY = IconWidth;
	float SaveX = PosX;
	float StartY = PosY;
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);
	struct ObjectLL* pNewObject = NULL;


	PosX       = SaveX+IconWidth*2;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pTempText,"Object : %c%s%c",quote,pCurrentlySelectedObject->pName,quote);
		pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Object);
	}
	PosY  += StepY;

	//if parent is null, turn of this selection
	if(pCurrentlySelectedScene==NULL)
	{
		bExpand_MainMenu_Object = false;
		return;	
	}

	if(bExpand_MainMenu_Object)
	{

		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : NewObject() ////
		////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"NewObject(                    )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Object);
			
			PosX    = SaveX+IconWidth*7;
			pField2 = InsertEditableMenuItem(pNewObjectName,PosX,PosY,320.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pNewObjectName);//pchar
			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					if(strlen(pNewObjectName)==0)
					{//check for no name
						OutputTextToConsole("error. enter a name for the new Object.",TextColor_ConsoleError);
						goto Label_Object_NewObjectFail;
					}
					else
					{
						//check for existing Object with same name
/*
						pObject = pCurrentlySelectedScene->pObjectHead;
						while(pObject)
						{
							if(strcmp(pObject->pName,pNewObjectName)==0)
							{//remove this item
								OutputTextToConsole("error. Object with that name already exists.",TextColor_ConsoleError);
								goto Label_Object_NewObjectFail;
							}
							pObject = pObject->pNext;
						}
*/
//						pCurrentlySelectedObject = MenuFunction_Object_NewObject(pNewObjectName);
						MenuFunction_Object_NewObject(pNewObjectName);
					}

					Label_Object_NewObjectFail:;
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////
		//// FUNCTION : OpenObject() ////
		/////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"OpenObject(                    )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Object);
			
			PosX    = SaveX+IconWidth*7;
			pField2 = InsertEditableMenuItem(pOpenObjectName,PosX,PosY,320.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pOpenObjectName);//pchar
			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Object_OpenObject(pOpenObjectName);

					//Label_Object_OpenObjectFail:;
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		/////////////////////////////////
		//// FUNCTION : SaveObject() ////
		/////////////////////////////////
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"SaveObject(                     )");
			//pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Object);

			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Object);

			PosX    = SaveX+IconWidth*7;
			
			if(pCurrentlySelectedObject)
			{
				pField2 = InsertEditableMenuItem(pSaveObjectName,PosX,PosY,320.0,TextColor_Editable,
							NULL,			//pfloat
							NULL,			//plong
							NULL,			//pword
							pSaveObjectName);//pchar
			}

			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
														
					IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Object_SaveObject(pSaveObjectName);
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////
		//// FUNCTION : CloseObject() ////
		//////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
	//		sprintf(pTempText,"CloseObject( %s )",pCurrentlySelectedObject->pName);
			sprintf(pTempText,"CloseObject(                     )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Object);


			PosX    = SaveX+IconWidth*7;
			sprintf(pTempText,"%s",pCurrentlySelectedObject->pName);
			pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_NotEditable);


			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Object_CloseObject();
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;
		
		//// FUNCTION : InitBoneWeights() ////

		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////
		//// FUNCTION : InitBoneWeights() ////
		//////////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"InitBoneWeights()");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Object);

			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Object_InitBoneWeights();
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;
		
		//// FUNCTION : InitBoneWeights() ////






		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////
		//// FUNCTION : ReSortObjects() ////
		////////////////////////////////////


		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"ReSortObjects()");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Object);

			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Object_ReSortObjects();
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////
		//// INSTANCE : Object ////
		///////////////////////////
		PosX = SaveX;
  		RenderMainMenu_InstancesText_Object(pCurrentlySelectedObject,PosX,PosY);
	}//end if(bExpand_MainMenu_Object)
/*
	//////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////
	//// INSTANCES : Objects ////
	/////////////////////////////

	PosX = SaveX;
	pObject = pCurrentlySelectedScene->pObjectHead;
	while(pObject)
	{
		PosX = SaveX + IconWidth;
		RenderMainMenu_InstancesText_Object(pObject,PosX,PosY);
		pObject = pObject->pNext;
	}
*/
}


void RenderMainMenu_FunctionsText_Mesh(float& PosX,float& PosY)
{
	struct MeshLL* pMesh = NULL;
	struct MeshLL* pNewMesh = NULL;
	struct FieldLL* pField = NULL;
	struct FieldLL* pField2 = NULL;
	float StepY = IconWidth;
	float SaveX = PosX;
	float StartY = PosY;
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);

	PosX       = SaveX+IconWidth*2;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pTempText,"Mesh : %c%s%c",quote,pCurrentlySelectedMesh->pName,quote);
		pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Mesh);
	}
	PosY  += StepY;

	if(bExpand_MainMenu_Mesh)
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////
		//// FUNCTION : NewMesh() ////
		//////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"NewMesh(                     )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Mesh);
			
			PosX    = SaveX+IconWidth*7;
			pField2 = InsertEditableMenuItem(pNewMeshName,PosX,PosY,320.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pNewMeshName);	//pchar

			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					if(strlen(pNewMeshName)==0)
					{//check for no name
						OutputTextToConsole("error. enter a name for the new Mesh.",TextColor_ConsoleError);
						goto Label_Mesh_NewMeshFail;
					}
					else
					{
/*
						//check for existing Mesh with same name
						pMesh = pCurrentlySelectedObject->pMeshHead;
						while(pMesh)
						{
							if(strcmp(pMesh->pName,pNewMeshName)==0)
							{//remove this item
								OutputTextToConsole("error. Mesh with that name already exists.",TextColor_ConsoleError);
								goto Label_Mesh_NewMeshFail;
							}
							pMesh = pMesh->pNext;
						}
*/
						pCurrentlySelectedMesh = MenuFunction_Mesh_NewMesh(pNewMeshName);
					}

					Label_Mesh_NewMeshFail:;
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////
		//// FUNCTION : OpenMesh() ////
		///////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"OpenMesh(                     )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Mesh);
			
			PosX    = SaveX+IconWidth*7;
			pField2 = InsertEditableMenuItem(pOpenMeshName,PosX,PosY,320.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pOpenMeshName);

			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Mesh_OpenMesh(pOpenMeshName);
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;
		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : SaveMesh() ////
		////////////////////////////////
		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"SaveMesh(                      )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Mesh);

			PosX    = SaveX+IconWidth*7;
			if(pCurrentlySelectedMesh)
			{
				pField2 = InsertEditableMenuItem(pSaveMeshName,PosX,PosY,320.0,TextColor_Editable,
							NULL,			//pfloat
							NULL,			//plong
							NULL,			//pword
							pSaveMeshName);//pchar
			}

			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Mesh_SaveMesh(pSaveMeshName);
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped
		PosY  += StepY;


		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////
		//// FUNCTION : CloseMesh() ////
		////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			//sprintf(pTempText,"CloseMesh( %s )",pCurrentlySelectedMesh->pName);
			sprintf(pTempText,"CloseMesh(                      )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Mesh);


			PosX    = SaveX+IconWidth*7;
			sprintf(pTempText,"%s",pCurrentlySelectedMesh->pName);
			pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_NotEditable);


			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Mesh_CloseMesh();
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;


		//////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////
		//// FUNCTION : ReSortMeshes() ////
		///////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"ReSortMeshes()");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem(pTempText,PosX,PosY,TextColor_Mesh);

			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Mesh_ReSortMeshes();
					//pCurrentlySelectedMesh = MenuFunction_Mesh_SelectNext();
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////
		//// INSTANCE : Mesh ////
		/////////////////////////
		PosX = SaveX;
  		RenderMainMenu_InstancesText_Mesh(pCurrentlySelectedMesh,PosX,PosY);
/*
		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////
		//// INSTANCES : Meshes ////
		////////////////////////////
		PosX = SaveX;
		pMesh = pCurrentlySelectedObject->pMeshHead;
		while(pMesh)
		{
			PosX = SaveX+IconWidth;
			RenderMainMenu_InstancesText_Mesh(pMesh,PosX,PosY);
			pMesh = pMesh->pNext;
		}
*/
	}
}

void RenderMainMenu_FunctionsText_Skeleton(float& PosX,float& PosY)
{
	struct MeshLL* pMesh = NULL;
	struct MeshLL* pNewMesh = NULL;
	struct FieldLL* pField = NULL;
	struct FieldLL* pField2 = NULL;
	struct FieldLL* pField3 = NULL;
	struct FieldLL* pField4 = NULL;
	struct FieldLL* pField5 = NULL;
	struct FieldLL* pField6 = NULL;
	struct FieldLL* pField7 = NULL;
	float StepY = IconWidth;
	float SaveX = PosX;
	float StartY = PosY;
	//enum_FILE_TYPE SkeletonFileType;
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);

	PosX       = SaveX+IconWidth*2;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		sprintf(pTempText,"Skeleton : %c%s%c",quote,pCurrentlySelectedSkeleton->pName,quote);
		pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Skeleton);

	}
	PosY  += StepY;

	if(bExpand_MainMenu_Skeleton)
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////
		//// FUNCTION : NewSkeleton() ////
		//////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;

			sprintf(pTempText,"NewSkeleton(                            )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem2(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Skeleton);
			
			PosX    = SaveX+IconWidth*8;
			pField2 = InsertEditableMenuItem2(pNewSkeletonName,PosX,PosY,190.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pNewSkeletonName);

			PosX    = SaveX+IconWidth*14;
			pField3 = InsertEditableMenuItem2(pNewSkeletonParentName,PosX,PosY,190.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pNewSkeletonParentName);



			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					if(strlen(pNewSkeletonName)==0)
					{//check for no name
						OutputTextToConsole("error. enter a name for the new Skeleton.",TextColor_ConsoleError);
						goto Label_Skeleton_NewSkeletonFail;
					}
					else
					{
						MenuFunction_Skeleton_New(pNewSkeletonName,pNewSkeletonParentName);
					}

					Label_Skeleton_NewSkeletonFail:;
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////
		//// FUNCTION : OpenSkeleton() ////
		///////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"OpenSkeleton(               )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem2(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Skeleton);
			
			PosX    = SaveX+IconWidth*8;
			pField2 = InsertEditableMenuItem2(pOpenSkeletonName,PosX,PosY,220.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pOpenSkeletonName);

			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Skeleton_Open(pOpenSkeletonName);
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;

		//////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////
		//// FUNCTION : SaveSkeleton() ////
		///////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"SaveSkeleton(               )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem2(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Skeleton);
			
			PosX    = SaveX+IconWidth*8;
			pField2 = InsertEditableMenuItem2(pSaveSkeletonName,PosX,PosY,220.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pSaveSkeletonName);

			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
	//					OutputTextToConsole("ToDo : fix MenuFunction_Skeleton_",TextColor_ConsoleError);
					MenuFunction_Skeleton_Save(pSaveSkeletonName);
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;


		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////
		//// FUNCTION : CloseSkeleton() ////
		////////////////////////////////////
		static unsigned long CloseKeyFrameTime = 0;

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"CloseSkeleton(                  )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem2(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Skeleton);

			PosX    = SaveX+IconWidth*8;
			sprintf(pTempText,"%s",pCurrentlySelectedSkeleton->pName);
			pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_NotEditable);

			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Skeleton_Close();
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped
		PosY  += StepY;


		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////
		//// INSTANCE : Skeleton ////
		/////////////////////////////
		PosX = SaveX;
  		RenderMainMenu_InstancesText_Skeleton(pCurrentlySelectedSkeleton,PosX,PosY);

	}//end if(bExpand_MainMenu_Skeleton)
}



void RenderMainMenu_FunctionsText_KeyFrame(float& PosX,float& PosY)
{
	struct MeshLL* pMesh = NULL;
	struct MeshLL* pNewMesh = NULL;
	struct FieldLL* pField = NULL;
	struct FieldLL* pField2 = NULL;
	struct FieldLL* pField3 = NULL;
	struct FieldLL* pField4 = NULL;
	struct FieldLL* pField5 = NULL;
	struct FieldLL* pField6 = NULL;
	struct FieldLL* pField7 = NULL;
	float StepY = IconWidth;
	char pFieldText[256];
	float SaveX = PosX;
	float StartY = PosY;
//	enum_FILE_TYPE KeyFrameFileType;
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);

	PosX   = SaveX+IconWidth*2;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		if(pCurrentlySelectedKeyFrame)
		{
			if(pCurrentlySelectedKeyFrame->time==KEYFRAME_RESTPOSE)
			{
				sprintf(pTempText,"KeyFrame : (rest pose)");
			}
			else if(pCurrentlySelectedKeyFrame->time==KEYFRAME_CURRPOSE)
			{
				sprintf(pTempText,"KeyFrame : (curr pose)");
			}
			else
			{
				sprintf(pTempText,"KeyFrame : %u : %u",pCurrentlySelectedKeyFrame->anim_num,pCurrentlySelectedKeyFrame->time);
			}
		}
		else
		{
			sprintf(pTempText,"KeyFrame : %c(null)%c",quote,quote);
		}
		pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_KeyFrame);
	}
	PosY  += StepY;

	if(bExpand_MainMenu_KeyFrame)
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////
		//// FUNCTION : NewKeyFrame() ////
		//////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;

			sprintf(pTempText,"NewKeyFrame(                         )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem2(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_KeyFrame);
			
			PosX    = SaveX+IconWidth*9;
			sprintf(pFieldText,"%u",NewKeyFrame_anim_num);
			pField3 = InsertEditableMenuItem2(pFieldText,PosX,PosY,177.0,TextColor_Editable,
						NULL,			//pfloat
						&NewKeyFrame_anim_num,			//plong
						NULL,			//pword
						NULL);

			PosX    = SaveX+IconWidth*14;
			sprintf(pFieldText,"%u",NewKeyFrame_time);
			pField2 = InsertEditableMenuItem2(pFieldText,PosX,PosY,214.0,TextColor_Editable,
						NULL,			//pfloat
						&NewKeyFrame_time,			//plong
						NULL,			//pword
						NULL);


			/////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////
			//multi text line
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{
				PosX = SaveX+IconWidth*11;
				InsertHalfHeightMenuItemTop("  anim num" ,PosX,PosY,TextColor_KeyFrame);
				PosX = SaveX+IconWidth*18;
				InsertHalfHeightMenuItemTop("time ms" ,PosX,PosY,TextColor_KeyFrame);

			}
			/////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////







			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Skeleton_KeyFrame_New(NewKeyFrame_anim_num,NewKeyFrame_time);

//					Label_KeyFrame_NewKeyFrameFail:;
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;
















		//////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////
		//// FUNCTION : CopyKeyFrame() ////
		///////////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;

			sprintf(pTempText,"CopyKeyFrame(");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem2(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_KeyFrame);
			
			PosX    = SaveX+IconWidth*9;
			sprintf(pFieldText,"%u",CopyKeyFrame_From_anim_num);
			pField3 = InsertEditableMenuItem2(pFieldText,PosX,PosY,177.0,TextColor_Editable,
						NULL,			//pfloat
						&CopyKeyFrame_From_anim_num,//plong
						NULL,			//pword
						NULL);

			PosX    = SaveX+IconWidth*14;
			sprintf(pFieldText,"%u",CopyKeyFrame_From_time);
			pField2 = InsertEditableMenuItem2(pFieldText,PosX,PosY,214.0,TextColor_Editable,
						NULL,			//pfloat
						&CopyKeyFrame_From_time,//plong
						NULL,			//pword
						NULL);

			PosX    = SaveX+IconWidth*20;
			sprintf(pFieldText,"%u",CopyKeyFrame_To_anim_num);
			pField3 = InsertEditableMenuItem2(pFieldText,PosX,PosY,177.0,TextColor_Editable,
						NULL,			//pfloat
						&CopyKeyFrame_To_anim_num,//plong
						NULL,			//pword
						NULL);

			PosX    = SaveX+IconWidth*25;
			sprintf(pFieldText,"%u",CopyKeyFrame_To_time);
			pField2 = InsertEditableMenuItem2(pFieldText,PosX,PosY,214.0,TextColor_Editable,
						NULL,			//pfloat
						&CopyKeyFrame_To_time,//plong
						NULL,			//pword
						NULL);


			/////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////
			//multi text line
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{
				PosX = SaveX+IconWidth*10;
				InsertHalfHeightMenuItemTop("from anim num" ,PosX,PosY,TextColor_KeyFrame);
				PosX = SaveX+IconWidth*17;
				InsertHalfHeightMenuItemTop("from time ms" ,PosX,PosY,TextColor_KeyFrame);
				PosX = SaveX+IconWidth*22;
				InsertHalfHeightMenuItemTop("to anim num" ,PosX,PosY,TextColor_KeyFrame);
				PosX = SaveX+IconWidth*28;
				InsertHalfHeightMenuItemTop("to time ms" ,PosX,PosY,TextColor_KeyFrame);
			}
			/////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////







			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					MenuFunction_Skeleton_KeyFrame_Copy(CopyKeyFrame_From_anim_num,
														CopyKeyFrame_From_time,
														CopyKeyFrame_To_anim_num,
														CopyKeyFrame_To_time);

//					Label_KeyFrame_NewKeyFrameFail:;
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;




		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////
		//// FUNCTION : CloseKeyFrame() ////
		////////////////////////////////////
		static unsigned long CloseKeyFrameTime = 0;

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"CloseKeyFrame(                  )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem2(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_KeyFrame);

			PosX    = SaveX+IconWidth*9;
			if(pCurrentlySelectedKeyFrame)
			{
				sprintf(pTempText,"%u : %u",pCurrentlySelectedKeyFrame->anim_num,pCurrentlySelectedKeyFrame->time);
			}
			else
			{
				sprintf(pTempText,"%c(null)%c",quote,quote);
			}
			pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_NotEditable);

			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					if(pCurrentlySelectedKeyFrame)
					{
						MenuFunction_Skeleton_KeyFrame_Close(pCurrentlySelectedKeyFrame->anim_num,pCurrentlySelectedKeyFrame->time);
					}													
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped
		PosY  += StepY;


		//////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////
		//// INSTANCE : KeyFrame ////
		///////////////////////////
		PosX = SaveX;
  		RenderMainMenu_InstancesText_KeyFrame(pCurrentlySelectedKeyFrame,PosX,PosY);

	}//end if(bExpand_MainMenu_KeyFrame)
}

void RenderMainMenu_FunctionsText_Joint(float& PosX,float& PosY)
{
	struct MeshLL* pMesh = NULL;
	struct MeshLL* pNewMesh = NULL;
	struct FieldLL* pField = NULL;
	struct FieldLL* pField2 = NULL;
	struct FieldLL* pField3 = NULL;
	struct FieldLL* pField4 = NULL;
	struct FieldLL* pField5 = NULL;
	struct FieldLL* pField6 = NULL;
	struct FieldLL* pField7 = NULL;
	float StepY = IconWidth;
	char pFieldText[256];
	float SaveX = PosX;
	float StartY = PosY;
//	enum_FILE_TYPE KeyFrameFileType;
	float ClipTop;
	float ClipBottom;
	float PreStep = 0;//(MainMenuDataHeight*0.5);
	static float fNewJointPosX = 0.0;
	static float fNewJointPosY = 0.0;
	static float fNewJointPosZ = 0.0;



	PosX   = SaveX+IconWidth*2;
	ClipTop    = (PosY-PreStep);
	ClipBottom = (PosY-PreStep)+IconWidth;
	if(	ClipTop<Render_WindowHeight &&
		ClipBottom>(StepY*num_console_lines))
	{
		if(	pCurrentlySelectedJoint &&
			pCurrentlySelectedJoint->pInfoItem)
		{
		 	sprintf(pTempText,"Joint : %c%s%c",quote,pCurrentlySelectedJoint->pInfoItem->pName,quote);
		}
		else
		{
		 	sprintf(pTempText,"Joint : %c(null)%c",quote,quote);
		}
		pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Joint);
	}
	PosY  += StepY;

	if(bExpand_MainMenu_Joint)
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////
		//// FUNCTION : NewJoint() ////
		///////////////////////////////

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;

//			sprintf(pTempText,"NewJoint(                                                              )");
			sprintf(pTempText,"NewJoint(                                             )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem2(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Joint);
			
			PosX    = SaveX+IconWidth*7;
			pField2 = InsertEditableMenuItem2(pNewJointName,PosX,PosY,178.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pNewJointName);

			PosX    = SaveX+IconWidth*12;
			pField3 = InsertEditableMenuItem2(pNewJointParentName,PosX,PosY,176.0,TextColor_Editable,
						NULL,			//pfloat
						NULL,			//plong
						NULL,			//pword
						pNewJointParentName);


			PosX    = SaveX+IconWidth*17;
			sprintf(pFieldText,"%.2f",fNewJointPosX);
			pField3 = InsertEditableMenuItem2(pFieldText,PosX,PosY,106.0,TextColor_Editable,
						&fNewJointPosX,	//pfloat
						NULL, //plong
						NULL, //pword
						NULL);//pchar

			PosX    = SaveX+IconWidth*20;
			sprintf(pFieldText,"%.2f",fNewJointPosY);
			pField3 = InsertEditableMenuItem2(pFieldText,PosX,PosY,106.0,TextColor_Editable,
						&fNewJointPosY,	//pfloat
						NULL, //plong
						NULL, //pword
						NULL);//pchar

			PosX    = SaveX+IconWidth*23;
			sprintf(pFieldText,"%.2f",fNewJointPosZ);
			pField3 = InsertEditableMenuItem2(pFieldText,PosX,PosY,106.0,TextColor_Editable,
						&fNewJointPosZ,	//pfloat
						NULL, //plong
						NULL, //pword
						NULL);//pchar



			/////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////
			//multi text line
			ClipTop    = (PosY-PreStep);
			ClipBottom = (PosY-PreStep)+IconWidth;
			if(	ClipTop<Render_WindowHeight &&
				ClipBottom>(StepY*num_console_lines))
			{
/*
				PosX = SaveX+IconWidth*9;
				InsertHalfHeightMenuItemTop("  joint name" ,PosX,PosY,   SelectedTextColor_Object);
				PosX = SaveX+IconWidth*14;
				InsertHalfHeightMenuItemTop("parent name" ,PosX,PosY,   SelectedTextColor_Object);
				PosX = SaveX+IconWidth*19;
				InsertHalfHeightMenuItemTop("yaw" ,PosX,PosY,SelectedTextColor_Object);
				PosX = SaveX+IconWidth*22;
				InsertHalfHeightMenuItemTop("pitch" ,PosX,PosY,SelectedTextColor_Object);
				PosX = SaveX+IconWidth*25;
				InsertHalfHeightMenuItemTop(" roll" ,PosX,PosY,SelectedTextColor_Object);
				PosX = SaveX+IconWidth*27;
				InsertHalfHeightMenuItemTop("   posX" ,PosX,PosY,SelectedTextColor_Object);
				PosX = SaveX+IconWidth*30;
				InsertHalfHeightMenuItemTop("   posY" ,PosX,PosY,SelectedTextColor_Object);
				PosX = SaveX+IconWidth*33;
				InsertHalfHeightMenuItemTop("   posZ" ,PosX,PosY,SelectedTextColor_Object);


				PosX = SaveX+IconWidth*9;
				InsertHalfHeightMenuItemTop("  joint name" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*14;
				InsertHalfHeightMenuItemTop("parent name" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*19;
				InsertHalfHeightMenuItemTop("yaw" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*22;
				InsertHalfHeightMenuItemTop("pitch" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*25;
				InsertHalfHeightMenuItemTop(" roll" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*27;
				InsertHalfHeightMenuItemTop("   posX" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*30;
				InsertHalfHeightMenuItemTop("   posY" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*33;
				InsertHalfHeightMenuItemTop("   posZ" ,PosX,PosY,TextColor_Joint);
*/

				PosX = SaveX+IconWidth*9;
				InsertHalfHeightMenuItemTop("  joint name" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*14;
				InsertHalfHeightMenuItemTop("parent name" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*18;
				InsertHalfHeightMenuItemTop("   posX" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*21;
				InsertHalfHeightMenuItemTop("   posY" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*24;
				InsertHalfHeightMenuItemTop("   posZ" ,PosX,PosY,TextColor_Joint);
				PosX = SaveX+IconWidth*24;

			}
			/////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////


			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
				
					if(pCurrentlySelectedSkeleton)
					{
						//user specifies absolute data, not relative data
						MenuFunction_Skeleton_Joint_New(pCurrentlySelectedSkeleton,pNewJointName,pNewJointParentName,fNewJointPosX,fNewJointPosY,fNewJointPosZ,false);
					}
					else
					{
						OutputTextToConsole("error. Select a Skeleton first",TextColor_ConsoleError);
					}




//					Label_Joint_NewJointFail:;
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped

		PosY  += StepY;



		//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////
		//// FUNCTION : CloseJoint() ////
		/////////////////////////////////
		static unsigned long CloseJointTime = 0;

		ClipTop    = (PosY-PreStep);
		ClipBottom = (PosY-PreStep)+IconWidth;
		if(	ClipTop<Render_WindowHeight &&
			ClipBottom>(StepY*num_console_lines))
		{
			PosX   = SaveX+IconWidth*2;
			sprintf(pTempText,"CloseJoint(                  )");
			if(PickDataView_IsCursorOnThisLine(CurrentMousePosition,PosY,PosY+IconWidth))//mouseover
				pField = InsertMenuItem2(pTempText,PosX,PosY,MouseOverTextColor);
			else
				pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_Joint);

			PosX    = SaveX+IconWidth*8;
			if(pCurrentlySelectedJoint && pCurrentlySelectedJoint->pInfoItem)
			{
				sprintf(pTempText,"%s",pCurrentlySelectedJoint->pInfoItem->pName);
			}
			else
			{
			 	sprintf(pTempText,"%c(null)%c",quote,quote);
			}
			pField = InsertMenuItem2(pTempText,PosX,PosY,TextColor_NotEditable);


			////Execute Function on click
			if(bWM_LBUTTONDOWN)
			{
				if(PickDataView_IsCursorClickingFunction(	CurrentMousePosition,
															IconWidth*3,//left
															SaveX+IconWidth*6,//right
															PosY,//top
															PosY+IconWidth))//bottom
				{			
					if(pCurrentlySelectedJoint)
					{
						MenuFunction_Skeleton_Joint_Close();
					}													
					bWM_LBUTTONDOWN = false;
				}//End if function selected, then execute function
			}//End if(bWM_LBUTTONDOWN)
		}//End if not clipped
		PosY  += StepY;


		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////
		//// INSTANCE : Joint ////
		//////////////////////////
		PosX = SaveX;
  		RenderMainMenu_InstancesText_Joint(pCurrentlySelectedJoint,PosX,PosY);

	}//end if(bExpand_MainMenu_Joint)
}

/*
void a()///////////////////////////////////////
void a()//// TYPE SPECIFIC MENU FUNCTIONS /////
void a()///////////////////////////////////////
void a()//////////////////////
void a()//// Application /////
void a()//////////////////////
*/
/*
void a()///////////////
void a()//// Scene ////
void a()///////////////
*/



bool 		VerifyAssumptions_Scene()
{
	if(pCurrentlySelectedScene==pAppTools_Scene && !bCurrentlyLoadingSystemFiles)
	{
		OutputTextToConsole("error. you cant modify the System Scene",TextColor_ConsoleError);
		return NULL;
	}
	//this level must handle possibility of :
	//	pRender_SceneHead==NULL (needed for adding first Scene item)
	return true;
}
//this vould be a SceneLL function, but we want the error messages.
//VerifyNames_Scene Assumes VerifyAssumptions_Scene() has been called.
bool 	VerifyNames_Scene(char* pName)
{
	struct SceneLL* pPrev = NULL;
	struct SceneLL* pThis = NULL;
	struct SceneLL* pNext = NULL;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return false;
	}

	if(SceneLL_Get(pRender_SceneHead,pPrev,pThis,pNext,pName))
	{
		OutputTextToConsole("error. a Scene with that name already exists.",TextColor_ConsoleError);
		return false;
	}
	return true;
}
bool SceneAlreadyExists(char* pName)
{
	struct SceneLL* pPrev = NULL;
	struct SceneLL* pThis = NULL;
	struct SceneLL* pNext = NULL;

	if(SceneLL_Get(pRender_SceneHead,pPrev,pThis,pNext,pName))
	{
		return true;
	}
	return false;
}

SceneLL* 	MenuFunction_Scene_NewScene(char* pName)
{
	SceneLL* pScene = NULL;
	SceneLL* pPrev = NULL;
	SceneLL* pThis = NULL;
	SceneLL* pNext = NULL;

	// New, nad Open are the only permitted functions while system scene is selected.
	//if(!VerifyAssumptions_Scene())return NULL;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return NULL;
	}

	GetNamePath(pName,".scn");

	//Does it already exist?
	if(SceneAlreadyExists(gp_TextBuffer1))
	{
		OutputTextToConsole("error. a Scene with that name already exists in memory.",TextColor_ConsoleError);
		return NULL;
	}

	pScene = SceneLL_Insert(0,gp_TextBuffer1,pRender_SceneHead);

	sprintf(pTempText,"success. %s has been created",gp_TextBuffer1);
	OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);

	RefreshSubordinates(pScene);
	return pScene;
}

SceneLL* 	MenuFunction_Scene_OpenScene(char* pName)
{
	struct SceneLL* pScene = NULL;
	struct SceneLL* pNewScene = NULL;
	enum_FILE_TYPE SceneFileType;

	// New, nad Open are the only permitted functions while system scene is selected.
	//if(!VerifyAssumptions_Scene())return NULL;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return false;
	}

	//Use Extension to determine load format
	SceneFileType = (enum_FILE_TYPE)GetFileType(pName);
	RemoveExtensionFromFileName(pName);

	if(SceneFileType==FILE_TYPE_SCN || SceneFileType==FILE_TYPE_NDT)
	{
		GetNamePath(pName,".scn");

		if(SceneAlreadyExists(gp_TextBuffer1))
		{
			OutputTextToConsole("error. a Scene with that name already exists in memory.",TextColor_ConsoleError);
			return false;
		}

		pNewScene = SceneLL_Insert(0,gp_TextBuffer1,pRender_SceneHead);

		if(SceneLL_Load(pNewScene,gp_TextBuffer3))
		{
			pCurrentlySelectedScene = pNewScene;
			sprintf(pTempText,"success. %s has been opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
			RefreshSubordinates(pNewScene);
		}
		else
		{
			SceneLL_Remove(pNewScene,pRender_SceneHead);
			sprintf(pTempText,"error, %s was not opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleError);
			return NULL;
		}
	}
	return pNewScene;
}

bool 		MenuFunction_Scene_SaveScene(char* pName)
{
	if(!VerifyAssumptions_Scene())return NULL;

	GetNamePath(pName,".scn");

	//Rename Scene To Match SaveName
	sprintf(pCurrentlySelectedScene->pName,"%s",gp_TextBuffer1);

	if(SceneLL_Save(pCurrentlySelectedScene,gp_TextBuffer3))
	{
		sprintf(pTempText,"success. %s has been saved",gp_TextBuffer3);
		OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
		return true;
	}
	else
	{
		sprintf(pTempText,"error. %s was not saved",gp_TextBuffer3);
		OutputTextToConsole(pTempText,TextColor_ConsoleError);
		return false;
	}
}

SceneLL* 	MenuFunction_Scene_CloseScene()
{
	struct SceneLL* pNewScene = NULL;

	if(!VerifyAssumptions_Scene())return NULL;

	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. no Scene was selected",TextColor_ConsoleError);
		return NULL;
	}

	SceneLL_Remove(pCurrentlySelectedScene,pRender_SceneHead);

	return MenuFunction_Scene_SelectNext();
}

SceneLL*  	MenuFunction_Scene_SelectFirst()
{
	struct SceneLL* pPrev = NULL;
	struct SceneLL* pThis = NULL;
	struct SceneLL* pNext = NULL;
	struct SceneLL* pFirstScene = NULL;

	if(pRender_SceneHead==NULL)
	{//no items
		pCurrentlySelectedScene = NULL;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}
	if(pRender_SceneHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedScene = pRender_SceneHead;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}
	if(pCurrentlySelectedScene==NULL)
	{
		pCurrentlySelectedScene = pRender_SceneHead;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}

	SceneLL_GetFirst(pRender_SceneHead,pPrev,pThis,pNext);
	pFirstScene = pThis;

	//collapse current Scene if a new one is being selected.
	if(pCurrentlySelectedScene!=pFirstScene)
	{
		if(pCurrentlySelectedScene)ClearBit(pCurrentlySelectedScene->SceneDataViewFlags,SCENE_DATAVIEW_SHOW_THIS_SCENE);
	}

	pCurrentlySelectedScene = pFirstScene;
	RefreshSubordinates(pCurrentlySelectedScene);
	return pCurrentlySelectedScene;
}
SceneLL*  	MenuFunction_Scene_SelectPrev()
{
	struct SceneLL* pPrev = NULL;
	struct SceneLL* pThis = NULL;
	struct SceneLL* pNext = NULL;
	struct SceneLL* pPrevScene = NULL;

	if(pRender_SceneHead==NULL)
	{//no items
		pCurrentlySelectedScene = NULL;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}
	if(pRender_SceneHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedScene = pRender_SceneHead;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}
	if(pCurrentlySelectedScene==NULL)
	{
		pCurrentlySelectedScene = pRender_SceneHead;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}

	SceneLL_Get(pRender_SceneHead,pPrev,pThis,pNext,pCurrentlySelectedScene->pName);
	if(pPrev)
	{
		pPrevScene = pPrev;
	}
	else
	{
		SceneLL_GetLast(pRender_SceneHead,pPrev,pThis,pNext);
		pPrevScene = pThis;
	}

	//collapse current Scene if a new one is being selected.
	if(pCurrentlySelectedScene!=pPrevScene)
	{
		if(pCurrentlySelectedScene)ClearBit(pCurrentlySelectedScene->SceneDataViewFlags,SCENE_DATAVIEW_SHOW_THIS_SCENE);
	}

	pCurrentlySelectedScene = pPrevScene;
	RefreshSubordinates(pCurrentlySelectedScene);
	return pCurrentlySelectedScene;
}
SceneLL*  	MenuFunction_Scene_SelectNext()
{
	struct SceneLL* pPrev = NULL;
	struct SceneLL* pThis = NULL;
	struct SceneLL* pNext = NULL;
	struct SceneLL* pNextScene = NULL;
	//if pSH is null, then there arre no scenes

	if(pRender_SceneHead==NULL)
	{//no items
		pCurrentlySelectedScene = NULL;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}
	if(pRender_SceneHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedScene = pRender_SceneHead;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}
	if(pCurrentlySelectedScene==NULL)
	{
		pCurrentlySelectedScene = pRender_SceneHead;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}

	SceneLL_Get(pRender_SceneHead,pPrev,pThis,pNext,pCurrentlySelectedScene->pName);
	if(pNext)
	{
		pNextScene = pNext;
	}
	else
	{
		SceneLL_GetFirst(pRender_SceneHead,pPrev,pThis,pNext);
		pNextScene = pThis;
	}

	//collapse current Scene if a new one is being selected.
	if(pCurrentlySelectedScene!=pNextScene)
	{
		if(pCurrentlySelectedScene)ClearBit(pCurrentlySelectedScene->SceneDataViewFlags,SCENE_DATAVIEW_SHOW_THIS_SCENE);
	}

	pCurrentlySelectedScene = pNextScene;
	RefreshSubordinates(pCurrentlySelectedScene);
	return pCurrentlySelectedScene;
}

SceneLL*  	MenuFunction_Scene_SelectLast()
{
	struct SceneLL* pPrev = NULL;
	struct SceneLL* pThis = NULL;
	struct SceneLL* pNext = NULL;
	struct SceneLL* pLastScene = NULL;

	if(pRender_SceneHead==NULL)
	{//no items
		pCurrentlySelectedScene = NULL;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}
	if(pRender_SceneHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedScene = pRender_SceneHead;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}
	if(pCurrentlySelectedScene==NULL)
	{
		pCurrentlySelectedScene = pRender_SceneHead;
		RefreshSubordinates(pCurrentlySelectedScene);
		return pCurrentlySelectedScene;
	}

	SceneLL_GetLast(pRender_SceneHead,pPrev,pThis,pNext);
	pLastScene = pThis;

	//collapse current Scene if a new one is being selected.
	if(pCurrentlySelectedScene!=pLastScene)
	{
		if(pCurrentlySelectedScene)ClearBit(pCurrentlySelectedScene->SceneDataViewFlags,SCENE_DATAVIEW_SHOW_THIS_SCENE);
	}

	pCurrentlySelectedScene = pLastScene;
	RefreshSubordinates(pCurrentlySelectedScene);
	return pCurrentlySelectedScene;
}

/*
void a()////////////////
void a()//// Object ////
void a()////////////////
*/



bool 		VerifyAssumptions_Object()
{
	if(pCurrentlySelectedScene==pAppTools_Scene && !bCurrentlyLoadingSystemFiles)
	{
		OutputTextToConsole("error. you cant modify the System Scene",TextColor_ConsoleError);
		return NULL;
	}
	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. Select a Scene first",TextColor_ConsoleError);
		return false;
	}
	//this level must handle possibility of :
	//	pCurrentlySelectedScene->pObjectHead==NULL (needed for adding first Object item)
	return true;
}
//this vould be a ObjectLL function, but we want the error messages.
//VerifyNames_Object Assumes VerifyAssumptions_Object() has been called.
bool 		VerifyNames_Object(char* pName)
{
	struct ObjectLL* pPrev = NULL;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pNext = NULL;

	if(pCurrentlySelectedScene==NULL)return false;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return false;
	}

	if(ObjectLL_Get(pCurrentlySelectedScene->pObjectHead,pPrev,pThis,pNext,pName))
	{
		OutputTextToConsole("error. a Object with that name already exists.",TextColor_ConsoleError);
		return false;
	}
	return true;
}

bool ObjectAlreadyExists(char* pName)
{
	struct ObjectLL* pPrev = NULL;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pNext = NULL;

	if(pCurrentlySelectedScene==NULL)return false;
	if(ObjectLL_Get(pCurrentlySelectedScene->pObjectHead,pPrev,pThis,pNext,pName))
	{
		return true;
	}
	return false;
}




ObjectLL* 	MenuFunction_Object_NewObject(char* pName)
{
	ObjectLL* pObject = NULL;
	ObjectLL* pPrev = NULL;
	ObjectLL* pThis = NULL;
	ObjectLL* pNext = NULL;

	if(!VerifyAssumptions_Object())return NULL;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return NULL;
	}

	GetNamePath(pName,".obb");

	//Does it already exist?
	if(ObjectAlreadyExists(gp_TextBuffer1))
	{
		OutputTextToConsole("error. a Object with that name already exists in memory.",TextColor_ConsoleError);
		return NULL;
	}

	pObject = ObjectLL_Insert(0,gp_TextBuffer1,pCurrentlySelectedScene->pObjectHead);

	sprintf(pTempText,"success. %s has been created for %s",gp_TextBuffer1,pCurrentlySelectedScene->pName);
	OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);

	RefreshSubordinates(pObject);
	return pObject;
}

ObjectLL* 	MenuFunction_Object_OpenObject(char* pName)
{
	struct ObjectLL* pObject = NULL;
	struct ObjectLL* pNewObject = NULL;
	enum_FILE_TYPE ObjectFileType;

	if(!VerifyAssumptions_Object())return NULL;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return false;
	}

	//Use Extension to determine load format
	ObjectFileType = (enum_FILE_TYPE)GetFileType(pOpenObjectName);
	RemoveExtensionFromFileName(pOpenObjectName);

	if(ObjectFileType==FILE_TYPE_TXT)
	{
		//HACKHACK diablo has 92 joints
		//just draw bones!
		if(bstrcmp(pName,"diablo.txt"))
		{
			ClearBit(App_RenderFlags,APP_SHOWJOINTS);
		}

		GetNamePath(pName,".txt");

		if(ObjectAlreadyExists(gp_TextBuffer1))
		{
			OutputTextToConsole("error. a Object with that name already exists in memory.",TextColor_ConsoleError);
			return false;
		}

		pNewObject = ObjectLL_Insert(0,gp_TextBuffer1,pCurrentlySelectedScene->pObjectHead);

		if(Load_txt(gp_TextBuffer3,pNewObject,24.0,true))//false))
		{
			pCurrentlySelectedObject = pNewObject;
			sprintf(pTempText,"success. %s has been opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
			RefreshSubordinates(pNewObject);
		}
		else
		{
			ObjectLL_Remove(pNewObject,pCurrentlySelectedScene->pObjectHead);
			sprintf(pTempText,"error, %s was not opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleError);
			return NULL;
		}
	}
	if(ObjectFileType==FILE_TYPE_OBB || ObjectFileType==FILE_TYPE_NDT)
	{
		GetNamePath(pName,".obb");

		if(ObjectAlreadyExists(gp_TextBuffer1))
		{
			OutputTextToConsole("error. a Object with that name already exists in memory.",TextColor_ConsoleError);
			return false;
		}

		pNewObject = ObjectLL_Insert(0,pName,pCurrentlySelectedScene->pObjectHead);

		if(ObjectLL_Load(pCurrentlySelectedScene->pObjectHead,gp_TextBuffer3))
		{
			pCurrentlySelectedObject = pNewObject;
			sprintf(pTempText,"success. %s has been opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
			RefreshSubordinates(pNewObject);
		}
		else
		{
			ObjectLL_Remove(pNewObject,pCurrentlySelectedScene->pObjectHead);
			sprintf(pTempText,"error, %s was not opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleError);
			return NULL;
		}
	}
	return pNewObject;
}

bool 		MenuFunction_Object_SaveObject(char* pName)
{
	if(!VerifyAssumptions_Object())return NULL;

	GetNamePath(pName,".obb");

	//Rename Object To Match SaveName
	sprintf(pCurrentlySelectedObject->pName,"%s",gp_TextBuffer1);

	if(ObjectLL_Save(pCurrentlySelectedObject,gp_TextBuffer3))
	{
		sprintf(pTempText,"success %s has been saved",gp_TextBuffer3);
		OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
		return true;
	}
	else
	{
		sprintf(pTempText,"error, %s was not saved",gp_TextBuffer3);
		OutputTextToConsole(pTempText,TextColor_ConsoleError);
		return false;
	}
}

ObjectLL* 	MenuFunction_Object_CloseObject()
{
	if(!VerifyAssumptions_Object())return NULL;

	if(pCurrentlySelectedObject==NULL)
	{
		OutputTextToConsole("error. no Object was selected.",TextColor_ConsoleError);
		return NULL;
	}

	ObjectLL_Remove(pCurrentlySelectedObject,pCurrentlySelectedScene->pObjectHead);

	return MenuFunction_Object_SelectNext();
}

void MenuFunction_Object_InitBoneWeights()
{
	if(!VerifyAssumptions_Object())return;

	if(pCurrentlySelectedObject==NULL)
	{
		OutputTextToConsole("error. no Object was selected.",TextColor_ConsoleError);
		return;
	}

	Object_InitBoneWeights(pCurrentlySelectedObject);
}

void		MenuFunction_Object_ReSortObjects()
{
	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. Select a scene first",TextColor_ConsoleError);
		return;
	}

	if(	pCurrentlySelectedScene &&
		pCurrentlySelectedScene->pObjectHead)
	{
		ObjectLL_Sort(pCurrentlySelectedScene->pObjectHead);
		OutputTextToConsole("success. Objects re-sorted",TextColor_ConsoleSuccess);
	}
	else
	{
		if(pCurrentlySelectedScene->pObjectHead)
		{
			OutputTextToConsole("error. No Objects.",TextColor_ConsoleError);
		}
		else
		{
			OutputTextToConsole("error. Select Scene first.",TextColor_ConsoleError);
		}
	}
}


















ObjectLL*  	MenuFunction_Object_SelectFirst()
{
	struct ObjectLL* pPrev = NULL;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pNext = NULL;
	struct ObjectLL* pFirstObject = NULL;


	if(pCurrentlySelectedScene==NULL)
	{//no scene... no objects.
		pCurrentlySelectedObject = 	NULL;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedScene->pObjectHead==NULL)
	{//no items
		pCurrentlySelectedObject = 	NULL;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedScene->pObjectHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedObject = 	pCurrentlySelectedScene->pObjectHead;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		pCurrentlySelectedObject = 	pCurrentlySelectedScene->pObjectHead;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}

	ObjectLL_GetFirst(pCurrentlySelectedScene->pObjectHead,pPrev,pThis,pNext);
	pFirstObject = pThis;

	//collapse current Object if a new one is being selected.
	if(pCurrentlySelectedObject!=pFirstObject)
	{
		if(pCurrentlySelectedObject)ClearBit(pCurrentlySelectedObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_THIS_OBJECT);
	}

	pCurrentlySelectedObject = pFirstObject;
	RefreshSubordinates(pFirstObject);
	return pCurrentlySelectedObject;
}
ObjectLL*  	MenuFunction_Object_SelectPrev()
{
	struct ObjectLL* pPrev = NULL;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pNext = NULL;
	struct ObjectLL* pPrevObject = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no objects.
		pCurrentlySelectedObject = 	NULL;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedScene->pObjectHead==NULL)
	{//no items
		pCurrentlySelectedObject = 	NULL;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedScene->pObjectHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedObject = 	pCurrentlySelectedScene->pObjectHead;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		pCurrentlySelectedObject = 	pCurrentlySelectedScene->pObjectHead;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}

	ObjectLL_Get(pCurrentlySelectedScene->pObjectHead,pPrev,pThis,pNext,pCurrentlySelectedObject->pName);
	if(pPrev)
	{
		pPrevObject = pPrev;
	}
	else
	{
		ObjectLL_GetLast(pCurrentlySelectedScene->pObjectHead,pPrev,pThis,pNext);
		pPrevObject = pThis;
	}

	//collapse current Object if a new one is being selected.
	if(pCurrentlySelectedObject!=pPrevObject)
	{
		if(pCurrentlySelectedObject)ClearBit(pCurrentlySelectedObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_THIS_OBJECT);
	}

	pCurrentlySelectedObject = pPrevObject;
	RefreshSubordinates(pCurrentlySelectedObject);
	return pCurrentlySelectedObject;
}
ObjectLL*  	MenuFunction_Object_SelectNext()
{
	struct ObjectLL* pPrev = NULL;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pNext = NULL;
	struct ObjectLL* pNextObject = NULL;
	//if pSH is null, then there arre no Objects

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no objects.
		pCurrentlySelectedObject = 	NULL;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedScene->pObjectHead==NULL)
	{//no items
		pCurrentlySelectedObject = 	NULL;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedScene->pObjectHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedObject = 	pCurrentlySelectedScene->pObjectHead;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedObject==NULL)
	{//first selection gets first item
		pCurrentlySelectedObject = 	pCurrentlySelectedScene->pObjectHead;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}

	ObjectLL_Get(pCurrentlySelectedScene->pObjectHead,pPrev,pThis,pNext,pCurrentlySelectedObject->pName);
	if(pNext)
	{
		pNextObject = pNext;
	}
	else
	{
		ObjectLL_GetFirst(pCurrentlySelectedScene->pObjectHead,pPrev,pThis,pNext);
		pNextObject = pThis;
	}

	//collapse current Object if a new one is being selected.
	if(pCurrentlySelectedObject!=pNextObject)
	{
		if(pCurrentlySelectedObject)ClearBit(pCurrentlySelectedObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_THIS_OBJECT);
	}

	pCurrentlySelectedObject = pNextObject;
	RefreshSubordinates(pCurrentlySelectedObject);
	return pCurrentlySelectedObject;
}

ObjectLL*  	MenuFunction_Object_SelectLast()
{
	struct ObjectLL* pPrev = NULL;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pNext = NULL;
	struct ObjectLL* pLastObject = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no objects.
		pCurrentlySelectedObject =	NULL;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedScene->pObjectHead==NULL)
	{//no items
		pCurrentlySelectedObject =	NULL;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedScene->pObjectHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedObject = 	pCurrentlySelectedScene->pObjectHead;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		pCurrentlySelectedObject = 	pCurrentlySelectedScene->pObjectHead;
		RefreshSubordinates(pCurrentlySelectedObject);
		return pCurrentlySelectedObject;
	}

	ObjectLL_GetLast(pCurrentlySelectedScene->pObjectHead,pPrev,pThis,pNext);
	pLastObject = pThis;

	//collapse current Object if a new one is being selected.
	if(pCurrentlySelectedObject!=pLastObject)
	{
		if(pCurrentlySelectedObject)ClearBit(pCurrentlySelectedObject->ObjectDataViewFlags,OBJECT_DATAVIEW_SHOW_THIS_OBJECT);
	}

	pCurrentlySelectedObject = pLastObject;
	RefreshSubordinates(pLastObject);
	return pCurrentlySelectedObject;
}





/*





*/
/*
void a()//////////////
void a()//// Mesh ////
void a()//////////////
*/



bool 		VerifyAssumptions_Mesh()
{
	if(pCurrentlySelectedScene==pAppTools_Scene && !bCurrentlyLoadingSystemFiles)
	{
		OutputTextToConsole("error. you cant modify the System Scene",TextColor_ConsoleError);
		return NULL;
	}
	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. Select a Scene first",TextColor_ConsoleError);
		return false;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return false;
	}
	//this level must handle possibility of :
	//	pCurrentlySelectedObject->pMeshHead==NULL (needed for adding first Mesh item)
	return true;
}
//this vould be a MeshLL function, but we want the error messages.
//VerifyNames_Mesh Assumes VerifyAssumptions_Mesh() has been called.
bool 		VerifyNames_Mesh(char* pName)
{
	struct MeshLL* pPrev = NULL;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pNext = NULL;

	if(pCurrentlySelectedObject==NULL)return false;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return false;
	}

	if(MeshLL_Get(pCurrentlySelectedObject->pMeshHead,pPrev,pThis,pNext,pName))
	{
		OutputTextToConsole("error. a Mesh with that name already exists.",TextColor_ConsoleError);
		return false;
	}
	return true;
}
bool MeshAlreadyExists(char* pName)
{
	struct MeshLL* pPrev = NULL;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pNext = NULL;

	if(MeshLL_Get(pCurrentlySelectedObject->pMeshHead,pPrev,pThis,pNext,pName))
	{
		return true;
	}
	return false;
}

MeshLL* 	MenuFunction_Mesh_NewMesh(char* pName)
{
	MeshLL* pMesh = NULL;
	MeshLL* pPrev = NULL;
	MeshLL* pThis = NULL;
	MeshLL* pNext = NULL;

	if(!VerifyAssumptions_Mesh())return NULL;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return NULL;
	}

	GetNamePath(pName,".msh");

	//Does it already exist?
	if(MeshAlreadyExists(gp_TextBuffer1))
	{
		OutputTextToConsole("error. a Mesh with that name already exists in memory.",TextColor_ConsoleError);
		return NULL;
	}

	pMesh = MeshLL_Insert(0,gp_TextBuffer1,pCurrentlySelectedObject->pMeshHead);

	sprintf(pTempText,"success. %s has been created for %s",gp_TextBuffer1,pCurrentlySelectedObject->pName);
	OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);

	RefreshSubordinates(pMesh);
	return pMesh;
}

MeshLL* 	MenuFunction_Mesh_OpenMesh(char* pInName)
{
	struct MeshLL* pMesh = NULL;
	struct MeshLL* pNewMesh = NULL;
	enum_FILE_TYPE MeshFileType;

	char szName[1024] = {0,};
	strcpy(szName, pInName);
	char* pName = szName;

	if(!VerifyAssumptions_Mesh())return NULL;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return false;
	}

	//Use Extension to determine load format
	MeshFileType = (enum_FILE_TYPE)GetFileType(pName);
	RemoveExtensionFromFileName(pName);

	if(MeshFileType==FILE_TYPE_TXT)
	{
		GetNamePath(pName,".txt");

		if(MeshAlreadyExists(gp_TextBuffer1))
		{
			OutputTextToConsole("error. a Mesh with that name already exists in memory.",TextColor_ConsoleError);
			return false;
		}

		pNewMesh = MeshLL_Insert(0,gp_TextBuffer1,pCurrentlySelectedObject->pMeshHead);

		if(Load_txt(gp_TextBuffer3,pNewMesh))
		{
			sprintf(pTempText,"success. %s has been opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
			RefreshSubordinates(pNewMesh);
		}
		else
		{
			MeshLL_Remove(pNewMesh,pCurrentlySelectedObject->pMeshHead);
			sprintf(pTempText,"error, %s was not opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleError);
			return NULL;
		}
	}
	if(MeshFileType==FILE_TYPE_MSH || MeshFileType==FILE_TYPE_NDT)
	{
		GetNamePath(pName,".txt");

		if(MeshAlreadyExists(gp_TextBuffer1))
		{
			OutputTextToConsole("error. a Mesh with that name already exists in memory.",TextColor_ConsoleError);
			return false;
		}

		pNewMesh = MeshLL_Insert(0,pName,pCurrentlySelectedObject->pMeshHead);

		if(MeshLL_Load(pNewMesh,gp_TextBuffer3))
		{
			pCurrentlySelectedMesh = pNewMesh;
			sprintf(pTempText,"success. %s has been opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
			RefreshSubordinates(pNewMesh);
		}
		else
		{
			MeshLL_Remove(pNewMesh,pCurrentlySelectedObject->pMeshHead);
			sprintf(pTempText,"error, %s was not opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleError);
			return NULL;
		}

	}
	return pNewMesh;
}

bool 		MenuFunction_Mesh_SaveMesh(char* pName)
{
	if(!VerifyAssumptions_Mesh())return NULL;

	GetNamePath(pName,".msh");

	//Rename Mesh To Match SaveName
	sprintf(pCurrentlySelectedMesh->pName,"%s",gp_TextBuffer1);

	if(MeshLL_Save(pCurrentlySelectedMesh,gp_TextBuffer3))
	{
		sprintf(pTempText,"success. %s has been saved",gp_TextBuffer3);
		OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
		return true;
	}
	else
	{
		sprintf(pTempText,"error, %s was not saved",gp_TextBuffer3);
		OutputTextToConsole(pTempText,TextColor_ConsoleError);
		return false;
	}
}

MeshLL* 	MenuFunction_Mesh_CloseMesh()
{
	if(!VerifyAssumptions_Mesh())return NULL;

	if(pCurrentlySelectedMesh==NULL)
	{
		OutputTextToConsole("error. no Mesh was selected.",TextColor_ConsoleError);
		return false;
	}

	MeshLL_Remove(pCurrentlySelectedMesh,pCurrentlySelectedObject->pMeshHead);

	return MenuFunction_Mesh_SelectNext();
}

MeshLL*  	MenuFunction_Mesh_SelectFirst()
{
	struct MeshLL* pPrev = NULL;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pNext = NULL;
	struct MeshLL* pFirstMesh = NULL;


	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Meshs.
		pCurrentlySelectedMesh = 	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Meshs.
		pCurrentlySelectedMesh =	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject->pMeshHead==NULL)
	{//no items
		pCurrentlySelectedMesh = 	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject->pMeshHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedMesh = 	pCurrentlySelectedObject->pMeshHead;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedMesh==NULL)
	{
		pCurrentlySelectedMesh = 	pCurrentlySelectedObject->pMeshHead;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}

	MeshLL_GetFirst(pCurrentlySelectedObject->pMeshHead,pPrev,pThis,pNext);
	pFirstMesh = pThis;

	//collapse current Mesh if a new one is being selected.
	if(pCurrentlySelectedMesh!=pFirstMesh)
	{
		if(pCurrentlySelectedMesh)ClearBit(pCurrentlySelectedMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_THIS_MESH);
	}

	pCurrentlySelectedMesh = pFirstMesh;
	RefreshSubordinates(pCurrentlySelectedMesh);
	return pCurrentlySelectedMesh;
}
MeshLL*  	MenuFunction_Mesh_SelectPrev()
{
	struct MeshLL* pPrev = NULL;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pNext = NULL;
	struct MeshLL* pPrevMesh = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Meshs.
		pCurrentlySelectedMesh = 	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Meshs.
		pCurrentlySelectedMesh =	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject->pMeshHead==NULL)
	{//no items
		pCurrentlySelectedMesh = 	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject->pMeshHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedMesh = 	pCurrentlySelectedObject->pMeshHead;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedMesh==NULL)
	{
		pCurrentlySelectedMesh = 	pCurrentlySelectedObject->pMeshHead;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}

	MeshLL_Get(pCurrentlySelectedObject->pMeshHead,pPrev,pThis,pNext,pCurrentlySelectedMesh->pName);
	if(pPrev)
	{
		pPrevMesh = pPrev;
	}
	else
	{
		MeshLL_GetLast(pCurrentlySelectedObject->pMeshHead,pPrev,pThis,pNext);
		pPrevMesh = pThis;
	}

	//collapse current Mesh if a new one is being selected.
	if(pCurrentlySelectedMesh!=pPrevMesh)
	{
		if(pCurrentlySelectedMesh)ClearBit(pCurrentlySelectedMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_THIS_MESH);
	}

	pCurrentlySelectedMesh = pPrevMesh;
	RefreshSubordinates(pCurrentlySelectedMesh);
	return pCurrentlySelectedMesh;
}
MeshLL*  	MenuFunction_Mesh_SelectNext()
{
	struct MeshLL* pPrev = NULL;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pNext = NULL;
	struct MeshLL* pNextMesh = NULL;
	//if pSH is null, then there arre no Meshs

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Meshs.
		pCurrentlySelectedMesh = 	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Meshs.
		pCurrentlySelectedMesh =	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject->pMeshHead==NULL)
	{//no items
		pCurrentlySelectedMesh =	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject->pMeshHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedMesh = 	pCurrentlySelectedObject->pMeshHead;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedMesh==NULL)
	{
		pCurrentlySelectedMesh = 	pCurrentlySelectedObject->pMeshHead;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}

	MeshLL_Get(pCurrentlySelectedObject->pMeshHead,pPrev,pThis,pNext,pCurrentlySelectedMesh->pName);
	if(pNext)
	{
		pNextMesh = pNext;
	}
	else
	{
		MeshLL_GetFirst(pCurrentlySelectedObject->pMeshHead,pPrev,pThis,pNext);
		pNextMesh = pThis;
	}

	//collapse current Mesh if a new one is being selected.
	if(pCurrentlySelectedMesh!=pNextMesh)
	{
		if(pCurrentlySelectedMesh)ClearBit(pCurrentlySelectedMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_THIS_MESH);
	}

	pCurrentlySelectedMesh = pNextMesh;
	RefreshSubordinates(pCurrentlySelectedMesh);
	return pCurrentlySelectedMesh;
}

MeshLL*  	MenuFunction_Mesh_SelectLast()
{
	struct MeshLL* pPrev = NULL;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pNext = NULL;
	struct MeshLL* pLastMesh = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Meshs.
		pCurrentlySelectedMesh =	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Meshs.
		pCurrentlySelectedMesh =	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject->pMeshHead==NULL)
	{//no items
		pCurrentlySelectedMesh =	NULL;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedObject->pMeshHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedMesh = 	pCurrentlySelectedObject->pMeshHead;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}
	if(pCurrentlySelectedMesh==NULL)
	{
		pCurrentlySelectedMesh = 	pCurrentlySelectedObject->pMeshHead;
		RefreshSubordinates(pCurrentlySelectedMesh);
		return pCurrentlySelectedMesh;
	}

	MeshLL_GetLast(pCurrentlySelectedObject->pMeshHead,pPrev,pThis,pNext);
	pLastMesh = pThis;

	//collapse current Mesh if a new one is being selected.
	if(pCurrentlySelectedMesh!=pLastMesh)
	{
		if(pCurrentlySelectedMesh)ClearBit(pCurrentlySelectedMesh->MeshDataViewFlags,MESH_DATAVIEW_SHOW_THIS_MESH);
	}

	pCurrentlySelectedMesh = pLastMesh;
	RefreshSubordinates(pCurrentlySelectedMesh);
	return pCurrentlySelectedMesh;
}

void		MenuFunction_Mesh_ReSortMeshes()
{
	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. Select a Scene first",TextColor_ConsoleError);
		return;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return;
	}

	if(	pCurrentlySelectedObject &&
		pCurrentlySelectedObject->pMeshHead)
	{
		MeshLL_Sort(pCurrentlySelectedObject->pMeshHead);
		OutputTextToConsole("success. Meshes re-sorted",TextColor_ConsoleSuccess);
	}
	else
	{
		if(pCurrentlySelectedObject->pMeshHead)
		{
			OutputTextToConsole("error. No Meshes.",TextColor_ConsoleError);
		}
		else
		{
			OutputTextToConsole("error. Select Object first.",TextColor_ConsoleError);
		}
	}
}

/*
void a()//////////////////
void a()//// Skeleton ////
void a()//////////////////
*/

bool 		VerifyAssumptions_Skeleton()
{
	if(pCurrentlySelectedScene==pAppTools_Scene && !bCurrentlyLoadingSystemFiles)
	{
		OutputTextToConsole("error. you cant modify the System Scene",TextColor_ConsoleError);
		return NULL;
	}
	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. Select a Scene first",TextColor_ConsoleError);
		return false;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return false;
	}
	//this level must handle possibility of :
	//	pCurrentlySelectedObject->pSkeletonHead==NULL (needed for adding first Skeleton item)
	return true;
}
//this vould be a SkeletonLL function, but we want the error messages.
//VerifyNames_Skeleton Assumes VerifyAssumptions_Skeleton() has been called.
bool 		VerifyNames_Skeleton(char* pName,char* pParentName)
{
	struct SkeletonLL* pPrev = NULL;
	struct SkeletonLL* pThis = NULL;
	struct SkeletonLL* pNext = NULL;

	if(pCurrentlySelectedObject==NULL)return false;

	///////////////////////////////////////
	//// Check To See If Parent Exists ////
	///////////////////////////////////////

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return false;
	}

	if(!SkeletonLL_Get(pCurrentlySelectedObject->pSkeletonHead,pPrev,pThis,pNext,pParentName))
	{
		OutputTextToConsole("error. parent does not exist. check spelling.",TextColor_ConsoleError);
		return false;
	}

	/////////////////////////////////////////
	//// Check To See If Skeleton Exists ////
	/////////////////////////////////////////

	if(pName==NULL || strlen(pName)==0)
	{//no name
		OutputTextToConsole("error. enter a name for the new Skeleton.",TextColor_ConsoleError);
		return false;
	}

	if(strcmp(pName,"null")==0)
	{//no name
		OutputTextToConsole("error. null is a reserved name. Top level joints parent is identified as null.",TextColor_ConsoleError);
		return false;
	}

	if(SkeletonLL_Get(pCurrentlySelectedObject->pSkeletonHead,pPrev,pThis,pNext,pName))
	{
		OutputTextToConsole("error. a Skeleton with that name already exists.",TextColor_ConsoleError);
		return false;
	}
	return true;
}


bool SkeletonAlreadyExists(char* pName)
{
	struct SkeletonLL* pPrev = NULL;
	struct SkeletonLL* pThis = NULL;
	struct SkeletonLL* pNext = NULL;

	if(SkeletonLL_Get(pCurrentlySelectedObject->pSkeletonHead,pPrev,pThis,pNext,pName))
	{
		return true;
	}
	return false;
}


SkeletonLL*	MenuFunction_Skeleton_New(char* pName,char* pParentName)
{
	struct SkeletonLL* pPrev = NULL;
	struct SkeletonLL* pThis = NULL;
	struct SkeletonLL* pNext = NULL;
	struct SkeletonLL* pSkeleton = NULL;

	if(!VerifyAssumptions_Skeleton())return NULL;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return NULL;
	}

	GetNamePath(pName,".ske");

	//Does it already exist?
	if(SkeletonAlreadyExists(gp_TextBuffer1))
	{
		OutputTextToConsole("error. a Skeleton with that name already exists in memory.",TextColor_ConsoleError);
		return NULL;
	}

	pSkeleton = SkeletonLL_New(pCurrentlySelectedObject->pSkeletonHead,pName,pParentName);

	if(pSkeleton)
	{
		sprintf(pTempText,"success. %s's New Skeleton.",pCurrentlySelectedObject->pName);
		OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
	}
	else
	{
		OutputTextToConsole("failed. SkeletonLL_New.",TextColor_ConsoleError);
	}

	RefreshSubordinates(pSkeleton);
	return pSkeleton;
}

SkeletonLL*	MenuFunction_Skeleton_Open(char* pName)
{
	enum_FILE_TYPE SkeletonFileType;

	if(!VerifyAssumptions_Skeleton())return NULL;

	if(!IsValidFilename(pName))
	{
		OutputTextToConsole("error. Name provided was invalid.",TextColor_ConsoleError);
		return false;
	}

	//Use Extension to determine load format
	SkeletonFileType = (enum_FILE_TYPE)GetFileType(pOpenSkeletonName);
	RemoveExtensionFromFileName(pOpenSkeletonName);

	if(SkeletonFileType==FILE_TYPE_SKE || SkeletonFileType==FILE_TYPE_NDT)
	{
		GetNamePath(pName,".ske");

		if(SceneAlreadyExists(gp_TextBuffer1))
		{
			OutputTextToConsole("error. a Scene with that name already exists in memory.",TextColor_ConsoleError);
			return false;
		}
		//???
		SkeletonLL_CloseALL(pCurrentlySelectedObject->pSkeletonHead);

		//note Skeleton_Open calls Skeleton_Insert

		if(SkeletonLL_Open(pCurrentlySelectedObject->pSkeletonHead,gp_TextBuffer3))
		{
			ObjectLL_AllMeshs_SetBit(pCurrentlySelectedObject,RENDER_TRANSFORMED);
			sprintf(pTempText,"success. %s has been opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
			RefreshSubordinates(pCurrentlySelectedObject->pSkeletonHead);
		}
		else
		{
			sprintf(pTempText,"error, %s was not opened",gp_TextBuffer3);
			OutputTextToConsole(pTempText,TextColor_ConsoleError);
			return NULL;
		}

		return pCurrentlySelectedObject->pSkeletonHead;
	}
	
 	OutputTextToConsole("error. failed to load file.",TextColor_ConsoleError);
	return NULL;
}
void 		MenuFunction_Skeleton_Edit(	char* pName,
										char* pParentName,
										char* pParentJointName,
										long playing_anim_num)
{
	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. Select a Scene first",TextColor_ConsoleError);
		return;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return;
	}

	if(SkeletonLL_Edit(pCurrentlySelectedObject->pSkeletonHead,pName,pParentName,pParentJointName,playing_anim_num))
	{
		sprintf(pTempText,"success. %s has been edited",pName);
		OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
	}
	else
	{
		OutputTextToConsole("error. invalid inputs",TextColor_ConsoleError);
	}
}
bool		MenuFunction_Skeleton_Save(char* pName)
{
	if(!VerifyAssumptions_Skeleton())return NULL;

	GetNamePath(pName,".ske");

	//Rename SKeleton To Match SaveName
	sprintf(pCurrentlySelectedSkeleton->pName,"%s",gp_TextBuffer1);

	if(SkeletonLL_Save(pCurrentlySelectedSkeleton,gp_TextBuffer3))
	{
		sprintf(pTempText,"success. %s has been saved",gp_TextBuffer3);
		OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
		return true;
	}
	else
	{
		sprintf(pTempText,"error, %s was not saved",gp_TextBuffer3);
		OutputTextToConsole(pTempText,TextColor_ConsoleError);
		return false;
	}
}

void		MenuFunction_Skeleton_Close()
{
	char pCloseMeName[256];

	if(!VerifyAssumptions_Skeleton())return;

	if(pCurrentlySelectedSkeleton==NULL)
	{
		OutputTextToConsole("error. no Skeleton was selected.",TextColor_ConsoleError);
		return;
	}

	sprintf(pCloseMeName,"%s",pCurrentlySelectedSkeleton->pName);

	//Closing Current... Update CurrentlySelected Pointer now. this is why CloseMeName 
	if(pCurrentlySelectedSkeleton->pNext)
	{
		pCurrentlySelectedSkeleton = pCurrentlySelectedSkeleton->pNext;
	}
	else
	{
		pCurrentlySelectedSkeleton = pCurrentlySelectedObject->pSkeletonHead;
	}

			
	SkeletonLL_Close(pCurrentlySelectedObject->pSkeletonHead,pCloseMeName,true);

	sprintf(pTempText,"success. Skeleton named %s was closed.",pCloseMeName);
	OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);


}



SkeletonLL*  	MenuFunction_Skeleton_SelectFirst()
{
	struct SkeletonLL* pPrev = NULL;
	struct SkeletonLL* pThis = NULL;
	struct SkeletonLL* pNext = NULL;
	struct SkeletonLL* pFirstSkeleton = NULL;


	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Skeletons.
		pCurrentlySelectedSkeleton = 	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Skeletons.
		pCurrentlySelectedSkeleton =	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject->pSkeletonHead==NULL)
	{//no items
		pCurrentlySelectedSkeleton = 	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject->pSkeletonHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedSkeleton = 	pCurrentlySelectedObject->pSkeletonHead;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{
		pCurrentlySelectedSkeleton = 	pCurrentlySelectedObject->pSkeletonHead;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}

	SkeletonLL_GetFirst(pCurrentlySelectedObject->pSkeletonHead,pPrev,pThis,pNext);
	pFirstSkeleton = pThis;

	//collapse current Skeleton if a new one is being selected.
	if(pCurrentlySelectedSkeleton!=pFirstSkeleton)
	{
		if(pCurrentlySelectedSkeleton)ClearBit(pCurrentlySelectedSkeleton->SkeletonDataViewFlags,SKELETON_DATAVIEW_SHOW_THIS_SKELETON);
	}

	pCurrentlySelectedSkeleton = pFirstSkeleton;
	RefreshSubordinates(pCurrentlySelectedSkeleton);
	return pCurrentlySelectedSkeleton;
}
SkeletonLL*  	MenuFunction_Skeleton_SelectPrev()
{
	struct SkeletonLL* pPrev = NULL;
	struct SkeletonLL* pThis = NULL;
	struct SkeletonLL* pNext = NULL;
	struct SkeletonLL* pPrevSkeleton = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Skeletons.
		pCurrentlySelectedSkeleton = 	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Skeletons.
		pCurrentlySelectedSkeleton =	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject->pSkeletonHead==NULL)
	{//no items
		pCurrentlySelectedSkeleton = 	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject->pSkeletonHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedSkeleton = 	pCurrentlySelectedObject->pSkeletonHead;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{
		pCurrentlySelectedSkeleton = 	pCurrentlySelectedObject->pSkeletonHead;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}

	SkeletonLL_Get(pCurrentlySelectedObject->pSkeletonHead,pPrev,pThis,pNext,pCurrentlySelectedSkeleton->pName);
	if(pPrev)
	{
		pPrevSkeleton = pPrev;
	}
	else
	{
		SkeletonLL_GetLast(pCurrentlySelectedObject->pSkeletonHead,pPrev,pThis,pNext);
		pPrevSkeleton = pThis;
	}

	//collapse current Skeleton if a new one is being selected.
	if(pCurrentlySelectedSkeleton!=pPrevSkeleton)
	{
		if(pCurrentlySelectedSkeleton)ClearBit(pCurrentlySelectedSkeleton->SkeletonDataViewFlags,SKELETON_DATAVIEW_SHOW_THIS_SKELETON);
	}

	pCurrentlySelectedSkeleton = pPrevSkeleton;
	RefreshSubordinates(pCurrentlySelectedSkeleton);
	return pCurrentlySelectedSkeleton;
}
SkeletonLL*  	MenuFunction_Skeleton_SelectNext()
{
	struct SkeletonLL* pPrev = NULL;
	struct SkeletonLL* pThis = NULL;
	struct SkeletonLL* pNext = NULL;
	struct SkeletonLL* pNextSkeleton = NULL;
	//if pSH is null, then there arre no Skeletons

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Skeletons.
		pCurrentlySelectedSkeleton = 	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Skeletons.
		pCurrentlySelectedSkeleton =	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject->pSkeletonHead==NULL)
	{//no items
		pCurrentlySelectedSkeleton =	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject->pSkeletonHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedSkeleton = 	pCurrentlySelectedObject->pSkeletonHead;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{
		pCurrentlySelectedSkeleton = 	pCurrentlySelectedObject->pSkeletonHead;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}

	SkeletonLL_Get(pCurrentlySelectedObject->pSkeletonHead,pPrev,pThis,pNext,pCurrentlySelectedSkeleton->pName);
	if(pNext)
	{
		pNextSkeleton = pNext;
	}
	else
	{
		SkeletonLL_GetFirst(pCurrentlySelectedObject->pSkeletonHead,pPrev,pThis,pNext);
		pNextSkeleton = pThis;
	}

	//collapse current Skeleton if a new one is being selected.
	if(pCurrentlySelectedSkeleton!=pNextSkeleton)
	{
		if(pCurrentlySelectedSkeleton)ClearBit(pCurrentlySelectedSkeleton->SkeletonDataViewFlags,SKELETON_DATAVIEW_SHOW_THIS_SKELETON);
	}

	pCurrentlySelectedSkeleton = pNextSkeleton;
	RefreshSubordinates(pCurrentlySelectedSkeleton);
	return pCurrentlySelectedSkeleton;
}

SkeletonLL*  	MenuFunction_Skeleton_SelectLast()
{
	struct SkeletonLL* pPrev = NULL;
	struct SkeletonLL* pThis = NULL;
	struct SkeletonLL* pNext = NULL;
	struct SkeletonLL* pLastSkeleton = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Skeletons.
		pCurrentlySelectedSkeleton =	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Skeletons.
		pCurrentlySelectedSkeleton =	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject->pSkeletonHead==NULL)
	{//no items
		pCurrentlySelectedSkeleton =	NULL;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedObject->pSkeletonHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedSkeleton = 	pCurrentlySelectedObject->pSkeletonHead;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{
		pCurrentlySelectedSkeleton = 	pCurrentlySelectedObject->pSkeletonHead;
		RefreshSubordinates(pCurrentlySelectedSkeleton);
		return pCurrentlySelectedSkeleton;
	}

	SkeletonLL_GetLast(pCurrentlySelectedObject->pSkeletonHead,pPrev,pThis,pNext);
	pLastSkeleton = pThis;

	//collapse current Skeleton if a new one is being selected.
	if(pCurrentlySelectedSkeleton!=pLastSkeleton)
	{
		if(pCurrentlySelectedSkeleton)ClearBit(pCurrentlySelectedSkeleton->SkeletonDataViewFlags,SKELETON_DATAVIEW_SHOW_THIS_SKELETON);
	}

	pCurrentlySelectedSkeleton = pLastSkeleton;
	RefreshSubordinates(pCurrentlySelectedSkeleton);
	return pCurrentlySelectedSkeleton;
}


/*
void a()//////////////////
void a()//// KeyFrame ////
void a()//////////////////
*/

bool 		VerifyAssumptions_KeyFrame()
{
	if(pCurrentlySelectedScene==pAppTools_Scene && !bCurrentlyLoadingSystemFiles)
	{
		OutputTextToConsole("error. you cant modify the System Scene",TextColor_ConsoleError);
		return NULL;
	}
	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. Select a Scene first",TextColor_ConsoleError);
		return false;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return false;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return false;
	}
	//this level must handle possibility of :
	//	pCurrentlySelectedKeyFrame->pKeyFrameHead==NULL (needed for adding first keyframe item)
	return true;
}
//this vould be a KeyFrameLL function, but we want the error messages.
//VerifyNames_KeyFrame Assumes VerifyAssumptions_KeyFrame() has been called.
bool 		VerifyNames_KeyFrame(unsigned long anim_num,unsigned long time)
{
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;

	if(pCurrentlySelectedSkeleton==NULL)return false;

	/////////////////////
	//// Test Inputs ////
	/////////////////////

	if(time>KEYFRAME_MAXFRAME)
	{
		sprintf(pTempText,"error. time>KEYFRAME_MAXFRAME.");
		OutputTextToConsole(pTempText,TextColor_ConsoleError);
		return false;
	}

	//////////////////////////////////////////////
	//// Check To See KeyFrame Already Exists ////
	//////////////////////////////////////////////

	if(KeyFrameLL_Get(pCurrentlySelectedSkeleton->pKeyFrameHead,pPrev,pThis,pNext,anim_num,time))
	{
		OutputTextToConsole("error. a KeyFrame with that name already exists.",TextColor_ConsoleError);
		return false;
	}
	return true;
}
KeyFrameLL* MenuFunction_Skeleton_KeyFrame_New(unsigned long anim_num,unsigned long time)
{
	struct KeyFrameLL* pKeyFrame = NULL;
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;
	struct JointLL* pJoint = NULL;
	struct JointLL* pPrevKeyFramesJoint = NULL;
	struct JointLL* pNextKeyFramesJoint = NULL;

	if(!VerifyAssumptions_KeyFrame())return NULL;
	if(!VerifyNames_KeyFrame(anim_num,time))return NULL;

	pKeyFrame = Skeleton_KeyFrame_New(pCurrentlySelectedSkeleton,anim_num,time);
	if(pKeyFrame==NULL)
	{
		OutputTextToConsole("error : KeyFrameLL_New failed.",TextColor_ConsoleError);
		return NULL;
	}

	OutputTextToConsole("success : New KeyFrame.",TextColor_ConsoleSuccess);

	RefreshSubordinates(pKeyFrame);
	return pKeyFrame;
	
}

KeyFrameLL* MenuFunction_Skeleton_KeyFrame_Copy(unsigned long from_anim_num,unsigned long from_time,unsigned long to_anim_num,unsigned long to_time)
{
	struct KeyFrameLL* pSrc_KeyFrame = NULL;
	struct KeyFrameLL* pDst_KeyFrame = NULL;
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;
	struct JointLL* pJoint = NULL;
	struct JointLL* pPrevKeyFramesJoint = NULL;
	struct JointLL* pNextKeyFramesJoint = NULL;

	if(!VerifyAssumptions_KeyFrame())return NULL;




	/////////////////////
	//// Test Inputs ////
	/////////////////////

	if(from_time>KEYFRAME_MAXFRAME)
	{
		sprintf(pTempText,"error. from_time>KEYFRAME_MAXFRAME.");
		OutputTextToConsole(pTempText,TextColor_ConsoleError);
		return false;
	}

	if(to_time>KEYFRAME_MAXFRAME)
	{
		sprintf(pTempText,"error. to_time>KEYFRAME_MAXFRAME.");
		OutputTextToConsole(pTempText,TextColor_ConsoleError);
		return false;
	}

	//verify source
	if(!KeyFrameLL_Get(pCurrentlySelectedSkeleton->pKeyFrameHead,pPrev,pSrc_KeyFrame,pNext,from_anim_num,from_time))
	{
		OutputTextToConsole("error. source Keyframe does not exist.",TextColor_ConsoleError);
		return false;
	}

	//verify destination, and create if necessary
	if(!KeyFrameLL_Get(pCurrentlySelectedSkeleton->pKeyFrameHead,pPrev,pDst_KeyFrame,pNext,to_anim_num,to_time))
	{
		OutputTextToConsole("warning. destination Keyframe does not exist.",TextColor_ConsoleSuccess);
		pDst_KeyFrame = Skeleton_KeyFrame_New(pCurrentlySelectedSkeleton,to_anim_num,to_time);
		if(pDst_KeyFrame==NULL)
		{
			OutputTextToConsole("error. KeyFrameLL_Copy failed.",TextColor_ConsoleError);
			return false;
		}
		else
		{
			OutputTextToConsole("success. destination KeyFrame created.",TextColor_ConsoleSuccess);
		}
	}

	KeyFrame_Copy(pSrc_KeyFrame,pDst_KeyFrame);

	OutputTextToConsole("success : KeyFrame copied.",TextColor_ConsoleError);

	RefreshSubordinates(pDst_KeyFrame);
	return pDst_KeyFrame;
	
}

void 		MenuFunction_Skeleton_KeyFrame_Close(unsigned long anim_num,unsigned long time)
{
	struct KeyFrameLL* pKeyFrame = NULL;
	unsigned long close_anim_num;
	unsigned long close_time;

	if(!VerifyAssumptions_KeyFrame())return;


	if(pCurrentlySelectedKeyFrame==NULL)
	{
		OutputTextToConsole("error. no KeyFrame was selected.",TextColor_ConsoleError);
		return;
	}

	if(pCurrentlySelectedKeyFrame->time>KEYFRAME_MAXFRAME)
	{
		sprintf(pTempText,"error. You cant close RestPose, nor CurrPose Keyframes.");
		OutputTextToConsole(pTempText,TextColor_ConsoleError);
		return;
	}


	close_anim_num = pCurrentlySelectedKeyFrame->anim_num;
	close_time     = pCurrentlySelectedKeyFrame->time;

	//Closing Current... Update CurrentlySelected Pointer now. this is why CloseMeName 
	if(pCurrentlySelectedKeyFrame->pNext)
	{
		pKeyFrame = pCurrentlySelectedKeyFrame->pNext;
	}
	else
	{
		pKeyFrame = pCurrentlySelectedSkeleton->pKeyFrameHead;
	}

	if(KeyFrameLL_Close(pCurrentlySelectedSkeleton->pKeyFrameHead,close_anim_num,close_time))
	{
		pCurrentlySelectedKeyFrame = pKeyFrame;
		OutputTextToConsole("success : KeyFrame Closed.",TextColor_ConsoleSuccess);
	}
	else//error
	{
		OutputTextToConsole("error : Not Found.",TextColor_ConsoleError);
	}
}


KeyFrameLL*  	MenuFunction_Skeleton_KeyFrame_SelectFirst()
{
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;
	struct KeyFrameLL* pFirstKeyFrame = NULL;


	if(pCurrentlySelectedScene==NULL)
	{//no scene... no KeyFrames.
		pCurrentlySelectedKeyFrame = 	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no KeyFrames.
		pCurrentlySelectedKeyFrame =	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedKeyFrame =	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton->pKeyFrameHead==NULL)
	{//no items
		pCurrentlySelectedKeyFrame = 	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton->pKeyFrameHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedKeyFrame = 	pCurrentlySelectedSkeleton->pKeyFrameHead;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{
		pCurrentlySelectedKeyFrame = 	pCurrentlySelectedSkeleton->pKeyFrameHead;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}

	KeyFrameLL_GetFirst(pCurrentlySelectedSkeleton->pKeyFrameHead,pPrev,pThis,pNext);
	pFirstKeyFrame = pThis;

	//collapse current KeyFrame if a new one is being selected.
	if(pCurrentlySelectedKeyFrame!=pFirstKeyFrame)
	{
		if(pCurrentlySelectedKeyFrame)ClearBit(pCurrentlySelectedKeyFrame->KeyFrameDataViewFlags,KEYFRAME_DATAVIEW_SHOW_THIS_KEYFRAME);
	}

	pCurrentlySelectedKeyFrame = pFirstKeyFrame;
	RefreshSubordinates(pCurrentlySelectedKeyFrame);
	return pCurrentlySelectedKeyFrame;
}
KeyFrameLL*  	MenuFunction_Skeleton_KeyFrame_SelectPrev()
{
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;
	struct KeyFrameLL* pPrevKeyFrame = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no KeyFrames.
		pCurrentlySelectedKeyFrame = 	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no KeyFrames.
		pCurrentlySelectedKeyFrame =	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedKeyFrame =	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton->pKeyFrameHead==NULL)
	{//no items
		pCurrentlySelectedKeyFrame = 	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton->pKeyFrameHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedKeyFrame = 	pCurrentlySelectedSkeleton->pKeyFrameHead;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{
		pCurrentlySelectedKeyFrame = 	pCurrentlySelectedSkeleton->pKeyFrameHead;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}

	KeyFrameLL_Get(pCurrentlySelectedSkeleton->pKeyFrameHead,pPrev,pThis,pNext,pCurrentlySelectedKeyFrame->anim_num,pCurrentlySelectedKeyFrame->time);
	if(pPrev)
	{
		pPrevKeyFrame = pPrev;
	}
	else
	{
		KeyFrameLL_GetLast(pCurrentlySelectedSkeleton->pKeyFrameHead,pPrev,pThis,pNext);
		pPrevKeyFrame = pThis;
	}

	//collapse current KeyFrame if a new one is being selected.
	if(pCurrentlySelectedKeyFrame!=pPrevKeyFrame)
	{
		if(pCurrentlySelectedKeyFrame)ClearBit(pCurrentlySelectedKeyFrame->KeyFrameDataViewFlags,KEYFRAME_DATAVIEW_SHOW_THIS_KEYFRAME);
	}

	pCurrentlySelectedKeyFrame = pPrevKeyFrame;
	RefreshSubordinates(pCurrentlySelectedKeyFrame);
	return pCurrentlySelectedKeyFrame;
}
KeyFrameLL*  	MenuFunction_Skeleton_KeyFrame_SelectNext()
{
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;
	struct KeyFrameLL* pNextKeyFrame = NULL;
	//if pSH is null, then there arre no KeyFrames

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no KeyFrames.
		pCurrentlySelectedKeyFrame = 	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no KeyFrames.
		pCurrentlySelectedKeyFrame =	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedKeyFrame =	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton->pKeyFrameHead==NULL)
	{//no items
		pCurrentlySelectedKeyFrame = 	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton->pKeyFrameHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedKeyFrame = 	pCurrentlySelectedSkeleton->pKeyFrameHead;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{
		pCurrentlySelectedKeyFrame = 	pCurrentlySelectedSkeleton->pKeyFrameHead;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}

	KeyFrameLL_Get(pCurrentlySelectedSkeleton->pKeyFrameHead,pPrev,pThis,pNext,pCurrentlySelectedKeyFrame->anim_num,pCurrentlySelectedKeyFrame->time);
	if(pNext)
	{
		pNextKeyFrame = pNext;
	}
	else
	{
		KeyFrameLL_GetFirst(pCurrentlySelectedSkeleton->pKeyFrameHead,pPrev,pThis,pNext);
		pNextKeyFrame = pThis;
	}

	//collapse current KeyFrame if a new one is being selected.
	if(pCurrentlySelectedKeyFrame!=pNextKeyFrame)
	{
		if(pCurrentlySelectedKeyFrame)ClearBit(pCurrentlySelectedKeyFrame->KeyFrameDataViewFlags,KEYFRAME_DATAVIEW_SHOW_THIS_KEYFRAME);
	}

	pCurrentlySelectedKeyFrame = pNextKeyFrame;
	RefreshSubordinates(pCurrentlySelectedKeyFrame);
	return pCurrentlySelectedKeyFrame;
}

KeyFrameLL*  	MenuFunction_Skeleton_KeyFrame_SelectLast()
{
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;
	struct KeyFrameLL* pLastKeyFrame = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no KeyFrames.
		pCurrentlySelectedKeyFrame = 	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no KeyFrames.
		pCurrentlySelectedKeyFrame =	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedKeyFrame =	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton->pKeyFrameHead==NULL)
	{//no items
		pCurrentlySelectedKeyFrame = 	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton->pKeyFrameHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedKeyFrame = 	pCurrentlySelectedSkeleton->pKeyFrameHead;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{
		pCurrentlySelectedKeyFrame = 	pCurrentlySelectedSkeleton->pKeyFrameHead;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}

	KeyFrameLL_GetLast(pCurrentlySelectedSkeleton->pKeyFrameHead,pPrev,pThis,pNext);
	pLastKeyFrame = pThis;

	//collapse current KeyFrame if a new one is being selected.
	if(pCurrentlySelectedKeyFrame!=pLastKeyFrame)
	{
		if(pCurrentlySelectedKeyFrame)ClearBit(pCurrentlySelectedKeyFrame->KeyFrameDataViewFlags,KEYFRAME_DATAVIEW_SHOW_THIS_KEYFRAME);
	}

	pCurrentlySelectedKeyFrame = pLastKeyFrame;
	RefreshSubordinates(pCurrentlySelectedKeyFrame);
	return pCurrentlySelectedKeyFrame;
}



KeyFrameLL*  	MenuFunction_Skeleton_KeyFrame_SelectRestPose()
{
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;
	struct KeyFrameLL* pLastKeyFrame = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no KeyFrames.
		pCurrentlySelectedKeyFrame = 	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no KeyFrames.
		pCurrentlySelectedKeyFrame =	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedKeyFrame =	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton->pKeyFrameHead==NULL)
	{//no items
		pCurrentlySelectedKeyFrame = 	NULL;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedSkeleton->pKeyFrameHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedKeyFrame = 	pCurrentlySelectedSkeleton->pKeyFrameHead;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{
		pCurrentlySelectedKeyFrame = 	pCurrentlySelectedSkeleton->pKeyFrameHead;
		RefreshSubordinates(pCurrentlySelectedKeyFrame);
		return pCurrentlySelectedKeyFrame;
	}

	KeyFrameLL_GetRestPose(pCurrentlySelectedSkeleton->pKeyFrameHead,pPrev,pThis,pNext);
	pLastKeyFrame = pThis;

	//collapse current KeyFrame if a new one is being selected.
	if(pCurrentlySelectedKeyFrame!=pLastKeyFrame)
	{
		if(pCurrentlySelectedKeyFrame)ClearBit(pCurrentlySelectedKeyFrame->KeyFrameDataViewFlags,KEYFRAME_DATAVIEW_SHOW_THIS_KEYFRAME);
	}

	pCurrentlySelectedKeyFrame = pLastKeyFrame;
	RefreshSubordinates(pCurrentlySelectedKeyFrame);
	return pCurrentlySelectedKeyFrame;
}







/*
void a()//////////////////
void a()////  Joints  ////
void a()//////////////////
*/


bool 		VerifyAssumptions_Joint()
{
	if(pCurrentlySelectedScene==pAppTools_Scene && !bCurrentlyLoadingSystemFiles)
	{
		OutputTextToConsole("error. you cant modify the System Scene",TextColor_ConsoleError);
		return NULL;
	}
	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. Select a Scene first",TextColor_ConsoleError);
		return false;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return false;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{
		OutputTextToConsole("error. Select a Skeleton first",TextColor_ConsoleError);
		return false;
	}
	//this level must handle possibility of :
	//	pCurrentlySelectedKeyFrame->pJointHead==NULL (needed for adding first joint item)
	return true;
}
//this vould be a JointLL function, but we want the error messages.
//VerifyNames_Joint Assumes VerifyAssumptions_Joint() has been called.
bool 		VerifyNames_Joint(char* pName,char* pParentName)
{
	struct JointLL* pPrev = NULL;
	struct JointLL* pThis = NULL;
	struct JointLL* pNext = NULL;

//	if(pCurrentlySelectedKeyFrame==NULL)return false;

	///////////////////////////////////////
	//// Check To See If Parent Exists ////
	///////////////////////////////////////

	if(pParentName==NULL || strlen(pParentName)==0)
	{//no parent name. top level bone uses "null" as parent
		OutputTextToConsole("error. enter a name for the parent Joint. Use null as parent name for top level Joints.",TextColor_ConsoleError);
		return false;
	}

	//todo fix check for exxisting parent joint in RPKF
	if(!JointLL_Get(pCurrentlySelectedKeyFrame->pJointHead,pPrev,pThis,pNext,pParentName))
	{
		OutputTextToConsole("error. parent does not exist. check spelling.",TextColor_ConsoleError);
		return false;
	}

	//////////////////////////////////////
	//// Check To See If Joint Exists ////
	//////////////////////////////////////

	if(pName==NULL || strlen(pName)==0)
	{//no name
		OutputTextToConsole("error. enter a name for the new Joint.",TextColor_ConsoleError);
		return false;
	}

	if(strcmp(pName,"null")==0)
	{//no name
		OutputTextToConsole("error. null is a reserved name. Top level joints parent is identified as null.",TextColor_ConsoleError);
		return false;
	}

	//todo fix check for exxisting joint in RPKF
	if(JointLL_Get(pCurrentlySelectedKeyFrame->pJointHead,pPrev,pThis,pNext,pName))
	{
		OutputTextToConsole("error. a Joint with that name already exists.",TextColor_ConsoleError);
		return false;
	}

  return true;
}

//________________________________________________________________________
//>Joints_New(SkeletonIn)
//	For skeleton, 
//		get RP keyframe
//			insert joint at end of childern of the parent
//			keep pointer to pInfoItem
//		For each keyframe xcept RP
//			insert joint at end of childern of the parent
//			point to RP's pInfoItem
//
JointLL*	MenuFunction_Skeleton_Joint_New(	
									struct SkeletonLL* pSkeleton,
									char* pName,
									char* pParentName,
									float PosX,
									float PosY,
									float PosZ,
									bool  bRelativeData)
{
	struct KeyFrameLL* pKeyFrameHead = NULL;
	struct KeyFrameLL* pKeyFrame = NULL;
	struct JointLL* pRestPose_Joint = NULL;
	struct JointLL* pJoint = NULL;
	struct JointLL* pPrev = NULL;
	struct JointLL* pThis = NULL;
	struct JointLL* pNext = NULL;
	unsigned long num_keyframes = 0;


	if(!VerifyAssumptions_Joint())return NULL;
	if(!VerifyNames_Joint(pName,pParentName))return NULL;

	//Set KFRP?

	////////////////////////////////////
	//// Add Joint To Each KeyFrame ////
	////////////////////////////////////

	pRestPose_Joint = Skeleton_Joint_New(pSkeleton,pName,pParentName);

	/////////////////////////////////////////
	//// Position, and Rotate Each Joint ////[the same]
	/////////////////////////////////////////

	pKeyFrame = pSkeleton->pKeyFrameHead;
	while(pKeyFrame)
	{
		if(JointLL_Get(pKeyFrame->pJointHead,pPrev,pJoint,pNext,pName))
		{

			if(bRelativeData)
			{
				PositionMatrix(pJoint->pRelativeMatrix,PosX,PosY,PosZ);
			}
			else
			{
				PositionMatrix(pJoint->pAbsoluteMatrix,PosX,PosY,PosZ);
			}
			#ifdef USE_QUATERNIONS
			PositionQuaternion(pJoint->Absolute_Position,PosX,PosY,PosZ);
			#endif//USE_QUATERNIONS
		}
		else
		{
			OutputTextToConsole("error. Recently Added Joint Is Missing.",TextColor_ConsoleError);
		}
//WHY DOES THIS BREAK BEND@.TXT??
//IT WORKS GREAT FOR JOINTNEW
		if(!bRelativeData)
		{
			KeyFrame_TransformJoints_AbsoluteToRelative(pKeyFrame,pRender_IdentityMatrix);//RP uses Identity
		}

		num_keyframes++;
		pKeyFrame = pKeyFrame->pNext;
	}

	sprintf(pTempText,"success. the Joint was added to %d KeyFrames",num_keyframes);
	OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);

	RefreshSubordinates(pSkeleton->pRestPose);//set RPKF, and new Joint
	RefreshSubordinates(pRestPose_Joint);
	return pRestPose_Joint;
}


void 		MenuFunction_Skeleton_Joint_Edit()
{

	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. Select a Scene first",TextColor_ConsoleError);
		return;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return;
	}
}




//CloseJoint must fix the Hiearchy when a Joint is removed.
void 		MenuFunction_Skeleton_Joint_Close()
{
	char CloseMeName[512];
	struct KeyFrameLL* pKeyFrame = NULL;
	struct JointLL* pPrev = NULL;
	struct JointLL* pThis = NULL;
	struct JointLL* pNext = NULL;
	////////////////////////////
	//// Verify Assumptions ////
	////////////////////////////

	if(!VerifyAssumptions_Joint())return;

	if(pCurrentlySelectedJoint==NULL)
	{
		OutputTextToConsole("error. Select a Joint first",TextColor_ConsoleError);
		return;
	}
	if(pCurrentlySelectedJoint->pInfoItem==NULL)
	{
		OutputTextToConsole("error. pCurrentlySelectedJoint->pInfoItem null",TextColor_ConsoleError);
		return;
	}

	//pCurrentlySelectedJoint will be advanced before the joint is actually removed, so make a copy of the name of joint to be removed.
	sprintf(CloseMeName,"%s",pCurrentlySelectedJoint->pInfoItem->pName);

	//////////////////////////////////////
	//// Check To See If Joint Exists ////
	//////////////////////////////////////
					  
	if(!JointLL_Get(pCurrentlySelectedKeyFrame->pJointHead,pPrev,pThis,pNext,CloseMeName))
	{
		OutputTextToConsole("error. Joint Doesnt Exist.",TextColor_ConsoleError);
		return;
	}

	//Closing Current... Update CurrentlySelected Pointer now. this is why CloseMeName 
	if(pCurrentlySelectedJoint->pNext)
	{
		pCurrentlySelectedJoint = pCurrentlySelectedJoint->pNext;
	}
	else
	{
		pCurrentlySelectedJoint = pCurrentlySelectedKeyFrame->pJointHead;
	}




	pKeyFrame = pCurrentlySelectedSkeleton->pKeyFrameHead;
	while(pKeyFrame)
	{
		if(!JointLL_Close(pKeyFrame->pJointHead,CloseMeName))
		{
			OutputTextToConsole("error. JointLL_Close failed : ertert",TextColor_ConsoleError);
		}
		pKeyFrame = pKeyFrame->pNext;
	}

/*
	if(pCurrentlySelectedJoint==NULL)
	{
		OutputTextToConsole("error. No Joint is selected.",TextColor_ConsoleError);
		return;
	}

	if(strcmp(pCurrentlySelectedJoint->pName,"SkeletonRootNode")==0)
	{
		OutputTextToConsole("error. You may not close SkeletonRootNode.",TextColor_ConsoleError);
		return;
	}	
	
	struct JointLL* pThis        = NULL;
	struct JointLL* pRemoveMe    = pCurrentlySelectedJoint;
	struct JointLL* pParent      = pCurrentlySelectedJoint->pParent;
	struct JointLL* pChildren    = pCurrentlySelectedJoint->pChildren;
	struct JointLL* pNextSibling = pCurrentlySelectedJoint->pNextSibling;

	assert(pParent);//no parent prob me ns root node, but we checked for that...




	if(pParent->pChildren == pRemoveMe)
	{//removing first child
		if(pRemoveMe->pChildren)
		{//pRemoveMe's child assumes position of pRemoveMe Joint.
			pParent->pChildren = pRemoveMe->pChildren;
			if(pRemoveMe->pChildren)
			{
				pRemoveMe->pChildren->pParent = pParent;
			}
			//If Joint to be erased has siblings,
			//add the Siblings to the end its parents siblings
			//find end of parents siblings list
			pThis = pRemoveMe->pChildren;
			while(pThis->pNextSibling)
			{
				pThis = pThis->pNextSibling;
			}
			pThis->pNextSibling = pRemoveMe->pNextSibling; 
		}
		else
		{//pRemoveMe's NextSibling assumes position of pRemoveMe Joint.
			//child assumes position of removed Joint.
			pParent->pChildren = pRemoveMe->pNextSibling;

			if(pRemoveMe->pNextSibling)
			{
				pRemoveMe->pNextSibling->pParent = pParent;
			}
		}
	}
	else
	{//pRemoveMe is a Sibling
		if(pRemoveMe->pChildren)
		{//pRemoveMe's child assumes position of pRemoveMe Joint.
			//Find previous Sibling. Possibly the first child.
			pThis = pRemoveMe->pParent->pChildren;
			while(pThis->pNextSibling != pRemoveMe)
			{
				pThis = pThis->pNextSibling;
			}
			//pThis now points to the previous sibling
			pThis->pNextSibling = pRemoveMe->pChildren;


			pThis = pRemoveMe->pChildren;
			while(pThis->pNextSibling)
			{
				pThis = pThis->pNextSibling;
			}
			pThis->pNextSibling = pRemoveMe->pNextSibling;
		}
		else
		{//pRemoveMe's NextSibling assumes position of pRemoveMe Joint.
			//Find previous Sibling. Possibly the first child.
			pThis = pRemoveMe->pParent->pChildren;
			while(pThis->pNextSibling != pRemoveMe)
			{
				pThis = pThis->pNextSibling;
			}
			//pThis now points to the previous sibling
			pThis->pNextSibling = pRemoveMe->pNextSibling;
		}
	}

	// Remove pRemoveMe
	KeyFrameLL_RemoveALL(pRemoveMe->pKeyFrameHead);
	SAFE_FREE(pRemoveMe);


	MenuFunction_Skeleton_SelectNextJoint();
*/
}









JointLL*  	MenuFunction_Skeleton_Joint_SelectFirst()
{
	struct JointLL* pPrev = NULL;
	struct JointLL* pThis = NULL;
	struct JointLL* pNext = NULL;
	struct JointLL* pFirstJoint = NULL;


	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Joints.
		pCurrentlySelectedJoint = 	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Joints.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame->pJointHead==NULL)
	{//no items
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame->pJointHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedJoint = 	pCurrentlySelectedKeyFrame->pJointHead;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedJoint==NULL)
	{
		pCurrentlySelectedJoint = 	pCurrentlySelectedKeyFrame->pJointHead;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}

	JointLL_GetFirst(pCurrentlySelectedKeyFrame->pJointHead,pPrev,pThis,pNext);
	pFirstJoint = pThis;

	//collapse current Joint if a new one is being selected.
	if(pCurrentlySelectedJoint!=pFirstJoint)
	{
		if(pCurrentlySelectedJoint)ClearBit(pCurrentlySelectedJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_THIS_JOINT);
	}

	pCurrentlySelectedJoint = pFirstJoint;
	RefreshSubordinates(pCurrentlySelectedJoint);
	return pCurrentlySelectedJoint;
}
JointLL*  	MenuFunction_Skeleton_Joint_SelectPrev()
{
	struct JointLL* pPrev = NULL;
	struct JointLL* pThis = NULL;
	struct JointLL* pNext = NULL;
	struct JointLL* pPrevJoint = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Joints.
		pCurrentlySelectedJoint = 	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Joints.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame->pJointHead==NULL)
	{//no items
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame->pJointHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedJoint = 	pCurrentlySelectedKeyFrame->pJointHead;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedJoint==NULL)
	{
		pCurrentlySelectedJoint = 	pCurrentlySelectedKeyFrame->pJointHead;
		return 						pCurrentlySelectedKeyFrame->pJointHead;
	}

	JointLL_Get(pCurrentlySelectedKeyFrame->pJointHead,pPrev,pThis,pNext,pCurrentlySelectedJoint->pInfoItem->pName);
	if(pPrev)
	{
		pPrevJoint = pPrev;
	}
	else
	{
		JointLL_GetLast(pCurrentlySelectedKeyFrame->pJointHead,pPrev,pThis,pNext);
		pPrevJoint = pThis;
	}

	//collapse current Joint if a new one is being selected.
	if(pCurrentlySelectedJoint!=pPrevJoint)
	{
		if(pCurrentlySelectedJoint)ClearBit(pCurrentlySelectedJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_THIS_JOINT);
	}

	pCurrentlySelectedJoint = pPrevJoint;
	RefreshSubordinates(pCurrentlySelectedJoint);
	return pCurrentlySelectedJoint;
}
JointLL*  	MenuFunction_Skeleton_Joint_SelectNext()
{
	struct JointLL* pPrev = NULL;
	struct JointLL* pThis = NULL;
	struct JointLL* pNext = NULL;
	struct JointLL* pNextJoint = NULL;
	//if pSH is null, then there arre no Joints

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Joints.
		pCurrentlySelectedJoint = 	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Joints.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame->pJointHead==NULL)
	{//no items
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame->pJointHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedJoint = 	pCurrentlySelectedKeyFrame->pJointHead;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedJoint==NULL)
	{
		pCurrentlySelectedJoint = 	pCurrentlySelectedKeyFrame->pJointHead;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}

	JointLL_Get(pCurrentlySelectedKeyFrame->pJointHead,pPrev,pThis,pNext,pCurrentlySelectedJoint->pInfoItem->pName);
	if(pNext)
	{
		pNextJoint = pNext;
	}
	else
	{
		JointLL_GetFirst(pCurrentlySelectedKeyFrame->pJointHead,pPrev,pThis,pNext);
		pNextJoint = pThis;
	}

	//collapse current Joint if a new one is being selected.
	if(pCurrentlySelectedJoint!=pNextJoint)
	{
		if(pCurrentlySelectedJoint)ClearBit(pCurrentlySelectedJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_THIS_JOINT);
	}

	pCurrentlySelectedJoint = pNextJoint;
	RefreshSubordinates(pCurrentlySelectedJoint);
	return pCurrentlySelectedJoint;
}

JointLL*  	MenuFunction_Skeleton_Joint_SelectLast()
{
	struct JointLL* pPrev = NULL;
	struct JointLL* pThis = NULL;
	struct JointLL* pNext = NULL;
	struct JointLL* pLastJoint = NULL;

	if(pCurrentlySelectedScene==NULL)
	{//no scene... no Joints.
		pCurrentlySelectedJoint = 	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedObject==NULL)
	{//no object... no Joints.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{//no skeleton... no KeyFrames.
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame->pJointHead==NULL)
	{//no items
		pCurrentlySelectedJoint =	NULL;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedKeyFrame->pJointHead->pNext==NULL)
	{//only 1 item
		pCurrentlySelectedJoint = 	pCurrentlySelectedKeyFrame->pJointHead;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}
	if(pCurrentlySelectedJoint==NULL)
	{
		pCurrentlySelectedJoint = 	pCurrentlySelectedKeyFrame->pJointHead;
		RefreshSubordinates(pCurrentlySelectedJoint);
		return pCurrentlySelectedJoint;
	}

	JointLL_GetLast(pCurrentlySelectedKeyFrame->pJointHead,pPrev,pThis,pNext);
	pLastJoint = pThis;

	//collapse current Joint if a new one is being selected.
	if(pCurrentlySelectedJoint!=pLastJoint)
	{
		if(pCurrentlySelectedJoint)ClearBit(pCurrentlySelectedJoint->JointDataViewFlags,JOINT_DATAVIEW_SHOW_THIS_JOINT);
	}

	pCurrentlySelectedJoint = pLastJoint;
	RefreshSubordinates(pCurrentlySelectedJoint);
	return pCurrentlySelectedJoint;
}

void 		MenuFunction_Skeleton_Joint_RemoveContributer(char* pName)
{
	struct JointLL* pJoint = NULL;
	unsigned long RemoveJointIndex = 0;

	if(pCurrentlySelectedScene==NULL)
	{
		OutputTextToConsole("error. Select a Scene first",TextColor_ConsoleError);
		return;
	}
	if(pCurrentlySelectedObject==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return;
	}
	if(pCurrentlySelectedSkeleton==NULL)
	{
		OutputTextToConsole("error. Select a Skeleton first",TextColor_ConsoleError);
		return;
	}
	if(pCurrentlySelectedKeyFrame==NULL)
	{
		OutputTextToConsole("error. Select a Object first",TextColor_ConsoleError);
		return;
	}
/*
	pJoint = GetJoint(pName,pCurrentlySelectedObject->pSkeleton);
	if(pJoint)
	{
		if(ObjectLL_RemoveContributer(	pCurrentlySelectedObject,
										pCurrentlySelectedJoint->UniqueID,
										pJoint->UniqueID))
		{
			ObjectLL_NormalizeWeights(pCurrentlySelectedObject);

			sprintf(pTempText,"success. %s Is no longer influencing the current Joints vertices.",pCurrentlySelectedJoint->pName);
			OutputTextToConsole(pTempText,TextColor_ConsoleSuccess);
		}
		else
		{
			sprintf(pTempText,"warning. %s Was not influencing the current Joints vertices.",pCurrentlySelectedJoint->pName);
			OutputTextToConsole(pTempText,TextColor_ConsoleError);
		}
	}
	else
	{
		sprintf(pTempText,"error. The Joint %s was not found.",pName);
		OutputTextToConsole(pTempText,TextColor_ConsoleError);
	}
*/
		OutputTextToConsole("todo : remove contributer",TextColor_ConsoleError);

}
/*
void a()/////////////////
void a()//// Console ////
void a()/////////////////
*/





char* 	GetConsoleText(int line_num)
{
	return pConsoleText[line_num].pText;
}
long 	GetConsoleTextColor(int line_num)
{
	return pConsoleText[line_num].Color;
}
void	OutputTextToConsole(char *pText,long Color)
{
	pConsoleText[0].Color = BlendColor(pConsoleText[0].Color,TextColor_ConsoleFadeTo,0.15);
	pConsoleText[1].Color = BlendColor(pConsoleText[1].Color,TextColor_ConsoleFadeTo,0.15);
	pConsoleText[2].Color = BlendColor(pConsoleText[2].Color,TextColor_ConsoleFadeTo,0.15);
	pConsoleText[3].Color = BlendColor(pConsoleText[3].Color,TextColor_ConsoleFadeTo,0.15);

	sprintf(pConsoleText[cConsoleText].pText," > %s",pText);
	pConsoleText[cConsoleText].Color = Color;
	//Increment Console Queue
	cConsoleText++;
	cConsoleText = WrapQueueIndex(cConsoleText);
}

long 	WrapQueueIndex(long index)
{
	if(index>=MAX_LINES_CONSOLE_TEXT)
	{
		index -= MAX_LINES_CONSOLE_TEXT;
	}
	return index;
}
long BlendColor(long Color1,long Color2,float t)
{
	int Alp1,Red1,Grn1,Blu1;
	int Alp2,Red2,Grn2,Blu2;
	int Alp3,Red3,Grn3,Blu3;
	long FinalColor;

	Alp1 = GetAlp8888(Color1);
	Red1 = GetRed8888(Color1);
	Grn1 = GetGrn8888(Color1);
	Blu1 = GetBlu8888(Color1);

	Alp2 = GetAlp8888(Color2);
	Red2 = GetRed8888(Color2);
	Grn2 = GetGrn8888(Color2);
	Blu2 = GetBlu8888(Color2);

	Alp3 = Lerp(t,Alp1,Alp2);
	Red3 = Lerp(t,Red1,Red2);
	Grn3 = Lerp(t,Grn1,Grn2);
	Blu3 = Lerp(t,Blu1,Blu2);

	FinalColor = PACKRGBA(Red3,Grn3,Blu3,Alp3);
	return FinalColor;
}


bool AppMenu_Create()
{
	long index = 0,i,j;

	 pNewSceneName 	= (char *)calloc(MAX_PATHNAME,sizeof(char));
	pOpenSceneName 	= (char *)calloc(MAX_PATHNAME,sizeof(char));
	pSaveSceneName 	= (char *)calloc(MAX_PATHNAME,sizeof(char));

	 pNewObjectName = (char *)calloc(MAX_PATHNAME,sizeof(char));
	pOpenObjectName = (char *)calloc(MAX_PATHNAME,sizeof(char));
	pSaveObjectName = (char *)calloc(MAX_PATHNAME,sizeof(char));

	 pNewMeshName 	= (char *)calloc(MAX_PATHNAME,sizeof(char));
	pOpenMeshName 	= (char *)calloc(MAX_PATHNAME,sizeof(char));
	pSaveMeshName 	= (char *)calloc(MAX_PATHNAME,sizeof(char));

	 pNewSkeletonParentName = (char *)calloc(MAX_PATHNAME,sizeof(char));
	 pNewSkeletonName 		= (char *)calloc(MAX_PATHNAME,sizeof(char));
	pOpenSkeletonName 		= (char *)calloc(MAX_PATHNAME,sizeof(char));
	pSaveSkeletonName 		= (char *)calloc(MAX_PATHNAME,sizeof(char));

	 pNewJointParentName 	= (char *)calloc(MAX_PATHNAME,sizeof(char));
	 pNewJointName 			= (char *)calloc(MAX_PATHNAME,sizeof(char));
	pOpenJointName 			= (char *)calloc(MAX_PATHNAME,sizeof(char));
	pSaveJointName 			= (char *)calloc(MAX_PATHNAME,sizeof(char));

	pTempText 				= (char *)calloc(MAX_PATHNAME,sizeof(char));

	pIconTextureRects 		= (fRECT *)calloc(256,sizeof(fRECT));
	memset(pIconTextureRects,0,256*sizeof(fRECT));

	sprintf( pNewSceneName,"filename");
	sprintf(pOpenSceneName,"girl\\girl");
	sprintf(pSaveSceneName,"filename");

	sprintf( pNewObjectName,"filename");
	sprintf(pOpenObjectName,"filename");
	sprintf(pSaveObjectName,"filename");

	sprintf( pNewMeshName,"filename");
	sprintf(pOpenMeshName,"filename");
	sprintf(pSaveMeshName,"filename");

	sprintf( pNewSkeletonParentName,"null");
	sprintf( pNewSkeletonName,"name");
	sprintf(pOpenSkeletonName,"filename");
	sprintf(pSaveSkeletonName,"filename");

	sprintf( pNewJointParentName,"null");
	sprintf( pNewJointName,"name");
	sprintf(pOpenJointName,"filename");
	sprintf(pSaveJointName,"filename");



	//See "icons.bmp"
	//LARGE ICONS 64x64
	// SEARCHFORICONBAR
	for(j=0;j<3;j++)
	{
		for(i=0;i<8;i++)
		{
			//index = j*8+i;
			pIconTextureRects[index].left    = (float)(i+0)*(64.0/512.0);
			pIconTextureRects[index].right   = (float)(i+1)*(64.0/512.0);
			pIconTextureRects[index].top     = (float)(j+0)*(64.0/512.0);
			pIconTextureRects[index].bottom  = (float)(j+1)*(64.0/512.0);
			index++;
		}
	}

	//NORMAL ICONS 36x36 (new method)
	for(j=0;j<6;j++)
	{
		for(i=0;i<14;i++)//512/36
		{
			//index = j*8+i;
			pIconTextureRects[index].left    = (float)(i+0)*(36.0/512.0);
			pIconTextureRects[index].right   = (float)(i+1)*(36.0/512.0);
			pIconTextureRects[index].top     = (float)(j+0)*(36.0/512.0)+(192.0/512.0);//3 rows of 64 height icons
			pIconTextureRects[index].bottom  = (float)(j+1)*(36.0/512.0)+(192.0/512.0);
			index++;
		}
	}
	return true;
}
void AppMenu_Delete()
{
	SAFE_FREE(pNewSceneName);
	SAFE_FREE(pOpenSceneName);
	SAFE_FREE(pSaveSceneName);
	SAFE_FREE(pNewObjectName);
	SAFE_FREE(pOpenObjectName);
	SAFE_FREE(pSaveObjectName);
	SAFE_FREE(pNewMeshName);
	SAFE_FREE(pOpenMeshName);
	SAFE_FREE(pSaveMeshName);
	SAFE_FREE(pNewSkeletonParentName);
	SAFE_FREE(pNewSkeletonName);
	SAFE_FREE(pOpenSkeletonName);
	SAFE_FREE(pSaveSkeletonName);
	SAFE_FREE(pNewJointParentName);
	SAFE_FREE(pNewJointName);
	SAFE_FREE(pOpenJointName);
	SAFE_FREE(pSaveJointName);
	SAFE_FREE(pTempText);
	SAFE_FREE(pIconTextureRects);
}


void About()
{
	OutputTextToConsole("Programming Dynamic Character Animation : TN Animator 5.0 w/ TN Modeler 1.0",TextColor_ConsoleNormal);
	OutputTextToConsole(" Get the latest Version of TN Animator at www.tanzanite.to",TextColor_ConsoleNormal);
	OutputTextToConsole("                written by David Paull",TextColor_ConsoleNormal);
	OutputTextToConsole("           published by Charles River Media",TextColor_ConsoleNormal);
}







/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////

#ifdef TechNaturePrivate
void Planet_RenderMenu_Background(float& PosX,float& PosY)
{

}

void Planet_RenderMenu_Text(float& PosX,float& PosY)
{

}
#endif	









//sets 2 global text buffers to contain app name, and path from 
// any open/save menu functions filename
//
//if pName = "models\tiger" &&
//if pExt  = ".obb"
//then...
//gp_TextBuffer1 = tiger
//gp_TextBuffer2 = c:\sourcecode\debug\models
//gp_TextBuffer3 = c:\sourcecode\debug\models\tiger.obb
void GetNamePath(char* pName,char* pExt)
{
	//get just name
	long length = strlen(pName);
	memset(gp_TextBuffer1,0,MAX_PATHNAME);
	memcpy(gp_TextBuffer1,pName,length);
	RemovePathFromFileName(gp_TextBuffer1);
	RemoveExtensionFromFileName(gp_TextBuffer1);

	//get path
	memset(gp_TextBuffer2,0,MAX_PATHNAME);
	memcpy(gp_TextBuffer2,pName,length);
	GetPath(gp_TextBuffer2);

	memset(gp_TextBuffer3,0,MAX_PATHNAME);
	sprintf(gp_TextBuffer3,"%s%s%s",gp_TextBuffer2,gp_TextBuffer1,pExt);
}








