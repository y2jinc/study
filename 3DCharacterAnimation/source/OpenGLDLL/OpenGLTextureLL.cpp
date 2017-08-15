// OpenGLTextureLL.cpp written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
#include "stdafx.h"
#include "OpenGLTextureLL.h"
#include <assert.h>
#include <stdio.h>
#include <malloc.h>



#define SAFE_RELEASE(p){ if(p) { (p)->Release(); (p)=NULL; } }

struct OpenGLTextureLL* OpenGLTextureLL_Insert(unsigned long SortKey,unsigned long UniqueID,struct OpenGLTextureLL*& pOpenGLTextureHead)
//struct OpenGLTextureLL* OpenGLTextureLL_Insert(unsigned long SortKey,unsigned long UniqueID)
//void OpenGLTextureLL_Insert()
{
	struct OpenGLTextureLL* pInsertMe = NULL;
	struct OpenGLTextureLL* pThis = NULL;
	struct OpenGLTextureLL* pNext = NULL;

	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe = (struct OpenGLTextureLL *)calloc(1,sizeof(struct OpenGLTextureLL));
	pInsertMe->pNext	  = NULL;
	pInsertMe->SortKey = SortKey;
	pInsertMe->UniqueID = UniqueID;
	pInsertMe->TextureID[0] = NULL;
	pInsertMe->TextureID[1] = NULL;
	pInsertMe->TextureID[2] = NULL;

	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pOpenGLTextureHead == NULL)
	{
		pOpenGLTextureHead = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pOpenGLTextureHead->SortKey)
	{	
		pInsertMe->pNext  = pOpenGLTextureHead;
		pOpenGLTextureHead      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pOpenGLTextureHead;
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

void OpenGLTextureLL_Remove(unsigned long UniqueID,struct OpenGLTextureLL*& pOpenGLTextureHead)

{
	struct OpenGLTextureLL* pRemoveMe  = NULL;
	struct OpenGLTextureLL* pThis = NULL;
	struct OpenGLTextureLL* pNext = NULL;
	struct OpenGLTextureLL* pLast = NULL;

	pThis = pOpenGLTextureHead;
	pLast = pOpenGLTextureHead;
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

			if(pRemoveMe == pOpenGLTextureHead)
			{//We are removing the head
				if(pNext==NULL)
				{//we are removing the last
					pOpenGLTextureHead = NULL;
				}
				else
				{//then pNext is new head
					pOpenGLTextureHead = pNext;
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
void OpenGLTextureLL_RemoveALL(struct OpenGLTextureLL*& pOpenGLTextureHead)
{
	struct OpenGLTextureLL* pRemoveMe  = NULL;
	struct OpenGLTextureLL* pThis = NULL;

	pThis = pOpenGLTextureHead;
	while(pThis!=NULL)
	{
		pRemoveMe = pThis;


		pThis = pThis->pNext;
		free(pRemoveMe);
	}
	pOpenGLTextureHead = NULL;
}

