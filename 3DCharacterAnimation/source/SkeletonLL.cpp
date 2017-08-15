//SkeletonLL.cpp written by david paull.
// Copyright (c) 2002 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
// LinkedList info Specific to SkeletonLL
// note : A Skeleton is inserted  based on parentnamre, and name. Item inserted after their parents, at the end of the parents list of children.
// note : A Skeleton is retrieved based on its name.
#include "stdafx.h"
#include "resource.h"
#include "Library.h"
#include "FileAccess.h"
#include <assert.h>
#include <stdio.h>
#include "KeyFrameLL.h"
#include "JointLL.h"

extern struct SkeletonLL* pCurrentlySelectedSkeleton;
extern char* gp_TextBuffer3;

void Skeleton_SaveKeyFrameData(HANDLE fH, struct KeyFrameLL* pKeyFrame);

//count the number of skeletons in objects pSkeletonHead
unsigned long SkeletonLL_Count(struct SkeletonLL* pSkeletonHeadIn)
{
	struct SkeletonLL* pThis = pSkeletonHeadIn;
	unsigned long num_skeletons = 0;

	while(pThis)
	{
		num_skeletons++;
		pThis = pThis->pNext;
	}
	return num_skeletons;
}


// SkeletonLL_Get() does not care about the hiearchial structure.
// pName is guaranteed to be unique
// bool SkeletonLL_Get(struct SkeletonLL*  pSkeletonHeadIn,

// example usage:
// struct SkeletonLL* pPrev,pThis,pNext
// if(SkeletonLL_Get(pObject->pSkeletonHead,pPrev,pThis,pNext,pName)


bool SkeletonLL_Get(struct SkeletonLL*  pSkeletonHeadIn,
					struct SkeletonLL*& pPrevIn,
					struct SkeletonLL*& pThisIn,
					struct SkeletonLL*& pNextIn,
					char* pName)
{
	/////////////////////
	//// Test Inputs ////
	/////////////////////

	if(pName==NULL)return false;

	// top level joints have "null" as their parents.
	// The get function might be called to return "null"
	// the "null" slot doesnt actually exist, but its position is the item before before pSkeletonHeadIn
	if(strcmp(pName,"null")==0)
	{//see SkeletonLL_Insert
		pPrevIn = NULL;
		pThisIn = NULL;
		pNextIn = pSkeletonHeadIn;
		return true;
	}
	
	//////////////////////////
	//// Search for pName ////
	//////////////////////////

	pPrevIn = NULL;
	pThisIn = pSkeletonHeadIn;
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

void SkeletonLL_GetFirst(	struct SkeletonLL*  pSkeletonHeadIn,
						struct SkeletonLL*& pPrev,
						struct SkeletonLL*& pThis,
						struct SkeletonLL*& pNext)
{
	pPrev = NULL;
	pThis = pSkeletonHeadIn;
	pNext = (pSkeletonHeadIn) ? (pSkeletonHeadIn->pNext) : (NULL);
}

void SkeletonLL_GetLast(	struct SkeletonLL*  pSkeletonHeadIn,
						struct SkeletonLL*& pPrev,
						struct SkeletonLL*& pThis,
						struct SkeletonLL*& pNext)
{
	pPrev = NULL;
	pThis = NULL;
	pNext = pSkeletonHeadIn;

	while(pNext)
	{
		pPrev = pThis;
		pThis = pNext;
		pNext = pThis->pNext;
	}
}


struct SkeletonLL* SkeletonLL_SelectNext(	struct SkeletonLL* pSkeletonHeadIn,
											char* pNameCurrentSkeleton)
{
	struct SkeletonLL* pPrev = NULL;
	struct SkeletonLL* pThis = NULL;
	struct SkeletonLL* pNext = NULL;

	if(SkeletonLL_Get(pSkeletonHeadIn,pPrev,pThis,pNext,pNameCurrentSkeleton))
	{
		if(pNext)
		{
			return pNext;
		}
		else
		{//last item in list. return head
			return pSkeletonHeadIn;
		}
	}
	else
	{//item DNE. return head.
		return pSkeletonHeadIn;
	}
}


////////////////////////////////////////////////////////////////////////////
//// SkeletonLL is a "Hiearchy LL"
////
//// Hiearchy LL : FAIL IF PARENT NOT FOUND, except for "" NULL parent
//// Hiearchy LL : FAIL IF NAME EXISTS, or if name "" 
//// Hiearchy LL : APPEND NEW ITEM TO END OF LL
//// Hiearchy LL : lacks sort key. so always append. no sort function.
//// Hiearchy LL : pName HAS TO BE UNIQUE.
//// Hiearchy LL : if top level pParentSkeletonName == "null"
////////////////////////////////////////////////////////////////////////////
struct SkeletonLL* SkeletonLL_Append(	struct SkeletonLL*& pSkeletonHeadIn,//pObject->pSkeletonHead
										char* pName,//skeleton name
										char* pParentName)//name of skeleton that holds joint that holds pParentJointName
{
	struct SkeletonLL* pAppendMe = NULL;
	struct SkeletonLL* pParent   = NULL;
	struct SkeletonLL* pPrev	 = NULL;
	struct SkeletonLL* pThis	 = NULL;
	struct SkeletonLL* pNext	 = NULL;

	if(pName==NULL)return NULL;
	if(pParentName==NULL)return NULL;
	
	///////////////////////////////////////
	//// Check To See If Parent Exists ////
	///////////////////////////////////////

	if(!SkeletonLL_Get(pSkeletonHeadIn,pPrev,pParent,pNext,pParentName))
	{
		//parent not found
		return NULL;
	}

	//////////////////////////////////////////////////
	//// Check To See If Skeleton Alredy Exists   ////
	////  	also pPrev is used later              ////
	////  	if pName DNE, pPrev points to last item///
	//////////////////////////////////////////////////
	
	if(SkeletonLL_Get(pSkeletonHeadIn,pPrev,pThis,pNext,pName))
	{
		return pThis;
	}

	/////////////////////////////////////////////////
	//// Allocate Memory For New SkeletonLL Item ////
	/////////////////////////////////////////////////

	pAppendMe = (struct SkeletonLL *)calloc(1,sizeof(struct SkeletonLL));
	sprintf(pAppendMe->pName,"%s",pName);
	sprintf(pAppendMe->pParentName,"%s",pParentName);

	PositionQuaternionIdentity(pAppendMe->Absolute_Position);
	RotationQuaternionIdentity(pAppendMe->Absolute_Rotation);

	//////////////////////////////////////////////////////
	//// Allocate Linked List Of KeyFrames            ////
	////	Every Skeleton gets 2 permanent keyframes ////
	//////////////////////////////////////////////////////
	
	pAppendMe->pRestPose = KeyFrameLL_New(pAppendMe->pKeyFrameHead,KEYFRAME_ANIMNUM0,KEYFRAME_RESTPOSE);
	pAppendMe->pCurrPose = KeyFrameLL_New(pAppendMe->pKeyFrameHead,KEYFRAME_ANIMNUM0,KEYFRAME_CURRPOSE);

	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pSkeletonHeadIn == NULL)
	{
		pSkeletonHeadIn = pAppendMe;
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
//void SkeletonLL_New(
struct SkeletonLL*  SkeletonLL_New(struct SkeletonLL*& pSkeletonHeadIn,
					char* pName,//new skeleton name
					char* pParentName)
{
	struct SkeletonLL* pSkeleton = NULL;

	if(pName==NULL)return NULL;

	pSkeleton = SkeletonLL_Append(pSkeletonHeadIn,pName,pParentName);
	return pSkeleton;
}


//Skeleton file open/save all keyframes, and joints
bool SkeletonLL_Open(struct SkeletonLL*& pSkeletonHeadIn,char *pFileName)
{
	HANDLE fH;
	struct SkeletonLL* pSkeleton = NULL;
	struct KeyFrameLL* pKeyFrame = NULL;
	struct KeyFrameLL* pRP_KeyFrame = NULL;
	struct JointLL*    pJoint    = NULL;
	struct JointLL*    pRP_Joint    = NULL;
	struct JointLL*    pSrcJoint    = NULL;
	struct JointLL*    pDstJoint    = NULL;
	unsigned long num_keyframes;
	unsigned long num_joints;
	char pName[256];
	char pParentName[256];
	char pParentJointName[256];
	long j,k;
	bool bCreateInfoItem = false;
	unsigned long anim_num;
	unsigned long time;

//	sprintf(FullName,"%s.ske",pFileName);
//	AddPathToFileName(FullName);
	
//	fH = CreateFile(FullName,GENERIC_READ,


	fH = CreateFile(pFileName,GENERIC_READ,
   	    0, (LPSECURITY_ATTRIBUTES) NULL,
   	    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
   	    (HANDLE) NULL);
	if(fH != INVALID_HANDLE_VALUE)
	{
	
		//Read Skeleton Data
		ReadFromFile(fH,(char*) pName,256*sizeof(char));
		ReadFromFile(fH,(char*) pParentName,256*sizeof(char));
		ReadFromFile(fH,(char*) pParentJointName,256*sizeof(char));

 		pSkeleton = SkeletonLL_Append(pSkeletonHeadIn,pName,pParentName);

		ReadFromFile(fH,(char*)&pSkeleton->playing_anim_num,sizeof(unsigned long));
		ReadFromFile(fH,(char*)&pSkeleton->looptime,sizeof(unsigned long));
		ReadFromFile(fH,(char*)&pSkeleton->flags,sizeof(unsigned long));

		//Read num_keyframes
		ReadFromFile(fH,(char*)&num_keyframes,sizeof(unsigned long));

		//Read all keyframes
		for(k=0;k<num_keyframes;k++)
		{
			ReadFromFile(fH,(char*)&anim_num,sizeof(unsigned long));
			ReadFromFile(fH,(char*)&time,    sizeof(unsigned long));

			pKeyFrame = KeyFrameLL_Insert(pSkeleton->pKeyFrameHead,anim_num,time);
			if(time==KEYFRAME_RESTPOSE)//k==0
			{//Restpose is stored first on purpose.
				pRP_KeyFrame = pKeyFrame;
			}


			ReadFromFile(fH,(char*)&num_joints,sizeof(unsigned long));

			//RP KF holds Joints that hold their own name. All other joints point to this data
			bCreateInfoItem = (pKeyFrame->time==KEYFRAME_RESTPOSE);
/*
			for(j=0;j<num_joints;j++)
			{
				pJoint = JointLL_Append(pKeyFrame->pJointHead,bCreateInfoItem);
				//pJoint = JointLL_Append(pJointHeadIn,pName,pParentName,bCreateInfoItem);//true always???
			}
*/			
			// load joint data
			pRP_Joint = pRP_KeyFrame->pJointHead;//null during first pass.
			for(j=0;j<num_joints;j++)
			{
				//JointLLWIERDNESS
				if(bCreateInfoItem)
				{//only RP holds names to save memory
					ReadFromFile(fH,(char*)pName,        256*sizeof(char));
					ReadFromFile(fH,(char*)pParentName,  256*sizeof(char));
					//ReadFromFile(fH,(char*) pJoint->pInfoItem->pSkeletonName,256*sizeof(char));
					pJoint = JointLL_Append(pKeyFrame->pJointHead,
											pName,
											pParentName,bCreateInfoItem);
				}
				else
				{//use the name from restpose. the joints in this KF, and RPKF are in same order
					pJoint = JointLL_Append(pKeyFrame->pJointHead,
											pRP_Joint->pInfoItem->pName,
											pRP_Joint->pInfoItem->pParentName,bCreateInfoItem);
					pJoint->pInfoItem = pRP_Joint->pInfoItem;//JointLLWIERDNESS
				}
				ReadFromFile(fH,(char*)&pJoint->pRelativeMatrix,16*sizeof(float));
				ReadFromFile(fH,(char*)&pJoint->pAbsoluteMatrix,16*sizeof(float));
				ReadFromFile(fH,(char*)&pJoint->UniqueID,sizeof(long));

				pRP_Joint = (pRP_Joint) ? (pRP_Joint->pNext) : (NULL);//increment pRP_Joint if its !null
			}//end for(j<num_joints

			//////////////////////////////////////////////////////////////////
			//// Point all keyframes to the memory allocated by the RP KF ////
			//////////////////////////////////////////////////////////////////
/*
			//JointLLWIERDNESS
			if(pKeyFrame->time==KEYFRAME_RESTPOSE)//we know its the last one. perfect time to reset all pointers.
			{//only RP holds names to save memory
				pSrcJoint = pKeyFrame->pJointHead;
				for(j=0;j<num_joints;j++)
				{
					//for each KF find joint index j
					for(kk=0;kk<(num_keyframes-1);kk++)//dont overwrite the last kf(RP KF) also pSrcJoint
					{
						pDstJoint = JointLL_Get(pJointHeadIn,j);
						pDstJoint->pInfoItem = pSrcJoint->pInfoItem;
					}
					pSrcJoint = pSrcJoint->pNext;
				}//end for(j<num_joints
			}
*/
		}//end for(k<num_keyframes
		CloseHandle(fH);
		return true;
	}//End Open File
	return false;
}//end SkeletonLL_Open





bool SkeletonLL_Edit(	struct SkeletonLL* pSkeleton,
						char* pName,
						char* pParentName,
						char* pParentJointName,
						unsigned long playing_anim_num)
{
	
	if(	pSkeleton==NULL ||
		pName==NULL ||
		pParentName==NULL ||
		pParentJointName==NULL)
	{
		return false;
	}

	// noname is not acceptable
	if(	strcmp(pName,"")==0 ||
		strcmp(pParentName,"")==0)
	{
		return false;
	}

	//cant create a null skeleton. null is the implied parent of all skeletons.
	if(strcmp(pName,"null")==0)
	{
		return false;
	}

	//parent and child cant have same name
	if(strcmp(pName,pParentName)==0)
	{
		return false;
	}

	
	sprintf(pSkeleton->pName,"%s",pName);
	sprintf(pSkeleton->pParentName,"%s",pParentName);
	sprintf(pSkeleton->pParentJointName,"%s",pParentJointName);
	pSkeleton->playing_anim_num = playing_anim_num;
	return true;
}

bool SkeletonLL_Save(struct SkeletonLL* pSkeleton,char *pFileName)
{
	HANDLE fH;
	int returnval;
	BOOL bResult = FALSE;
	struct KeyFrameLL* pKeyFrame = NULL;
	struct JointLL*    pJoint    = NULL;
	unsigned long num_keyframes;
//	char FullName[512];

//	sprintf(FullName,"%s.ske",pFileName);
//	AddPathToFileName(FullName);

//	long length = strlen(pFileName);

//	if(length==0 || length>=MAX_PATHNAME)return ;

/*

	memset(gp_TextBuffer1,0,MAX_PATHNAME);
	memcpy(gp_TextBuffer1,pFileName,length);
	RemoveFilenameAndExt(gp_TextBuffer1);
*/
	fH = CreateFile(pFileName,GENERIC_WRITE,
   	    0, (LPSECURITY_ATTRIBUTES) NULL,
   	    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
   	    (HANDLE) NULL);
	if(fH != INVALID_HANDLE_VALUE)
	{//the skeletons file .ske holds skeleton ,keyframes, and joints all in one
		//Write Skeleton Data
		WriteToFile(fH,(char*) pSkeleton->pName,256*sizeof(char));
		WriteToFile(fH,(char*) pSkeleton->pParentName,256*sizeof(char));
		WriteToFile(fH,(char*) pSkeleton->pParentJointName,256*sizeof(char));
		WriteToFile(fH,(char*)&pSkeleton->playing_anim_num,sizeof(unsigned long));
		WriteToFile(fH,(char*)&pSkeleton->looptime,sizeof(unsigned long));
		WriteToFile(fH,(char*)&pSkeleton->flags,sizeof(unsigned long));
		//Write num_keyframes
		num_keyframes = KeyFrameLL_Count(pSkeleton->pKeyFrameHead);
		WriteToFile(fH,(char*)&num_keyframes,sizeof(unsigned long));

		//Write all keyframes
		//store RP KF first, since it holds the joint names. it will need to be the first one loaded.

		Skeleton_SaveKeyFrameData(fH,pSkeleton->pRestPose);

		pKeyFrame = pSkeleton->pKeyFrameHead;
		while(pKeyFrame)
		{
			if(pKeyFrame != pSkeleton->pRestPose)
			{//verify its different. we already stored RP
				Skeleton_SaveKeyFrameData(fH,pKeyFrame);
			}
			pKeyFrame = pKeyFrame->pNext;
		}
		returnval = CloseHandle(fH);

		return true;
	}//End Open File
	return false;
}

void Skeleton_SaveKeyFrameData(HANDLE fH, struct KeyFrameLL* pKeyFrame)
{
	unsigned long num_joints;
	struct JointLL*    pJoint    = NULL;

	if(pKeyFrame)
	{
		//Write KeyFrame Data
		//store RP KF first.
		WriteToFile(fH,(char*)&pKeyFrame->anim_num,sizeof(unsigned long));
		WriteToFile(fH,(char*)&pKeyFrame->time,sizeof(unsigned long));
		//Write num_joints
		num_joints = JointLL_Count(pKeyFrame->pJointHead);
		WriteToFile(fH,(char*)&num_joints,sizeof(unsigned long));

		//Write all joints
		pJoint = pKeyFrame->pJointHead;
		while(pJoint)
		{
			//JointLLWIERDNESS
			if(pKeyFrame->time==KEYFRAME_RESTPOSE)
			{
				WriteToFile(fH,(char*) pJoint->pInfoItem->pName,256);
				WriteToFile(fH,(char*) pJoint->pInfoItem->pParentName,256);
				//WriteToFile(fH,(char*) pJoint->pJointInfoItem->pSkeletonName,256);
			}
			WriteToFile(fH,(char*)&pJoint->pRelativeMatrix,16*sizeof(float));
			WriteToFile(fH,(char*)&pJoint->pAbsoluteMatrix,16*sizeof(float));
			WriteToFile(fH,(char*)&pJoint->UniqueID,sizeof(long));
			pJoint = pJoint->pNext;
		}
	}
}






// SkeletonLL_Close() performs LL Remove
// sorting uses bCloseKeyFrames=false. the sort closes an item after copying pointers to the children
bool SkeletonLL_Close(struct SkeletonLL*& pSkeletonHeadIn,char* pName, bool bCloseKeyFrames)
{
	struct SkeletonLL* pPrev    = NULL;
	struct SkeletonLL* pCloseMe = NULL;
	struct SkeletonLL* pNext = NULL;

	if(!SkeletonLL_Get(pSkeletonHeadIn,pPrev,pCloseMe,pNext,pName))
	{//a skeleton with that name not found
		return false;
	}

	///////////////////////////////////
	//// Are We Removing The Head? ////
	///////////////////////////////////

	if(pCloseMe == pSkeletonHeadIn)
	{//We are removing the head
		if(pNext==NULL)
		{//we are removing the last item in list
			pSkeletonHeadIn = NULL;
		}
		else
		{//then pNext is new head
			pSkeletonHeadIn = pNext;
		}
	}
	else
	{//We are removing a normal item
		assert(pPrev!=NULL);//how could pPrev==null, and pCloseMe not be first item!?
		pPrev->pNext = pNext;
	}

	//MainMenu MainMenu
	if(pCurrentlySelectedSkeleton==pCloseMe)
	{//if we delete current Skeleton, clear all pointers to the data
		pCurrentlySelectedSkeleton = pNext;
	}

	if(bCloseKeyFrames)KeyFrameLL_CloseALL(pCloseMe->pKeyFrameHead);
	
	SAFE_FREE(pCloseMe);
	return true;
}



// SkeletonLL_CloseALL() performs LL RemoveALL
void SkeletonLL_CloseALL(struct SkeletonLL*& pSkeletonHeadIn)
{
	struct SkeletonLL* pCloseMe  = NULL;
	struct SkeletonLL* pThis = NULL;

	pThis = pSkeletonHeadIn;
	while(pThis!=NULL)
	{
		pCloseMe = pThis;
		pThis    = pThis->pNext;//increment BEFORE destroying pCloseMe
		SkeletonLL_Close(pSkeletonHeadIn,pCloseMe->pName,true);
	}
	pSkeletonHeadIn = NULL;
}



// SkeletonLL_
// For Every Skeleton in real LL, 
// 		if it has pFindMyChildren as parent
// 		then append it to new list.
/*
void SkeletonLL_HiearchySortLL(struct SkeletonLL*& pSkeletonHeadIn)
{
	struct SkeletonLL* pNewSkeletonHead = NULL;
	struct SkeletonLL* pCloseMe = NULL;
	struct SkeletonLL* pThis = NULL;

	//to make code cleaner, create a dummy head.
	pFindMyChildren = SkeletonLL_Append(pNewSkeletonHead,"null","null");
	while(pFindMyChildren)
	{
		pThis = pSkeletonHeadIn;
		while(pThis)
		{
			if(strcmp(pThis->pParentName,pFindMyChildren->pName)==0)
			{
				pSkeleton = SkeletonLL_Append(pNewSkeletonHead,pThis->pName,pThis->pParentName);
				memcpy(pSkeleton,pThis,sizeof(SkeletonLL));
			    pSkeleton->pNext = NULL;//its being appended to list, so null. previous pointer value invalid now.
			}
			pThis = pNext;
		}
		pFindMyChildren = pFindMyChildren->pNext;
	}
	//close dummy head. first item in LL
	pCloseMe = pNewSkeletonHead;
	pNewSkeletonHead = pNewSkeletonHead->pNext;
	SAFE_FREE(pCloseMe);
	SkeletonLL_CloseALL(pSkeletonHeadIn,false);//the keyframes are now pointed to by the new sorted LL, so dont close keyframes, just old skeletonLL. 

	
	pSkeletonHeadIn = pNewSkeletonHead;
}
*/
/*
void SkeletonLL_HiearchySortLL(struct SkeletonLL*& pSkeletonHeadIn)
{
	struct SkeletonLL* pNewSkeletonHead = NULL;
	struct SkeletonLL* pThis = NULL;

	//to make code cleaner, create a dummy head.
	pFindMyChildren = SkeletonLL_Append(pNewSkeletonHead,"null","null");
	while(pFindMyChildren)
	{
		pThis = pSkeletonHeadIn;
		while(pThis)
		{
			if(strcmp(pThis->pParentName,pFindMyChildren->pName)==0)
			{
				pSkeleton = SkeletonLL_Append(pNewSkeletonHead,pThis->pName,pThis->pParentName);
				memcpy(pSkeleton,pThis,sizeof(SkeletonLL));
			    pSkeleton-pNext = NULL;
			}
			pThis = pNext;
		}
		pFindMyChildren = pFindMyChildren->pNext;
	}
	SkeletonLL_Close(struct SkeletonLL*& pSkeletonHeadIn,char* pName,bool bCloseKeyFrames)
	pSkeletonHeadIn = NULL;
}

*/




































//________________________________________________________________________
//>Joints_New(SkeletonIn)
//	For skeleton, 
//		get RP keyframe
//			insert joint at end of childern of the parent
//			keep pointer to pInfoItem
//		For each keyframe xcept RP
//			insert joint at end of childern of the parent
//			point to RP's pInfoItem


//check do other joints point to pinfoitem

//Add Joint To All Existing Keyframes in a Skeleton
JointLL* Skeleton_Joint_New( 	struct SkeletonLL* pSkeleton,
								char* pName,
								char* pParentName)
{
	struct KeyFrameLL* pKeyFrame = NULL;
	struct JointLL* pRestPose_Joint = NULL;
	struct JointLL* pPrev = NULL;
	struct JointLL* pThis = NULL;
	struct JointLL* pNext = NULL;
	struct JointLL* pJoint = NULL;

	pKeyFrame = pSkeleton->pRestPose;
	if(JointLL_Get(pKeyFrame->pJointHead,pPrev,pThis,pNext,pName))
	{//Already Exists.
		return pThis;
	}

	pRestPose_Joint = JointLL_New(pKeyFrame->pJointHead,pName,pParentName,true);

	pKeyFrame = pSkeleton->pKeyFrameHead;
	while(pKeyFrame)
	{
		if(pKeyFrame->time==KEYFRAME_RESTPOSE)
		{//already added joint to RPKF
		}
		else
		{
			pJoint = JointLL_New(pKeyFrame->pJointHead,pName,pParentName,false);
			pJoint->pInfoItem = pRestPose_Joint->pInfoItem;
		}
		pKeyFrame = pKeyFrame->pNext;
	}
	return pRestPose_Joint;
}







KeyFrameLL* Skeleton_KeyFrame_New( struct SkeletonLL* pSkeleton,unsigned long anim_num,unsigned long time)
{
	struct KeyFrameLL* pRestPose_KeyFrame = NULL;
	struct KeyFrameLL* pKeyFrame = NULL;
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;
	struct JointLL* pPrevJoint = NULL;
	struct JointLL* pThisJoint = NULL;
	struct JointLL* pNextJoint = NULL;
	struct JointLL* pJoint = NULL;
	struct JointLL* pPrevKeyFramesJoint = NULL;
	struct JointLL* pNextKeyFramesJoint = NULL;
	struct JointLL* pRestPoseKeyFramesJoint = NULL;
	aQUATERNION A,B,C;
	float t;
	bool bValid_pPrev,bValid_pNext;


	if(KeyFrameLL_Get(pSkeleton->pKeyFrameHead,pPrev,pThis,pNext,anim_num,time))
	{//Already Exists.
		return pThis;
	}

	//Step1 : Create New KeyFrame. Each KeyFrame holds a "skeleton" of Joints, in a particular pose.
	pKeyFrame = KeyFrameLL_New(pSkeleton->pKeyFrameHead,anim_num,time);
	if(pKeyFrame==NULL)
	{
		return NULL;
	}

	// Step2:Get RestPose Keyframe. 
	// The RestPose Keyframe holds the name of each joint.
	if(KeyFrameLL_Get(pSkeleton->pKeyFrameHead,pPrev,pRestPose_KeyFrame,pNext,KEYFRAME_ANIMNUM0,KEYFRAME_RESTPOSE))
	{
		//// Step3:Get prev, and next keyframes to the one that was just created in Step1.
		////	for each joint in prev, add blended joint to new keyframe which has no joints yet.
		if(KeyFrameLL_Get(pSkeleton->pKeyFrameHead,pPrev,pThis,pNext,anim_num,time))
		{
			assert(pThis==pKeyFrame);//it would have to be!


			bValid_pPrev = (pPrev && pPrev->anim_num == pKeyFrame->anim_num);
			bValid_pNext = (pNext && pNext->anim_num == pKeyFrame->anim_num && pNext->time<=KEYFRAME_MAXFRAME);

			////////////////////////
			////////  pPrev ////////
			////////  pNext ////////
			////////////////////////
			if(bValid_pPrev && bValid_pNext)
			{//blend
				t = (float)(pThis->time-pPrev->time)/(float)(pNext->time-pPrev->time);

				pPrevKeyFramesJoint = pPrev->pJointHead;
				pNextKeyFramesJoint = pNext->pJointHead;
				while(pPrevKeyFramesJoint || pNextKeyFramesJoint)//theoretically same number joints
				{
					assert(pPrevKeyFramesJoint && pNextKeyFramesJoint);
					assert(pPrevKeyFramesJoint->pInfoItem==pNextKeyFramesJoint->pInfoItem);//sholdnt they?
					//Get RestPose Joint, so we can hook up pInfoItem.
					if(JointLL_Get(pRestPose_KeyFrame->pJointHead,pPrevJoint,pThisJoint,pNextJoint,pPrevKeyFramesJoint->pInfoItem->pName))
					{
						pJoint = JointLL_New(	pKeyFrame->pJointHead,
												pPrevKeyFramesJoint->pInfoItem->pName,
												pPrevKeyFramesJoint->pInfoItem->pParentName,
												(bool)(pKeyFrame->time==KEYFRAME_RESTPOSE));
						//Hook up pInfoItem. Every new joint must point to its restpose equavalant name
						pJoint->pInfoItem = pThisJoint->pInfoItem;
					}

					#ifdef USE_QUATERNIONS
					QuaternionSlerp(pJoint->Relative_Rotation,
									pPrevKeyFramesJoint->Relative_Rotation,
									pNextKeyFramesJoint->Relative_Rotation,
									t);

					QuaternionSlerp(pJoint->Relative_Position,
									pPrevKeyFramesJoint->Relative_Position,
									pNextKeyFramesJoint->Relative_Position,
									t);

					QuaternionSlerp(pJoint->Absolute_Rotation,
									pPrevKeyFramesJoint->Absolute_Rotation,
									pNextKeyFramesJoint->Absolute_Rotation,
									t);

					QuaternionSlerp(pJoint->Absolute_Position,
									pPrevKeyFramesJoint->Absolute_Position,
									pNextKeyFramesJoint->Absolute_Position,
									t);

					#endif//USE_QUATERNIONS

					///////////////////////////////
					//// Blend Relative Matrix ////
					///////////////////////////////

					CreateQuaternionFromMatrix(A,pPrevKeyFramesJoint->pRelativeMatrix);
					CreateQuaternionFromMatrix(B,pNextKeyFramesJoint->pRelativeMatrix);
					QuaternionSlerp(C,A,B,t);
					CreateMatrixFromQuaternion(pJoint->pRelativeMatrix,C);

					pJoint->pRelativeMatrix[12] = Lerp(t,pPrevKeyFramesJoint->pRelativeMatrix[12],pNextKeyFramesJoint->pRelativeMatrix[12]);				
					pJoint->pRelativeMatrix[13] = Lerp(t,pPrevKeyFramesJoint->pRelativeMatrix[13],pNextKeyFramesJoint->pRelativeMatrix[13]);				
					pJoint->pRelativeMatrix[14] = Lerp(t,pPrevKeyFramesJoint->pRelativeMatrix[14],pNextKeyFramesJoint->pRelativeMatrix[14]);				


					///////////////////////////////
					//// Blend Absolute Matrix ////
					///////////////////////////////

					CreateQuaternionFromMatrix(A,pPrevKeyFramesJoint->pAbsoluteMatrix);
					CreateQuaternionFromMatrix(B,pNextKeyFramesJoint->pAbsoluteMatrix);
					QuaternionSlerp(C,A,B,t);
					CreateMatrixFromQuaternion(pJoint->pAbsoluteMatrix,C);

					pJoint->pAbsoluteMatrix[12] = Lerp(t,pPrevKeyFramesJoint->pAbsoluteMatrix[12],pNextKeyFramesJoint->pAbsoluteMatrix[12]);				
					pJoint->pAbsoluteMatrix[13] = Lerp(t,pPrevKeyFramesJoint->pAbsoluteMatrix[13],pNextKeyFramesJoint->pAbsoluteMatrix[13]);				
					pJoint->pAbsoluteMatrix[14] = Lerp(t,pPrevKeyFramesJoint->pAbsoluteMatrix[14],pNextKeyFramesJoint->pAbsoluteMatrix[14]);				


					pPrevKeyFramesJoint = pPrevKeyFramesJoint->pNext;
					pNextKeyFramesJoint = pNextKeyFramesJoint->pNext;
				}
			}
			////////////////////////
			////////  pPrev ////////
			//////// !pNext ////////
			////////////////////////
			if(bValid_pPrev && !bValid_pNext)
			{//Copy pPrev to PKeyFrame
				pPrevKeyFramesJoint = pPrev->pJointHead;
				while(pPrevKeyFramesJoint)
				{
					assert(pPrevKeyFramesJoint->pInfoItem);

					//Get RestPose Joint, so we can hook up pInfoItem.
					if(JointLL_Get(pRestPose_KeyFrame->pJointHead,pPrevJoint,pThisJoint,pNextJoint,pPrevKeyFramesJoint->pInfoItem->pName))
					{
						pJoint = JointLL_New(	pKeyFrame->pJointHead,
												pPrevKeyFramesJoint->pInfoItem->pName,
												pPrevKeyFramesJoint->pInfoItem->pParentName,
												(bool)(pKeyFrame->time==KEYFRAME_RESTPOSE));
						//Hook up pInfoItem. Every new joint must point to its restpose equavalant name
						pJoint->pInfoItem = pThisJoint->pInfoItem;
					}

					#ifdef USE_QUATERNIONS
					memcpy(&pJoint->Relative_Rotation,&pPrevKeyFramesJoint->Relative_Rotation,sizeof(aQUATERNION));
					memcpy(&pJoint->Relative_Position,&pPrevKeyFramesJoint->Relative_Position,sizeof(aQUATERNION));
					memcpy(&pJoint->Absolute_Rotation,&pPrevKeyFramesJoint->Absolute_Rotation,sizeof(aQUATERNION));
					memcpy(&pJoint->Absolute_Position,&pPrevKeyFramesJoint->Absolute_Position,sizeof(aQUATERNION));
					#endif//USE_QUATERNIONS

					memcpy(&pJoint->pRelativeMatrix,&pPrevKeyFramesJoint->pRelativeMatrix,16*sizeof(float));
					memcpy(&pJoint->pAbsoluteMatrix,&pPrevKeyFramesJoint->pAbsoluteMatrix,16*sizeof(float));

					pPrevKeyFramesJoint = pPrevKeyFramesJoint->pNext;
				}
			}
			////////////////////////
			//////// !pPrev ////////
			////////  pNext ////////
			////////////////////////
			if(!bValid_pPrev && bValid_pNext)
			{//Copy pNext to PKeyFrame
				pNextKeyFramesJoint = pNext->pJointHead;//!!!w
				while(pNextKeyFramesJoint)
				{
					assert(pNextKeyFramesJoint->pInfoItem);

					//Get RestPose Joint, so we can hook up pInfoItem.
					if(JointLL_Get(pRestPose_KeyFrame->pJointHead,pPrevJoint,pThisJoint,pNextJoint,pNextKeyFramesJoint->pInfoItem->pName))
					{
						pJoint = JointLL_New(	pKeyFrame->pJointHead,
												pNextKeyFramesJoint->pInfoItem->pName,
												pNextKeyFramesJoint->pInfoItem->pParentName,
												(bool)(pKeyFrame->time==KEYFRAME_RESTPOSE));
						//Hook up pInfoItem. Every new joint must point to its restpose equavalant name
						pJoint->pInfoItem = pThisJoint->pInfoItem;
					}

					#ifdef USE_QUATERNIONS
					memcpy(&pJoint->Relative_Rotation,&pNextKeyFramesJoint->Relative_Rotation,sizeof(aQUATERNION));
					memcpy(&pJoint->Relative_Position,&pNextKeyFramesJoint->Relative_Position,sizeof(aQUATERNION));
					memcpy(&pJoint->Absolute_Rotation,&pNextKeyFramesJoint->Absolute_Rotation,sizeof(aQUATERNION));
					memcpy(&pJoint->Absolute_Position,&pNextKeyFramesJoint->Absolute_Position,sizeof(aQUATERNION));
					#endif//USE_QUATERNIONS

					memcpy(&pJoint->pRelativeMatrix,&pNextKeyFramesJoint->pRelativeMatrix,16*sizeof(float));
					memcpy(&pJoint->pAbsoluteMatrix,&pNextKeyFramesJoint->pAbsoluteMatrix,16*sizeof(float));

					pNextKeyFramesJoint = pNextKeyFramesJoint->pNext;
				}
			}
			////////////////////////
			//////// !pPrev ////////
			//////// !pNext ////////
			////////////////////////
			if(!bValid_pPrev && !bValid_pNext)
			{//Copy RESTPOSE to PKeyFrame
				pRestPoseKeyFramesJoint = pRestPose_KeyFrame->pJointHead;//!!!
				while(pRestPoseKeyFramesJoint)
				{
					assert(pRestPoseKeyFramesJoint->pInfoItem);
					//Get RestPose Joint, so we can hook up pInfoItem.
					if(JointLL_Get(pRestPose_KeyFrame->pJointHead,pPrevJoint,pThisJoint,pNextJoint,pRestPoseKeyFramesJoint->pInfoItem->pName))
					{
						pJoint = JointLL_New(	pKeyFrame->pJointHead,
												pRestPoseKeyFramesJoint->pInfoItem->pName,
												pRestPoseKeyFramesJoint->pInfoItem->pParentName,
												(bool)(pKeyFrame->time==KEYFRAME_RESTPOSE));
						//Hook up pInfoItem. Every new joint must point to its restpose equavalant name
						pJoint->pInfoItem = pThisJoint->pInfoItem;

						#ifdef USE_QUATERNIONS
						memcpy(&pJoint->Relative_Rotation,&pThisJoint->Relative_Rotation,sizeof(aQUATERNION));
						memcpy(&pJoint->Relative_Position,&pThisJoint->Relative_Position,sizeof(aQUATERNION));
						memcpy(&pJoint->Absolute_Rotation,&pThisJoint->Absolute_Rotation,sizeof(aQUATERNION));
						memcpy(&pJoint->Absolute_Position,&pThisJoint->Absolute_Position,sizeof(aQUATERNION));
						#endif//USE_QUATERNIONS
						memcpy(&pJoint->pRelativeMatrix,  &pThisJoint->pRelativeMatrix,16*sizeof(float));
						memcpy(&pJoint->pAbsoluteMatrix,  &pThisJoint->pAbsoluteMatrix,16*sizeof(float));
					}
					pRestPoseKeyFramesJoint = pRestPoseKeyFramesJoint->pNext;
				}
			}
		}
	}
	else
	{//get failed
		assert(0 && "error : Failed to get recently inserted item. possible corrupted memory.");
	}

	return pKeyFrame;
}










