//Common.cpp written by david paull.
//This should be called Tools.cpp!!!
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to

#include "stdafx.h"//this all needs to go into render.cpp
#include "globals.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>//tolower

#include <time.h>//clock()
#include <assert.h>


extern FILE* DiskFile;
extern aVECTOR CameraPos;


void DoubleCreateAxisAngleFromMatrix(	double* pAxis,
										double& Angle,
										double* pM);

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




	char FullName[512];

	sprintf(FullName,"%s.bmp",pFileName);
	AddPathToFileName(FullName);



	
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	long HeaderSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
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
	if(LastError!=183)assert(LastError==0);//already exists warning
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

bool LoadTexture(	char*  pFileName,
					char* pFullPathFileName,
					aTEXTURE& pTexture,
					long flags) 
{
	unsigned char *pSrc = NULL;
	unsigned char *pDst = NULL;
	unsigned long Red = 0;
	unsigned long Grn = 0;
	unsigned long Blu = 0;
	unsigned long i,j,index,width,height;
	HBITMAP hbm;
	BITMAP bmData;
	char FullName[512];


	DiskFile=fopen("log.log","a");
	fprintf(DiskFile,"LoadTexture %s \n",pFileName);
	fclose(DiskFile);						


	if(pFullPathFileName)
	{
		sprintf(FullName,"%s",pFullPathFileName);
	}
	else
	{
		if(strlen(pFileName))
		{
			sprintf(FullName,"%s",pFileName);
			AddPathToFileName(FullName);
		}
		else
		{
			sprintf(FullName,"image.bmp");
			AddPathToFileName(FullName);
		}
	}

	//DiskFile=fopen("log.log","a");
	//fprintf(DiskFile,"try to Load %s \n",FullName);
	//fclose(DiskFile);						

	hbm = (HBITMAP)LoadImage(NULL,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hbm==NULL)//file not found
	{
		DiskFile=fopen("log.log","a");
		fprintf(DiskFile,"Original File Not Found hbm = %d \n",hbm);
		fclose(DiskFile);						

		sprintf(FullName,"default.bmp");
		AddPathToFileName(FullName);

		hbm = (HBITMAP)LoadImage(NULL,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

		if(hbm!=NULL)
		{
			GetObject(hbm,sizeof(BITMAP),&bmData);

			pSrc   = (unsigned char *)bmData.bmBits;

			strcpy(pTexture.pName,FullName);
			pTexture.Width  = bmData.bmWidth;
			pTexture.Height = bmData.bmHeight;
			pTexture.pChar  = (unsigned char *)calloc(pTexture.Width*pTexture.Height*3,sizeof(unsigned char));
			pTexture.flags  = flags;

			//local copies for readability
			width  = pTexture.Width; 
			height = pTexture.Height;
			pDst   = pTexture.pChar;
			for(j=0;j<height;j++)
			{
				memcpy(&pDst[j*width*3],&pSrc[(height-j-1)*bmData.bmWidthBytes],width*3*sizeof(char));
			}

			//Calculate UniqueID which associates an image with a number.
			//this allows us to search for any texture using a simple compare, rather than string compare. 
			//ie if(texnum==1) is better than if(strcmp(texname,name[1]))
			for(j=0;j<height;j++)
			{
				for(i=0;i<width;i++)
				{
					index = j*width*3+i*3;
					Red += pDst[index+0];
					Grn += pDst[index+1];
					Blu += pDst[index+2];
				}
			}
			pTexture.UniqueID = (Red<<6)+(Grn<<3)+Blu;

			pTexture.InVRAM   = false;

    		return true;
		}
		else//bookerror2:fixed
		{//default.bmp not found. make white texture
			strcpy(pTexture.pName,FullName);
			pTexture.Width  = 16;
			pTexture.Height = 16;
			pTexture.pChar  = (unsigned char *)calloc(pTexture.Width*pTexture.Height*3,sizeof(unsigned char));
			pTexture.flags  = flags;

			width  = pTexture.Width; 
			height = pTexture.Height;
			pDst   = pTexture.pChar;

			for(j=0;j<height;j++)
			{
				for(i=0;i<width;i++)
				{
					index = j*width*3+i*3;
					pDst[index+0] = 255;
					pDst[index+1] = 255;
					pDst[index+2] = 255;
				}
			}
			pTexture.UniqueID = 1;//problem

			pTexture.InVRAM   = false;

    		return true;
		}
	}
	else//normal load bitmap
	{
		GetObject(hbm,sizeof(BITMAP),&bmData);
		pSrc   = (unsigned char *)bmData.bmBits;


		strcpy(pTexture.pName,FullName);
		pTexture.Width  = bmData.bmWidth;
		pTexture.Height = bmData.bmHeight;
		pTexture.pChar  = (unsigned char *)calloc(pTexture.Width*pTexture.Height*3,sizeof(unsigned char));
		pTexture.flags  = flags;


		//local copies for readability
		width  = pTexture.Width; 
		height = pTexture.Height;
		pDst   = pTexture.pChar;
		for(j=0;j<height;j++)
		{
			memcpy(&pDst[j*width*3],&pSrc[(height-j-1)*bmData.bmWidthBytes],width*3*sizeof(char));
		}

		//Calculate UniqueID which associates an image with a number.
		//this allows us to search for any texture using a simple compare, rather than string compare. 
		//ie if(texnum==1) is better than if(strcmp(texname,name[1]))
		for(j=0;j<height;j++)
		{
			for(i=0;i<width;i++)
			{
				index = j*width*3+i*3;
				Red += pDst[index+0];
				Grn += pDst[index+1];
				Blu += pDst[index+2];
			}
		}
		pTexture.UniqueID = (Red<<6)+(Grn<<3)+Blu;

		pTexture.InVRAM   = false;

    	return true;
	}
}






bool LoadRGBATexture(char*  pFileName,
					char* pFullPathFileName,
					aTEXTURE& pTexture,
					long flags) 
{
	unsigned char *pSrc = NULL;
	unsigned char *pDst = NULL;
	unsigned long Red = 0;
	unsigned long Grn = 0;
	unsigned long Blu = 0;
	unsigned long Alp = 0;
	unsigned long i,j,index,width,height,dstindex;
	HBITMAP hbm;
	BITMAP bmData;
	char FullName[512];


	DiskFile=fopen("log.log","a");
	fprintf(DiskFile,"LoadTexture %s \n",pFileName);
	fclose(DiskFile);						


	if(pFullPathFileName)
	{
		sprintf(FullName,"%s",pFullPathFileName);
	}
	else
	{
		if(strlen(pFileName))
		{
			sprintf(FullName,"%s",pFileName);
			AddPathToFileName(FullName);
		}
		else
		{
			sprintf(FullName,"image.bmp");
			AddPathToFileName(FullName);
		}
	}

	DiskFile=fopen("log.log","a");
	fprintf(DiskFile,"try to Load %s \n",FullName);
	fclose(DiskFile);						

	hbm = (HBITMAP)LoadImage(NULL,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hbm==NULL)//file not found
	{
		DiskFile=fopen("log.log","a");
		fprintf(DiskFile,"Original File Not Found hbm = %d \n",hbm);
		fclose(DiskFile);						


		sprintf(FullName,"default.bmp");
		AddPathToFileName(FullName);


		DiskFile=fopen("log.log","a");
		fprintf(DiskFile,"try to Load %s \n",FullName);
		fclose(DiskFile);						

		hbm = (HBITMAP)LoadImage(NULL,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);


		DiskFile=fopen("log.log","a");
		fprintf(DiskFile,"hbm = %d \n",hbm);
		fclose(DiskFile);						
		


		GetObject(hbm,sizeof(BITMAP),&bmData);

		pSrc   = (unsigned char *)bmData.bmBits;

		strcpy(pTexture.pName,FullName);
		pTexture.Width  = bmData.bmWidth;
		pTexture.Height = bmData.bmHeight;
		pTexture.pChar  = (unsigned char *)calloc(pTexture.Width*pTexture.Height*3,sizeof(unsigned char));
		pTexture.flags  = flags;

		//local copies for readability
		width  = pTexture.Width; 
		height = pTexture.Height;
		pDst   = pTexture.pChar;

		for(j=0;j<height;j++)
		{
			for(i=0;i<width;i++)
			{
				dstindex = j*width*4+i*4;
				index    = j*bmData.bmWidthBytes+i*3;
				pDst[dstindex+0] = pSrc[index+0];
				pDst[dstindex+1] = pSrc[index+1];
				pDst[dstindex+2] = pSrc[index+2];
				Alp = (pSrc[index+0]+pSrc[index+1]+pSrc[index+2])/3;
				if(Alp>255)Alp=255;
				pDst[dstindex+3] = Alp;
//			memcpy(&pDst[j*width*3],&pSrc[(height-j-1)*bmData.bmWidthBytes],width*3*sizeof(char));
			}
		}

//		for(j=0;j<height;j++)
//		{
//			memcpy(&pDst[j*width*3],&pSrc[(height-j-1)*bmData.bmWidthBytes],width*3*sizeof(char));
//		}
//
		//Calculate UniqueID which associates an image with a number.
		//this allows us to search for any texture using a simple compare, rather than string compare. 
		//ie if(texnum==1) is better than if(strcmp(texname,name[1]))
		for(j=0;j<height;j++)
		{
			for(i=0;i<width;i++)
			{
				index = j*width*3+i*3;
				Red += pDst[index+0];
				Grn += pDst[index+1];
				Blu += pDst[index+2];
			}
		}
		pTexture.UniqueID = (Red<<6)+(Grn<<3)+Blu;

		pTexture.InVRAM   = false;

    	return true;
	}
	else//normal load bitmap
	{
		GetObject(hbm,sizeof(BITMAP),&bmData);
		pSrc   = (unsigned char *)bmData.bmBits;

		DiskFile=fopen("log.log","a");
		fprintf(DiskFile,"Normal 1 \n");
		fclose(DiskFile);						

		strcpy(pTexture.pName,FullName);
		pTexture.Width  = bmData.bmWidth;
		pTexture.Height = bmData.bmHeight;
		pTexture.pChar  = (unsigned char *)calloc(pTexture.Width*pTexture.Height*3,sizeof(unsigned char));
		pTexture.flags  = flags;

		DiskFile=fopen("log.log","a");
		fprintf(DiskFile,"Normal 2 \n");
		fclose(DiskFile);						

		//local copies for readability
		width  = pTexture.Width; 
		height = pTexture.Height;
		pDst   = pTexture.pChar;
		for(j=0;j<height;j++)
		{
			memcpy(&pDst[j*width*3],&pSrc[(height-j-1)*bmData.bmWidthBytes],width*3*sizeof(char));
		}
		DiskFile=fopen("log.log","a");
		fprintf(DiskFile,"Normal 3 \n");
		fclose(DiskFile);						

		//Calculate UniqueID which associates an image with a number.
		//this allows us to search for any texture using a simple compare, rather than string compare. 
		//ie if(texnum==1) is better than if(strcmp(texname,name[1]))
		for(j=0;j<height;j++)
		{
			for(i=0;i<width;i++)
			{
				index = j*width*3+i*3;
				Red += pDst[index+0];
				Grn += pDst[index+1];
				Blu += pDst[index+2];
			}
		}
		pTexture.UniqueID = (Red<<6)+(Grn<<3)+Blu;

		pTexture.InVRAM   = false;

    	return true;
	}
}

bool CreateTexture(char* pName,
					aTEXTURE& pTexture,
					long Width,
					long Height,
					long flags) 
{
	unsigned char *pSrc = NULL;
	unsigned char *pDst = NULL;
	unsigned long Red = 0;
	unsigned long Grn = 0;
	unsigned long Blu = 0;

	strcpy(pTexture.pName,pName);
	pTexture.Width  = Width;
	pTexture.Height = Height;
	pTexture.pChar  = (unsigned char *)calloc(pTexture.Width*pTexture.Height*3,sizeof(unsigned char));
	pTexture.flags  = flags;
	pTexture.UniqueID = 0;//BAD THINGS if this isnt updated
	pTexture.InVRAM   = false;
   	return true;
}

bool UpdateUniqueID(aTEXTURE& pTexture)
{
	unsigned char *pSrc = NULL;
	unsigned long Red = 0;
	unsigned long Grn = 0;
	unsigned long Blu = 0;
	unsigned long i,j,index;

	assert(pTexture.InVRAM==false);//unique id is used to correlate with a texture in VRAM. dont change the num if it is in VRAM
	assert(pTexture.pChar);

	pSrc = pTexture.pChar;
	for(j=0;j<pTexture.Height;j++)
	{
		for(i=0;i<pTexture.Width;i++)
		{
			index = j*pTexture.Width*3+i*3;
			Red += pSrc[index+0];
			Grn += pSrc[index+1];
			Blu += pSrc[index+2];
		}
	}
	pTexture.UniqueID = (Red<<6)+(Grn<<3)+Blu;
   	return true;
}
void ReleaseTexture(aTEXTURE* pTexture)
{
	free(pTexture->pChar);
	free(pTexture->pAlpha);
	free(pTexture->pAPITEXTURE);
	memset(pTexture,0,sizeof(aTEXTURE));
}



/*
void Transpose(float *pMat)
{
	float pTemp[16];
	memcpy(pTemp,pMat,16*sizeof(float));
	pMat[0 ] = pTemp[0 ];
	pMat[1 ] = pTemp[4 ];
	pMat[2 ] = pTemp[8 ];
	pMat[3 ] = pTemp[12];
	pMat[4 ] = pTemp[1 ];
	pMat[5 ] = pTemp[5 ];
	pMat[6 ] = pTemp[9 ];
	pMat[7 ] = pTemp[13];
	pMat[8 ] = pTemp[2 ];
	pMat[9 ] = pTemp[6 ];
	pMat[10] = pTemp[10];
	pMat[11] = pTemp[14];
	pMat[12] = pTemp[3];
	pMat[13] = pTemp[7];
	pMat[14] = pTemp[11];
	pMat[15] = pTemp[15];
}

void LeftToRightHand(float *pMat)
{
//	pMat[0 ] = -pMat[0 ];
//	pMat[4 ] = -pMat[4 ];
//	pMat[8 ] = -pMat[8 ];
//	pMat[12] = -pMat[12];

	pMat[1 ] = -pMat[1 ];
	pMat[5 ] = -pMat[5 ];
	pMat[9 ] = -pMat[9 ];
	pMat[13] = -pMat[13];

//	pMat[2 ] = -pMat[2 ];
//	pMat[6 ] = -pMat[6 ];
//	pMat[10] = -pMat[10];
//	pMat[14] = -pMat[14];

//	pMat[0 ] = -pMat[0 ];
//	pMat[4 ] = -pMat[4 ];
//	pMat[8 ] = -pMat[8 ];
//	pMat[12] = -pMat[12];
//	pMat[1 ] = -pMat[1 ];
//	pMat[5 ] = -pMat[5 ];
//	pMat[9 ] = -pMat[9 ];
//	pMat[13] = -pMat[13];


//	pMat[8 ] = -pMat[8 ];
//	pMat[9 ] = -pMat[9 ];
//	pMat[10] = -pMat[10];
//	pMat[11] = -pMat[11];
}

*/









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


void SetBit(unsigned long& flags,unsigned long flag)
{
	flags |= flag;
}
void SwapBit(unsigned long& flags,unsigned long flag)
{
	flags ^= flag;
}
void ClearBit(unsigned long& flags,unsigned long flag)
{
	if(flags&flag)
	{
		//??flags ^= flag;
		flags-=flag;
	}
}
bool IsBitSet(unsigned long& flags,unsigned long flag)
{
	if(flags&flag)
	{
		return true;
	}
	return false;
}

bool PointInRect(POINT point,fRECT Clip)
{
	float x = (float)point.x;
	float y = (float)point.y;
	if(x>Clip.left && x<Clip.right)
	{
		if(y<Clip.bottom && y>Clip.top)
		{
			return true;
		}
	}
	return false;
}
fRECT ClipRectToRect(fRECT Rect,fRECT Clip)
{
	fRECT Final;
	Final.left   = 0.0;
	Final.right  = 0.0;
	Final.top    = 0.0;
	Final.bottom = 0.0;
	//first test if rect is totally outside the clip region ...
	//or if partially clipped. if(Rect.left<Clip.left)

	//Clip Rect To ClipRect's Left Plane
	if(Rect.left<Clip.left)
	{
		//Test For Total Out
		if(Rect.right<Clip.left)
		{
			return Final;
		}
		Rect.left = Clip.left;
	}
	//Clip Rect To ClipRect's Right Plane
	if(Rect.right>Clip.right)
	{
		//Test For Total Out
		if(Rect.left>Clip.right)
		{
			return Final;
		}
		Rect.right = Clip.right;
	}
	//Clip Rect To ClipRect's top Plane
	if(Rect.top<Clip.top)
	{
		//Test For Total Out
		if(Rect.bottom<Clip.top)
		{
			return Final;
		}
		Rect.top = Clip.top;
	}
	//Clip Rect To ClipRect's Bottom Plane
	if(Rect.bottom>Clip.bottom)
	{
		//Test For Total Out
		if(Rect.top>Clip.bottom)
		{
			return Final;
		}
		Rect.bottom = Clip.bottom;
	}
	Final.left   = Rect.left;
	Final.right  = Rect.right;
	Final.top    = Rect.top;
	Final.bottom = Rect.bottom;
	return Final;
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
BOOL IntersectTriangle( aVECTOR& orig,
                        aVECTOR& dir, 
                        aVECTOR& v0,
                        aVECTOR& v1, 
                        aVECTOR& v2,
						float* t, 
						float* u, 
						float* v)
{
    FLOAT det;
    // Find vectors for two edges sharing vert0
    aVECTOR edge1;
    aVECTOR edge2;

    edge1.x = v1.x - v0.x;
    edge1.y = v1.y - v0.y;
    edge1.z = v1.z - v0.z;


    edge2.x = v2.x - v0.x;
    edge2.y = v2.y - v0.y;
    edge2.z = v2.z - v0.z;



    // Begin calculating determinant - also used to calculate U parameter
    aVECTOR pvec;
//    D3DXVec3Cross( &pvec, &dir, &edge2 );
	MacroCross(	pvec.x,pvec.y,pvec.z,
				dir.x,dir.y,dir.z,
				edge2.x,edge2.y,edge2.z);

    // If determinant is near zero, ray lies in plane of triangle
//    FLOAT det = D3DXVec3Dot( &edge1, &pvec );
	MacroDot(det,
			 edge1.x,edge1.y,edge1.z,
			 pvec.x,pvec.y,pvec.z);

    if( det < 0.0001f )
        return FALSE;

    // Calculate distance from vert0 to ray origin
    aVECTOR tvec;

	tvec.x = orig.x - v0.x;
	tvec.y = orig.y - v0.y;
	tvec.z = orig.z - v0.z;

    // Calculate U parameter and test bounds
	float TempU;
	float TempV;
	float TempT;

//    *u = D3DXVec3Dot( &tvec, &pvec );

	MacroDot(TempU,
			 tvec.x,tvec.y,tvec.z,
			 pvec.x,pvec.y,pvec.z);
    *u = TempU;


    if( *u < 0.0f || *u > det )
        return FALSE;

    // Prepare to test V parameter
    aVECTOR qvec;
//  D3DXVec3Cross( &qvec, &tvec, &edge1 );

	MacroCross(	qvec.x,qvec.y,qvec.z,
				tvec.x,tvec.y,tvec.z,
				edge1.x,edge1.y,edge1.z);


    // Calculate V parameter and test bounds
//    *v = D3DXVec3Dot( &dir, &qvec );

	MacroDot(TempV,
			 dir.x,dir.y,dir.z,
			 qvec.x,qvec.y,qvec.z);
    *v = TempV;


    if( *v < 0.0f || *u + *v > det )
        return FALSE;

    // Calculate t, scale parameters, ray intersects triangle
//    *t = D3DXVec3Dot( &edge2, &qvec );
	MacroDot(TempT,
			 edge2.x,edge2.y,edge2.z,
			 qvec.x,qvec.y,qvec.z);
    *t = TempT;


    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return TRUE;
}

void Identity(float *pMat)
{
	memset(pMat,0,16*sizeof(float));
	pMat[0 ] = 1.0;
	pMat[5 ] = 1.0;
	pMat[10] = 1.0;
	pMat[15] = 1.0;
}

void Scale(float *pMat,float scale)
{
	memset(pMat,0,16*sizeof(float));
	pMat[0 ] = scale;
	pMat[5 ] = scale;
	pMat[10] = scale;
	pMat[15] = 1.0;
}
void Scale(	float *pMat,
			float sx,
			float sy,
			float sz)
{
	memset(pMat,0,16*sizeof(float));
	pMat[0 ] = sx;
	pMat[5 ] = sy;
	pMat[10] = sz;
	pMat[15] = 1.0;
}

void CreateRandomVector(aVECTOR* pMat)
{
	float Length;
	pMat->x = RandNegOneToOne;
	pMat->y = RandNegOneToOne;
	pMat->z = RandNegOneToOne;
	MacroNormalize(Length,pMat->x,pMat->y,pMat->z);	
}

void PositionMatrix(float *pMat,float Ax,float Ay,float Az)
{
    pMat[12] = Ax;
    pMat[13] = Ay;
    pMat[14] = Az;
    pMat[15] = 1.0;
} 


float* CreateMatrixRotationX(float* pOut, float angle)
{
    float fsin,fcos;

    if(pOut==NULL)
        return NULL;

    fsin = sin(angle);
    fcos = cos(angle);

    pOut[0]  = 1.0f; pOut[1]  =  0.0f; pOut[2]  = 0.0f; pOut[3]  = 0.0f;
    pOut[4]  = 0.0f; pOut[5]  =  fcos; pOut[6]  = fsin; pOut[7]  = 0.0f;
    pOut[8]  = 0.0f; pOut[9]  = -fsin; pOut[10] = fcos; pOut[11] = 0.0f;
    pOut[12] = 0.0f; pOut[13] =  0.0f; pOut[14] = 0.0f; pOut[15] = 1.0f;
    return pOut;
}

float* CreateMatrixRotationY(float* pOut, float angle)
{
    float fsin,fcos;

    if(pOut==NULL)
        return NULL;

    fsin = sin(angle);
    fcos = cos(angle);

    pOut[0]  = fcos; pOut[1]  =  0.0f; pOut[2]  = -fsin; pOut[3]  = 0.0f;
    pOut[4]  = 0.0f; pOut[5]  =  1.0f; pOut[6]  =  0.0f; pOut[7]  = 0.0f;
    pOut[8]  = fsin; pOut[9]  =  0.0f; pOut[10] =  fcos; pOut[11] = 0.0f;
    pOut[12] = 0.0f; pOut[13] =  0.0f; pOut[14] =  0.0f; pOut[15] = 1.0f;

    return pOut;
}


float* CreateMatrixRotationZ(float* pOut, float angle)
{
    float fsin,fcos;

    if(pOut==NULL)
        return NULL;

    fsin = sin(angle);
    fcos = cos(angle);

    pOut[0]  =  fcos; pOut[1]  =  fsin; pOut[2]  =  0.0f; pOut[3]  = 0.0f;
    pOut[4]  = -fsin; pOut[5]  =  fcos; pOut[6]  =  0.0f; pOut[7]  = 0.0f;
    pOut[8]  =  0.0f; pOut[9]  =  0.0f; pOut[10] =  1.0f; pOut[11] = 0.0f;
    pOut[12] =  0.0f; pOut[13] =  0.0f; pOut[14] =  0.0f; pOut[15] = 1.0f;
    return pOut;
}

//Checked With D3DXMatrixRotationYawPitchRoll(&Mat1,Yaw,Pitch,Roll);
float* CreateMatrixYawPitchRoll(float* pOut,float yaw, float pitch, float roll)
{
	float pMat1[16];
	float pMat2[16];
	float pMat3[16];
	float pMat4[16];

	CreateMatrixRotationX(pMat1,pitch);
	CreateMatrixRotationY(pMat2,yaw);
	CreateMatrixRotationZ(pMat3,roll);

	MatrixMul(pMat4,pMat1,pMat2);
	MatrixMul( pOut,pMat3,pMat4);
	return pOut;
}

void MatrixMul(float* pDst,float* pA,float* pB)
{
	assert(pDst!=pA);//src, and dst cant be same. no a=a*b
	assert(pDst!=pB);
	pDst[0]  =  (pA[0]  * pB[0])  +  (pA[1]  * pB[4])  +  (pA[2]  * pB[8] );
	pDst[1]  =  (pA[0]  * pB[1])  +  (pA[1]  * pB[5])  +  (pA[2]  * pB[9] );
	pDst[2]  =  (pA[0]  * pB[2])  +  (pA[1]  * pB[6])  +  (pA[2]  * pB[10]);
	pDst[3]  =  0.0;
	pDst[4]  =  (pA[4]  * pB[0])  +  (pA[5]  * pB[4])  +  (pA[6]  * pB[8] );
	pDst[5]  =  (pA[4]  * pB[1])  +  (pA[5]  * pB[5])  +  (pA[6]  * pB[9] );
	pDst[6]  =  (pA[4]  * pB[2])  +  (pA[5]  * pB[6])  +  (pA[6]  * pB[10]);
	pDst[7]  =  0.0;
	pDst[8]  =  (pA[8]  * pB[0])  +  (pA[9]  * pB[4])  +  (pA[10] * pB[8] );
	pDst[9]  =  (pA[8]  * pB[1])  +  (pA[9]  * pB[5])  +  (pA[10] * pB[9] );
	pDst[10] =  (pA[8]  * pB[2])  +  (pA[9]  * pB[6])  +  (pA[10] * pB[10]);
	pDst[11] =  0.0;
	pDst[12] =  (pA[12] * pB[0])  +  (pA[13] * pB[4])  +  (pA[14] * pB[8] )  + pB[12];
	pDst[13] =  (pA[12] * pB[1])  +  (pA[13] * pB[5])  +  (pA[14] * pB[9] )  + pB[13];
	pDst[14] =  (pA[12] * pB[2])  +  (pA[13] * pB[6])  +  (pA[14] * pB[10])  + pB[14];
	pDst[15] =  1.0;
}

void DoubleMatrixMul(double* pDst,float* pA,float* pB)
{
	pDst[0]  =  ((double)pA[0]  * (double)pB[0])  +  ((double)pA[1]  * (double)pB[4])  +  ((double)pA[2]  * (double)pB[8] );
	pDst[1]  =  ((double)pA[0]  * (double)pB[1])  +  ((double)pA[1]  * (double)pB[5])  +  ((double)pA[2]  * (double)pB[9] );
	pDst[2]  =  ((double)pA[0]  * (double)pB[2])  +  ((double)pA[1]  * (double)pB[6])  +  ((double)pA[2]  * (double)pB[10]);
	pDst[3]  =  0.0;
	pDst[4]  =  ((double)pA[4]  * (double)pB[0])  +  ((double)pA[5]  * (double)pB[4])  +  ((double)pA[6]  * (double)pB[8] );
	pDst[5]  =  ((double)pA[4]  * (double)pB[1])  +  ((double)pA[5]  * (double)pB[5])  +  ((double)pA[6]  * (double)pB[9] );
	pDst[6]  =  ((double)pA[4]  * (double)pB[2])  +  ((double)pA[5]  * (double)pB[6])  +  ((double)pA[6]  * (double)pB[10]);
	pDst[7]  =  0.0;
	pDst[8]  =  ((double)pA[8]  * (double)pB[0])  +  ((double)pA[9]  * (double)pB[4])  +  ((double)pA[10] * (double)pB[8] );
	pDst[9]  =  ((double)pA[8]  * (double)pB[1])  +  ((double)pA[9]  * (double)pB[5])  +  ((double)pA[10] * (double)pB[9] );
	pDst[10] =  ((double)pA[8]  * (double)pB[2])  +  ((double)pA[9]  * (double)pB[6])  +  ((double)pA[10] * (double)pB[10]);
	pDst[11] =  0.0;
	pDst[12] =  ((double)pA[12] * (double)pB[0])  +  ((double)pA[13] * (double)pB[4])  +  ((double)pA[14] * (double)pB[8] )  + (double)pB[12];
	pDst[13] =  ((double)pA[12] * (double)pB[1])  +  ((double)pA[13] * (double)pB[5])  +  ((double)pA[14] * (double)pB[9] )  + (double)pB[13];
	pDst[14] =  ((double)pA[12] * (double)pB[2])  +  ((double)pA[13] * (double)pB[6])  +  ((double)pA[14] * (double)pB[10])  + (double)pB[14];
	pDst[15] =  1.0;
}

void RotateVectorUsingMatrix(	aVECTOR* pDstV,
								aVECTOR* pV,
								float* pM)
{
	pDstV->x = pV->x*pM[0] + pV->y*pM[4] + pV->z* pM[ 8];
	pDstV->y = pV->x*pM[1] + pV->y*pM[5] + pV->z* pM[ 9];
	pDstV->z = pV->x*pM[2] + pV->y*pM[6] + pV->z* pM[10];
}

void RotatePointUsingMatrix(aVECTOR* pDstV,
							aVECTOR* pP,
							float* pM)
{
	pDstV->x = pP->x*pM[0] + pP->y*pM[4] + pP->z*pM[ 8] + pM[12];
	pDstV->y = pP->x*pM[1] + pP->y*pM[5] + pP->z*pM[ 9] + pM[13];
	pDstV->z = pP->x*pM[2] + pP->y*pM[6] + pP->z*pM[10] + pM[14];
}
void CreateRotationMatrixAroundWorldX(float* pM,float Radians)
{
	float Sin = (float)sin(Radians);
	float Cos = (float)cos(Radians);
	if(pM == NULL)return;
	Identity(pM);
	pM[ 5] =  Cos;
	pM[ 6] =  Sin;
	pM[ 9] = -Sin;
	pM[10] =  Cos;
}
void CreateRotationMatrixAroundWorldY(float* pM,float Radians)
{
	float Sin = (float)sin(Radians);
	float Cos = (float)cos(Radians);
	if(pM == NULL)return;
	Identity(pM);
	pM[ 0] =  Cos;
	pM[ 2] = -Sin;
	pM[ 8] =  Sin;
	pM[10] =  Cos;
}
void CreateRotationMatrixAroundWorldZ(float* pM,float Radians)
{
	float Sin = (float)sin(Radians);
	float Cos = (float)cos(Radians);
	if(pM == NULL)return;
	Identity(pM);
	pM[0] =  Cos;
	pM[1] =  Sin;
	pM[4] = -Sin;
	pM[5] =  Cos;
}

void AxisRotationMatrix(float *pMat,
						float Ax,
						float Ay,
						float Az,
						float Angle)
{
	float Length;
	float fCos = (float)cos(Angle);
	float fSin = (float)sin(Angle);
	if(pMat == NULL)return;
	MacroNormalize(Length,Ax,Ay,Az);

	pMat[0]  = (Ax*Ax)*(1.0f-fCos)+fCos;
	pMat[1]  = (Ay*Ax)*(1.0f-fCos)+(Az*fSin);
	pMat[2]  = (Az*Ax)*(1.0f-fCos)-(Ay*fSin);
	pMat[3]  = 0.0;

	pMat[4]  = (Ax*Ay)*(1.0f-fCos)-(Az*fSin);
	pMat[5]  = (Ay*Ay)*(1.0f-fCos)+fCos ;
	pMat[6]  = (Az*Ay)*(1.0f-fCos)+(Ax*fSin);
	pMat[7]  = 0.0;

	pMat[8]  = (Ax*Az)*(1.0f-fCos)+(Ay*fSin);
	pMat[9]  = (Ay*Az)*(1.0f-fCos)-(Ax*fSin);
	pMat[10] = (Az*Az)*(1.0f-fCos)+fCos;
	pMat[11] = 0.0;

	pMat[12] = 0.0;
	pMat[13] = 0.0;
	pMat[14] = 0.0;
	pMat[15] = 1.0;
} 

void DoubleAxisRotationMatrix(	double *pMat,
								double Ax,
								double Ay,
								double Az,
								double Angle)
{
	double ooLength;
	double fCos = cos(Angle);
	double fSin = sin(Angle);
	if(pMat == NULL)return;

	//normalize
	ooLength = (1.0/(sqrt(Ax*Ax+Ay*Ay+Az*Az)));
	Ax  *= ooLength;
	Ay  *= ooLength;
	Az  *= ooLength;
	//normalize

	pMat[0]  = (Ax*Ax)*(1.0f-fCos)+fCos;
	pMat[1]  = (Ay*Ax)*(1.0f-fCos)+(Az*fSin);
	pMat[2]  = (Az*Ax)*(1.0f-fCos)-(Ay*fSin);
	pMat[3]  = 0.0;

	pMat[4]  = (Ax*Ay)*(1.0f-fCos)-(Az*fSin);
	pMat[5]  = (Ay*Ay)*(1.0f-fCos)+fCos ;
	pMat[6]  = (Az*Ay)*(1.0f-fCos)+(Ax*fSin);
	pMat[7]  = 0.0;

	pMat[8]  = (Ax*Az)*(1.0f-fCos)+(Ay*fSin);
	pMat[9]  = (Ay*Az)*(1.0f-fCos)-(Ax*fSin);
	pMat[10] = (Az*Az)*(1.0f-fCos)+fCos;
	pMat[11] = 0.0;

	pMat[12] = 0.0;
	pMat[13] = 0.0;
	pMat[14] = 0.0;
	pMat[15] = 1.0;
} 


//Inverse Rotation Matrix. 
void InverseMatrix(float* pDst,float* pSrc)
{
	//////////////////////////////////////////////
	//// Calculate Transpose Of Source Matrix ////
	//////////////////////////////////////////////

	if(pSrc == NULL)return;
	if(pDst == NULL)return;

 	pDst[0]  = pSrc[0]; 
 	pDst[1]  = pSrc[4]; 
 	pDst[2]  = pSrc[8]; 
 	pDst[3]  = 0.0; 

	pDst[4]  = pSrc[1]; 
	pDst[5]  = pSrc[5]; 
	pDst[6]  = pSrc[9]; 
 	pDst[7]  = 0.0; 

	pDst[8]  = pSrc[2]; 
	pDst[9]  = pSrc[6]; 
	pDst[10] = pSrc[10];
 	pDst[11] = 0.0; 

	//////////////////////////////////////////////////////////////////////
	//// Transform Negated Source Position Into New Coordinate System ////
	//////////////////////////////////////////////////////////////////////

	pDst[12] = (-pSrc[12]*pDst[0])+(-pSrc[13]*pDst[4])+(-pSrc[14]*pDst[8] );
	pDst[13] = (-pSrc[12]*pDst[1])+(-pSrc[13]*pDst[5])+(-pSrc[14]*pDst[9] );
	pDst[14] = (-pSrc[12]*pDst[2])+(-pSrc[13]*pDst[6])+(-pSrc[14]*pDst[10]);
 	pDst[15] = 1.0; 
}

void TransposeMatrix(float* pDst,float* pSrc)
{
	//////////////////////////////////////////////
	//// Calculate Transpose Of Source Matrix ////
	//////////////////////////////////////////////

	if(pSrc == NULL)return;
	if(pDst == NULL)return;

 	pDst[0]  = pSrc[0]; 
 	pDst[1]  = pSrc[4]; 
 	pDst[2]  = pSrc[8]; 

	pDst[4]  = pSrc[1]; 
	pDst[5]  = pSrc[5]; 
	pDst[6]  = pSrc[9]; 

	pDst[8]  = pSrc[2]; 
	pDst[9]  = pSrc[6]; 
	pDst[10] = pSrc[10];
}
/*
void InverseMatrix(double* pDst,float* pSrc)
{
	//////////////////////////////////////////////
	//// Calculate Transpose Of Source Matrix ////
	//////////////////////////////////////////////

 	pDst[0]  = (double)pSrc[0]; 
 	pDst[1]  = (double)pSrc[4]; 
 	pDst[2]  = (double)pSrc[8]; 
 	pDst[3]  = (double)0.0; 

	pDst[4]  = (double)pSrc[1]; 
	pDst[5]  = (double)pSrc[5]; 
	pDst[6]  = (double)pSrc[9]; 
 	pDst[7]  = (double)0.0; 

	pDst[8]  = (double)pSrc[2]; 
	pDst[9]  = (double)pSrc[6]; 
	pDst[10] = (double)pSrc[10];
 	pDst[11] = (double)0.0; 

	//////////////////////////////////////////////////////////////////////
	//// Transform Negated Source Position Into New Coordinate System ////
	//////////////////////////////////////////////////////////////////////

	pDst[12] = (-(double)pSrc[12]*pDst[0])+(-(double)pSrc[13]*pDst[4])+(-(double)pSrc[14]*pDst[8] );
	pDst[13] = (-(double)pSrc[12]*pDst[1])+(-(double)pSrc[13]*pDst[5])+(-(double)pSrc[14]*pDst[9] );
	pDst[14] = (-(double)pSrc[12]*pDst[2])+(-(double)pSrc[13]*pDst[6])+(-(double)pSrc[14]*pDst[10]);
 	pDst[15] = 1.0; 
}
*/


//basically a 180 degree rotation around matrices V Axis
//Fast, and well suited to particles that always face camera.
void CreateMatrixFacingMatrix(	float* pDstM,
								float* pView,
								float x,
								float y,
								float z)
{
	if(pDstM == NULL)return;
	if(pView == NULL)return;

	Identity(pDstM);

	pDstM[0 ] = -pView[0 ];
	pDstM[1 ] = -pView[1 ];
	pDstM[2 ] = -pView[2 ];

	pDstM[4 ] =  pView[4 ];
	pDstM[5 ] =  pView[5 ];
	pDstM[6 ] =  pView[6 ];

	pDstM[ 8] = -pView[8 ];
	pDstM[ 9] = -pView[9 ];
	pDstM[10] = -pView[10];

/*
	pDstM[0]  = pView[0];
	pDstM[1]  = pView[4];
	pDstM[2]  = pView[8];

	pDstM[4]  = pView[1];
	pDstM[5]  = pView[5];
	pDstM[6]  = pView[9];

	pDstM[ 8] = -pView[2];
	pDstM[ 9] = -pView[6];
	pDstM[10] = -pView[10];
*/

	pDstM[12] = x;
	pDstM[13] = y;
	pDstM[14] = z;
}







void CreateMatrixFacingPointOnOneAxis(	aVECTOR& Pos,//position of final matrix
										aVECTOR& Dir,//axis to rotate about
										aVECTOR& LookAt,//point to look at
										float* pDst)
{
	float Length;
	aVECTOR vU;
	aVECTOR vV;
	aVECTOR vN;

	if(pDst == NULL)return;


	vN.x = LookAt.x-Pos.x;
	vN.y = LookAt.y-Pos.y;
	vN.z = LookAt.z-Pos.z;
	MacroNormalize(Length,vN.x,vN.y,vN.z);

	vV.x = Dir.x;
	vV.y = Dir.y;
	vV.z = Dir.z;
	MacroNormalize(Length,vV.x,vV.y,vV.z);

	MacroCross(	vU.x,vU.y,vU.z,
				vV.x,vV.y,vV.z,
				vN.x,vN.y,vN.z);

	MacroNormalize(Length,vU.x,vU.y,vU.z);

	MacroCross(	vN.x,vN.y,vN.z,
				vU.x,vU.y,vU.z,
				vV.x,vV.y,vV.z);


 	pDst[0]  = vU.x;
 	pDst[1]  = vU.y;
 	pDst[2]  = vU.z;
 	pDst[3]  = 0.0; 

	pDst[4]  = vV.x;
	pDst[5]  = vV.y;
	pDst[6]  = vV.z;
 	pDst[7]  = 0.0; 

	pDst[8]  = vN.x;
	pDst[9]  = vN.y;
	pDst[10] = vN.z;
 	pDst[11] = 0.0; 

	pDst[12] = Pos.x;
	pDst[13] = Pos.y;
	pDst[14] = Pos.z;
 	pDst[15] = 1.0; 



//	vAxis.x = Dir.x;
//	vAxis.y = Dir.y;
//	vAxis.z = Dir.z;

//	vPtoC.x = LookAt.x-Pos.x;
//	vPtoC.y = LookAt.y-Pos.y;
//	vPtoC.z = LookAt.z-Pos.z;

//	MacroNormalize(Length,vAxis.x,vAxis.y,vAxis.z);
//	MacroNormalize(Length,vPtoC.x,vPtoC.y,vPtoC.z);	
/*
	MacroCross(	vPerpToAxis.x,vPerpToAxis.y,vPerpToAxis.z,
				vPtoC.x,vPtoC.y,vPtoC.z,
				vAxis.x,vAxis.y,vAxis.z);
	MacroNormalize(Length,vPerpToAxis.x,vPerpToAxis.y,vPerpToAxis.z);

 	pDst[0]  = vPerpToAxis.x;
 	pDst[1]  = vPerpToAxis.y;
 	pDst[2]  = vPerpToAxis.z;
 	pDst[3]  = 0.0; 

	pDst[4]  = vAxis.x;
	pDst[5]  = vAxis.y;
	pDst[6]  = vAxis.z;
 	pDst[7]  = 0.0; 

	pDst[8]  = -vPtoC.x;
	pDst[9]  = -vPtoC.y;
	pDst[10] = -vPtoC.z;
 	pDst[11] = 0.0; 

	pDst[12] = Pos.x;
	pDst[13] = Pos.y;
	pDst[14] = Pos.z;
 	pDst[15] = 1.0; 
*/
}



void CreateLookAtMatrix(aVECTOR& Pos,
						aVECTOR& Up,//typically <0,1,0>
						aVECTOR& LookAtPos,
						float* pDst)
{
	float Length;
	aVECTOR vU;
	aVECTOR vV;
	aVECTOR vN;

	if(pDst == NULL)return;

	vN.x = LookAtPos.x-Pos.x;
	vN.y = LookAtPos.y-Pos.y;
	vN.z = LookAtPos.z-Pos.z;
	MacroNormalize(Length,vN.x,vN.y,vN.z);

	vV.x = Up.x;
	vV.y = Up.y;
	vV.z = Up.z;
	MacroNormalize(Length,vV.x,vV.y,vV.z);
	MacroCross(	vU.x,vU.y,vU.z,
				vV.x,vV.y,vV.z,
				vN.x,vN.y,vN.z);

	MacroCross(	vV.x,vV.y,vV.z,
				vN.x,vN.y,vN.z,
				vU.x,vU.y,vU.z);

 	pDst[0]  = vU.x;
 	pDst[1]  = vU.y;
 	pDst[2]  = vU.z;
 	pDst[3]  = 0.0; 

	pDst[4]  = vV.x;
	pDst[5]  = vV.y;
	pDst[6]  = vV.z;
 	pDst[7]  = 0.0; 

	pDst[8]  = vN.x;
	pDst[9]  = vN.y;
	pDst[10] = vN.z;
 	pDst[11] = 0.0; 

	pDst[12] = Pos.x;
	pDst[13] = Pos.y;
	pDst[14] = Pos.z;
 	pDst[15] = 1.0; 
}
void CreateQuaternionArrowMatrix(aVECTOR& Pos,
								aVECTOR& Up,//typically <0,1,0>
								aQUATERNION& pQ,
								float* pDst)
{
	float pMat1[16];
	float pMat2[16];
	aVECTOR LookAtPos;
	aVECTOR Axis;


    float cosAngle = pQ.w;
    float sinAngle = sqrt(1.0-cosAngle*cosAngle);
	float angle = acos(cosAngle) * 2.0;

	LookAtPos.x = pQ.x/sinAngle;  
	LookAtPos.y = pQ.y/sinAngle;  
	LookAtPos.z = pQ.z/sinAngle;  

	CreateLookAtMatrix(Pos,Up,LookAtPos,pMat1);

	Axis.x = pDst[8]; 
	Axis.y = pDst[9]; 
	Axis.z = pDst[10];

	CreateRotationMatrixAroundWorldZ(pMat2,angle);
	MatrixMul(pDst,pMat2,pMat1);
}

void CreateQuaternionFadeMatrix(aVECTOR& Pos,
								aVECTOR& Up,//typically <0,1,0>
								aQUATERNION& pQ,
								float* pDst)
{


	aVECTOR LookAtPos;

    float cosAngle = pQ.w;
    float sinAngle = sqrt(1.0-cosAngle*cosAngle);
	float angle = acos(cosAngle) * 2.0;

	LookAtPos.x = pQ.x/sinAngle;  
	LookAtPos.y = pQ.y/sinAngle;  
	LookAtPos.z = pQ.z/sinAngle;  

	CreateLookAtMatrix(Pos,Up,LookAtPos,pDst);
}


void CreateMatrixFromQuaternionOld(float* pM,
								aQUATERNION& pQ)
{
/*
    float xx = pQ.x*pQ.x;
    float xy = pQ.x*pQ.y;
    float wx = pQ.w*pQ.x;
    float yy = pQ.y*pQ.y;
    float xz = pQ.x*pQ.z;
    float wy = pQ.w*pQ.y;
    float zz = pQ.z*pQ.z;
    float yz = pQ.y*pQ.z;
    float wz = pQ.w*pQ.z;
    
    pM[0]  = 1 - 2 * ( yy + zz ); 
    pM[1]  =     2 * ( xy - wz );
    pM[2]  =     2 * ( xz + wy );
    pM[3]  = 0.0;

    pM[4]  =     2 * ( xy + wz );
    pM[5]  = 1 - 2 * ( xx + zz );
    pM[6]  =     2 * ( yz - wx );
    pM[7]  = 0.0;

    pM[8]  =     2 * ( xz - wy );
    pM[9]  =     2 * ( yz + wx );
    pM[10] = 1 - 2 * ( xx + yy );
    pM[11] = 0.0;

    pM[12] = 0.0;
    pM[13] = 0.0;
    pM[14] = 0.0;
    pM[15] = 1.0;
*/


	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	x2 = pQ.x + pQ.x; 
	y2 = pQ.y + pQ.y; 
	z2 = pQ.z + pQ.z;
	xx = pQ.x * x2;   
	xy = pQ.x * y2;   
	xz = pQ.x * z2;
	yy = pQ.y * y2;   
	yz = pQ.y * z2;   
	zz = pQ.z * z2;
	wx = pQ.w * x2;   
	wy = pQ.w * y2;   
	wz = pQ.w * z2;

	pM[0]  = 1.0 - (yy + zz);
	pM[1]  = xy + wz;
	pM[2]  = xz - wy;
	pM[3]  = 0.0;

	pM[4]  = xy - wz;
	pM[5]  = 1.0 - (xx + zz);
	pM[6]  = yz + wx;
	pM[7]  = 0.0;

	pM[8]  = xz + wy;
	pM[9]  = yz - wx;
	pM[10] = 1.0 - (xx + yy);
	pM[11] = 0.0;


	pM[12] = 0;
	pM[13] = 0;
	pM[14] = 0;
	pM[15] = 1;



}



/*

void CreateMatrixFromQuaternion(float* pM,
								aQUATERNION& pQ)
{
//
//    float xx = pQ.x*pQ.x;
//    float xy = pQ.x*pQ.y;
//    float wx = pQ.w*pQ.x;
//    float yy = pQ.y*pQ.y;
//    float xz = pQ.x*pQ.z;
//    float wy = pQ.w*pQ.y;
//    float zz = pQ.z*pQ.z;
//    float yz = pQ.y*pQ.z;
//    float wz = pQ.w*pQ.z;
//    
//    pM[0]  = 1 - 2 * ( yy + zz ); 
//    pM[1]  =     2 * ( xy - wz );
//    pM[2]  =     2 * ( xz + wy );
//    pM[3]  = 0.0;
//
//    pM[4]  =     2 * ( xy + wz );
//    pM[5]  = 1 - 2 * ( xx + zz );
//    pM[6]  =     2 * ( yz - wx );
//    pM[7]  = 0.0;
//
//    pM[8]  =     2 * ( xz - wy );
//    pM[9]  =     2 * ( yz + wx );
//    pM[10] = 1 - 2 * ( xx + yy );
//    pM[11] = 0.0;
//
//    pM[12] = 0.0;
//    pM[13] = 0.0;
//    pM[14] = 0.0;
//    pM[15] = 1.0;



	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	x2 = pQ.x + pQ.x; 
	y2 = pQ.y + pQ.y; 
	z2 = pQ.z + pQ.z;
	xx = pQ.x * x2;   
	xy = pQ.x * y2;   
	xz = pQ.x * z2;
	yy = pQ.y * y2;   
	yz = pQ.y * z2;   
	zz = pQ.z * z2;
	wx = pQ.w * x2;   
	wy = pQ.w * y2;   
	wz = pQ.w * z2;
//	pM[0]  = 1.0 - (yy + zz);
//	pM[1]  = xy - wz;
//	pM[2]  = xz + wy;
//	pM[3]  = 0.0;
//
//	pM[4]  = xy + wz;
//	pM[5]  = 1.0 - (xx + zz);
//	pM[6]  = yz - wx;
//	pM[7]  = 0.0;
//
//	pM[8]  = xz - wy;
//	pM[9]  = yz + wx;
//	pM[10] = 1.0 - (xx + yy);
//	pM[11] = 0.0;
	pM[0]  = 1.0 - (yy + zz);
	pM[1]  = xy + wz;
	pM[2]  = xz - wy;
	pM[3]  = 0.0;

	pM[4]  = xy - wz;
	pM[5]  = 1.0 - (xx + zz);
	pM[6]  = yz + wx;
	pM[7]  = 0.0;

	pM[8]  = xz + wy;
	pM[9]  = yz - wx;
	pM[10] = 1.0 - (xx + yy);
	pM[11] = 0.0;


	pM[12] = 0;
	pM[13] = 0;
	pM[14] = 0;
	pM[15] = 1;



}

*/



void CreateQuaternionFromMatrixOld(aQUATERNION& Q,
								float* pM)
{
	float s;
	float trace = pM[0] + pM[5] + pM[10] + 1.0;

	float* pQ = (float*)&Q;
	int next[3] = {1, 2, 0};

    if(trace > 0.0)
    {
        s = 0.5 / sqrt(trace);
        pQ[3] = 0.25/s;


//		pQ[0] = (pM[9]-pM[6])*s;
//		pQ[1] = (pM[2]-pM[8])*s;
//		pQ[2] = (pM[4]-pM[1])*s;

        pQ[0] = (pM[6]-pM[9])*s;
        pQ[1] = (pM[8]-pM[2])*s;
        pQ[2] = (pM[1]-pM[4])*s;

    }
	else
	{
/*
		if(pM[0] > pM[5] && pM[0] > pM[10])
		{//X
			s  = sqrt( 1.0 + pM[0] - pM[5] - pM[10] ) * 2;
	        Q.x = 0.5 / s;
    	    Q.y = (pM[1] + pM[4] ) / s;
        	Q.z = (pM[2] + pM[8] ) / s;
	        Q.w = (pM[6] + pM[9] ) / s;
		}
		if(pM[5] > pM[0] && pM[5] > pM[10])
		{//Y
			s  = sqrt( 1.0 + pM[5] - pM[0] - pM[10] ) * 2;
    	    Q.x = (pM[1] + pM[4] ) / s;
	        Q.y = 0.5 / s;
	        Q.z = (pM[6] + pM[9] ) / s;
        	Q.w = (pM[2] + pM[8] ) / s;
		}
		if(pM[15] > pM[0] && pM[15] > pM[10])
		{//Z
			s  = sqrt( 1.0 + pM[10] - pM[0] - pM[5] ) * 2;
        	Q.x = (pM[2] + pM[8] ) / s;
	        Q.y = (pM[6] + pM[9] ) / s;
	        Q.z = 0.5 / s;
    	    Q.w = (pM[1] + pM[4] ) / s;
		}
*/

		if(pM[0] > pM[5] && pM[0] > pM[10])
		{//Xbad
			s  = sqrt( 1.0 + pM[0] - pM[5] - pM[10] ) * 2;
	        Q.x = 0.5 / s;
    	    Q.y = (pM[4] + pM[1] ) / s;
        	Q.z = (pM[8] + pM[2] ) / s;
	        Q.w = (pM[9] + pM[6] ) / s;
		}
		if(pM[5] > pM[0] && pM[5] > pM[10])
		{//Ybad
			s  = sqrt( 1.0 + pM[5] - pM[0] - pM[10] ) * 2;
    	    Q.x = (pM[4] + pM[1] ) / s;
	        Q.y = 0.5 / s;
	        Q.z = (pM[9] + pM[6] ) / s;
        	Q.w = (pM[8] + pM[2] ) / s;
		}
		if(pM[15] > pM[0] && pM[15] > pM[10])
		{//Zbad
			s  = sqrt( 1.0 + pM[10] - pM[0] - pM[5] ) * 2;
        	Q.x = (pM[8] + pM[2] ) / s;
	        Q.y = (pM[9] + pM[6] ) / s;
	        Q.z = 0.5 / s;
    	    Q.w = (pM[4] + pM[1] ) / s;
		}
/*
		i = 0;

		if (pM[5 ] > pM[0]) i = 1;
		if (pM[10] > pM[i]) i = 2;

		j = next[i];
		k = next[j];

		s = sqrt ( (pM[(i*4+i)]- (pM[(j*4+j)]+pM[(k*4+k)])) + 1.0);

		pQ[i] = s * 0.5;

		if (s != 0.0) s = 0.5 / s;

//		q[3] = (m[j][k] - m[k][j]) * s;
//		q[j] = (m[i][j] + m[j][i]) * s;
//		q[k] = (m[i][k] + m[k][i]) * s;


		//Assume over,down
//		pQ[3] = (pM[k*4+j] - pM[j*4+k]) * s;
//		pQ[j] = (pM[j*4+i] + pM[i*4+j]) * s;
//		pQ[k] = (pM[k*4+i] + pM[i*4+k]) * s;

		//Assume down,over
		pQ[3] = (pM[j*4+k] - pM[k*4+j]) * s;
		pQ[j] = (pM[i*4+j] + pM[j*4+i]) * s;
		pQ[k] = (pM[i*4+k] + pM[k*4+i]) * s;
*/
	}
}
/*
void CreateQuaternionFromMatrix(aQUATERNION& Q,
								float* pM)
{
	int i,j,k;
	float s;
	float trace = pM[0] + pM[5] + pM[10];
	float* pQ = (float*)&Q;
	int next[3] = {1, 2, 0};
    if(trace > 0.0)
    {
        float s = (float)sqrt(trace+1.0);
        pQ[3] = s/2.0;
		s = 0.5/s;

//        pQ[3] = s*0.5;
//		s = 0.5/s;
//        pQ.x = (pM[6]-pM[9]) / (2.0*s);
//        pQ.y = (pM[8]-pM[2]) / (2.0*s);
//        pQ.z = (pM[1]-pM[4]) / (2.0*s);

        pQ[0] = (pM[6]-pM[9])*s;
        pQ[1] = (pM[8]-pM[2])*s;
        pQ[2] = (pM[1]-pM[4])*s;

    }
	else
	{
		i = 0;

		if (pM[5 ] > pM[0]) i = 1;
		if (pM[10] > pM[i]) i = 2;

		j = next[i];
		k = next[j];

		s = sqrt ( (pM[(i*4+i)]- (pM[(j*4+j)]+pM[(k*4+k)])) + 1.0);

		pQ[i] = s * 0.5;

		if (s != 0.0) s = 0.5 / s;

//		q[3] = (m[j][k] - m[k][j]) * s;
//		q[j] = (m[i][j] + m[j][i]) * s;
//		q[k] = (m[i][k] + m[k][i]) * s;


		//Assume over,down
//		pQ[3] = (pM[k*4+j] - pM[j*4+k]) * s;
//		pQ[j] = (pM[j*4+i] + pM[i*4+j]) * s;
//		pQ[k] = (pM[k*4+i] + pM[i*4+k]) * s;

		//Assume down,over
		pQ[3] = (pM[j*4+k] - pM[k*4+j]) * s;
		pQ[j] = (pM[i*4+j] + pM[j*4+i]) * s;
		pQ[k] = (pM[i*4+k] + pM[k*4+i]) * s;
	}
}
*/
void QuaternionConjugate(aQUATERNION& DstQ,aQUATERNION& SrcQ)
{
	DstQ.x = -SrcQ.x;
	DstQ.y = -SrcQ.y;
	DstQ.z = -SrcQ.z;
	DstQ.w =  SrcQ.w;
}
void QuaternionMultiply(aQUATERNION& pC,
						aQUATERNION& pA,
						aQUATERNION& pB)
{
//    pC.x = pB.w*pA.x + pB.x*pA.w + pB.y*pA.z + pB.z*pA.y;
//    pC.y = pB.w*pA.y + pB.y*pA.w + pB.z*pA.x + pB.x*pA.z;
//    pC.z = pB.w*pA.z + pB.z*pA.w + pB.x*pA.y + pB.y*pA.x;
//    pC.w = pB.w*pA.w + pB.x*pA.x + pB.y*pA.y + pB.z*pA.z;

    pC.x =  pA.x*pB.w + pA.y*pB.z - pA.z*pB.y + pA.w*pB.x;
    pC.y = -pA.x*pB.z + pA.y*pB.w + pA.z*pB.x + pA.w*pB.y;
    pC.z =  pA.x*pB.y - pA.y*pB.x + pA.z*pB.w + pA.w*pB.z;
    pC.w = -pA.x*pB.x - pA.y*pB.y - pA.z*pB.z + pA.w*pB.w;


}

void QuaternionNormalize(aQUATERNION& pQ)
{
    float square,oosquare;

	square = 	pQ.x * pQ.x + 
				pQ.y * pQ.y + 
				pQ.z * pQ.z	+ 
				pQ.w * pQ.w;
	
	if(square>0.0)
	{
		oosquare = (float)(1.0/sqrt(square));
	}
	else 
	{
		oosquare = 1.0;
	}

    pQ.x *= oosquare;
    pQ.y *= oosquare;
    pQ.z *= oosquare;
    pQ.w *= oosquare;
}

void CreateVectorAngleFromQuaternion(	aVECTOR& pV,
										float& angle,
										aQUATERNION& pQ)
{
    float cosAngle = pQ.w;
    float sinAngle = sqrt(1.0-cosAngle*cosAngle);
	angle = acos(cosAngle) * 2.0;
	pV.x = pQ.x/sinAngle;  
	pV.y = pQ.y/sinAngle;  
	pV.z = pQ.z/sinAngle;  
}
void QuaternionSlerpOld( 	aQUATERNION& pC,
						aQUATERNION pA,//we want local copy to modify
						aQUATERNION pB,
						float t)
{
    float fScale1;
    float fScale2;

    // Compute dot product, aka cos(theta):
    float fCosTheta = pA.x*pB.x + pA.y*pB.y + pA.z*pB.z + pA.w*pB.w;

    if( fCosTheta < 0.0f )
    {
        // Flip start quaternion
        pA.x = -pA.x; 
        pA.y = -pA.y; 
        pA.x = -pA.z; 
        pA.w = -pA.w;
        fCosTheta = -fCosTheta;
    }

    if( fCosTheta + 1.0f > 0.05f )
    {
        // If the quaternions are close, use linear interploation
        if( 1.0f - fCosTheta < 0.05f )
        {
            fScale1 = 1.0f - t;
            fScale2 = t;
        }
        else // Otherwise, do spherical interpolation
        {
            float fTheta    = (float)acos( fCosTheta );
            float fSinTheta = (float)sin( fTheta );
            
            fScale1 = (float)sin( fTheta * (1.0f-t) ) / fSinTheta;
            fScale2 = (float)sin( fTheta * t ) / fSinTheta;
        }
    }
    else
    {
        pB.x = -pA.y;
        pB.y =  pA.x;
        pB.z = -pA.w;
        pB.w =  pA.z;
        fScale1 = (float)sin( Pi * (0.5f - t) );
        fScale2 = (float)sin( Pi * t );
    }

    pC.x = fScale1 * pA.x + fScale2 * pB.x;
    pC.y = fScale1 * pA.y + fScale2 * pB.y;
    pC.z = fScale1 * pA.z + fScale2 * pB.z;
    pC.w = fScale1 * pA.w + fScale2 * pB.w;
}









clock_t StartTime;
float GetSeconds(bool reset)
{
	clock_t CurrentTime;

	if(reset)
	{
		StartTime = clock();
	}

	CurrentTime = clock();

	double cpSeconds = CLOCKS_PER_SEC;
	double Seconds = (double)(CurrentTime - StartTime) / CLOCKS_PER_SEC;

	return((float)Seconds);
}





//MAY CONTAIN BUG> untested
unsigned long GetSecondsOld(bool reset)
{
	clock_t CurrentTime;

	if(reset)
	{
		StartTime = clock();
	}

	CurrentTime = clock();

	unsigned long Seconds = (double)(CurrentTime - StartTime) / CLOCKS_PER_SEC;

	return(Seconds);
}




//Call this to get time since it was last called.
//Call this right when you start an animation to init LastTime;
unsigned long GetElapsedMilliSeconds()
{
	static clock_t LastTime = 0;
	clock_t CurrentTime;

	CurrentTime = clock();

	unsigned long MilliSeconds = (double)(CurrentTime - LastTime);

	LastTime = CurrentTime;
	return(MilliSeconds);
}










const int XX = 0;
const int YY = 1;
const int ZZ = 2;
const int WW = 3;

//void QuaternionToMatrix(Quaternion_t q,Matrix_t m)
void CreateMatrixFromQuaternion(float* pM,
								aQUATERNION& Q)
{

	/*
	If q is guaranteed to be a unit quaternion, s will always
	be 1.  In that case, this calculation can be optimized out.
	*/
	float norm,s,xx,yy,zz,xy,xz,yz,wx,wy,wz;

	norm = 	Q.x*Q.x + 
			Q.y*Q.y + 
			Q.z*Q.z + 
			Q.w*Q.w;

	s = (norm > 0.0) ? (2.0/norm) : (0.0);
	
	/*
	Precalculate coordinate products
	*/
	xx = Q.x * Q.x * s;
	yy = Q.y * Q.y * s;
	zz = Q.z * Q.z * s;
	xy = Q.x * Q.y * s;
	xz = Q.x * Q.z * s;
	yz = Q.y * Q.z * s;
	wx = Q.w * Q.x * s;
	wy = Q.w * Q.y * s;
	wz = Q.w * Q.z * s;

	/*
	Calculate 3x3 matrix from orthonormal basis
	*/
	pM[0]  = 1.0 - (yy + zz);
	pM[1]  = xy + wz;
	pM[2]  = xz - wy;

	pM[4]  = xy - wz;
	pM[5]  = 1.0 - (xx + zz);
	pM[6]  = yz + wx;

	pM[8]  = xz + wy;
	pM[9]  = yz - wx;
	pM[10] = 1.0 - (xx + yy);

/*
    pM[0]  = 1.0 - (yy + zz);
    pM[4]  = xy + wz;
    pM[8]  = xz - wy;

    pM[1]  = xy - wz;
    pM[5]  = 1.0 - (xx + zz);
    pM[9]  = yz + wx;

    pM[2]  = xz + wy;
    pM[6]  = yz - wx;
    pM[10] = 1.0 - (xx + yy);
*/

	pM[3]  = 0.0;
	pM[7]  = 0.0;
	pM[11] = 0.0;

	pM[12] = 0.0;
	pM[13] = 0.0;
	pM[14] = 0.0;
	pM[15] = 1.0;
}








//this works. no angle > pi.
//very intuit
void CreateQuaternionFromMatrix(aQUATERNION& Q,
								float* pM)
{
	int next[3]={1,2,0};
	float Trace;
	float sqrtTrace;
	float oo2sqrtTrace;
/*
	Trace = pM[0]+pM[5]+pM[10]+pM[15];
	sqrtTrace = 0.5*sqrt(Trace);
	float s=0.25/sqrtTrace;
	Q.x = (pM[6]-pM[9])*s;
	Q.y = (pM[8]-pM[2])*s;
	Q.z = (pM[1]-pM[4])*s;
	Q.w = sqrtTrace;
*/
	//MATTOAXIS

	Trace = pM[0]+pM[5]+pM[10]+pM[15];
	sqrtTrace = sqrt(Trace);

	if(1)//sqrtTrace<2.0)
	{
		oo2sqrtTrace = 1.0/(2.0*sqrtTrace);
		Q.x = (pM[6]-pM[9])*oo2sqrtTrace;
		Q.y = (pM[8]-pM[2])*oo2sqrtTrace;
		Q.z = (pM[1]-pM[4])*oo2sqrtTrace;
		Q.w = sqrtTrace/2.0;
	}
	else
	{
		Q.x = 0.0;
		Q.y = 0.0;
		Q.z = 0.0;
		Q.w = 1.0;
	}

}

void CreateQuaternionFromMatrixGOOD2(aQUATERNION& Q,
								float* pM)
{
	int next[3]={1,2,0};
	float Trace;
	float sqrtTrace;
	float oo2sqrtTrace;

	Trace = pM[0]+pM[5]+pM[10]+pM[15];
	if(Trace >= 1.0)
	{
		sqrtTrace = sqrt(Trace);
		oo2sqrtTrace = 1.0/(2.0*sqrtTrace);
 		Q.x = (pM[6]-pM[9])*oo2sqrtTrace;
		Q.y = (pM[8]-pM[2])*oo2sqrtTrace;
		Q.z = (pM[1]-pM[4])*oo2sqrtTrace;
		Q.w = sqrtTrace/2.0;
	}
	else
	{
		if(pM[0]>pM[5] && pM[0]>pM[10])
		{
			sqrtTrace = sqrt(pM[0]-pM[5]-pM[10]+1.0);
			oo2sqrtTrace = 1.0/(2.0*sqrtTrace);
			Q.x  = sqrtTrace/2.0;
			Q.y  = (pM[4]+pM[1])*oo2sqrtTrace;
			Q.z  = (pM[8]+pM[2])*oo2sqrtTrace;
			Q.w  = (pM[6]-pM[9])*oo2sqrtTrace;
		}
		else if(pM[5]>pM[0] && pM[5]>pM[10])
		{
			sqrtTrace = sqrt(pM[5]-pM[10]-pM[0]+1.0);
			oo2sqrtTrace = 1.0/(2.0*sqrtTrace);
			Q.x  = ( pM[1]+pM[4] )*oo2sqrtTrace;
			Q.y  = sqrtTrace/2.0;
			Q.z  = ( pM[9]+pM[6] )*oo2sqrtTrace;
			Q.w  = ( pM[8]-pM[2] )*oo2sqrtTrace;
		}
		else
		{
			sqrtTrace = sqrt(pM[10]-pM[0]-pM[5]+1.0);
			oo2sqrtTrace = 1.0/(2.0*sqrtTrace);
			Q.x  = ( pM[2]+pM[8] )*oo2sqrtTrace;
			Q.y  = ( pM[6]+pM[9] )*oo2sqrtTrace;
			Q.z  = sqrtTrace/2.0;
			Q.w  = ( pM[1]-pM[4] )*oo2sqrtTrace;
		}
	}
}


void CreateQuaternionFromAxisAngle(aQUATERNION& Q,aVECTOR Axis,float angle)
{
	float sin_a = sin( angle / 2 );
    float cos_a = cos( angle / 2 );
	float oosin_a = 1.0/sin_a;

    Q.x    = Axis.x * oosin_a;
    Q.y    = Axis.y * oosin_a;
    Q.z    = Axis.z * oosin_a;
    Q.w    = cos_a;
}

void CreateQuaternionFromMatrixGOOD(aQUATERNION& Q,
								float* pM)
{
	int i,j,k;
	int next[3]={1,2,0};
	float Trace;
	float sqrtTrace;
	float oo2sqrtTrace;

	Trace = pM[0]+pM[5]+pM[10]+pM[15];
	if(Trace >= 1.0)
	{
		sqrtTrace = sqrt(Trace);
		oo2sqrtTrace = 1.0/(2.0*sqrtTrace);
 		Q.x = (pM[6]-pM[9])*oo2sqrtTrace;
		Q.y = (pM[8]-pM[2])*oo2sqrtTrace;
		Q.z = (pM[1]-pM[4])*oo2sqrtTrace;
		Q.w = sqrtTrace/2.0;
	}
	else
	{
		if(pM[0] > pM[5])
		{
			i = 0;//x
		}
		else
		{
			i = 1;//y
		}

		if (pM[10] > pM[(i*4+i)])
		{
			i = 2;//z
		}

		j = next[i];
		k = next[j];

		sqrtTrace = sqrt(pM[(i*4+i)]-pM[(j*4+j)]-pM[(k*4+k)]+1.0);
		oo2sqrtTrace = 1.0/(2.0*sqrtTrace);

		float* pQ = (float*)&Q.x;
		pQ[i]  = sqrtTrace/2.0;
		pQ[j]  = ( pM[(j*4+i)]+pM[(i*4+j)] )*oo2sqrtTrace;
		pQ[k]  = ( pM[(k*4+i)]+pM[(i*4+k)] )*oo2sqrtTrace;
		pQ[WW] = ( pM[(j*4+k)]-pM[(k*4+j)] )*oo2sqrtTrace;
	}
}

//warning : this function has an accuracy problem.  ;(
//this project is now using quaternions.
//I believe the calculation for axis of rotation is the source of error
void MatrixSlerp(	float* pC,//destination
					float* pA,//matrix "from"
					float* pB,//matrix "to"
					float t)
{
	aVECTOR axis;
	float pInverse[16];
	float pTransition[16];
	float pRelative[16];
	float angle;

	InverseMatrix(pInverse,pA);

	//rotation only
	pInverse[12] =  0.0;
	pInverse[13] =  0.0;
	pInverse[14] =  0.0;
	pInverse[15] =  1.0;

	// T = (A^-1)*B
	MatrixMul(pTransition,pInverse,pB);
	CreateAxisAngleFromMatrix(axis,angle,pTransition);


	//create rotation matrix with scaled angle
	if(angle==0.0)
	{//no pA, and pB are the same
		memcpy(pC,pB,16*sizeof(float));
	}
	else
	{
		AxisRotationMatrix(	pRelative,
							axis.x,
							axis.y,
							axis.z,
							(t*angle));
		MatrixMul(pC,pRelative,pA);
	}
}

void DoubleMatrixSlerp(	float* pC,//destination
						float* pA,//matrix "from"
						float* pB,//matrix "to"
						float t)
{
//	aVECTOR axis;
	float pInverse[16];
	double pRelative[16];
	double pAxis[3];//vector
	double pTransition[16];
	double angle;




	OrthonormalizeOrientation(pA);
	OrthonormalizeOrientation(pB);


	InverseMatrix(pInverse,pA);

	//rotation only
	pInverse[12] =  0.0;
	pInverse[13] =  0.0;
	pInverse[14] =  0.0;
	pInverse[15] =  1.0;

	// T = (A^-1)*B
	DoubleMatrixMul(pTransition,pInverse,pB);
	DoubleCreateAxisAngleFromMatrix(pAxis,angle,pTransition);


	//create rotation matrix with scaled angle
	if(angle==0.0)
	{//no pA, and pB are the same
		memcpy(pC,pA,16*sizeof(float));
	}
	else
	{
		DoubleAxisRotationMatrix(
							pRelative,
							pAxis[0],
							pAxis[1],
							pAxis[2],
							((double)t*angle));

		pC[0]  =  (pRelative[0]  * (double)pA[0])  +  (pRelative[1]  * (double)pA[4])  +  (pRelative[2]  * (double)pA[8] );
		pC[1]  =  (pRelative[0]  * (double)pA[1])  +  (pRelative[1]  * (double)pA[5])  +  (pRelative[2]  * (double)pA[9] );
		pC[2]  =  (pRelative[0]  * (double)pA[2])  +  (pRelative[1]  * (double)pA[6])  +  (pRelative[2]  * (double)pA[10]);
		pC[3]  =  0.0;
		pC[4]  =  (pRelative[4]  * (double)pA[0])  +  (pRelative[5]  * (double)pA[4])  +  (pRelative[6]  * (double)pA[8] );
		pC[5]  =  (pRelative[4]  * (double)pA[1])  +  (pRelative[5]  * (double)pA[5])  +  (pRelative[6]  * (double)pA[9] );
		pC[6]  =  (pRelative[4]  * (double)pA[2])  +  (pRelative[5]  * (double)pA[6])  +  (pRelative[6]  * (double)pA[10]);
		pC[7]  =  0.0;
		pC[8]  =  (pRelative[8]  * (double)pA[0])  +  (pRelative[9]  * (double)pA[4])  +  (pRelative[10] * (double)pA[8] );
		pC[9]  =  (pRelative[8]  * (double)pA[1])  +  (pRelative[9]  * (double)pA[5])  +  (pRelative[10] * (double)pA[9] );
		pC[10] =  (pRelative[8]  * (double)pA[2])  +  (pRelative[9]  * (double)pA[6])  +  (pRelative[10] * (double)pA[10]);
		pC[11] =  0.0;
		pC[12] =  (pRelative[12] * (double)pA[0])  +  (pRelative[13] * (double)pA[4])  +  (pRelative[14] * (double)pA[8] )  + (double)pA[12];
		pC[13] =  (pRelative[12] * (double)pA[1])  +  (pRelative[13] * (double)pA[5])  +  (pRelative[14] * (double)pA[9] )  + (double)pA[13];
		pC[14] =  (pRelative[12] * (double)pA[2])  +  (pRelative[13] * (double)pA[6])  +  (pRelative[14] * (double)pA[10])  + (double)pA[14];
		pC[15] =  1.0;

	}
	OrthonormalizeOrientation(pC);
}
/*
void AxisRotationMatrix(float *pMat,
						float Ax,
						float Ay,
						float Az,
						float Angle)

	float Length;
	float fCos = (float)cos(Angle);
	float fSin = (float)sin(Angle);
	if(pMat == NULL)return;
	MacroNormalize(Length,Ax,Ay,Az);

	pMat[0]  = (Ax*Ax)*(1.0f-fCos)+fCos;
	pMat[1]  = (Ay*Ax)*(1.0f-fCos)+(Az*fSin);
	pMat[2]  = (Az*Ax)*(1.0f-fCos)-(Ay*fSin);
	pMat[3]  = 0.0;

	pMat[4]  = (Ax*Ay)*(1.0f-fCos)-(Az*fSin);
	pMat[5]  = (Ay*Ay)*(1.0f-fCos)+fCos ;
	pMat[6]  = (Az*Ay)*(1.0f-fCos)+(Ax*fSin);
	pMat[7]  = 0.0;

	pMat[8]  = (Ax*Az)*(1.0f-fCos)+(Ay*fSin);
	pMat[9]  = (Ay*Az)*(1.0f-fCos)-(Ax*fSin);
	pMat[10] = (Az*Az)*(1.0f-fCos)+fCos;
	pMat[11] = 0.0;

	pMat[12] = 0.0;
	pMat[13] = 0.0;
	pMat[14] = 0.0;
	pMat[15] = 1.0;


*/

void CreateAxisAngleFromMatrix(	aVECTOR& Axis,
								float& Angle,
								float* pM)
{
	float Length;
	float Trace;
	float sqrtTrace;
/*
	int i,j,k;
	int next[3]={1,2,0};
	float oo2sqrtTrace;
	Trace = pM[0]+pM[5]+pM[10]+pM[15];
	sqrtTrace = 0.5*sqrt(Trace);
	float s=0.25/sqrtTrace;
	Axis.x = (pM[6]-pM[9])*s;
	Axis.y = (pM[8]-pM[2])*s;
	Axis.z = (pM[1]-pM[4])*s;
	Angle = acos(sqrtTrace) * 2.0;
*/
/*
	Trace = pM[0]+pM[5]+pM[10]+pM[15];
	sqrtTrace = sqrt(Trace);
	oo2sqrtTrace = 1.0/(2.0*sqrtTrace);
	Axis.x = (pM[6]-pM[9])*oo2sqrtTrace;
	Axis.y = (pM[8]-pM[2])*oo2sqrtTrace;
	Axis.z = (pM[1]-pM[4])*oo2sqrtTrace;
	Angle = acos(sqrtTrace/2.0) * 2.0;
*/

	Trace = pM[0]+pM[5]+pM[10]+pM[15];
	sqrtTrace = sqrt(Trace);
	Angle = acos(sqrtTrace/2.0) * 2.0;

	if(Angle>0.0)
	{
		Axis.x = pM[6]-pM[9];
		Axis.y = pM[8]-pM[2];
		Axis.z = pM[1]-pM[4];
		MacroNormalize(Length,Axis.x,Axis.y,Axis.z);
	}
	else//cant normalize zero length vector
	{
		Axis.x = 0.0;
		Axis.y = 0.0;
		Axis.z = 0.0;
		Angle  = 0.0;
	}
}






void DoubleCreateAxisAngleFromMatrix(	double* pAxis,
										double& Angle,
										double* pM)
{
	double ooLength;
	double Trace;
	double Ax,Ay,Az;

//	Trace = pM[0]+pM[5]+pM[10]+pM[15];
//	sqrtTrace = sqrt(Trace);
//	Angle = acos(sqrtTrace/2.0) * 2.0;

	Trace = (pM[0]+pM[5]+pM[10]-1)/2.0;
	Angle = acos(Trace);


	if(Angle>0.0)
	{
		Ax   = pM[6]-pM[9];
		Ay   = pM[8]-pM[2];
		Az   = pM[1]-pM[4];

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		ooLength = ((sqrt(	Ax*Ax+
								Ay*Ay+
								Az*Az)));
		Angle = asin(2.0*ooLength);
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

		ooLength = (1.0/(sqrt(	Ax*Ax+
								Ay*Ay+
								Az*Az)));
		Ax  *= ooLength;
		Ay  *= ooLength;
		Az  *= ooLength;

		pAxis[0] = Ax;
		pAxis[1] = Ay;
		pAxis[2] = Az;
	}
	else//cant normalize zero length vector
	{
		pAxis[0] = 0.0;
		pAxis[1] = 0.0;
		pAxis[2] = 0.0;
		Angle    = 0.0;
	}
}




void CreateAxisAngleFromMatrixGOOD(	aVECTOR& Axis,
								float& Angle,
								float* pM)
{
	/*
	This code can be optimized for m[kW][kW] = 1, which 
	should always be true.  This optimization is excluded
	here for clarity.
	*/
	
    float Length;
    float cosAngle;
	float Tr,fourD;
	int i,j,k;
	

	Tr = pM[0] + pM[5] + pM[10] + pM[15];
	/*
	w >= 0.5 ?
	*/
	if(Tr >= 1.0)
	{
		fourD = 2.0*sqrt(Tr);
		Axis.x = (pM[ (YY*4+ZZ) ] - pM[ (ZZ*4+YY) ]);
		Axis.y = (pM[ (ZZ*4+XX) ] - pM[ (XX*4+ZZ) ]);
		Axis.z = (pM[ (XX*4+YY) ] - pM[ (YY*4+XX) ]);

//		Axis.x = (pM[ (ZZ*4+YY) ] - pM[ (YY*4+ZZ) ])/fourD;
//		Axis.y = (pM[ (XX*4+ZZ) ] - pM[ (ZZ*4+XX) ])/fourD;
//		Axis.z = (pM[ (YY*4+XX) ] - pM[ (XX*4+YY) ])/fourD;
		MacroNormalize(Length,Axis.x,Axis.y,Axis.z);
	    cosAngle = fourD/4.0;
		Angle    = acos(cosAngle) * 2.0;
	}
	else
	{
		/*
		Find the largest component.  
		*/
		if ( 	pM[ (XX*4+XX) ] > 
				pM[ (YY*4+YY) ])
		{
			i = XX;
		}
		else
		{
			i = YY;
		}

		if ( 	pM[ (ZZ*4+ZZ) ] > 
				pM[ (i *4+ i) ])
		{
			i = ZZ;
		}

		
		/*
		Set j and k to point to the next two components
		*/
		j = (i+1)%3;
		k = (j+1)%3;

		/*
		fourD = 4 * largest component
		*/
		fourD = 2.0*sqrt(pM[ (i*4+i) ] - pM[ (j*4+j) ] - pM[ (k*4+k) ] + 1.0 );

		/*
		Set the largest component
		*/
		float* pAxis = (float*)&Axis.x;

		pAxis[i] = fourD/4.0;
		pAxis[j] = (pM[ (j*4+i) ] + pM[ (i*4+j) ])/fourD;
		pAxis[k] = (pM[ (k*4+i) ] + pM[ (i*4+k) ])/fourD;
	    cosAngle = (pM[ (k*4+j) ] - pM[ (j*4+k) ])/fourD;
		Angle    = acos(cosAngle) * 2.0;
		MacroNormalize(Length,Axis.x,Axis.y,Axis.z);
	}
}


void QuaternionSlerp( 	aQUATERNION& C,
						aQUATERNION  A,//we want local copy to modify
						aQUATERNION  B,
						float t)
{
	float fAlpha,fTheta,fBeta,fCosTheta,oosinTheta;

	fCosTheta = A.x*B.x + 
				A.y*B.y + 
				A.z*B.z + 
				A.w*B.w;

	if(fCosTheta < 0.0f)
	{// Flip one quaternion
		B.x = -B.x; 
		B.y = -B.y; 
		B.z = -B.z; 
		B.w = -B.w;
		fCosTheta = -fCosTheta;
	}

	fAlpha = t;
	fBeta  = 1.0f - fAlpha;

	if(1.0f - fCosTheta > 0.001f) 
	{//use spherical interpolation
		fTheta     = acos(fCosTheta);
		oosinTheta = 1.0/sin(fTheta);
		fBeta      = sin(fTheta*fBeta )*oosinTheta;
		fAlpha     = sin(fTheta*fAlpha)*oosinTheta;
	}

	// Do the interpolation
	C.x = fBeta*A.x + fAlpha*B.x;
	C.y = fBeta*A.y + fAlpha*B.y;
	C.z = fBeta*A.z + fAlpha*B.z;
	C.w = fBeta*A.w + fAlpha*B.w;
}
/*last one

void QuaternionSlerp( 	aQUATERNION& C,
						aQUATERNION  A,//we want local copy to modify
						aQUATERNION  B,
						float t)
{
	float sina,sinat,sinaomt,angle,fCosTheta;

    fCosTheta = A.x*B.x + 
    			A.y*B.y + 
    			A.z*B.z + 
    			A.w*B.w;

    if( fCosTheta < 0.0f )
    {
        // Flip start quaternion
        A.x = -A.x; 
        A.y = -A.y; 
        A.x = -A.z; 
        A.w = -A.w;
        fCosTheta = -fCosTheta;
    }

	angle   = acos(fCosTheta);
	sina    = (sin(angle));
	sinat   = (sin(angle*t));
	sinaomt = (sin(angle*(1-t)));

    C.x = (sinaomt * A.x + sinat * B.x)/sina;
    C.y = (sinaomt * A.y + sinat * B.y)/sina;
    C.z = (sinaomt * A.z + sinat * B.z)/sina;
    C.w = (sinaomt * A.w + sinat * B.w)/sina;
}
*/
//no invert axis
void QuaternionSlerp2( 	aQUATERNION& C,
						aQUATERNION  A,//we want local copy to modify
						aQUATERNION  B,
						float t)
{
	float sina,sinat,sinaomt,angle,fCosTheta;

    fCosTheta = A.x*B.x + 
    			A.y*B.y + 
    			A.z*B.z + 
    			A.w*B.w;

	angle   = acos(fCosTheta);
	sina    = (sin(angle));
	sinat   = (sin(angle*t));
	sinaomt = (sin(angle*(1-t)));

    C.x = (sinaomt * A.x + sinat * B.x)/sina;
    C.y = (sinaomt * A.y + sinat * B.y)/sina;
    C.z = (sinaomt * A.z + sinat * B.z)/sina;
    C.w = (sinaomt * A.w + sinat * B.w)/sina;
}



void OrthonormalizeOrientation(float* pMat)
{
	float Length;
    aVECTOR X;
    aVECTOR Y;
    aVECTOR Z;

    X.x = pMat[0];
    X.y = pMat[4];
    X.z = pMat[8];

    Y.x = pMat[1];
    Y.y = pMat[5];
    Y.z = pMat[9];

	MacroNormalize(Length,X.x,X.y,X.z);	

	MacroCross(	Z.x,Z.y,Z.z,
				X.x,X.y,X.z,
				Y.x,Y.y,Y.z);

	MacroCross(	Y.x,Y.y,Y.z,
				Z.x,Z.y,Z.z,
				X.x,X.y,X.z);
//////////////not needed unless scaled matrix
	MacroNormalize(Length,Y.x,Y.y,Y.z);	
	MacroNormalize(Length,Z.x,Z.y,Z.z);	
//////////////not needed unless scaled matrix

    pMat[0] = X.x; pMat[1] = Y.x; pMat[2] = Z.x;
    pMat[4] = X.y; pMat[5] = Y.y; pMat[6] = Z.y;
    pMat[8] = X.z; pMat[9] = Y.z; pMat[10] = Z.z;
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
	{
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
	if(strcmp(pExt,"obj")==0) 
	{
		return FILE_TYPE_OBJ;
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









//UNTESTED!!!!!!!!!!!!!!
//UNTESTED!!!!!!!!!!!!!!
//UNTESTED!!!!!!!!!!!!!!
//UNTESTED!!!!!!!!!!!!!!
void VectorMulRotationMatrix(	aVECTOR* pDstV,
								aVECTOR* pSrcV,
								float* pMat)
{
	pDstV->x  = pSrcV->x*pMat[0] + 
				pSrcV->y*pMat[4] + 
				pSrcV->z*pMat[8];
	pDstV->y  = pSrcV->x*pMat[1] + 
				pSrcV->y*pMat[5] + 
				pSrcV->z*pMat[9];
	pDstV->z  = pSrcV->x*pMat[2] + 
				pSrcV->y*pMat[6] + 
				pSrcV->z*pMat[10];
}

//UNTESTED!!!!!!!!!!!!!!
//UNTESTED!!!!!!!!!!!!!!
//UNTESTED!!!!!!!!!!!!!!
//UNTESTED!!!!!!!!!!!!!!
void VectorMulFullMatrix(aVECTOR* pDstV,
						 aVECTOR* pSrcV,
						 float* pMat)
{
	pDstV->x  = pSrcV->x*pMat[0] + 
				pSrcV->y*pMat[4] + 
				pSrcV->z*pMat[8] +
				pMat[12];
	pDstV->y  = pSrcV->x*pMat[1] + 
				pSrcV->y*pMat[5] + 
				pSrcV->z*pMat[9] +
				pMat[13];
	pDstV->z  = pSrcV->x*pMat[2] + 
				pSrcV->y*pMat[6] + 
				pSrcV->z*pMat[10]+
				pMat[14];
}


void VertexMatrixMul(	aVECTOR* pDstV,
						VERTEX_XYZNRGBUV* pSrcV,
						float* pMat)
{
	pDstV->x  = pSrcV->x*pMat[0] + 
				pSrcV->y*pMat[4] + 
				pSrcV->z*pMat[8] +
				pMat[12];
	pDstV->y  = pSrcV->x*pMat[1] + 
				pSrcV->y*pMat[5] + 
				pSrcV->z*pMat[9] +
				pMat[13];
	pDstV->z  = pSrcV->x*pMat[2] + 
				pSrcV->y*pMat[6] + 
				pSrcV->z*pMat[10]+
				pMat[14];
}
//VertexNormal Rotate
void NormalMatrixMul(	aVECTOR* pDstV,
						VERTEX_XYZNRGBUV* pSrcV,
						float* pMat)
{
	pDstV->x  = pSrcV->nx*pMat[0] + 
				pSrcV->ny*pMat[4] + 
				pSrcV->nz*pMat[8];
	pDstV->y  = pSrcV->nx*pMat[1] + 
				pSrcV->ny*pMat[5] + 
				pSrcV->nz*pMat[9];
	pDstV->z  =	pSrcV->nx*pMat[2] + 
				pSrcV->ny*pMat[6] + 
				pSrcV->nz*pMat[10];
}






// P		-
//	\		|
//	 \		|d
//	  \		|
// +-<-V	-
float Distance_PointToVector(	aVECTOR RayOrigin,
								aVECTOR	RayDir,
								aVECTOR Point)
{
	float Length;
	aVECTOR ToPoint;
	aVECTOR ProjectedPoint;
	aVECTOR P1toP2;
	
	ToPoint.x = Point.x-RayOrigin.x;
	ToPoint.y = Point.y-RayOrigin.y;
	ToPoint.z = Point.z-RayOrigin.z;

	MacroDot(Length,ToPoint.x,
					ToPoint.y,
					ToPoint.z,
					RayDir.x,
					RayDir.y,
					RayDir.z);

	ProjectedPoint.x = RayOrigin.x+RayDir.x*Length;
	ProjectedPoint.y = RayOrigin.y+RayDir.y*Length;
	ProjectedPoint.z = RayOrigin.z+RayDir.z*Length;
	
	P1toP2.x = ProjectedPoint.x-Point.x;
	P1toP2.y = ProjectedPoint.y-Point.y;
	P1toP2.z = ProjectedPoint.z-Point.z;

	MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);

	return Length;
}







float Distance_PointToBone(	aVECTOR RayOrigin,
							aVECTOR	RayDir,
							float RayLength,
							aVECTOR Point)
{
	float Length;
	aVECTOR ToPoint;
	aVECTOR ProjectedPoint;
	aVECTOR P1toP2;
	
	ToPoint.x = Point.x-RayOrigin.x;
	ToPoint.y = Point.y-RayOrigin.y;
	ToPoint.z = Point.z-RayOrigin.z;

	MacroDot(Length,ToPoint.x,
					ToPoint.y,
					ToPoint.z,
					RayDir.x,
					RayDir.y,
					RayDir.z);

	if(Length<=0)
	{
		P1toP2.x = RayOrigin.x-Point.x;
		P1toP2.y = RayOrigin.y-Point.y;
		P1toP2.z = RayOrigin.z-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
	}
	else if(Length>0 && Length<RayLength)
	{
		ProjectedPoint.x = RayOrigin.x+RayDir.x*Length;
		ProjectedPoint.y = RayOrigin.y+RayDir.y*Length;
		ProjectedPoint.z = RayOrigin.z+RayDir.z*Length;
	
		P1toP2.x = ProjectedPoint.x-Point.x;
		P1toP2.y = ProjectedPoint.y-Point.y;
		P1toP2.z = ProjectedPoint.z-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
	}
	else//Length > RayLength
	{
		P1toP2.x = (RayOrigin.x+RayDir.x*RayLength)-Point.x;
		P1toP2.y = (RayOrigin.y+RayDir.y*RayLength)-Point.y;
		P1toP2.z = (RayOrigin.z+RayDir.z*RayLength)-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
	}
	return Length;
}
