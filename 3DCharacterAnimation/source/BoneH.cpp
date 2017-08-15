//BoneH.cpp written by david paull.
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
// struct BoneH defined in Globals.h

#include "stdafx.h"
#include "resource.h"
#include "globals.h"
#include "FieldLL.h"
#include <assert.h>
#include <stdio.h>


//..................................................................
//Bone "Bone1"
//      SelectBone({current object}"object1") -> "Bone1"
//	  NewChildBone({name}"bone2",{current bone}"bone1")
//	    DeleteBone({current bone}"bone1")
//	      MoveBone()
//	    RotateBone()
//..................................................................
unsigned long FindNextAvailableUniqueID(struct BoneH* pRootBone);
BoneH* GetBoneUsingIndex(struct BoneH* pRootNode,long& LookingForThisBoneIndex);
bool Recursive_GetBoneUsingIndex(	struct BoneH* pCurrent,
									struct BoneH*& pNextBone,//fill this with result
									long&  Counter,//counter 1,2,3... until we reach LFTBIndex
									long   LookingForThisBoneIndex);//currently examining this bone.

bool Recursive_GetBone(char* pName,struct BoneH* pCurrent);

//BoneH* Recursive_GetNextBone(struct BoneH* pCurrent,bool firstcall);
bool Recursive_SelectBone(struct BoneH* pCurrent,bool firstcall);
bool Recursive_RemoveALL(struct BoneH* pCurrent);
bool Recursive_TransformRelativeToAbsolute(struct BoneH* pCurrent,unsigned long time);
bool Recursive_TransformAbsoluteToRelative(struct BoneH* pCurrent,unsigned long time);
void InitializeNewBone(	struct BoneH* pNewBone,
						char* pName,
						float Length,//basically, the length of parent bone
				   		unsigned long Angle);

extern struct ObjectLL* pCurrentlySelectedObject;
extern struct BoneH* pCurrentlySelectedBone;
extern char pTempText[];
extern long SuccessTextColor;
extern long ErrorTextColor;
extern long NormalTextColor;



struct BoneH* pg_GetBone = NULL;
struct BoneH* pg_SelectBone = NULL;
//long LookingForThisBoneIndex = 0;







//cycle through bones of pCurrentAnimation[STARTFRAME]
//BROKEN BROKEN BROKEN //











//void TransformSkeletonBones(struct BoneH* pCurrent,)
//{
//}
/*
void UpdateFrames(struct BoneH* pCurrent,float* pCurrentMatrix)
{
	float TempMatrix[16];
    struct BoneH* pThis = pCurrent->pChildren;

	MatrixMul(	TempMatrix,
				pCurrent->pRotationMatrix,
				pCurrentMatrix);
	
	MatrixMul(	pCurrent->pCombinedMatrix,
				TempMatrix,
				pCurrent->pTranslationMatrix);
    while(pThis != NULL)
    {
        UpdateFrames(pThis,pCurrent->pCombinedMatrix);
        pThis = pThis->pNextSibling;
    }
}
*/


//multiple insert functions call call this init function
//void InitializeNewBone(struct BoneH* pNewBone,char* pName)
void InitializeNewBone(	struct BoneH* pNewBone,
						char* pName,
				   		unsigned long UniqueID,
						float Length,       
						//Offset,//basically, the length of parent bone
				   		unsigned long Angle)
{
	struct KeyFrameLL* pKeyFrame  = NULL;
	//pName

	//pKeyFrameHead : points to LL of keyframes.
	// Neutral - rest pose.
	// Current - working memory.
	if(pNewBone==NULL)return;


	sprintf(pNewBone->pName,"%s",pName);
	pNewBone->Length = Length;
	pNewBone->UniqueID = UniqueID;

	//pNewBone->AbsoluteLength = 1.0;

	pNewBone->pKeyFrameHead = NULL;

	pNewBone->pRestPose = KeyFrameLL_Insert(KEYFRAME_RESTPOSE,pNewBone->pKeyFrameHead);
//	pNewBone->pPrevPose = KeyFrameLL_Insert(KEYFRAME_PREVPOSE,pNewBone->pKeyFrameHead);
	pNewBone->pCurrPose = KeyFrameLL_Insert(KEYFRAME_CURRPOSE,pNewBone->pKeyFrameHead);
//	pNewBone->pNextPose = KeyFrameLL_Insert(KEYFRAME_NEXTPOSE,pNewBone->pKeyFrameHead);

	//Every object has min/max keyframes, so any time can be interpolated.
	KeyFrameLL_Insert(KEYFRAME_MINFRAME,pNewBone->pKeyFrameHead);
	KeyFrameLL_Insert(KEYFRAME_MAXFRAME,pNewBone->pKeyFrameHead);

	pNewBone->pParent		= NULL;
	pNewBone->pChildren   	= NULL;
	pNewBone->pNextSibling  = NULL;

	//Create "initial angle" matrix for each keyframe.
	pKeyFrame = pNewBone->pKeyFrameHead;
	while(pKeyFrame)
	{
		CreateMatrixRotationZ(pKeyFrame->pRelativeMatrix,Angle*DegToRad);
//		CreateMatrixRotationZ(pKeyFrame->pAbsoluteMatrix,Angle*DegToRad);
		pKeyFrame = pKeyFrame->pNext;
	}


	pNewBone->pRestPose->pRelativeMatrix[12] = 0.0;
	pNewBone->pRestPose->pRelativeMatrix[13] = 0.0;

	if(pNewBone->pParent)
	{
		pNewBone->pRestPose->pRelativeMatrix[13] = pNewBone->pParent->Length;
	}

	pNewBone->pRestPose->pRelativeMatrix[14] = 0.0;


}


//all siblings must have same parent
//if parent is null, then it is root node.
//Add sibling(brother,sister), named pName, to pCurrentlySelectedBone
//								struct BoneH* pParent,
//								struct BoneH* pChildren)
struct BoneH* NewSiblingBone(	char* pName, 
								float Length,
						   		unsigned long Angle,
								struct BoneH* pFirstChild)
{
	struct BoneH* pNewBone = NULL;
	struct BoneH* pThis = NULL;
	struct BoneH* pNext = NULL;
	unsigned long UniqueID = 0;

	//allocate memory for a new bone
	pNewBone = (struct BoneH *)calloc(1,sizeof(struct BoneH));
	pNewBone->pParent = pFirstChild->pParent;
	//initialize data for new bone
	UniqueID = FindNextAvailableUniqueID(pCurrentlySelectedObject->pSkeleton);
	InitializeNewBone(pNewBone,pName,UniqueID,Length,Angle);

	//attach to end of list.
	pThis = pFirstChild;
	while(pThis->pNextSibling)
	{
		pThis = pThis->pNextSibling;
	}
	pThis->pNextSibling = pNewBone;
	pNewBone->pParent   = pFirstChild->pParent;

	return pNewBone;
}


unsigned long FindNextAvailableUniqueID(struct BoneH* pRootBone)
{
	struct BoneH* pBone = NULL;
	bool UniqueIDalreadyTaken = false;
	unsigned long LookingForThisUniqueID = 0;
	long LookingForThisBone = 0;

	while(1)
	{
		LookingForThisBone = 0;
		UniqueIDalreadyTaken = false;
	 	while(pBone = GetBoneUsingIndex(pRootBone,LookingForThisBone))
		{
			if(pBone->UniqueID==LookingForThisUniqueID)
			{
				UniqueIDalreadyTaken = true;
				break;
			}
		}
	 	if(UniqueIDalreadyTaken)
	 	{
			LookingForThisUniqueID++;
		}
		else
		{
			return LookingForThisUniqueID;
		}
	}
}

//Add bone, named pName, to pParent
//must provide either
//1)parent bone
//2)pCurrentAnimation->pSkeleton
struct BoneH* NewBone(	char* pName, 
						float Length,
				   		unsigned long Angle,
						struct BoneH* pParent)
//						struct BoneH*& pSkeletonRootNode)
{
	struct BoneH* pNewBone = NULL;
	struct BoneH* pThis = NULL;
	struct BoneH* pNext = NULL;
	unsigned long UniqueID = 0;



	if(pParent)
	{//parent already has firstchild, so add a sibling to the first child
		if(pParent->pChildren)
		{
			//attach additional child to parent.
			return NewSiblingBone(pName,Length,Angle,pParent->pChildren);
		}
		else
		{
			//attach first child to parent.
			pNewBone = (struct BoneH *)calloc(1,sizeof(struct BoneH));
			pNewBone->pParent = pParent;

			UniqueID = FindNextAvailableUniqueID(pCurrentlySelectedObject->pSkeleton);
			InitializeNewBone(pNewBone,pName,UniqueID,Length,Angle);
			pParent->pChildren = pNewBone;
			pNewBone->pParent  = pParent;
			return pNewBone;
		}
	}
	else
	{
		//possible source of memory leak if caller doesnt recieve return value
		//possible source of memory leak if caller doesnt recieve return value
		//possible source of memory leak if caller doesnt recieve return value

		//attach first child to root node.
		//!!CALLING APP must attach return value to pSkeleton if NULL parent is passed into this function
		pNewBone = (struct BoneH *)calloc(1,sizeof(struct BoneH));
		if(pCurrentlySelectedObject && pCurrentlySelectedObject->pSkeleton)
		{
			UniqueID = FindNextAvailableUniqueID(pCurrentlySelectedObject->pSkeleton);
		}
		else
		{//very first object.
			UniqueID = 0;
		}
		InitializeNewBone(pNewBone,pName,UniqueID,Length,Angle);
		return pNewBone;

		//possible source of memory leak if caller doesnt recieve return value
		//possible source of memory leak if caller doesnt recieve return value
		//possible source of memory leak if caller doesnt recieve return value
	}
	assert(0);
}




//Getbone based on a name.
//Getbone based on a name.
//Getbone based on a name.
//Getbone based on a name.
//Getbone based on a name.
long CurrentSelectBoneIndex=0;
struct BoneH* GetBone(char* pName,struct BoneH* pCurrent)
{
	pg_GetBone = NULL;
	if(strlen(pName)==0)
	{
		return pg_GetBone;
	}
	CurrentSelectBoneIndex=0;
	Recursive_GetBone(pName,pCurrent);
	return pg_GetBone;
}
bool Recursive_GetBone(char* pName,struct BoneH* pCurrent)
{
	if(pCurrent==NULL)return false;

	if(strcmp(pCurrent->pName,pName)==0)
	{//stop looking we found it
		pg_GetBone = pCurrent;
		return false;
	}

    struct BoneH* pThis = pCurrent->pChildren;
    while(pThis != NULL)
    {
 		CurrentSelectBoneIndex++;
 		if(!Recursive_GetBone(pName,pThis))
 		{//stop looking we found it
 			return false;
 		}
        pThis = pThis->pNextSibling;
    }
    return true;
}
//Getbone based on a name.
//Getbone based on a name.
//Getbone based on a name.
//Getbone based on a name.
//Getbone based on a name.
//Getbone based on a name.


//when no more childrem, go thro siblings.

bool Bone_RemoveALL(struct BoneH*& pSkeleton)
{
    struct BoneH* pThis = NULL;
	if(pSkeleton==NULL)return false;

   	pThis = pSkeleton;
	if(pThis)
	{
		Recursive_RemoveALL(pThis);
	}
	pSkeleton = NULL;
   	return true;
}

bool Recursive_RemoveALL(struct BoneH* pCurrent)
{
    struct BoneH* pThis = pCurrent->pChildren;
    struct BoneH* pNextSibling = NULL;
    while(pThis != NULL)
    {
		//pthis gets nullified, so save pNextSibling.
		pNextSibling = pThis->pNextSibling;
 		Recursive_RemoveALL(pThis);
        pThis = pNextSibling;
    }
	KeyFrameLL_RemoveALL(pCurrent->pKeyFrameHead);

	//MainMenu MainMenu
	if(pCurrentlySelectedBone==pCurrent)
	{//if we delete current Bone, clear all pointers to the data
		pCurrentlySelectedBone = NULL;
	}

	SAFE_FREE(pCurrent);
    return false;
}




//Absolute matrices are correct, so update Relative Matrices
void Bone_TransformAbsoluteToRelative(struct BoneH* pSkeletonRootNode,unsigned long time)
{
    struct BoneH* pThis = NULL;

	if(pSkeletonRootNode==NULL)return;

	pThis = pSkeletonRootNode;
//	while(pThis != NULL)//root node may have siblings
	if(pThis != NULL)//root node may have siblings
	{
 		Recursive_TransformAbsoluteToRelative(pSkeletonRootNode,time);
	    //pThis = pThis->pNextSibling;
	}
}
bool Recursive_TransformAbsoluteToRelative(struct BoneH* pCurrent,unsigned long time)
{
	float pInverseChildMatrix[16];
	struct BoneH* pThis = pCurrent->pChildren;
	KeyFrameLL* pCurr_KeyFrame = NULL;
	KeyFrameLL* pThis_KeyFrame = NULL;
	KeyFrameLL* pPrev_KeyFrame = NULL;


  	// T = (A^-1)*B
  	// Cr = (Pa^-1)*Ca
	//InverseMatrix(pInverseChildMatrix,pThis->pAbsoluteMatrix);
	//InverseMatrix(pInverseChildMatrix,pCurrent->pRestPose->pAbsoluteMatrix);

	pCurr_KeyFrame = KeyFrameLL_GetKeyframe(pCurrent->pKeyFrameHead,
											pPrev_KeyFrame,
											time);

	InverseMatrix(pInverseChildMatrix,pCurr_KeyFrame->pAbsoluteMatrix);
	while(pThis != NULL)
	{

		pThis_KeyFrame = KeyFrameLL_GetKeyframe(pThis->pKeyFrameHead,
												pPrev_KeyFrame,
												time);

		if(pThis_KeyFrame->time != time)
		{
//			sprintf(pTempText,"wanted %u got %u",time,pThis_KeyFrame->time);
//			OutputTextToConsole(pTempText,ErrorTextColor);
		}
//		MatrixMul(	pThis->pRestPose->pRelativeMatrix,
//					pThis->pRestPose->pAbsoluteMatrix,
//					pInverseChildMatrix);
		MatrixMul(	pThis_KeyFrame->pRelativeMatrix,
					pThis_KeyFrame->pAbsoluteMatrix,
					pInverseChildMatrix);
		OrthonormalizeOrientation(pThis_KeyFrame->pRelativeMatrix);

		if(!Recursive_TransformAbsoluteToRelative(pThis,time))
		{
			return false;
		}
		pThis = pThis->pNextSibling;
	}
	return true;
}


//Relative matrices are correct, so update Absolute Matrices
void Bone_TransformRelativeToAbsolute(struct BoneH* pSkeletonRootNode,unsigned long time)
{
    struct BoneH* pThis = NULL;
	if(pSkeletonRootNode==NULL)return;

	pThis = pSkeletonRootNode;
//	while(pThis != NULL)//root node may have siblings
	if(pThis != NULL)//root node may have siblings
	{
		Recursive_TransformRelativeToAbsolute(pThis,time);
	    //pThis = pThis->pNextSibling;
	}
}
bool Recursive_TransformRelativeToAbsolute(struct BoneH* pCurrent,unsigned long time)
{
	struct BoneH* pThis = pCurrent->pChildren;
	KeyFrameLL* pCurr_KeyFrame = NULL;
	KeyFrameLL* pThis_KeyFrame = NULL;
	KeyFrameLL* pPrev_KeyFrame = NULL;


	pCurr_KeyFrame = KeyFrameLL_GetKeyframe(pCurrent->pKeyFrameHead,
											pPrev_KeyFrame,
											time);

	while(pThis != NULL)
	{
		//bone may have been scaled. update rel mat offset.
		//pThis->pRestPose->pRelativeMatrix[12] = 0.0;
		//pThis->pRestPose->pRelativeMatrix[13] = pThis->pParent->Length;
		//pThis->pRestPose->pRelativeMatrix[14] = 0.0;


		pThis_KeyFrame = KeyFrameLL_GetKeyframe(pThis->pKeyFrameHead,
												pPrev_KeyFrame,
												time);
		if(pThis_KeyFrame->time != time)
		{
//			sprintf(pTempText,"wanted %u got %u",time,pThis_KeyFrame->time);
//			OutputTextToConsole(pTempText,ErrorTextColor);
		}

		pThis_KeyFrame->pRelativeMatrix[12] = 0.0;
		pThis_KeyFrame->pRelativeMatrix[13] = pThis->pParent->Length;
		pThis_KeyFrame->pRelativeMatrix[14] = 0.0;



//		MatrixMul(	pThis->pRestPose->pAbsoluteMatrix,
//					pThis->pRestPose->pRelativeMatrix,
//					pCurrent->pRestPose->pAbsoluteMatrix);

		MatrixMul(	pThis_KeyFrame->pAbsoluteMatrix,
					pThis_KeyFrame->pRelativeMatrix,
					pCurr_KeyFrame->pAbsoluteMatrix);

		if(!Recursive_TransformRelativeToAbsolute(pThis,time))
		{
			return false;
		}
		pThis = pThis->pNextSibling;
	}
	return true;
}









//		MatrixMul(	pThis->pAbsoluteMatrix,
//					pParentAbsoluteMatrix,
//					pThis->pRelativeMatrix);





//??$$%%#@#@@!!~
//you control LookingForThisBoneIndex
//	start at 0 to touch each element once. returns NULL at end of list.
//		like render does to render bones. starts at 0 every frame.

//  or you can maintain the index like the menu does. 
//		The menu starts at 0 once, then cycles, at users command. 


//DOES NOT RETURN UNIQUEID BONE
//INDEX IS ONLY A COUNTER.
//currently no func to return bone based on uniqueid.
BoneH* GetBoneUsingIndex(struct BoneH* pRootNode,long& LookingForThisBoneIndex)//@@#$$$%^%^&&**(())
{
//	static long LookingForThisBoneIndex = 0;
	long   Counter = 0;
	BoneH* pNextBone = NULL;

	if(pRootNode==NULL)
	{
		return NULL;
	}

//	CurrentSelectBoneIndex = 0;
	
	struct BoneH* pThis = pRootNode;
	while(pThis != NULL)//root node may have siblings
	{
		if(!Recursive_GetBoneUsingIndex(pThis,
										pNextBone,
										Counter,
										LookingForThisBoneIndex))
		{//we found it
			LookingForThisBoneIndex++;
    		return pNextBone;
		}
 		Counter++;
	    pThis = pThis->pNextSibling;
	}

	//cant fint it. return null.
	//next call will return first item
	LookingForThisBoneIndex = 0;
	return NULL;
}



bool Recursive_GetBoneUsingIndex(	struct BoneH* pCurrent,
									struct BoneH*& pNextBone,//fill this with result
									long&  Counter,//counter 1,2,3... until we reach LFTBIndex
									long   LookingForThisBoneIndex)//currently examining this bone.
{
    struct BoneH* pThis = pCurrent->pChildren;

	if(Counter>=LookingForThisBoneIndex)
	{
     	pNextBone = pCurrent;
		return false;//found it
	}

    while(pThis != NULL)
    {
 		Counter++;
		if(!Recursive_GetBoneUsingIndex(pThis,
										pNextBone,
										Counter,
										LookingForThisBoneIndex))
 		{
 			return false;//found it
 		}
        pThis = pThis->pNextSibling;
    }
    return true;
}


//CANT BE CALLED DIRECTLY. 
//ASSUMES FILE IS ALREADY OPEN from Skeleton_Save
void KeyFrameLL_Save(HANDLE fH,struct KeyFrameLL* pKeyFrame)
{
	WriteToFile(fH,(char*)&pKeyFrame->time,sizeof(unsigned long));
	WriteToFile(fH,(char*) pKeyFrame->pRelativeMatrix,16*sizeof(float));
	WriteToFile(fH,(char*) pKeyFrame->pAbsoluteMatrix,16*sizeof(float));
}


void BoneH_Save(HANDLE fH,struct BoneH* pBone)
{
	unsigned long num_keyframes = 0;
	struct KeyFrameLL* pKeyFrame = NULL;

	//write this bones name
	WriteToFile(fH,(char*) pBone->pName,256);
	WriteToFile(fH,(char*)&pBone->Length,sizeof(float));
	WriteToFile(fH,(char*)&pBone->UniqueID,sizeof(unsigned long));
	//write this bones PARENTS name
	WriteToFile(fH,(char*) pBone->pParent->pName,256);

	//count keyframes
	pKeyFrame = pBone->pKeyFrameHead;
	while(pKeyFrame)
	{
	 	num_keyframes++;
		pKeyFrame = pKeyFrame->pNext;
	}

	//write num_keyframes
	WriteToFile(fH,(char*)&num_keyframes,sizeof(unsigned long));

	//write n keyframes
	pKeyFrame = pBone->pKeyFrameHead;
	while(pKeyFrame)
	{
		KeyFrameLL_Save(fH,pKeyFrame);
		pKeyFrame = pKeyFrame->pNext;
	}
}

bool Skeleton_Save(struct BoneH* pSkeleton,char* pFileName)
{
	HANDLE fH;
	int returnval;
	BOOL bResult = FALSE;
	unsigned char *pSrc = NULL;
	char FullName[512];
	struct BoneH* pRootBone = NULL;
	struct BoneH* pBone = NULL;
	long Counter = 0;//technically not used
	unsigned long num_bones = 0;

	sprintf(FullName,"%s.ske",pFileName);
	AddPathToFileName(FullName);

	
	fH = CreateFile(FullName,GENERIC_WRITE,
   	    0, (LPSECURITY_ATTRIBUTES) NULL,
   	    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
   	    (HANDLE) NULL);
	if(fH != INVALID_HANDLE_VALUE)
	{
		//count bones
		pRootBone = pSkeleton;
		Counter = 0;
		while(pBone = GetBoneUsingIndex(pRootBone,Counter))
		{
			num_bones++;
		}

		WriteToFile(fH,(char*)&num_bones,sizeof(unsigned long));

		//write bones
		pRootBone = pSkeleton;
		Counter = 0;
		while(pBone = GetBoneUsingIndex(pRootBone,Counter))
		{
			BoneH_Save(fH,pBone);
		}

		returnval = CloseHandle(fH);
		return true;
	}//End Open File
	return false;
}//End 




void KeyFrameLL_Open(HANDLE fH,struct KeyFrameLL* pKeyFrame)
{
	ReadFromFile(fH,(char*)&pKeyFrame->time,sizeof(unsigned long));
	ReadFromFile(fH,(char*) pKeyFrame->pRelativeMatrix,16*sizeof(float));
	ReadFromFile(fH,(char*) pKeyFrame->pAbsoluteMatrix,16*sizeof(float));
}


void BoneH_Open(HANDLE fH,struct BoneH* pBone,char* pBoneParentName)
{
	unsigned long i;
	unsigned long num_keyframes = 0;
	struct KeyFrameLL LoadKeyFrame;
	struct KeyFrameLL* pNewKeyFrame = NULL;

	memset(&LoadKeyFrame,0,sizeof(struct KeyFrameLL));

	ReadFromFile(fH,(char*) pBone->pName,   256*sizeof(char));
	ReadFromFile(fH,(char*)&pBone->Length,      sizeof(float));
	ReadFromFile(fH,(char*)&pBone->UniqueID,      sizeof(unsigned long));
	ReadFromFile(fH,(char*) pBoneParentName,256*sizeof(char));
	ReadFromFile(fH,(char*)&num_keyframes,sizeof(unsigned long));

	for(i=0;i<num_keyframes;i++)
	{
		KeyFrameLL_Open(fH,&LoadKeyFrame);
		pNewKeyFrame = KeyFrameLL_Insert(LoadKeyFrame.time,
										 pBone->pKeyFrameHead);
		memcpy(pNewKeyFrame->pRelativeMatrix,LoadKeyFrame.pRelativeMatrix,16*sizeof(float));
		memcpy(pNewKeyFrame->pAbsoluteMatrix,LoadKeyFrame.pAbsoluteMatrix,16*sizeof(float));

		if(pNewKeyFrame->time == KEYFRAME_RESTPOSE)
		{
			pBone->pRestPose = pNewKeyFrame;
		}
		if(pNewKeyFrame->time == KEYFRAME_CURRPOSE)
		{
			pBone->pCurrPose = pNewKeyFrame;
		}
	}
}

bool Skeleton_Open(struct BoneH*& pSkeleton,char* pFileName)
{
	HANDLE fH;
	int returnval;
	BOOL bResult = FALSE;
	unsigned char *pSrc = NULL;
	char FullName[512];
	char pBoneParentName[256];
	unsigned long num_bones = 0;
	struct BoneH LoadBone;//memory
	struct BoneH* pNewBone = NULL;
	struct BoneH* pParentBone = NULL;
	long i;

	if(pSkeleton==NULL)return false;//memory must already exist to fill


	memset(&LoadBone,0,sizeof(struct BoneH));

	sprintf(FullName,"%s.ske",pFileName);
	AddPathToFileName(FullName);
	
	fH = CreateFile(FullName,GENERIC_READ,
   	    0, (LPSECURITY_ATTRIBUTES) NULL,
   	    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
   	    (HANDLE) NULL);
	if(fH != INVALID_HANDLE_VALUE)
	{
		ReadFromFile(fH,(char*)&num_bones,sizeof(unsigned long));

		for(i=0;i<num_bones;i++)
		{
			//load bone data into LoadBone
			BoneH_Open(fH,&LoadBone,pBoneParentName);

			//get pointer to its parent
			pParentBone = GetBone(pBoneParentName,pSkeleton);

			if(strcmp(LoadBone.pName,"SkeletonRootNode")==0)
			{//RootNode Already Exists
				//remove the 2 intro keys
				KeyFrameLL_RemoveALL(pSkeleton->pKeyFrameHead);

				//surgery 
				//this is bad.
				//Bad, Bad programmer, no biskit for you.
				pSkeleton->UniqueID       = LoadBone.UniqueID;

				pSkeleton->pRestPose      = LoadBone.pRestPose;
				pSkeleton->pCurrPose      = LoadBone.pCurrPose;
				pSkeleton->pKeyFrameHead  = LoadBone.pKeyFrameHead;
			}
			else
			{
				//add a new bone to the new skeleton.
				//the skeleton was orignialy created calling NewBone.
				pNewBone = NewBone(	LoadBone.pName, 
								   	LoadBone.Length,
					   			   	0.0,//doesnt matter
									pParentBone);

				//remove the 2 intro keys
				KeyFrameLL_RemoveALL(pNewBone->pKeyFrameHead);

				//surgery 
				//this is bad.
				//Bad, Bad programmer, no biskit for you.
				pNewBone->UniqueID       = LoadBone.UniqueID;
	
				pNewBone->pRestPose      = LoadBone.pRestPose;
				pNewBone->pCurrPose      = LoadBone.pCurrPose;
				pNewBone->pKeyFrameHead  = LoadBone.pKeyFrameHead;
			}

			LoadBone.pRestPose      = NULL;
			LoadBone.pCurrPose      = NULL;
			LoadBone.pKeyFrameHead  = NULL;
		}

		returnval = CloseHandle(fH);
		return true;
	}//End Open File
	return false;
}//End 


//KeyFrames are added to Skeletons.
//Each bone in skeleton must insert the new keyframe.
//Each Bone holds a linkedlist of all of its keyframes.
//The new keyframe is an interpolation of prev, and next keyframes.

//Call Bone_TransformRelativeToAbsolute() to reconnect the rotated bones into a skeleton.

//CHECK FOR EXISTING KF!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void AddKeyFrame(	struct BoneH* pSkeletonRootNode,
		   			unsigned long time,
		   			bool StoreInCurrent)
{
	aQUATERNION A;
	aQUATERNION B;
	aQUATERNION C;
	struct BoneH* pBone = NULL;
	struct KeyFrameLL* pNewKeyFrame  = NULL;
	struct KeyFrameLL* pPrevKeyFrame = NULL;
	struct KeyFrameLL* pNextKeyFrame = NULL;
	struct KeyFrameLL* pExistingKeyFrame = NULL;

	if(pSkeletonRootNode==NULL)return;


	//test for existing frame

	if(!StoreInCurrent)
	{	
		pExistingKeyFrame = KeyFrameLL_GetKeyframe(	pSkeletonRootNode->pKeyFrameHead,
												  	pPrevKeyFrame,
													time);

		if(pExistingKeyFrame->time == time)
		{
			sprintf(pTempText,"error. Keyframe at time %u already exists.",time);
			OutputTextToConsole(pTempText,ErrorTextColor);
			return;
		}
	}

	long counter = 0;

	while(pBone = GetBoneUsingIndex(pSkeletonRootNode,counter))
	{
		if(StoreInCurrent)
		{	
			pNewKeyFrame = pBone->pCurrPose;
		}
		else
		{
			pNewKeyFrame = KeyFrameLL_InsertSpecial(time,pPrevKeyFrame,pBone->pKeyFrameHead);
		}
		if(pPrevKeyFrame)
		{
			pNextKeyFrame = pNewKeyFrame->pNext;
			//here is the interpolation. 
			// t = t-t1/t2-t1
			float t1 = (float)(time-pPrevKeyFrame->time);
			float t2 = (float)(pNextKeyFrame->time-pPrevKeyFrame->time);
			float t  = t1/t2;

			CreateQuaternionFromMatrix(A,pPrevKeyFrame->pRelativeMatrix);
			CreateQuaternionFromMatrix(B,pNextKeyFrame->pRelativeMatrix);
			QuaternionSlerp(C,A,B,t);
			CreateMatrixFromQuaternion(pNewKeyFrame->pRelativeMatrix,C);
		}
	}
}
bool RemoveKeyFrame(struct BoneH* pSkeletonRootNode,unsigned long time)
{
	long ItemsFound = 0;
	long counter = 0;
	struct BoneH* pBone = NULL;

	if(pSkeletonRootNode==NULL)return false;

	while(pBone = GetBoneUsingIndex(pSkeletonRootNode,counter))
	{
		ItemsFound += KeyFrameLL_Remove(time,pBone->pKeyFrameHead);
	}
	return (ItemsFound>0);
}

void UpdateCurrentPose(	struct BoneH* pSkeletonRootNode,
		   				unsigned long time)
{
	struct BoneH* pBone = NULL;
	struct KeyFrameLL* pNewKeyFrame  = NULL;
	struct KeyFrameLL* pPrevKeyFrame = NULL;
	struct KeyFrameLL* pNextKeyFrame = NULL;
	struct KeyFrameLL* pExistingKeyFrame = NULL;

	aQUATERNION A;
	aQUATERNION B;
	aQUATERNION C;
	if(pSkeletonRootNode==NULL)return;

	long counter = 0;

	while(pBone = GetBoneUsingIndex(pSkeletonRootNode,counter))
	{
		pNewKeyFrame = KeyFrameLL_GetKeyframe(pBone->pKeyFrameHead,pPrevKeyFrame,time);
		pNextKeyFrame = pNewKeyFrame->pNext;

		if(pNewKeyFrame->time == time)
		{
			if(strcmp(pBone->pName,"SkeletonRootNode")==0)
			{//RootNode use absolute matrix
				memcpy(pBone->pCurrPose->pAbsoluteMatrix,pNewKeyFrame->pAbsoluteMatrix,16*sizeof(float));
			}
			
			memcpy(pBone->pCurrPose->pRelativeMatrix,pNewKeyFrame->pRelativeMatrix,16*sizeof(float));

		}
		else
		{//Interpolate New KeyFrame
			// t = t-t1/t2-t1
			float t1 = (float)(time-pNewKeyFrame->time);
			float t2 = (float)(pNextKeyFrame->time-pNewKeyFrame->time);
			float t  = t1/t2;

			if(strcmp(pBone->pName,"SkeletonRootNode")==0)
			{//RootNode use absolute matrix
				//Interpolate Rotation Matrices
				CreateQuaternionFromMatrix(A,pNewKeyFrame->pAbsoluteMatrix);
				CreateQuaternionFromMatrix(B,pNextKeyFrame->pAbsoluteMatrix);
				QuaternionSlerp(C,A,B,t);
				CreateMatrixFromQuaternion(pBone->pCurrPose->pAbsoluteMatrix,C);

				//Interpolate Position
				pBone->pCurrPose->pAbsoluteMatrix[12] = Lerp(t,pNewKeyFrame->pAbsoluteMatrix[12],pNextKeyFrame->pAbsoluteMatrix[12]);				
				pBone->pCurrPose->pAbsoluteMatrix[13] = Lerp(t,pNewKeyFrame->pAbsoluteMatrix[13],pNextKeyFrame->pAbsoluteMatrix[13]);				
				pBone->pCurrPose->pAbsoluteMatrix[14] = Lerp(t,pNewKeyFrame->pAbsoluteMatrix[14],pNextKeyFrame->pAbsoluteMatrix[14]);				

			}
			//else
			{
				CreateQuaternionFromMatrix(A,pNewKeyFrame->pRelativeMatrix);
				CreateQuaternionFromMatrix(B,pNextKeyFrame->pRelativeMatrix);
				QuaternionSlerp(C,A,B,t);
				CreateMatrixFromQuaternion(pBone->pCurrPose->pRelativeMatrix,C);
			}
		}
	}
}

void UpdateCurrentPoseBROKENMATRIXVERSION(	struct BoneH* pSkeletonRootNode,
		   				unsigned long time)
{
	struct BoneH* pBone = NULL;
	struct KeyFrameLL* pNewKeyFrame  = NULL;
	struct KeyFrameLL* pPrevKeyFrame = NULL;
	struct KeyFrameLL* pNextKeyFrame = NULL;
	struct KeyFrameLL* pExistingKeyFrame = NULL;

	if(pSkeletonRootNode==NULL)return;

	//Check for Existing Keyframe at time time
	//If root node has a KF at time, then all bones also have one. 
/*
	pExistingKeyFrame = KeyFrameLL_GetKeyframe(	pSkeletonRootNode->pKeyFrameHead,
											  	pPrevKeyFrame,
												time);

	if(pExistingKeyFrame->time == time)
	{
		bExistingKeyFrame = true;
	}
 */
	long counter = 0;

	while(pBone = GetBoneUsingIndex(pSkeletonRootNode,counter))
	{
		pNewKeyFrame = KeyFrameLL_GetKeyframe(pBone->pKeyFrameHead,pPrevKeyFrame,time);
		pNextKeyFrame = pNewKeyFrame->pNext;

		if(pNewKeyFrame->time == time)
		{
			memcpy(pBone->pCurrPose->pRelativeMatrix,pNewKeyFrame->pRelativeMatrix,16*sizeof(float));
		}
		else
		{//Interpolate New KeyFrame
			// t = t-t1/t2-t1
			float t1 = (float)(time-pNewKeyFrame->time);
			float t2 = (float)(pNextKeyFrame->time-pNewKeyFrame->time);
			float t  = t1/t2;
			if(1)//t<0.5)
			{
				MatrixSlerp(pBone->pCurrPose->pRelativeMatrix,
							pNewKeyFrame->pRelativeMatrix,
							pNextKeyFrame->pRelativeMatrix,
							t);//HACKHACK
//				DoubleMatrixSlerp(pBone->pCurrPose->pRelativeMatrix,
//							pNewKeyFrame->pRelativeMatrix,
//							pNextKeyFrame->pRelativeMatrix,
//							t);//HACKHACK
			}
			else
			{
				t = 1.0-t;
				DoubleMatrixSlerp(pBone->pCurrPose->pRelativeMatrix,
							pNextKeyFrame->pRelativeMatrix,
							pNewKeyFrame->pRelativeMatrix,
							t);//HACKHACK
			}

		}
	}
}

/////
/////
/////
/////
/////
/////
/////
/////
/////
