//SceneLL.h written by david paull
struct SceneLL* SceneLL_Insert(	unsigned long SortKey,
							    char* pName,
							    struct SceneLL*& pSceneHead);
void SceneLL_RemoveALL(struct SceneLL*& pSceneHeadIn);
void SceneLL_Remove(char* pName,
					struct SceneLL*& pSceneHeadIn);
void SceneLL_Remove(struct SceneLL*& pRemoveMe,
					struct SceneLL*& pSceneHeadIn);
bool SceneLL_Get(	struct SceneLL*  pSceneHeadIn,
					struct SceneLL*& pPrevIn,
					struct SceneLL*& pThisIn,
					struct SceneLL*& pNextIn,
					char* pName);
void  SceneLL_GetFirst(	struct SceneLL*  pSceneHeadIn,
						struct SceneLL*& pPrev,
						struct SceneLL*& pThis,
						struct SceneLL*& pNext);
void  SceneLL_GetLast(	struct SceneLL*  pSceneHeadIn,
						struct SceneLL*& pPrev,
						struct SceneLL*& pThis,
						struct SceneLL*& pNext);
void SceneLL_Sort(struct SceneLL*& pSceneHeadIn);
bool SceneLL_Load(struct SceneLL* pScene,char *pFileName);
bool SceneLL_Save(struct SceneLL* pScene,char *pFileName);

void SceneLL_SetInVRAM(struct SceneLL* pScene,bool bInVRAM);
