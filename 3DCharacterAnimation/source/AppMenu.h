//AppMenu.h
#ifndef _AppMenu_H_
#define _AppMenu_H_

//////////////////////////////
//////////////////////////////
//////////////////////////////
//// MAIN MENU DATA FLAGS ////
//////////////////////////////
//////////////////////////////
//////////////////////////////
#define MAX_LINES_CONSOLE_TEXT 4

#define APP_DATAVIEW_SHOW_FLAGS			0x00000001L

#define SCENE_DATAVIEW_SHOW_THIS_SCENE	0x00000001L
#define SCENE_DATAVIEW_SHOW_VIEWMATRIX	0x00000002L
#define SCENE_DATAVIEW_SHOW_PROJMATRIX	0x00000004L
#define SCENE_DATAVIEW_SHOW_OBJECTHEAD	0x00000008L

#define OBJECT_DATAVIEW_SHOW_THIS_OBJECT	0x00000001L
#define OBJECT_DATAVIEW_SHOW_MATRIX			0x00000002L
#define OBJECT_DATAVIEW_SHOW_MESHHEAD		0x00000004L
#define OBJECT_DATAVIEW_SHOW_SKELETON		0x00000008L
#define OBJECT_DATAVIEW_SHOW_FLAGS   		0x00000010L

#define MESH_DATAVIEW_SHOW_THIS_MESH		0x00000001L
#define MESH_DATAVIEW_SHOW_MATRIX			0x00000002L
#define MESH_DATAVIEW_SHOW_LOCALVERTICES	0x00000004L
#define MESH_DATAVIEW_SHOW_3dINDICES		0x00000008L
#define MESH_DATAVIEW_SHOW_SRCBLEND			0x00000010L
#define MESH_DATAVIEW_SHOW_DSTBLEND			0x00000020L
#define MESH_DATAVIEW_SHOW_FLAGS			0x00000040L
#define MESH_DATAVIEW_SHOW_WORLDVERTICES	0x00000080L


#define SKELETON_DATAVIEW_SHOW_THIS_SKELETON	0x00000001L
#define SKELETON_DATAVIEW_SHOW_FLAGS			0x00000002L

#define KEYFRAME_DATAVIEW_SHOW_THIS_KEYFRAME	0x00000001L

#define JOINT_DATAVIEW_SHOW_THIS_JOINT    	0x00000001L
#define JOINT_DATAVIEW_SHOW_RELATIVE_MATRIX 0x00000002L
#define JOINT_DATAVIEW_SHOW_ABSOLUTE_MATRIX 0x00000004L


void 		About();
bool 		AppMenu_Create();
void 		AppMenu_Delete();


void 		OutputTextToConsole(char *pText,long Color);

void 		RefreshSubordinates(struct SceneLL* pScene);
void 		RefreshSubordinates(struct ObjectLL* pObject);
void 		RefreshSubordinates(struct MeshLL* pMesh);
void 		RefreshSubordinates(struct PrimitiveLL* pPrimitive);
void 		RefreshSubordinates(VERTEX_XYZNRGBUV* pLocalVerts);
void 		RefreshSubordinates(WORD* pIndices);
void 		RefreshSubordinates(struct _aTEXTURE* pTexture);
void 		RefreshSubordinates(struct SkeletonLL* pSkeleton);
void 		RefreshSubordinates(struct KeyFrameLL* pKeyFrame);
void 		RefreshSubordinates(struct JointLL* pJoint);

//MENU FUNCTIONS
void 		MenuFunction_Application_SetTime(unsigned long NewTime);
void 		MenuFunction_Application_StartTime();
void 		MenuFunction_Application_StopTime();

SceneLL* 	MenuFunction_Scene_NewScene(char* pName);
SceneLL* 	MenuFunction_Scene_OpenScene(char* pName);
bool 		MenuFunction_Scene_SaveScene(char* pName);
SceneLL* 	MenuFunction_Scene_CloseScene();
SceneLL*  	MenuFunction_Scene_SelectFirst();
SceneLL*  	MenuFunction_Scene_SelectPrev();
SceneLL*  	MenuFunction_Scene_SelectNext();
SceneLL*  	MenuFunction_Scene_SelectLast();

ObjectLL* 	MenuFunction_Object_NewObject(char* pName);
ObjectLL* 	MenuFunction_Object_OpenObject(char* pName);
bool 		MenuFunction_Object_SaveObject(char* pName);
ObjectLL* 	MenuFunction_Object_CloseObject();
void 		MenuFunction_Object_InitBoneWeights();
void		MenuFunction_Object_ReSortObjects();
ObjectLL*  	MenuFunction_Object_SelectFirst();
ObjectLL*  	MenuFunction_Object_SelectPrev();
ObjectLL*  	MenuFunction_Object_SelectNext();
ObjectLL*  	MenuFunction_Object_SelectLast();

MeshLL* 	MenuFunction_Mesh_NewMesh(char* pName);
MeshLL* 	MenuFunction_Mesh_OpenMesh(char* pName);
bool 		MenuFunction_Mesh_SaveMesh(char* pName);
MeshLL* 	MenuFunction_Mesh_CloseMesh();
MeshLL*  	MenuFunction_Mesh_SelectFirst();
MeshLL*  	MenuFunction_Mesh_SelectPrev();
MeshLL*  	MenuFunction_Mesh_SelectNext();
MeshLL*  	MenuFunction_Mesh_SelectLast();
void		MenuFunction_Mesh_ReSortMeshes();


SkeletonLL*	MenuFunction_Skeleton_New(char* pName,char* pParentName);
SkeletonLL*	MenuFunction_Skeleton_Open(char* pName);
void 		MenuFunction_Skeleton_Edit(	char* pName,
										char* pParentName,
										char* pParentJointName,
										long playing_anim_num);
bool		MenuFunction_Skeleton_Save(char* pName);
void		MenuFunction_Skeleton_Close();
SkeletonLL* MenuFunction_Skeleton_SelectFirst();
SkeletonLL* MenuFunction_Skeleton_SelectPrev();
SkeletonLL* MenuFunction_Skeleton_SelectNext();
SkeletonLL* MenuFunction_Skeleton_SelectLast();
void 		MenuFunction_Skeleton_InitWeights();

KeyFrameLL* MenuFunction_Skeleton_KeyFrame_New(unsigned long anim_num,unsigned long time);
KeyFrameLL* MenuFunction_Skeleton_KeyFrame_Copy(unsigned long from_anim_num,unsigned long from_time,unsigned long to_anim_num,unsigned long to_time);
void 		MenuFunction_Skeleton_KeyFrame_Close(unsigned long anim_num,unsigned long time);
KeyFrameLL* MenuFunction_Skeleton_KeyFrame_SelectFirst();
KeyFrameLL* MenuFunction_Skeleton_KeyFrame_SelectPrev();
KeyFrameLL* MenuFunction_Skeleton_KeyFrame_SelectNext();
KeyFrameLL* MenuFunction_Skeleton_KeyFrame_SelectLast();
KeyFrameLL* MenuFunction_Skeleton_KeyFrame_SelectRestPose();


JointLL*	MenuFunction_Skeleton_Joint_New(	
									struct SkeletonLL* pSkeleton,
									char* pName,
									char* pParentName,
									float PosX,
									float PosY,
									float PosZ,
									bool  bRelativeData);

void 		MenuFunction_Skeleton_Joint_Edit();
void 		MenuFunction_Skeleton_Joint_Close();
JointLL* 	MenuFunction_Skeleton_Joint_SelectFirst();
JointLL* 	MenuFunction_Skeleton_Joint_SelectPrev();
JointLL* 	MenuFunction_Skeleton_Joint_SelectNext();
JointLL* 	MenuFunction_Skeleton_Joint_SelectLast();
void 		MenuFunction_Skeleton_Joint_RemoveContributer(char* pName);
char* 		GetConsoleText(int line_num);
long 		GetConsoleTextColor(int line_num);
void		OutputTextToConsole(char *pText,long Color);
long 		WrapQueueIndex(long index);
long 		BlendColor(long Color1,long Color2,float t);


#endif//_AppMenu_H_
