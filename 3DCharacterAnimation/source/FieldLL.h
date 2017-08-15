//FieldLL.h
#ifndef _FieldLL_H_
#define _FieldLL_H_
struct FieldLL* FieldLL_Insert(	
				unsigned long SortKey,
				char* pName,
				char* pText,
				float s,
				float t,
				float width,
				float height,
				fRECT Screen,
				float fontscale,
				float xpos,//of text
				float ypos,//of text
				long textcolor,
				unsigned long flags,
				unsigned long cursorpos,
				float* pfloat,//null if not editable float
				unsigned long* plong,//null if not editable word
				unsigned short* pword,//null if not editable word
				char* pchar,//null if not editable char
				FontObject* pFont,//null for default font [0] 
				struct FieldLL*& pFieldHeadIn);
float CalcWidthString(char* pText,FontObject* pFont,bool bKerning);
float CalcWidthField(FieldLL* pField,FontObject* pFont);
void FieldLL_RemoveVolatile(struct FieldLL*& pFieldHeadIn);
void FieldLL_WM_KEYDOWN(FieldLL* pField,WPARAM wParam, LPARAM lParam);
void FieldLL_WM_CHAR(FieldLL* pField,WPARAM wParam, LPARAM lParam);
void FieldKeepCursorInView(FieldLL* pField,FontObject* pFont);
void PickDataViewField(POINT MousePos,FieldLL* pField);
void ClipRectX(	struct RectLL* pRect,
				float x,
				int dir,
				struct RectLL*& pVisibleLLHead);
void ClipRectY(	struct RectLL* pRect,
				float y,
				int dir,
				struct RectLL*& pVisibleLLHead);

//void NoClipNoOverlapRectLLaddToIconLL(struct RectLL* pInitialLL,
//									struct IconLL*& pIconHeadIn);
void UpdateMainMenu(bool left_mouse_button_down);
/*

void FieldLL_Remove(char* pName,struct FieldLL*& pFieldHeadIn);
void FieldLL_RemoveVolatile(struct FieldLL*& pFieldHeadIn);
void FieldLL_RemoveALL(struct FieldLL*& pFieldHeadIn);
void FieldLL_Sort(struct FieldLL*& pFieldHeadIn);

void Home(FieldLL* pField);
void End(FieldLL* pField);
void Insert(FieldLL* pField,char Char);
void Delete(FieldLL* pField);
void BackSpace(FieldLL* pField);
int PickFieldChar(struct SceneLL* pScene,FieldLL* pField,FontObject* pFont);
FieldLL* SelectField(struct SceneLL* pScene,FieldLL* pFieldHeadIn,POINT ptCursor);
void FieldLL_WM_KEYDOWN(FieldLL* pField,WPARAM wParam, LPARAM lParam);
void FieldLL_WM_CHAR(FieldLL* pField,WPARAM wParam, LPARAM lParam);
float CalcWidthField(FieldLL* pField,FontObject* pFont);
float CalcWidthString(char* pText,FontObject* pFont,bool bKerning);
void FieldKeepCursorInView(FieldLL* pField,FontObject* pFont);
*/

#endif//_FieldLL_H_
