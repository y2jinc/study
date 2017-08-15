//RectLL.h written by david paull
struct RectLL* RectLL_Insert(  	unsigned long SortKey,
								long ColorIndex,
								float left,
								float right,
								float top,
								float bottom,
								struct RectLL*& pRectHeadIn);
void RectLL_Remove( struct RectLL* pRect,
					struct RectLL*& pRectHeadIn);
void RectLL_RemoveALL(	struct RectLL*& pRectHeadIn);
