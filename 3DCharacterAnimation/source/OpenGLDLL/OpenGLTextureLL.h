#include <GL\gl.h>

struct OpenGLTextureLL
{
	unsigned long SortKey;//used to compare to a meshes texture, to see if it is already in VRAM
	unsigned long UniqueID;//used to compare to a meshes texture, to see if it is already in VRAM
	GLuint	TextureID[3];//mipmapping
	struct OpenGLTextureLL* pNext;
};

//struct OpenGLTextureLL* OpenGLTextureLL_Insert(unsigned long SortKey,unsigned long UniqueID,struct OpenGLTextureLL*& pOpenGLTextureHead);
//struct OpenGLTextureLL* OpenGLTextureLL_Insert(unsigned long SortKey,unsigned long UniqueID);
//void OpenGLTextureLL_Insert();
struct OpenGLTextureLL* OpenGLTextureLL_Insert(unsigned long SortKey,unsigned long UniqueID,struct OpenGLTextureLL*& pOpenGLTextureHead);
void OpenGLTextureLL_Remove(unsigned long UniqueID,struct OpenGLTextureLL*& pOpenGLTextureHead);
void OpenGLTextureLL_RemoveALL(struct OpenGLTextureLL*& pOpenGLTextureHead);
