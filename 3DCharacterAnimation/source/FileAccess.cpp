//WindowsXP.cpp written by david paull.
//	System Dependent Functions.
//	this will be packed into dll(like API dlls.) when MacOSX.cpp is created.
// Copyright (c) 2002 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>//tolower
#include <time.h>//clock()
#include <assert.h>
#include "Library.h"
#include "FileAccess.h"
#include "MeshLL.h"
#include "SkeletonLL.h"
#include "KeyFrameLL.h"
#include "JointLL.h"

#define DISK 1
#define MAX_CALLS 10000
#define EXIT_WHEN_DONE 0


/////////////////////
//// Extern Data ////
/////////////////////

//extern FILE* DiskFile;
//extern aVECTOR CameraPos;
extern float pRender_IdentityMatrix[];
extern FILE* DiskFile;
extern char* gp_TextBuffer1;
extern char* gp_TextBuffer2;
extern char* gp_TextBuffer3;

////////////////////
//// Local Data ////
////////////////////

typedef struct _LOADTXT_RGBA
{
	float r;
	float g;
	float b;
	float a;
}LOADTXT_RGBA;

typedef struct _LOADTXT_TXYZ
{
	float time;
	float x;
	float y;
	float z;
}LOADTXT_TXYZ;
typedef struct _LOADTXT_DATA
{
	char pName[256];
	char pParentName[256];
	float p_x;//restpose
	float p_y;
	float p_z;
	float r_x;
	float r_y;
	float r_z;

	unsigned long num_position_keys;
	unsigned long num_rotation_keys;
	
	LOADTXT_TXYZ Position[1000];
	LOADTXT_TXYZ Rotation[1000];
}LOADTXT_DATA;

typedef struct _LOADTXT_MATERIAL
{
	char name[256];
	LOADTXT_RGBA ambient;
	LOADTXT_RGBA diffuse;
	LOADTXT_RGBA specular;
	LOADTXT_RGBA emissive;
	float power;
	float alpha;
	char texname1[256];
	char texname2[256];
}LOADTXT_MATERIAL;

////////////////////////////////////
//// Extern Function Prototypes ////
////////////////////////////////////
extern void AppendDebugMsg(char *pChar);

///////////////////////////////////
//// Local Function Prototypes ////
///////////////////////////////////
void AppendDebugMsg(char *pChar);

void SetFileHeader(	LPBITMAPFILEHEADER	fileHeader,
					long bfType,
					long bfSize,
					long bfReserved1,
					long bfReserved2,
					long bfOffBits);
void SetInfoHeader(	LPBITMAPINFOHEADER	infoHeader,
				  	long biSize,
				  	long biWidth,
				  	long biHeight,
				  	long biPlanes,
				  	long biBitCount,
				  	long biCompression,
				  	long biSizeImage,
				  	long biXPelsPerMeter,
				  	long biYPelsPerMeter,
				  	long biClrUsed,
				  	long biClrImportant);

////////////////////////////////////////////////////////////////////////////////////

void AddPathToFileName(char *pFileName)
{
	char OriginalName[512];
	char FullPath[512];
	char Path[512];
	char FileName[512];

	memset(OriginalName,0,512);
	memset(FullPath,0,512);
	memset(Path,0,512);
	memset(FileName,0,512);

	strncpy(OriginalName,pFileName,strlen(pFileName));
	GetModuleFileName(NULL,FullPath,512);
	char *c = strrchr(FullPath, 92);//92 = backslash
	if(c)
	{
		int len = (int)(c - FullPath);
		strncpy(Path,FullPath,len);
		sprintf(pFileName,"%s\\%s",Path,OriginalName);
	}
	else{}//no \ backslash found in name.
}

//removes path leaving program name, and extension
//in  : c:\sourcecode\render.exe
//out : render.exe
void RemovePathFromFileName(char *pFileName)
{
	char pName[256];
	char *pChar = strrchr(pFileName, 92);//92 = backslash
	if(pChar)
	{
		pChar++;//step off /

		int len = 256-((int)(pChar-pFileName));

		memset(pName,0,256*sizeof(char));
		
		strncpy(pName,pChar,len);

		sprintf(pFileName,"%s",pName);

	}
	else{}//no \ backslash found in name.
}

//removes program name, and extension
//in  : c:\sourcecode\render.exe
//out : c:\sourcecode\
//
//
void RemoveFilenameAndExt(char *pFileName)
{
	char pName[256];
	char *pChar = strrchr(pFileName, 92);//92 = backslash
	if(pChar)
	{
//		pChar++;//step off /

		int len = (int)(pChar-pFileName);

		memset(pName,0,256*sizeof(char));
		
		strncpy(pName,pFileName,len);

		sprintf(pFileName,"%s",pName);

	}
	else{}//no \ backslash found in name.
}
void GetPath(char *pFileName)
{
	char* p_TempBuffer1 = (char *)calloc(MAX_PATHNAME,sizeof(char));
	char* p_TempBuffer2 = (char *)calloc(MAX_PATHNAME,sizeof(char));

	GetModuleFileName(NULL,p_TempBuffer1,MAX_PATHNAME);
	RemoveFilenameAndExt(p_TempBuffer1);

	char *pChar = strrchr(pFileName, 92);//92 = backslash
	if(pChar)
	{
		pChar++;//step off /

		int length = (int)(pChar-pFileName);

		memset(p_TempBuffer2,0,MAX_PATHNAME);
		memcpy(p_TempBuffer2,pFileName,length);

		if(	isalpha(p_TempBuffer2[0]) &&
		   	p_TempBuffer2[1] == ':' && 
			p_TempBuffer2[2] == 92)
		{//c:\   user has provided absolute address
			sprintf(pFileName,"%s",p_TempBuffer2);
		}
		else
		{
			sprintf(pFileName,"%s\\%s",p_TempBuffer1,p_TempBuffer2);
		}
	}
	else
	{
		sprintf(pFileName,"%s\\",p_TempBuffer1);
	}//no \ backslash found in name.


	SAFE_FREE(p_TempBuffer1);
	SAFE_FREE(p_TempBuffer2);
}



//removes program name, and extension
//in  : c:\sourcecode\render.exe
//out : c:\sourcecode\render
void RemoveExtensionFromFileName(char *pFileName)
{
	char OriginalName[512];
	char pName[512];

	memset(OriginalName,0,512);
	memset(pName,0,512);

	sprintf(OriginalName,"%s",pFileName);

	char *c = strrchr(OriginalName,46);//46 = .
	if(c)
	{
		int len = (int)(c - OriginalName);
		strncpy(pName,OriginalName,len);
		sprintf(pFileName,"%s",pName);
	}
	else{}//no . dot found in name. no extension

}

bool IsValidFilename(char* pName)
{
	long length;

	if(pName==NULL) return false;

	length = strlen(pName);
	if(length==0 || length>=MAX_PATHNAME)
	{
		return false;
	}
	if(strchr(pName,ASCII_CR))
	{
		return false;
	}
	return true;
}

bool LoadBMP(	char* pFileName,
			 	unsigned char*& pBits,
			 	long&  Width,
			 	long&  Height)
{
	unsigned char *pSrc = NULL;
	HBITMAP hbm;
	BITMAP bmData;
	char FullName[512];

	sprintf(FullName,"%s.bmp",pFileName);
	AddPathToFileName(FullName);

	hbm = (HBITMAP)LoadImage(NULL,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hbm==NULL)//file not found
	{
		sprintf(FullName,"default.bmp");
		AddPathToFileName(FullName);
		hbm = (HBITMAP)LoadImage(NULL,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

		GetObject(hbm,sizeof(BITMAP),&bmData);
		pSrc   = (unsigned char *)bmData.bmBits;
		Width  = bmData.bmWidth;
		Height = bmData.bmHeight;
		pBits  = (unsigned char *)calloc(Width*Height*3,sizeof(unsigned char));

		for(int j=0;j<Height;j++)
		{
			memcpy(&pBits[j*Width*3],&pSrc[(Height-j-1)*bmData.bmWidthBytes],Width*3*sizeof(char));
		}
    	return true;
	}
	else//normal load bitmap
	{
		GetObject(hbm,sizeof(BITMAP),&bmData);
		pSrc   = (unsigned char *)bmData.bmBits;
		Width  = bmData.bmWidth;
		Height = bmData.bmHeight;
		pBits  = (unsigned char *)calloc(Width*Height*3,sizeof(unsigned char));

		for(int j=0;j<Height;j++)
		{
			memcpy(&pBits[j*Width*3],&pSrc[(Height-j-1)*bmData.bmWidthBytes],Width*3*sizeof(char));
		}

//		for(int j=Height-1;j>=0;j--)
/*
		for(int j=0;j<Height;j++)
		{
			memcpy(&pBits[j*Width*3],&pSrc[(j)*bmData.bmWidthBytes],Width*3*sizeof(char));
		}
*/
    	return true;
	}
    return false;
}

//write 24 bit image to disk
bool SaveBMP(	char* pFileName,
				unsigned char* pBits,
				long  Width,
				long  Height) 
{ 
	BOOL bResult = FALSE;
	HANDLE fH;
	int returnval;
	DWORD size,justwritten,byteswritten,count;


	//fix for non pow2 sizes. need to test when activated. load needs similar help. width is padded to pow2 if it isnt already.
	//#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
	//DWORD save_pitch = WIDTHBYTES(Width * 24);

	char FullName[512];

	sprintf(FullName,"%s.bmp",pFileName);
	AddPathToFileName(FullName);



	
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	long HeaderSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	//long ImageSize  = save_pitch*Height;
	long ImageSize  = Width*Height*3;
	long FileSize   = HeaderSize+ImageSize;


	SetFileHeader(&fileHeader,19778,FileSize,0,0,HeaderSize);
	SetInfoHeader(&infoHeader,
				  sizeof(BITMAPINFOHEADER),
				  Width,Height,1,24,0,ImageSize,0,0,0,0);

	fH = CreateFile(FullName,GENERIC_WRITE,
   	    0, (LPSECURITY_ATTRIBUTES) NULL,
   	    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
   	    (HANDLE) NULL);

	DWORD LastError = GetLastError();
	if(LastError==183)
	{
	}
	else if(LastError==5)//file is read only
	{
		return false;
	}
	else
	{
		assert(LastError==0);//already exists warning
	}
	
	////////////////////////////////////
	//// Write The BitmapFileHeader ////
	////////////////////////////////////

	byteswritten = 0;
	size = sizeof(BITMAPFILEHEADER);
	while(byteswritten<size)
	{
//IF YOU ARE STUCK IN THIS LOOP, YOU PASSED IN THE WRONG WIDTH, AND HEIGHT
      	count = size-byteswritten;
		if(count>30000)count=30000;
		bResult = WriteFile(fH,(LPCVOID)&((&fileHeader)[byteswritten]),count,&justwritten,NULL);
		byteswritten += justwritten;
//IF YOU ARE STUCK IN THIS LOOP, YOU PASSED IN THE WRONG WIDTH, AND HEIGHT
	}

	////////////////////////////////////
	//// Write The BitmapInfoHeader ////
	////////////////////////////////////

	byteswritten = 0;
	size = sizeof(BITMAPINFOHEADER);
	while(byteswritten<size)
	{
      	count = size-byteswritten;
		if(count>30000)count=30000;
		bResult = WriteFile(fH,(LPCVOID)&((&infoHeader)[byteswritten]),count,&justwritten,NULL);
		byteswritten += justwritten;
	}

	/////////////////////////////////////
	//// Write The Bitmap Image Data ////
	/////////////////////////////////////


	for(int j=0;j<Height;j++)
	{//BMP store the image upside down
		unsigned char *pSrc = &pBits[((Height-j-1)*Width*3)];
		byteswritten = 0;
		size = Width*3;
		//size = save_pitch;
		while(byteswritten<size)
		{
    	  	count = size-byteswritten;
			if(count>30000)count=30000;
			bResult = WriteFile(fH,(LPCVOID)&pSrc[byteswritten],count,&justwritten,NULL);
			byteswritten += justwritten;
		}
	}
	returnval = CloseHandle(fH);
   	return true;
} 

//API Independent Load Texture.
//load 24bit bmp from disk
//currently, alpha chanel is created here based on color intensity 



void SetFileHeader(	LPBITMAPFILEHEADER	fileHeader,
					long bfType,
					long bfSize,
					long bfReserved1,
					long bfReserved2,
					long bfOffBits)
{
	fileHeader->bfType      = bfType     ;
	fileHeader->bfSize      = bfSize     ;
	fileHeader->bfReserved1 = bfReserved1;
	fileHeader->bfReserved2 = bfReserved2;
	fileHeader->bfOffBits   = bfOffBits  ;
}
void SetInfoHeader(	LPBITMAPINFOHEADER	infoHeader,
				  	long biSize,
				  	long biWidth,
				  	long biHeight,
				  	long biPlanes,
				  	long biBitCount,
				  	long biCompression,
				  	long biSizeImage,
				  	long biXPelsPerMeter,
				  	long biYPelsPerMeter,
				  	long biClrUsed,
				  	long biClrImportant)
{
	infoHeader->biSize          = biSize         ;
	infoHeader->biWidth         = biWidth        ;
	infoHeader->biHeight        = biHeight       ;
	infoHeader->biPlanes        = biPlanes       ;
	infoHeader->biBitCount      = biBitCount     ;
	infoHeader->biCompression   = biCompression  ;
	infoHeader->biSizeImage     = biSizeImage    ;
	infoHeader->biXPelsPerMeter = biXPelsPerMeter;
	infoHeader->biYPelsPerMeter = biYPelsPerMeter;
	infoHeader->biClrUsed       = biClrUsed      ;
	infoHeader->biClrImportant  = biClrImportant ;
}


void ReadFromFile(HANDLE fH,char* pData,long Size)
{
	BOOL bResult = FALSE;
	long count = 0;
	unsigned long justread = 0;
	long bytesread = 0;

	//if you get stuck in this loop, it is possible that you are loading old data that doesnt follow the new format
	while(bytesread<Size)
	{
      	count = Size-bytesread;
		if(count>30000)count=30000;

		bResult = ReadFile(fH,(LPVOID)&pData[bytesread],count,&justread,NULL);
		bytesread += justread;
	}
}
void WriteToFile(HANDLE fH,char* pData,long Size)
{
	BOOL bResult = FALSE;
	long count = 0;
	unsigned long justwritten = 0;
	long byteswritten = 0;
	while(byteswritten<Size)
	{
		//limit packet size to 30000
	  	count = Size-byteswritten;
		if(count>30000)count=30000;

		bResult = WriteFile(fH,(LPCVOID)&pData[byteswritten],count,&justwritten,NULL);
		byteswritten += justwritten;
	}
}

unsigned int ReadNextInteger(char *ch)
{
	char Integer[128];
	unsigned int index;

	if(DiskFile==NULL)return 0;//not quite correct, but better than crashing.

	/////////////////////
	//// Skip Spaces ////
	/////////////////////
	while((*ch==ASCII_SP)  || 
		  (*ch==ASCII_TAB) || 
		  (*ch==ASCII_LF))
	{
		*ch = fgetc(DiskFile);if(*ch==EOF)goto LABEL_DONE_READING_INTEGER;
	}

	////////////////////
	//// Get Number ////
	////////////////////

	index = 0;
	memset(Integer,0,128);
	while((*ch!=ASCII_SP)  && 
		  (*ch!=ASCII_TAB) && 
		  (*ch!=ASCII_LF))
	{
		Integer[index] = *ch;
		index++;
		*ch = fgetc(DiskFile);if(*ch==EOF)goto LABEL_DONE_READING_INTEGER;
	}
	return(atol(Integer));
	LABEL_DONE_READING_INTEGER:;
	return 0;
}
float ReadNextFloat(char *ch)
{
	char Float[128];
	unsigned int index;

	if(DiskFile==NULL)return 0.0;//not quite correct, but better than crashing.

	/////////////////////
	//// Skip Spaces ////
	/////////////////////
	while((*ch==ASCII_SP)  || 
		  (*ch==ASCII_TAB) || 
		  (*ch==ASCII_LF))
	{
		*ch = fgetc(DiskFile);if(*ch==EOF)goto LABEL_DONE_READING_FLOAT;
	}

	////////////////////
	//// Get Number ////
	////////////////////

	index = 0;
	memset(Float,0,128);
	while((*ch!=ASCII_SP)  && 
		  (*ch!=ASCII_TAB) && 
		  (*ch!=ASCII_LF))
	{
		Float[index] = *ch;
		index++;
		*ch = fgetc(DiskFile);if(*ch==EOF)goto LABEL_DONE_READING_FLOAT;
	}
	return(atof(Float));
	LABEL_DONE_READING_FLOAT:;
	return 0;
}












void OutputMatrix(float* pMat)
{
	sprintf(gp_TextBuffer3,"	%f	%f	%f	%f\n",pMat[0],pMat[1],pMat[2],pMat[3]);
	AppendDebugMsg(gp_TextBuffer3);
	sprintf(gp_TextBuffer3,"	%f	%f	%f	%f\n",pMat[4],pMat[5],pMat[6],pMat[7]);
	AppendDebugMsg(gp_TextBuffer3);
	sprintf(gp_TextBuffer3,"	%f	%f	%f	%f\n",pMat[8],pMat[9],pMat[10],pMat[11]);
	AppendDebugMsg(gp_TextBuffer3);
	sprintf(gp_TextBuffer3,"	%f	%f	%f	%f\n",pMat[12],pMat[13],pMat[14],pMat[15]);
	AppendDebugMsg(gp_TextBuffer3);
}

void InitDebugMsg()
{
	DiskFile=fopen("errors.txt","w");
	DWORD error = GetLastError();

	if(DiskFile)
	{
		fprintf(DiskFile,"Tanzanite Software - TN Animator Log File.\n");
		fclose(DiskFile);
	}
	else
	{
		//your log.log file is readonly, or disk is full.
	}
}

void AppendDebugMsg(char *pChar)
{
	static unsigned long num_calls = 0;

	if(DISK)
	{
		if(num_calls<MAX_CALLS)
		{
			#ifdef OUTPUT_TO_DEBUGGER
			sprintf(gp_TextBuffer3,"%s\n",pChar);
			OutputDebugString(gp_TextBuffer3);
			#endif

			#ifdef OUTPUT_TO_FILE
			DiskFile=fopen("errors.txt","a");
			if(DiskFile)
			{
				fprintf(DiskFile,"%s\n",pChar);
				fclose(DiskFile);
			}
			#endif
			num_calls++;
		}
		else
		{
			if(EXIT_WHEN_DONE)PostQuitMessage(0);
		}
	}
}

//handles negative number spacing correctly
void fprintfloat(float value,bool cr)
{
	if(value>=0.0)
	{
		fprintf(DiskFile,"	 %f",value);
	}
	else
	{
		fprintf(DiskFile,"	%f",value);
	}
	if(cr)
	{
		fprintf(DiskFile,"\n",value);
	}
}






//void LoadSprocket(struct MeshLL* pMesh)
//Loads 3DSMAX .ase files without bones. geometry only
void LoadASE(struct MeshLL* pMesh,float Scale,char* pFileName,char* pTextureFileName)
{
	char FileName[512];
	char FullPath[512];
	char Path[512];
	static long PartCounter = 0;
//	float 	  Scale = 1.0;
	aVECTOR Translate;
	char Command[512];
	char ch;
	unsigned int index;
	unsigned int VertexIndex;
	unsigned int FaceIndex;
	unsigned int Unused;
	unsigned int LoadedVerticies = 0;
	unsigned int LoadedFaces     = 0;
	bool Done = false;
	char* GEOMOBJECT = "GEOMOBJECT";
	char* NUM_VERTEX = "MESH_NUMVERTEX";
	char* NUM_FACES  = "MESH_NUMFACES";
	char* VERTEX     = "MESH_VERTEX";
	char* FACE       = "MESH_FACE";
	char* TVERTEX = "MESH_TVERT";
	char* NUM_TVERTEX = "MESH_NUMTVERTEX";
	char* PROP_MOTIONBLUR = "PROP_MOTIONBLUR";
	long temp = (int)(RandZeroToOne*255);
	long Diffuse = 0xffffffff;

	VERTEX_XYZNRGBUV* pVertex = NULL;
	WORD* pIndicies = NULL;



	Translate.x = 0.0;
	Translate.y = 0.0;
	Translate.z = 0.0;

	PartCounter = 0;

	memset(FullPath,0,512);
	memset(Path,0,512);
	memset(FileName,0,512);
	GetModuleFileName(NULL,FullPath,512);
	char *c = strrchr(FullPath, 92);//92 = backslash
	int len = (int)(c - FullPath);
	strncpy(Path,FullPath,len);
//	sprintf(FileName,"%s\\Sprocketa.ase",Path);
	sprintf(FileName,"%s\\%s",Path,pFileName);


	assert(pMesh->pLocalVerts==NULL);
	assert(pMesh->pIndices==NULL);

//	pMesh->pWorldVerts = (VERTEX_XYZNRGBUV *)calloc(10000,sizeof(VERTEX_XYZNRGBUV));
//	pMesh->pIndices  = (WORD *)calloc(10000,sizeof(WORD));

	pMesh->pLocalVerts    = (VERTEX_XYZNRGBUV   *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
	pMesh->pWorldVerts    = (VERTEX_XYZNRGBUV   *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
	pMesh->pIndices  = (WORD            *)calloc(MAX_INDICES ,sizeof(WORD));

//	pMesh->pWorldVerts    = (VERTEX_XYZNRGBUV   *)calloc(700,sizeof(VERTEX_XYZNRGBUV));
//	pMesh->pIndices  = (WORD            *)calloc(700,sizeof(WORD));

	pVertex = pMesh->pLocalVerts;
	pIndicies = pMesh->pIndices;


 	if((DiskFile = fopen(FileName,"r+t")) != NULL )
	{
		while(1)
		{
			ch = fgetc(DiskFile);
			if(ch==EOF)
			{
				goto LABEL_DONE_READING_ASE;
			}
			if(ch == '*')
			{
				memset(Command,0,128);
				index = 0;
				//////////////////////////
				//// Fin Next Command ////
				//////////////////////////
				ch = fgetc(DiskFile);
				while((ch!=ASCII_SP) && 
					  (ch!=ASCII_TAB) && 
					  (ch!=ASCII_LF))
				{
					Command[index] = ch;
					index++;
					ch = fgetc(DiskFile);if(ch==EOF)goto LABEL_DONE_READING_ASE;
				}
				///////////////////////////
				//// Intrepret Command ////
				///////////////////////////
/*
				if(strcmp(Command,GEOMOBJECT)==0) 
				{
					char text[512];
					sprintf(text,"Part %d\n",PartCounter);
					pMesh = MeshLL_Insert(0,text,pObjectClipBoard->pMeshHead);

					assert(pMesh->pWorldVerts==NULL);
					assert(pMesh->pIndices==NULL);

					pMesh->pWorldVerts = (VERTEX_XYZNRGBUV *)calloc(10000,sizeof(VERTEX_XYZNRGBUV));
					pMesh->pIndices  = (WORD *)calloc(10000,sizeof(WORD));

					pVertex = pMesh->pWorldVerts;
					pIndicies = pMesh->pIndices;

					PartCounter++;

				}
*/
				if(strcmp(Command,NUM_VERTEX)==0) 
				{
					pMesh->num_verts = ReadNextInteger(&ch);
					assert(pMesh->num_verts<MAX_VERTICES);
				}//End if(strcmp(Command,NUM_VERTEX)==0) 
				else if(strcmp(Command,NUM_TVERTEX)==0) 
				{
//					*num_verts = ReadNextInteger(&ch);
				}//End if(strcmp(Command,NUM_VERTEX)==0) 
				else if(strcmp(Command,NUM_FACES)==0) 
				{
					pMesh->num_tris = ReadNextInteger(&ch);
					assert((pMesh->num_tris*3)<MAX_INDICES);
				}//End else if(strcmp(Command,NUM_FACES)==0) 
				else if(strcmp(Command,VERTEX)==0) 
				{
					VertexIndex = ReadNextInteger(&ch);

					assert(VertexIndex>=0);
					assert(VertexIndex<MAX_VERTICES);

					pVertex[VertexIndex].x = ReadNextFloat(&ch);
					pVertex[VertexIndex].y = ReadNextFloat(&ch);
					pVertex[VertexIndex].z = ReadNextFloat(&ch);
					LoadedVerticies++;
				}//End else if(strcmp(Command,VERTEX)==0) 
				else if(strcmp(Command,TVERTEX)==0) 
				{
					VertexIndex = ReadNextInteger(&ch);

					assert(VertexIndex>=0);
					assert(VertexIndex<MAX_VERTICES);

					pVertex[VertexIndex].tu = ReadNextFloat(&ch);
					pVertex[VertexIndex].tv = ReadNextFloat(&ch);
				}//End else if(strcmp(Command,VERTEX)==0) 
				else if(strcmp(Command,FACE)==0) 
				{
					FaceIndex = ReadNextInteger(&ch);
					Unused    = ReadNextInteger(&ch);
					pIndicies[(FaceIndex*3+0)] = ReadNextFloat(&ch);
					Unused    = ReadNextInteger(&ch);
					pIndicies[(FaceIndex*3+2)] = ReadNextFloat(&ch);
					Unused    = ReadNextInteger(&ch);
					pIndicies[(FaceIndex*3+1)] = ReadNextFloat(&ch);
					LoadedFaces++;
				}//End else if(strcmp(Command,VERTEX)==0) 
			}//End if(ch == '*')
		}//End while(1)
		LABEL_DONE_READING_ASE:;
 		fclose(DiskFile);

		//////////////////////////////////
		//// Calculate Vertex Normals ////
		//////////////////////////////////

		for(int i=0;i<LoadedVerticies;i++)
		{
			pVertex[i].nx = 0.0f;
			pVertex[i].ny = 0.0f;
			pVertex[i].nz = 0.0f;
		}
		float Length,Ax,Ay,Az,Bx,By,Bz,Rx,Ry,Rz;
		long V0,V1,V2;
		for(int i=0;i<LoadedFaces;i++)
		{
			V0 = pIndicies[(i*3+0)];
			V1 = pIndicies[(i*3+1)];
			V2 = pIndicies[(i*3+2)];

			Ax = pVertex[V2].x - pVertex[V1].x;
			Ay = pVertex[V2].y - pVertex[V1].y;
			Az = pVertex[V2].z - pVertex[V1].z;

			Bx = pVertex[V0].x - pVertex[V1].x;
			By = pVertex[V0].y - pVertex[V1].y;
			Bz = pVertex[V0].z - pVertex[V1].z;

			MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
			MacroNormalize(Length,Rx,Ry,Rz);
			
			pVertex[V0].nx += Rx;
			pVertex[V0].ny += Ry;
			pVertex[V0].nz += Rz;

			pVertex[V1].nx += Rx;
			pVertex[V1].ny += Ry;
			pVertex[V1].nz += Rz;

			pVertex[V2].nx += Rx;
			pVertex[V2].ny += Ry;
			pVertex[V2].nz += Rz;
		}

		for(int i=0;i<LoadedVerticies;i++)
		{
			MacroNormalize(Length,pVertex[i].nx,pVertex[i].ny,pVertex[i].nz);


		}


		///////////////
		//// Scale ////
		///////////////

		if(Scale != 1.0)
		{
			for(int i=0;i<LoadedVerticies;i++)
			{
				pVertex[i].x *= Scale;
				pVertex[i].y *= Scale;
				pVertex[i].z *= Scale;
			}
		}
/*
		//TRANSLATE
		for(i=0;i<LoadedVerticies;i++)
		{
			pVertex[i].x += 2.0;
			pVertex[i].y += 0.0;
			pVertex[i].z += 0.0;
		}
*/
		//TRANSLATE
		for(int i=0;i<LoadedVerticies;i++)
		{
//			pVertex[i].z += 1.0;
//			pVertex[i].x *= 2.0;
//			pVertex[i].y *= 0.0;
//			pVertex[i].z *= 0.0;
		}

		if(1)
		{
			float TextureScale = 1.0;
			for(int i=0;i<LoadedVerticies;i++)
			{
//diffusediffuse
//diffusediffuse
				//crazy colors	
//				Diffuse = 0xff000000 | (((int)(RandZeroToOne*255))<<16) | (((int)(RandZeroToOne*255))<<8) | (((int)(RandZeroToOne*255)));

				//BW
				long temp = 64 + ((int)(RandZeroToOne*192));
//				Diffuse = 0xff000000 | (temp<<16) | (temp<<8) | temp;
//diffusediffuse
//diffusediffuse
				pVertex[i].diffuse = Diffuse;
				pVertex[i].tu = (pVertex[i].x*TextureScale+0.5);
				pVertex[i].tv = (pVertex[i].y*TextureScale+0.5);
			}
		}
		///////////////////
		//// Translate ////
		///////////////////
/*
		aVECTOR Average;

		Average.x = 0.0;
		Average.y = 0.0;
		Average.z = 0.0;
		for(i=0;i<LoadedVerticies;i++)
		{
			Average.x += pVertex[i].x;
			Average.y += pVertex[i].y;
			Average.z += pVertex[i].z;
		}
		Average.x /= (float)LoadedVerticies;
		Average.y /= (float)LoadedVerticies;
		Average.z /= (float)LoadedVerticies;

		for(i=0;i<LoadedVerticies;i++)
		{
			pVertex[i].x -= Average.x;
			pVertex[i].y -= Average.y;
			pVertex[i].z -= Average.z;
//			pVertex[i].x += Translate.x;
//			pVertex[i].y += Translate.y;
//			pVertex[i].z += Translate.z;
		}
*/
		LoadTexture(pTextureFileName,NULL,pMesh->Texture,TEXTURE_FLAG_4444);
		if((LoadedVerticies == pMesh->num_verts) && (LoadedFaces == pMesh->num_tris))
		{
			return;//true
		}
	}//End if fopen(pFileName,"r+t") != NULL
//	assert(0);
}

/*
typedef struct _VERTEX_LOADTXT
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	long diffuse;//color
	float tu;
	float tv;
	long  bi;//index into normal array
} VERTEX_LOADTXT;
*/

bool Load_txt(char* pFileName,struct ObjectLL* pObject,float fps, bool bSmoothGroups)
{
	KeyFrameLL*  pKeyFrame = NULL;
	KeyFrameLL*  pPrev_KeyFrame = NULL;
	KeyFrameLL*  pNewKeyFrame = NULL;
//	LOADTXT_TXYZ* pRotationKeys = NULL;
//	LOADTXT_TXYZ* pPositionKeys = NULL;
	LOADTXT_MATERIAL* pMaterials;
	LOADTXT_DATA* p_txt_DataFromDisk;
	MeshLL* *pMeshPointers;		//?? how many required
//	BoneH* pBone = NULL;
	//BoneH* pNewBone = NULL;
	//BoneH* pParentBone = NULL;
	char TempBuffer[512];
//	char FileName[512];
//	char FullPath[512];
//	char Path[512];
//	char pName[512];
	aVECTOR Normal;//normals in .txt are stored in smaller list than verts. eventually copied back into pVeritces
	VERTEX_XYZNRGBUV* pV = NULL;
	WORD* pI = NULL;
	PERVERTEX_JOINT_DATA* pPVBD = NULL;
	LOADTXT_TXYZ Position;
	LOADTXT_TXYZ Rotation;
	float* pRestPoseMatrix = NULL;
	MeshLL* pMesh = NULL;
	long Red,Grn,Blu,Diffuse;
	int na,nb,nc,smoothingroup;
	float Length,Ax,Ay,Az,Bx,By,Bz,Rx,Ry,Rz;
	long V0,V1,V2;
	long i,j,k,cVert,cTri;
	int flags;
	int Frames,Frame;
	int num_joints;
	int num_meshes;
	int num_normals;
	int num_materials;
	int num_position_keys;
	int num_rotation_keys;
	int MaterialID;
	bool bTexturesMissing = false;
	FILE* pFile = NULL;
	unsigned long time = 0;
	JointLL* pJoint = NULL;
	float pRestPoseRelativeMatrix[16];
	float pJointRelativeMatrix[16];
	struct KeyFrameLL* pPrev = NULL;
	struct KeyFrameLL* pThis = NULL;
	struct KeyFrameLL* pNext = NULL;
	struct KeyFrameLL* pRestPose_KeyFrame = NULL;
	struct JointLL* pPrevJ = NULL;
	struct JointLL* pThisJ = NULL;
	struct JointLL* pNextJ = NULL;


	pMaterials    		= (LOADTXT_MATERIAL *)calloc(1000,sizeof(LOADTXT_MATERIAL));
	pMeshPointers 		= (MeshLL*          *)calloc(1000,sizeof(MeshLL*));
	p_txt_DataFromDisk 	= (LOADTXT_DATA     *)calloc(1000,sizeof(LOADTXT_DATA));

/*
	memset(FullPath,0,512);
	memset(Path,0,512);
	memset(FileName,0,512);
	GetModuleFileName(NULL,FullPath,512);
	char *c = strrchr(FullPath, 92);//92 = backslash
	int len = (int)(c - FullPath);
	strncpy(Path,FullPath,len);
	sprintf(FileName,"%s\\%s.txt",Path,pFileName);


	sprintf(pName,"%s",pFileName);
	RemoveExtensionFromFileName(pName);


	pFile = fopen(FileName, "r");

*/

	long length;
	length = strlen(pFileName);
	if(length==0 || length>=MAX_PATHNAME)return false;
	memset(gp_TextBuffer1,0,MAX_PATHNAME);
	memcpy(gp_TextBuffer1,pFileName,length);
	RemoveFilenameAndExt(gp_TextBuffer1);


	pFile = fopen(pFileName, "r");

	if(pFile == NULL)
		return false;


	// Check Title
	if(fscanf(pFile, " // MilkShape 3D ASCII ") == EOF)
		goto Label_LoadTXT_ERROR;

	// Read in frames, and frame
	if(fscanf(pFile, " Frames: %d Frame: %d ",&Frames,&Frame) != 2)
		goto Label_LoadTXT_ERROR;

	// Read in num_meshes
	if(fscanf(pFile, " Meshes: %d ", &num_meshes) != 1)
		goto Label_LoadTXT_ERROR;


	//use main menu functions, so set pObject as current.
	//pCurrentlySelectedObject = pObject;

	// Read in all the meshes
	for(i=0;i<num_meshes;i++)
	{

		//Read mesh name, flags, material id
		if(fscanf(pFile, " \"%[^\"]\" %d %d ",TempBuffer,&flags,&MaterialID) != 3)
			goto Label_LoadTXT_ERROR;

		///////////////////////////////////////
		//// Allocate Memory For This Mesh ////
		///////////////////////////////////////

	 	pMesh = MeshLL_Insert(10,TempBuffer,pObject->pMeshHead);
		//pMesh->RenderFlags  = RENDER_VISIBLE|RENDER_ZBUFFERREAD|RENDER_ZBUFFERWRITE|RENDER_BILINEARFILTER|RENDER_LIGHTING|RENDER_TEXTURED|RENDER_TRANSLUCENT_VERTEX;
//		pMesh->RenderFlags  = RENDER_VISIBLE|RENDER_ZBUFFERREAD|RENDER_ZBUFFERWRITE|RENDER_BILINEARFILTER|RENDER_LIGHTING|RENDER_WIREFRAME;
		pMesh->RenderFlags  = RENDER_VISIBLE|RENDER_ZBUFFERREAD|RENDER_ZBUFFERWRITE|RENDER_BILINEARFILTER|RENDER_LIGHTING|RENDER_TEXTURED;

		//save for final addition of materials		
		pMeshPointers[i] = pMesh;

		pMesh->pLocalVerts        	= (VERTEX_XYZNRGBUV     *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
		pMesh->pWorldVerts        	= (VERTEX_XYZNRGBUV     *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
		pMesh->pPerVertexJointData 	= (PERVERTEX_JOINT_DATA *)calloc(MAX_VERTICES,sizeof(PERVERTEX_JOINT_DATA));
		pMesh->pSelectedVerts 		= (unsigned char*		)calloc(MAX_VERTICES,sizeof(unsigned char));
		pMesh->pVertexFlags 		= (unsigned char*		)calloc(MAX_VERTICES,sizeof(unsigned char));
		pMesh->pIndices      		= (WORD                 *)calloc(MAX_INDICES ,sizeof(WORD));
		pMesh->MaterialID         	= MaterialID;//USED ONLY IN THIS LOADER CODE

		pV    = pMesh->pLocalVerts;
		pI    = pMesh->pIndices;
		pPVBD = pMesh->pPerVertexJointData;

		////////////////////////////
		//// Vertices XYZ UV Bi ////
		////////////////////////////

		if(fscanf(pFile, " %d ", &pMesh->num_verts) != 1)
			goto Label_LoadTXT_ERROR;

		for(j=0;j<pMesh->num_verts;j++)
		{
			if(fscanf(pFile, " %d %f %f %f %f %f %d ",
					  &flags,
					  &pV[j].x,
					  &pV[j].y,
					  &pV[j].z,
					  &pV[j].tu,
					  &pV[j].tv,
					  &pPVBD[j].index[0]) != 7)
			{
				goto Label_LoadTXT_ERROR;
			}
		 	//searchfor:CONVERTRIGHTHANDTOLEFTHANDCOORD
		 	pV[j].z  = -pV[j].z;

		 	pV[j].diffuse = 0xffabcdef;
		 	//pV[j].diffuse = RandZeroToOne*0xffffffff;
			//milkshape only has one bone per vertex
			//load as 1 bone at 100%
			//this can be blended later

			pPVBD[j].index[1] = -1;//flag as no contributer
			pPVBD[j].index[2] = -1;
			pPVBD[j].index[3] = -1;
			
			pPVBD[j].weight[0] = 1.0;
			pPVBD[j].weight[1] = 0.0;//flag as no contributer
			pPVBD[j].weight[2] = 0.0;
			pPVBD[j].weight[3] = 0.0;
		}

		////////////////////////
		//// Vertex Normals ////
		////////////////////////

		if(fscanf(pFile," %d ",&num_normals)!=1)
			goto Label_LoadTXT_ERROR;

		for(j=0;j<num_normals;j++)
		{//ignore normals. they are calculated later
			if(fscanf(pFile," %f %f %f ",&Normal.x,&Normal.y,&Normal.z) != 3)
			{
				goto Label_LoadTXT_ERROR;
			}
		}

		////////////////////
		//// Index List ////
		////////////////////


		if(fscanf(pFile, " %d ", &pMesh->num_tris) != 1)
			goto Label_LoadTXT_ERROR;

		for(j=0;j<pMesh->num_tris;j++)
		{
			if(fscanf(pFile, " %d %d %d %d %d %d %d %d ",
					  &flags,
					  &pI[j*3+0],
					  &pI[j*3+1],
					  &pI[j*3+2],
					  &na,
					  &nb,
					  &nc,
					  &smoothingroup) != 8)
			{
				goto Label_LoadTXT_ERROR;
			}

			if(	pI[j*3+0]>=pMesh->num_verts ||
				pI[j*3+1]>=pMesh->num_verts ||
				pI[j*3+2]>=pMesh->num_verts)
			{
				printf("error");
			}

			if(	pI[j*3+0]>=MAX_VERTICES ||
				pI[j*3+1]>=MAX_VERTICES ||
				pI[j*3+2]>=MAX_VERTICES)
			{
				printf("error");
				assert(0 && "too many vertices");
			}





		}

		//change from CW to CCW
//		for(j=0;j<pMesh->num_tris;j++)
//		{
//			V0 = pI[(j*3+0)];
//			V1 = pI[(j*3+1)];
//			V2 = pI[(j*3+2)];
//			pI[(j*3+0)] = V0;
//			pI[(j*3+1)] = V2;
//			pI[(j*3+2)] = V1;
//		}

		if(bSmoothGroups)
		{

			////////////////////////////////////////////
			//// Find ALL tris attached to vertices ////
			////////////////////////////////////////////

			for(j=0;j<pMesh->num_verts;j++)
			{
				pV[j].nx = 0.0f;
				pV[j].ny = 0.0f;
				pV[j].nz = 0.0f;
			}

			float fEpsilon = 0.01;
			long num_normals = 0;			
			for(cVert=0;cVert<pMesh->num_verts;cVert++)
			{
				num_normals = 0;			

				for(cTri=0;cTri<pMesh->num_tris;cTri++)
				{
					V0 = pI[(cTri*3+0)];
					V1 = pI[(cTri*3+1)];
					V2 = pI[(cTri*3+2)];

					//// V0 ////

					if(fabs(pV[V0].x-pV[cVert].x)<fEpsilon)
					{
						if(fabs(pV[V0].y-pV[cVert].y)<fEpsilon)
						{
							if(fabs(pV[V0].z-pV[cVert].z)<fEpsilon)
							{
								Ax = pV[V2].x - pV[V1].x;
								Ay = pV[V2].y - pV[V1].y;
								Az = pV[V2].z - pV[V1].z;

								Bx = pV[V0].x - pV[V1].x;
								By = pV[V0].y - pV[V1].y;
								Bz = pV[V0].z - pV[V1].z;

								MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
								MacroNormalize(Length,Rx,Ry,Rz);


								pV[cVert].nx += Rx;
								pV[cVert].ny += Ry;
								pV[cVert].nz += Rz;
								num_normals++;
							}
						}
					}				

					//// V1 ////

					if(fabs(pV[V1].x-pV[cVert].x)<fEpsilon)
					{
						if(fabs(pV[V1].y-pV[cVert].y)<fEpsilon)
						{
							if(fabs(pV[V1].z-pV[cVert].z)<fEpsilon)
							{
								Ax = pV[V2].x - pV[V1].x;
								Ay = pV[V2].y - pV[V1].y;
								Az = pV[V2].z - pV[V1].z;

								Bx = pV[V0].x - pV[V1].x;
								By = pV[V0].y - pV[V1].y;
								Bz = pV[V0].z - pV[V1].z;

								MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
								MacroNormalize(Length,Rx,Ry,Rz);


								pV[cVert].nx += Rx;
								pV[cVert].ny += Ry;
								pV[cVert].nz += Rz;
								num_normals++;
							}
						}
					}				

					//// V2 ////

					if(fabs(pV[V2].x-pV[cVert].x)<fEpsilon)
					{
						if(fabs(pV[V2].y-pV[cVert].y)<fEpsilon)
						{
							if(fabs(pV[V2].z-pV[cVert].z)<fEpsilon)
							{
								Ax = pV[V2].x - pV[V1].x;
								Ay = pV[V2].y - pV[V1].y;
								Az = pV[V2].z - pV[V1].z;

								Bx = pV[V0].x - pV[V1].x;
								By = pV[V0].y - pV[V1].y;
								Bz = pV[V0].z - pV[V1].z;

								MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
								MacroNormalize(Length,Rx,Ry,Rz);


								pV[cVert].nx += Rx;
								pV[cVert].ny += Ry;
								pV[cVert].nz += Rz;
								num_normals++;
							}
						}
					}				

				}//End for cTri<pMesh->num_tris

				pV[cVert].nx /= (float)num_normals;
				pV[cVert].ny /= (float)num_normals;
				pV[cVert].nz /= (float)num_normals;
			}//End for cVert<pMesh->num_verts
		}//End if(bSmoothGroups)
		else
		{//regular
			//////////////////////////////////
			//// Calculate Vertex Normals ////
			//////////////////////////////////

			for(j=0;j<pMesh->num_verts;j++)
			{
				pV[j].nx = 0.0f;
				pV[j].ny = 0.0f;
				pV[j].nz = 0.0f;
			}
			for(j=0;j<pMesh->num_tris;j++)
			{
				V0 = pI[(j*3+0)];
				V1 = pI[(j*3+1)];
				V2 = pI[(j*3+2)];

				Ax = pV[V2].x - pV[V1].x;
				Ay = pV[V2].y - pV[V1].y;
				Az = pV[V2].z - pV[V1].z;

				Bx = pV[V0].x - pV[V1].x;
				By = pV[V0].y - pV[V1].y;
				Bz = pV[V0].z - pV[V1].z;

				MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
				MacroNormalize(Length,Rx,Ry,Rz);
				
				pV[V0].nx += Rx;
				pV[V0].ny += Ry;
				pV[V0].nz += Rz;

				pV[V1].nx += Rx;
				pV[V1].ny += Ry;
				pV[V1].nz += Rz;

				pV[V2].nx += Rx;
				pV[V2].ny += Ry;
				pV[V2].nz += Rz;
			}
			for(j=0;j<pMesh->num_verts;j++)
			{
				MacroNormalize(Length,pV[j].nx,pV[j].ny,pV[j].nz);
			}
		}
	}

	///////////////////
	//// Materials ////
	///////////////////

	if(fscanf(pFile," Materials: %d ",&num_materials) != 1)
	   	goto Label_LoadTXT_ERROR;

	for(i=0;i<num_materials;i++)
	{
		if(fscanf(pFile, " \"%[^\"]\" %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ",
					pMaterials[i].name,
					&pMaterials[i].ambient.r,	
					&pMaterials[i].ambient.g,	
					&pMaterials[i].ambient.b,	
					&pMaterials[i].ambient.a,
					&pMaterials[i].diffuse.r,	
					&pMaterials[i].diffuse.g,	
					&pMaterials[i].diffuse.b,	
					&pMaterials[i].diffuse.a,
					&pMaterials[i].specular.r,	
					&pMaterials[i].specular.g,	
					&pMaterials[i].specular.b,	
					&pMaterials[i].specular.a,
					&pMaterials[i].emissive.r,	
					&pMaterials[i].emissive.g,	
					&pMaterials[i].emissive.b,	
					&pMaterials[i].emissive.a,
					&pMaterials[i].power,
					&pMaterials[i].alpha) != 19)
		   	goto Label_LoadTXT_ERROR;

		int index = 1;

		/////////////////////////////
		//// Read Texture 1 Name ////
		/////////////////////////////
		// ".\ precedes every  texture name 
		// ex. ".\image.bmp"
		memset(TempBuffer,0,256*sizeof(char));
		fgets(TempBuffer, 256, pFile);
		sscanf(TempBuffer, " \".\\%[^\"]\" ", pMaterials[i].texname1);

		memset(TempBuffer,0,256*sizeof(char));
		fgets(TempBuffer, 256, pFile);
		sscanf(TempBuffer, " \".\\%[^\"]\" ", pMaterials[i].texname2);
	}

	//////////////////////////////
	//// Joints, and or Bones ////
	//////////////////////////////

	if(fscanf(pFile," Bones: %d ",&num_joints) != 1)
	   	goto Label_LoadTXT_ERROR;

	assert(num_joints<1000);
	if(num_joints>0)
	{
//		pRotationKeys = (LOADTXT_TXYZ *)calloc(10000,sizeof(LOADTXT_TXYZ));
//		pPositionKeys = (LOADTXT_TXYZ *)calloc(10000,sizeof(LOADTXT_TXYZ));


//		SetBit(pMesh->RenderFlags,RENDER_TRANSFORMED);

		//Erase Any Existing Skeleton
		SkeletonLL_CloseALL(pObject->pSkeletonHead);
		SkeletonLL*	pSkeleton = NULL;
		pSkeleton = SkeletonLL_New(pObject->pSkeletonHead,"Skeleton","null");
		//MenuFunction_Skeleton_New("Skeleton","null");//!NO MENU FUNCTIONS FROM HERE
//		pCurrentlySelectedSkeleton = pSkeleton;
		/////////////////////////////
		//// LOAD DATA FROM DISK ////
		//// LOAD DATA FROM DISK ////
		//// LOAD DATA FROM DISK ////
		/////////////////////////////
		for(i=0;i<num_joints;i++)
		{
			////////////////////////
			//// Load Bone Name ////
			////////////////////////
	        if(!fgets (TempBuffer,512,pFile))
	        {
			   	goto Label_LoadTXT_ERROR;
	        }
			memset(p_txt_DataFromDisk[i].pName,0,256);
	        sscanf(TempBuffer,"\"%[^\"]\"",p_txt_DataFromDisk[i].pName);


			//////////////////////////
			//// Load Parent Name ////
			//////////////////////////

	        if(!fgets (TempBuffer,512,pFile))
	        {
			   	goto Label_LoadTXT_ERROR;
	        }
			memset(p_txt_DataFromDisk[i].pParentName,0,256);
	        sscanf(TempBuffer,"\"%[^\"]\"",p_txt_DataFromDisk[i].pParentName);
			if(strcmp(p_txt_DataFromDisk[i].pParentName,"")==0) 
			{//this system flags top level joints as "null", not "". convert now.
				sprintf(p_txt_DataFromDisk[i].pParentName,"null");
			}

			//////////////////////////////
			//// Load Joint Rest Pose ////
			//////////////////////////////

	        if(fscanf(pFile, " %d %f %f %f %f %f %f",//flags, posx, posy, posz, rotx, roty, rotz
	                    &flags,
	                    &p_txt_DataFromDisk[i].p_x,
	                    &p_txt_DataFromDisk[i].p_y,
	                    &p_txt_DataFromDisk[i].p_z,
	                    &p_txt_DataFromDisk[i].r_x,
	                    &p_txt_DataFromDisk[i].r_y,
	                    &p_txt_DataFromDisk[i].r_z) != 7)
			{
			   	goto Label_LoadTXT_ERROR;
			}

		 	//searchfor:CONVERTRIGHTHANDTOLEFTHANDCOORD
			//p_txt_DataFromDisk[i].p_z    = -p_txt_DataFromDisk[i].p_z;
		 	//searchfor:CONVERTRIGHTHANDTOLEFTHANDCOORD
			//p_txt_DataFromDisk[i].r_z    = -p_txt_DataFromDisk[i].r_z;

			////////////////////////////
			//// Load Position Keys ////
			////////////////////////////

			if(fscanf(pFile, " %d ", &num_position_keys) != 1)
				goto Label_LoadTXT_ERROR;
			p_txt_DataFromDisk[i].num_position_keys = num_position_keys;
			for(j=0;j<num_position_keys;j++)
			{
				if(fscanf(pFile," %f %f %f %f ",&Position.time,&Position.x,&Position.y,&Position.z) != 4)
				   	goto Label_LoadTXT_ERROR;

				p_txt_DataFromDisk[i].Position[j].time = Position.time;
				p_txt_DataFromDisk[i].Position[j].x    = Position.x;
				p_txt_DataFromDisk[i].Position[j].y    = Position.y;
				p_txt_DataFromDisk[i].Position[j].z    = Position.z;

			 	//searchfor:CONVERTRIGHTHANDTOLEFTHANDCOORD
				//p_txt_DataFromDisk[i].Position[j].z    = -p_txt_DataFromDisk[i].Position[j].z;

			}

			////////////////////////////
			//// Load Rotation Keys ////
			////////////////////////////
			
			if(fscanf(pFile, " %d ", &num_rotation_keys) != 1)
				goto Label_LoadTXT_ERROR;
			p_txt_DataFromDisk[i].num_rotation_keys = num_rotation_keys;
			for(j=0;j<num_rotation_keys;j++)
			{
				if(fscanf(pFile," %f %f %f %f ",&Rotation.time,&Rotation.x,&Rotation.y,&Rotation.z) != 4)
				   	goto Label_LoadTXT_ERROR;

				p_txt_DataFromDisk[i].Rotation[j].time = Rotation.time;
				p_txt_DataFromDisk[i].Rotation[j].x    = Rotation.x;
				p_txt_DataFromDisk[i].Rotation[j].y    = Rotation.y;
				p_txt_DataFromDisk[i].Rotation[j].z    = Rotation.z;

			 	//searchfor:CONVERTRIGHTHANDTOLEFTHANDCOORD
				//p_txt_DataFromDisk[i].Rotation[j].z    = -p_txt_DataFromDisk[i].Rotation[j].z;

			}

			///////////////////////////////////////////////////////////////
			//// Convert Loaded Data Into Internal TN Animatior Format ////
			///////////////////////////////////////////////////////////////
			assert(num_position_keys==num_rotation_keys && "your hosed. email your .txt file (as text not attachment) to support@tanzanite.to");//
			//if num_position_keys==num_rotation_keys then interpolation code is required.
			// if there is an extra positionKF then a rotationKF for that same time must be created.
			
		}//End for(i=0;i<num_joints;i++)


		/////////////////////////
		//// CONVERT DATA 	 ////
		////  CONVERT DATA 	 ////
		////   CONVERT DATA  ////
		////    CONVERT DATA ////
		/////////////////////////
		


		///////////////////////////
		//// Add KeyFrames Now ////
		///////////////////////////


		for(k=0;k<num_rotation_keys;k++)
		{
			//use first joint [0] in .txt file to get times for each keyframe
			assert(p_txt_DataFromDisk[0].Rotation[k].time == p_txt_DataFromDisk[0].Position[k].time);//rot/pos key mismatch. npk must== nrk

			time = (unsigned long)(((float)p_txt_DataFromDisk[0].Rotation[k].time/(float)Frames)*1000.0);
			pKeyFrame = KeyFrameLL_New(pSkeleton->pKeyFrameHead,0,time);
		}



		////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////
		////                  RESTPOSE       								////
		//// Convert MS3D TXT RESTPOSE       								////
		//// RESTPOSE pass strips this data out from txt : 					////
		//// RESTPOSE KeyFrame has to be filled first because 
		//// 	All of its Joints allocate pInfoItem, which all other 
		//// 	Joints point to.
		//// 																////
		//// Bones: 3														////
		//// "joint1"														////
		//// ""																////
		//// 8 5.000000 0.000000 5.000000 0.000000 0.000000 0.000000		////
		//// 																////
		//// "joint2"														////
		//// "joint1"														////
		//// 8 5.000000 0.000000 5.000000 0.000000 0.000000 0.000000		////
		//// 																////
		//// "joint3"														////
		//// "joint2"														////
		//// 8 5.000000 0.000000 5.000000 0.000000 0.000000 0.000000		////
		//// 																////
		////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////


		if(!KeyFrameLL_Get(pSkeleton->pKeyFrameHead,pPrev,pRestPose_KeyFrame,pNext,KEYFRAME_ANIMNUM0,KEYFRAME_RESTPOSE))
		{
			assert(0);//cant get newly created item.
		}

//		pCurrentlySelectedKeyFrame = pKeyFrame;


		for(j=0;j<num_joints;j++)
		{
			/*
			pJoint = MenuFunction_Skeleton_Joint_New(pSkeleton,
											p_txt_DataFromDisk[j].pName,
											p_txt_DataFromDisk[j].pParentName,
											0.0,
											0.0,
											0.0,true);//we are loading relative data, not absolute data
			*/
			pJoint = Skeleton_Joint_New(pSkeleton,p_txt_DataFromDisk[j].pName,p_txt_DataFromDisk[j].pParentName);

			CreateMatrixYawPitchRoll(	pRestPoseRelativeMatrix,
										-p_txt_DataFromDisk[j].r_y,//searchfor:YYYYAWPITCHROLL
										-p_txt_DataFromDisk[j].r_x,
										p_txt_DataFromDisk[j].r_z);

			PositionMatrix(				pRestPoseRelativeMatrix,
										p_txt_DataFromDisk[j].p_x,
										p_txt_DataFromDisk[j].p_y,
										-p_txt_DataFromDisk[j].p_z);




			memcpy(pJoint->pRelativeMatrix,pRestPoseRelativeMatrix,16*sizeof(float));
			pJoint->UniqueID = j;

		}

		////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////
		////                  KEYFRAMES     								////
		//// Convert MS3D TXT KEYFRAMES     								////
		//// KEYFRAMES pass strips this data out from txt : 				////
		////																////
		//// 3																////
		//// 1.000000 0.000000 0.000000 0.000000							////
		//// 15.000000 0.000000 0.000000 0.000000							////
		//// 30.000000 0.000000 0.000000 0.000000							////
		//// 3																////
		//// 1.000000 0.000000 0.000000 0.000000							////
		//// 15.000000 0.000000 0.000000 0.000000							////
		//// 30.000000 0.000000 0.000000 0.000000							////
		////																////
		//// 3																////
		//// 1.000000 0.000000 0.000000 0.000000							////
		//// 15.000000 0.000000 0.000000 0.000000							////
		//// 30.000000 0.000000 0.000000 0.000000							////
		//// 3																////
		//// 1.000000 0.000000 0.000000 0.000000							////
		//// 15.000000 0.000000 0.000000 0.000000							////
		//// 30.000000 0.000000 0.000000 0.000000							////
		////																////
		//// 3																////
		//// 1.000000 0.000000 0.000000 0.000000							////
		//// 15.000000 0.000000 0.000000 0.000000							////
		//// 30.000000 0.000000 0.000000 0.000000							////
		//// 3																////
		//// 1.000000 0.000000 0.000000 0.000000							////
		//// 15.000000 0.000000 0.000000 0.000000							////
		//// 30.000000 0.000000 0.000000 0.000000							////
		////																////
		////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////

		for(k=0;k<num_rotation_keys;k++)
		{
			time = (unsigned long)(((float)p_txt_DataFromDisk[0].Rotation[k].time/(float)Frames)*1000.0);
			//time = (unsigned long)(((float)pRotationKeys[k].time/(float)Frames)*1000.0);
			if(KeyFrameLL_Get(pSkeleton->pKeyFrameHead,pPrev,pKeyFrame,pNext,KEYFRAME_ANIMNUM0,time))
			{
//				pCurrentlySelectedKeyFrame = pKeyFrame;
				for(j=0;j<num_joints;j++)
				{
					//pJoint = MenuFunction_Skeleton_Joint_New(pSkeleton,
					//								p_txt_DataFromDisk[j].pName,
					//								p_txt_DataFromDisk[j].pParentName,
					//								0.0,0.0,0.0);
					if(JointLL_Get(pKeyFrame->pJointHead,pPrevJ,pJoint,pNextJ,p_txt_DataFromDisk[j].pName))//pPrevJ,pNextJ not used
					{
						CreateMatrixYawPitchRoll(	pJointRelativeMatrix,
													-p_txt_DataFromDisk[j].Rotation[k].y,//searchfor:YYYYAWPITCHROLL
													-p_txt_DataFromDisk[j].Rotation[k].x,
													p_txt_DataFromDisk[j].Rotation[k].z);

						PositionMatrix(				pJointRelativeMatrix,
													p_txt_DataFromDisk[j].Position[k].x,
													p_txt_DataFromDisk[j].Position[k].y,
													-p_txt_DataFromDisk[j].Position[k].z);

						if(JointLL_Get(pRestPose_KeyFrame->pJointHead,pPrevJ,pThisJ,pNextJ,p_txt_DataFromDisk[j].pName))//pPrevJ,pNextJ not used
						{
//							MatrixMul(pJoint->pRelativeMatrix,pThisJ->pRelativeMatrix,pJointRelativeMatrix);
//							memcpy(pJoint->pRelativeMatrix,pThisJ->pRelativeMatrix,16*sizeof(float));
							MatrixMul(pJoint->pRelativeMatrix,pJointRelativeMatrix,pThisJ->pRelativeMatrix);
							pJoint->UniqueID = j;
						}
					}
				}
			}
		}

		//////////////////////////////////////////////////////
		//// Finally, Reconnect the bones of the Skeleton ////
		//////////////////////////////////////////////////////
		
		//RestPose
		if(!KeyFrameLL_Get(pSkeleton->pKeyFrameHead,pPrev,pKeyFrame,pNext,KEYFRAME_ANIMNUM0,KEYFRAME_RESTPOSE))
		{
			assert(0);//cant get newly created item.
		}
		//KeyFrame_TransformJoints_RelativeToAbsolute(pKeyFrame,pObject->pMatrix);
		KeyFrame_TransformJoints_RelativeToAbsolute(pKeyFrame,pRender_IdentityMatrix);
		
		//Remaining KeyFrame
		for(k=0;k<num_rotation_keys;k++)
		{//each keyframe represents a skeleton that needs to be reconnected.
			time = (unsigned long)(((float)p_txt_DataFromDisk[0].Rotation[k].time/(float)Frames)*1000.0);
			//time = (unsigned long)(((float)pRotationKeys[k].time/(float)Frames)*1000.0);
			if(!KeyFrameLL_Get(pSkeleton->pKeyFrameHead,pPrev,pKeyFrame,pNext,KEYFRAME_ANIMNUM0,time))//can only load 1 anim/per .txt file. animnum==0
			{
				assert(0);//cant get newly created item.
			}
			//KeyFrame_TransformJoints_RelativeToAbsolute(pKeyFrame,pObject->pMatrix);
			KeyFrame_TransformJoints_RelativeToAbsolute(pKeyFrame,pRender_IdentityMatrix);
		}

//		SAFE_FREE(pRotationKeys);
//		SAFE_FREE(pPositionKeys);
	}//End if num_joints>0

	//////////////////////
	//// Fill Diffuse ////
	//////////////////////

	//	Alp = (int)(alpha*255.0);
	for(i=0;i<num_meshes;i++)
	{
		pMesh = pMeshPointers[i];

		if(num_joints>0)
		{//take this oportunity to set the xform bit if joint data is loaded
			SetBit(pMesh->RenderFlags,RENDER_TRANSFORMED);
		}

		pV    = pMesh->pLocalVerts;
		if(num_materials>0)
		{
			Red = pMaterials[pMesh->MaterialID].diffuse.r*255;	
			Grn = pMaterials[pMesh->MaterialID].diffuse.g*255;	
			Blu = pMaterials[pMesh->MaterialID].diffuse.b*255;	
			Diffuse = 0xff000000 | (Red<<16) | (Grn<<8) | Blu;
//			Diffuse = 0x66000000 | (Red<<16) | (Grn<<8) | Blu;
//			Diffuse = (Alp<<24) | (Red<<16) | (Grn<<8) | Blu;
//			int alpha = RandZeroToOne * 255.0;
			for(j=0;j<pMesh->num_verts;j++)
			{
//				alpha = (RandZeroToOne * 255.0);
//				Diffuse = (alpha<<24) | (Red<<16) | (Grn<<8) | Blu;
				pV[j].diffuse = Diffuse;
			}
			sprintf(gp_TextBuffer2,"%s\\%s",gp_TextBuffer1,pMaterials[pMesh->MaterialID].texname1);
			LoadTexture(NULL,gp_TextBuffer2,pMesh->Texture,TEXTURE_FLAG_4444);
		}
	}



	SAFE_FREE(pMaterials);
	SAFE_FREE(pMeshPointers);
	SAFE_FREE(p_txt_DataFromDisk);


	//Clean Exit
	fclose(pFile);
	return true;

	//Error Exit
	Label_LoadTXT_ERROR:;
	fclose(pFile);

	return false;
}

bool Load_txt(char* pFileName,struct MeshLL* pMesh)
{
	LOADTXT_MATERIAL Materials[100];//?? how many required//100 meshes MAX
	MeshLL* MeshPointers[100];		//?? how many required
//	BoneH* pNewBone = NULL;
	char TempBuffer[512];
//	char FileName[512];
//	char FullPath[512];
//	char Path[512];
	char BoneName[512];
	char ParentName[512];
	aVECTOR BonePosition;
	aVECTOR BoneRotation;
	aVECTOR Normal;//normals in .txt are stored in smaller list than verts. eventually copied back into pVeritces
	VERTEX_XYZNRGBUV* pV = NULL;
	WORD* pI = NULL;
	PERVERTEX_JOINT_DATA* pPVBD = NULL;
	LOADTXT_TXYZ Position;
	LOADTXT_TXYZ Rotation;
	ObjectLL* pObject = NULL;
//	MeshLL* pMesh = NULL;
	long Red,Grn,Blu,Diffuse;
	int na,nb,nc,smoothingroup;
	float Length,Ax,Ay,Az,Bx,By,Bz,Rx,Ry,Rz;
	long V0,V1,V2;
	long i,j;
	int flags;
	int Frames,Frame;
	int num_bones;
	int num_meshes;
	int num_normals;
	int num_materials;
	int num_position_keys;
	int num_rotation_keys;
	int MaterialID;
	bool bTexturesMissing = false;
	FILE* pFile = NULL;

/*
	memset(FullPath,0,512);
	memset(Path,0,512);
	memset(FileName,0,512);
	GetModuleFileName(NULL,FullPath,512);
	char *c = strrchr(FullPath, 92);//92 = backslash
	int len = (int)(c - FullPath);
	strncpy(Path,FullPath,len);
	sprintf(FileName,"%s\\%s.txt",Path,pFileName);

	pFile = fopen(FileName, "r");
*/

	long length;
	length = strlen(pFileName);
	if(length==0 || length>=MAX_PATHNAME)return false;
	memset(gp_TextBuffer1,0,MAX_PATHNAME);
	memcpy(gp_TextBuffer1,pFileName,length);
	RemoveFilenameAndExt(gp_TextBuffer1);


	pFile = fopen(pFileName, "r");
	if(pFile == NULL)
		return false;



	memset(Materials,0,100*sizeof(LOADTXT_MATERIAL));
	memset(MeshPointers,0,100*sizeof(MeshLL*));


	// Check Title
	if(fscanf(pFile, " // MilkShape 3D ASCII ") == EOF)
		goto Label_LoadTXT_ERROR;

	// Read in frames, and frame
	if(fscanf(pFile, " Frames: %d Frame: %d ",&Frames,&Frame) != 2)
		goto Label_LoadTXT_ERROR;

	// Read in num_meshes
	if(fscanf(pFile, " Meshes: %d ", &num_meshes) != 1)
		goto Label_LoadTXT_ERROR;

//	pObject = ObjectLL_Insert(0,pFileName,pScene->pObjectHead);

	// Read in all the meshes
	for(i=0;i<num_meshes;i++)
	{

		//Read mesh name, flags, material id
		if(fscanf(pFile, " \"%[^\"]\" %d %d ",TempBuffer,&flags,&MaterialID) != 3)
			goto Label_LoadTXT_ERROR;

		///////////////////////////////////////
		//// Allocate Memory For This Mesh ////
		///////////////////////////////////////

		pMesh->RenderFlags  = RENDER_VISIBLE|RENDER_ZBUFFERREAD|RENDER_ZBUFFERWRITE|RENDER_BILINEARFILTER|RENDER_TEXTURED;
		MeshPointers[i] = pMesh;
		pMesh->pLocalVerts        = (VERTEX_XYZNRGBUV    *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
		pMesh->pWorldVerts        = (VERTEX_XYZNRGBUV    *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
		pMesh->pPerVertexJointData = (PERVERTEX_JOINT_DATA *)calloc(MAX_VERTICES,sizeof(PERVERTEX_JOINT_DATA));
		pMesh->pSelectedVerts 		= (unsigned char*		)calloc(MAX_VERTICES,sizeof(unsigned char));
		pMesh->pVertexFlags 		= (unsigned char*		)calloc(MAX_VERTICES,sizeof(unsigned char));
		pMesh->pIndices      = (WORD                *)calloc(MAX_INDICES ,sizeof(WORD));
		pMesh->MaterialID         = MaterialID;//USED ONLY IN THIS LOADER CODE

		pV    = pMesh->pLocalVerts;
		pI    = pMesh->pIndices;
		pPVBD = pMesh->pPerVertexJointData;

		////////////////////////////
		//// Vertices XYZ UV Bi ////
		////////////////////////////

		if(fscanf(pFile, " %d ", &pMesh->num_verts) != 1)
			goto Label_LoadTXT_ERROR;

		for(j=0;j<pMesh->num_verts;j++)
		{
/*facetize
			if(fscanf(pFile, " %d %f %f %f %f %f %d ",
					  &flags,
					  &TempVerts[j].x,
					  &TempVerts[j].y,
					  &TempVerts[j].z,
					  &TempVerts[j].tu,
					  &TempVerts[j].tv,
					  &Red) != 7)
*/

			if(fscanf(pFile, " %d %f %f %f %f %f %d ",
					  &flags,
					  &pV[j].x,
					  &pV[j].y,
					  &pV[j].z,
					  &pV[j].tu,
					  &pV[j].tv,
					  &Red) != 7)

					  //&pPVBD[j].index[0]) != 7)
			{
				goto Label_LoadTXT_ERROR;
			}
			//milkshape only has one bone per vertex
			//load as 1 bone at 100%
			//this can be blended later
//			pPVBD[j].weight[0] = 1.0;
		}

		////////////////////////
		//// Vertex Normals ////
		////////////////////////

		if(fscanf(pFile," %d ",&num_normals)!=1)
			goto Label_LoadTXT_ERROR;

		for(j=0;j<num_normals;j++)
		{//ignore normals. they are calculated later
			if(fscanf(pFile," %f %f %f ",&Normal.x,&Normal.y,&Normal.z) != 3)
			{
				goto Label_LoadTXT_ERROR;
			}
		}






		//DEBUG INIT
		//DEBUG INIT
		//DEBUG INIT
		PERVERTEX_JOINT_DATA* pPVBD = NULL;
		pPVBD = pMesh->pPerVertexJointData;
		int ii;
		for(ii=0;ii<MAX_VERTICES;ii++)
		{
			pPVBD[ii].index[0] = 1;
			pPVBD[ii].index[1] = 2;
			pPVBD[ii].index[2] = 3;
			pPVBD[ii].index[3] = 4;

			pPVBD[ii].weight[0] = RandZeroToOne;
			pPVBD[ii].weight[1] = RandZeroToOne;
			pPVBD[ii].weight[2] = RandZeroToOne;
			pPVBD[ii].weight[3] = RandZeroToOne;

			MacroNormalizeWeights(Length,
							pPVBD[ii].weight[0],
							pPVBD[ii].weight[1],
							pPVBD[ii].weight[2],
							pPVBD[ii].weight[3]);
		}	
		//DEBUG INIT
		//DEBUG INIT
		//DEBUG INIT




		////////////////////
		//// Index List ////
		////////////////////


		if(fscanf(pFile, " %d ", &pMesh->num_tris) != 1)
			goto Label_LoadTXT_ERROR;

		for(j=0;j<pMesh->num_tris;j++)
		{
			if(fscanf(pFile, " %d %d %d %d %d %d %d %d ",
					  &flags,
					  &pI[j*3+0],
					  &pI[j*3+1],
					  &pI[j*3+2],
					  &na,
					  &nb,
					  &nc,
					  &smoothingroup) != 8)
			{
				goto Label_LoadTXT_ERROR;
			}
		}
		//change from CW to CCW
		for(j=0;j<pMesh->num_tris;j++)
		{
			V0 = pI[(j*3+0)];
			V1 = pI[(j*3+1)];
			V2 = pI[(j*3+2)];
			pI[(j*3+0)] = V0;
			pI[(j*3+1)] = V2;
			pI[(j*3+2)] = V1;
		}
		//////////////////////////////////
		//// Calculate Vertex Normals ////
		//////////////////////////////////
/*facectize
		long VertexIndex = 0;
		for(j=0;j<pMesh->num_tris;j++)
		{
			V0 = pI[(j*3+0)];
			V1 = pI[(j*3+1)];
			V2 = pI[(j*3+2)];

			Ax = TempVerts[V2].x - TempVerts[V1].x;
			Ay = TempVerts[V2].y - TempVerts[V1].y;
			Az = TempVerts[V2].z - TempVerts[V1].z;

			Bx = TempVerts[V0].x - TempVerts[V1].x;
			By = TempVerts[V0].y - TempVerts[V1].y;
			Bz = TempVerts[V0].z - TempVerts[V1].z;
			MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
			MacroNormalize(Length,Rx,Ry,Rz);

			pI[(j*3+0)] = VertexIndex+0;
			pI[(j*3+1)] = VertexIndex+1;
			pI[(j*3+2)] = VertexIndex+2;
			
			pV[VertexIndex].x  = TempVerts[V0].x;
			pV[VertexIndex].y  = TempVerts[V0].y;
			pV[VertexIndex].z  = TempVerts[V0].z;
			pV[VertexIndex].nx = Rx;
			pV[VertexIndex].ny = Ry;
			pV[VertexIndex].nz = Rz;
			pV[VertexIndex].tu = TempVerts[V0].tu;
			pV[VertexIndex].tv = TempVerts[V0].tv;
			VertexIndex++;

			pV[VertexIndex].x  = TempVerts[V1].x;
			pV[VertexIndex].y  = TempVerts[V1].y;
			pV[VertexIndex].z  = TempVerts[V1].z;
			pV[VertexIndex].nx = Rx;
			pV[VertexIndex].ny = Ry;
			pV[VertexIndex].nz = Rz;
			pV[VertexIndex].tu = TempVerts[V1].tu;
			pV[VertexIndex].tv = TempVerts[V1].tv;
			VertexIndex++;

			pV[VertexIndex].x  = TempVerts[V2].x;
			pV[VertexIndex].y  = TempVerts[V2].y;
			pV[VertexIndex].z  = TempVerts[V2].z;
			pV[VertexIndex].nx = Rx;		
			pV[VertexIndex].ny = Ry;
			pV[VertexIndex].nz = Rz;
			pV[VertexIndex].tu = TempVerts[V2].tu;
			pV[VertexIndex].tv = TempVerts[V2].tv;
			VertexIndex++;
		}
		pMesh->num_verts = VertexIndex;
*/


		for(j=0;j<pMesh->num_verts;j++)
		{
			pV[j].nx = 0.0f;
			pV[j].ny = 0.0f;
			pV[j].nz = 0.0f;
		}
		for(j=0;j<pMesh->num_tris;j++)
		{
			V0 = pI[(j*3+0)];
			V1 = pI[(j*3+1)];
			V2 = pI[(j*3+2)];

			Ax = pV[V2].x - pV[V1].x;
			Ay = pV[V2].y - pV[V1].y;
			Az = pV[V2].z - pV[V1].z;

			Bx = pV[V0].x - pV[V1].x;
			By = pV[V0].y - pV[V1].y;
			Bz = pV[V0].z - pV[V1].z;

			MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
			MacroNormalize(Length,Rx,Ry,Rz);
			
			pV[V0].nx += Rx;
			pV[V0].ny += Ry;
			pV[V0].nz += Rz;

			pV[V1].nx += Rx;
			pV[V1].ny += Ry;
			pV[V1].nz += Rz;

			pV[V2].nx += Rx;
			pV[V2].ny += Ry;
			pV[V2].nz += Rz;
		}
		for(j=0;j<pMesh->num_verts;j++)
		{
			MacroNormalize(Length,pV[j].nx,pV[j].ny,pV[j].nz);
		}

	}

	///////////////////
	//// Materials ////
	///////////////////

	if(fscanf(pFile," Materials: %d ",&num_materials) != 1)
	   	goto Label_LoadTXT_ERROR;

//	for(i=0;i<num_materials;i++)
	for(i=0;i<1;i++)
	{
		if(fscanf(pFile, " \"%[^\"]\" %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ",
					Materials[i].name,
					&Materials[i].ambient.r,	
					&Materials[i].ambient.g,	
					&Materials[i].ambient.b,	
					&Materials[i].ambient.a,
					&Materials[i].diffuse.r,	
					&Materials[i].diffuse.g,	
					&Materials[i].diffuse.b,	
					&Materials[i].diffuse.a,
					&Materials[i].specular.r,	
					&Materials[i].specular.g,	
					&Materials[i].specular.b,	
					&Materials[i].specular.a,
					&Materials[i].emissive.r,	
					&Materials[i].emissive.g,	
					&Materials[i].emissive.b,	
					&Materials[i].emissive.a,
					&Materials[i].power,
					&Materials[i].alpha) != 19)
		   	goto Label_LoadTXT_ERROR;

		fgets(TempBuffer, 256, pFile);
		if(TempBuffer == NULL)
		   	goto Label_LoadTXT_ERROR;
		if(TempBuffer[0] == '\"' && TempBuffer[1] == '\"')
			Materials[i].texname1[0] = '\0';
		else if(sscanf(TempBuffer, " \"%[^\"]\" ", Materials[i].texname1) != 1)
		   	goto Label_LoadTXT_ERROR;

		// First line... read in the source filename for Texture 2.
		fgets(TempBuffer, 256, pFile);
		if(TempBuffer == NULL)
		   	goto Label_LoadTXT_ERROR;
		if(TempBuffer[0] == '\"' && TempBuffer[1] == '\"')
			Materials[i].texname2[0] = '\0';
		else if(sscanf(TempBuffer, " \"%[^\"]\" ", Materials[i].texname2) != 1)
		   	goto Label_LoadTXT_ERROR;

		//LOAD TEXTURES??!?Q??
	}

	//////////////////////////////
	//// Joints, and or Bones ////
	//////////////////////////////

	if(fscanf(pFile," Bones: %d ",&num_bones) != 1)
	   	goto Label_LoadTXT_ERROR;

	for(i=0;i<num_bones;i++)
	{
		memset(BoneName,0,512);
		memset(ParentName,0,512);



        if(!fgets (TempBuffer,512,pFile))
        {
		   	goto Label_LoadTXT_ERROR;
        }
        sscanf(TempBuffer,"\"%[^\"]\"",BoneName);


        if(!fgets (TempBuffer,512,pFile))
        {
		   	goto Label_LoadTXT_ERROR;
        }
        sscanf(TempBuffer,"\"%[^\"]\"",ParentName);

        if(fscanf(pFile, " %d %f %f %f %f %f %f",
                    &flags,
                    &BonePosition.x,&BonePosition.y,&BonePosition.z,
                    &BoneRotation.x,&BoneRotation.y,&BoneRotation.z) != 7)
		   	goto Label_LoadTXT_ERROR;

/*
		pNewBone = BoneH_Insert_ChildToParent(BoneName,ParentName,pObject->Skeleton);


		CreateMatrixYawPitchRoll(	pNewBone->pBoneMatrix,
									BoneRotation.x,
									BoneRotation.y,
									BoneRotation.z);

		PositionMatrix(pNewBone->pBoneMatrix,
					   BonePosition.x,
					   BonePosition.y,
					   BonePosition.z);

		if(i>0)
		{
		Identity(pNewBone->pBoneMatrix);
			PositionMatrix(pNewBone->pBoneMatrix,
						   BonePosition.x*BoneVector.x,
						   BonePosition.y*BoneVector.y,
						   BonePosition.z*BoneVector.z);
		}
		//this gets updated first time through
		BoneVector.x = pNewBone->pBoneMatrix[4];
		BoneVector.y = pNewBone->pBoneMatrix[5];
		BoneVector.z = pNewBone->pBoneMatrix[6];
*/
		/////////////////////
		//// Translation ////
		/////////////////////

		if(fscanf(pFile, " %d ", &num_position_keys) != 1)
			goto Label_LoadTXT_ERROR;
		for(i=0;i<num_position_keys;i++)
		{
			if(fscanf(pFile," %f %f %f %f ",&Position.time,&Position.x,&Position.y,&Position.z) != 4)
			   	goto Label_LoadTXT_ERROR;
		}

		//////////////////
		//// Rotation ////
		//////////////////
		
		if(fscanf(pFile, " %d ", &num_rotation_keys) != 1)
			goto Label_LoadTXT_ERROR;
		for(i=0;i<num_rotation_keys;i++)
		{
			if(fscanf(pFile," %f %f %f %f ",&Rotation.time,&Rotation.x,&Rotation.y,&Rotation.z) != 4)
			   	goto Label_LoadTXT_ERROR;
		}
	}

	//////////////////////
	//// Fill Diffuse ////
	//////////////////////

//	Alp = (int)(alpha*255.0);
//	for(i=0;i<num_meshes;i++)
	for(i=0;i<1;i++)
	{
		pMesh = MeshPointers[i];
		pV    = pMesh->pLocalVerts;

		Red = Materials[pMesh->MaterialID].diffuse.r*255;	
		Grn = Materials[pMesh->MaterialID].diffuse.g*255;	
		Blu = Materials[pMesh->MaterialID].diffuse.b*255;	
		Diffuse = 0xff000000 | (Red<<16) | (Grn<<8) | Blu;
//		Diffuse = 0x66000000 | (Red<<16) | (Grn<<8) | Blu;
//		Diffuse = (Alp<<24) | (Red<<16) | (Grn<<8) | Blu;

//		int alpha = RandZeroToOne * 255.0;


		for(j=0;j<pMesh->num_verts;j++)
		{
//			alpha = (RandZeroToOne * 255.0);
//			Diffuse = (alpha<<24) | (Red<<16) | (Grn<<8) | Blu;
			pV[j].diffuse = Diffuse;
		}

		sprintf(gp_TextBuffer2,"%s\\%s",gp_TextBuffer1,Materials[pMesh->MaterialID].texname1);
		LoadTexture(NULL,gp_TextBuffer2,pMesh->Texture,TEXTURE_FLAG_4444);

	}


	//Clean Exit
	fclose(pFile);
	return true;

	//Error Exit
	Label_LoadTXT_ERROR:;
	fclose(pFile);
	return false;
}






MeshLL* MeshMenu_LoadTXTintoMesh(char* pFileName)
{
	LOADTXT_MATERIAL Materials[100];//?? how many required//100 meshes MAX
	MeshLL* MeshPointers[100];		//?? how many required
///	BoneH* pNewBone = NULL;
	char TempBuffer[512];
	char FileName[512];
	char FullPath[512];
	char Path[512];
	char BoneName[512];
	char ParentName[512];
	aVECTOR BonePosition;
	aVECTOR BoneRotation;
	aVECTOR Normal;//normals in .txt are stored in smaller list than verts. eventually copied back into pVeritces
	VERTEX_XYZNRGBUV* pV = NULL;
	WORD* pI = NULL;
	PERVERTEX_JOINT_DATA* pPVBD = NULL;
	LOADTXT_TXYZ Position;
	LOADTXT_TXYZ Rotation;
	ObjectLL* pObject = NULL;
	MeshLL* pMesh = NULL;
	long Red,Grn,Blu,Diffuse;
	int na,nb,nc,smoothingroup;
	float Length,Ax,Ay,Az,Bx,By,Bz,Rx,Ry,Rz;
	long V0,V1,V2;
	long i,j;
	int flags;
	int Frames,Frame;
	int num_bones;
	int num_meshes;
	int num_normals;
	int num_materials;
	int num_position_keys;
	int num_rotation_keys;
	int MaterialID;
	bool bTexturesMissing = false;
	FILE* pFile = NULL;


	memset(FullPath,0,512);
	memset(Path,0,512);
	memset(FileName,0,512);
	GetModuleFileName(NULL,FullPath,512);
	char *c = strrchr(FullPath, 92);//92 = backslash
	int len = (int)(c - FullPath);
	strncpy(Path,FullPath,len);
	sprintf(FileName,"%s\\%s",Path,pFileName);

	pFile = fopen(FileName, "r");
	if(pFile == NULL)
		return false;



	memset(Materials,0,100*sizeof(LOADTXT_MATERIAL));
	memset(MeshPointers,0,100*sizeof(MeshLL*));


	// Check Title
	if(fscanf(pFile, " // MilkShape 3D ASCII ") == EOF)
		goto Label_LoadTXT_ERROR;

	// Read in frames, and frame
	if(fscanf(pFile, " Frames: %d Frame: %d ",&Frames,&Frame) != 2)
		goto Label_LoadTXT_ERROR;

	// Read in num_meshes
	if(fscanf(pFile, " Meshes: %d ", &num_meshes) != 1)
		goto Label_LoadTXT_ERROR;

//	pObject = ObjectLL_Insert(0,pFileName,pScene->pObjectHead);

	// Read in all the meshes
	for(i=0;i<num_meshes;i++)
//	for(i=0;i<1;i++)
	{

		//Read mesh name, flags, material id
		if(fscanf(pFile, " \"%[^\"]\" %d %d ",TempBuffer,&flags,&MaterialID) != 3)
			goto Label_LoadTXT_ERROR;

		///////////////////////////////////////
		//// Allocate Memory For This Mesh ////
		///////////////////////////////////////

//	 	pMesh = MeshLL_Insert(10,TempBuffer,pObject->pMeshHead);
	 	pMesh = MeshLL_CreateNode(	pFileName,
	 								MAX_VERTICES,
	 								MAX_INDICES,
	 								RENDER_VISIBLE); 	
		
//		pMesh->RenderFlags  = RenderFlags;
		pMesh->RenderFlags  = RENDER_VISIBLE|RENDER_ZBUFFERREAD|RENDER_ZBUFFERWRITE|RENDER_BILINEARFILTER|RENDER_TEXTURED;
//		pMesh->RenderFlags  = RENDER_VISIBLE|RENDER_ZBUFFERREAD|RENDER_ZBUFFERWRITE|RENDER_BILINEARFILTER|RENDER_TEXTURED|RENDER_LIGHTING|RENDER_TRANSLUCENT_VERTEX;

		//save for final addition of materials		
		MeshPointers[i] = pMesh;


//		pMesh->pWorldVerts        = (VERTEX_XYZNRGBUV    *)calloc(MAX_VERTICES,sizeof(VERTEX_XYZNRGBUV));
//		pMesh->pPerVertexJointData = (PERVERTEX_JOINT_DATA *)calloc(MAX_VERTICES,sizeof(PERVERTEX_JOINT_DATA));
//		pMesh->pIndices      = (WORD                *)calloc(MAX_INDICES ,sizeof(WORD));
		pMesh->MaterialID         = MaterialID;//USED ONLY IN THIS LOADER CODE

		pV    = pMesh->pLocalVerts;
		pI    = pMesh->pIndices;
//		pPVBD = pMesh->pPerVertexJointData;

		////////////////////////////
		//// Vertices XYZ UV Bi ////
		////////////////////////////

		if(fscanf(pFile, " %d ", &pMesh->num_verts) != 1)
			goto Label_LoadTXT_ERROR;

		for(j=0;j<pMesh->num_verts;j++)
		{
/*facetize
			if(fscanf(pFile, " %d %f %f %f %f %f %d ",
					  &flags,
					  &TempVerts[j].x,
					  &TempVerts[j].y,
					  &TempVerts[j].z,
					  &TempVerts[j].tu,
					  &TempVerts[j].tv,
					  &Red) != 7)
*/

			if(fscanf(pFile, " %d %f %f %f %f %f %d ",
					  &flags,
					  &pV[j].x,
					  &pV[j].y,
					  &pV[j].z,
					  &pV[j].tu,
					  &pV[j].tv,
					  &Red) != 7)

					  //&pPVBD[j].index[0]) != 7)
			{
				goto Label_LoadTXT_ERROR;
			}
			//milkshape only has one bone per vertex
			//load as 1 bone at 100%
			//this can be blended later
//			pPVBD[j].weight[0] = 1.0;
		}

		////////////////////////
		//// Vertex Normals ////
		////////////////////////

		if(fscanf(pFile," %d ",&num_normals)!=1)
			goto Label_LoadTXT_ERROR;

		for(j=0;j<num_normals;j++)
		{//ignore normals. they are calculated later
			if(fscanf(pFile," %f %f %f ",&Normal.x,&Normal.y,&Normal.z) != 3)
			{
				goto Label_LoadTXT_ERROR;
			}
		}

		////////////////////
		//// Index List ////
		////////////////////


		if(fscanf(pFile, " %d ", &pMesh->num_tris) != 1)
			goto Label_LoadTXT_ERROR;

		for(j=0;j<pMesh->num_tris;j++)
		{
			if(fscanf(pFile, " %d %d %d %d %d %d %d %d ",
					  &flags,
					  &pI[j*3+0],
					  &pI[j*3+1],
					  &pI[j*3+2],
					  &na,
					  &nb,
					  &nc,
					  &smoothingroup) != 8)
			{
				goto Label_LoadTXT_ERROR;
			}
		}
		//change from CW to CCW
		for(j=0;j<pMesh->num_tris;j++)
		{
			V0 = pI[(j*3+0)];
			V1 = pI[(j*3+1)];
			V2 = pI[(j*3+2)];
			pI[(j*3+0)] = V0;
			pI[(j*3+1)] = V2;
			pI[(j*3+2)] = V1;
		}
		//////////////////////////////////
		//// Calculate Vertex Normals ////
		//////////////////////////////////
/*facectize
		long VertexIndex = 0;
		for(j=0;j<pMesh->num_tris;j++)
		{
			V0 = pI[(j*3+0)];
			V1 = pI[(j*3+1)];
			V2 = pI[(j*3+2)];

			Ax = TempVerts[V2].x - TempVerts[V1].x;
			Ay = TempVerts[V2].y - TempVerts[V1].y;
			Az = TempVerts[V2].z - TempVerts[V1].z;

			Bx = TempVerts[V0].x - TempVerts[V1].x;
			By = TempVerts[V0].y - TempVerts[V1].y;
			Bz = TempVerts[V0].z - TempVerts[V1].z;
			MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
			MacroNormalize(Length,Rx,Ry,Rz);

			pI[(j*3+0)] = VertexIndex+0;
			pI[(j*3+1)] = VertexIndex+1;
			pI[(j*3+2)] = VertexIndex+2;
			
			pV[VertexIndex].x  = TempVerts[V0].x;
			pV[VertexIndex].y  = TempVerts[V0].y;
			pV[VertexIndex].z  = TempVerts[V0].z;
			pV[VertexIndex].nx = Rx;
			pV[VertexIndex].ny = Ry;
			pV[VertexIndex].nz = Rz;
			pV[VertexIndex].tu = TempVerts[V0].tu;
			pV[VertexIndex].tv = TempVerts[V0].tv;
			VertexIndex++;

			pV[VertexIndex].x  = TempVerts[V1].x;
			pV[VertexIndex].y  = TempVerts[V1].y;
			pV[VertexIndex].z  = TempVerts[V1].z;
			pV[VertexIndex].nx = Rx;
			pV[VertexIndex].ny = Ry;
			pV[VertexIndex].nz = Rz;
			pV[VertexIndex].tu = TempVerts[V1].tu;
			pV[VertexIndex].tv = TempVerts[V1].tv;
			VertexIndex++;

			pV[VertexIndex].x  = TempVerts[V2].x;
			pV[VertexIndex].y  = TempVerts[V2].y;
			pV[VertexIndex].z  = TempVerts[V2].z;
			pV[VertexIndex].nx = Rx;		
			pV[VertexIndex].ny = Ry;
			pV[VertexIndex].nz = Rz;
			pV[VertexIndex].tu = TempVerts[V2].tu;
			pV[VertexIndex].tv = TempVerts[V2].tv;
			VertexIndex++;
		}
		pMesh->num_verts = VertexIndex;
*/


		for(j=0;j<pMesh->num_verts;j++)
		{
			pV[j].nx = 0.0f;
			pV[j].ny = 0.0f;
			pV[j].nz = 0.0f;
		}
		for(j=0;j<pMesh->num_tris;j++)
		{
			V0 = pI[(j*3+0)];
			V1 = pI[(j*3+1)];
			V2 = pI[(j*3+2)];

			Ax = pV[V2].x - pV[V1].x;
			Ay = pV[V2].y - pV[V1].y;
			Az = pV[V2].z - pV[V1].z;

			Bx = pV[V0].x - pV[V1].x;
			By = pV[V0].y - pV[V1].y;
			Bz = pV[V0].z - pV[V1].z;

			MacroCross(Rx,Ry,Rz,Ax,Ay,Az,Bx,By,Bz);
			MacroNormalize(Length,Rx,Ry,Rz);
			
			pV[V0].nx += Rx;
			pV[V0].ny += Ry;
			pV[V0].nz += Rz;

			pV[V1].nx += Rx;
			pV[V1].ny += Ry;
			pV[V1].nz += Rz;

			pV[V2].nx += Rx;
			pV[V2].ny += Ry;
			pV[V2].nz += Rz;
		}
		for(j=0;j<pMesh->num_verts;j++)
		{
			MacroNormalize(Length,pV[j].nx,pV[j].ny,pV[j].nz);
		}

	}

	///////////////////
	//// Materials ////
	///////////////////

	if(fscanf(pFile," Materials: %d ",&num_materials) != 1)
	   	goto Label_LoadTXT_ERROR;

//	for(i=0;i<num_materials;i++)
	for(i=0;i<1;i++)
	{
		if(fscanf(pFile, " \"%[^\"]\" %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ",
					Materials[i].name,
					&Materials[i].ambient.r,	
					&Materials[i].ambient.g,	
					&Materials[i].ambient.b,	
					&Materials[i].ambient.a,
					&Materials[i].diffuse.r,	
					&Materials[i].diffuse.g,	
					&Materials[i].diffuse.b,	
					&Materials[i].diffuse.a,
					&Materials[i].specular.r,	
					&Materials[i].specular.g,	
					&Materials[i].specular.b,	
					&Materials[i].specular.a,
					&Materials[i].emissive.r,	
					&Materials[i].emissive.g,	
					&Materials[i].emissive.b,	
					&Materials[i].emissive.a,
					&Materials[i].power,
					&Materials[i].alpha) != 19)
		   	goto Label_LoadTXT_ERROR;

		fgets(TempBuffer, 256, pFile);
		if(TempBuffer == NULL)
		   	goto Label_LoadTXT_ERROR;
		if(TempBuffer[0] == '\"' && TempBuffer[1] == '\"')
			Materials[i].texname1[0] = '\0';
		else if(sscanf(TempBuffer, " \"%[^\"]\" ", Materials[i].texname1) != 1)
		   	goto Label_LoadTXT_ERROR;

		// First line... read in the source filename for Texture 2.
		fgets(TempBuffer, 256, pFile);
		if(TempBuffer == NULL)
		   	goto Label_LoadTXT_ERROR;
		if(TempBuffer[0] == '\"' && TempBuffer[1] == '\"')
			Materials[i].texname2[0] = '\0';
		else if(sscanf(TempBuffer, " \"%[^\"]\" ", Materials[i].texname2) != 1)
		   	goto Label_LoadTXT_ERROR;

		//LOAD TEXTURES??!?Q??
	}

	//////////////////////////////
	//// Joints, and or Bones ////
	//////////////////////////////

	if(fscanf(pFile," Bones: %d ",&num_bones) != 1)
	   	goto Label_LoadTXT_ERROR;

	for(i=0;i<num_bones;i++)
	{
		memset(BoneName,0,512);
		memset(ParentName,0,512);



        if(!fgets (TempBuffer,512,pFile))
        {
		   	goto Label_LoadTXT_ERROR;
        }
        sscanf(TempBuffer,"\"%[^\"]\"",BoneName);


        if(!fgets (TempBuffer,512,pFile))
        {
		   	goto Label_LoadTXT_ERROR;
        }
        sscanf(TempBuffer,"\"%[^\"]\"",ParentName);

        if(fscanf(pFile, " %d %f %f %f %f %f %f",
                    &flags,
                    &BonePosition.x,&BonePosition.y,&BonePosition.z,
                    &BoneRotation.x,&BoneRotation.y,&BoneRotation.z) != 7)
		   	goto Label_LoadTXT_ERROR;

		/////////////////////
		//// Translation ////
		/////////////////////

		if(fscanf(pFile, " %d ", &num_position_keys) != 1)
			goto Label_LoadTXT_ERROR;
		for(i=0;i<num_position_keys;i++)
		{
			if(fscanf(pFile," %f %f %f %f ",&Position.time,&Position.x,&Position.y,&Position.z) != 4)
			   	goto Label_LoadTXT_ERROR;
		}

		//////////////////
		//// Rotation ////
		//////////////////
		
		if(fscanf(pFile, " %d ", &num_rotation_keys) != 1)
			goto Label_LoadTXT_ERROR;
		for(i=0;i<num_rotation_keys;i++)
		{
			if(fscanf(pFile," %f %f %f %f ",&Rotation.time,&Rotation.x,&Rotation.y,&Rotation.z) != 4)
			   	goto Label_LoadTXT_ERROR;
		}
	}

	//////////////////////
	//// Fill Diffuse ////
	//////////////////////

//	Alp = (int)(alpha*255.0);
//	for(i=0;i<num_meshes;i++)
	for(i=0;i<1;i++)
	{
		pMesh = MeshPointers[i];
		pV    = pMesh->pLocalVerts;

		Red = Materials[pMesh->MaterialID].diffuse.r*255;	
		Grn = Materials[pMesh->MaterialID].diffuse.g*255;	
		Blu = Materials[pMesh->MaterialID].diffuse.b*255;	
		Diffuse = 0xff000000 | (Red<<16) | (Grn<<8) | Blu;
//		Diffuse = 0x66000000 | (Red<<16) | (Grn<<8) | Blu;
//		Diffuse = (Alp<<24) | (Red<<16) | (Grn<<8) | Blu;

//		int alpha = RandZeroToOne * 255.0;


		for(j=0;j<pMesh->num_verts;j++)
		{
//			alpha = (RandZeroToOne * 255.0);
//			Diffuse = (alpha<<24) | (Red<<16) | (Grn<<8) | Blu;
			pV[j].diffuse = Diffuse;
		}

		LoadTexture(Materials[pMesh->MaterialID].texname1,NULL,pMesh->Texture,TEXTURE_FLAG_4444);

	}


	//Clean Exit
	fclose(pFile);
	return pMesh;

	//Error Exit
	Label_LoadTXT_ERROR:;
	fclose(pFile);
	return NULL;
}



enum_FILE_TYPE GetFileType(char* pFileName)
{
	long i,length;
	long lastdot = -1;
	char pExt[4];

	length = strlen(pFileName);
	for(i=0;i<length;i++)
	{
		if(pFileName[i] == 46)//'.'
		{
			lastdot = i;
		}
	}
	if(lastdot==-1)
	{//no dot in name
		//return FILE_TYPE_UKN;
		return FILE_TYPE_NDT;
	}
	if((length-lastdot)<4)
	{
		return FILE_TYPE_UKN;
	}
	
	pExt[0] = tolower(pFileName[lastdot+1]);
	pExt[1] = tolower(pFileName[lastdot+2]);
	pExt[2] = tolower(pFileName[lastdot+3]);
	pExt[3] = 0;

	if(strcmp(pExt,"scn")==0) 
	{
		return FILE_TYPE_SCN;
	}
	if(strcmp(pExt,"obb")==0) 
	{
		return FILE_TYPE_OBB;
	}
	if(strcmp(pExt,"msh")==0) 
	{
		return FILE_TYPE_MSH;
	}
	if(strcmp(pExt,"ske")==0) 
	{
		return FILE_TYPE_SKE;
	}
	if(strcmp(pExt,"ase")==0) 
	{
		return FILE_TYPE_ASE;
	}
	if(strcmp(pExt,"txt")==0) 
	{
		return FILE_TYPE_TXT;
	}
	return FILE_TYPE_UKN;
}


