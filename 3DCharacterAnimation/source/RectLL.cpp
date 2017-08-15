// RectLL.cpp written by david paull
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

extern float Render_WindowWidth;
extern float Render_WindowHeight;
//SortKey
//BEVELEDGE
//applicaation +12     0
//scene        +8	   1
//object       +4	   2
//mesh         +0	   3
//                     4
//console              5
struct RectLL* RectLL_Insert(  	unsigned long SortKey,
								long ColorIndex,
								float left,
								float right,
								float top,
								float bottom,
								struct RectLL*& pRectHeadIn)
{
	struct RectLL* pRectHead = pRectHeadIn;
	struct RectLL* pInsertMe = NULL;
	struct RectLL* pThis = NULL;
	struct RectLL* pNext = NULL;
	fRECT Clip,ClippedRect,CurrentRect;

	//clip input to screen first	
	Clip.left   = 0.0;
	Clip.right  = Render_WindowWidth; 
	Clip.top    = 0.0;
	Clip.bottom = Render_WindowHeight;

	CurrentRect.left   = left  ;
	CurrentRect.right  = right ;
	CurrentRect.top    = top   ;
	CurrentRect.bottom = bottom;

	ClippedRect.left   = left  ;
	ClippedRect.right  = right ;
	ClippedRect.top    = top   ;
	ClippedRect.bottom = bottom;


	ClippedRect = ClipRectToRect(CurrentRect,Clip);
	if((ClippedRect.left   == 0.0) &&
	   (ClippedRect.right  == 0.0) &&
	   (ClippedRect.top    == 0.0) &&
	   (ClippedRect.bottom == 0.0))
	{
		return NULL;
	}



	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe = (struct RectLL *)calloc(1,sizeof(struct RectLL));
	pInsertMe->pNext	  = NULL;

	pInsertMe->SortKey = SortKey;
	pInsertMe->ColorIndex = ColorIndex;//used to select uv coords in icon.bmp
	pInsertMe->left    = ClippedRect.left;
	pInsertMe->right   = ClippedRect.right;
	pInsertMe->top     = ClippedRect.top;
	pInsertMe->bottom  = ClippedRect.bottom;

	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pRectHead == NULL)
	{
		pRectHeadIn = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pRectHead->SortKey)
	{	
		pInsertMe->pNext  = pRectHead;
		pRectHeadIn      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pRectHead;
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

void RectLL_Remove( struct RectLL* pRect,
					struct RectLL*& pRectHeadIn)
{
	struct RectLL* pRectHead = pRectHeadIn;
	struct RectLL* pRemoveMe  = NULL;
	struct RectLL* pThis = NULL;
	struct RectLL* pNext = NULL;
	struct RectLL* pLast = NULL;

	pThis = pRectHead;
	pLast = pRectHead;
	while(pThis!=NULL)
	{
		//if(pThis->UniqueID == UniqueID)
		if(pThis == pRect)
		{//remove this item
			pRemoveMe = pThis;

			pNext = pRemoveMe->pNext;

			pThis = pThis->pNext;//update this now before we delete mem pointed to by pThis
			///////////////////////////////////
			//// Are We Removing The Head? ////
			///////////////////////////////////

			if(pRemoveMe == pRectHead)
			{//We are removing the head
				if(pNext==NULL)
				{//we are removing the last
					pRectHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pRectHeadIn = pNext;
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
void RectLL_RemoveALL(struct RectLL*& pRectHeadIn)

{
	struct RectLL* pRemoveMe  = NULL;
	struct RectLL* pThis = NULL;

	pThis = pRectHeadIn;
	while(pThis!=NULL)
	{
		pRemoveMe = pThis;
		pThis = pThis->pNext;
		free(pRemoveMe);
	}
	pRectHeadIn = NULL;
}
