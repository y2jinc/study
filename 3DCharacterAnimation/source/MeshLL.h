//Mesh.h written by david paull

void MeshLL_SetAllVertices_Alpha(struct MeshLL* pMesh,float alpha);
void MeshLL_SetInVRAM(struct MeshLL* pMesh,bool bInVRAM);
void MeshLL_ColorizeUsingBoneWeights(	struct MeshLL* pMesh,
										unsigned long JointIndex,
										unsigned long Red,
										unsigned long Grn,
										unsigned long Blu);

unsigned long MeshLL_Count(struct MeshLL* pMeshHeadIn);

void Mesh_FlagVerticesAsVisible(float* pObjectMatrix,
								struct MeshLL* pMesh);
void Mesh_FlagVerticesAsSelected(	float* pObjectMatrix,
									struct MeshLL* pMesh,
									POINT ptRectStart,
									POINT ptRectStop);


struct MeshLL* MeshLL_ReInsert(	struct MeshLL* 	pMesh,
								struct MeshLL*& pMeshHeadIn);

void Mesh_Mouse_Move_SelectedVertices(	float* pObjectMatrix,
										struct MeshLL* pMesh,
										POINT MouseDiff);


void MeshLL_ColorizeWhite(struct MeshLL* pMesh);
void MeshLL_ColorizeBlack(struct MeshLL* pMesh);
//void MeshLL_ColorizeBlue(struct MeshLL* pMesh);
void MeshLL_ColorizeBlue(struct MeshLL* pMesh);
void MeshLL_ColorizeGreen(struct MeshLL* pMesh);
void MeshLL_CopyLocalToWorld(struct MeshLL* pObject);

struct MeshLL* MeshLL_Insert(unsigned long SortKey,
							 char* pName,
							 struct MeshLL*& pMeshHeadIn);

void MeshLL_Remove(	struct MeshLL*& pRemoveMe,
					struct MeshLL*& pMeshHeadIn);

void MeshLL_RemoveALL(struct MeshLL*& pMeshHeadIn);

bool MeshLL_Get(	struct MeshLL*  pMeshHeadIn,
					struct MeshLL*& pPrevIn,
					struct MeshLL*& pThisIn,
					struct MeshLL*& pNextIn,
					char* pName);
void MeshLL_GetFirst(	struct MeshLL*  pMeshHeadIn,
						struct MeshLL*& pPrev,
						struct MeshLL*& pThis,
						struct MeshLL*& pNext);
void MeshLL_GetLast(	struct MeshLL*  pMeshHeadIn,
						struct MeshLL*& pPrev,
						struct MeshLL*& pThis,
						struct MeshLL*& pNext);


bool MeshLL_Load(struct MeshLL* pMesh,char *pFileName);
bool MeshLL_Save(struct MeshLL* pMesh,char *pFileName);
void MeshLL_Sort(struct MeshLL*& pMeshHeadIn);

struct MeshLL* MeshLL_CreateNode(	char* pName,
									unsigned long max_w_verts,
									long max_w_indices,
									unsigned long RenderFlags);
void MeshLL_GetMinMaxXYZ(	struct MeshLL* pMesh,
							float& MeshMinX,
							float& MeshMinY,
							float& MeshMinZ,
							float& MeshMaxX,
							float& MeshMaxY,
							float& MeshMaxZ);
