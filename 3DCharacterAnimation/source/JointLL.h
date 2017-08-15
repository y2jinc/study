//JointLL.h written by david paull
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to

unsigned long JointLL_Count(struct JointLL* pJointHeadIn);
bool JointLL_Get(	struct JointLL*  pJointHeadIn,
					struct JointLL*& pPrev,
					struct JointLL*& pThis,
					struct JointLL*& pNext,
					char* pName);
struct JointLL* JointLL_Get_Child(	struct JointLL*  pJointHeadIn,
									char* pParentName);

void JointLL_GetFirst(	struct JointLL*  pJointHeadIn,
							struct JointLL*& pPrev,
							struct JointLL*& pThis,
							struct JointLL*& pNext);
void JointLL_GetLast(struct JointLL*  pJointHeadIn,
						struct JointLL*& pPrev,
						struct JointLL*& pThis,
						struct JointLL*& pNext);
struct JointLL* JointLL_Append(	struct JointLL*& pJointHeadIn,//pObject->pJointHead
								char* pName,//Joint name
								char* pParentName,//name of Joint that holds joint that holds pParentJointName
								bool bCreateInfoItem);//this is false during a sort(). true when creating normally.
struct JointLL* JointLL_New(struct JointLL*& pJointHeadIn,
							char* pName,//new Joint name
							char* pParentName,
							bool bCreateInfoItem);//this is false during a sort(). true when creating normally.
void JointLL_Edit(	struct JointLL* pJoint,
					char* pName,//Joint name
					unsigned long playing_anim_num);
bool JointLL_Close(struct JointLL*& pJointHeadIn,char* pName);
void JointLL_CloseALL(struct JointLL*& pJointHeadIn);


void Joint_TransformJoint_AbsoluteToRelative(struct JointLL* pJoint, float* pActorMatrix);
