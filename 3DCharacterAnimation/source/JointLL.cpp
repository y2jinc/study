//todo : allocate pInfoItem for restpose DOH!!!!


//JointLL.cpp written by david paull.
// ::note:: Joints_()  functions operate on all joints in all keyframes. 
// ::note:: JointLL_() functions operate only on a single joint, single keyframe's JointLL
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
// ::note:: struct JointLL defined in Globals.h
// ::note:: JointLL items are hiearchialy sorted by parent name, and name.

#include "stdafx.h"
#include "resource.h"
#include "Library.h"
#include "FieldLL.h"
#include <assert.h>
#include <stdio.h>

extern struct JointLL* pCurrentlySelectedJoint;

//count the number of joints in a keyframes pJointHead
unsigned long JointLL_Count(struct JointLL* pJointHeadIn)
{
	struct JointLL* pThis = pJointHeadIn;
	unsigned long num_joints = 0;

	while(pThis)
	{
		num_joints++;
		pThis = pThis->pNext;
	}
	return num_joints;
}


bool JointLL_Get(	struct JointLL*  pJointHeadIn,//can be NULL
					struct JointLL*& pPrevIn,
					struct JointLL*& pThisIn,
					struct JointLL*& pNextIn,
					char* pName)
{
	/////////////////////
	//// Test Inputs ////
	/////////////////////

	if(pName==NULL)return false;

	// top level joints have "null" as their parents.
	// The get function might be called to return "null"
	// the "null" slot doesnt actually exist, but its position is the item before before pJointHeadIn
	if(strcmp(pName,"null")==0)
	{//see JointLL_Insert
		pPrevIn = NULL;
		pThisIn = NULL;
		pNextIn = pJointHeadIn;
		return true;
	}
	
	//////////////////////////
	//// Search for pName ////
	//////////////////////////

	pPrevIn = NULL;
	pThisIn = pJointHeadIn;
	pNextIn = (pThisIn) ? (pThisIn->pNext) : (NULL);

	while(pThisIn)
	{
		if(strcmp(pThisIn->pInfoItem->pName,pName)==0)//if you crash here, you need to hook up pInfoItem after adding joint. a joint doesnt have pInfoItem 
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



//finds first child only.
//this allows a joint to know where the endpoint of its "bone" is
//currently this function is only used in InitWeights
struct JointLL* JointLL_Get_Child(	struct JointLL*  pJointHeadIn,
									char* pParentName)
{
	struct JointLL* pJoint = NULL;
	
	/////////////////////
	//// Test Inputs ////
	/////////////////////

	if(pParentName==NULL)return NULL;

	////////////////////////////////
	//// Search for pParentName ////
	////////////////////////////////

	pJoint = pJointHeadIn;
	while(pJoint)
	{
		if(strcmp(pJoint->pInfoItem->pParentName,pParentName)==0)
		{//found pname. pointers are correct
			return pJoint;
		}
		pJoint = pJoint->pNext;
	}
	return NULL;
}

void JointLL_GetFirst(	struct JointLL*  pJointHeadIn,
							struct JointLL*& pPrev,
							struct JointLL*& pThis,
							struct JointLL*& pNext)
{
	pPrev = NULL;
	pThis = pJointHeadIn;
	pNext = (pJointHeadIn) ? (pJointHeadIn->pNext) : (NULL);
}

void JointLL_GetLast(struct JointLL*  pJointHeadIn,
						struct JointLL*& pPrev,
						struct JointLL*& pThis,
						struct JointLL*& pNext)
{
	pPrev = NULL;
	pThis = NULL;
	pNext = pJointHeadIn;

	while(pNext)
	{
		pPrev = pThis;
		pThis = pNext;
		pNext = pThis->pNext;
	}
}




struct JointLL* JointLL_SelectNext(	struct JointLL*  pJointHeadIn,char* pNameCurrentJoint)
{
	struct JointLL* pPrev = NULL;
	struct JointLL* pThis = NULL;
	struct JointLL* pNext = NULL;

	if(pJointHeadIn==NULL)return NULL;
	if(pNameCurrentJoint==NULL)return pJointHeadIn;

	if(JointLL_Get(pJointHeadIn,pPrev,pThis,pNext,pNameCurrentJoint))
	{
		if(pNext)
		{
			return pNext;
		}
		else
		{//last item in list. return head
			return pJointHeadIn;
		}
	}
	else
	{//item DNE. return head.
		return pJointHeadIn;
	}
}




////////////////////////////////////////////////////////////////////////////
//// JointLL is a "Hiearchy LL"
////
//// Hiearchy LL : FAIL IF PARENT NOT FOUND, except for "" NULL parent
//// Hiearchy LL : FAIL IF NAME EXISTS, or if name "" 
//// Hiearchy LL : APPEND NEW ITEM TO END OF LL
//// Hiearchy LL : lacks sort key. so always append. no sort function.
//// Hiearchy LL : pName HAS TO BE UNIQUE.
//// Hiearchy LL : if top level pParentJointName == "null"
//// Set bCreateInfoItem=true if KF that owns joint is RestPose
////////////////////////////////////////////////////////////////////////////
struct JointLL* JointLL_Append(	struct JointLL*& pJointHeadIn,//pObject->pJointHead
								char* pName,//Joint name
								char* pParentName,//name of Joint that holds joint that holds pParentJointName
								bool bCreateInfoItem)//this is false during a sort(). true when creating normally.
{
	struct JointLL* pAppendMe = NULL;
	struct JointLL* pParent   = NULL;
	struct JointLL* pPrev	 = NULL;
	struct JointLL* pThis	 = NULL;
	struct JointLL* pNext	 = NULL;

	if(pName==NULL)return NULL;
	if(pParentName==NULL)return NULL;
	
	///////////////////////////////////////
	//// Check To See If Parent Exists ////
	///////////////////////////////////////

	if(!JointLL_Get(pJointHeadIn,pPrev,pParent,pNext,pParentName))
	{
		//parent not found
		return NULL;
	}

	//////////////////////////////////////////////////
	//// Check To See If Joint Alredy Exists      ////
	////  	also pPrev is used later              ////
	////  	if pName DNE, pPrev points to last item///
	//////////////////////////////////////////////////
	
	if(JointLL_Get(pJointHeadIn,pPrev,pThis,pNext,pName))
	{
		return pThis;
	}

	/////////////////////////////////////////////////
	//// Allocate Memory For New JointLL Item ////
	/////////////////////////////////////////////////

	pAppendMe = (struct JointLL *)calloc(1,sizeof(struct JointLL));

	////////////////////////////////////////
	//// Find Lowest Available UniqueID ////
	////////////////////////////////////////
	long LowestUniqueID = 0;
	pThis = pJointHeadIn;
	while(pThis!=NULL && pThis->UniqueID==LowestUniqueID)
	{
		LowestUniqueID++;
		pThis = pThis->pNext;
	}
	pAppendMe->UniqueID = LowestUniqueID;

	//////////////////////////////
	//// Set Idenetity Matrix ////
	//////////////////////////////

	#ifdef USE_QUATERNIONS
	RotationQuaternionIdentity(pAppendMe->Relative_Rotation);
	PositionQuaternionIdentity(pAppendMe->Relative_Position);
	RotationQuaternionIdentity(pAppendMe->Absolute_Rotation);
	PositionQuaternionIdentity(pAppendMe->Absolute_Position);
	#endif//USE_QUATERNIONS
	
	Identity(pAppendMe->pRelativeMatrix);
	Identity(pAppendMe->pAbsoluteMatrix);

	if(bCreateInfoItem)
	{
		pAppendMe->b_pInfoItemReal = true;
		pAppendMe->pInfoItem = (struct JointInfoItem *)calloc(1,sizeof(struct JointInfoItem));
		sprintf(pAppendMe->pInfoItem->pName,"%s",pName);
		sprintf(pAppendMe->pInfoItem->pParentName,"%s",pParentName);
	}

	///////////////////////////////////////////////////////
	//// Initialize Pointer To Parent Joint in same LL ////
	///////////////////////////////////////////////////////

	pAppendMe->pParent = pParent;



	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pJointHeadIn == NULL)
	{
		pJointHeadIn = pAppendMe;
		return pAppendMe;
	}

	//////////////////////////
	//// Append pAppendMe ////
	//////////////////////////

	assert(pPrev->pNext==NULL);
	pPrev->pNext = pAppendMe;
	pAppendMe->pNext = NULL;
	return pAppendMe;
}

//void JointLL_New(
struct JointLL* JointLL_New(struct JointLL*& pJointHeadIn,
							char* pName,//new Joint name
							char* pParentName,
							bool bCreateInfoItem)//this is false during a sort(). true when creating normally.
{
	struct JointLL* pJoint = NULL;

	if(pName==NULL)return NULL;

	pJoint = JointLL_Append(pJointHeadIn,pName,pParentName,bCreateInfoItem);//true always???
	return pJoint;
}

void JointLL_Edit(	struct JointLL* pJoint,
					char* pName,
					char* pParentName,
					char* pSkeletonName)
{
	if(strcmp(pName,"")==0)
	{
		assert(0 && "joint must have name");
	}
	if(strcmp(pParentName,"")==0)
	{//"null" is acceptable 
		assert(0 && "parentjoint must have name");
	}

	if(pJoint && pJoint->pInfoItem)
	{
		sprintf(pJoint->pInfoItem->pName,"%s",pName);
		sprintf(pJoint->pInfoItem->pParentName,"%s",pParentName);
//		sprintf(pJoint->pInfoItem->pSkeletonName,"%s",pSkeletonName);
	}
}

// JointLL_Close() performs LL Remove
// you can *optionally* closes children. 
// 		this is because only the RP KF actually allocated it.
// 		the append function has the correcponding CreateInfoItem
//
// also sorting closes an item after copying pointers to the children
bool JointLL_Close(struct JointLL*& pJointHeadIn,char* pName)
{
	struct JointLL* pPrev    = NULL;
	struct JointLL* pCloseMe = NULL;
	struct JointLL* pNext = NULL;

	if(!JointLL_Get(pJointHeadIn,pPrev,pCloseMe,pNext,pName))
	{//a Joint with that name not found
		return false;
	}

	///////////////////////////////////
	//// Are We Removing The Head? ////
	///////////////////////////////////

	if(pCloseMe == pJointHeadIn)
	{//We are removing the head
		if(pNext==NULL)
		{//we are removing the last item in list
			pJointHeadIn = NULL;
		}
		else
		{//then pNext is new head
			pJointHeadIn = pNext;
		}
	}
	else
	{//We are removing a normal item
		assert(pPrev!=NULL);//how could pPrev==null, and pCloseMe not be first item!?
		pPrev->pNext = pNext;
	}

	//MainMenu MainMenu
	if(pCurrentlySelectedJoint==pCloseMe)
	{//if we delete current Joint, clear all pointers to the data
		pCurrentlySelectedJoint = pNext;
	}

	if(pCloseMe->b_pInfoItemReal)SAFE_FREE(pCloseMe->pInfoItem);
	SAFE_FREE(pCloseMe);
	return true;
}


// JointLL_CloseALL() performs LL RemoveALL
void JointLL_CloseALL(struct JointLL*& pJointHeadIn)
{
	struct JointLL* pCloseMe  = NULL;
	struct JointLL* pThis = NULL;

	pThis = pJointHeadIn;
	while(pThis!=NULL)
	{
		pCloseMe = pThis;
		pThis    = pThis->pNext;//increment BEFORE destroying pCloseMe
		JointLL_Close(pJointHeadIn,pCloseMe->pInfoItem->pName);
	}
	pJointHeadIn = NULL;
}




//mouse rotate joint
void Joint_TransformJoint_AbsoluteToRelative(struct JointLL* pJoint, float* pActorMatrix)
{
	struct JointLL* pParent = NULL;
	float pInverseParentAbsoluteMatrix[16];

	if(pJoint)
	{
		pParent = pJoint->pParent;

		if(strcmp(pJoint->pInfoItem->pParentName,"null")==0)//change to long compare.
		{//top level in hiearchy
			//Jrel = Jabs
			//memcpy(pJoint->pRelativeMatrix,pJoint->pAbsoluteMatrix,16*sizeof(float));
			InverseMatrix(	pInverseParentAbsoluteMatrix,
							pActorMatrix);

			MatrixMul(	pJoint->pRelativeMatrix,
						pJoint->pAbsoluteMatrix,
						pInverseParentAbsoluteMatrix);

			OrthonormalizeOrientation(pJoint->pRelativeMatrix);
		}
		else
		{
			InverseMatrix(	pInverseParentAbsoluteMatrix,
							pParent->pAbsoluteMatrix);

			MatrixMul(	pJoint->pRelativeMatrix,
						pJoint->pAbsoluteMatrix,
						pInverseParentAbsoluteMatrix);

			OrthonormalizeOrientation(pJoint->pRelativeMatrix);
		}
	}
}



// JointLL_
// For Every Joint in real LL, 
// 		if it has pFindMyChildren as parent
// 		then append it to new list.
/*
void JointLL_HiearchySortLL(struct JointLL*& pJointHeadIn)
{
	struct JointLL* pNewJointHead = NULL;
	struct JointLL* pCloseMe = NULL;
	struct JointLL* pThis = NULL;
	struct JointLL* pFindMyChildren = NULL;

	//to make code cleaner, create a dummy head.
	pFindMyChildren = JointLL_Append(pNewJointHead,"null","null",false);

	while(pFindMyChildren)
	{
		pThis = pJointHeadIn;
		while(pThis)
		{
			if(strcmp(pThis->pParentName,pFindMyChildren->pName)==0)
			{
				pJoint = JointLL_Append(pNewJointHead,pThis->pName,pThis->pParentName);
				memcpy(pJoint,pThis,sizeof(JointLL));
			    pJoint->pNext = NULL;//its being appended to list, so null. previous pointer value invalid now.
			}
			pThis = pNext;
		}
		pFindMyChildren = pFindMyChildren->pNext;
	}
	//close dummy head. first item in LL
	pCloseMe = pNewJointHead;
	pNewJointHead = pNewJointHead->pNext;
	SAFE_FREE(pCloseMe);
	JointLL_CloseALL(pJointHeadIn,false);//the keyframes are now pointed to by the new sorted LL, so dont close keyframes, just old JointLL. 

	
	pJointHeadIn = pNewJointHead;
}

*/




/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
