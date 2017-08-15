//KeyFrameLL.h written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to

unsigned long KeyFrameLL_Count(struct KeyFrameLL* pKeyFrameHeadIn);
bool KeyFrameLL_Get(struct KeyFrameLL*  pKeyFrameHeadIn,
					struct KeyFrameLL*& pPrev,
					struct KeyFrameLL*& pThis,
					struct KeyFrameLL*& pNext,
					unsigned long anim_num,
					unsigned long time);
void KeyFrameLL_GetFirst(	struct KeyFrameLL*  pKeyFrameHeadIn,
							struct KeyFrameLL*& pPrev,
							struct KeyFrameLL*& pThis,
							struct KeyFrameLL*& pNext);
void KeyFrameLL_GetLast(struct KeyFrameLL*  pKeyFrameHeadIn,
						struct KeyFrameLL*& pPrev,
						struct KeyFrameLL*& pThis,
						struct KeyFrameLL*& pNext);
void KeyFrameLL_GetRestPose(
						struct KeyFrameLL*  pKeyFrameHeadIn,
						struct KeyFrameLL*& pPrev,
						struct KeyFrameLL*& pThis,
						struct KeyFrameLL*& pNext);
struct KeyFrameLL* KeyFrameLL_Insert(struct KeyFrameLL*& pKeyFrameHeadIn,unsigned long anim_num,unsigned long time);
struct KeyFrameLL*  KeyFrameLL_New(struct KeyFrameLL*& pKeyFrameHeadIn,unsigned long anim_num,unsigned long time);

//void KeyFrameLL_Sort(struct KeyFrameLL*& pKeyFrameHeadIn);
//struct KeyFrameLL* KeyFrameLL_SelectPrev(	struct KeyFrameLL*  pKeyFrameHeadIn,
//											unsigned long anim_num,
//											unsigned long time);
//struct KeyFrameLL* KeyFrameLL_SelectNext(	struct KeyFrameLL*  pKeyFrameHeadIn,
//											unsigned long anim_num,
//											unsigned long time);
bool KeyFrameLL_Close(struct KeyFrameLL*& pKeyFrameHeadIn,unsigned long anim_num,unsigned long time);
void KeyFrameLL_CloseALL(struct KeyFrameLL*& pKeyFrameHeadIn);
void KeyFrame_Slerp(struct KeyFrameLL* pF,//pFinal
					struct KeyFrameLL* pA,
					struct KeyFrameLL* pB,
					unsigned long time);
void KeyFrame_TransformJoints_AbsoluteToRelative(struct KeyFrameLL* pKeyFrame, float* pActorMatrix);
void KeyFrame_TransformJoints_RelativeToAbsolute(struct KeyFrameLL* pKeyFrame, float* pActorMatrix);
void KeyFrame_Copy(struct KeyFrameLL* pKeyFrame_Src,struct KeyFrameLL* pKeyFrame_Dst);

