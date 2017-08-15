// IconLL.cpp written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
#include "stdafx.h"
#include "resource.h"
#include "Library.h"
#include <assert.h>
#include <stdio.h>

struct IconLL* IconLL_Insert(	char* pName,
								unsigned long SortKey,
								fRECT Screen,
								fRECT Texture,
								unsigned long flags,
								struct IconLL*& pIconHeadIn)
{
	struct IconLL* pIconHead = pIconHeadIn;
	struct IconLL* pInsertMe = NULL;
	struct IconLL* pThis = NULL;
	struct IconLL* pNext = NULL;

	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe = (struct IconLL *)calloc(1,sizeof(struct IconLL));
	pInsertMe->pNext	  = NULL;

	sprintf(pInsertMe->pName,"%s",pName);
	pInsertMe->SortKey = SortKey;
	pInsertMe->Screen.left   = Screen.left;
	pInsertMe->Screen.right  = Screen.right;
	pInsertMe->Screen.top    = Screen.top;
	pInsertMe->Screen.bottom = Screen.bottom;

	pInsertMe->Texture.left   = Texture.left;
	pInsertMe->Texture.right  = Texture.right;
	pInsertMe->Texture.top    = Texture.top;
	pInsertMe->Texture.bottom = Texture.bottom;

	pInsertMe->flags = flags;

	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pIconHead == NULL)
	{
		pIconHeadIn = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pIconHead->SortKey)
	{	
		pInsertMe->pNext  = pIconHead;
		pIconHeadIn      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pIconHead;
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

void IconLL_Remove(char* pName,
					struct IconLL*& pIconHeadIn)

{
	struct IconLL* pIconHead = pIconHeadIn;
	struct IconLL* pRemoveMe  = NULL;
	struct IconLL* pThis = NULL;
	struct IconLL* pNext = NULL;
	struct IconLL* pLast = NULL;

	pThis = pIconHead;
	pLast = pIconHead;
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

			if(pRemoveMe == pIconHead)
			{//We are removing the head
				if(pNext==NULL)
				{//we are removing the last
					pIconHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pIconHeadIn = pNext;
				}
			}
			else
			{//We are removing a normal item
				pLast->pNext = pNext;
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

void IconLL_RemoveVolatile(struct IconLL*& pIconHeadIn)
{
	struct IconLL* pRemoveMe  = NULL;
	struct IconLL* pThis = NULL;
	struct IconLL* pNext = NULL;

	pThis = pIconHeadIn;
	while(pThis!=NULL)
	{
		if(pThis->flags&TEXT_FLAG_VOLATILE)
		{
			pRemoveMe = pThis;
			if(pRemoveMe == pIconHeadIn)
			{//We are removing the head
				if(pThis->pNext==NULL)
				{//we are removing the last
					pIconHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pIconHeadIn = pThis->pNext;
				}
			}
			pThis = pThis->pNext;
			free(pRemoveMe);
		}
		else
		{
			pThis = pThis->pNext;
		}
	}
}
void IconLL_RemoveALL(struct IconLL*& pIconHeadIn)

{
	struct IconLL* pRemoveMe  = NULL;
	struct IconLL* pThis = NULL;

	pThis = pIconHeadIn;
	while(pThis!=NULL)
	{
		pRemoveMe = pThis;
		pThis = pThis->pNext;
		free(pRemoveMe);
	}
	pIconHeadIn = NULL;
}
void IconLL_Sort(struct IconLL*& pIconHeadIn)
{
	struct IconLL* pNewIconHead = NULL;
	struct IconLL* pThis = NULL;

	pThis = pIconHeadIn;
	while(pThis!=NULL)
	{
		IconLL_Insert(	pThis->pName,
						pThis->SortKey,
						pThis->Screen,
						pThis->Texture,
						pThis->flags,
						pNewIconHead);
		pThis = pThis->pNext;
	}
	pIconHeadIn = pNewIconHead;
}

/*
BOOL IconLL_Save(char *pFileName)
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
		struct IconLL* pItem = pFileHead;
		while(pItem)
		{
			Size = 5*sizeof(unsigned long);
			memcpy(pMiniBuffer2,(char*)&pItem->IconType,Size);
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
	//pointer to mesh Icon
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

typedef struct _IconLL
{
	unsigned long BinIndex;
	_BinLL 	 *pBinHead;
	_IconLL *pPrev;
	_IconLL *pNext;
}IconLL;


*/