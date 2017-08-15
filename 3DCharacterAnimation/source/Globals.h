// Globals.h written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to



//_11, _12, _13, _14
//_21, _22, _23, _24
//_31, _32, _33, _34
//_41, _42, _43, _44
//
//0  1  2  3 
//4  5  6  7
//8  9  10 11
//12 13 14 15


//...............................................................................
//Visual C++ Suppress Warnings
#pragma warning(disable : 4244)//double to float
#pragma warning(disable : 4305)//init double to float
#pragma warning(disable : 4018)//signed/unsigned mismatch
#pragma warning(disable : 4800)//forcing value to bool 'true' or 'false' (performance warning)
//...............................................................................
#define SPACE_CHAR 83 //the width of this char is used for the width of a space.
#define CURSOR_CHAR 83 //the width, and height of this char is used for the w&h of the Cursor.
#define VK_ESC 27
#define VK_1 49
#define VK_2 50
#define VK_3 51
#define VK_4 52
#define VK_5 53
#define VK_6 54
#define VK_7 55
#define VK_8 56
#define VK_9 57
#define VK_0 48
#define FIELD_MAX_CHAR 256
#define MAX_LONG 0xffffffff
//#define MAX_VERTICES 2048
#define MAX_VERTICES 20000
//512*4
//#define MAX_INDICES 10000
#define MAX_INDICES 60000
// 512*6
#define SAFE_FREE(p) { if(p) { free(p); (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//...............................................................................
#define Lerp(t, a, b)\
			(a + t * (b - a))

//bookbook
#define Pi 3.14159265358979323846264338327950288419716939937510582097494459230781

#define RadToDeg 57.295779513082320876798154814105
#define DegToRad 0.017453292519943295769236907684886

#define vk_pg_up   33
#define vk_pg_down 34

#define FLT_MAX         3.402823466e+38F        /* max value */
#define FLT_MIN         1.175494351e-38F        /* min positive value */

//...............................................................................
#define PACKRGBA(r,g,b,a) \
    ((long)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
//...............................................................................
#define MacroNormalize(Length,Ax,Ay,Az)\
			((Length=(1.0/(sqrt(Ax*Ax+Ay*Ay+Az*Az)))),\
			(Ax *= Length),\
			(Ay *= Length),\
			(Az *= Length))
//...............................................................................
/*
#define MacroNormalize4(Length,Ax,Ay,Az,Aw)\
			((Length=(1.0/(sqrt(Ax*Ax+Ay*Ay+Az*Az+Aw*Aw)))),\
			(Ax *= Length),\
			(Ay *= Length),\
			(Az *= Length),\
			(Aw *= Length))
*/
//...............................................................................
#define MacroNormalizeWeights(Length,Ax,Ay,Az,Aw)\
			((Length=(1.0/(Ax+Ay+Az+Aw))),\
			(Ax *= Length),\
			(Ay *= Length),\
			(Az *= Length),\
			(Aw *= Length))

//...............................................................................
#define MacroMagnitude(Length,Ax,Ay,Az)\
			(Length=(float)(sqrt(Ax*Ax+Ay*Ay+Az*Az)))
//...............................................................................
#define MacroDot(Length,Ax,Ay,Az,Bx,By,Bz)\
			(Length = (Ax*Bx+Ay*By+Az*Bz)) 
//...............................................................................
#define MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz)\
			((Rx = Ay*Bz-Az*By),\
			( Ry = Az*Bx-Ax*Bz),\
			( Rz = Ax*By-Ay*Bx))
//...............................................................................
#define Clamp(t,l,h)\
			( (t<l) ? (l) : ((t>h)?(h):(t)) )
//...............................................................................
#define FILLfRECT(pR,a,b,c,d)\
		   (((pR)->left   = (float)a),\
		    ((pR)->right  = (float)b),\
		    ((pR)->top    = (float)c),\
		    ((pR)->bottom = (float)d))
//...............................................................................
//0  1  2  3
//4  5  6  7 
//8  9  10 11
//12 13 14 15
#define FillIdentity(pV)\
		   ((pV[0 ] = 1.0),\
		    (pV[1 ] = 0.0),\
		    (pV[2 ] = 0.0),\
		    (pV[3 ] = 0.0),\
		    (pV[4 ] = 0.0),\
		    (pV[5 ] = 1.0),\
		    (pV[6 ] = 0.0),\
		    (pV[7 ] = 0.0),\
		    (pV[8 ] = 0.0),\
		    (pV[9 ] = 0.0),\
		    (pV[10] = 1.0),\
		    (pV[11] = 0.0),\
		    (pV[12] = 0.0),\
		    (pV[13] = 0.0),\
		    (pV[14] = 0.0),\
		    (pV[15] = 1.0))
//...............................................................................
#define	RandZeroToOne    ((float)rand() / 32767.0)
#define RandNegOneToOne (((float)rand() / 16383.0)-1.0)
//...............................................................................
#define MinValueOutOf3(x,y,z)( (x<y) ? ((x<z) ? x:z) : ((y<z) ? y:z) )
#define MinIndexOutOf3(x,y,z)( (x<y) ? ((x<z) ? 0:2) : ((y<z) ? 1:2) )

#define MedValueOutOf3(x,y,z)( (x>y) ? ((x>z) ? MaximumValue(y,z    ):MaximumValue(x,y    )) : ((y>z) ? MaximumValue(x,z    ):MaximumValue(x,y    )) )
#define MedIndexOutOf3(x,y,z)( (x>y) ? ((x>z) ? MaximumIndex(y,z,1,2):MaximumIndex(x,y,0,1)) : ((y>z) ? MaximumIndex(x,z,0,2):MaximumIndex(x,y,0,1)) )

#define MaxValueOutOf3(x,y,z)( (x>y) ? ((x>z) ? x:z) : ((y>z) ? y:z) )
#define MaxIndexOutOf3(x,y,z)( (x>y) ? ((x>z) ? 0:2) : ((y>z) ? 1:2) )
//...............................................................................
typedef enum {

	MENU_MODE_NONE  		  = 0x00000001L,
	MENU_MODE_ICONBAR		  = 0x00000002L,
	MENU_MODE_ICONBARMAINMENU = 0x00000004L,
} MENU_MODE;
//...............................................................................
typedef enum {
	MOUSE_MODE_MENU		= 0x00000001L,
	MOUSE_MODE_OBJECT	= 0x00000002L,
	MOUSE_MODE_MESH   	= 0x00000004L,
	MOUSE_MODE_BONE    	= 0x00000008L,
	MOUSE_MODE_WEIGHT  	= 0x00000010L,
	MOUSE_MODE_VERT    	= 0x00000020L,
	MOUSE_MODE_TRI    	= 0x00000040L,
} MOUSE_MODE;
//...............................................................................
typedef enum {
	KEYBOARD_MODE_MENU    		= 0x00000001L,
	KEYBOARD_MODE_CAMERA    	= 0x00000002L,
} KEYBOARD_MODE;
//...............................................................................


typedef enum {
	FILE_TYPE_SCN		= 0x00000001L,
	FILE_TYPE_OBJ		= 0x00000002L,
	FILE_TYPE_MSH		= 0x00000004L,
	FILE_TYPE_SKE		= 0x00000008L,
	FILE_TYPE_ASE		= 0x00000010L,
	FILE_TYPE_TXT		= 0x00000020L,
	FILE_TYPE_UKN		= 0x00000040L,//unknown
	FILE_TYPE_NDT		= 0x00000080L,//no .  ie no .msh. no extension.
} enum_FILE_TYPE;

/////////////////////////
//// Rendering Flags ////
/////////////////////////

#define RENDER_ORTHOGRAPHIC 	0x00000001L
#define RENDER_WIREFRAME 		0x00000002L
#define RENDER_SELECTABLE  		0x00000004L
#define RENDER_ZBUFFERREAD 		0x00000008L
#define RENDER_ZBUFFERWRITE 	0x00000010L
#define RENDER_BILINEARFILTER 	0x00000020L
#define RENDER_ALPHAVERTEX   	0x00000040L
#define RENDER_ALPHATEXTURE 	0x00000080L
#define RENDER_LIGHTING 		0x00000100L
#define RENDER_TEXTURED  		0x00000200L
#define RENDER_TRANSFORMED  	0x00000400L//skeleton has transformed meshes vertices, so render using identity.
#define RENDER_VISIBLE 			0x00000800L

////////////////////////
//// Blending Flags ////
////////////////////////

#define BLEND_ZERO				0x00000001L
#define BLEND_ONE				0x00000002L
#define BLEND_SRCCOLOR			0x00000004L
#define BLEND_INVSRCCOLOR		0x00000008L
#define BLEND_SRCALPHA			0x00000010L
#define BLEND_INVSRCALPHA		0x00000020L
#define BLEND_DESTALPHA 		0x00000040L
#define BLEND_INVDESTALPHA		0x00000080L
#define BLEND_DESTCOLOR   		0x00000100L
#define BLEND_INVDESTCOLOR		0x00000200L
#define BLEND_SRCALPHASAT 		0x00000400L

#define TEXTURE_FLAG_565  		0x00000001L
#define TEXTURE_FLAG_5551 		0x00000002L
#define TEXTURE_FLAG_4444 		0x00000004L

#define TEXT_FLAG_KERNING       0x00000001L
#define TEXT_FLAG_EDITABLE      0x00000002L
#define TEXT_FLAG_LEFTJUSTIFY   0x00000004L
#define TEXT_FLAG_RIGHTJUSTIFY  0x00000008L
#define TEXT_FLAG_VERTICALSCALE 0x00000010L//squeezes, or stretches height to perfectly fit vertically.
#define TEXT_FLAG_SELECTED      0x00000020L
#define TEXT_FLAG_VOLATILE      0x00000040L//these can be easily deleted per frame

//////////////////////////////
//////////////////////////////
//////////////////////////////
//// MAIN MENU DATA FLAGS ////
//////////////////////////////
//////////////////////////////
//////////////////////////////

#define SCENE_DATAVIEW_SHOW_THIS_SCENE	0x00000001L
#define SCENE_DATAVIEW_SHOW_VIEWMATRIX	0x00000002L
#define SCENE_DATAVIEW_SHOW_PROJMATRIX	0x00000004L
#define SCENE_DATAVIEW_SHOW_OBJECTHEAD	0x00000008L

#define OBJECT_DATAVIEW_SHOW_THIS_OBJECT	0x00000001L
#define OBJECT_DATAVIEW_SHOW_MATRIX			0x00000002L
#define OBJECT_DATAVIEW_SHOW_MESHHEAD		0x00000004L
#define OBJECT_DATAVIEW_SHOW_SKELETON		0x00000008L

#define MESH_DATAVIEW_SHOW_THIS_MESH	0x00000001L
#define MESH_DATAVIEW_SHOW_MATRIX		0x00000002L
#define MESH_DATAVIEW_SHOW_3dVERTICES	0x00000004L
#define MESH_DATAVIEW_SHOW_3dINDICES	0x00000008L
#define MESH_DATAVIEW_SHOW_SRCBLEND		0x00000010L
#define MESH_DATAVIEW_SHOW_DSTBLEND		0x00000020L
#define MESH_DATAVIEW_SHOW_FLAGS		0x00000040L

////////////////////////////////////
//// Important KeyFrame Indices ////
////////////////////////////////////
#define KEYFRAME_MINFRAME 0
#define KEYFRAME_MAXFRAME 4294967293//(-3) //Last possible index
#define KEYFRAME_CURRPOSE 4294967294//(-2) //current pose  //if time between kf's, this is the interpolated key
#define KEYFRAME_RESTPOSE 4294967295//(-1) //rest pose index (unsigned long)



#define FILE_TYPE 0
#define SCENE_TYPE 1
#define OBJECT_TYPE 2
#define MESH_TYPE 3
#define MATRIX_TYPE 4




#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4a
#define VK_K 0x4b
#define VK_L 0x4c
#define VK_M 0x4d
#define VK_N 0x4e
#define VK_O 0x4f
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5a
#define ASCII_TAB 9
#define ASCII_LF  10
#define ASCII_SP  32





//.................................................................................................
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
///////////////                     /////////////////
///////////////    ENUMERATIONS     /////////////////
///////////////                     /////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

//.................................................................................................
enum API_TYPE//Api Choice
{
	AT_None = 0,
	AT_Software,
	AT_Direct3D,
	AT_OpenGL,
};
//.................................................................................................
enum RES_TYPE//Resolution Choice
{
	RT_Window = 0,
	RT_640,
	RT_800,
	RT_1024,
	RT_1280,
};
//.................................................................................................
enum D3D_ERROR//Api Choice
{
	D3DERR_NOERROR = 0,
	D3DERR_RESTOREVBIB,
	D3DERR_BEGINSCENEFAILED,
};
//.................................................................................................
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
///////////////                     /////////////////
///////////////   DATA STRUCTURES   /////////////////
///////////////                     /////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

//.................................................................................................
typedef struct _fRECT 
{
	float left;
	float right;
	float top;
	float bottom;
}fRECT;
//.................................................................................................
typedef struct _aVECTOR 
{
	float x;
	float y;
	float z;
} aVECTOR;
//.................................................................................................
typedef struct _aDOUBLE_VECTOR 
{
	double x;
	double y;
	double z;
} aDOUBLE_VECTOR;
//.................................................................................................
typedef struct _aQUATERNION 
{
	float x;
	float y;
	float z;
	float w;
} aQUATERNION;
//.................................................................................................
typedef struct _aPLANE 
{
	float x;
	float y;
	float z;
	float d;
} aPLANE;
//.................................................................................................
typedef struct _VERTEX_XYZNRGBUV
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	long diffuse;//color
	float tu;
	float tv;
} VERTEX_XYZNRGBUV;
//.................................................................................................
typedef struct _PERVERTEX_BONE_DATA
{
	float weight[4];
	unsigned char index[4];
} PERVERTEX_BONE_DATA;
//.................................................................................................
typedef struct _LineOfConsoleText 
{
	char pText[256];
	long Color;
}LineOfConsoleText;
//.................................................................................................
typedef struct _aTEXTURE 
{
	char pName[256];
	unsigned long Width;
	unsigned long Height;
	unsigned long flags;
	unsigned long UniqueID;
	unsigned long InVRAM;	//really a bool, but keep alignment. "also in video memory"
	unsigned char* pChar;	//24,32bit textures 888,8888
	unsigned char* pAlpha;	//8bit alpha channel 4444,8888
	unsigned char* pAPITEXTURE;//used to hold pointer to d3d's vram copy. Ack!
	unsigned int OpenGLTextureID;//used to index opengl texture list.

	WORD* pWord;//16bit textures 5551,555,565,4444
} aTEXTURE;
//.................................................................................................
typedef struct _FontObject 
{
	char pName[256];
	float CharWidth;//for no kerning.
	struct MeshLL* pBackground;
	struct MeshLL* pText1;
	struct MeshLL* pText2;
	struct MeshLL* pIcons;
	struct MeshLL* pOverlay;
	fRECT pCharRectXY[128];
	fRECT pCharRectUV[128];
	VERTEX_XYZNRGBUV ptlV[256*4];
	WORD pI[256*6];
}FontObject;

//.................................................................................................
////////////////
//// IconLL ////
////////////////
struct IconLL
{
	char pName[256];
	unsigned long SortKey;
	fRECT Screen;
	fRECT Texture;
	struct IconLL* pNext;
	unsigned long flags;
};
//.................................................................................................
////////////////
//// RectLL ////
////////////////
struct RectLL
{
	unsigned long SortKey;
	long ColorIndex;
	float left;
	float right;
	float top;
	float bottom;
	struct RectLL* pNext;
};
//.................................................................................................
////////////////////
//// KeyFrameLL ////
////////////////////

struct KeyFrameLL
{
	//time : time based SoryKey. in ms.
	//	-1(0xffffffff) is rest pose.
	//	-2 is "scratch pad" memory. holds pose for current time.
	//	-3 max time allowed
	//	 0 min time allowed
	unsigned long time;
	float pRelativeMatrix[16];
	float pAbsoluteMatrix[16];
	/////////////////////////
	struct KeyFrameLL* pNext;
};
//.................................................................................................
///////////////
//// BoneH ////
///////////////
//Each BoneH Item in in a LL of Siblings, or bones at the same level in hiearchy.
//Each BoneH Item optionally has a LL of Childern, or bones at one level deeper hiearchy.
//a Neutral(rest pose) keyframe is automatically added to every BoneH item. it cant be removed xcept for removeall
//a Current(working memory)(scratch pad) keyframe is automatically added to every BoneH item. it cant be removed xcept for removeall

struct BoneH//Item
{
	char pName[256];

	unsigned long UniqueID;//vertices will reference this number in an unsigned char. 256 bones max. 

	float Length;
	//instant access pointers.
	struct KeyFrameLL* pRestPose;//rest pose. skin attached to pRestPose's pose.
	struct KeyFrameLL* pCurrPose;//interpolated keyframe at t

	//pKeyFrameHead : points to LL of keyframes.
	//	One of those keyframes is the Neutral rest pose.
	//	One of those keyframes is the Current working memory.
	struct KeyFrameLL* pKeyFrameHead;
	//////////////////////////////////////
	struct BoneH* pParent;
	struct BoneH* pChildren;
	struct BoneH* pNextSibling;
	//////////////////////////////////////
	//ToDo : struct BoneH* pSkeleton;//attach hand sub-skeleton to wrist
};
//.................................................................................................
////////////////
//// MeshLL ////
////////////////
struct MeshLL
{
	char pName[256];
	unsigned short SortKey;

	float pMatrix[16];

	VERTEX_XYZNRGBUV* pLocalVerts;	//pointer to Local Verts. Aligned with Skeleton. 
	VERTEX_XYZNRGBUV* pWorldVerts;	//pointer to World Verts. deformed, and xformed into world.
	PERVERTEX_BONE_DATA* pPerVertexBoneData;
  	WORD* pIndices;			//pointer to Indices

	unsigned long num_verts;
	unsigned long num_tris;

	unsigned long RenderFlags;//change to flags
	unsigned long RestoreFlags;//used by menu, which changes render flags sometimes. it can restore them easily.

	aTEXTURE Texture;
	aTEXTURE MultiTexture;//not used yet.

	unsigned long SrcBlend;
	unsigned long DstBlend;

	unsigned long MeshDataViewFlags;

	long bShowThisMesh;//true/false for text display. nothing above scene, so scene will hold this value.
	long bShowMatrix;//true/false for text display
	long bShow3dVertices;//true/false for text display
	long bShow2dVertices;//true/false for text display
	long bShowTriangles;//true/false for text display
	long bShowFlags;//true/false for text display

	long MaterialID;//for loading .txt model file

	struct MeshLL* pPrev;
	struct MeshLL* pNext;
};
//.................................................................................................
//////////////////
//// ObjectLL ////
//////////////////
struct ObjectLL
{
	char pName[256];
	unsigned long SortKey;

	float  pMatrix[16];

	struct MeshLL* pMeshHead;//many meshes
	struct BoneH* pSkeleton;//only one skeleton. each bone contains keyframes of itself for each animation

	struct ObjectLL* pPrev;
	struct ObjectLL* pNext;

	unsigned long ObjectDataViewFlags;

	long bShowThisObject;//true/false for text display. nothing above scene, so scene will hold this value.
	long bShowMatrix;//true/false for text display
	long bShowMeshHead;//true/false for text display
	long bShowSkeleton;//true/false for text display
	long bUseObjectMatrix;//true/false 
};
//.................................................................................................
/////////////////
//// SceneLL ////
/////////////////
//scene needs texture list, so mult meshes use same copy of texture.
struct SceneLL
{
	char pName[256];
	unsigned long SortKey;
	float pView[16];//Main Cameras View Matrix.
	float pProj[16];//3D Projection Matrix
	float pOrtho[16];//2D Orthographic Matrix
	struct ObjectLL* pObjectHead;
	struct SceneLL* pPrev;
	struct SceneLL* pNext;
	unsigned long SceneDataViewFlags;
};
//.................................................................................................
/////////////////
//// FieldLL ////
/////////////////
//A Field holds text
struct FieldLL 
{
	unsigned long SortKey;
	char pName[256];
	char pText[(FIELD_MAX_CHAR+1)];
	float s;//0-1 parameter for screen positioning
	float t;
	float width;//for bg frame
	float height;
	fRECT Screen;//screen coords for clipping window. also bg window.
	float fontscale;
	float xpos;					//Start X scrolling position

	float screenx;					//Start X scrolling position
	float screeny;					//Start X scrolling position

	long textcolor;
	unsigned long flags;	//Kerning,editable

	float* pfloat;//pointer to the value being edited
	unsigned long* plong;//ex. time index
	unsigned short* pword;//ex. indices
	char* pchar;
	unsigned long cursorpos;

	FontObject* pFont;
	
	struct FieldLL* pNext;
};
//.................................................................................................
typedef struct _RESOLUTIONDATA
{
	float width;
	float height;
	float aspectratio;
}RESOLUTIONDATA;
//.................................................................................................
struct OutputMatrixLL//view matrix data as visible vectors
{
	struct MeshLL X;
	struct MeshLL Y;
	struct MeshLL Z;
	struct MeshLL U;
	struct MeshLL V;
	struct MeshLL N;
	struct MeshLL Bone;
	struct OutputMatrixLL* pNext;
};
//.................................................................................................
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
///////////////                     /////////////////
/////////////// FUNCTION PROTOTYPES /////////////////
///////////////                     /////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


//////////////////
//// Software ////
//////////////////

extern bool CreateSoftwareWindow(HINSTANCE hInstance,char* title, int width, int height, int bits,int nCmdShow);
extern void ChooseOpenGLorD3DWindow();
extern void DeleteSoftwareWindow();


//////////////////
//// Direct3D ////
//////////////////

extern bool VerifyRenderD3D();
extern HWND CreateDirect3DWindow(HINSTANCE hInstance,char* title,int bits,RES_TYPE ResType,int nCmdShow);
extern void DeleteDirect3DWindow();
extern void UpdateDirect3DWindow();
extern D3D_ERROR UpdateRenderD3D();
extern bool RestoreVertexBuffers();

extern D3D_ERROR BeginSceneD3D();
//extern D3D_ERROR RenderMeshD3D(struct SceneLL* pScene,struct MeshLL* pMesh);
extern D3D_ERROR RenderMeshD3D(struct MeshLL* pMesh,
								float* pView,
								float* pProj,
								float* pOrtho);


extern D3D_ERROR Render3dMeshD3D(struct MeshLL* pMesh);
extern D3D_ERROR EndSceneD3D();

extern bool ReSizeDirect3D(long Width, long Height);

////////////////
//// OpenGL ////
////////////////

extern HWND CreateOpenGLWindow(HINSTANCE hInstance,char* title, int bits, RES_TYPE ResType,int nCmdShow);
extern void DeleteOpenGLWindow();

extern void BeginSceneOpenGL();
extern void RenderMeshOpenGL(	struct MeshLL* pMesh,
								float* pView,
								float* pProj,
								float* pOrtho);
extern void Render2dMeshOpenGL(struct SceneLL* pScene,struct MeshLL* pMesh);
extern void Render3dMeshOpenGL(struct MeshLL* pMesh);
extern void EndSceneOpenGL();

extern void ReSizeOpenGL(long Width,long Height);
extern void Handle_WM_ACTIVATE(WPARAM wParam, LPARAM lParam);
extern bool LoadTextureIntoVRAMOpenGL(aTEXTURE* pTexture);
////////////////
//// Render ////
////////////////
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern float CurrentWindowWidth;
extern float CurrentWindowHeight;
extern bool windowedflag;
extern bool apiactive;


////////////////
//// Common ////
////////////////
void VertexMatrixMul(	aVECTOR* pDstV,
						VERTEX_XYZNRGBUV* pSrcV,
						float* pMat);

void NormalMatrixMul(	aVECTOR* pDstV,
						VERTEX_XYZNRGBUV* pSrcV,
						float* pMat);

float Distance_PointToVector(	aVECTOR RayOrigin,
								aVECTOR	RayDir,
								aVECTOR Point);

float Distance_PointToBone(	aVECTOR RayOrigin,
							aVECTOR	RayDir,
							float RayLength,
							aVECTOR Point);

void RemoveExtensionFromFileName(char *pFileName);
void ReadFromFile(HANDLE fH,char* pData,long Size);
void WriteToFile(HANDLE fH,char* pData,long Size);
void VectorMulRotationMatrix(	aVECTOR* pDstV,
								aVECTOR* pSrcV,
								float* pMat);
void VectorMulFullMatrix(aVECTOR* pDstV,
						 aVECTOR* pSrcV,
						 float* pMat);


enum_FILE_TYPE GetFileType(char* pFileName);

void OrthonormalizeOrientation(float* pMat);
void PositionMatrix(float *pMat,float Ax,float Ay,float Az);
//void LeftToRightHand(float *pMat);
//void Transpose(float *pMat);
void AxisRotationMatrix(float *pMat,float Ax,float Ay,float Az,float Angle);
void TempLoadModel(struct MeshLL* pMesh);
void TempLoadModel2(struct MeshLL* pMesh);
void AddPathToFileName(char *pFileName);
void RemovePathFromFileName(char *pFileName);
bool LoadBMP(char*  pFileName,
			 unsigned char*& pBits,
			 long&  Width,
			 long&  Height);
bool SaveBMP(char* pFileName,
			 unsigned char* pBits,
			 long  Width,
			 long  Height); 

bool LoadTexture(	char*  pFileName,
					char* pFullPathFileName,
					aTEXTURE& pTexture,
					long flags);

bool CreateTexture(char* pName,
					aTEXTURE& pTexture,
					long Width,
					long Height,
					long flags) ;

bool UpdateUniqueID(aTEXTURE& pTexture);
void SetBit(unsigned long& flags,unsigned long flag);
void SwapBit(unsigned long& flags,unsigned long flag);
void ClearBit(unsigned long& flags,unsigned long flag);
bool IsBitSet(unsigned long& flags,unsigned long flag);
bool PointInRect(POINT point,fRECT Clip);
fRECT ClipRectToRect(fRECT Rect,fRECT Clip);
unsigned int ReadNextInteger(char *ch);
float ReadNextFloat(char *ch);
BOOL IntersectTriangle( aVECTOR& orig,
                        aVECTOR& dir, 
                        aVECTOR& v0,
                        aVECTOR& v1, 
                        aVECTOR& v2,
						float* t, 
						float* u, 
						float* v);
void Identity(float *pMat);
void Scale(float *pMat,float scale);
void Scale(	float *pMat,
			float sx,
			float sy,
			float sz);
void CreateRandomVector(aVECTOR* pMat);
float* CreateMatrixRotationX(float* pOut, float angle);
float* CreateMatrixRotationY(float* pOut, float angle);
float* CreateMatrixRotationZ(float* pOut, float angle);
float* CreateMatrixYawPitchRoll(float* pOut,float yaw, float pitch, float roll);
void MatrixMul(float* pDst,float* pA,float* pB);
void RotateVectorUsingMatrix(	aVECTOR* pDstV,
								aVECTOR* pV,
								float* pM);
void RotatePointUsingMatrix(aVECTOR* pDstV,
							aVECTOR* pP,
							float* pM);
void CreateRotationMatrixAroundWorldX(float* pM,float Radians);
void CreateRotationMatrixAroundWorldY(float* pM,float Radians);
void CreateRotationMatrixAroundWorldZ(float* pM,float Radians);
void InverseMatrix(float* pDst,float* pSrc);
//void InverseMatrix(double* pDst,float* pSrc);
void CreateMatrixFacingMatrix(	float* pDstM,
								float* pView,
								float x,
								float y,
								float z);

void CreateMatrixFacingPointOnOneAxis(	aVECTOR& Pos,//position of final matrix
										aVECTOR& Dir,//axis to rotate about
										aVECTOR& LookAt,//point to look at
										float* pDst);
void CreateLookAtMatrix(aVECTOR& Pos,
						aVECTOR& Up,//typically <0,1,0>
						aVECTOR& LookAtPos,
						float* pDst);


void CreateMatrixFromQuaternion(float* pM,
								aQUATERNION& pQ);
void CreateQuaternionFromAxisAngle(aQUATERNION& Q,aVECTOR Axis,float angle);
void CreateQuaternionFromMatrix(aQUATERNION& pQ,
								float* pM);

void CreateQuaternionFromMatrixGOOD2(aQUATERNION& Q,
								float* pM);

void QuaternionConjugate(aQUATERNION& DstQ,aQUATERNION& SrcQ);
void QuaternionMultiply(aQUATERNION& pC,
						aQUATERNION& pA,
						aQUATERNION& pB);

void QuaternionNormalize(aQUATERNION& pQ);
void QuaternionSlerp( 	aQUATERNION& pC,
						aQUATERNION pA,
						aQUATERNION pB,
						float t);

void CreateVectorAngleFromQuaternion(	aVECTOR& pV,
										float& angle,
										aQUATERNION& pQ);


void CreateAxisAngleFromMatrix(	aVECTOR& Axis,
								float& Angle,
								float* pM);


void DoubleCreateAxisAngleFromMatrix(	aVECTOR& Axis,
										double& Angle,
										float* pM);

void MatrixSlerp(	float* pC,//destination
					float* pA,//matrix "from"
					float* pB,//matrix "to"
					float t);
void DoubleMatrixSlerp(	float* pC,//destination
						float* pA,//matrix "from"
						float* pB,//matrix "to"
						float t);
float GetSeconds(bool reset);
unsigned long GetMilliSeconds(bool reset);
unsigned long GetElapsedMilliSeconds();

void CreateQuaternionFadeMatrix(aVECTOR& Pos,
								aVECTOR& Up,//typically <0,1,0>
								aQUATERNION& pQ,
								float* pDst);
void CreateQuaternionArrowMatrix(aVECTOR& Pos,
								aVECTOR& Up,//typically <0,1,0>
								aQUATERNION& pQ,
								float* pDst);

//				SceneLL
// 			ObjectLL 	CameraLL	LightLL
//		MeshLL


/////////////////////
//// Linked List ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
struct SceneLL* SceneLL_Insert(	unsigned long SortKey,
							    char* pName,
							    struct SceneLL*& pSceneHead);
void SceneLL_RemoveALL(struct SceneLL*& pSceneHeadIn);
void SceneLL_Remove(char* pName,
					struct SceneLL*& pSceneHeadIn);
void SceneLL_Remove(struct SceneLL*& pRemoveMe,
					struct SceneLL*& pSceneHeadIn);
void SceneLL_Sort(struct SceneLL*& pSceneHeadIn);
bool SceneLL_Load(struct SceneLL* pScene,char *pFileName);
bool SceneLL_Save(struct SceneLL* pScene,char *pFileName);
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void ObjectLL_NormalizeWeights(	struct ObjectLL* pObject);

void ObjectLL_DeformMeshesUsingSkeleton(struct ObjectLL* pObject);

void ObjectLL_FindContributers(	struct ObjectLL* pObject,
								unsigned long CurrBoneIndex,
								unsigned char* pContributers);

bool ObjectLL_RemoveContributer(struct ObjectLL* pObject,
								unsigned long CurrBoneIndex,
								unsigned long RemoveBoneIndex);

void ObjectLL_InitWeights(struct ObjectLL* pObject);
struct ObjectLL* ObjectLL_Insert(unsigned long SortKey,
								 char* pName,
								 struct ObjectLL*& pObjectHeadIn);
void ObjectLL_Remove(	struct ObjectLL*& pRemoveMe,
						struct ObjectLL*& pObjectHeadIn);
void ObjectLL_Remove(char* pName,
					 struct ObjectLL*& pObjectHeadIn);
void ObjectLL_RemoveALL(struct ObjectLL*& pObjectHeadIn);
void ObjectLL_Sort(struct ObjectLL*& pObjectHeadIn);
bool ObjectLL_Load(struct ObjectLL* pObject,char *pFileName);
bool ObjectLL_Save(struct ObjectLL* pObject,char *pFileName);
void ObjectLL_GetMinMaxXYZ(	struct ObjectLL* pObject,
							float& ObjectMinX,
							float& ObjectMinY,
							float& ObjectMinZ,
							float& ObjectMaxX,
							float& ObjectMaxY,
							float& ObjectMaxZ);

void ObjectLL_ColorizeWhite(struct ObjectLL* pObject);
void ObjectLL_ColorizeBlack(struct ObjectLL* pObject);
void ObjectLL_CopyLocalToWorld(struct ObjectLL* pObject);
void ObjectLL_ColorizeUsingBoneWeights(	struct ObjectLL* pObject,
										unsigned long BoneIndex,
										unsigned long Red,
										unsigned long Grn,
										unsigned long Blu);
void ObjectLL_AllMeshs_SetBit(struct ObjectLL* pObject,unsigned long flags);
void ObjectLL_AllMeshs_ClearBit(struct ObjectLL* pObject,unsigned long flags);
void ObjectLL_AllMeshs_BackupRenderFlags(struct ObjectLL* pObject);
void ObjectLL_AllMeshs_RestoreRenderFlags(struct ObjectLL* pObject);

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void MeshLL_AddOffsetXYZ(	struct MeshLL* pMesh,
							float OffsetX,
							float OffsetY,
							float OffsetZ);

struct MeshLL* MeshLL_Insert(unsigned long SortKey,
							 char* pName,
							 struct MeshLL*& pMeshHeadIn);
void MeshLL_Remove(char* pName,
				   struct MeshLL*& pMeshHeadIn);
void MeshLL_Remove(	struct MeshLL*& pRemoveMe,
					struct MeshLL*& pMeshHeadIn);
void MeshLL_RemoveALL(struct MeshLL*& pMeshHeadIn);
void MeshLL_Sort(struct MeshLL*& pMeshHeadIn);
struct MeshLL* MeshLL_CreateNode(	char* pName,
									unsigned long max_w_verts,
									long max_w_indices,
									unsigned long RenderFlags);
bool MeshLL_Save(struct MeshLL* pMesh,char *pFileName);
bool MeshLL_Load(struct MeshLL* pMesh,char *pFileName);
void MeshLL_Orthographic3dTo2dRender(struct MeshLL* pMesh);
void MeshLL_Orthographic3dTo2dFlatShadedRender(struct MeshLL* pMesh);
void MeshLL_Create2dAnd3DVertexArrays(struct MeshLL* pMesh);
void MeshLL_GetMinMaxXYZ(	struct MeshLL* pMesh,
							float& MeshMinX,
							float& MeshMinY,
							float& MeshMinZ,
							float& MeshMaxX,
							float& MeshMaxY,
							float& MeshMaxZ);

void MeshLL_ColorizeWhite(struct MeshLL* pMesh);
void MeshLL_ColorizeBlack(struct MeshLL* pMesh);
void MeshLL_CopyLocalToWorld(struct MeshLL* pObject);
void MeshLL_ColorizeUsingBoneWeights(	struct MeshLL* pMesh,
										unsigned long BoneIndex,
										unsigned long Red,
										unsigned long Grn,
										unsigned long Blu);
void MeshLL_NormalizeBoneWeights(struct MeshLL* pMesh);
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
struct AnimationLL* AnimationLL_Insert(unsigned long SortKey,
								 char* pName,
								 struct AnimationLL*& pAnimationHeadIn);
void AnimationLL_Remove(char* pName,
					 struct AnimationLL*& pAnimationHeadIn);
void AnimationLL_RemoveALL(struct AnimationLL*& pAnimationHeadIn);
void AnimationLL_Sort(struct AnimationLL*& pAnimationHeadIn);
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
struct KeyFrameLL* KeyFrameLL_Insert(	unsigned long time,
										struct KeyFrameLL*& pKeyFrameHeadIn);
struct KeyFrameLL* KeyFrameLL_InsertSpecial(	unsigned long time,
												struct KeyFrameLL*& pPrevKeyFrame,//return pointer to prev
												struct KeyFrameLL*& pKeyFrameHeadIn);
bool KeyFrameLL_Remove(	unsigned long time,struct KeyFrameLL*& pKeyFrameHeadIn);
void KeyFrameLL_Remove(	struct KeyFrameLL* pRemoveMe,
						unsigned long& num_keyframes,
						struct KeyFrameLL*& pKeyFrameHeadIn);
void KeyFrameLL_RemoveALL(	struct KeyFrameLL*& pKeyFrameHeadIn);
							//unsigned long& num_keyframes);
void KeyFrameLL_Sort(struct KeyFrameLL*& pKeyFrameHeadIn);

struct KeyFrameLL*  KeyFrameLL_GetKeyframe(	struct KeyFrameLL*& pKeyFrameHeadIn,
											struct KeyFrameLL*& pPrevKeyFrame,
											unsigned long time);
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
struct FieldLL* FieldLL_Insert(	
				unsigned long SortKey,
				char* pName,
				char* pText,
				float s,
				float t,
				float width,
				float height,
				fRECT Screen,
				float fontscale,
				float xpos,
				long textcolor,
				unsigned long flags,
				unsigned long cursorpos,
				float* pfloat,			//null if not editable float
				unsigned long* plong,	//null if not editable word
				unsigned short* pword,	//null if not editable word
				char* pchar,			//null if not editable char
				FontObject* pFont,		//null for default font [0] 
				struct FieldLL*& pFieldHeadIn);


void FieldLL_Remove(char* pName,struct FieldLL*& pFieldHeadIn);
void FieldLL_RemoveVolatile(struct FieldLL*& pFieldHeadIn);
void FieldLL_RemoveALL(struct FieldLL*& pFieldHeadIn);
void FieldLL_Sort(struct FieldLL*& pFieldHeadIn);
int PickFieldChar(struct SceneLL* pScene,FieldLL* pField,FontObject* pFont);
FieldLL* SelectField(struct SceneLL* pScene,FieldLL* pFieldHeadIn,POINT ptCursor);
void FieldLL_WM_KEYDOWN(FieldLL* pField,WPARAM wParam, LPARAM lParam);
void FieldLL_WM_CHAR(FieldLL* pField,WPARAM wParam, LPARAM lParam);
float CalcWidthField(FieldLL* pField,FontObject* pFont);
void FieldKeepCursorInView(FieldLL* pField,FontObject* pFont);
void InsertIconIntoIconLL(	struct SceneLL* pScene,
								float& PosX,
								float& PosY,
								char* pIcons);
void RenderMatrix(	float* pMat,
					float NewPosX,
					float& PosY);
//void Render3dVertices(	VERTEX_XYZNRGBUV *pWorldVerts,
void Render3dVertices(	VERTEX_XYZNRGBUV *pLocalVerts,
						unsigned long num_verts,
						float NewPosX,
						float& PosY);
void Render2dVertices(	VERTEX_XYZNRGBUV *pScreenVerts,
						unsigned long num_s_verts,
						float NewPosX,
						float& PosY);
void RenderIndices(	WORD* pIndices,
					unsigned long num_tris,
					float NewPosX,
					float& PosY);
void PickDataView(	POINT MousePos,
					long MinY,
					long MaxY,
					unsigned long* pFlags,
					unsigned long  iFlag);
void PickDataViewField(POINT MousePos,FieldLL* pField);
void RenderMeshStructureText(	struct SceneLL* pScene,
			   					struct MeshLL* pMesh,
								float& PosX,
								float& PosY);
void RenderObjectStructureText(	struct SceneLL* pScene,
								struct ObjectLL* pObject,
								float& PosX,
								float& PosY);
void RenderSceneStructureText(	struct SceneLL* pScene,
								float& PosX,
								float& PosY);
void RenderMeshStructureBackground(		struct SceneLL* pScene,
										struct MeshLL* pMesh,
										float& PosX,
										float& PosY);
void RenderObjectStructureBackground(	struct SceneLL* pScene,
										struct ObjectLL* pObject,
										float& PosX,
										float& PosY);
void RenderSceneStructureBackground(	struct SceneLL* pScene,
										float& PosX,
										float& PosY);
void UpdateMainMenu(bool left_mouse_button_down);
void ClipNoOverlapRectLLaddToIconLL(struct RectLL* pInitialLL,
									struct IconLL*& pIconHeadIn);
void ClipRectX(	struct RectLL* pRect,
				float x,
				int dir,
				struct RectLL*& pVisibleLLHead);
void ClipRectY(	struct RectLL* pRect,
				float y,
				int dir,
				struct RectLL*& pVisibleLLHead);

SceneLL* 	MenuFunction_Scene_SelectNextScene();
ObjectLL* 	MenuFunction_Object_SelectNextObject();
MeshLL*  	MenuFunction_Mesh_SelectNextMesh();
BoneH*  	MenuFunction_Bone_SelectNextBone();

void RefreshSubordinates(struct SceneLL* pScene);
void RefreshSubordinates(struct ObjectLL* pObject);
void RefreshSubordinates(struct MeshLL* pMesh);
////////////////////////////////////////////////////////////////////
struct IconLL* IconLL_Insert(	char* pName,
								unsigned long SortKey,
								fRECT Screen,
								fRECT Texture,
								unsigned long flags,
								struct IconLL*& pIconHeadIn);
void IconLL_Remove(char* pName,
					struct IconLL*& pIconHeadIn);
void IconLL_RemoveALL(struct IconLL*& pIconHeadIn);
void IconLL_Sort(struct IconLL*& pIconHeadIn);
////////////////////////////////////////////////////////////////////
struct RectLL* RectLL_Insert(  	unsigned long SortKey,
								long ColorIndex,
								float left,
								float right,
								float top,
								float bottom,
								struct RectLL*& pRectHeadIn);
void RectLL_Remove( struct RectLL* pRect,
					struct RectLL*& pRectHeadIn);
void RectLL_RemoveALL(	struct RectLL*& pRectHeadIn);
////////////////////////////////////////////////////////////////////

extern char gszWindowClass[];// The title bar text
extern API_TYPE ApiType;//D3D or OpenGL?
extern RES_TYPE ResType;//Resolution Choice
extern RESOLUTIONDATA ResolutionData[];

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

