//IconLL.h written by david paull
struct IconLL* IconLL_Insert(	char* pName,
								unsigned long SortKey,
								fRECT Screen,
								fRECT Texture,
								unsigned long flags,
								struct IconLL*& pIconHeadIn);
void IconLL_Remove(char* pName,
					struct IconLL*& pIconHeadIn);
void IconLL_RemoveALL(struct IconLL*& pIconHeadIn);
void IconLL_Sort(struct IconLL*& pIconHeadIn);
