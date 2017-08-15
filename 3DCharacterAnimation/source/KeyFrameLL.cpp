// KeyFrameLL.cpp written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
// LinkedList info Specific to SkeletonLL
// note : A KeyFrame is inserted  based on time.
// note : A KeyFrame is retrieved based on time.
#include "stdafx.h"
#include "resource.h"
#include "Library.h"
#include <assert.h>
#include <stdio.h>
#include "KeyFrameLL.h"
#include "JointLL.h"

extern struct KeyFrameLL* pCurrentlySelectedKeyFrame;
extern bool inverseit(float* in,float* out );

//count the number of KeyFrame in a Skeletons pKeyFrameHead
unsigned long KeyFrameLL_Count(struct KeyFrameLL* pKeyFrameHeadIn)
{
	struct KeyFrameLL* pThis = pKeyFrameHeadIn;
	unsigned long num_keyframes = 0;

	while(pThis)
	{
		num_keyframes++;
		pThis = pThis->pNext;
	}
	return num_keyframes;
}


// KeyFrameLL_Get() 
// example usage:
// struct KeyFrameLL* pPrev,pThis,pNext
// if(KeyFrameLL_Get(pSkeleton->pKeyFrameHead,pPrev,pThis,pNext,anim_num,time)
bool KeyFrameLL_Get(struct KeyFrameLL*  pKeyFrameHeadIn,
					struct KeyFrameLL*& pPrev,
					struct KeyFrameLL*& pThis,
					struct KeyFrameLL*& pNext,
					unsigned long anim_num,
					unsigned long time)
{
	//debug trap
	if(time==KEYFRAME_CURRPOSE)assert(anim_num==0 && "KeyFrameLL_Get:anim num must be 0 when requesting RESTPOSE");
	if(time==KEYFRAME_RESTPOSE)assert(anim_num==0 && "KeyFrameLL_Get:anim num must be 0 when requesting RESTPOSE");

	/////////////////////
	//// Test Inputs ////
	/////////////////////

	if(pKeyFrameHeadIn==NULL)return false;

	///////////////////////////////
	//// Search for : anim_num ////
	///////////////////////////////
	
	pPrev = NULL;
	pThis = NULL;
	pNext = pKeyFrameHeadIn;

	while(	pNext && 
			pNext->anim_num < anim_num &&
			pNext->time<=KEYFRAME_MAXFRAME)
	{
		pPrev = pThis;
		pThis = pNext;
		pNext = pThis->pNext;
	}

	////////////////////////////////
	//// Continue search : time ////
	////////////////////////////////

	while(	pNext && 
			pNext->time <= time &&
			((time==KEYFRAME_CURRPOSE || time==KEYFRAME_RESTPOSE) ? (true) : (pNext->anim_num == anim_num)) &&//only check this if we are not trying to get CURRPOS, or RESTPOSE
			((time==KEYFRAME_CURRPOSE || time==KEYFRAME_RESTPOSE) ? (true) : (pNext->time<=KEYFRAME_MAXFRAME)))//dont step into CURRPOSE, and RESTPOSE, unless it is asked for
	{
		pPrev = pThis;
		pThis = pNext;
		pNext = pThis->pNext;
	}

	// found current insert position.
	if(	pThis && 
		pThis->anim_num == anim_num && 
		pThis->time == time)
	{//found the item, current pointers are correct.
		return true;//item exists
	}
	else
	{//item does not exist. insert between pThis, and pNext. create insert position between pThis, and pNext
		pPrev = pThis;
		pThis = NULL;//item tobe created, if caller wants to
		//pNext = pNext;
		return false;//item dne
	}
}


void KeyFrameLL_GetFirst(	struct KeyFrameLL*  pKeyFrameHeadIn,
							struct KeyFrameLL*& pPrev,
							struct KeyFrameLL*& pThis,
							struct KeyFrameLL*& pNext)
{
	pPrev = NULL;
	pThis = pKeyFrameHeadIn;
	pNext = (pKeyFrameHeadIn) ? (pKeyFrameHeadIn->pNext) : (NULL);
}

//return last valid user keyframe
//currpose, and restpose are always the last 2 KF
void KeyFrameLL_GetLast(struct KeyFrameLL*  pKeyFrameHeadIn,
						struct KeyFrameLL*& pPrev,
						struct KeyFrameLL*& pThis,
						struct KeyFrameLL*& pNext)
{
	pPrev = NULL;
	pThis = NULL;
	pNext = pKeyFrameHeadIn;

	while(pNext->time!=KEYFRAME_CURRPOSE)
	{
		pPrev = pThis;
		pThis = pNext;
		pNext = pThis->pNext;
	}
}


//RestPose is assumed to be the last keyframe.
void KeyFrameLL_GetRestPose(
						struct KeyFrameLL*  pKeyFrameHeadIn,
						struct KeyFrameLL*& pPrev,
						struct KeyFrameLL*& pThis,
						struct KeyFrameLL*& pNext)
{
	pPrev = NULL;
	pThis = NULL;
	pNext = pKeyFrameHeadIn;

	while(pNext)
	{
		pPrev = pThis;
		pThis = pNext;
		pNext = pThis->pNext;
	}
}

// KeyFrameLL_Insert()
// time (0)-(0xffffffff)
// pKeyFrameHeadIn = pSkeleton->pKeyFrameHead
struct KeyFrameLL* KeyFrameLL_Insert(struct KeyFrameLL*& pKeyFrameHeadIn,unsigned long anim_num,unsigned long time)
{
	struct KeyFrameLL* pInsertMe = NULL;
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;

	////////////////////////////////////////////////
	//// Check To See If KeyFrame Alredy Exists ////
	////////////////////////////////////////////////

	if(KeyFrameLL_Get(pKeyFrameHeadIn,pPrev,pThis,pNext,anim_num,time))
	{
		return pThis;
	}

	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////

	pInsertMe = (struct KeyFrameLL *)calloc(1,sizeof(struct KeyFrameLL));
	pInsertMe->anim_num = anim_num;
	pInsertMe->time = time;

	////////////////////////////////////////////////////////
	//// Is This The First Item Added to an empty list? ////
	////////////////////////////////////////////////////////

	if(pKeyFrameHeadIn == NULL)
	{
		pKeyFrameHeadIn = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->anim_num == pKeyFrameHeadIn->anim_num)
	{	
		if(pInsertMe->time <= pKeyFrameHeadIn->time)
		{	
			pInsertMe->pNext  = pKeyFrameHeadIn;
			pKeyFrameHeadIn   = pInsertMe;
			return pInsertMe;
		}
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pPrev->pNext = pInsertMe;
	pInsertMe->pNext = pNext;
	return pInsertMe;
}



//void KeyFrameLL_New(
struct KeyFrameLL* KeyFrameLL_New(struct KeyFrameLL*& pKeyFrameHeadIn,unsigned long anim_num,unsigned long time)
{
	struct KeyFrameLL* pKeyFrame = NULL;

	pKeyFrame = KeyFrameLL_Insert(pKeyFrameHeadIn,anim_num,time);
	return pKeyFrame;
}

void KeyFrameLL_Sort(struct KeyFrameLL*& pKeyFrameHeadIn)
{
	struct KeyFrameLL* pNewKeyFrameHead = NULL;
	struct KeyFrameLL* pThis = NULL;

	pThis = pKeyFrameHeadIn;
	while(pThis!=NULL)
	{
		KeyFrameLL_Insert(pNewKeyFrameHead,pThis->anim_num,pThis->time);
		pThis = pThis->pNext;
	}

	KeyFrameLL_CloseALL(pKeyFrameHeadIn);//close original "incorrect order" linked list

	pKeyFrameHeadIn = pNewKeyFrameHead;//assign new linked list to head pointer
}
/*
struct KeyFrameLL* KeyFrameLL_SelectPrev(	struct KeyFrameLL*  pKeyFrameHeadIn,
											unsigned long anim_num,
											unsigned long time)
{
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;

	if(pKeyFrameHeadIn==NULL)return NULL;

	if(KeyFrameLL_Get(pKeyFrameHeadIn,pPrev,pThis,pNext,anim_num,time))
	{//kf exists
		if(pPrev)
		{
			return pPrev;
		}
		else
		{//it is the first item in list, so wrap around, and return last item
			KeyFrameLL_GetLast(pKeyFrameHeadIn,pPrev,pThis,pNext);
			return pThis;
		}
	}
	else
	{//kf item DNE. return head.
		return pKeyFrameHeadIn;
	}
}
struct KeyFrameLL* KeyFrameLL_SelectNext(	struct KeyFrameLL*  pKeyFrameHeadIn,
											unsigned long anim_num,
											unsigned long time)
{
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;

	if(pKeyFrameHeadIn==NULL)return NULL;

	if(KeyFrameLL_Get(pKeyFrameHeadIn,pPrev,pThis,pNext,anim_num,time))
	{//kf exists
		if(pNext)
		{
			return pNext;
		}
		else
		{//last item in list. return head
			return pKeyFrameHeadIn;
		}
	}
	else
	{//kf item DNE. return head.
		return pKeyFrameHeadIn;
	}
}
*/
// KeyFrameLL_Close() performs LL Remove
bool KeyFrameLL_Close(struct KeyFrameLL*& pKeyFrameHeadIn,unsigned long anim_num,unsigned long time)
{
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;//CloseME
	struct KeyFrameLL* pNext = NULL;

	if(!KeyFrameLL_Get(pKeyFrameHeadIn,pPrev,pThis,pNext,anim_num,time))
	{//a KeyFrame with that time not found
		return false;
	}
	///////////////////////////////////
	//// Are We Removing The Head? ////
	///////////////////////////////////

	if(pThis == pKeyFrameHeadIn)
	{//We are removing the head
		if(pNext==NULL)
		{//we are removing the last
			pKeyFrameHeadIn = NULL;
		}
		else
		{//then pNext is new head
			pKeyFrameHeadIn = pNext;
		}
	}
	else
	{//We are removing a normal item
		assert(pPrev!=NULL);//how could pPrev==null, and pCloseMe not be first item!?
		pPrev->pNext = pNext;
	}

	//MainMenu MainMenu
	if(pCurrentlySelectedKeyFrame==pThis)
	{//if we delete current KeyFrame, clear all pointers to the data
		pCurrentlySelectedKeyFrame = pNext;
	}

	JointLL_CloseALL(pThis->pJointHead);

	SAFE_FREE(pThis);
	return true;
}


// KeyFrameLL_CloseALL() performs LL RemoveALL
void KeyFrameLL_CloseALL(struct KeyFrameLL*& pKeyFrameHeadIn)
{
	struct KeyFrameLL* pCloseMe  = NULL;
	struct KeyFrameLL* pThis = NULL;

	pThis = pKeyFrameHeadIn;
	while(pThis!=NULL)
	{
		pCloseMe = pThis;
		pThis    = pThis->pNext;//increment BEFORE destroying pCloseMe
		KeyFrameLL_Close(pKeyFrameHeadIn,pCloseMe->anim_num,pCloseMe->time);
	}
	pKeyFrameHeadIn = NULL;
}






/*
//render.cpp 1247
//void RenderMesh(struct SceneLL* pScene,
//				struct ObjectLL* pObject,
//				struct MeshLL* pMesh)
//{
//	float pFinalModelMatrix[16];
//	if(	pObject==NULL ||
//		System_CurrentTime==KEYFRAME_RESTPOSE ||
//		pMesh->RenderFlags & RENDER_TRANSFORMED)
//	{	//use identity matrix in place of pObject->pMatrix
//		//if pObject==NULL ::  then the mesh shouldnt have a parent. thus no pObj->pMat.
//		//if System_CurrentTime==KEYFRAME_RESTPOSE :: restpose assumes identity
//		//if TRANSFORMED :: pObject->pMatrix is applied to Skeleton, and therefore in resulting world vertices.
//		//if TRANSFORMED :: pMesh->pMatrix is guaranteed to be Identity
//		API_RenderMesh(	pMesh,
//						pMesh->pMatrix,
//						pSystem_ViewMatrix,
//						pSystem_ProjMatrix,
//						pSystem_OrthoMatrix);
//	}
//	else
//	{
//		MatrixMul(pFinalModelMatrix,pObject->pMatrix,pMesh->pMatrix);
//		API_RenderMesh(	pMesh,
//						pFinalModelMatrix,
//						pSystem_ViewMatrix,
//						pSystem_ProjMatrix,
//						pSystem_OrthoMatrix);
//	}
//}


*/




/////////////////////////////
//// KeyFrame_Slerp Info ////
/////////////////////////////
//// Assumed that pF!=pA ie they have diff times. this is inforced at NewKeyFrame time.
//// Assumed that pF!=pB
//// Assumed pA->time<pB->time
//// time contains time between A, and B for pFinal.
//// pFinal may be a newly added KeyFrame, or RestPose. 
////    time==pFinal->time if not RestPose. (RP time is -1, and cant be altered)
void KeyFrame_Slerp(struct KeyFrameLL* pF,//pFinal
					struct KeyFrameLL* pA,
					struct KeyFrameLL* pB,
					unsigned long time)

{
	aQUATERNION A,B,F;
	struct JointLL* pF_Joint = NULL;
	struct JointLL* pA_Joint = NULL;
	struct JointLL* pB_Joint = NULL;

	assert(pA->time<pB->time);
	float t1 = (float)(time - pA->time);
	float t2 = (float)(pB->time - pA->time);
	float t  = t1/t2;

	pF_Joint = pF->pJointHead;
	pA_Joint = pA->pJointHead;
	pB_Joint = pB->pJointHead;
	while(pF_Joint)
	{
		//Interpolate Rotation
		CreateQuaternionFromMatrix(A,pA_Joint->pRelativeMatrix);
		CreateQuaternionFromMatrix(B,pB_Joint->pRelativeMatrix);
		QuaternionSlerp(F,A,B,t);
		CreateMatrixFromQuaternion(pF_Joint->pRelativeMatrix,F);

		//Interpolate Position
		pF_Joint->pRelativeMatrix[12] = Lerp(t,pA_Joint->pRelativeMatrix[12],pB_Joint->pRelativeMatrix[12]);				
		pF_Joint->pRelativeMatrix[13] = Lerp(t,pA_Joint->pRelativeMatrix[13],pB_Joint->pRelativeMatrix[13]);				
		pF_Joint->pRelativeMatrix[14] = Lerp(t,pA_Joint->pRelativeMatrix[14],pB_Joint->pRelativeMatrix[14]);				

		pF_Joint = pF_Joint->pNext;
		pA_Joint = pA_Joint->pNext;
		pB_Joint = pB_Joint->pNext;
	}
}






//// Jrel = Joint Relative Matrix
//// Jabs = Joint Absolute Matrix
//// Prel = Joints Parents Relative Matrix
//// Pabs = Joints Parents Absolute Matrix
// T = (A^-1)*B
// Jr = (Pa^-1)*Ja

void KeyFrame_TransformJoints_AbsoluteToRelative(struct KeyFrameLL* pKeyFrame, float* pActorMatrix)
{
	struct JointLL* pJoint  = NULL;
	struct JointLL* pParent = NULL;
	float pInverseParentAbsoluteMatrix[16];

	pJoint = pKeyFrame->pJointHead;
	while(pJoint)
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
		pJoint = pJoint->pNext;
	}
}

void KeyFrame_TransformJoints_RelativeToAbsolute(struct KeyFrameLL* pKeyFrame, float* pActorMatrix)
{
	struct JointLL* pJoint  = NULL;
	struct JointLL* pParent = NULL;

	pJoint = pKeyFrame->pJointHead;
	while(pJoint)
	{
		pParent = pJoint->pParent;

		if(strcmp(pJoint->pInfoItem->pParentName,"null")==0)//change to long compare.
		{//top level in hiearchy
			//Jabs = Identity*Jrel
			//memcpy(pJoint->pAbsoluteMatrix,pJoint->pRelativeMatrix,16*sizeof(float));
			//Jabs = pActorMat*Jrel  
			MatrixMul(	pJoint->pAbsoluteMatrix,
						pJoint->pRelativeMatrix,
						pActorMatrix);
		}
		else
		{

			//Jabs = Pabs*Jrel  
			MatrixMul(	pJoint->pAbsoluteMatrix,
						pJoint->pRelativeMatrix,
						pParent->pAbsoluteMatrix);
		}
		pJoint = pJoint->pNext;
	}
}


void KeyFrame_Copy(struct KeyFrameLL* pKeyFrame_Src,struct KeyFrameLL* pKeyFrame_Dst)
{
	struct JointLL* pJoint_Src  = NULL;
	struct JointLL* pJoint_Dst  = NULL;

	pJoint_Src = pKeyFrame_Src->pJointHead;
	pJoint_Dst = pKeyFrame_Dst->pJointHead;
	while(pJoint_Src && pJoint_Dst) 
	{
		memcpy(	pJoint_Dst->pRelativeMatrix,
				pJoint_Src->pRelativeMatrix,
				16*sizeof(float));
		memcpy(	pJoint_Dst->pAbsoluteMatrix,
				pJoint_Src->pAbsoluteMatrix,
				16*sizeof(float));
		pJoint_Src = pJoint_Src->pNext;
		pJoint_Dst = pJoint_Dst->pNext;
	}
}














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