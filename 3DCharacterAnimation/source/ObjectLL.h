//ObjectLL.h written by david paull
void ObjectLL_SetAllMeshs_Flags(struct ObjectLL*& pObject,unsigned long flags);
void ObjectLL_SetInVRAM(struct ObjectLL* pObject,bool bInVRAM);
void ObjectLL_ClearAllMeshs_Flags(struct ObjectLL*& pObject,unsigned long flags);
void ObjectLL_SetAllMeshs_BlendMode(struct ObjectLL*& pObject,long SrcBlend,long DstBlend);
void ObjectLL_SetAllMeshs_VertexAlpha(struct ObjectLL*& pObject,float alpha);
void ObjectLL_ColorizeUsingBoneWeights(	struct ObjectLL* pObject,
										unsigned long JointIndex,
										unsigned long Red,
										unsigned long Grn,
										unsigned long Blu);
void Object_InitBoneWeights(struct ObjectLL* pObject);
void Object_FlagVerticesAsVisible(	struct ObjectLL* pObject);
void Object_FlagVerticesAsSelected(	struct ObjectLL* pObject,
									POINT ptRectStart,
									POINT ptRectStop);
void Object_Mouse_Move_SelectedVertices(struct ObjectLL* pObject,POINT MouseDiff);

void ObjectLL_DeformMeshesUsingSkeleton(struct ObjectLL* pObject);


bool ObjectLL_Get(	struct ObjectLL*  pObjectHeadIn,
					struct ObjectLL*& pPrevIn,
					struct ObjectLL*& pThisIn,
					struct ObjectLL*& pNextIn,
					char* pName);
void ObjectLL_GetFirst(	struct ObjectLL*  pObjectHeadIn,
						struct ObjectLL*& pPrev,
						struct ObjectLL*& pThis,
						struct ObjectLL*& pNext);
void ObjectLL_GetLast(	struct ObjectLL*  pObjectHeadIn,
						struct ObjectLL*& pPrev,
						struct ObjectLL*& pThis,
						struct ObjectLL*& pNext);

bool ObjectLL_Load(struct ObjectLL* pObject,char *pFileName);
bool ObjectLL_Save(struct ObjectLL* pObject,char *pFileName);
void ObjectLL_Sort(struct ObjectLL*& pObjectHeadIn);

struct ObjectLL* ObjectLL_Insert(unsigned long SortKey,
								 char* pName,
								 struct ObjectLL*& pObjectHeadIn);

void ObjectLL_Remove(	struct ObjectLL*& pRemoveMe,
						struct ObjectLL*& pObjectHeadIn);

void ObjectLL_GetMinMaxXYZ(	struct ObjectLL* pObject,
							float& ObjectMinX,
							float& ObjectMinY,
							float& ObjectMinZ,
							float& ObjectMaxX,
							float& ObjectMaxY,
							float& ObjectMaxZ);


void ObjectLL_AllMeshs_SetBit(struct ObjectLL* pObject,unsigned long flags);
void ObjectLL_AllMeshs_ClearBit(struct ObjectLL* pObject,unsigned long flags);
void ObjectLL_AllMeshs_BackupRenderFlags(struct ObjectLL* pObject);
void ObjectLL_AllMeshs_RestoreRenderFlags(struct ObjectLL* pObject);

void ObjectLL_RemoveALL(struct ObjectLL*& pObjectHeadIn);
