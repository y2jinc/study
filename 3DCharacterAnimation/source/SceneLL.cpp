// SceneLL.cpp written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
#include "stdafx.h"
#include "resource.h"
#include "Library.h"
#include "FileAccess.h"
#include "ObjectLL.h"
#include <assert.h>
#include <stdio.h>


extern struct SceneLL* pCurrentlySelectedScene;
extern char* gp_TextBuffer1;
extern char* gp_TextBuffer2;
extern char* gp_TextBuffer3;


//long NextSceneIndex = 0;

extern void AppendDebugMsg(char *pChar);



struct SceneLL* SceneLL_Insert(	unsigned long SortKey,
								char* pName,
								struct SceneLL*& pSceneHeadIn)

{
	struct SceneLL* pSceneHead = pSceneHeadIn;
	struct SceneLL* pInsertMe = NULL;
	struct SceneLL* pThis = NULL;
	struct SceneLL* pNext = NULL;

	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe = (struct SceneLL *)calloc(1,sizeof(struct SceneLL));
	pInsertMe->pNext	   = NULL;
	pInsertMe->pObjectHead = NULL;			  
	pInsertMe->SortKey = SortKey;
	sprintf(pInsertMe->pName,"%s",pName);

	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pSceneHead == NULL)
	{
		pSceneHeadIn = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pSceneHead->SortKey)
	{	
		pInsertMe->pNext  = pSceneHead;
		pSceneHeadIn      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pSceneHead;
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

void SceneLL_Remove(char* pName,
					struct SceneLL*& pSceneHeadIn)

{
	struct SceneLL* pSceneHead = pSceneHeadIn;
	struct SceneLL* pRemoveMe  = NULL;
	struct SceneLL* pThis = NULL;
	struct SceneLL* pNext = NULL;
	struct SceneLL* pLast = NULL;

	pThis = pSceneHead;
	pLast = pSceneHead;
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

			if(pRemoveMe == pSceneHead)
			{//We are removing the head
				if(pNext==NULL)
				{//we are removing the last
					pSceneHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pSceneHeadIn = pNext;
				}
			}
			else
			{//We are removing a normal item
				pLast->pNext = pNext;
			}

			//free all objects.
			ObjectLL_RemoveALL(pRemoveMe->pObjectHead);

			//MainMenu MainMenu
			if(pCurrentlySelectedScene==pRemoveMe)
			{//if we delete current scene, clear all pointers to the data
				pCurrentlySelectedScene = NULL;
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
void SceneLL_Remove(struct SceneLL*& pRemoveMe,
					struct SceneLL*& pSceneHeadIn)

{
	struct SceneLL* pSceneHead = pSceneHeadIn;
//	struct SceneLL* pRemoveMe  = NULL;
	struct SceneLL* pThis = NULL;
	struct SceneLL* pNext = NULL;
	struct SceneLL* pLast = NULL;


	pThis = pSceneHead;
	pLast = pSceneHead;
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

			if(pThis == pSceneHead)
			{//We are removing the head
				if(pThis->pNext==NULL)
				{//we are removing the last
					pSceneHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pSceneHeadIn = pThis->pNext;
				}
			}
			else
			{//We are removing a normal item
				pLast->pNext = pThis->pNext;
			}

			//free all objects.
			ObjectLL_RemoveALL(pThis->pObjectHead);

			//MainMenu MainMenu
			if(pThis == pCurrentlySelectedScene)
			{//if we delete current scene, clear all pointers to the data
				pCurrentlySelectedScene = NULL;
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
	assert(0);//scene not found
}

void SceneLL_RemoveALL(struct SceneLL*& pSceneHeadIn)

{
	struct SceneLL* pRemoveMe  = NULL;
	struct SceneLL* pThis = NULL;

	pThis = pSceneHeadIn;
	while(pThis!=NULL)
	{
		pRemoveMe = pThis;
		pThis = pThis->pNext;

		//free all objects.
		ObjectLL_RemoveALL(pRemoveMe->pObjectHead);

		//MainMenu MainMenu
		if(pCurrentlySelectedScene==pRemoveMe)
		{//if we delete current scene, clear all pointers to the data
			pCurrentlySelectedScene = NULL;
		}

		free(pRemoveMe);
	}
	pSceneHeadIn = NULL;

}





bool SceneLL_Get(	struct SceneLL*  pSceneHeadIn,
					struct SceneLL*& pPrevIn,
					struct SceneLL*& pThisIn,
					struct SceneLL*& pNextIn,
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
	pThisIn = pSceneHeadIn;
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

void SceneLL_GetFirst(	struct SceneLL*  pSceneHeadIn,
						struct SceneLL*& pPrev,
						struct SceneLL*& pThis,
						struct SceneLL*& pNext)
{
	pPrev = NULL;
	pThis = pSceneHeadIn;
	pNext = (pSceneHeadIn) ? (pSceneHeadIn->pNext) : (NULL);
}

void SceneLL_GetLast(	struct SceneLL*  pSceneHeadIn,
						struct SceneLL*& pPrev,
						struct SceneLL*& pThis,
						struct SceneLL*& pNext)
{
	pPrev = NULL;
	pThis = NULL;
	pNext = pSceneHeadIn;

	while(pNext)
	{
		pPrev = pThis;
		pThis = pNext;
		pNext = pThis->pNext;
	}
}

//out of date?
//see mesh resort.
void SceneLL_Sort(struct SceneLL*& pSceneHeadIn)
{
	struct SceneLL* pNewSceneHead = NULL;
	struct SceneLL* pThis = NULL;

	pThis = pSceneHeadIn;
	while(pThis!=NULL)
	{
		SceneLL_Insert(	pThis->SortKey,
						pThis->pName,
						pNewSceneHead);
		pThis = pThis->pNext;
	}
	pSceneHeadIn = pNewSceneHead;
}





/*
BOOL SceneLL_Save(char *pFileName)
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
		struct SceneLL* pItem = pFileHead;
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





*/























bool SceneLL_Load(struct SceneLL* pScene,char *pFileName)
{
//	struct SceneLL* pNewScene = NULL;
	struct ObjectLL* pNewObject = NULL;
	HANDLE fH;
	long i,length;
	int returnval;
	BOOL bResult = FALSE;
	unsigned long num_objects;
	unsigned char *pSrc = NULL;
	char pObjectName[256];
//	char pSceneName[256];

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
		ReadFromFile(fH,(char*) pScene->pName,256);
		ReadFromFile(fH,(char*)&pScene->SortKey,sizeof(unsigned long));
		ReadFromFile(fH,(char*)&num_objects,sizeof(unsigned long));

		for(i=0;i<num_objects;i++)
		{
			ReadFromFile(fH,(char*)pObjectName,256);
			pNewObject = ObjectLL_Insert(0,pObjectName,pScene->pObjectHead);

				
			sprintf(gp_TextBuffer2,"%s\\%s.obb",gp_TextBuffer1,pObjectName);

			if(ObjectLL_Load(pNewObject,gp_TextBuffer2))
			{
			}
			else
			{
			}
		}
				
		ObjectLL_Sort(pScene->pObjectHead);

		returnval = CloseHandle(fH);
		return true;
	}//End Open File
	return false;
}//End 

bool SceneLL_Save(struct SceneLL* pScene,char *pFileName)
{
	struct ObjectLL* pObject = NULL;
	HANDLE fH;
	int returnval;
	BOOL bResult = FALSE;
	unsigned long num_objects;
	unsigned char *pSrc = NULL;
//	char FullName[512];

//	sprintf(FullName,"%s.scn",pFileName);
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
		WriteToFile(fH,(char*) pScene->pName,256);
		WriteToFile(fH,(char*)&pScene->SortKey,sizeof(unsigned long));

		////////////////////////////
		//// Count All Objectes ////
		////////////////////////////
		
		num_objects = 0;
		pObject = pScene->pObjectHead;
		while(pObject)	 
		{
			num_objects++;
		 	pObject = pObject->pNext;
		}
		WriteToFile(fH,(char*)&num_objects,sizeof(unsigned long));

		///////////////////////////////
		//// Save All Object Names ////
		///////////////////////////////

		pObject = pScene->pObjectHead;
		while(pObject)	 
		{
			WriteToFile(fH,(char*)pObject->pName,256);
		 	pObject = pObject->pNext;
		}

		////////////////////
		//// CLOSE File ////
		////////////////////

		returnval = CloseHandle(fH);

		///////////////////////////
		//// Save All Objectes ////
		///////////////////////////

		pObject = pScene->pObjectHead;
		while(pObject)	 
		{
			sprintf(gp_TextBuffer2,"%s\\%s.obb",gp_TextBuffer1,pObject->pName);

			ObjectLL_Save(pObject,gp_TextBuffer2);
		 	pObject = pObject->pNext;
		}
		return true;
	}//End Open File
	return false;
}//End 



void SceneLL_SetInVRAM(struct SceneLL* pScene,bool bInVRAM)
{
	struct ObjectLL* pObject = NULL;

	if(pScene==NULL)return;
	pObject = pScene->pObjectHead;
	while(pObject)	 
	{
		ObjectLL_SetInVRAM(pObject,bInVRAM);
	 	pObject = pObject->pNext;
	}
}
