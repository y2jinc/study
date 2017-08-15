// Library.h written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com

//	Library is the innermost core of the entire project
//	All global defines, and structures are defined here
// 	If the structure is a LinkedList, its code will have a (.cpp) file of the same name. ie SceneLL.cpp
// 	If the structure is a Single Item,  its code will be in Library.cpp. ie "struct aVECTOR"
//	

// "KF" KeyFrame : each skeleton has n joints. each joint has m keyframes. each keyframe holds a orientation, and position of that joint.
// "RP" Rest Pose KeyFrame : aka neutral pose. the RP KF is the last KF in the KF LL. the mesh is aligned with the rest pose joint. the mesh will follow this joint for all other keyframes.
// "CP" Current Pose KeyFrame :  this holds the interpolated joints for any given time t. It is "working memory" "scratch pad" the CP KY is used for rendering as it holds the current pose

//_11, _12, _13, _14
//_21, _22, _23, _24
//_31, _32, _33, _34
//_41, _42, _43, _44
//
//0  1  2  3 
//4  5  6  7
//8  9  10 11
//12 13 14 15
#ifndef _LIBRARY_H_
#define _LIBRARY_H_

//...............................................................................
//Visual C++ Suppress Warnings
#pragma warning(disable : 4244)//double to float
#pragma warning(disable : 4305)//init double to float
#pragma warning(disable : 4018)//signed/unsigned mismatch
#pragma warning(disable : 4800)//forcing value to bool 'true' or 'false' (performance warning)
//...............................................................................
#define Pi 3.14159265358979323846264338327950288419716939937510582097494459230781
#define RadToDeg 57.295779513082320876798154814105
#define DegToRad 0.017453292519943295769236907684886
#define SPACE_CHAR 83 //the width of this char is used for the width of a space.
#define CURSOR_CHAR 83 //the width, and height of this char is used for the w&h of the Cursor.
#define FIELD_MAX_CHAR 256
#define MAX_LONG 0xffffffff
#define MAX_VERTICES 5000//the editor allocates MAX, the player allocates minimum necessary
#define MAX_INDICES  12000
#define MAX_JOINTS 256
#define MAX_PATHNAME 2048//same as INTERNET_MAX_PATH_LENGTH

#define FLT_MAX 3.402823466e+38F //max value 3400...0.0
#define FLT_MIN 1.175494351e-38F //min positive value 0.00...0017

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

#define ASCII_MIN 33 //CreateFont
#define ASCII_MAX 127//not inclusive
#define ASCII_SLASH  47
#define ASCII_ASTRISK  42
#define ASCII_TAB 9
#define ASCII_LF  10
#define ASCII_CR  13
#define ASCII_SP  32
#define vk_pg_up   33
#define vk_pg_down 34
//...............................................................................
#define SAFE_FREE(p) { if(p) { free(p); (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
//...............................................................................
#define bstrcmp(a,b)(strcmp(a,b)==0)
//...............................................................................
#define Lerp(t, a, b)\
			(a + t * (b - a))
//...............................................................................
#define PACKRGBA(r,g,b,a) \
		    ((long)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
//...............................................................................
#define GetAlp8888(Color) (Color>>24)
#define GetRed8888(Color)((Color>>16)&255)
#define GetGrn8888(Color)((Color>>8 )&255)
#define GetBlu8888(Color)((Color    )&255)
//...............................................................................
#define MacroNormalize(Length,Ax,Ay,Az)\
			((Length=(1.0/(sqrt(Ax*Ax+Ay*Ay+Az*Az)))),\
			(Ax *= Length),\
			(Ay *= Length),\
			(Az *= Length))
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

/////////////////////////////
//// App Rendering Flags ////
/////////////////////////////
#define APP_SHOWJOINTS	  			0x00000001L//display joints
#define APP_SHOWBONES	  			0x00000002L//display bones



//if you change the order of this list, 
//	existing meshes wont load correctly.
#define RENDER_ORTHOGRAPHIC 		0x00000001L//use ortho matrix, otherwise use perspective matrix
#define RENDER_WIREFRAME 			0x00000002L//render wireframe, otherwise render solid(normal)
#define RENDER_SELECTABLE  			0x00000004L//
#define RENDER_ZBUFFERREAD 			0x00000008L//Read from zbuffer before rendering.
#define RENDER_ZBUFFERWRITE 		0x00000010L//write to zbuffer
#define RENDER_BILINEARFILTER 		0x00000020L//use bilinear filter, otherwise use nearest(point sampling)
#define RENDER_TRANSLUCENT_VERTEX   0x00000040L//per vertex alpha
#define RENDER_TRANSLUCENT_TEXTURE	0x00000080L//ex stained glass window. per pixel alpha
#define RENDER_LIGHTING 			0x00000100L//turn lighting on
#define RENDER_TEXTURED  			0x00000200L//turn texturing on, otherwise just use base color(pV[].diffuse)
#define RENDER_TRANSFORMED  		0x00000400L//user transformed. render world verts with identity matrix, otherwise render local verts with local to world matrix(LtoW = (pObj->pMatrix)*(pMsh->pMatrix)). pObj->pMatrix is the ActorMatrix.
#define RENDER_VISIBLE 				0x00000800L//render mesh, otherwise skip
#define RENDER_TRANSPARENT_TEXTURE	0x00001000L//colorkey. ex. leaf casts bitmap shadow



#define OBJECT_ACTORMATRIX	  		0x00000001L//Render using (pObj->pMatrix)*(pMsh->pMatrix), otherwise just use pMsh->pMatrix. pObj->pMatrix is the ActorMatrix. this effects RenderMesh, and RenderSkeleton. this is a convienent way to force models back to XYZ origin for joint adjustment.
#define OBJECT_MESHES_WIREFRAME 	0x00000002L//these are modes that help modeleing by making entire body translucent so you can see bones for example
#define OBJECT_MESHES_ALPHAVERTEX   0x00000004L//They can cause damage to your meshes, because this sets all meshes to yes, or no, not a toggle.
#define OBJECT_MESHES_TEXTURED  	0x00000008L//


//////////////////////
//// VERTEX Flags ////
//////////////////////

#define VERTEX_SELECTED 	0x00000001L//User has selected these verts
#define VERTEX_VISIBLE 		0x00000002L//when editing verts, verts facing camera are flaged so the can be rendered brighter than backfacing verts.

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


////////////////////////////////////
//// Important KeyFrame Indices ////
////////////////////////////////////

#define KEYFRAME_MINFRAME 0
#define KEYFRAME_MAXFRAME 4294967293//(-3) //Last possible index
#define KEYFRAME_CURRPOSE 4294967294//(-2) //current pose  //if time between kf's, this is the interpolated key
#define KEYFRAME_RESTPOSE 4294967295//(-1) //rest pose index (unsigned long)

#define KEYFRAME_ANIMNUM0 0//always zero
//#define ANIMNUM_CURRPOSE 0//always zero

#define FILE_TYPE 0
#define SCENE_TYPE 1
#define OBJECT_TYPE 2
#define MESH_TYPE 3
#define MATRIX_TYPE 4

//.................................................................................................
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
///////////////                     /////////////////
///////////////    ENUMERATIONS     /////////////////
///////////////                     /////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

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
	MOUSE_MODE_JOINT  	= 0x00000008L,
	MOUSE_MODE_WEIGHT  	= 0x00000010L,
	MOUSE_MODE_VERT    	= 0x00000020L,
	MOUSE_MODE_TRI    	= 0x00000040L,
} MOUSE_MODE;
//...............................................................................
typedef enum {
	KEYBOARD_MODE_MENU   = 0x00000001L,
	KEYBOARD_MODE_CAMERA = 0x00000002L,
} KEYBOARD_MODE;
//...............................................................................
typedef enum {
	FILE_TYPE_SCN		= 0x00000001L,
	FILE_TYPE_OBB		= 0x00000002L,
	FILE_TYPE_MSH		= 0x00000004L,
	FILE_TYPE_SKE		= 0x00000008L,
	FILE_TYPE_ASE		= 0x00000010L,
	FILE_TYPE_TXT		= 0x00000020L,
	FILE_TYPE_UKN		= 0x00000040L,//unknown
	FILE_TYPE_NDT		= 0x00000080L,//no .  ie no .msh. no extension.
} enum_FILE_TYPE;
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
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
///////////////                            /////////////////
///////////////   SIMPLE DATA STRUCTURES   /////////////////
///////////////                            /////////////////
///////////////     code located in 	   /////////////////
///////////////       Library.cpp          /////////////////
///////////////                            /////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

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
typedef struct _PERVERTEX_JOINT_DATA
{
	float weight[4];
	unsigned char index[4];	//this limits us to 256 joints max
} PERVERTEX_JOINT_DATA;
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
}aTEXTURE;
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
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
///////////////                             ////////////////
/////////////// LINKED LIST DATA STRUCTURES ////////////////
///////////////                             ////////////////
///////////////       code located in 		////////////////
///////////////       [TypeName].cpp        ////////////////
///////////////                             ////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//
// //	  ////// //	  // //	 //	///// ////	 	
// //		//	 ///  // //	//	//	  //  /		
// //		//	 //	/ // ////	////  //  /		
// //		//	 //	 /// //	//	//	  //  /		
// /////  ////// //	  // //	 //	///// ////		
//
// 	 //	   //////  ////	//////  ////	
//	 //		 //	  //	  //   //		
//	 //		 //	  ////	  //   ////	
//	 //	   	 //		 //	  //      //
//	 ///// ////// ////	  //   ////	
//
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
///////////////////////
//// JointInfoItem ////
///////////////////////

struct JointInfoItem
{
	char pName[256];
	char pParentName[256];
};


struct PrimitiveLL//TBA
{
	char pName[256];
	long flags;
};



//////////////////////////////////////////////////////////////////////////
// JointLL : CONFUSING LINKED LIST
// searchfor : JointLLWIERDNESS
// -each keyframe holds a 'skeleton' of Joints.
// -each joint *could* store its own name, but that would waste memory.
// -only the RestPose(RP) KeyFrame(KF) Joints calloc ram for pJointInfoItem. 
// -every other joint of same name, in similar keyframe LL, points to RP memory.
//     this keeps the total size down.
// -for example. assume 10 keyframes, 1 RP, 1 CurrPose, 8 anim keyframes.
// -each joint in the RP keyframe allocates 1 pJointInfoItem
// -each joint in the 9 other keyframes point to the correct pJointInfoItem
// -for example, if the arm bone is index 4 in RP then its 4 in any anim KF.
// 	JointInsert inserts a new joint into the RP KF first based on name, and parent name.
//		It also returns the index(number in order of appearence in LL 0,1,2...)
//		The new joint in then inserted into all other keyframes based on this index. >why, because JointLL insert is based on name, and parentname, and these joints dont have names to save memory
//	Insert, and Load set pJointInfoItem to point to the correct mem location.
//
// pParent points to pJointInfoItem->pParentName's JointLL Item in same KF
//////////////////////////////////////////////////////////////////////////
struct JointLL
{
	bool b_pInfoItemReal;//all joints in RP keyframe has this set to true, and callocs pInforItem Memory. All other joints point to that memory. 
	struct JointInfoItem* pInfoItem;//only joints in the RP KF allocate this. the same joints in other KFs point to the RP KFs pointer.

//	aQUATERNION Relative_Rotation;//load/save
//	aQUATERNION Relative_Position;//load/save
//	aQUATERNION Absolute_Rotation;//generated, based on relative. Absolute is only correct during mouse rotation, where abs is manipulated, and rel is updated to match
//	aQUATERNION Absolute_Position;//generated, based on relative. Absolute is only correct during mouse rotation, where abs is manipulated, and rel is updated to match

	//debug	
	float pRelativeMatrix[16];//rendering/picking
	float pAbsoluteMatrix[16];

	unsigned long JointDataViewFlags;
	long UniqueID;

	struct JointLL* pParent;
	struct JointLL* pNext;
};

//.................................................................................................
////////////////////
//// KeyFrameLL ////
////////////////////
struct KeyFrameLL
{
	//anim_num : First SortKey.
	unsigned long anim_num;

	//time : second SortKey. in ms.
	//	( 0)(0x00000000) : min time allowed
	//	(-3)(0xfffffffd) : max time allowed
	//	(-2)(0xfffffffe) : is "scratch pad" memory. holds pose for current time.
	//	(-1)(0xffffffff) : is rest pose.
	unsigned long time;

	unsigned long KeyFrameDataViewFlags;

	struct JointLL* pJointHead;

	struct KeyFrameLL* pNext;
};


//.................................................................................................
////////////////////
//// SkeletonLL ////
////////////////////
struct SkeletonLL
{
	char pName[256];//Unique Skeleton Name
	char pParentName[256];//name of parent skeleton. ie body.ske is parent to hand.ske.
	//////////////////////////////////////////////////////////////////////////////
	char pParentJointName[256];//name of joint that ske is attached to. ie wrist joint is parent to hand.ske
	long playing_anim_num;//current animation
	unsigned long starttime;//compare this to System_CurrentTime to know when to loop
	unsigned long looptime;//there has to be a looptime KF, and it is == to the 0 keyframe

	//// LL Of KeyFrames ////
	struct KeyFrameLL* pKeyFrameHead;
	//Instant Access Pointers
	struct KeyFrameLL* pRestPose;//rest pose. skin attached to pRestPose's pose. in LL
	struct KeyFrameLL* pCurrPose;//interpolated keyframe at t. in LL

	//if pParentJointName=="" fill with pThisSkeletonsObject->pMatrix -> quat
	//if pParentJointName=="joint" fill with joints RP keyframes sbsolute quat
	//filled at render time every frame
	aQUATERNION Absolute_Position;//not used yet
	aQUATERNION Absolute_Rotation;

	long flags;//mirror x,y,z,loop_anim,
	unsigned long SkeletonDataViewFlags;
	unsigned long RenderFlags;

	//the Skeletons are stored in Hiearchial order. Parents are before all their children. 
	struct SkeletonLL* pNext;
};
//.................................................................................................
///////////////////// InitWeights is only used when initializing the weights of a Mesh for the first time.
//// InitWeights //// To simplify code, this data structure was created to cache the data in a more friendly format.
///////////////////// The Biggest difference is now each bone has a list of its children. 

struct BoneItem
{
	aVECTOR Pos;
	aVECTOR Dir;
	struct JointLL* pJoint;//Joint that rotates this bone. more than one bone can reference the same Joint.
};

////////////////
//// MeshLL ////
////////////////
//NOTE : if you change MeshLL, YOU MUST RECOMPILE RENDERING DLLS
//NOTE : if you change MeshLL, YOU MUST RECOMPILE RENDERING DLLS
struct MeshLL
{
	char pName[256];
	unsigned short SortKey;

	float pMatrix[16];

	VERTEX_XYZNRGBUV* pLocalVerts;	//pointer to Local Verts. Aligned with Skeleton. 
	VERTEX_XYZNRGBUV* pWorldVerts;	//pointer to World Verts. deformed, and xformed into world.
	PERVERTEX_JOINT_DATA* pPerVertexJointData;
	unsigned char*	pVertexFlags;//selected, visible
	unsigned char*	pSelectedVerts;//phasing out

  	WORD* pIndices;			//pointer to Indices

	struct PrimitiveLL* pPrimitiveHead;//LL of primitives to be CSG'd into a single "skin mesh"

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

	unsigned long num_joints;//next available unique jointid.
	unsigned long num_animations;

	struct SkeletonLL* pSkeletonHead;//joints contain keframes

	float visual_joint_radius;//scales the joint's (mouse picking sphere) per object to account for ants, and elephants.



	struct ObjectLL* pPrev;
	struct ObjectLL* pNext;

	unsigned long ObjectDataViewFlags;
	unsigned long RenderFlags;//change to flags

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
	struct ObjectLL* pObjectHead;
	struct SceneLL* pPrev;
	struct SceneLL* pNext;
	unsigned long SceneDataViewFlags;
};
//.................................................................................................
///////////////////top level Application. Only 1 Application allowed.
//// AppItem ////
/////////////////
struct AppItem
{
	char pName[256];
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
	float xpos;	//Start X scrolling position
	float ypos;	//Start X scrolling position

	long textcolor;
	unsigned long flags; //Kerning,editable

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


/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""aVECTOR""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/


void CreateRandomVector(aVECTOR* pMat);
void RotateVectorUsingMatrix(	aVECTOR* pDstV,
								aVECTOR* pV,
								float* pM);
void RotatePointUsingMatrix(aVECTOR* pDstV,
							aVECTOR* pP,
							float* pM);
void VectorMulRotationMatrix(	aVECTOR* pDstV,
								aVECTOR* pSrcV,
								float* pMat);
void VectorMulFullMatrix(aVECTOR* pDstV,
						 aVECTOR* pSrcV,
						 float* pMat);
void VertexMatrixMul(	aVECTOR* pDstV,
						VERTEX_XYZNRGBUV* pSrcV,
						float* pMat);
void VertexMatrixMul(	VERTEX_XYZNRGBUV* pDstV,
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
float Min_Distance_PointToBone(	aVECTOR Point,
								aVECTOR RayOrigin,
								aVECTOR	RayDir);
float Tri_Distance_PointToBone(	aVECTOR Point,
								aVECTOR RayOrigin,
								aVECTOR	RayDir);
bool TransformVertex(	VERTEX_XYZNRGBUV* pDstV,
						VERTEX_XYZNRGBUV* pSrcV,
						float* pMat,
						float ScreenWidth,
						float ScreenHeight);
void FillXYZNRGBUV(VERTEX_XYZNRGBUV* pV,
					float x,
					float y,
					float z,
					float nx,
					float ny,
					float nz,
					long diffuse,
					float u,
					float v);

/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""TRIANGLE""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/

BOOL IntersectTriangle( aVECTOR& orig,
                        aVECTOR& dir, 
                        aVECTOR& v0,
                        aVECTOR& v1, 
                        aVECTOR& v2,
						float* t, 
						float* u, 
						float* v);
void CalculatePlaneNormal(aVECTOR& vR,aVECTOR pA,aVECTOR pB,aVECTOR pC);
/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""MATRIX""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/
void Identity(float *pMat);
void Scale(float *pMat,float scale);
void Scale(	float *pMat,
			float sx,
			float sy,
			float sz);
void PositionMatrix(float *pMat,float Ax,float Ay,float Az);
void LookAt(float* pMat,aVECTOR* pPos,aVECTOR* pLookAt);
void FrustumMatrix( float* pMat,
					float l,float r,
					float t,float b,
					float n,float f);
void CreateOrthoMatrix(float* pMat);
void CreateProjectionMatrix(float* pMat,float fov,float nearplane,float farplane);
float* CreateMatrixRotationX(float* pOut, float angle);
float* CreateMatrixRotationY(float* pOut, float angle);
float* CreateMatrixRotationZ(float* pOut, float angle);
float* CreateMatrixYawPitchRoll(float* pOut,float yaw, float pitch, float roll);
void MatrixMul(float* pDst,float* pA,float* pB);
void MatrixMul4x4(float* pDst,float* pA,float* pB);
void CreateRotationMatrixAroundWorldX(float* pM,float Radians);
void CreateRotationMatrixAroundWorldY(float* pM,float Radians);
void CreateRotationMatrixAroundWorldZ(float* pM,float Radians);
void AxisRotationMatrix(float *pMat,
						float Ax,
						float Ay,
						float Az,
						float Angle);
void InverseMatrix(float* pDst,float* pSrc);
void TransposeMatrix(float* pDst,float* pSrc);
void CreateMatrixFacingMatrix(	float* pDstM,
								float* pView,
								float x,
								float y,
								float z);
void CreateMatrixFacingPointOnOneAxis(	aVECTOR& Pos,
										aVECTOR& Dir,
										aVECTOR& LookAt,
										float* pDst);
void CreateLookAtMatrix(aVECTOR& Pos,
						aVECTOR& Up,
						aVECTOR& LookAtPos,
						float* pDst);
void CreateLookAtMatrixY(aVECTOR& Pos,
						aVECTOR& Up,
						aVECTOR& LookAtPos,
						float* pDst);
void CreateMatrixFromQuaternion(float* pM,
								aQUATERNION& Q);
bool IsMatrixIdentity(float* pM);
/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""aQUATERNION"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/

void CreateQuaternionFromMatrix(aQUATERNION& Q,
								float* pM);
void QuaternionConjugate(aQUATERNION& DstQ,aQUATERNION& SrcQ);
void QuaternionMultiply(aQUATERNION& pC,
						aQUATERNION& pA,
						aQUATERNION& pB);
void QuaternionNormalize(aQUATERNION& pQ);
void CreateVectorAngleFromQuaternion(	aVECTOR& pV,
										float& angle,
										aQUATERNION& pQ);
void CreateQuaternionArrowMatrix(aVECTOR& Pos,
								aVECTOR& Up,//typically <0,1,0>
								aQUATERNION& pQ,
								float* pDst);
void CreateQuaternionFadeMatrix(aVECTOR& Pos,
								aVECTOR& Up,//typically <0,1,0>
								aQUATERNION& pQ,
								float* pDst);
void RotationQuaternionIdentity(aQUATERNION& Q);
void PositionQuaternionIdentity(aQUATERNION& Q);
void CreateRotationQuaternionFromMatrix(aQUATERNION& Q,float* pM);
void CreatePositionQuaternionFromMatrix(aQUATERNION& Q,float* pM);
void PositionQuaternion(aQUATERNION& Q,float PosX,float PosY,float PosZ);
void CreateQuaternionFromAxisAngle(aQUATERNION& Q,aVECTOR Axis,float angle);
void CreateQuaternionFromMatrix(aQUATERNION& Q,
								float* pM);
void MatrixSlerp(	float* pC,
					float* pA,
					float* pB,
					float t);
void CreateAxisAngleFromMatrix(	aVECTOR& Axis,
								float& Angle,
								float* pM);
void QuaternionSlerp( 	aQUATERNION& C,
						aQUATERNION  A,
						aQUATERNION  B,
						float t);
void OrthonormalizeOrientation(float* pMat);
/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""aTEXTURE""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/

bool LoadTexture(	char*  pFileName,
					char* pFullPathFileName,
					aTEXTURE& pTexture,
					long flags); 
bool LoadRGBATexture(char*  pFileName,
					char* pFullPathFileName,
					aTEXTURE& pTexture,
					long flags); 
bool CreateTexture(char* pName,
					aTEXTURE& pTexture,
					long Width,
					long Height,
					long flags); 
bool UpdateUniqueID(aTEXTURE& pTexture);
void ReleaseTexture(aTEXTURE* pTexture);

/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""EXTRA TOOLS""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/


void SetBit(unsigned long& flags,unsigned long flag);
void SwapBit(unsigned long& flags,unsigned long flag);
void ClearBit(unsigned long& flags,unsigned long flag);
bool IsBitSet(unsigned long& flags,unsigned long flag);
void ClearBit(unsigned char& flags,unsigned char flag);
bool PointInRect(POINT point,fRECT Clip);
fRECT ClipRectToRect(fRECT Rect,fRECT Clip);
float GetSeconds(bool reset);
unsigned long GetSecondsLong(bool reset);
unsigned long GetElapsedMilliSeconds();
bool IntesectSphere(	aVECTOR& 	RayPos,
						aVECTOR& 	RayDir,
						aVECTOR& 	Center,
						double		Radius,
						double		*rin,
						double		*rout);
void CreatePickRay(	struct ObjectLL* pParentObject,
					struct MeshLL* pMesh,
					POINT ptCursor,
					aVECTOR& Pos,aVECTOR& Dir);

#endif//#define _LIBRARY_H_
