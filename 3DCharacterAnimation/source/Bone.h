//BoneH.h written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to


BoneH* GetBoneUsingIndex(struct BoneH* pRootNode,long& LookingForThisBoneIndex);

bool Skeleton_Save(struct BoneH* pSkeleton,char* pFileName);
bool Skeleton_Open(struct BoneH*& pSkeleton,char* pFileName);

struct BoneH* NewBone(	char* pName, 
						float Length,
				   		unsigned long Angle,
						struct BoneH* pParent);
bool Bone_RemoveALL(struct BoneH*& pSkeleton);
struct BoneH* GetBone(char* pName,struct BoneH* pCurrent);


void Bone_TransformRelativeToAbsolute(struct BoneH* pSkeletonRootNode,unsigned long time);
void Bone_TransformAbsoluteToRelative(struct BoneH* pSkeletonRootNode,unsigned long time);

void AddKeyFrame(	struct BoneH* pSkeletonRootNode,
		   			unsigned long time,
		   			bool StoreInCurrent);
bool RemoveKeyFrame(struct BoneH* pSkeletonRootNode,unsigned long time);
void UpdateCurrentPose(	struct BoneH* pSkeletonRootNode,
		   				unsigned long time);
