// Direct3DTextureLL.cpp written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to

#include "stdafx.h"
#include "..\globals.h"
//#include "resource.h"
#include "Direct3DTextureLL.h"
#include <assert.h>
#include <stdio.h>


struct Direct3DTextureLL* Direct3DTextureLL_Insert(unsigned long SortKey,unsigned long UniqueID,struct Direct3DTextureLL*& pDirect3DTextureHead)
//struct Direct3DTextureLL* Direct3DTextureLL_Insert(unsigned long SortKey,unsigned long UniqueID)
//void Direct3DTextureLL_Insert()
{
	struct Direct3DTextureLL* pInsertMe = NULL;
	struct Direct3DTextureLL* pThis = NULL;
	struct Direct3DTextureLL* pNext = NULL;

	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe = (struct Direct3DTextureLL *)calloc(1,sizeof(struct Direct3DTextureLL));
	pInsertMe->pNext	  = NULL;
	pInsertMe->SortKey = SortKey;
	pInsertMe->UniqueID = UniqueID;
	pInsertMe->pD3DTexture8 = NULL;

	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pDirect3DTextureHead == NULL)
	{
		pDirect3DTextureHead = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pDirect3DTextureHead->SortKey)
	{	
		pInsertMe->pNext  = pDirect3DTextureHead;
		pDirect3DTextureHead      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pDirect3DTextureHead;
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

void Direct3DTextureLL_Remove(unsigned long UniqueID,struct Direct3DTextureLL*& pDirect3DTextureHead)

{
	struct Direct3DTextureLL* pRemoveMe  = NULL;
	struct Direct3DTextureLL* pThis = NULL;
	struct Direct3DTextureLL* pNext = NULL;
	struct Direct3DTextureLL* pLast = NULL;

	pThis = pDirect3DTextureHead;
	pLast = pDirect3DTextureHead;
	while(pThis!=NULL)
	{
		if(pThis->UniqueID == UniqueID)
		//if(strcmp(pThis->pName,pName)==0)
		{//remove this item
			pRemoveMe = pThis;

			pNext = pRemoveMe->pNext;

			pThis = pThis->pNext;//update this now before we delete mem pointed to by pThis
			///////////////////////////////////
			//// Are We Removing The Head? ////
			///////////////////////////////////

			if(pRemoveMe == pDirect3DTextureHead)
			{//We are removing the head
				if(pNext==NULL)
				{//we are removing the last
					pDirect3DTextureHead = NULL;
				}
				else
				{//then pNext is new head
					pDirect3DTextureHead = pNext;
				}
			}
			else
			{//We are removing a normal item
				pLast->pNext = pNext;
			}
		    SAFE_RELEASE(pRemoveMe->pD3DTexture8);
			free(pRemoveMe);
		}
		else
		{//check next item
			pLast = pThis;
			pThis = pThis->pNext;
		}
	}
}
void Direct3DTextureLL_RemoveALL(struct Direct3DTextureLL*& pDirect3DTextureHead)
{
	struct Direct3DTextureLL* pRemoveMe  = NULL;
	struct Direct3DTextureLL* pThis = NULL;

	pThis = pDirect3DTextureHead;
	while(pThis!=NULL)
	{
		pRemoveMe = pThis;

	    SAFE_RELEASE(pRemoveMe->pD3DTexture8);

		pThis = pThis->pNext;
		free(pRemoveMe);
	}
	pDirect3DTextureHead = NULL;
}

