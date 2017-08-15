////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// FieldLL.cpp written by david paull. 1
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
// FieldLL.cpp contains code for the ENTIRE MENU
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// BOOKMARKS : 
// searchfor:BEVELEDGE
// searchfor MAINMENUMAINCALLER
// SEPERATESKELETON
// ICONBARTEXTCALLOUT
// CALL_FUNCTION_WHEN_FLAG_IS_TOGGLED
////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Library.h"
#include "FieldLL.h"
#include "ObjectLL.h"
#include "MeshLL.h"
#include "SkeletonLL.h"
#include "KeyFrameLL.h"
#include "JointLL.h"
#include <assert.h>
#include <stdio.h>
#define DISK 0

#define TechNaturePrivate

#define NO_FUNCTION 0
#define NewScene 1

//extern long CurrentNumFields;
extern POINT LastSelectedPoint;
extern float Render_WindowWidth;
extern float Render_WindowHeight;
extern FontObject AppTools_Fonts[];




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
				struct FieldLL*& pFieldHeadIn)

{
	struct FieldLL* pFieldHead = pFieldHeadIn;
	struct FieldLL* pInsertMe = NULL;
	struct FieldLL* pThis = NULL;
	struct FieldLL* pNext = NULL;

//	long namelength = strlen(pName);
//	long textlength = strlen(pText);//error? for(i=0;i<(atoi(MeshMenuText[0]));i++) [0] is incorrect
//	assert(namelength<=256);
//	assert(textlength<=FIELD_MAX_CHAR);
	///////////////////////////////////////
	//// Initialize Linked List Packet ////
	///////////////////////////////////////
//CurrentNumFields++;

	pInsertMe = (struct FieldLL *)calloc(1,sizeof(struct FieldLL));
	pInsertMe->SortKey     = SortKey;
	sprintf(pInsertMe->pName,"%s",pName);
	sprintf(pInsertMe->pText,"%s",pText);
	pInsertMe->s           = s;
	pInsertMe->t		   = t;
	pInsertMe->width	   = width;
	pInsertMe->height	   = height;
	pInsertMe->Screen	   = Screen;
	pInsertMe->fontscale   = fontscale;
	pInsertMe->xpos		   = xpos;
	pInsertMe->ypos		   = ypos;
//	pInsertMe->screenx     = 0.0;
//	pInsertMe->screeny     = 0.0;
	pInsertMe->textcolor   = textcolor;
	pInsertMe->flags	   = flags;
	pInsertMe->cursorpos   = cursorpos;
	pInsertMe->pfloat      = pfloat;
	pInsertMe->plong       = plong;
	pInsertMe->pword       = pword;
	pInsertMe->pchar       = pchar;
	pInsertMe->pFont       = pFont;
	pInsertMe->pNext	   = NULL;

	////////////////////////////////////////////////////////////
	//// Is This The Absolute First Item Added to the list? ////
	////////////////////////////////////////////////////////////

	if(pFieldHead == NULL)
	{
		pFieldHeadIn = pInsertMe;
		return pInsertMe;
	}

	////////////////////////////////
	//// Do We Have A New Head? ////
	////////////////////////////////

	if(pInsertMe->SortKey <= pFieldHead->SortKey)
	{	
		pInsertMe->pNext  = pFieldHead;
		pFieldHeadIn      = pInsertMe;
		return pInsertMe;
	}

	/////////////////////////////////////
	//// Search For Insert Position. ////
	/////////////////////////////////////

	pThis = pFieldHead;
	while(	pThis->pNext && 
		   (pInsertMe->SortKey > pThis->pNext->SortKey))
	{
		pThis = pThis->pNext;
	}
	pNext = pThis->pNext;
	pThis->pNext = pInsertMe;
	pInsertMe->pNext = pNext;
	return pInsertMe;
}

void FieldLL_Remove(char* pName,struct FieldLL*& pFieldHeadIn)
{
	struct FieldLL* pFieldHead = pFieldHeadIn;
	struct FieldLL* pRemoveMe  = NULL;
	struct FieldLL* pThis = NULL;
	struct FieldLL* pNext = NULL;
	struct FieldLL* pLast = NULL;

	pThis = pFieldHead;
	pLast = pFieldHead;
	while(pThis!=NULL)
	{
		//if(pThis->UniqueID == UniqueID)
		if(strcmp(pThis->pName,pName)==0)
		{//remove this item
			pRemoveMe = pThis;

			pNext = pRemoveMe->pNext;

			pThis = pThis->pNext;//update this now before we delete mem pointed to by pThis

			///////////////////////////////////
			//// Are We Removing The Head? ////
			///////////////////////////////////

			if(pRemoveMe == pFieldHead)
			{//We are removing the head
				if(pNext==NULL)
				{//we are removing the last
					pFieldHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pFieldHeadIn = pNext;
				}
			}
			else
			{//We are removing a normal item
				pLast->pNext = pNext;
			}
			free(pRemoveMe);
		}
		else
		{//check next item
			pLast = pThis;
			pThis = pThis->pNext;
		}
	}
}

void FieldLL_RemoveVolatile(struct FieldLL*& pFieldHeadIn)
{
	struct FieldLL* pRemoveMe  = NULL;
	struct FieldLL* pThis = NULL;
	struct FieldLL* pNext = NULL;

	pThis = pFieldHeadIn;
	while(pThis!=NULL)
	{
		if(pThis->flags&TEXT_FLAG_VOLATILE)
		{
			pRemoveMe = pThis;
			if(pRemoveMe == pFieldHeadIn)
			{//We are removing the head
				if(pThis->pNext==NULL)
				{//we are removing the last
					pFieldHeadIn = NULL;
				}
				else
				{//then pNext is new head
					pFieldHeadIn = pThis->pNext;
				}
			}
			pThis = pThis->pNext;
			free(pRemoveMe);
		}
		else
		{
			pThis = pThis->pNext;
		}
	}
}

void FieldLL_RemoveALL(struct FieldLL*& pFieldHeadIn)
{
	struct FieldLL* pRemoveMe  = NULL;
	struct FieldLL* pThis = NULL;

	pThis = pFieldHeadIn;
	while(pThis!=NULL)
	{
		pRemoveMe = pThis;
		pThis = pThis->pNext;
		free(pRemoveMe);
	}
	pFieldHeadIn = NULL;
}

//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
void FieldLL_Sort(struct FieldLL*& pFieldHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
//SEE MeshLL_ReInsert(	struct MeshLL* 	pMesh,struct MeshLL*& pMeshHeadIn)
{
	struct FieldLL* pNewFieldHead = NULL;
	struct FieldLL* pItem = NULL;

	pItem = pFieldHeadIn;
	while(pItem!=NULL)
	{
		FieldLL_Insert(	pItem->SortKey,
						pItem->pName,
						pItem->pText,
						pItem->s,
						pItem->t,
						pItem->width,
						pItem->height,
						pItem->Screen,
						pItem->fontscale,
						pItem->xpos,
						pItem->ypos,
						pItem->textcolor,
						pItem->flags,
						pItem->cursorpos,
						pItem->pfloat,
						pItem->plong,
						pItem->pword,
						pItem->pchar,
						pItem->pFont,
						pNewFieldHead);
		pItem = pItem->pNext;
	}


	FieldLL_RemoveALL(pFieldHeadIn);

	pFieldHeadIn = pNewFieldHead;
}



//moves chars between start, and stop one char(byte) to the left.
//used in backspace, and delete
void ShiftCharsLeftOne(char* pText,long startindex,long numchars)
{
	long i;
	assert(startindex>0);//cant shift left slot 0
	assert((startindex+numchars)<=FIELD_MAX_CHAR);//cant shift left slot 0
	for(i=startindex;i<(startindex+numchars);i++)
	{

		assert((i-1)>=0);
		assert((i-1)<FIELD_MAX_CHAR);
		assert((i)>=0);
		assert((i)<FIELD_MAX_CHAR);

		pText[i-1] = pText[i];
	}
	assert((i-1)>=0);
	assert((i-1)<FIELD_MAX_CHAR);
	pText[i-1] = 0;//clear the right most char. 
}
//moves chars between start, and stop one char(byte) to the right.
//used in insert.
void ShiftCharsRightOne(char* pText,long startindex,long numchars)
{
	long i;
	long endindex = startindex+numchars;
	for(i=endindex;i>startindex;i--)
	{
		assert((i)>=0);
		assert((i)<FIELD_MAX_CHAR);
		assert((i-1)>=0);
		assert((i-1)<FIELD_MAX_CHAR);

		pText[i] = pText[i-1];
	}

	assert((i)>=0);
	assert((i)<FIELD_MAX_CHAR);
	pText[i] = ASCII_SP;//clear the left most char. 
}





void Home(FieldLL* pField)
{
	pField->cursorpos = 0;
}
void End(FieldLL* pField)
{
	long length = strlen(pField->pText);
	pField->cursorpos = length;
}
void Insert(FieldLL* pField,char Char)
{
	long length = strlen(pField->pText);
	char* StopPointer = NULL;
	if(length>=FIELD_MAX_CHAR)
	{
		return;
	}


	if(Char==ASCII_CR)
	{//InValid Character
		return;
	}


	//If editing a number, only accept 0-9, and .
	if(pField->pfloat || pField->pword || pField->plong)
	{
		if((Char>=48 && Char<=57) || Char==45 || Char==46)
		{//Valid Character
		}
		else
		{
			return;
		}		
	}	



	if(length == pField->cursorpos)
	{
		assert(pField->cursorpos>=0);
		assert(pField->cursorpos<FIELD_MAX_CHAR);

		pField->pText[pField->cursorpos] = Char;
		if(pField->pfloat)
		{
			*pField->pfloat = (float)strtod(pField->pText,&StopPointer);
		}
		if(pField->plong)
		{
			*pField->plong = (float)strtol(pField->pText,&StopPointer,10);
		}
		if(pField->pword)
		{
			*pField->pword = (float)strtol(pField->pText,&StopPointer,10);
		}
		if(pField->pchar)
		{
			pField->pchar[pField->cursorpos] = Char;
		}


		pField->cursorpos++;
	}
	else
	{
		ShiftCharsRightOne(pField->pText,pField->cursorpos,(length-pField->cursorpos));

		assert(pField->cursorpos>=0);
		assert(pField->cursorpos<FIELD_MAX_CHAR);

		pField->pText[pField->cursorpos] = Char;
		if(pField->pfloat)
		{
			*pField->pfloat = (float)strtod(pField->pText,&StopPointer);
		}
		if(pField->plong)
		{
			*pField->plong = (float)strtol(pField->pText,&StopPointer,10);
		}
		if(pField->pword)
		{
			*pField->pword = (float)strtol(pField->pText,&StopPointer,10);
		}
		if(pField->pchar)
		{
			pField->pchar[pField->cursorpos] = Char;
		}
		pField->cursorpos++;
	}
}
void Delete(FieldLL* pField)
{
	char* StopPointer = NULL;
	long length = strlen(pField->pText);

	if(length == 0)
	{
		return;
	}

	if(	pField->pfloat && 
		pField->pText[pField->cursorpos] == '.')
	{//cant delete decimal point
		return;
	}

	ShiftCharsLeftOne(pField->pText,(pField->cursorpos+1),(length-(pField->cursorpos+1)));

	//Update Actual Value
	if(pField->pfloat)
	{
		*pField->pfloat = (float)strtod(pField->pText,&StopPointer);
	}
	if(pField->plong)
	{
		*pField->plong = (float)strtol(pField->pText,&StopPointer,10);
	}
	if(pField->pword)
	{
		*pField->pword = (float)strtol(pField->pText,&StopPointer,10);
	}
	if(pField->pchar)
	{
		length = strlen(pField->pchar);
		if(length >0)
		{
			ShiftCharsLeftOne(pField->pchar,(pField->cursorpos+1),(length-(pField->cursorpos+1)));
		}
	}
}
//backspace does the following:
//1) shiftleft from cursor to end of text string
void BackSpace(FieldLL* pField)
{
	char* StopPointer = NULL;
	long length = strlen(pField->pText);
	if(length == 0)
	{
		return;
	}

	if(pField->cursorpos==0)
	{
		return;
	}

	if(	pField->pfloat && 
		pField->pText[pField->cursorpos-1] == '.')
	{//cant delete decimal point
		return;
	}

	ShiftCharsLeftOne(pField->pText,pField->cursorpos,(length-pField->cursorpos));

	//Update Actual Value
	if(pField->pfloat)
	{
		*pField->pfloat = (float)strtod(pField->pText,&StopPointer);
	}
	if(pField->plong)
	{
		*pField->plong = (float)strtol(pField->pText,&StopPointer,10);
	}
	if(pField->pword)
	{
		*pField->pword = (float)strtol(pField->pText,&StopPointer,10);
	}
	if(pField->pchar)
	{
		ShiftCharsLeftOne(pField->pchar,pField->cursorpos,(length-pField->cursorpos));
	}

	pField->cursorpos--;
}



//returns index of char in field's text array.
int PickFieldChar(struct SceneLL* pScene,FieldLL* pField,FontObject* pFont)
{
	fRECT CharRect;
	long i;
	char Ascii = 0;
	char* pText = &pField->pText[0];
	long length = strlen(pText);	
	float PosX = 0.0;
	POINT ptCursor;
	float ClipX,ClipY,ClipW,ClipH;
	fRECT Clip;
	VERTEX_XYZNRGBUV* pSrcV = NULL;//local space text chars. 32 units wideXYZ, 32 texels across
	VERTEX_XYZNRGBUV pCamV[4];//camera space. cam at 0,0,0 text on near plane
	long cDstV = 0;

	assert(length<=FIELD_MAX_CHAR);

//    GetCursorPos(&ptCursor);
//    ScreenToClient(SavehWnd,&ptCursor);
	ptCursor = LastSelectedPoint;
	for(i=0;i<length;i++)
	{
		Ascii = pText[i];

		if(Ascii>=32 && Ascii<=126)
		{//Valid Character
			pSrcV = &pFont->ptlV[(Ascii*4)];
			if(Ascii==ASCII_SP)
			{//space
				pSrcV = &pFont->ptlV[(SPACE_CHAR*4)];//@
			}

			//KERNING PART 1
			if(Ascii==ASCII_SP)
			{//space
				if(pField->flags&TEXT_FLAG_KERNING)
				{
					PosX -= pFont->pCharRectXY[SPACE_CHAR].left*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
			else
			{//space
				if(pField->flags&TEXT_FLAG_KERNING)
				{
					PosX -= pFont->pCharRectXY[Ascii].left*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}

			///////////////////////////////////////////////////
			//// Calculate Screen Space Clipping Rect      ////
			//// Assums No Camera Rotation, or Translation ////
			///////////////////////////////////////////////////

			ClipX = pField->s*Render_WindowWidth;
			ClipY = pField->t*Render_WindowHeight;
			ClipW = pField->width*Render_WindowWidth;
			ClipH = pField->height*Render_WindowHeight;

			Clip.left   = ClipX;
			Clip.right  = ClipX+ClipW;
			Clip.top    = ClipY-(ClipH/2.0);
			Clip.bottom = ClipY+(ClipH/2.0);

			//transform pixel width rect to screen space rect
//			ClipSS.left   = (((2.0*Clip.left  )/Render_WindowWidth )-1.0)/pScene->pProj[0];
//			ClipSS.right  = (((2.0*Clip.right )/Render_WindowWidth )-1.0)/pScene->pProj[0];
//			ClipSS.top    = (((2.0*Clip.top   )/Render_WindowHeight)-1.0)/pScene->pProj[5];
//			ClipSS.bottom = (((2.0*Clip.bottom)/Render_WindowHeight)-1.0)/pScene->pProj[5];


			///////////////////////////////////////////////
			//// Calculate Screen Space Character Rect ////
			///////////////////////////////////////////////

			memcpy(pCamV,pSrcV,4*sizeof(VERTEX_XYZNRGBUV));

			//make cursor the height of field. Leaving width the same as current char
			pCamV[0].y = -ClipH*0.5;
			pCamV[1].y = -ClipH*0.5;

			pCamV[2].y = +ClipH*0.5;
			pCamV[3].y = +ClipH*0.5;

			//scale text
			pCamV[0].x *= pField->fontscale;
			pCamV[0].y *= pField->fontscale;

			pCamV[1].x *= pField->fontscale;
			pCamV[1].y *= pField->fontscale;

			pCamV[2].x *= pField->fontscale;
			pCamV[2].y *= pField->fontscale;

			pCamV[3].x *= pField->fontscale;
			pCamV[3].y *= pField->fontscale;
			//position text 
			pCamV[0].x += pField->s*Render_WindowWidth;
			pCamV[0].y += pField->t*Render_WindowHeight;

			pCamV[1].x += pField->s*Render_WindowWidth;
			pCamV[1].y += pField->t*Render_WindowHeight;

			pCamV[2].x += pField->s*Render_WindowWidth;
			pCamV[2].y += pField->t*Render_WindowHeight;

			pCamV[3].x += pField->s*Render_WindowWidth;
			pCamV[3].y += pField->t*Render_WindowHeight;

			pCamV[0].x += PosX;
			pCamV[1].x += PosX;
			pCamV[2].x += PosX;
			pCamV[3].x += PosX;

			pCamV[0].x -= pField->xpos;
			pCamV[1].x -= pField->xpos;
			pCamV[2].x -= pField->xpos;
			pCamV[3].x -= pField->xpos;

			pCamV[0].y -= pField->ypos;
			pCamV[1].y -= pField->ypos;
			pCamV[2].y -= pField->ypos;
			pCamV[3].y -= pField->ypos;


			CharRect.left   = pCamV[0].x;
			CharRect.right  = pCamV[3].x;
			CharRect.top    = pCamV[0].y;
			CharRect.bottom = pCamV[3].y;

			if(PointInRect(ptCursor,CharRect))
			{
				pField->cursorpos = i;
				return i;
			}

			//KERNING PART 2
			if(Ascii==ASCII_SP)
			{//space
				if(pField->flags&&TEXT_FLAG_KERNING)
				{
					PosX += pFont->pCharRectXY[SPACE_CHAR].right*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
			else
			{
				if(pField->flags&&TEXT_FLAG_KERNING)
				{
					PosX += pFont->pCharRectXY[Ascii].right*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
		}
	}
	return -1;
}

FieldLL* SelectField(struct SceneLL* pScene,FieldLL* pFieldHeadIn,POINT ptCursor)
{
	float ClipX,ClipY,ClipW,ClipH;
	fRECT Clip;
	FieldLL* pField;

	pField = pFieldHeadIn;
	while(pField)
	{
		if(pField->flags & TEXT_FLAG_EDITABLE)
		{
			ClipX = pField->s*Render_WindowWidth;
			ClipY = pField->t*Render_WindowHeight;
			ClipW = pField->width*Render_WindowWidth;
			ClipH = pField->height*Render_WindowHeight;

			Clip.left   = ClipX;
			Clip.right  = ClipX+ClipW;
			Clip.top    = ClipY-(ClipH/2.0);
			Clip.bottom = ClipY+(ClipH/2.0);

			if(PointInRect(ptCursor,Clip))
			{
				PickFieldChar(pScene,pField,&AppTools_Fonts[0]);
				return pField;
			}
		}
		pField = pField->pNext;
	}
	return NULL;
}







//FieldLL.cpp changes pField->cursorpos;
//Render.cpp changes pField->xpos to keep cursor visible
//WM_KEYDOWN handles arrow movement, and the esc key. WM_CHAR gets chars from keyboard
void FieldLL_WM_KEYDOWN(FieldLL* pField,WPARAM wParam, LPARAM lParam)
{//KEYDOWN doesnt insert into field
	int Key = (int)wParam;
	assert(pField);
	long length = strlen(pField->pText);

	if(!IsBitSet(pField->flags,TEXT_FLAG_EDITABLE))
	{
		assert(0);//not edatable, !!!
	}


	if(Key == VK_LEFT)
	{
		if(pField->cursorpos>0)
		{
			pField->cursorpos--;
		}
	}
	else if(Key == VK_RIGHT)
	{
		if(pField->cursorpos<length)//clamp to string length?
		{
			pField->cursorpos++;
		}
	}
	else if(Key==VK_BACK)
	{
		BackSpace(pField);
	}
	else if(Key==VK_DELETE)
	{
		Delete(pField);
	}
	if(Key==VK_HOME)
	{
		Home(pField);
	}
	if(Key==VK_END)
	{
		End(pField);
	}
}



//FieldLL.cpp changes pField->cursorpos;
//Render.cpp changes pField->xpos to keep cursor visible
//WM_CHAR gets chars from keyboard. WM_KEYDOWN handles arrow movement, and the esc key.
void FieldLL_WM_CHAR(FieldLL* pField,WPARAM wParam, LPARAM lParam)
{
	int Key = (int)wParam;
	if(Key==VK_BACK)
	{
		return;
	}
	Insert(pField,Key);
}


float CalcWidthField(FieldLL* pField,FontObject* pFont)
{
	VERTEX_XYZNRGBUV* pSrcV = NULL;//local space text chars. 32 units wideXYZ, 32 texels across
	long i;
	char Ascii = 0;
	char* pText = NULL;
	long length;	
	float PosX = 0.0;

	if(	pField==NULL ||	pField->pText==NULL)return 0.0;

	pText = &pField->pText[0];
	length = strlen(pText);	
	assert(length<=FIELD_MAX_CHAR);
	for(i=0;i<length;i++)
	{
		Ascii = pText[i];
		if(Ascii==0)
		{//cursor is past last char in text string.
			Ascii=ASCII_SP;//tell cursor to emulate space char
		}
		if(Ascii>=32 && Ascii<=126)
		{//Valid Character
			pSrcV = &pFont->ptlV[(Ascii*4)];
			if(Ascii==ASCII_SP)
			{//space
				pSrcV = &pFont->ptlV[(SPACE_CHAR*4)];//@
			}
	
			//KERNING PART 1
			if(Ascii==ASCII_SP)
			{//space
				if(pField->flags&TEXT_FLAG_KERNING)
				{
					PosX -= pFont->pCharRectXY[SPACE_CHAR].left*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
			else
			{//space
				if(pField->flags&TEXT_FLAG_KERNING)
				{
					PosX -= pFont->pCharRectXY[Ascii].left*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
			

			//KERNING PART 2
			if(Ascii==ASCII_SP)
			{//space
				if(pField->flags&&TEXT_FLAG_KERNING)
				{
					PosX += pFont->pCharRectXY[SPACE_CHAR].right*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
			else
			{
				if(pField->flags&&TEXT_FLAG_KERNING)
				{
					PosX += pFont->pCharRectXY[Ascii].right*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}
		}
	}
	return PosX;
}






float CalcWidthString(char* pText,FontObject* pFont,bool bKerning)
{
	float fontscale = 1.0;

	VERTEX_XYZNRGBUV* pSrcV = NULL;//local space text chars. 32 units wideXYZ, 32 texels across
	long i;
	char Ascii = 0;
	long length;	
	float PosX = 0.0;

	if(pText==NULL)return 0.0;

	length = strlen(pText);	
	assert(length<=FIELD_MAX_CHAR);
	for(i=0;i<length;i++)
	{
		Ascii = pText[i];
		if(Ascii==0)
		{//cursor is past last char in text string.
			Ascii=ASCII_SP;//tell cursor to emulate space char
		}
		if(Ascii>=32 && Ascii<=126)
		{//Valid Character
			pSrcV = &pFont->ptlV[(Ascii*4)];
			if(Ascii==ASCII_SP)
			{//space
				pSrcV = &pFont->ptlV[(SPACE_CHAR*4)];//@
			}
	
			//KERNING PART 1
			if(Ascii==ASCII_SP)
			{//space
				if(bKerning)
				{
					PosX -= pFont->pCharRectXY[SPACE_CHAR].left*fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*fontscale;
				}
			}
			else
			{//space
				if(bKerning)
				{
					PosX -= pFont->pCharRectXY[Ascii].left*fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*fontscale;
				}
			}
			

			//KERNING PART 2
			if(Ascii==ASCII_SP)
			{//space
				if(bKerning)
				{
					PosX += pFont->pCharRectXY[SPACE_CHAR].right*fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*fontscale;
				}
			}
			else
			{
				if(bKerning)
				{
					PosX += pFont->pCharRectXY[Ascii].right*fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*fontscale;
				}
			}
		}
	}
	return PosX;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//1 find left, and right edge of cursor
//2 force xpos allows cursor to be fully visible.
//3 adjust xpos accordingly.
void FieldKeepCursorInView(FieldLL* pField,FontObject* pFont)
{
	long i;
	char Ascii = 0;
	char* pText = &pField->pText[0];
	long length = strlen(pText);	
	float PosX = 0.0;
	assert(length<=FIELD_MAX_CHAR);
	long LeftX,RightX;
	

	VERTEX_XYZNRGBUV* pSrcV = NULL;//local space text chars. 32 units wideXYZ, 32 texels across
	VERTEX_XYZNRGBUV* pDstV = NULL;
	long cDstV = 0;

	if(length==pField->cursorpos)
	{//cursor is past last char in text string.
		length++;//take one more step to include the cursor.
	}
	for(i=0;i<length;i++)
	{
		Ascii = pText[i];
		if(Ascii==0)
		{//cursor is past last char in text string.
			Ascii=ASCII_SP;//tell cursor to emulate space char
		}
/*
		if(Ascii==ASCII_SP)
		{//space
			pSrcV = &pFont->pV[(SPACE_CHAR*4)];//@
			PosX -= pFont->pCharRectXY[SPACE_CHAR].left;
			PosX += pFont->pCharRectXY[SPACE_CHAR].right;
		}
*/
		if(Ascii>=32 && Ascii<=126)
		{//Valid Character
			pSrcV = &pFont->ptlV[(Ascii*4)];

			//Kerning Part 1. 
			//Add space to left for kerning.
			// add Left width to current write position, PosX.
			if(i==pField->cursorpos)
			{//render cursor!
				
				LeftX = PosX - pField->xpos;
				if(LeftX<0)
				{
					pField->xpos += LeftX;
				}


				if(pField->flags&TEXT_FLAG_KERNING)
				{
					PosX -= pFont->pCharRectXY[Ascii].left*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
				//Kerning Part 2. 
				if(pField->flags&&TEXT_FLAG_KERNING)
				{
					PosX += pFont->pCharRectXY[Ascii].right*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}

				float Width;
				Width  = pField->width*Render_WindowWidth;
//				Width  = pField->width;
				RightX = PosX - pField->xpos;
				if(RightX>Width)
				{
					pField->xpos += (RightX-Width);
				}
			}
			else//not a selected character
			{//Keep searching for selected character 
				if(pField->flags&TEXT_FLAG_KERNING)
				{
					PosX -= pFont->pCharRectXY[Ascii].left*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
				//Kerning Part 2. 
				if(pField->flags&&TEXT_FLAG_KERNING)
				{
					PosX += pFont->pCharRectXY[Ascii].right*pField->fontscale;
				}
				else
				{
					PosX += (pFont->CharWidth/2)*pField->fontscale;
				}
			}

		}
	}
}




