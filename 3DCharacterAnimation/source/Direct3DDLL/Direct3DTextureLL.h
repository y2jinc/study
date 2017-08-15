#pragma once

#include <d3d8.h>

struct Direct3DTextureLL
{
	unsigned long SortKey;//used to compare to a meshes texture, to see if it is already in VRAM
	unsigned long UniqueID;//used to compare to a meshes texture, to see if it is already in VRAM
	LPDIRECT3DTEXTURE8 pD3DTexture8;   // The d3d texture for this font
	struct Direct3DTextureLL* pNext;
};

//struct Direct3DTextureLL* Direct3DTextureLL_Insert(unsigned long SortKey,unsigned long UniqueID,struct Direct3DTextureLL*& pDirect3DTextureHead);
//struct Direct3DTextureLL* Direct3DTextureLL_Insert(unsigned long SortKey,unsigned long UniqueID);
//void Direct3DTextureLL_Insert();
struct Direct3DTextureLL* Direct3DTextureLL_Insert(unsigned long SortKey,unsigned long UniqueID,struct Direct3DTextureLL*& pDirect3DTextureHead);
void Direct3DTextureLL_Remove(unsigned long UniqueID,struct Direct3DTextureLL*& pDirect3DTextureHead);
void Direct3DTextureLL_RemoveALL(struct Direct3DTextureLL*& pDirect3DTextureHead);
