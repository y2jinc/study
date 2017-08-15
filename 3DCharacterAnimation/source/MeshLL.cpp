// MeshLL.cpp written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to

#include "stdafx.h"
#include "resource.h"
#include "Library.h"
#include "FileAccess.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

extern struct MeshLL* pCurrentlySelectedMesh;
extern float pRender_ViewMatrix[];
extern float pRender_ProjMatrix[];
extern float pRender_IdentityMatrix[];
extern float Render_WindowWidth;
extern float Render_WindowHeight;
extern char* gp_TextBuffer1;
extern char* gp_TextBuffer2;
extern char* gp_TextBuffer3;

extern void AppendDebugMsg(char *pChar);

void MeshLL_CalculateVertexNormals(struct MeshLL* pMesh);

unsigned long MeshLL_Count(struct MeshLL* pMeshHeadIn)
{
	struct MeshLL* pThis = pMeshHeadIn;
	unsigned long num_meshes = 0;

	while(pThis)
	{
		num_meshes++;
		pThis = pThis->pNext;
	}
	return num_meshes;
}


//pMesh->RenderFlags & RENDER_SOLID
struct MeshLL* MeshLL_Insert(unsigned long SortKey,
							 char* pName,
							 struct MeshLL*& pMeshHeadIn)

{
	struct MeshLL* pMeshHead = pMeshHeadIn;
	struct MeshLL* pInsertMe = NULL;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pNext = NULL;

	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe = (struct MeshLL *)calloc(1,sizeof(struct MeshLL));
	pInsertMe->pNext	  = NULL;
	pInsertMe->SortKey = SortKey;
	pInsertMe->RenderFlags = RENDER_VISIBLE|RENDER_SELECTABLE;
//	pInsertMe->Scale = 1.0;
	pInsertMe->SrcBlend = BLEND_SRCCOLOR;
	pInsertMe->DstBlend = BLEND_INVSRCCOLOR;

	FillIdentity(pInsertMe->pMatrix);
	sprintf(pInsertMe->pName,"%s",pName);

	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pMeshHead == NULL)
	{
		pMeshHeadIn = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pMeshHead->SortKey)
	{	
		pInsertMe->pNext  = pMeshHead;
		pMeshHeadIn      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pMeshHead;
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

struct MeshLL* MeshLL_ReInsert(	struct MeshLL* 	pMesh,
								struct MeshLL*& pMeshHeadIn)
{
	struct MeshLL* pMeshHead = pMeshHeadIn;
	struct MeshLL* pInsertMe = pMesh;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pNext = NULL;

	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe->pNext	  = NULL;

	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pMeshHead == NULL)
	{
		pMeshHeadIn = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pMeshHead->SortKey)
	{	
		pInsertMe->pNext  = pMeshHead;
		pMeshHeadIn      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pMeshHead;
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
void MeshLL_Remove(char* pName,
					struct MeshLL*& pMeshHeadIn)

{
	struct MeshLL* pMeshHead = pMeshHeadIn;
	struct MeshLL* pRemoveMe  = NULL;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pNext = NULL;
	struct MeshLL* pLast = NULL;

	pThis = pMeshHead;
	pLast = pMeshHead;
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

			if(pRemoveMe == pMeshHead)
			{//We are removing the head
				if(pNext==NULL)
				{//we are removing the last
					pMeshHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pMeshHeadIn = pNext;
				}
			}
			else
			{//We are removing a normal item
				pLast->pNext = pNext;
			}

			//MainMenu MainMenu
			if(pCurrentlySelectedMesh==pRemoveMe)
			{//if we delete current mesh, clear all pointers to the data
				pCurrentlySelectedMesh = NULL;
			}

			SAFE_FREE(pRemoveMe->pLocalVerts);
			SAFE_FREE(pRemoveMe->pWorldVerts);
			SAFE_FREE(pRemoveMe->pPerVertexJointData);
			SAFE_FREE(pRemoveMe->pVertexFlags);
			SAFE_FREE(pRemoveMe->pSelectedVerts);
			SAFE_FREE(pRemoveMe->pIndices);
			ReleaseTexture(&pRemoveMe->Texture);
			ReleaseTexture(&pRemoveMe->MultiTexture);

//TODO:
//REMOVE : struct PrimitiveLL* pPrimitiveHead;//LL of primitives to be CSG'd into a single "skin mesh"
		

			free(pRemoveMe);
		}
		else
		{//check next item
			pLast = pThis;
			pThis = pThis->pNext;
		}
	}
}

void MeshLL_Remove(	struct MeshLL*& pRemoveMe,
					struct MeshLL*& pMeshHeadIn)
{
	struct MeshLL* pMeshHead = pMeshHeadIn;
//	struct MeshLL* pRemoveMe  = NULL;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pNext = NULL;
	struct MeshLL* pLast = NULL;

	pThis = pMeshHead;
	pLast = pMeshHead;
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

			if(pThis == pMeshHead)
			{//We are removing the head
				if(pThis->pNext==NULL)
				{//we are removing the last
					pMeshHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pMeshHeadIn = pThis->pNext;
				}
			}
			else
			{//We are removing a normal item
				pLast->pNext = pThis->pNext;
			}





			//MainMenu MainMenu
			if(pThis==pCurrentlySelectedMesh)
			{//if we delete current mesh, clear all pointers to the data
				pCurrentlySelectedMesh = NULL;
			}

			SAFE_FREE(pThis->pLocalVerts);
			SAFE_FREE(pThis->pWorldVerts);
			SAFE_FREE(pThis->pPerVertexJointData);
			SAFE_FREE(pThis->pVertexFlags);
			SAFE_FREE(pThis->pSelectedVerts);
			SAFE_FREE(pThis->pIndices);
			ReleaseTexture(&pThis->Texture);
			ReleaseTexture(&pThis->MultiTexture);

			free(pThis);
			return;
		}
		else
		{//check next item
			pLast = pThis;
			pThis = pThis->pNext;
		}
	}
	assert(0);//msh not found
}

void MeshLL_RemoveALL(struct MeshLL*& pMeshHeadIn)
{
	struct MeshLL* pRemoveMe  = NULL;
	struct MeshLL* pThis = NULL;

	pThis = pMeshHeadIn;
	while(pThis!=NULL)
	{
		pRemoveMe = pThis;
		pThis = pThis->pNext;

		//MainMenu MainMenu
		if(pCurrentlySelectedMesh==pRemoveMe)
		{//if we delete current mesh, clear all pointers to the data
			pCurrentlySelectedMesh = NULL;
		}

		SAFE_FREE(pRemoveMe->pLocalVerts);
		SAFE_FREE(pRemoveMe->pWorldVerts);
		SAFE_FREE(pRemoveMe->pPerVertexJointData);
		SAFE_FREE(pRemoveMe->pVertexFlags);
		SAFE_FREE(pRemoveMe->pSelectedVerts);
		SAFE_FREE(pRemoveMe->pIndices);
		ReleaseTexture(&pRemoveMe->Texture);
		ReleaseTexture(&pRemoveMe->MultiTexture);

		free(pRemoveMe);
	}
	pMeshHeadIn = NULL;
}

















bool MeshLL_Get(	struct MeshLL*  pMeshHeadIn,
					struct MeshLL*& pPrevIn,
					struct MeshLL*& pThisIn,
					struct MeshLL*& pNextIn,
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
	pThisIn = pMeshHeadIn;
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

void MeshLL_GetFirst(	struct MeshLL*  pMeshHeadIn,
						struct MeshLL*& pPrev,
						struct MeshLL*& pThis,
						struct MeshLL*& pNext)
{
	pPrev = NULL;
	pThis = pMeshHeadIn;
	pNext = (pMeshHeadIn) ? (pMeshHeadIn->pNext) : (NULL);
}

void MeshLL_GetLast(	struct MeshLL*  pMeshHeadIn,
						struct MeshLL*& pPrev,
						struct MeshLL*& pThis,
						struct MeshLL*& pNext)
{
	pPrev = NULL;
	pThis = NULL;
	pNext = pMeshHeadIn;

	while(pNext)
	{
		pPrev = pThis;
		pThis = pNext;
		pNext = pThis->pNext;
	}
}


void MeshLL_Sort(struct MeshLL*& pMeshHeadIn)
{
	struct MeshLL* pNewMeshHead = NULL;
	struct MeshLL* pThis = NULL;
	struct MeshLL* pReInsertMe = NULL;
	struct MeshLL* pSaveMesh = NULL;

	pThis       = pMeshHeadIn;
	pMeshHeadIn = NULL;
	while(pThis!=NULL)
	{
		pReInsertMe = pThis;
		pSaveMesh = pThis->pNext;
		MeshLL_ReInsert(pReInsertMe,pMeshHeadIn);
		pThis = pSaveMesh;
	}
}



//This is used to create Meshes for the rending engine
// These Meshs will not be in the scene hiearchy
// Used for font, and mouse pointer
struct MeshLL* MeshLL_CreateNode(	char* pName,
									unsigned long max_w_verts,
									long max_w_indices,
//									unsigned long max_s_verts,
//									long max_s_indices,
									unsigned long RenderFlags)
{
	WORD* pI = NULL;
	struct MeshLL* pInsertMe = NULL;
	////////////////////////////////////////////
	//// Initialize Single Linked List Node ////
	////////////////////////////////////////////

	pInsertMe = (struct MeshLL *)calloc(1,sizeof(struct MeshLL));
	pInsertMe->RenderFlags = RenderFlags;
//	pInsertMe->Scale = 1.0;
	pInsertMe->SortKey = 1;

	pInsertMe->pLocalVerts    = (VERTEX_XYZNRGBUV   *)calloc(max_w_verts,sizeof(VERTEX_XYZNRGBUV));
	pInsertMe->pWorldVerts    = (VERTEX_XYZNRGBUV   *)calloc(max_w_verts,sizeof(VERTEX_XYZNRGBUV));
	pInsertMe->pSelectedVerts 		= (unsigned char*		)calloc(MAX_VERTICES,sizeof(unsigned char));
	pInsertMe->pVertexFlags 		= (unsigned char*		)calloc(MAX_VERTICES,sizeof(unsigned char));
	pInsertMe->pIndices  = (WORD    *)calloc(max_w_indices ,sizeof(WORD));
	pInsertMe->pPerVertexJointData = (PERVERTEX_JOINT_DATA *)calloc(MAX_VERTICES,sizeof(PERVERTEX_JOINT_DATA));

	//DEBUG INIT
	//DEBUG INIT
	//DEBUG INIT
	int ii;
	PERVERTEX_JOINT_DATA *pPVBD = NULL;
	pPVBD = pInsertMe->pPerVertexJointData;
	for(ii=0;ii<MAX_VERTICES;ii++)
	{
		pPVBD[ii].weight[0] = 0.25;
		pPVBD[ii].weight[1] = 0.25;
		pPVBD[ii].weight[2] = 0.25;
	}	
	//DEBUG INIT
	//DEBUG INIT
	//DEBUG INIT




//	pInsertMe->pScreenIndices = (WORD    *)calloc(max_s_indices ,sizeof(WORD));
//	pInsertMe->pPerVertexJointData = (PERVERTEX_JOINT_DATA *)calloc(MAX_VERTICES,sizeof(PERVERTEX_JOINT_DATA));


	//assume quads. used by font.
	pI = pInsertMe->pIndices;
	for(int i=0;i<max_w_indices/6;i++)
	{//init indices list. each char is a quad 4 verts, 6 indices
		//triangles are described in counter clock wise order
		pI[i*6+0] = i*4+0;
		pI[i*6+1] = i*4+2;
		pI[i*6+2] = i*4+1;

		pI[i*6+3] = i*4+1;
		pI[i*6+4] = i*4+2;
		pI[i*6+5] = i*4+3;
	}

//	AxisRotationMatrix(pInsertMe->pMatrix,0.0,0.0,-1.0,0.0);
//	PositionMatrix(pInsertMe->pMatrix,0,0,0);

	FillIdentity(pInsertMe->pMatrix);

	sprintf(pInsertMe->pName,"%s",pName);
	return pInsertMe;
}




//todo:make more like : void SkeletonLL_Open(HANDLE fH,struct ObjectLL* pObject)
//   which calls insert internally
bool MeshLL_Load(struct MeshLL* pMesh,char *pFileName)
{
	HANDLE fH;
	int returnval;
	unsigned char *pSrc = NULL;
	long length;
	//	char FullName[512];

//	sprintf(FullName,"%s.msh",pFileName);
//	AddPathToFileName(FullName);

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
		ReadFromFile(fH,(char*) pMesh->pName,256);
		ReadFromFile(fH,(char*)&pMesh->SortKey,sizeof(unsigned long));
		ReadFromFile(fH,(char*) pMesh->pMatrix,16*sizeof(float));
		ReadFromFile(fH,(char*)&pMesh->num_verts,sizeof(unsigned long));
		ReadFromFile(fH,(char*)&pMesh->num_tris,sizeof(unsigned long));
		ReadFromFile(fH,(char*)&pMesh->RenderFlags,sizeof(unsigned long));

		
		ReadFromFile(fH,(char*)pMesh->Texture.pName,256*sizeof(char));
		ReadFromFile(fH,(char*)pMesh->MultiTexture.pName,256*sizeof(char));

		ReadFromFile(fH,(char*)&pMesh->SrcBlend,sizeof(unsigned long));
		ReadFromFile(fH,(char*)&pMesh->DstBlend,sizeof(unsigned long));

		assert(pMesh->pWorldVerts==NULL);
		assert(pMesh->pLocalVerts==NULL);
		assert(pMesh->pPerVertexJointData==NULL);
		assert(pMesh->pIndices==NULL);
		assert(pMesh->pSelectedVerts==NULL);
		assert(pMesh->pVertexFlags==NULL);

		long MV = MAX_VERTICES;
		long MI = MAX_INDICES;


		SAFE_FREE(pMesh->pLocalVerts);
		SAFE_FREE(pMesh->pWorldVerts);
		SAFE_FREE(pMesh->pPerVertexJointData);
		SAFE_FREE(pMesh->pIndices);
		SAFE_FREE(pMesh->pSelectedVerts);
		SAFE_FREE(pMesh->pVertexFlags);
		ReleaseTexture(&pMesh->Texture);
		ReleaseTexture(&pMesh->MultiTexture);

		pMesh->pLocalVerts    = (VERTEX_XYZNRGBUV   *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
		pMesh->pWorldVerts    = (VERTEX_XYZNRGBUV   *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));//this is needed for FlagVerticesAsVisible effect. also needed for obj flagged as TRANSFORMED
		pMesh->pPerVertexJointData = (PERVERTEX_JOINT_DATA *)calloc(MAX_VERTICES,sizeof(PERVERTEX_JOINT_DATA));
		pMesh->pSelectedVerts 		= (unsigned char*		)calloc(MAX_VERTICES,sizeof(unsigned char));
		pMesh->pVertexFlags 		= (unsigned char*		)calloc(MAX_VERTICES,sizeof(unsigned char));
		pMesh->pIndices  = (WORD            *)calloc(MAX_INDICES ,sizeof(WORD));

		ReadFromFile(fH,(char*) pMesh->pLocalVerts,pMesh->num_verts*sizeof(VERTEX_XYZNRGBUV));
		ReadFromFile(fH,(char*) pMesh->pPerVertexJointData,pMesh->num_verts*sizeof(PERVERTEX_JOINT_DATA));

		ReadFromFile(fH,(char*) pMesh->pIndices,pMesh->num_tris*3*sizeof(WORD));


		returnval = CloseHandle(fH);

//		LoadTexture(NULL,pMesh->Texture.pName,pMesh->Texture,TEXTURE_FLAG_4444);///4444??? must get this info from mesh somehow

		sprintf(gp_TextBuffer2,"%s\\%s",gp_TextBuffer1,pMesh->Texture.pName);

		LoadTexture(NULL,gp_TextBuffer2,pMesh->Texture,TEXTURE_FLAG_4444);///4444??? must get this info from mesh somehow
		return true;
	}//End Open File
	return false;
}//End 

bool MeshLL_Save(struct MeshLL* pMesh,char *pFileName)
{
	HANDLE fH;
	int returnval;
	BOOL bResult = FALSE;
	unsigned char *pSrc = NULL;
//	char FullName[512];
	char pTextureName[256];

//	sprintf(FullName,"%s.msh",pFileName);
//	AddPathToFileName(FullName);

//	long length = strlen(pFileName);

//	if(length==0 || length>=MAX_PATHNAME)return false;

	fH = CreateFile(pFileName,GENERIC_WRITE,
   	    0, (LPSECURITY_ATTRIBUTES) NULL,
   	    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
   	    (HANDLE) NULL);
	if(fH != INVALID_HANDLE_VALUE)
	{
		WriteToFile(fH,(char*) pMesh->pName,256);
		WriteToFile(fH,(char*)&pMesh->SortKey,sizeof(unsigned long));
		WriteToFile(fH,(char*) pMesh->pMatrix,16*sizeof(float));
		WriteToFile(fH,(char*)&pMesh->num_verts,sizeof(unsigned long));
		WriteToFile(fH,(char*)&pMesh->num_tris,sizeof(unsigned long));
		WriteToFile(fH,(char*)&pMesh->RenderFlags,sizeof(unsigned long));

		memcpy(pTextureName,pMesh->Texture.pName,256*sizeof(char));
		RemovePathFromFileName(pTextureName);
		WriteToFile(fH,(char*)&pTextureName,256*sizeof(char));

		memcpy(pTextureName,pMesh->MultiTexture.pName,256*sizeof(char));
		RemovePathFromFileName(pTextureName);
		WriteToFile(fH,(char*)&pTextureName,256*sizeof(char));

		WriteToFile(fH,(char*)&pMesh->SrcBlend,sizeof(unsigned long));
		WriteToFile(fH,(char*)&pMesh->DstBlend,sizeof(unsigned long));

		WriteToFile(fH,(char*) pMesh->pLocalVerts,pMesh->num_verts*sizeof(VERTEX_XYZNRGBUV));
		WriteToFile(fH,(char*) pMesh->pPerVertexJointData,pMesh->num_verts*sizeof(PERVERTEX_JOINT_DATA));

		WriteToFile(fH,(char*) pMesh->pIndices,pMesh->num_tris*3*sizeof(WORD));

		returnval = CloseHandle(fH);

		return true;
	}//End Open File
	return false;
}//End 



//some objct want both 3d, and 2d arrays, but previous load functions only allocated one or the other.
//This function will allocate memory for the other.
void MeshLL_Create2dAnd3DVertexArrays(struct MeshLL* pMesh)
{
	assert(pMesh!=NULL);
	if(pMesh->pLocalVerts   ==NULL)pMesh->pLocalVerts    = (VERTEX_XYZNRGBUV   *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
	if(pMesh->pWorldVerts   ==NULL)pMesh->pWorldVerts    = (VERTEX_XYZNRGBUV   *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
	if(pMesh->pIndices ==NULL)pMesh->pIndices  = (WORD            *)calloc(MAX_INDICES ,sizeof(WORD));
}



void MeshLL_SetAllVertices_Alpha(struct MeshLL* pMesh,float alpha)
{
	long i,iAlpha;
	VERTEX_XYZNRGBUV* pVert = NULL;

	assert(pMesh->pWorldVerts!=NULL);

	pVert = pMesh->pWorldVerts;

	iAlpha = (int)(alpha*255.0);
	iAlpha <<= 24;

	for(i=0;i<pMesh->num_verts;i++)
	{
		pVert[i].diffuse = iAlpha | (pVert[i].diffuse & 0x00ffffff);
	}
}

void MeshLL_SetInVRAM(struct MeshLL* pMesh,bool bInVRAM)
{
	pMesh->Texture.InVRAM      = bInVRAM;
	pMesh->MultiTexture.InVRAM = bInVRAM;
}

void MeshLL_GetMinMaxXYZ(	struct MeshLL* pMesh,
							float& MeshMinX,
							float& MeshMinY,
							float& MeshMinZ,
							float& MeshMaxX,
							float& MeshMaxY,
							float& MeshMaxZ)
{
	long i;
	VERTEX_XYZNRGBUV* pVert = NULL;

	if(pMesh == NULL ||pMesh->pLocalVerts == NULL)
	{
		MeshMinX = 0.0;
		MeshMinY = 0.0;
		MeshMinZ = 0.0;

		MeshMaxX = 1.0;
		MeshMaxY = 1.0;
		MeshMaxZ = 1.0;
		return;
	}

	pVert = pMesh->pLocalVerts;

	if(pMesh->num_verts>0)
	{
		//init MeshMin Max with first vert.
		MeshMinX = pVert[0].x;
		MeshMinY = pVert[0].y;
		MeshMinZ = pVert[0].z;
		MeshMaxX = pVert[0].x;
		MeshMaxY = pVert[0].y;
		MeshMaxZ = pVert[0].z;
		//start with next item
		for(i=1;i<pMesh->num_verts;i++)
		{
			if(pVert[i].x<MeshMinX)MeshMinX=pVert[i].x;
			if(pVert[i].x>MeshMaxX)MeshMaxX=pVert[i].x;

			if(pVert[i].y<MeshMinY)MeshMinY=pVert[i].y;
			if(pVert[i].y>MeshMaxY)MeshMaxY=pVert[i].y;

			if(pVert[i].z<MeshMinZ)MeshMinZ=pVert[i].z;
			if(pVert[i].z>MeshMaxZ)MeshMaxZ=pVert[i].z;
		}
	}
}


void MeshLL_Change_Winding(struct MeshLL* pMesh)
{
	long i,V0,V1,V2;
	VERTEX_XYZNRGBUV* pV = NULL;
	WORD* pI = NULL;

	assert(pMesh->pWorldVerts   != NULL);
	assert(pMesh->pIndices != NULL);

	pV = pMesh->pWorldVerts;
	pI = pMesh->pIndices;

	for(i=0;i<pMesh->num_tris;i++)
	{
		V0 = pI[(i*3+0)];
		V1 = pI[(i*3+1)];
		V2 = pI[(i*3+2)];
		pI[(i*3+0)] = V0;
		pI[(i*3+1)] = V2;
		pI[(i*3+2)] = V1;
	}
}















//	AxisRotationMatrix(pInsertMe->pMatrix,0.0,0.0,-1.0,0.0);


//makes a 3d object flat so that it can be rendered as 2d screen space verts
//converts XYZNUV data into XYZRGBUV data. white color assumd.
/*
void MeshLL_Orthographic3dTo2dRender(struct MeshLL* pMesh)
{
	long i;
	long Diffuse = 0xffffffff;
	long Specular = 0x00000000;
	VERTEX_XYZNUV* pWorldVerts = NULL;
	VERTEX_XYZRGBUV* pScreenVerts = NULL;
	WORD* pIndices = NULL;
	WORD* pScreenIndices = NULL;
	float *pLtoW = pMesh->pMatrix;
	aVECTOR Vert;
	assert(pMesh->pWorldVerts   !=NULL);
	assert(pMesh->pScreenVerts  !=NULL);
	assert(pMesh->pIndices !=NULL);
	assert(pMesh->pScreenIndices!=NULL);

	pWorldVerts    = pMesh->pWorldVerts;
	pScreenVerts   = pMesh->pScreenVerts;
	pIndices  = pMesh->pIndices;
	pScreenIndices = pMesh->pScreenIndices;

	memcpy(pScreenIndices,pIndices,pMesh->num_tris*3*sizeof(WORD));
	for(i=0;i<pMesh->num_verts;i++)
	{
//		Vert.x  = (pLtoW->_11*pWorldVerts[i].x)  + (pLtoW->_21*pWorldVerts[i].y)  + (pLtoW->_31*pWorldVerts[i].z)  + pLtoW->_41;
//		Vert.y  = (pLtoW->_12*pWorldVerts[i].x)  + (pLtoW->_22*pWorldVerts[i].y)  + (pLtoW->_32*pWorldVerts[i].z)  + pLtoW->_42;
//		Vert.z  = (pLtoW->_13*pWorldVerts[i].x)  + (pLtoW->_23*pWorldVerts[i].y)  + (pLtoW->_33*pWorldVerts[i].z)  + pLtoW->_43;

		Vert.x  = (pLtoW[0]*pWorldVerts[i].x)  + (pLtoW[4]*pWorldVerts[i].y)  + (pLtoW[ 8]*pWorldVerts[i].z)  + pLtoW[12];
		Vert.y  = (pLtoW[1]*pWorldVerts[i].x)  + (pLtoW[5]*pWorldVerts[i].y)  + (pLtoW[ 9]*pWorldVerts[i].z)  + pLtoW[13];
		Vert.z  = (pLtoW[2]*pWorldVerts[i].x)  + (pLtoW[6]*pWorldVerts[i].y)  + (pLtoW[10]*pWorldVerts[i].z)  + pLtoW[14];

		pScreenVerts[i].x = (Vert.x+2.0)*10.0;
		pScreenVerts[i].y = (Vert.y+2.0)*10.0;
		pScreenVerts[i].z = 1.0;
		pScreenVerts[i].rhw = 1.0;
		pScreenVerts[i].diffuse = Diffuse;
		pScreenVerts[i].specular = Specular;
		pScreenVerts[i].tu = pWorldVerts[i].tu;
		pScreenVerts[i].tv = pWorldVerts[i].tv;
	}
	pMesh->num_s_verts = pMesh->num_verts;
	pMesh->num_s_tris  = pMesh->num_tris; 
}

VERTEX_XYZNUV pXFormedVerts[MAX_VERTICES];
void MeshLL_Orthographic3dTo2dFlatShadedRender(struct MeshLL* pMesh)
{
	long DiffuseIntensity;
	long SpecularIntensity;

	static long  CurrentTime;
	long  LastTime;
	float TimeRatio;

	float *pLtoW = pMesh->pMatrix;
	float Length,Ax,Ay,Az,Bx,By,Bz,Rx,Ry,Rz;

    aVECTOR Light;
    aVECTOR Light2;
    aVECTOR ToLight;
	aVECTOR ToViewer;
	aVECTOR Reflected;
	aVECTOR Normal;
	aVECTOR CenterOfGravity;
	aVECTOR vEyePt;
	float fPhong;
	float fPower = 5.0;
	float CameraD,PlaneD;
	unsigned int V[3];
	unsigned int cFace;
	int i;
	float NdotL;
	VERTEX_XYZNUV* pWorldVerts = NULL;
	VERTEX_XYZRGBUV* pScreenVerts = NULL;
	WORD* pIndices = NULL;
	WORD* pScreenIndices = NULL;

	Light.x =  -10.0;
	Light.y =  -10.0;
	Light.z =  -10.0;
	
	vEyePt.x =  0.0;
	vEyePt.y =  0.0;
	vEyePt.z = -10.0;

	assert(pMesh->pWorldVerts   !=NULL);
	assert(pMesh->pScreenVerts  !=NULL);
	assert(pMesh->pIndices !=NULL);
	assert(pMesh->pScreenIndices!=NULL);




//	free(pMesh->pScreenVerts);
//	pMesh->pScreenVerts   = (VERTEX_XYZRGBUV *)calloc(10000,sizeof(VERTEX_XYZRGBUV));





	pWorldVerts    = pMesh->pWorldVerts;
	pScreenVerts   = pMesh->pScreenVerts;
	pIndices  = pMesh->pIndices;
	pScreenIndices = pMesh->pScreenIndices;

//	memset(pScreenIndices,0,MAX_INDICES);
//	memset(pScreenVerts,0,MAX_VERTICES);

	pMesh->num_s_verts = 0;
	pMesh->num_s_tris  = 0;
	/////////////////////////////////////////////
	//// Transform Verts From Local To World ////
	/////////////////////////////////////////////

//	memcpy(pScreenIndices,pIndices,pMesh->num_tris*3*sizeof(WORD));

	for(i=0;i<pMesh->num_verts;i++)
	{
		pXFormedVerts[i].x  = (pLtoW[0]*pWorldVerts[i].x)  + (pLtoW[4]*pWorldVerts[i].y)  + (pLtoW[ 8]*pWorldVerts[i].z)  + pLtoW[12];
		pXFormedVerts[i].y  = (pLtoW[1]*pWorldVerts[i].x)  + (pLtoW[5]*pWorldVerts[i].y)  + (pLtoW[ 9]*pWorldVerts[i].z)  + pLtoW[13];
		pXFormedVerts[i].z  = (pLtoW[2]*pWorldVerts[i].x)  + (pLtoW[6]*pWorldVerts[i].y)  + (pLtoW[10]*pWorldVerts[i].z)  + pLtoW[14];
	}

	////////////////////////////////////////////////////
	//// Transform Face Normals From Local To World ////
	////////////////////////////////////////////////////

//	for(i=0;i<pMesh->num_verts;i++)
//	{
//		pXFormedVerts[i].nx  = (pLtoW[0]*pWorldVerts[i].nx)  + (pLtoW[4]*pWorldVerts[i].ny)  + (pLtoW[ 8]*pWorldVerts[i].nz);
//		pXFormedVerts[i].ny  = (pLtoW[1]*pWorldVerts[i].nx)  + (pLtoW[5]*pWorldVerts[i].ny)  + (pLtoW[ 9]*pWorldVerts[i].nz);
//		pXFormedVerts[i].nz  = (pLtoW[2]*pWorldVerts[i].nx)  + (pLtoW[6]*pWorldVerts[i].ny)  + (pLtoW[10]*pWorldVerts[i].nz);
//	}

	for(cFace=0;cFace<pMesh->num_tris;cFace++)
	{
		V[0] = pIndices[(cFace*3+0)];
		V[1] = pIndices[(cFace*3+2)];
		V[2] = pIndices[(cFace*3+1)];

	 	assert(V[0]>=0);
	 	assert(V[1]>=0);
	 	assert(V[2]>=0);
	 	assert(V[0]<MAX_VERTICES);
	 	assert(V[1]<MAX_VERTICES);
	 	assert(V[2]<MAX_VERTICES);

		Ax = pXFormedVerts[V[2]].x - pXFormedVerts[V[1]].x;
		Ay = pXFormedVerts[V[2]].y - pXFormedVerts[V[1]].y;
		Az = pXFormedVerts[V[2]].z - pXFormedVerts[V[1]].z;

		Bx = pXFormedVerts[V[0]].x - pXFormedVerts[V[1]].x;
		By = pXFormedVerts[V[0]].y - pXFormedVerts[V[1]].y;
		Bz = pXFormedVerts[V[0]].z - pXFormedVerts[V[1]].z;

		MacroCross(Normal.x,Normal.y,Normal.z,Ax,Ay,Az,Bx,By,Bz);
		MacroNormalize(Length,Normal.x,Normal.y,Normal.z);

		MacroDot(PlaneD ,Normal.x,Normal.y,Normal.z,pXFormedVerts[V[1]].x,pXFormedVerts[V[1]].y,pXFormedVerts[V[1]].z);
		MacroDot(CameraD,Normal.x,Normal.y,Normal.z,vEyePt.x,vEyePt.y,vEyePt.z);
//		if(CameraD > PlaneD)
		if(CameraD > PlaneD)
		{

			CenterOfGravity.x = (pXFormedVerts[V[0]].x + pXFormedVerts[V[1]].x + pXFormedVerts[V[2]].x)*0.3333333;
			CenterOfGravity.y = (pXFormedVerts[V[0]].y + pXFormedVerts[V[1]].y + pXFormedVerts[V[2]].y)*0.3333333;
			CenterOfGravity.z = (pXFormedVerts[V[0]].z + pXFormedVerts[V[1]].z + pXFormedVerts[V[2]].z)*0.3333333;
			////////////////////////////////////////
			//// Create Vector From CG to Light ////
			////////////////////////////////////////
			ToLight.x = Light.x - CenterOfGravity.x;
			ToLight.y = Light.y - CenterOfGravity.y;
			ToLight.z = Light.z - CenterOfGravity.z;
			MacroNormalize(Length,ToLight.x,ToLight.y,ToLight.z);

			/////////////////////////////////////////
			//// Create Vector From CG to Viewer ////
			/////////////////////////////////////////
			ToViewer.x = vEyePt.x - CenterOfGravity.x;
			ToViewer.y = vEyePt.y - CenterOfGravity.y;
			ToViewer.z = vEyePt.z - CenterOfGravity.z;
			MacroNormalize(Length,ToViewer.x,ToViewer.y,ToViewer.z);

			////////////////////////////////////
			//// Calculate Reflected Vector ////
			////////////////////////////////////

			MacroDot(NdotL,Normal.x,Normal.y,Normal.z,ToLight.x,ToLight.y,ToLight.z);
			Reflected.x = 2.0f * NdotL * Normal.x - ToLight.x;
			Reflected.y = 2.0f * NdotL * Normal.y - ToLight.y;
			Reflected.z = 2.0f * NdotL * Normal.z - ToLight.z;
			MacroNormalize(Length,Reflected.x,Reflected.y,Reflected.z);

			//////////////////////////////
			//// Calculate Phong Term ////
			//////////////////////////////

			MacroDot(Length,ToViewer.x,ToViewer.y,ToViewer.z,Reflected.x,Reflected.y,Reflected.z);
			if(Length >= 0.0)
			{
				fPhong = Length/(fPower-fPower*Length+Length);
				if(fPhong>0.5)fPhong=0.5;
			}
			else
			{
				fPhong = 0.0f;
			}

if(1)//(pMesh->num_s_tris)<pMesh->num_tris)//BSHACK
{
			pScreenIndices[(pMesh->num_s_tris*3+0)] = (pMesh->num_s_verts+0);
			pScreenIndices[(pMesh->num_s_tris*3+1)] = (pMesh->num_s_verts+1);
			pScreenIndices[(pMesh->num_s_tris*3+2)] = (pMesh->num_s_verts+2);
			pMesh->num_s_tris++;
}
			if(NdotL<0.0)NdotL=0.0;
			
			DiffuseIntensity  = (int)(NdotL*255.0);
			SpecularIntensity = (int)(fPhong*255.0);

//			DiffuseIntensity  = (int)((NdotL*255.0)+(fPhong*255.0));
//			if(DiffuseIntensity>255)DiffuseIntensity=255;

//			DiffuseIntensity  = (int)(fPhong*255.0);
//			DiffuseIntensity  = (int)(RandZeroToOne*255.0);
//			SpecularIntensity = (int)(RandZeroToOne*255.0);

if(1)//(pMesh->num_s_verts+3)<pMesh->num_verts)//BSHACK
{
			for(i=0;i<3;i++)
			{

				pScreenVerts[pMesh->num_s_verts].x  = (pXFormedVerts[V[i]].x+2.0)*9.0;
				pScreenVerts[pMesh->num_s_verts].y  = (pXFormedVerts[V[i]].y+2.0)*9.0;
				pScreenVerts[pMesh->num_s_verts].z  = 1.0;
				pScreenVerts[pMesh->num_s_verts].rhw = 1.0;

				pScreenVerts[pMesh->num_s_verts].diffuse  = PACKRGBA(DiffuseIntensity ,DiffuseIntensity ,DiffuseIntensity ,DiffuseIntensity );
				pScreenVerts[pMesh->num_s_verts].specular = PACKRGBA(SpecularIntensity,SpecularIntensity,SpecularIntensity,SpecularIntensity);

//				pScreenVerts[pMesh->num_s_verts].diffuse  = PACKRGBA(9,9,255,99);
//				pScreenVerts[pMesh->num_s_verts].specular = PACKRGBA(0,0,0,0);

//				pScreenVerts[pMesh->num_s_verts].tu = pXFormedVerts[V[i]].tu;
//				pScreenVerts[pMesh->num_s_verts].tv = pXFormedVerts[V[i]].tv;
				pScreenVerts[pMesh->num_s_verts].tu = pWorldVerts[V[i]].tu;
//				pScreenVerts[pMesh->num_s_verts].tv = (fPhong*30.0)/256.0;//pWorldVerts[V[i]].tv;
				pScreenVerts[pMesh->num_s_verts].tv = pWorldVerts[V[i]].tv;



				pMesh->num_s_verts++;
			}
}



			assert((pMesh->num_s_tris*3)<MAX_INDICES);
			assert(pMesh->num_s_verts<MAX_VERTICES);



		}//End if(CameraD > PlaneD)
	}//End for(cFace=0;cFace<cT_Gem;cFace++)
}



*/













void MeshLL_ColorizeBlue(struct MeshLL* pMesh)
{
	VERTEX_XYZNRGBUV* pLV = NULL;
	VERTEX_XYZNRGBUV* pWV = NULL;
	aVECTOR ToLight;
	aVECTOR Normal;
	float LdotN;
	float Length;
	float Ka = 0.1;
	float Kd = 1.0;
	long i;
	int red = 26;
	int grn = 26;
	int blu = 26;
	int alp = 255;
 
	if(pMesh==NULL)return;
 
	ToLight.x = 1.0;
	ToLight.y = 1.0;
	ToLight.z = -1.0;
	MacroNormalize(Length,ToLight.x,ToLight.y,ToLight.z);	

	assert(pMesh->pWorldVerts!=NULL);
	pLV = pMesh->pLocalVerts;
	pWV = pMesh->pWorldVerts;

	for(i=0;i<pMesh->num_verts;i++)
	{
		NormalMatrixMul(&Normal,
						&pLV[i],
						pMesh->pMatrix);//in this case, pMesh->pMat is final matrix
		MacroNormalize(Length,Normal.x,Normal.y,Normal.z);
	
		MacroDot(LdotN,	ToLight.x,ToLight.y,ToLight.z,
						Normal.x,Normal.y,Normal.z);

		blu = (int)(((LdotN+1.0)*0.5*Kd+Ka)*255.0);
		blu = Clamp(blu,0,255);

		pLV[i].diffuse = PACKRGBA(red,grn,blu,alp);
		pWV[i].diffuse = 0xffff0000;
	}
}
void MeshLL_ColorizeGreen(struct MeshLL* pMesh)
{
	VERTEX_XYZNRGBUV* pLV = NULL;
	VERTEX_XYZNRGBUV* pWV = NULL;
	aVECTOR ToLight;
	aVECTOR Normal;
	float LdotN;
	float Length;
	float Ka = 0.1;
	float Kd = 1.0;
	long i;
	int red = 26;
	int grn = 26;
	int blu = 26;
	int alp = 255;
 
	if(pMesh==NULL)return;
 
	ToLight.x = 1.0;
	ToLight.y = 1.0;
	ToLight.z = -1.0;
	MacroNormalize(Length,ToLight.x,ToLight.y,ToLight.z);	

	assert(pMesh->pWorldVerts!=NULL);
	pLV = pMesh->pLocalVerts;
	pWV = pMesh->pWorldVerts;

	for(i=0;i<pMesh->num_verts;i++)
	{
		NormalMatrixMul(&Normal,
						&pLV[i],
						pMesh->pMatrix);//in this case, pMesh->pMat is final matrix
		MacroNormalize(Length,Normal.x,Normal.y,Normal.z);
	
		MacroDot(LdotN,	ToLight.x,ToLight.y,ToLight.z,
						Normal.x,Normal.y,Normal.z);

		grn = (int)(((LdotN+1.0)*0.5*Kd+Ka)*255.0);
		grn = Clamp(grn,0,255);

		pLV[i].diffuse = PACKRGBA(red,grn,blu,alp);
		pWV[i].diffuse = 0xffff0000;
	}
}
void MeshLL_ColorizeWhite(struct MeshLL* pMesh)
{
	VERTEX_XYZNRGBUV* pLV = NULL;
	VERTEX_XYZNRGBUV* pWV = NULL;
	aVECTOR ToLight;
	aVECTOR Normal;
	float LdotN;
	float Length;
	float Ka = 0.1;
	float Kd = 1.0;
	long i;
	int red = 26;
	int grn = 26;
	int blu = 26;
	int alp = 255;
 
	if(pMesh==NULL)return;
 
	ToLight.x = 1.0;
	ToLight.y = 1.0;
	ToLight.z = -1.0;
	MacroNormalize(Length,ToLight.x,ToLight.y,ToLight.z);	

	assert(pMesh->pWorldVerts!=NULL);
	pLV = pMesh->pLocalVerts;
	pWV = pMesh->pWorldVerts;

	for(i=0;i<pMesh->num_verts;i++)
	{
		NormalMatrixMul(&Normal,
						&pLV[i],
						pMesh->pMatrix);//in this case, pMesh->pMat is final matrix
		MacroNormalize(Length,Normal.x,Normal.y,Normal.z);
	
		MacroDot(LdotN,	ToLight.x,ToLight.y,ToLight.z,
						Normal.x,Normal.y,Normal.z);

		grn = (int)(((LdotN+1.0)*0.5*Kd+Ka)*255.0);
		grn = Clamp(grn,0,255);

		blu = grn;
		red = grn;
		pLV[i].diffuse = PACKRGBA(red,grn,blu,alp);
		pWV[i].diffuse = 0xffff0000;
	}
}

/*
void MeshLL_ColorizeGreen(struct MeshLL* pMesh)
{
	VERTEX_XYZNRGBUV* pLV = NULL;
	VERTEX_XYZNRGBUV* pWV = NULL;
	long i;

	if(pMesh==NULL)return;

	assert(pMesh->pWorldVerts!=NULL);
	pLV = pMesh->pLocalVerts;
	pWV = pMesh->pWorldVerts;

	for(i=0;i<pMesh->num_verts;i++)
	{
		pLV[i].diffuse = 0xff00ff00;
		pWV[i].diffuse = 0xff00ff00;
	}
}
*/
/*

void MeshLL_ColorizeBlue(struct MeshLL* pMesh)
{
	VERTEX_XYZNRGBUV* pLV = NULL;
	VERTEX_XYZNRGBUV* pWV = NULL;
	long i;

	if(pMesh==NULL)return;

	assert(pMesh->pWorldVerts!=NULL);
	pLV = pMesh->pLocalVerts;
	pWV = pMesh->pWorldVerts;

	for(i=0;i<pMesh->num_verts;i++)
	{
		pLV[i].diffuse = 0xff00aaaa;
		pWV[i].diffuse = 0xff00aaaa;
	}
}
*/
/*
void MeshLL_ColorizeWhite(struct MeshLL* pMesh)
{
	VERTEX_XYZNRGBUV* pLV = NULL;
	VERTEX_XYZNRGBUV* pWV = NULL;
	long i;

	if(pMesh==NULL)return;

	assert(pMesh->pWorldVerts!=NULL);
	pLV = pMesh->pLocalVerts;
	pWV = pMesh->pWorldVerts;

	for(i=0;i<pMesh->num_verts;i++)
	{
		pLV[i].diffuse = 0xffffffff;
		pWV[i].diffuse = 0xffffffff;
	}
}
*/
void MeshLL_ColorizeBlack(struct MeshLL* pMesh)
{
	long i;
	VERTEX_XYZNRGBUV* pLV = NULL;
	VERTEX_XYZNRGBUV* pWV = NULL;

	if(pMesh==NULL)return;

	assert(pMesh->pWorldVerts!=NULL);
	pLV = pMesh->pLocalVerts;
	pWV = pMesh->pWorldVerts;

	for(i=0;i<pMesh->num_verts;i++)
	{
		//pLV[i].diffuse = 0xff000000;
		pWV[i].diffuse = 0xff000000;
	}
}
void MeshLL_CopyLocalToWorld(struct MeshLL* pMesh)
{
	VERTEX_XYZNRGBUV* pLV = NULL;
	VERTEX_XYZNRGBUV* pWV = NULL;

	if(pMesh==NULL)return;

	assert(pMesh->pLocalVerts!=NULL);
	assert(pMesh->pWorldVerts!=NULL);

	pLV = pMesh->pLocalVerts;
	pWV = pMesh->pWorldVerts;

	memcpy(pWV,pLV,pMesh->num_verts*sizeof(VERTEX_XYZNRGBUV));
}





//2 255,0,0,

void MeshLL_ColorizeUsingBoneWeights(	struct MeshLL* pMesh,
										unsigned long JointIndex,
										unsigned long Red,
										unsigned long Grn,
										unsigned long Blu)
{
	long i,j;
	long iRed;
	long iGrn;
	long iBlu;
	VERTEX_XYZNRGBUV* pVert = NULL;
	PERVERTEX_JOINT_DATA* pPVBData = NULL;

	if(pMesh==NULL)return;

	assert(pMesh->pWorldVerts!=NULL);
	assert(pMesh->pPerVertexJointData!=NULL);

	pVert = pMesh->pWorldVerts;
	//pVert = pMesh->pLocalVerts;
	pPVBData = pMesh->pPerVertexJointData;

//	assert(pMesh->RenderFlags&RENDER_TRANSFORMED);

	for(i=0;i<pMesh->num_verts;i++)
	{
		pVert[i].diffuse = 0xff000000;
		for(j=0;j<4;j++)
		{
			if(pPVBData[i].index[j] == JointIndex)
			{
				iRed = (int)((float)Red*pPVBData[i].weight[j]);
				iGrn = (int)((float)Grn*pPVBData[i].weight[j]);
				iBlu = (int)((float)Blu*pPVBData[i].weight[j]);
				pVert[i].diffuse += (((iRed)&0xff)<<16);
				pVert[i].diffuse += (((iGrn)&0xff)<<8 );
				pVert[i].diffuse +=  ((iBlu)&0xff);
			}
		}
//	 	pVert[i].diffuse = RandZeroToOne*0xffffffff;
//		pVert[i].diffuse = 0xff00ff00;
	}
}





void MeshLL_NormalizeBoneWeights(struct MeshLL* pMesh)
{
	float Length;
	long i;
	VERTEX_XYZNRGBUV* pVert = NULL;
	PERVERTEX_JOINT_DATA* pPVBData = NULL;

	if(pMesh==NULL)return;

	assert(pMesh->pLocalVerts!=NULL);
	assert(pMesh->pPerVertexJointData!=NULL);

	pVert = pMesh->pWorldVerts;
	pPVBData = pMesh->pPerVertexJointData;

	for(i=0;i<pMesh->num_verts;i++)
	{
		MacroNormalizeWeights(Length,
						pPVBData[i].weight[0],
						pPVBData[i].weight[1],
						pPVBData[i].weight[2],
						pPVBData[i].weight[3]);
	}
}



void MeshLL_AddOffsetXYZ(	struct MeshLL* pMesh,
							float OffsetX,
							float OffsetY,
							float OffsetZ)
{
	long i;
	VERTEX_XYZNRGBUV* pVert = NULL;

	if(pMesh == NULL ||pMesh->pLocalVerts == NULL)
	{
		return;
	}

	pVert = pMesh->pLocalVerts;

	//start with next item
	for(i=0;i<pMesh->num_verts;i++)
	{
		pVert[i].x += OffsetX;
		pVert[i].y += OffsetY;
		pVert[i].z += OffsetZ;
	}
}
















void MeshLL_CalculateVertexNormals(struct MeshLL* pMesh)
{
	long i;
	VERTEX_XYZNRGBUV* pVert = NULL;
	WORD* pIndices = NULL;
	float Length,Ax,Ay,Az,Bx,By,Bz,Rx,Ry,Rz;
	unsigned int V0,V1,V2;

	if(pMesh == NULL ||pMesh->pLocalVerts == NULL)
	{
		return;
	}

	pVert     = pMesh->pLocalVerts;
	pIndices  = pMesh->pIndices;


	//////////////////////////////////
	//// Calculate Vertex Normals ////
	//////////////////////////////////

	for(i=0;i<pMesh->num_verts;i++)
	{
		pVert[i].nx = 0.0f;
		pVert[i].ny = 0.0f;
		pVert[i].nz = 0.0f;
	}
	for(i=0;i<pMesh->num_tris;i++)
	{
		V0 = pIndices[(i*3+0)];
		V1 = pIndices[(i*3+1)];
		V2 = pIndices[(i*3+2)];

		Ax = pVert[V2].x - pVert[V1].x;
		Ay = pVert[V2].y - pVert[V1].y;
		Az = pVert[V2].z - pVert[V1].z;

		Bx = pVert[V0].x - pVert[V1].x;
		By = pVert[V0].y - pVert[V1].y;
		Bz = pVert[V0].z - pVert[V1].z;

		MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
		MacroNormalize(Length,Rx,Ry,Rz);
		
		pVert[V0].nx += Rx;
		pVert[V0].ny += Ry;
		pVert[V0].nz += Rz;

		pVert[V1].nx += Rx;
		pVert[V1].ny += Ry;
		pVert[V1].nz += Rz;

		pVert[V2].nx += Rx;
		pVert[V2].ny += Ry;
		pVert[V2].nz += Rz;
	}
	for(i=0;i<pMesh->num_verts;i++)
	{
		MacroNormalize(Length,pVert[i].nx,pVert[i].ny,pVert[i].nz);
	}

}









//for every tri facing camera, flag its vertices as visible.
//this is better than using vertex normals to determine vertex visibility. That case has errors on edges.
//							int* pVisibleV)

void Mesh_FlagVerticesAsVisible(float* pObjectMatrix,
								struct MeshLL* pMesh)
{
	aVECTOR CameraPos;
	float PdotN,CdotN;
	float Length,Ax,Ay,Az,Bx,By,Bz,Rx,Ry,Rz;
	float pInverseView[16];
	long i,V1,V2,V3;
	VERTEX_XYZNRGBUV* 	pLocalVerts = NULL;
	VERTEX_XYZNRGBUV* 	pWorldVerts = NULL;
	unsigned char*		pVertexFlags = NULL;//selected, visible
	float pFinalModelMatrix[16];
	WORD* pIndices = NULL;

	InverseMatrix(pInverseView,pRender_ViewMatrix);
	CameraPos.x = pInverseView[12];
	CameraPos.y = pInverseView[13];
	CameraPos.z = pInverseView[14];
				 
	pLocalVerts  = pMesh->pLocalVerts;
	pWorldVerts  = pMesh->pWorldVerts;
	pVertexFlags = pMesh->pVertexFlags;
	pIndices     = pMesh->pIndices;

	//CALCULATEFINALMESHMATRIX
	if(0)//pMesh->RenderFlags & RENDER_TRANSFORMED)
	{
		memcpy(	pFinalModelMatrix,
				pObjectMatrix,
				//pRender_IdentityMatrix,
				16*sizeof(float));
	}
	else
	{
		MatrixMul(pFinalModelMatrix,pMesh->pMatrix,pObjectMatrix);
	}

	//emulate LocalToWorld transform here
	for(i=0;i<pMesh->num_verts;i++)
	{
		VertexMatrixMul(&pWorldVerts[i],
						&pLocalVerts[i],
						pFinalModelMatrix);

		//while we are here, clear the visible list for "assume not visible"
		//pVisibleV[i] = false;
		ClearBit(pVertexFlags[i],VERTEX_VISIBLE);
	}

	// for every tri facing camera, flag its vertices as visible.
	for(i=0;i<pMesh->num_tris;i++)
	{
		V1 = pIndices[(i*3+0)];
		V2 = pIndices[(i*3+1)];
		V3 = pIndices[(i*3+2)];

		////////////////////////////////
		//// Calculate Plane Normal ////
		////////////////////////////////

		Ax = pWorldVerts[V3].x - pWorldVerts[V2].x;
		Ay = pWorldVerts[V3].y - pWorldVerts[V2].y;
		Az = pWorldVerts[V3].z - pWorldVerts[V2].z;

		Bx = pWorldVerts[V1].x - pWorldVerts[V2].x;
		By = pWorldVerts[V1].y - pWorldVerts[V2].y;
		Bz = pWorldVerts[V1].z - pWorldVerts[V2].z;

		MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
		MacroNormalize(Length,Rx,Ry,Rz);

		//////////////////////////////////////
		//// Calculate Center Of Triangle ////
		//////////////////////////////////////

		MacroDot(PdotN,Rx,Ry,Rz,pWorldVerts[V1].x,pWorldVerts[V1].y,pWorldVerts[V1].z);
		MacroDot(CdotN,Rx,Ry,Rz,CameraPos.x,CameraPos.y,CameraPos.z);

		if((-CdotN) > (-PdotN))
		{
			pVertexFlags[V1] |= VERTEX_VISIBLE;
			pVertexFlags[V2] |= VERTEX_VISIBLE;
			pVertexFlags[V3] |= VERTEX_VISIBLE;
		}				
	}
}


























void Mesh_FlagVerticesAsSelected(	float* pObjectMatrix,
									struct MeshLL* pMesh,
									POINT ptRectStart,
									POINT ptRectStop)
{
	VERTEX_XYZNRGBUV* pLocalVerts = NULL;
	VERTEX_XYZNRGBUV* pWorldVerts = NULL;
	POINT ptUL;
	POINT ptLR;
	float pInverseView[16];
	float pInverseModel[16];
	float pInverseFinal[16];
	float CdotN1,CdotN2,CdotN3,CdotN4;
	float PdotN1,PdotN2,PdotN3,PdotN4;
	unsigned char*		pVertexFlags = NULL;//selected, visible
	float pFinalModelMatrix[16];

	long i;
	//vertors defining selection frustum in camera coords
	aVECTOR cvUL,cvUR,cvLL,cvLR;

	//vertors defining selection frustum in world coords
	aVECTOR wvUL,wvUR,wvLL,wvLR;

	aVECTOR wpPos;
	aVECTOR nTop;
	aVECTOR nBottom;
	aVECTOR nLeft;
	aVECTOR nRight;


	//the source rect can go any which way, so find UL, and LR to simplify
	//find upper, and lower
	if(ptRectStart.y<ptRectStop.y)
	{
		ptUL.y = ptRectStart.y;
		ptLR.y = ptRectStop.y;
	}
	else
	{
		ptUL.y = ptRectStop.y;
		ptLR.y = ptRectStart.y;
	}
	//find left, and right
	if(ptRectStart.x<ptRectStop.x)
	{
		ptUL.x = ptRectStart.x;
		ptLR.x = ptRectStop.x;
	}
	else
	{
		ptUL.x = ptRectStop.x;
		ptLR.x = ptRectStart.x;
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

	cvUL.x =  (((2.0*ptUL.x)/Render_WindowWidth )-1.0)/pRender_ProjMatrix[0];
	cvUL.y = -(((2.0*ptUL.y)/Render_WindowHeight)-1.0)/pRender_ProjMatrix[5];
	cvUL.z =  1.0;

	cvUR.x =  (((2.0*ptLR.x)/Render_WindowWidth )-1.0)/pRender_ProjMatrix[0];
	cvUR.y = -(((2.0*ptUL.y)/Render_WindowHeight)-1.0)/pRender_ProjMatrix[5];
	cvUR.z =  1.0;

	cvLL.x =  (((2.0*ptUL.x)/Render_WindowWidth )-1.0)/pRender_ProjMatrix[0];
	cvLL.y = -(((2.0*ptLR.y)/Render_WindowHeight)-1.0)/pRender_ProjMatrix[5];
	cvLL.z =  1.0;

	cvLR.x =  (((2.0*ptLR.x)/Render_WindowWidth )-1.0)/pRender_ProjMatrix[0];
	cvLR.y = -(((2.0*ptLR.y)/Render_WindowHeight)-1.0)/pRender_ProjMatrix[5];
	cvLR.z =  1.0;


	pLocalVerts  = pMesh->pLocalVerts;
	pWorldVerts  = pMesh->pWorldVerts;
	pVertexFlags = pMesh->pVertexFlags;

	InverseMatrix(pInverseView,pRender_ViewMatrix);

	//CALCULATEFINALMESHMATRIX
	if(0)//pMesh->RenderFlags & RENDER_TRANSFORMED)
	{
		memcpy(	pFinalModelMatrix,
				pObjectMatrix,
	  //			pRender_IdentityMatrix,
				16*sizeof(float));
	}
	else
	{
		MatrixMul(pFinalModelMatrix,pMesh->pMatrix,pObjectMatrix);
	}


	InverseMatrix(pInverseModel,pFinalModelMatrix);

	MatrixMul(pInverseFinal,pInverseView,pInverseModel);

	wpPos.x = pInverseFinal[12];
	wpPos.y = pInverseFinal[13];
	wpPos.z = pInverseFinal[14];

	wvUL.x  = cvUL.x*pInverseFinal[0] + cvUL.y*pInverseFinal[4] + cvUL.z*pInverseFinal[8 ] + wpPos.x;
	wvUL.y  = cvUL.x*pInverseFinal[1] + cvUL.y*pInverseFinal[5] + cvUL.z*pInverseFinal[9 ] + wpPos.y;
	wvUL.z  = cvUL.x*pInverseFinal[2] + cvUL.y*pInverseFinal[6] + cvUL.z*pInverseFinal[10] + wpPos.z;

	wvUR.x  = cvUR.x*pInverseFinal[0] + cvUR.y*pInverseFinal[4] + cvUR.z*pInverseFinal[8 ] + wpPos.x;
	wvUR.y  = cvUR.x*pInverseFinal[1] + cvUR.y*pInverseFinal[5] + cvUR.z*pInverseFinal[9 ] + wpPos.y;
	wvUR.z  = cvUR.x*pInverseFinal[2] + cvUR.y*pInverseFinal[6] + cvUR.z*pInverseFinal[10] + wpPos.z;

	wvLL.x  = cvLL.x*pInverseFinal[0] + cvLL.y*pInverseFinal[4] + cvLL.z*pInverseFinal[8 ] + wpPos.x;
	wvLL.y  = cvLL.x*pInverseFinal[1] + cvLL.y*pInverseFinal[5] + cvLL.z*pInverseFinal[9 ] + wpPos.y;
	wvLL.z  = cvLL.x*pInverseFinal[2] + cvLL.y*pInverseFinal[6] + cvLL.z*pInverseFinal[10] + wpPos.z;

	wvLR.x  = cvLR.x*pInverseFinal[0] + cvLR.y*pInverseFinal[4] + cvLR.z*pInverseFinal[8 ] + wpPos.x;
	wvLR.y  = cvLR.x*pInverseFinal[1] + cvLR.y*pInverseFinal[5] + cvLR.z*pInverseFinal[9 ] + wpPos.y;
	wvLR.z  = cvLR.x*pInverseFinal[2] + cvLR.y*pInverseFinal[6] + cvLR.z*pInverseFinal[10] + wpPos.z;

	CalculatePlaneNormal(nTop   , wvUL, wpPos, wvUR);
	CalculatePlaneNormal(nRight , wvUR, wpPos, wvLR);
	CalculatePlaneNormal(nBottom, wvLR, wpPos, wvLL);
	CalculatePlaneNormal(nLeft  , wvLL, wpPos, wvUL);

	// for every vert see if it is in front of each of the 4 frustum planes
	MacroDot(CdotN1,nTop.x,nTop.y,nTop.z,wpPos.x,wpPos.y,wpPos.z);
	MacroDot(CdotN2,nBottom.x,nBottom.y,nBottom.z,wpPos.x,wpPos.y,wpPos.z);
	MacroDot(CdotN3,nLeft.x,nLeft.y,nLeft.z,wpPos.x,wpPos.y,wpPos.z);
	MacroDot(CdotN4,nRight.x,nRight.y,nRight.z,wpPos.x,wpPos.y,wpPos.z);

	for(i=0;i<pMesh->num_verts;i++)
	{
		//MacroDot(PdotN1,nTop.x,nTop.y,nTop.z,pWorldVerts[i].x,pWorldVerts[i].y,pWorldVerts[i].z);
		//MacroDot(PdotN2,nBottom.x,nBottom.y,nBottom.z,pWorldVerts[i].x,pWorldVerts[i].y,pWorldVerts[i].z);
		//MacroDot(PdotN3,nLeft.x,nLeft.y,nLeft.z,pWorldVerts[i].x,pWorldVerts[i].y,pWorldVerts[i].z);
		//MacroDot(PdotN4,nRight.x,nRight.y,nRight.z,pWorldVerts[i].x,pWorldVerts[i].y,pWorldVerts[i].z);

		//MacroDot(CdotN ,nTop.x,nTop.y,nTop.z,wpPos.x,wpPos.y,wpPos.z);
		MacroDot(PdotN1,nTop.x,nTop.y,nTop.z,pLocalVerts[i].x,pLocalVerts[i].y,pLocalVerts[i].z);
		MacroDot(PdotN2,nBottom.x,nBottom.y,nBottom.z,pLocalVerts[i].x,pLocalVerts[i].y,pLocalVerts[i].z);
		MacroDot(PdotN3,nLeft.x,nLeft.y,nLeft.z,pLocalVerts[i].x,pLocalVerts[i].y,pLocalVerts[i].z);
		MacroDot(PdotN4,nRight.x,nRight.y,nRight.z,pLocalVerts[i].x,pLocalVerts[i].y,pLocalVerts[i].z);

		ClearBit(pVertexFlags[i],VERTEX_SELECTED);
		if((PdotN1) > (CdotN1))
		{
			if((PdotN2) > (CdotN2))
			{
				if((PdotN3) > (CdotN3))
				{
					if((PdotN4) > (CdotN4))
					{
						pVertexFlags[i] |= VERTEX_SELECTED;
						//pSelectedVerts[i] = true;
					}				
				}				
			}				
		}				
	}				
}

void Mesh_Mouse_Move_SelectedVertices(	float* pObjectMatrix,
										struct MeshLL* pMesh,
										POINT MouseDiff)
{
	aVECTOR Offset;
	aVECTOR CameraPos;
	float MinSquaredDistance,SquaredDistance,MinDistance,Ax,Ay,Az;
	long i;
	VERTEX_XYZNRGBUV* pLocalVerts 	= NULL;
	VERTEX_XYZNRGBUV* pWorldVerts 	= NULL;
	unsigned char*	  pVertexFlags 	= NULL;//selected, visible
	float pInverseView[16];
	float pInverseModel[16];
	float pInverseFinal[16];
	float pFinalModelMatrix[16];



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


	InverseMatrix(pInverseView,pRender_ViewMatrix);

	//CALCULATEFINALMESHMATRIX
	if(0)//pMesh->RenderFlags & RENDER_TRANSFORMED)
	{
		memcpy(	pFinalModelMatrix,
				pObjectMatrix,
	  //			pRender_IdentityMatrix,
				16*sizeof(float));
	}
	else
	{
		MatrixMul(pFinalModelMatrix,pMesh->pMatrix,pObjectMatrix);
	}

	InverseMatrix(pInverseModel,pFinalModelMatrix);

	MatrixMul(pInverseFinal,pInverseView,pInverseModel);







	pLocalVerts  = pMesh->pLocalVerts;
	pWorldVerts  = pMesh->pWorldVerts;
	pVertexFlags = pMesh->pVertexFlags;

	CameraPos.x = pInverseFinal[12];
	CameraPos.y = pInverseFinal[13];
	CameraPos.z = pInverseFinal[14];
	MinSquaredDistance = FLT_MAX;

	for(i=0;i<pMesh->num_verts;i++)
	{
		if(pVertexFlags[i] & VERTEX_SELECTED)
		{
			Ax = pLocalVerts[i].x-CameraPos.x;
			Ay = pLocalVerts[i].y-CameraPos.y;
			Az = pLocalVerts[i].z-CameraPos.z;
			SquaredDistance = (Ax*Ax)+(Ay*Ay)+(Az*Az);
			if(SquaredDistance<MinSquaredDistance)
			{
				MinSquaredDistance = SquaredDistance;
			}
		}
	}
	
	MinDistance = sqrt(MinSquaredDistance);
	
	Offset.x = ((MouseDiff.x/(Render_WindowWidth *0.5))/pRender_ProjMatrix[0])*MinDistance;
	Offset.y = ((MouseDiff.y/(Render_WindowHeight*0.5))/pRender_ProjMatrix[5])*MinDistance;
	Offset.z = 0.0;

	for(i=0;i<pMesh->num_verts;i++)
	{
		if(pVertexFlags[i] & VERTEX_SELECTED)
		{
			pLocalVerts[i].x += Offset.x*pInverseFinal[0];
			pLocalVerts[i].y += Offset.x*pInverseFinal[1];
			pLocalVerts[i].z += Offset.x*pInverseFinal[2];
			pLocalVerts[i].x += Offset.y*pInverseFinal[4];
			pLocalVerts[i].y += Offset.y*pInverseFinal[5];
			pLocalVerts[i].z += Offset.y*pInverseFinal[6];
		}
	}
}


