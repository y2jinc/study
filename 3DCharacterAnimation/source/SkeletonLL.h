//SkeletonLL.h written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to

unsigned long SkeletonLL_Count(struct SkeletonLL* pSkeletonHeadIn);
bool SkeletonLL_Get(	struct SkeletonLL*  pSkeletonHeadIn,
					struct SkeletonLL*& pPrev,
					struct SkeletonLL*& pThis,
					struct SkeletonLL*& pNext,
					char* pName);
void SkeletonLL_GetFirst(	struct SkeletonLL*  pSkeletonHeadIn,
						struct SkeletonLL*& pPrev,
						struct SkeletonLL*& pThis,
						struct SkeletonLL*& pNext);
void SkeletonLL_GetLast(	struct SkeletonLL*  pSkeletonHeadIn,
						struct SkeletonLL*& pPrev,
						struct SkeletonLL*& pThis,
						struct SkeletonLL*& pNext);
struct SkeletonLL* SkeletonLL_SelectNext(	struct SkeletonLL* pSkeletonHeadIn,
											char* pNameCurrentSkeleton);
struct SkeletonLL* SkeletonLL_Append(	struct SkeletonLL*& pSkeletonHeadIn,//pObject->pSkeletonHead
								char* pName,//Skeleton name
								char* pParentSkeletonName);//name of Skeleton that holds Skeleton that holds pParentSkeletonName
struct SkeletonLL*  SkeletonLL_New(struct SkeletonLL*& pSkeletonHeadIn,
					char* pName,//new Skeleton name
					char* pParentName);
bool SkeletonLL_Edit(	struct SkeletonLL* pSkeleton,
						char* pName,
						char* pParentName,
						char* pParentJointName,
						unsigned long playing_anim_num);
bool SkeletonLL_Save(HANDLE fH,struct SkeletonLL* pSkeleton,char *pFileName);
bool SkeletonLL_Close(struct SkeletonLL*& pSkeletonHeadIn,char* pName, bool bCloseKeyFrames);
void SkeletonLL_CloseALL(struct SkeletonLL*& pSkeletonHeadIn);
void SkeletonLL_HiearchySortLL(struct SkeletonLL*& pSkeletonHeadIn);




JointLL* Skeleton_Joint_New( 	struct SkeletonLL* pSkeleton,
								char* pName,
								char* pParentName);
KeyFrameLL* Skeleton_KeyFrame_New( 	struct SkeletonLL* pSkeleton,
									unsigned long anim_num,
									unsigned long time);

bool SkeletonLL_Open(struct SkeletonLL*& pSkeletonHeadIn,char *pFileName);
bool SkeletonLL_Save(struct SkeletonLL* pSkeleton,char *pFileName);

void SkeletonLL_CloseALL(struct SkeletonLL*& pSkeletonHeadIn);
