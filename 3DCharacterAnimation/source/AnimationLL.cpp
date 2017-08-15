// AnimationLL.cpp written by david paull
#include "stdafx.h"
#include "resource.h"
#include "globals.h"
#include <assert.h>
#include <stdio.h>
#include "Mesh.h"
#include "Bone.h"

extern struct AnimationLL* pCurrentlySelectedAnimation;

struct AnimationLL* AnimationLL_Insert(	unsigned long SortKey,
									char* pName,
									struct AnimationLL*& pAnimationHeadIn)

{
	struct AnimationLL* pAnimationHead = pAnimationHeadIn;
	struct AnimationLL* pInsertMe = NULL;
	struct AnimationLL* pThis = NULL;
	struct AnimationLL* pNext = NULL;

	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe = (struct AnimationLL *)calloc(1,sizeof(struct AnimationLL));
	sprintf(pInsertMe->pName,"%s",pName);
	pInsertMe->SortKey = SortKey;

	memset(&pInsertMe->SkeletonRootNode,0,sizeof(struct BoneH));
	sprintf(pInsertMe->SkeletonRootNode.pName,"%s Skeleton Root Node",pName);
//	Identity(pInsertMe->SkeletonRootNode.pRotationMatrix);
//	Identity(pInsertMe->SkeletonRootNode.pTranslationMatrix);
//	Identity(pInsertMe->SkeletonRootNode.pOriginalRotationMatrix);
//	Identity(pInsertMe->SkeletonRootNode.pOriginalPositionMatrix);
//	Identity(pInsertMe->SkeletonRootNode.pCombinedMatrix);
//	Identity(pInsertMe->SkeletonRootNode.pRelativeMatrix);
//	Identity(pInsertMe->SkeletonRootNode.pAbsoluteMatrix);
	pInsertMe->SkeletonRootNode.flags      = BONE_ROOTBONE;


	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pAnimationHead == NULL)
	{
		pAnimationHeadIn = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pAnimationHead->SortKey)
	{	
		pInsertMe->pNext  = pAnimationHead;
		pAnimationHeadIn      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pAnimationHead;
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

void AnimationLL_Remove(char* pName,
					struct AnimationLL*& pAnimationHeadIn)

{
	struct AnimationLL* pAnimationHead = pAnimationHeadIn;
	struct AnimationLL* pRemoveMe  = NULL;
	struct AnimationLL* pThis = NULL;
	struct AnimationLL* pNext = NULL;
	struct AnimationLL* pLast = NULL;

	pThis = pAnimationHead;
	pLast = pAnimationHead;
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

			if(pRemoveMe == pAnimationHead)
			{//We are removing the head
				if(pNext==NULL)
				{//we are removing the last
					pAnimationHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pAnimationHeadIn = pNext;
				}
			}
			else
			{//We are removing a normal item
				pLast->pNext = pNext;
			}

			//free all Animations.
			//KeyFrameLL_RemoveALL(pRemoveMe->pKeyFrameHead);
			Bone_RemoveALL(&pRemoveMe->SkeletonRootNode);

			//MainMenu MainMenu
			if(pCurrentlySelectedAnimation==pRemoveMe)
			{//if we delete current Animation, clear all pointers to the data
				pCurrentlySelectedAnimation = NULL;
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
void AnimationLL_RemoveALL(struct AnimationLL*& pAnimationHeadIn)

{
	struct AnimationLL* pRemoveMe  = NULL;
	struct AnimationLL* pThis = NULL;

	pThis = pAnimationHeadIn;
	while(pThis!=NULL)
	{
		pRemoveMe = pThis;
		pThis = pThis->pNext;

		//free all Animations.
//		KeyFrameLL_RemoveALL(pRemoveMe->pKeyFrameHead);
		Bone_RemoveALL(&pRemoveMe->SkeletonRootNode);

		//MainMenu MainMenu
		if(pCurrentlySelectedAnimation==pRemoveMe)
		{//if we delete current Animation, clear all pointers to the data
			pCurrentlySelectedAnimation = NULL;
		}

		free(pRemoveMe);
	}
	pAnimationHeadIn = NULL;
}
void AnimationLL_Sort(struct AnimationLL*& pAnimationHeadIn)
{
	struct AnimationLL* pNewAnimationHead = NULL;
	struct AnimationLL* pThis = NULL;

	pThis = pAnimationHeadIn;
	while(pThis!=NULL)
	{
		AnimationLL_Insert(	pThis->SortKey,
						pThis->pName,
						pNewAnimationHead);
		pThis = pThis->pNext;
	}
	pAnimationHeadIn = pNewAnimationHead;
}

