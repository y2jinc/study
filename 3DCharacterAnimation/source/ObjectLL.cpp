// ObjectLL.cpp written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to

#include "stdafx.h"
#include "resource.h"
#include "Library.h"
#include "FileAccess.h"
#include "MeshLL.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "MeshLL.h"
#include "SkeletonLL.h"
#include "JointLL.h"

extern struct ObjectLL* pCurrentlySelectedObject;//unacceptable "reacharound!!!"
extern float* 			pRender_MatrixArray[];
extern float  			pRender_ViewMatrix[];
extern unsigned long    System_CurrentTime;//ms
extern struct MeshLL*  	pSystem_VertexHandles;
extern POINT ptSystem_SelectRectStart;
extern POINT ptSystem_SelectRectStop;
extern char* gp_TextBuffer1;
extern char* gp_TextBuffer2;
extern char* gp_TextBuffer3;

extern void AppendDebugMsg(char *pChar);

struct ObjectLL* ObjectLL_Insert(	unsigned long SortKey,
									char* pName,
									struct ObjectLL*& pObjectHeadIn)

{
	struct ObjectLL* pObjectHead = pObjectHeadIn;
	struct ObjectLL* pInsertMe = NULL;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pNext = NULL;

	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe = (struct ObjectLL *)calloc(1,sizeof(struct ObjectLL));
	pInsertMe->SortKey = SortKey;
	FillIdentity(pInsertMe->pMatrix);
	sprintf(pInsertMe->pName,"%s",pName);

//	if(strcmp(pExt,"obj")==0) 
//	pInsertMe->pJointHead = New("SkeletonRootNode",0.0,0,NULL);

	//init skeleton
	//pInsertMe->pSkeleton = (struct Skeleton *)calloc(1,sizeof(struct Skeleton));
	//sprintf(pInsertMe->pSkeleton->pName,"%s Skeleton",pName);

	//INIT Skeleton. Each ObjectLL Item has a Skeleton, even if no Joints are attached.

//	pCurrentlySelectedObject->pSkeleton = (struct Skeleton *)calloc(1,sizeof(struct Skeleton));

/*
	memset(&pInsertMe->JointHead,0,sizeof(struct JointLL));
	sprintf(pInsertMe->JointHead.pName,"%s Root Joint",pName);
	pInsertMe->JointHead.flags      = Joint_ROOTJoint;
	pInsertMe->JointHead.JointLength = 1.0;
	Identity(pInsertMe->JointHead.pRotationMatrix);
	Identity(pInsertMe->JointHead.pTranslationMatrix);
*/
	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pObjectHead == NULL)
	{
		pObjectHeadIn = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pObjectHead->SortKey)
	{	
		pInsertMe->pNext  = pObjectHead;
		pObjectHeadIn      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pObjectHead;
	while(	pThis->pNext && 
		   (pInsertMe->SortKey > pThis->pNext->SortKey))
	{
		pThis = pThis->pNext;
	}
	pNext = pThis->pNext;
	pThis->pNext = pInsertMe;
	pInsertMe->pNext = pNext;
	return pInsertMe;
}

void ObjectLL_Remove(char* pName,
					struct ObjectLL*& pObjectHeadIn)

{
	struct ObjectLL* pObjectHead = pObjectHeadIn;
	struct ObjectLL* pRemoveMe  = NULL;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pNext = NULL;
	struct ObjectLL* pLast = NULL;

	pThis = pObjectHead;
	pLast = pObjectHead;
	while(pThis!=NULL)
	{
		//if(pThis->UniqueID == UniqueID)
		if(strcmp(pThis->pName,pName)==0)
		{//remove this item
			pRemoveMe = pThis;

			pNext = pRemoveMe->pNext;

			pThis = pThis->pNext;//update this now before we delete mem pointed to by pThis
			///////////////////////////////////
			//// Are We Removing The Head? ////
			///////////////////////////////////

			if(pRemoveMe == pObjectHead)
			{//We are removing the head
				if(pNext==NULL)
				{//we are removing the last
					pObjectHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pObjectHeadIn = pNext;
				}
			}
			else
			{//We are removing a normal item
				pLast->pNext = pNext;
			}

			//free all objects.
			MeshLL_RemoveALL(pRemoveMe->pMeshHead);
 			SkeletonLL_CloseALL(pRemoveMe->pSkeletonHead);

			//MainMenu MainMenu
			if(pCurrentlySelectedObject==pRemoveMe)
			{//if we delete current object, clear all pointers to the data
				pCurrentlySelectedObject = NULL;
			}

			free(pRemoveMe);
		}
		else
		{//check next item
			pLast = pThis;
			pThis = pThis->pNext;
		}
	}
}
void ObjectLL_Remove(	struct ObjectLL*& pRemoveMe,
						struct ObjectLL*& pObjectHeadIn)
{
	struct ObjectLL* pObjectHead = pObjectHeadIn;
//	struct ObjectLL* pRemoveMe  = NULL;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pNext = NULL;
	struct ObjectLL* pLast = NULL;

	pThis = pObjectHead;
	pLast = pObjectHead;
	while(pThis!=NULL)
	{
		if(pThis == pRemoveMe)
		{//remove this item
//			pRemoveMe = pThis;
//			pNext = pRemoveMe->pNext;
//			pThis = pThis->pNext;//update this now before we delete mem pointed to by pThis
			///////////////////////////////////
			//// Are We Removing The Head? ////
			///////////////////////////////////

			if(pThis == pObjectHead)
			{//We are removing the head
				if(pThis->pNext==NULL)
				{//we are removing the last
					pObjectHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pObjectHeadIn = pThis->pNext;
				}
			}
			else
			{//We are removing a normal item
				pLast->pNext = pThis->pNext;
			}

			//free all objects.
			MeshLL_RemoveALL(pThis->pMeshHead);
			SkeletonLL_CloseALL(pThis->pSkeletonHead);

			//MainMenu MainMenu
			if(pThis==pCurrentlySelectedObject)
			{//if we delete current object, clear all pointers to the data
				pCurrentlySelectedObject = NULL;
			}
			free(pThis);
			return;
		}
		else
		{//check next item
			pLast = pThis;
			pThis = pThis->pNext;
		}
	}
	assert(0);//obj not found
}
void ObjectLL_RemoveALL(struct ObjectLL*& pObjectHeadIn)

{
	struct ObjectLL* pRemoveMe  = NULL;
	struct ObjectLL* pThis = NULL;

	pThis = pObjectHeadIn;
	while(pThis!=NULL)
	{
		pRemoveMe = pThis;
		pThis = pThis->pNext;

		//free all objects.
		MeshLL_RemoveALL(pRemoveMe->pMeshHead);
		SkeletonLL_CloseALL(pRemoveMe->pSkeletonHead);


		//MainMenu MainMenu
		if(pCurrentlySelectedObject==pRemoveMe)
		{//if we delete current object, clear all pointers to the data
			pCurrentlySelectedObject = NULL;
		}

		free(pRemoveMe);
	}
	pObjectHeadIn = NULL;
}
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
/*
void ObjectLL_Sort(struct ObjectLL*& pObjectHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
{
	struct ObjectLL* pNewObjectHead = NULL;
	struct ObjectLL* pThis = NULL;

	pThis = pObjectHeadIn;
	while(pThis!=NULL)
	{
		ObjectLL_Insert(	pThis->SortKey,
						pThis->pName,
						pNewObjectHead);
		pThis = pThis->pNext;
	}
	pObjectHeadIn = pNewObjectHead;
}
*/

struct ObjectLL* ObjectLL_ReInsert(	struct ObjectLL* 	pObject,
									struct ObjectLL*& pObjectHeadIn)
{
	struct ObjectLL* pObjectHead = pObjectHeadIn;
	struct ObjectLL* pInsertMe = pObject;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pNext = NULL;

	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe->pNext	  = NULL;

	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pObjectHead == NULL)
	{
		pObjectHeadIn = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pObjectHead->SortKey)
	{	
		pInsertMe->pNext  = pObjectHead;
		pObjectHeadIn      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pObjectHead;
	while(	pThis->pNext && 
		   (pInsertMe->SortKey > pThis->pNext->SortKey))
	{
		pThis = pThis->pNext;
	}
	pNext = pThis->pNext;
	pThis->pNext = pInsertMe;
	pInsertMe->pNext = pNext;
	return pInsertMe;
}








bool ObjectLL_Get(	struct ObjectLL*  pObjectHeadIn,
					struct ObjectLL*& pPrevIn,
					struct ObjectLL*& pThisIn,
					struct ObjectLL*& pNextIn,
					char* pName)
{
	/////////////////////
	//// Test Inputs ////
	/////////////////////

	if(pName==NULL)return false;

	//////////////////////////
	//// Search for pName ////
	//////////////////////////

	pPrevIn = NULL;
	pThisIn = pObjectHeadIn;
	pNextIn = (pThisIn) ? (pThisIn->pNext) : (NULL);

	while(pThisIn)
	{
		if(strcmp(pThisIn->pName,pName)==0)
		{//found pname. pointers are correct
			return true;
		}
		pPrevIn = pThisIn;
		pThisIn = pNextIn;
		pNextIn = (pThisIn) ? (pThisIn->pNext) : (NULL);
	}
	//failed to find pName.
	//pointers are correct : pprev points to last item, and pthi&pnext are null
	return false;//item dne
}

void ObjectLL_GetFirst(	struct ObjectLL*  pObjectHeadIn,
						struct ObjectLL*& pPrev,
						struct ObjectLL*& pThis,
						struct ObjectLL*& pNext)
{
	pPrev = NULL;
	pThis = pObjectHeadIn;
	pNext = (pObjectHeadIn) ? (pObjectHeadIn->pNext) : (NULL);
}

void ObjectLL_GetLast(	struct ObjectLL*  pObjectHeadIn,
						struct ObjectLL*& pPrev,
						struct ObjectLL*& pThis,
						struct ObjectLL*& pNext)
{
	pPrev = NULL;
	pThis = NULL;
	pNext = pObjectHeadIn;

	while(pNext)
	{
		pPrev = pThis;
		pThis = pNext;
		pNext = pThis->pNext;
	}
}






void ObjectLL_Sort(struct ObjectLL*& pObjectHeadIn)
{
	struct ObjectLL* pNewObjectHead = NULL;
	struct ObjectLL* pThis = NULL;
	struct ObjectLL* pReInsertMe = NULL;
	struct ObjectLL* pSaveObject = NULL;

	pThis       = pObjectHeadIn;
	pObjectHeadIn = NULL;
	while(pThis!=NULL)
	{
		pReInsertMe = pThis;
		pSaveObject = pThis->pNext;
		ObjectLL_ReInsert(pReInsertMe,pObjectHeadIn);
		pThis = pSaveObject;
	}
}


//bool ObjectLL_Load(struct ObjectLL*& pObjectHeadIn,char *pFileName)
bool ObjectLL_Load(struct ObjectLL* pObject,char *pFileName)
{
//	struct ObjectLL* pNewObject = NULL;
	struct MeshLL* pNewMesh = NULL;
	HANDLE fH;
	long i,length;
//	int returnval;
	BOOL bResult = FALSE;
	unsigned long num_meshes;
	unsigned long num_skeletons;
	unsigned char *pSrc = NULL;

//	char pObjectName[256];
	char pMeshName[256];
	char pSkeletonName[256];
	char pSkeletonParentName[256];
//	char FullName[512];

	struct SkeletonLL* pSkeleton = NULL;


//	if(pObject==NULL)return false;//you must create memory
	
//	sprintf(FullName,"%s.obb",pFileName);//obj is used by the compiler
//	AddPathToFileName(FullName);
	
//	fH = CreateFile(FullName,GENERIC_READ,


	length = strlen(pFileName);

	if(length==0 || length>=MAX_PATHNAME)return false;

	memset(gp_TextBuffer1,0,MAX_PATHNAME);
	memcpy(gp_TextBuffer1,pFileName,length);
	RemoveFilenameAndExt(gp_TextBuffer1);

	fH = CreateFile(pFileName,GENERIC_READ,
   	    0, (LPSECURITY_ATTRIBUTES) NULL,
   	    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
   	    (HANDLE) NULL);
	if(fH != INVALID_HANDLE_VALUE)
	{
		ReadFromFile(fH,(char*) pObject->pName,256);
		ReadFromFile(fH,(char*)&pObject->SortKey,sizeof(unsigned long));
		ReadFromFile(fH,(char*) pObject->pMatrix,16*sizeof(float));

		ReadFromFile(fH,(char*)&num_meshes,sizeof(unsigned long));

		/////////////////////////
		//// Open All Meshes ////
		/////////////////////////

		for(i=0;i<num_meshes;i++)
		{
			ReadFromFile(fH,(char*)pMeshName,256);
			pNewMesh = MeshLL_Insert(0,pMeshName,pObject->pMeshHead);

			sprintf(gp_TextBuffer2,"%s\\%s.msh",gp_TextBuffer1,pMeshName);

			if(MeshLL_Load(pNewMesh,gp_TextBuffer2))
			{
			}
			else
			{
				//you could free pNewMesh.
			}
		}

		MeshLL_Sort(pObject->pMeshHead);


		///////////////////////////
		//// Open All Skeleton ////
		///////////////////////////

		ReadFromFile(fH,(char*)&num_skeletons,sizeof(unsigned long));

		for(i=0;i<num_skeletons;i++)
		{
			ReadFromFile(fH,(char*)pSkeletonName,256);
			ReadFromFile(fH,(char*)pSkeletonParentName,256);

			//note Skeleton_Open calls Skeleton_Insert
			/*
			pSkeleton = SkeletonLL_Append(	pObject->pSkeletonHead,
											pSkeletonName,
											pSkeletonParentName);

			*/
			sprintf(gp_TextBuffer2,"%s\\%s.ske",gp_TextBuffer1,pSkeletonName);

			if(SkeletonLL_Open(pObject->pSkeletonHead,gp_TextBuffer2))
			{
			}
			else
			{
				//you could free pNewSkeleton.
			}
		}
		CloseHandle(fH);
		return true;
	}//End Open File
	return false;
}//End 

bool ObjectLL_Save(struct ObjectLL* pObject,char *pFileName)
{
	struct MeshLL* pMesh = NULL;
	HANDLE fH;
	int returnval;
	BOOL bResult = FALSE;
	unsigned long num_meshes;
	unsigned long num_skeletons;
//	unsigned long num_chars;
	unsigned char *pSrc = NULL;
//	char FullName[512];
	struct SkeletonLL* pSkeleton = NULL;

//	sprintf(FullName,"%s.obb",pFileName);//obj is used by the compiler
//	AddPathToFileName(FullName);

	long length = strlen(pFileName);

	if(length==0 || length>=MAX_PATHNAME)return false;

	memset(gp_TextBuffer1,0,MAX_PATHNAME);
	memcpy(gp_TextBuffer1,pFileName,length);
	RemoveFilenameAndExt(gp_TextBuffer1);
	
	fH = CreateFile(pFileName,GENERIC_WRITE,
   	    0, (LPSECURITY_ATTRIBUTES) NULL,
   	    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
   	    (HANDLE) NULL);
	if(fH != INVALID_HANDLE_VALUE)
	{
		WriteToFile(fH,(char*) pObject->pName,256);
		WriteToFile(fH,(char*)&pObject->SortKey,sizeof(unsigned long));
		WriteToFile(fH,(char*) pObject->pMatrix,16*sizeof(float));

		//////////////////////////
		//// Count All Meshes ////
		//////////////////////////
		
		num_meshes = MeshLL_Count(pObject->pMeshHead);
		WriteToFile(fH,(char*)&num_meshes,sizeof(unsigned long));

		//////////////////////////////////////////////
		//// Save All Mesh Names                  ////
		//// Each Mesh is stored in its own file. ////
		//////////////////////////////////////////////

		pMesh = pObject->pMeshHead;
		while(pMesh)	 
		{
			WriteToFile(fH,(char*)pMesh->pName,256);
		 	pMesh = pMesh->pNext;
		}

		/////////////////////////////
		//// Count All Skeletons ////
		/////////////////////////////
		
		num_skeletons = SkeletonLL_Count(pObject->pSkeletonHead);
		WriteToFile(fH,(char*)&num_skeletons,sizeof(unsigned long));

		//////////////////////////////////////////////////
		//// Save All Skeleton Names                  ////
		//// Each Skeleton is stored in its own file. ////
		//////////////////////////////////////////////////

		pSkeleton = pObject->pSkeletonHead;
		while(pSkeleton)	 
		{
			WriteToFile(fH,(char*)pSkeleton->pName,256);
			WriteToFile(fH,(char*)pSkeleton->pParentName,256);
		 	pSkeleton = pSkeleton->pNext;
		}

		///////////////////////////
		//// CLOSE Object File ////
		//// CLOSE Object File //////////////////////////////
		//// Meshes    are saved in their own *.msh File ////
		//// Skeletons are saved in their own *.ske File ////
		/////////////////////////////////////////////////////

		returnval = CloseHandle(fH);

		/////////////////////////
		//// Save All Meshes ////
		/////////////////////////

		pMesh = pObject->pMeshHead;
		while(pMesh)	 
		{
			sprintf(gp_TextBuffer2,"%s\\%s.msh",gp_TextBuffer1,pMesh->pName);

			MeshLL_Save(pMesh,gp_TextBuffer2);
		 	pMesh = pMesh->pNext;
		}

		///////////////////////////
		//// Save All Skeleton ////
		///////////////////////////

		pSkeleton = pObject->pSkeletonHead;
		while(pSkeleton)	 
		{
			sprintf(gp_TextBuffer2,"%s\\%s.ske",gp_TextBuffer1,pSkeleton->pName);

			SkeletonLL_Save(pSkeleton,gp_TextBuffer2);
		 	pSkeleton = pSkeleton->pNext;
		}
		return true;

	}//End Open File
	return false;
}//End 



/*
BOOL ObjectLL_Save(char *pFileName)
{
	HANDLE fH;
	int returnval;
	BOOL bResult = FALSE;
	DWORD justwritten,byteswritten,count;
	unsigned long Size;
	unsigned long Index;
	unsigned char *pSrc = NULL;

	fH = CreateFile(pFileName,GENERIC_WRITE,
   	    0, (LPSECURITY_ATTRIBUTES) NULL,
   	    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
   	    (HANDLE) NULL);
	if(fh != INVALID_HANDLE_VALUE)
	{
		struct ObjectLL* pItem = pFileHead;
		while(pItem)
		{
			Size = 5*sizeof(unsigned long);
			memcpy(pMiniBuffer2,(char*)&pItem->ObjectType,Size);
			Index = Size;

			Size = pItem->PacketSize;
			memcpy(&pMiniBuffer2[Index],pItem->pPacket,Size);
			Index += Size;

			/////////////////////////////////
			//// Write This Node To Disk ////
			/////////////////////////////////

			byteswritten = 0;
			Size = 5*sizeof(unsigned long)+pItem->PacketSize;
			while(byteswritten<Size)
			{
				//limit packet size to 30000
    		  	count = Size-byteswritten;
				if(count>30000)count=30000;

				bResult = WriteFile(fH,(LPCVOID)&pMiniBuffer2[byteswritten],count,&justwritten,NULL);
				byteswritten += justwritten;
			}
			pItem = pItem->pNext;
		}//End while(pItem)
		returnval = CloseHandle(fH);
	}//End Open File
	return FALSE;
}//End 


*/


/*



typedef struct _MeshLL
{
	//pointer to mesh object
	unsigned long MeshIndex;
	_MeshLL	*pPrev;
	_MeshLL	*pNext;
}MeshLL;

typedef struct _ObjLL
{
	unsigned long ObjIndex;
	_MeshLL	*pMeshHead;
	_ObjLL 	*pPrev;
	_ObjLL 	*pNext;
}ObjLL;

typedef struct _BinLL
{
	unsigned long BinIndex;
	_ObjLL 	*pObjHead;
	_BinLL 	*pPrev;
	_BinLL 	*pNext;
}BinLL;

typedef struct _ObjectLL
{
	unsigned long BinIndex;
	_BinLL 	 *pBinHead;
	_ObjectLL *pPrev;
	_ObjectLL *pNext;
}ObjectLL;


*/


void ObjectLL_SetAllMeshs_Flags(struct ObjectLL*& pObject,unsigned long flags)
{
	struct MeshLL* pMesh = NULL;

	pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		pMesh->RenderFlags |= flags;
	 	pMesh = pMesh->pNext;
	}
}

void ObjectLL_SetInVRAM(struct ObjectLL* pObject,bool bInVRAM)
{
	struct MeshLL* pMesh = NULL;

	pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		MeshLL_SetInVRAM(pMesh,bInVRAM);
	 	pMesh = pMesh->pNext;
	}
}




void ObjectLL_ClearAllMeshs_Flags(struct ObjectLL*& pObject,unsigned long flags)
{
	struct MeshLL* pMesh = NULL;

	pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		pMesh->RenderFlags -= (pMesh->RenderFlags & flags);
	 	pMesh = pMesh->pNext;
	}
}



//used to restore after editing Joint weights.
void ObjectLL_ColorizeWhite(struct ObjectLL* pObject)
{
	struct MeshLL* pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		MeshLL_ColorizeWhite(pMesh);
	 	pMesh = pMesh->pNext;
	}
}
void ObjectLL_ColorizeBlack(struct ObjectLL* pObject)
{
	struct MeshLL* pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		MeshLL_ColorizeBlack(pMesh);
	 	pMesh = pMesh->pNext;
	}
}
void ObjectLL_CopyLocalToWorld(struct ObjectLL* pObject)
{
	struct MeshLL* pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		MeshLL_CopyLocalToWorld(pMesh);
	 	pMesh = pMesh->pNext;
	}
}


//assumes current pose holds pose for current time.
//	by calling UpdateCurrentPose(	struct JointH* pSkeletonRootNode,unsigned long time)
//this function uses skeleton using pCurrPose keyframe
// calculates transition matrices from
//	pRestPose->Absolute
//		TO
//	pCurrPose->Absolute
		// T = (A^-1)*B
		//MatrixMul(pFinalModelMatrix,pBone->pRestPose->pAbsoluteMatrix,pObject->pMatrix);
//wait mul by obj already handled
		//KEYWORD:RENDER_TRANSFORMED_VERTEXSELECTION

//		if(System_CurrentTime==KEYFRAME_RESTPOSE)
//		{
//			MatrixMul(pFinalModelMatrix,pBone->pCurrPose->pAbsoluteMatrix,pSystem_IdentityMatrix);
//		}
//		else
//		{
//			MatrixMul(pFinalModelMatrix,pBone->pCurrPose->pAbsoluteMatrix,pObject->pMatrix);
//		}
//		MatrixMul(pTransition,pInverse,pFinalModelMatrix);

	//create Transition Matrices that transform each Joint from RestPose to CurrPose
	//when local space vertices are multiplied with the Trans Matrix, the vertex assumes
	//	its natural position on the current poses skeleton.

void ObjectLL_DeformMeshesUsingSkeleton(struct ObjectLL* pObject)
{
//	struct JointLL* pJoint = NULL;
	VERTEX_XYZNRGBUV* pLocalVert = NULL;
	VERTEX_XYZNRGBUV* pWorldVert = NULL;
	PERVERTEX_JOINT_DATA* pPVBData = NULL;
	float pInverse[16];
	float pTransition[16];
//	float pFinalModelMatrix[16];
	long i;
	long counter = 0;//not same as uniqueid
	aVECTOR NewPos0,NewPos1,NewPos2,NewPos3;

	struct SkeletonLL* pSkeleton;
	struct KeyFrameLL* pRestPose;//rest pose. skin attached to pRestPose's pose. in LL
	struct KeyFrameLL* pCurrPose;//interpolated keyframe at t. in LL
	struct JointLL* pRestPose_Joint = NULL;
	struct JointLL* pCurrPose_Joint = NULL;

	NewPos0.x = 0.0;
	NewPos0.y = 0.0;
	NewPos0.z = 0.0;
	NewPos1.x = 0.0;
	NewPos1.y = 0.0;
	NewPos1.z = 0.0;
	NewPos2.x = 0.0;
	NewPos2.y = 0.0;
	NewPos2.z = 0.0;
	NewPos3.x = 0.0;
	NewPos3.y = 0.0;
	NewPos3.z = 0.0;

	if(pObject==NULL)return;
	if(pObject->pSkeletonHead==NULL)//return;
	{//no skeleton, so just copy local to world, so that editweights at least looks correct.
		struct MeshLL* pMesh = pObject->pMeshHead;
		while(pMesh)	 
		{
			MeshLL_CopyLocalToWorld(pMesh);
		 	pMesh = pMesh->pNext;
		}
		return;
	}

	pSkeleton = pObject->pSkeletonHead;
	while(pSkeleton)
	{
		pRestPose = pSkeleton->pRestPose;
		pCurrPose = pSkeleton->pCurrPose;

		pRestPose_Joint = pRestPose->pJointHead;
		pCurrPose_Joint = pCurrPose->pJointHead;

		counter = 0;
		while(pRestPose_Joint && pCurrPose_Joint)
		{
			InverseMatrix(pInverse,pRestPose_Joint->pAbsoluteMatrix);
			MatrixMul(pTransition,pInverse,pCurrPose_Joint->pAbsoluteMatrix);
			memcpy(pRender_MatrixArray[pRestPose_Joint->UniqueID],pTransition,16*sizeof(float));
			//memcpy(pRender_MatrixArray[counter],pTransition,16*sizeof(float));
		
			counter++;
			pRestPose_Joint = pRestPose_Joint->pNext;
			pCurrPose_Joint = pCurrPose_Joint->pNext;
		}
		pSkeleton = pSkeleton->pNext;
	}


	struct MeshLL* pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		pPVBData   = pMesh->pPerVertexJointData;
		pLocalVert = pMesh->pLocalVerts;
		pWorldVert = pMesh->pWorldVerts;
		////////////////////////////////////
		for(i=0;i<pMesh->num_verts;i++)
		{
			//wV = (lV*M[bi1])*bw1 +
			//     (lV*M[bi2])*bw2 +
			//     (lV*M[bi3])*bw3 +
			//     (lV*M[bi4])*bw4;

			//do Vertex Matrix Multiply first. 
			//	the vertex is transformed by each Joint as if it were the only bone.
/*
			if(pPVBData[i].index[0] == 2 || pPVBData[i].index[1] == 2)
			{
				printf("f");
			}	
*/
			if(pPVBData[i].weight[0]!=0.0)
			{
				VertexMatrixMul(&NewPos0,&pLocalVert[i],pRender_MatrixArray[pPVBData[i].index[0]]);
			}
			if(pPVBData[i].weight[1]!=0.0)
			{
				VertexMatrixMul(&NewPos1,&pLocalVert[i],pRender_MatrixArray[pPVBData[i].index[1]]);
			}
			if(pPVBData[i].weight[2]!=0.0)
			{
				VertexMatrixMul(&NewPos2,&pLocalVert[i],pRender_MatrixArray[pPVBData[i].index[2]]);
			}
			if(pPVBData[i].weight[3]!=0.0)
			{
				VertexMatrixMul(&NewPos3,&pLocalVert[i],pRender_MatrixArray[pPVBData[i].index[3]]);
			}

			// now blend resulting positions together, using weights.

			pWorldVert[i].x = 	(NewPos0.x*pPVBData[i].weight[0]) +
								(NewPos1.x*pPVBData[i].weight[1]) +
								(NewPos2.x*pPVBData[i].weight[2]) +
								(NewPos3.x*pPVBData[i].weight[3]);

			pWorldVert[i].y = 	(NewPos0.y*pPVBData[i].weight[0]) +
								(NewPos1.y*pPVBData[i].weight[1]) +
								(NewPos2.y*pPVBData[i].weight[2]) +
								(NewPos3.y*pPVBData[i].weight[3]);

			pWorldVert[i].z = 	(NewPos0.z*pPVBData[i].weight[0]) +
								(NewPos1.z*pPVBData[i].weight[1]) +
								(NewPos2.z*pPVBData[i].weight[2]) +
								(NewPos3.z*pPVBData[i].weight[3]);




			/////////////////////////////
			//// Interpolate Normals ////
			/////////////////////////////
	

			if(pPVBData[i].weight[0]!=0.0)
				NormalMatrixMul(&NewPos0,&pLocalVert[i],pRender_MatrixArray[pPVBData[i].index[0]]);
			if(pPVBData[i].weight[1]!=0.0)
				NormalMatrixMul(&NewPos1,&pLocalVert[i],pRender_MatrixArray[pPVBData[i].index[1]]);
			if(pPVBData[i].weight[2]!=0.0)
				NormalMatrixMul(&NewPos2,&pLocalVert[i],pRender_MatrixArray[pPVBData[i].index[2]]);
			if(pPVBData[i].weight[3]!=0.0)
				NormalMatrixMul(&NewPos3,&pLocalVert[i],pRender_MatrixArray[pPVBData[i].index[3]]);

			// now blend resulting positions together, using weights.

			pWorldVert[i].nx = 	(NewPos0.x*pPVBData[i].weight[0]) +
								(NewPos1.x*pPVBData[i].weight[1]) +
								(NewPos2.x*pPVBData[i].weight[2]) +
								(NewPos3.x*pPVBData[i].weight[3]);

			pWorldVert[i].ny = 	(NewPos0.y*pPVBData[i].weight[0]) +
								(NewPos1.y*pPVBData[i].weight[1]) +
								(NewPos2.y*pPVBData[i].weight[2]) +
								(NewPos3.y*pPVBData[i].weight[3]);

			pWorldVert[i].nz = 	(NewPos0.z*pPVBData[i].weight[0]) +
								(NewPos1.z*pPVBData[i].weight[1]) +
								(NewPos2.z*pPVBData[i].weight[2]) +
								(NewPos3.z*pPVBData[i].weight[3]);


//			pWorldVert[i].nx = pLocalVert[i].nx;
//			pWorldVert[i].ny = pLocalVert[i].ny;
//			pWorldVert[i].nz = pLocalVert[i].nz;

			pWorldVert[i].diffuse = pLocalVert[i].diffuse;//gets overwritten by editweights

			pWorldVert[i].tu = pLocalVert[i].tu;
			pWorldVert[i].tv = pLocalVert[i].tv;

		}
	 	pMesh = pMesh->pNext;
	}
}
















void ObjectLL_ReplaceWeightID(	struct ObjectLL* pObject,
								unsigned long OldIndex,
								unsigned long NewIndex)
{
}


void ObjectLL_ColorizeUsingBoneWeights(	struct ObjectLL* pObject,
										unsigned long JointIndex,
										unsigned long Red,
										unsigned long Grn,
										unsigned long Blu)
{
	struct MeshLL* pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		MeshLL_ColorizeUsingBoneWeights(pMesh,JointIndex,Red,Grn,Blu);
	 	pMesh = pMesh->pNext;
	}
}



void ObjectLL_AllMeshs_SetBit(struct ObjectLL* pObject,unsigned long flags)
{
	struct MeshLL* pMesh = NULL;

	pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		SetBit(pMesh->RenderFlags,flags);
	 	pMesh = pMesh->pNext;
	}
}
void ObjectLL_AllMeshs_ClearBit(struct ObjectLL* pObject,unsigned long flags)
{
	struct MeshLL* pMesh = NULL;

	pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		ClearBit(pMesh->RenderFlags,flags);
	 	pMesh = pMesh->pNext;
	}
}

void ObjectLL_SetAllMeshs_BlendMode(struct ObjectLL* pObject,long SrcBlend,long DstBlend)
{
	struct MeshLL* pMesh = NULL;

	pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		pMesh->SrcBlend = SrcBlend;
		pMesh->DstBlend = DstBlend;
	 	pMesh = pMesh->pNext;
	}
}
void ObjectLL_SetAllMeshs_VertexAlpha(struct ObjectLL* pObject,float alpha)
{
	struct MeshLL* pMesh = NULL;

	pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		MeshLL_SetAllVertices_Alpha(pMesh,alpha);
	 	pMesh = pMesh->pNext;
	}
}


void ObjectLL_GetMinMaxXYZ(	struct ObjectLL* pObject,
							float& ObjectMinX,
							float& ObjectMinY,
							float& ObjectMinZ,
							float& ObjectMaxX,
							float& ObjectMaxY,
							float& ObjectMaxZ)
{
	struct MeshLL* pMesh = NULL;
	float MinX,MinY,MinZ;
	float MaxX,MaxY,MaxZ;

	if(pObject == NULL ||pObject->pMeshHead == NULL)
	{
		ObjectMinX = 0.0;
		ObjectMinY = 0.0;
		ObjectMinZ = 0.0;

		ObjectMaxX = 1.0;
		ObjectMaxY = 1.0;
		ObjectMaxZ = 1.0;
		return;
	}


	pMesh = pObject->pMeshHead;

	//init ObjectMin Max with first Mesh.
	MeshLL_GetMinMaxXYZ(pMesh,	ObjectMinX,ObjectMinY,ObjectMinZ,
								ObjectMaxX,ObjectMaxY,ObjectMaxZ);

	//start with next item
	pMesh = pMesh->pNext;
	while(pMesh)	 
	{
		MeshLL_GetMinMaxXYZ(pMesh,	MinX,MinY,MinZ,
									MaxX,MaxY,MaxZ);

		if(MinX<ObjectMinX)ObjectMinX=MinX;
		if(MaxX>ObjectMaxX)ObjectMaxX=MaxX;

		if(MinY<ObjectMinY)ObjectMinY=MinY;
		if(MaxY>ObjectMaxY)ObjectMaxY=MaxY;

		if(MinZ<ObjectMinZ)ObjectMinZ=MinZ;
		if(MaxZ>ObjectMaxZ)ObjectMaxZ=MaxZ;
		pMesh = pMesh->pNext;
	}
}


void ObjectLL_AllMeshs_BackupRenderFlags(struct ObjectLL* pObject)
{
	struct MeshLL* pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
	 	pMesh->RestoreFlags = pMesh->RenderFlags;
	 	pMesh = pMesh->pNext;
	}
}
void ObjectLL_AllMeshs_RestoreRenderFlags(struct ObjectLL* pObject)
{
	struct MeshLL* pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
	 	pMesh->RenderFlags = pMesh->RestoreFlags;
	 	pMesh = pMesh->pNext;
	}
}






//Works on restpose Keyframe
void Object_InitBoneWeights(struct ObjectLL* pObject)
{
	struct SkeletonLL* pSkeleton = NULL;
	struct JointLL* pJoint = NULL;
	struct JointLL* pJoint2 = NULL;
	struct JointLL* pRestPose_Joint = NULL;
	struct JointLL* pRestPose_Joint2 = NULL;
	struct JointLL* pChild = NULL;
	struct MeshLL* pMesh = NULL;
	struct KeyFrameLL* pPrev_KeyFrame  = NULL;
	struct KeyFrameLL* pRestPose_KeyFrame  = NULL;
	struct InitWeightsItem* pInitWeightsItem = NULL;
	long i;
	VERTEX_XYZNRGBUV* pVert = NULL;
	PERVERTEX_JOINT_DATA* pPVBData = NULL;
	aVECTOR Point; 
	float Distance;
	VERTEX_XYZNRGBUV* pLocalVert = NULL;
	VERTEX_XYZNRGBUV* pWorldVert = NULL;

	struct BoneItem* pBoneItem  = NULL;//extra pointer
	struct BoneItem* pBoneItem2 = NULL;//extra pointer
	struct BoneItem* pBoneItems = NULL;//calloc
	unsigned long    cBoneItems = 0;

	float Primary_Distance,Secondary_Distance;
	unsigned long Primary_UniqueID,Secondary_UniqueID;
	long  Primary_Index;
	float w1,w2;

//	long cAB;
	long cBI;

	if(pObject==NULL || pObject->pSkeletonHead==NULL)return;

	pBoneItems = (struct BoneItem*)calloc(MAX_JOINTS,sizeof(struct BoneItem));

	pSkeleton = pObject->pSkeletonHead;
	while(pSkeleton)
	{
		pRestPose_KeyFrame = pSkeleton->pRestPose;

		//////////////////////////////////
		//// Make A List Of All Bones ////
		//////////////////////////////////
		pJoint = pRestPose_KeyFrame->pJointHead;
		while(pJoint)
		{
			if(strcmp(pJoint->pInfoItem->pParentName,"null")==0)
			{//no bone between first XYZ origin, and first Joint.
			}
			else
			{// Generate Bone with Parent Joint as Focus
				pBoneItem = &pBoneItems[cBoneItems];
				cBoneItems++;

				pBoneItem->pJoint = pJoint->pParent;

				pBoneItem->Pos.x  = pJoint->pParent->pAbsoluteMatrix[12];
				pBoneItem->Pos.y  = pJoint->pParent->pAbsoluteMatrix[13];
				pBoneItem->Pos.z  = pJoint->pParent->pAbsoluteMatrix[14];

				pBoneItem->Dir.x  = pJoint->pAbsoluteMatrix[12] - pBoneItem->Pos.x;
				pBoneItem->Dir.y  = pJoint->pAbsoluteMatrix[13] - pBoneItem->Pos.y;
				pBoneItem->Dir.z  = pJoint->pAbsoluteMatrix[14] - pBoneItem->Pos.z;
			}
			pJoint = pJoint->pNext;
		}

		/////////////////////////////////////////////////////////////////////////////
		//// Append To List the end point Joints. Last joint is a spherical bone ////
		//// 	It cant be a real bone because we dont know where the endpoint
		////	would be.
		////	A Bone is a Joint(@), and a Bone(=)
		////	@=
		////	@=@= usually there is another joint attached.
		////	@=@=@= The last joint was missed by previous pass because they have no children
		////	@=@=@=@ The next pass looks fo joints without children. 
		////	The last bone is spherical.(pos, but no dir)
		/////////////////////////////////////////////////////////////////////////////
		pJoint = pRestPose_KeyFrame->pJointHead;
		while(pJoint)
		{
			//// See If This Joint Has children
			pJoint2 = pRestPose_KeyFrame->pJointHead;
			while(pJoint2)
			{
				if(strcmp(pJoint2->pInfoItem->pParentName,pJoint->pInfoItem->pName)==0)
				{
					//found a child, therefore a bone has already been made
					//jump over code to continue checking pJoint LL.
					goto LABEL_Found_Children;
				}
				pJoint2 = pJoint2->pNext;
			}
			//you get here if no children
			pBoneItem = &pBoneItems[cBoneItems];
			cBoneItems++;

			pBoneItem->pJoint = pJoint;

			pBoneItem->Pos.x  = pJoint->pAbsoluteMatrix[12];
			pBoneItem->Pos.y  = pJoint->pAbsoluteMatrix[13];
			pBoneItem->Pos.z  = pJoint->pAbsoluteMatrix[14];

			pBoneItem->Dir.x  = 0.0;
			pBoneItem->Dir.y  = 0.0;
			pBoneItem->Dir.z  = 0.0;

			LABEL_Found_Children:;//goto target

			pJoint = pJoint->pNext;
		}
		pSkeleton = pSkeleton->pNext;
	}											
	//////////////////////
	//////////////////////
	//////////////////////
	////              ////
	//// INIT WEIGHTS ////
	////              ////
	//// INIT WEIGHTS ////
	////              ////
	//////////////////////
	//////////////////////
	//////////////////////


	pMesh = pObject->pMeshHead;
	while(pMesh)	 
	{
		//SET THIS FOR THE USER
		SetBit(pMesh->RenderFlags,RENDER_TRANSFORMED);

		pPVBData   = pMesh->pPerVertexJointData;
		pLocalVert = pMesh->pLocalVerts;
		pWorldVert = pMesh->pWorldVerts;
		////////////////////////////////////
		for(i=0;i<pMesh->num_verts;i++)
		{
			//// Find Closest Bone.
			//// A Bone is a Joint, and a offset to another joint.

			Point.x = pLocalVert[i].x;//RestPose, and LocalVerts Are Aligned.
			Point.y = pLocalVert[i].y;
			Point.z = pLocalVert[i].z;
					
			/////////////////////////////////////////////////////////
			//// Find Primary Bone That Influences Current Point ////
			/////////////////////////////////////////////////////////

			Primary_Distance = FLT_MAX;
			Primary_Index    = -1;
			for(cBI=0;cBI<cBoneItems;cBI++)
			{
				pBoneItem = &pBoneItems[cBI];
				Distance = Min_Distance_PointToBone(Point,pBoneItem->Pos,pBoneItem->Dir);
				if(Distance<Primary_Distance)
				{
					Primary_Distance = Distance;
					Primary_Index    = cBI;
				  	Primary_UniqueID = pBoneItem->pJoint->UniqueID;
				}
			}
 
			///////////////////////////////////////////////////////////
			//// Find Secondary Bone That Influences Current Point ////
			///////////////////////////////////////////////////////////

			Secondary_Distance = FLT_MAX;
			for(cBI=0;cBI<cBoneItems;cBI++)
			{
				if(cBI != Primary_Index)
				{//not primary
					pBoneItem = &pBoneItems[cBI];
					//Distance = Tri_Distance_PointToBone(Point,pBoneItem->Pos,pBoneItem->Dir);
					Distance = Min_Distance_PointToBone(Point,pBoneItem->Pos,pBoneItem->Dir);
					if(Distance<Secondary_Distance)
					{	//Found a bone that is closer than the others...
						//Test To See If this bone is attached to primary
						//If any of the 3 following conditions are true, accept secondary
						//1)Is This the Primarys Parent?
						//2)Does This have same parent as Primary?
						//3)Does This have Primary is parent?
												
						pBoneItem2 = &pBoneItems[Primary_Index];


						//(new)if primary is endjoint, abort, EJ get 100% dont accept secondary.
						if(pBoneItem->Dir.x+pBoneItem->Dir.y+pBoneItem->Dir.z  != 0.0)
						{
							//(1)Is This the Primarys Parent?
							if(	(strcmp(pBoneItem->pJoint->pInfoItem->pName,
										pBoneItem2->pJoint->pInfoItem->pParentName)==0) ||
								//(2)Does This have same parent as Primary?
								//(new)if pBItem2 is endjoint, abort. EJ get 100%, so no secondary 
								((strcmp(pBoneItem->pJoint->pInfoItem->pParentName,
										pBoneItem2->pJoint->pInfoItem->pParentName)==0) && 
										(pBoneItem2->Dir.x+pBoneItem2->Dir.y+pBoneItem2->Dir.z  != 0.0))	||
								//(3)Does This have Primary is parent?
								(strcmp(pBoneItem->pJoint->pInfoItem->pParentName,
										pBoneItem2->pJoint->pInfoItem->pName)==0))
							{
								//store min distance. tridistance selects secondary bone.
								Secondary_Distance = Min_Distance_PointToBone(Point,pBoneItem->Pos,pBoneItem->Dir);
							  	Secondary_UniqueID = pBoneItem->pJoint->UniqueID;
							}
						}
					}
				}
			}

			if(Primary_Index != -1 && Secondary_Distance < FLT_MAX)
			{
				assert(Primary_Distance<=Secondary_Distance);
				if((Primary_Distance*2.0)>Secondary_Distance)
				{//then close enough to blend
					
					w2 = ((Primary_Distance*2.0)-Secondary_Distance)/(Primary_Distance*2.0);
					w1 = 1.0-w2;

					pPVBData[i].weight[0] = w1;
					pPVBData[i].weight[1] = w2;
					pPVBData[i].weight[2] = 0.0;
					pPVBData[i].weight[3] = 0.0;

					pPVBData[i].index[0] =   Primary_UniqueID;
					pPVBData[i].index[1] = Secondary_UniqueID;
					pPVBData[i].index[2] = -1;
					pPVBData[i].index[3] = -1;
				}
				else
				{//secondary bone is too far. Only use Primary Bone
					pPVBData[i].weight[0] = 1.0;
					pPVBData[i].weight[1] = 0.0;
					pPVBData[i].weight[2] = 0.0;
					pPVBData[i].weight[3] = 0.0;

					pPVBData[i].index[0]  = Primary_UniqueID;
					pPVBData[i].index[1]  = -1;
					pPVBData[i].index[2]  = -1;
					pPVBData[i].index[3]  = -1;
				}
			}
			else if(Primary_Index != -1 && Secondary_Distance == FLT_MAX)
			{//only one joint
				pPVBData[i].weight[0] = 1.0;
				pPVBData[i].weight[1] = 0.0;
				pPVBData[i].weight[2] = 0.0;
				pPVBData[i].weight[3] = 0.0;

				pPVBData[i].index[0]  = Primary_UniqueID;
				pPVBData[i].index[1]  = -1;
				pPVBData[i].index[2]  = -1;
				pPVBData[i].index[3]  = -1;
			}

			else
			{
				assert(0);//how?
			}
		}//End for i<pMesh->num_verts
	 	pMesh = pMesh->pNext;
	}
	SAFE_FREE(pBoneItems);
}


void Object_FlagVerticesAsVisible(	struct ObjectLL* pObject)
{
	struct MeshLL* pMesh = NULL;

	pMesh = (pObject) ? (pObject->pMeshHead) : (NULL);
	while(pMesh)	 
	{
		Mesh_FlagVerticesAsVisible(pObject->pMatrix,pMesh);
	 	pMesh = pMesh->pNext;
	}
}
void Object_FlagVerticesAsSelected(	struct ObjectLL* pObject,
									POINT ptRectStart,
									POINT ptRectStop)
{
	struct MeshLL* pMesh = NULL;

	pMesh = (pObject) ? (pObject->pMeshHead) : (NULL);
	while(pMesh)	 
	{
		Mesh_FlagVerticesAsSelected(pObject->pMatrix,pMesh,ptRectStart,ptRectStop);
	 	pMesh = pMesh->pNext;
	}
}




//CALCULATEFINALMESHMATRIX
void Object_Mouse_Move_SelectedVertices(struct ObjectLL* pObject,POINT MouseDiff)
{
	float pInverseView[16];
//	float pModelFinal[16];
//	float pInverseFinal[16];
//	float pInverseModel[16];
	struct MeshLL* pMesh = NULL;

	//Inverse view matrix to get world matrix defining camera
	InverseMatrix(pInverseView,pRender_ViewMatrix);

	pMesh = (pObject) ? (pObject->pMeshHead) : (NULL);
	while(pMesh)	 
	{
/*
		//concat obj, and mesh matrices. pModelFinal defines local to world
		//MatrixMul(pModelFinal,pObject->pMatrix,pMesh->pMatrix);
		MatrixMul(pModelFinal,pMesh->pMatrix,pObject->pMatrix);

		//pInverseModel defines world to local
		InverseMatrix(pInverseModel,pModelFinal);
		
		//pInverseFinal is the camera in local coordinates. The relationship between camera, and mesh are the same.
		//The camera has been inverse transformed into local space so that local vertices can be processed, avoiding LtoW transform for each vertex.
		//The resulting local coord camera will be different for each mesh given their obj, and msh matrices.
		MatrixMul(pInverseFinal,pInverseView,pInverseModel);
*/
		Mesh_Mouse_Move_SelectedVertices(pObject->pMatrix,pMesh,MouseDiff);

	 	pMesh = pMesh->pNext;
	}
}








///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
///////////////////
