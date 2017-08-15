//AppTools.cpp written by david paull.
//anything that creates Objects, or Meshes, that might be usable in other applications
//this includes fonts, mouse arrow icons, pick sphere, and pick plane
// Copyright (c) 2002 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to
#include "stdafx.h"
#include <math.h>//sqrt
#include "Library.h"
#include "FileAccess.h"

/////////////////////////////////////////
//////// Extern Global Variables ////////
/////////////////////////////////////////
extern HWND  Render_hWnd;
extern float pRender_ProjMatrix[];
extern float pRender_ViewMatrix[];
extern float IconScale;
extern float App_PickSphereRadius;
extern float Render_WindowWidth;
extern float Render_WindowHeight;
extern aDOUBLE_VECTOR CurrentMouseSpherePickVector;
extern aDOUBLE_VECTOR PreviousMouseSpherePickVector;
extern bool bCtrlKeyDown;
extern struct SceneLL* pRender_SceneHead;

//////////////////////////////////////////
//////// 'local' Global Variables ////////
//////////////////////////////////////////

FontObject 			AppTools_Fonts[2];

struct MeshLL*  	pAppTools_MousePointer 	= NULL;//mouse (arrow, cursor, pointer) <--
struct MeshLL*  	pAppTools_VertexHandles = NULL;//particles showing vertices
struct SceneLL* 	pAppTools_Scene  		= NULL;//pointer to scene that holds mouse pointer, and visible vectors. system geometry.
struct ObjectLL*	pAppTools_Object 		= NULL;//
struct MeshLL*  	pAppTools_PickSphere 	= NULL;
struct MeshLL*  	pAppTools_PickPlane  	= NULL;
struct MeshLL*  	pAppTools_XYZ 			= NULL;//3d geometry version
struct MeshLL*  	pAppTools_XYZSphere 	= NULL;
struct MeshLL*  	pAppTools_UVN 			= NULL;
struct MeshLL*  	pAppTools_Vector   	= NULL;//used to draw bones, or any vector like joint offset
struct MeshLL*		pAppTools_VectorX  	= NULL;//2d quad face camera method
struct MeshLL*		pAppTools_VectorY  	= NULL;
struct MeshLL*		pAppTools_VectorZ  	= NULL;


void CreateFontTexture(FontObject* pFont)
{
	fRECT Rect;
	long cPage,x,y,i,j,s,t,SourceX,SourceY;
	long xMin,xMax,yMin,yMax;
	long SourceIndex,DestIndex;
	long StartX,StartY,StopX,StopY;	
	long CharWidth = 32;
	long NumTexturePagesPerFont	= 2;	// 128chars / (256/32)*(256/32)
	long MaxCharsPerPageStrip	= 128;	// Max Number Of Chars To Buffer Per Texture Page. DrawPrimitive.
	long cNumCellsAcross = 256/CharWidth;
	long Diffuse = 0xffffffff;
	long Specular = 0x00000000;
	float rhw = 1.0;
	HFONT HFont;
	BITMAPINFO BMI;
	HBITMAP SmallBitmap;//CharWidth 
	VERTEX_XYZNRGBUV* pV = NULL;
	WORD* pI = NULL;
	unsigned char* pSmallBits = NULL;
	unsigned char* pTextTexture = NULL;
	unsigned char* pSrc = NULL;
	unsigned char* pDst = NULL;
	char Ascii;
	char AsciiSP;
	BITMAP bmSmall;


	//Diffuse = PACKRGBA(97,89,166,0);
	
	CreateTexture("font0",pFont->pText1->Texture,256,256,TEXTURE_FLAG_4444);
	CreateTexture("font1",pFont->pText2->Texture,256,256,TEXTURE_FLAG_4444);
	//load icons
	LoadTexture("icons.bmp",NULL,pFont->pIcons->Texture,TEXTURE_FLAG_4444);

    HDC hdc = CreateCompatibleDC(NULL);
	HFont = CreateFont((39),0, 0, 0,  FW_NORMAL , FALSE, FALSE, FALSE,
			        			ANSI_CHARSET,
			        			OUT_DEFAULT_PRECIS,
			        			CLIP_DEFAULT_PRECIS,
				                ANTIALIASED_QUALITY,
//				                DEFAULT_QUALITY,
							    DEFAULT_PITCH,
//    						    "Courier New");
//    						    "Century Gothic");
								pFont->pName);
//    						    "Times New Roman");


	BMI.bmiHeader.biSize           	= sizeof(BITMAPINFOHEADER); 
	BMI.bmiHeader.biWidth           = CharWidth; 
	BMI.bmiHeader.biHeight			= -CharWidth; 
	BMI.bmiHeader.biPlanes 			= 1;
	BMI.bmiHeader.biBitCount 		= 24;
	BMI.bmiHeader.biCompression     = BI_RGB; 
	BMI.bmiHeader.biSizeImage		= 0; 
	BMI.bmiHeader.biXPelsPerMeter  	= 0;
	BMI.bmiHeader.biYPelsPerMeter	= 0;
	BMI.bmiHeader.biClrUsed 		= 0;
	BMI.bmiHeader.biClrImportant	= 0;
	BMI.bmiColors[0].rgbBlue     	= 0; 
	BMI.bmiColors[0].rgbGreen    	= 0; 
	BMI.bmiColors[0].rgbRed      	= 0; 
	BMI.bmiColors[0].rgbReserved 	= 0;

	SmallBitmap = CreateDIBSection(hdc,&BMI,DIB_RGB_COLORS,(void **)&pSmallBits,NULL, NULL); 

	SelectObject(hdc,SmallBitmap);
	SelectObject(hdc,HFont);
	SetBkMode(hdc,TRANSPARENT);
	SetTextAlign(hdc,TA_CENTER); 
	SetTextColor(hdc,RGB(255,255,255));

	GetObject(SmallBitmap,sizeof(BITMAP),&bmSmall);

	Ascii = 0;
	for(cPage=0;cPage<NumTexturePagesPerFont;cPage++)
	{
		if(cPage==0)pTextTexture = pFont->pText1->Texture.pChar;
		if(cPage==1)pTextTexture = pFont->pText2->Texture.pChar;

		for(t=0;t<cNumCellsAcross;t++)
		{
			for(s=0;s<cNumCellsAcross;s++)
			{
//				if(Ascii >= ASCII_MIN && Ascii < ASCII_MAX)
				if(Ascii >= 32 && Ascii < ASCII_MAX)
				{
					xMin =  100000;
					xMax = -100000;
					yMin =  100000;
					yMax = -100000;
	
					for(j=0;j<abs(bmSmall.bmHeight);j++)
					{
						memset(&pSmallBits[j*bmSmall.bmWidthBytes],0,bmSmall.bmWidth*3);
					}

					if(Ascii==ASCII_SP)
					{//
						AsciiSP = (char)SPACE_CHAR;
						TextOut(hdc,(CharWidth/2),-7,&AsciiSP,sizeof(char));//*2 in middle of surf*4
					}
					else
					{//normal char
						TextOut(hdc,(CharWidth/2),-7,&Ascii,sizeof(char));//*2 in middle of surf*4
						//AB-TextOut(hdc,(CharWidth/2),-13,&Ascii,sizeof(char));//*2 in middle of surf*4
					}



	//DEBUG
	SaveBMP("Small",(unsigned char*)pSmallBits,CharWidth,CharWidth);
					/////////////////////////////////////////////
					//// Copy To Correct Slot In Texture Map ////
					/////////////////////////////////////////////



					/////////////////////////////
					//// Access Current Cell ////
					/////////////////////////////

					StartX 	= s*CharWidth;
					StartY	= t*CharWidth;
					StopX 	= StartX+CharWidth;
					StopY	= StartY+CharWidth;

					////////////////////////////////////
					//// Step In Destination Pixels ////
					////////////////////////////////////

					for(y=StartY;y<StopY;y++)
					{
						for(x=StartX;x<StopX;x++)
						{
							SourceX   = (x-StartX);
							SourceY   = (y-StartY);
							SourceIndex = SourceY*(CharWidth*3)+(SourceX*3);
							DestIndex   = y*(pFont->pText2->Texture.Width*3)+(x*3);

							pTextTexture[DestIndex+0] = pSmallBits[SourceIndex+0];
							pTextTexture[DestIndex+1] = pSmallBits[SourceIndex+1];
							pTextTexture[DestIndex+2] = pSmallBits[SourceIndex+2];

							if(pSmallBits[SourceIndex+1]>0)
							{
								if(x<xMin)xMin = x;
								if(x>xMax)xMax = x;
								if(y<yMin)yMin = y;
								if(y>yMax)yMax = y;
							}
						}//End for(x=StartX;x<StopX;x++)
					}//End for(y=StartY;y<StopY;y++)

					////////////////////////////////////
					//// Create Unique Minimum Rect ////
					////////////////////////////////////

					Rect.left 	= (float)(xMin-1);	
					Rect.right 	= (float)(xMax+2);
					Rect.top 	= (float)(yMin-1);
					Rect.bottom = (float)(yMax+2);

					Rect.left   = Clamp(Rect.left  ,StartX,StopX);
					Rect.right  = Clamp(Rect.right ,StartX,StopX);
					Rect.top    = Clamp(Rect.top   ,StartY,StopY);
					Rect.bottom = Clamp(Rect.bottom,StartY,StopY);
					
					////////////////////////////////
					//// Save Unique World Rect ////
					////  TEXTURE SPACE			////
					////////////////////////////////


					//+x is to left +y is up
					pFont->pCharRectXY[Ascii].left 	    = IconScale*(Rect.left   - ((float)StartX+(float)CharWidth*0.5f));
					pFont->pCharRectXY[Ascii].right 	= IconScale*(Rect.right  - ((float)StartX+(float)CharWidth*0.5f));
					pFont->pCharRectXY[Ascii].top 		= IconScale*(Rect.top    - ((float)StartY+(float)CharWidth*0.5f));
					pFont->pCharRectXY[Ascii].bottom 	= IconScale*(Rect.bottom - ((float)StartY+(float)CharWidth*0.5f));
					
					//////////////////////////////////
					//// Save Unique Texture Rect ////
					//////////////////////////////////

					pFont->pCharRectUV[Ascii].left 	= (Rect.left    / (float)pFont->pText2->Texture.Width);//+(0.5/(float)pFont->pText2->Texture.Width);
					pFont->pCharRectUV[Ascii].right = (Rect.right	/ (float)pFont->pText2->Texture.Width);//+(0.5/(float)pFont->pText2->Texture.Width);
					pFont->pCharRectUV[Ascii].top 	= (Rect.top		/ (float)pFont->pText2->Texture.Width);//+(0.5/(float)pFont->pText2->Texture.Width);
					pFont->pCharRectUV[Ascii].bottom= (Rect.bottom	/ (float)pFont->pText2->Texture.Width);//+(0.5/(float)pFont->pText2->Texture.Width);


				}//End if(Ascii >= MinAscii && Ascii < MaxAscii)
				Ascii++;
			}//End for t<cNumCellsAcross
		}//End for s<cNumCellsAcross
		//DEBUG
		if(cPage==0)SaveBMP("Large0",(unsigned char*)pTextTexture,256,256);
		if(cPage==1)SaveBMP("Large1",(unsigned char*)pTextTexture,256,256);
    }//END for cPage<NumTexturePagesPerFont

	//////////////////////////////////////
	//// Fill Vertices For Char Quads ////
	//////////////////////////////////////
	pV = pFont->ptlV;
	for(i=0;i<128;i++)
	{

		//01
		//23

		FillXYZNRGBUV(&pV[i*4+0],pFont->pCharRectXY[i].left, pFont->pCharRectXY[i].top,   0.0,  0.0,0.0,1.0  ,Diffuse,pFont->pCharRectUV[i].left, pFont->pCharRectUV[i].top);		
		FillXYZNRGBUV(&pV[i*4+1],pFont->pCharRectXY[i].right,pFont->pCharRectXY[i].top,   0.0,  0.0,0.0,1.0  ,Diffuse,pFont->pCharRectUV[i].right,pFont->pCharRectUV[i].top);
		FillXYZNRGBUV(&pV[i*4+2],pFont->pCharRectXY[i].left, pFont->pCharRectXY[i].bottom,0.0,  0.0,0.0,1.0  ,Diffuse,pFont->pCharRectUV[i].left, pFont->pCharRectUV[i].bottom);
		FillXYZNRGBUV(&pV[i*4+3],pFont->pCharRectXY[i].right,pFont->pCharRectXY[i].bottom,0.0,  0.0,0.0,1.0  ,Diffuse,pFont->pCharRectUV[i].right,pFont->pCharRectUV[i].bottom);

	}

	/////////////////////////////////////
	//// Fill Indices For Char Quads ////
	/////////////////////////////////////

	pI = pFont->pI;
	for(i=0;i<128;i++)
	{//init indices list. each char is a quad 4 verts, 6 indices
		pI[i*6+0] = i*4+0;
		pI[i*6+1] = i*4+1;
		pI[i*6+2] = i*4+2;

		pI[i*6+3] = i*4+1;
		pI[i*6+4] = i*4+3;
		pI[i*6+5] = i*4+2;
	}

	/////////////////////////////////////////////////////////////////

	UpdateUniqueID(pFont->pText1->Texture);
	UpdateUniqueID(pFont->pText2->Texture);

	pFont->CharWidth = pFont->pCharRectXY[SPACE_CHAR].right-pFont->pCharRectXY[64].left;//@

	SaveBMP("Large0",(unsigned char*)pFont->pText1->Texture.pChar,256,256);
	SaveBMP("Large1",(unsigned char*)pFont->pText2->Texture.pChar,256,256);

}

bool PickThisMatrix(float* pMatrix)
{
	POINT ptCursor;
	aVECTOR CursorPos;
	aVECTOR CameraPos;
	aVECTOR SpherePos;
	aVECTOR PickedPos;
	aVECTOR RayDir;
	float pInverseView[16];
	double tin,tout;
	float Radius = App_PickSphereRadius;
	float Length;
	bool result;

	if(Radius<1.0)Radius = 1.0;
	

//	if(pCurrentlySelectedScene==NULL)return false;

    GetCursorPos(&ptCursor);
    ScreenToClient(Render_hWnd,&ptCursor);

	CursorPos.x =  (((2.0*ptCursor.x)/Render_WindowWidth )-1.0)/pRender_ProjMatrix[0];
	CursorPos.y = -(((2.0*ptCursor.y)/Render_WindowHeight)-1.0)/pRender_ProjMatrix[5];
	CursorPos.z =  1.0;

	InverseMatrix(pInverseView,pRender_ViewMatrix);

	RayDir.x  = CursorPos.x*pInverseView[0] + CursorPos.y*pInverseView[4] + CursorPos.z*pInverseView[8 ];
	RayDir.y  = CursorPos.x*pInverseView[1] + CursorPos.y*pInverseView[5] + CursorPos.z*pInverseView[9 ];
	RayDir.z  = CursorPos.x*pInverseView[2] + CursorPos.y*pInverseView[6] + CursorPos.z*pInverseView[10];

	MacroNormalize(Length,RayDir.x,RayDir.y,RayDir.z);

	CameraPos.x = pInverseView[12];
	CameraPos.y = pInverseView[13];
	CameraPos.z = pInverseView[14];

	SpherePos.x = pMatrix[12];
	SpherePos.y = pMatrix[13];
	SpherePos.z = pMatrix[14];

	result = IntesectSphere(CameraPos,RayDir,SpherePos,(double)Radius,&tin,&tout);

	if(result)
	{
		//calculate world XYZ of intersection point of mouse pos projected onto sphere.
		PickedPos.x = CameraPos.x + tin * RayDir.x;
		PickedPos.y = CameraPos.y + tin * RayDir.y;
		PickedPos.z = CameraPos.z + tin * RayDir.z;

		//create vector from center of sphere to projected mouse pos
		CurrentMouseSpherePickVector.x = PickedPos.x-SpherePos.x;
		CurrentMouseSpherePickVector.y = PickedPos.y-SpherePos.y;
		CurrentMouseSpherePickVector.z = PickedPos.z-SpherePos.z;
		MacroNormalize(Length,CurrentMouseSpherePickVector.x,CurrentMouseSpherePickVector.y,CurrentMouseSpherePickVector.z);
	}

	return result;
}







bool Mouse_Rotate_CurrentMatrix(float* pMatrix)
{
	aVECTOR Axis;
	aVECTOR MatrixPosition;
	float pPreviousToCurrentMat[16];
	float pModel[16];
	double angle,cosangle;
	double length;
	double AdotA;

	PreviousMouseSpherePickVector.x = CurrentMouseSpherePickVector.x;
	PreviousMouseSpherePickVector.y = CurrentMouseSpherePickVector.y;
	PreviousMouseSpherePickVector.z = CurrentMouseSpherePickVector.z;
	if(PickThisMatrix(pMatrix))
	{
		if(bCtrlKeyDown)//control camera
		{//twist about LOS vector
		}
		else
		{//arc ball type rotation
			//double precision dot todotodo product point-center of sphere
			cosangle = 	(PreviousMouseSpherePickVector.x*CurrentMouseSpherePickVector.x) +
						(PreviousMouseSpherePickVector.y*CurrentMouseSpherePickVector.y) +
						(PreviousMouseSpherePickVector.z*CurrentMouseSpherePickVector.z);

			angle = acos(cosangle);

			if(angle>0.0)
			{
				//double precision cross product
				Axis.x = PreviousMouseSpherePickVector.y*CurrentMouseSpherePickVector.z-PreviousMouseSpherePickVector.z*CurrentMouseSpherePickVector.y;
				Axis.y = PreviousMouseSpherePickVector.z*CurrentMouseSpherePickVector.x-PreviousMouseSpherePickVector.x*CurrentMouseSpherePickVector.z;
				Axis.z = PreviousMouseSpherePickVector.x*CurrentMouseSpherePickVector.y-PreviousMouseSpherePickVector.y*CurrentMouseSpherePickVector.x;

				//double precision dot
				AdotA = (Axis.x*Axis.x) + (Axis.y*Axis.y) + (Axis.z*Axis.z);
				if(AdotA>0.0)
				{
					//double precision normalize
					length = 1.0/(sqrt(Axis.x*Axis.x+Axis.y*Axis.y+Axis.z*Axis.z));
					Axis.x *= length;
					Axis.y *= length;
					Axis.z *= length;

					AxisRotationMatrix(pPreviousToCurrentMat,Axis.x,Axis.y,Axis.z,angle);

					memcpy(pModel,pMatrix,16*sizeof(float));

					MatrixPosition.x = pMatrix[12];
					MatrixPosition.y = pMatrix[13];
					MatrixPosition.z = pMatrix[14];

					pMatrix[12] = 0.0;
					pMatrix[13] = 0.0;
					pMatrix[14] = 0.0;

					MatrixMul(pMatrix,pModel,pPreviousToCurrentMat);

					//dont translate sphere at all
					pMatrix[12] = MatrixPosition.x;
					pMatrix[13] = MatrixPosition.y;
					pMatrix[14] = MatrixPosition.z;

					//MatrixMul(pCurrentlySelectedMatrix,pPreviousToCurrentMat,pModel);
					OrthonormalizeOrientation(pMatrix);

					memcpy(pModel,pAppTools_PickSphere->pMatrix,16*sizeof(float));
					MatrixMul(pAppTools_PickSphere->pMatrix,pModel,pPreviousToCurrentMat);
					OrthonormalizeOrientation(pAppTools_PickSphere->pMatrix);


					///////////////////////////////
					float pFinal[16];
					float pScale[16];
					Scale(pScale,App_PickSphereRadius);
					MatrixMul(pFinal,pAppTools_PickSphere->pMatrix,pScale);
					memcpy(pAppTools_PickSphere->pMatrix,pFinal,16*sizeof(float));
					///////////////////////////////


					pAppTools_PickSphere->pMatrix[12] = pMatrix[12];
					pAppTools_PickSphere->pMatrix[13] = pMatrix[13];
					pAppTools_PickSphere->pMatrix[14] = pMatrix[14];

				}//End if(AdotA>0.0)
			}//End if(angle>0.0)
		}//End else arc ball type rotation
	}
	return true;
}




//pActorMatrix = pObject->pMatrix
bool Mouse_Move_CurrentMatrix(float* pMatrix)
//bool Mouse_Move_CurrentMatrix(float* pMatrix)
{
	POINT ptCursor;
	aVECTOR CameraToMatrix;
	aVECTOR CursorPos;
	aVECTOR RayDir;
	float pInverseView[16];
	float Distance;
	float Radius = 1.0;

	if(pMatrix==NULL)return false;

    GetCursorPos(&ptCursor);
    ScreenToClient(Render_hWnd,&ptCursor);

	CursorPos.x =  (((2.0*ptCursor.x)/Render_WindowWidth )-1.0)/pRender_ProjMatrix[0];
	CursorPos.y = -(((2.0*ptCursor.y)/Render_WindowHeight)-1.0)/pRender_ProjMatrix[5];
	CursorPos.z =  1.0;

	InverseMatrix(pInverseView,pRender_ViewMatrix);

	RayDir.x  = CursorPos.x*pInverseView[0] + CursorPos.y*pInverseView[4] + CursorPos.z*pInverseView[8 ];
	RayDir.y  = CursorPos.x*pInverseView[1] + CursorPos.y*pInverseView[5] + CursorPos.z*pInverseView[9 ];
	RayDir.z  = CursorPos.x*pInverseView[2] + CursorPos.y*pInverseView[6] + CursorPos.z*pInverseView[10];

	CameraToMatrix.x = pMatrix[12]-pInverseView[12];
	CameraToMatrix.y = pMatrix[13]-pInverseView[13];
	CameraToMatrix.z = pMatrix[14]-pInverseView[14];

	MacroDot(Distance,	CameraToMatrix.x,
						CameraToMatrix.y,
						CameraToMatrix.z,
						pInverseView[8],
						pInverseView[9],
						pInverseView[10]);
	
	pMatrix[12] = pInverseView[12] + (RayDir.x * Distance);
	pMatrix[13] = pInverseView[13] + (RayDir.y * Distance);
	pMatrix[14] = pInverseView[14] + (RayDir.z * Distance);


	///////////////////////////////
	float pFinal[16];
	float pScale[16];
	OrthonormalizeOrientation(pAppTools_PickSphere->pMatrix);

	Scale(pScale,App_PickSphereRadius);
	MatrixMul(pFinal,pAppTools_PickSphere->pMatrix,pScale);
	memcpy(pAppTools_PickSphere->pMatrix,pFinal,16*sizeof(float));
	///////////////////////////////


	pAppTools_PickSphere->pMatrix[12] = pMatrix[12];
	pAppTools_PickSphere->pMatrix[13] = pMatrix[13];
	pAppTools_PickSphere->pMatrix[14] = pMatrix[14];

	return true;
}


void RefreshPointer2(struct MeshLL* pMesh)//KEEP THIS : SCREEN QUAD
{
	struct SceneLL* 	pScene  = NULL;
	VERTEX_XYZNRGBUV Vert[4];
	VERTEX_XYZNRGBUV* pWorldVerts = NULL;
	WORD* pIndices = NULL;
	float Scale = 32.0;
	long Diffuse = 0xffffffff;

	POINT ptCursor;
	

	pWorldVerts   = pMesh->pWorldVerts;
	pIndices = pMesh->pIndices;
	pScene = pRender_SceneHead;

    GetCursorPos( &ptCursor );
    ScreenToClient( Render_hWnd  , &ptCursor );

	Vert[0].x =  0+ptCursor.x-0.5;
	Vert[0].y =  0+ptCursor.y-0.5;
	Vert[0].z =  1.0;

	Vert[1].x =  Scale+ptCursor.x-0.5;
	Vert[1].y =  0    +ptCursor.y-0.5;
	Vert[1].z =  1.0;

	Vert[2].x =  0    +ptCursor.x-0.5;
	Vert[2].y =  Scale+ptCursor.y-0.5;
	Vert[2].z =  1.0;

	Vert[3].x =  Scale+ptCursor.x-0.5;
	Vert[3].y =  Scale+ptCursor.y-0.5;
	Vert[3].z =  1.0;

	FillXYZNRGBUV(&pWorldVerts[0],Vert[0].x,Vert[0].y,Vert[0].z,0.0,0.0,1.0,Diffuse,((00.0-0.0)/32.0)	,((00.0-0.0)/32.0));//256.0));
	FillXYZNRGBUV(&pWorldVerts[1],Vert[1].x,Vert[1].y,Vert[1].z,0.0,0.0,1.0,Diffuse,((32.0-0.0)/32.0)	,((00.0-0.0)/32.0));///256.0));
	FillXYZNRGBUV(&pWorldVerts[2],Vert[2].x,Vert[2].y,Vert[2].z,0.0,0.0,1.0,Diffuse,((00.0-0.0)/32.0)	,((32.0-0.0)/32.0));///256.0));
	FillXYZNRGBUV(&pWorldVerts[3],Vert[3].x,Vert[3].y,Vert[3].z,0.0,0.0,1.0,Diffuse,((32.0-0.0)/32.0)	,((32.0-0.0)/32.0));///256.0));

	//triangles are described in counter clock wise order
	pIndices[0 ] = 0;
	pIndices[1 ] = 2;
	pIndices[2 ] = 1;

	pIndices[3 ] = 1;
	pIndices[4 ] = 2;
	pIndices[5 ] = 3;

	pMesh->num_verts = 4;
	pMesh->num_tris = 2;
}


void CreateVectorGeometry(struct MeshLL* pMesh,int VectorType)
{
	long Diffuse = 0xffffffff;
	VERTEX_XYZNRGBUV* pLocalVerts = NULL;
	WORD* pIndices = NULL;



	pLocalVerts = pMesh->pLocalVerts;
	pIndices = pMesh->pIndices;

	float Scale = 1.0;
	float ArrowLength = 1.0;
	float HeadLength  = 0.2851;//(72/256)from bitmap
	float StemLength  = 1.0-HeadLength;//(72/256)from bitmap
	float hWidth      = 0.1428;//1/7 from bitmap. 7 vectors

	fRECT ArrowHeadXY;
	fRECT ArrowStemXY;
	fRECT ArrowHeadUV;
	fRECT ArrowStemUV;



	ArrowHeadXY.left   = hWidth*0.5;
	ArrowHeadXY.right  = - hWidth*0.5;
	ArrowHeadXY.top    =  1.0;
	ArrowHeadXY.bottom =  StemLength;

	ArrowStemXY.left   = hWidth*0.5;
	ArrowStemXY.right  = - hWidth*0.5;
	ArrowStemXY.top    =  StemLength;
	ArrowStemXY.bottom =  0.0;

	ArrowHeadUV.left   = 0.0;
	ArrowHeadUV.right  = 1.0/7.0;
	ArrowHeadUV.top    = 0.0;
	ArrowHeadUV.bottom = HeadLength;

	if(VectorType==0)//x
	{
		ArrowHeadUV.left   = 0.0;
		ArrowHeadUV.right  = 37.0/256.0;
		ArrowHeadUV.top    = 0.0;
		ArrowHeadUV.bottom = HeadLength;

		ArrowStemUV.left   = 0.0;
		ArrowStemUV.right  = 37.0/256.0;
		ArrowStemUV.top    = HeadLength;
		ArrowStemUV.bottom = 1.0;
	}
	if(VectorType==1)//y
	{
		ArrowHeadUV.left   = 37.0/256.0;
		ArrowHeadUV.right  = 73.0/256.0;
		ArrowHeadUV.top    = 0.0;
		ArrowHeadUV.bottom = HeadLength;

		ArrowStemUV.left   = 37.0/256.0;
		ArrowStemUV.right  = 73.0/256.0;
		ArrowStemUV.top    = HeadLength;
		ArrowStemUV.bottom = 1.0;
	}
	if(VectorType==2)//z
	{
		ArrowHeadUV.left   = 73.0/256.0;
		ArrowHeadUV.right  = 109.0/256.0;
		ArrowHeadUV.top    = 0.0;
		ArrowHeadUV.bottom = HeadLength;

		ArrowStemUV.left   = 73.0/256.0;
		ArrowStemUV.right  = 109.0/256.0;
		ArrowStemUV.top    = HeadLength;
		ArrowStemUV.bottom = 1.0;
	}
	if(VectorType==3)//u
	{
		ArrowHeadUV.left   = 109.0/256.0;
		ArrowHeadUV.right  = 146.0/256.0;
		ArrowHeadUV.top    = 0.0;
		ArrowHeadUV.bottom = HeadLength;

		ArrowStemUV.left   = 109.0/256.0;
		ArrowStemUV.right  = 146.0/256.0;
		ArrowStemUV.top    = HeadLength;
		ArrowStemUV.bottom = 1.0;
	}
	if(VectorType==4)//v
	{
		ArrowHeadUV.left   = 146.0/256.0;
		ArrowHeadUV.right  = 183.0/256.0;
		ArrowHeadUV.top    = 0.0;
		ArrowHeadUV.bottom = HeadLength;

		ArrowStemUV.left   = 146.0/256.0;
		ArrowStemUV.right  = 183.0/256.0;
		ArrowStemUV.top    = HeadLength;
		ArrowStemUV.bottom = 1.0;
	}
	if(VectorType==5)//n
	{
		ArrowHeadUV.left   = 182.0/256.0;
		ArrowHeadUV.right  = 219.0/256.0;
		ArrowHeadUV.top    = 0.0;
		ArrowHeadUV.bottom = HeadLength;

		ArrowStemUV.left   = 182.0/256.0;
		ArrowStemUV.right  = 219.0/256.0;
		ArrowStemUV.top    = HeadLength;
		ArrowStemUV.bottom = 1.0;
	}
	if(VectorType==6)//p
	{
		ArrowHeadUV.left   = 219.0/256.0;
		ArrowHeadUV.right  = 256.0/256.0;
		ArrowHeadUV.top    = 0.0;
		ArrowHeadUV.bottom = HeadLength;

		ArrowStemUV.left   = 219.0/256.0;
		ArrowStemUV.right  = 256.0/256.0;
		ArrowStemUV.top    = HeadLength;
		ArrowStemUV.bottom = 1.0;

	}


	if(VectorType==7)//BoneRED
	{
		ArrowHeadUV.left   = 0.0;
		ArrowHeadUV.right  = 37.0/256.0;
		ArrowHeadUV.top    = 0.0;
		ArrowHeadUV.bottom = HeadLength;

		ArrowStemUV.left   = 0.0;
		ArrowStemUV.right  = 37.0/256.0;
		ArrowStemUV.top    = HeadLength;
		ArrowStemUV.bottom = 1.0;

		//go below zero to include o axle which is at zero
		ArrowStemXY.bottom = -(14.0/256.0);

		//make wide
		ArrowHeadXY.left   = hWidth*0.7;
		ArrowHeadXY.right  = - hWidth*0.7;

		ArrowStemXY.left   = hWidth*0.7;
		ArrowStemXY.right  = -hWidth*0.7;
	}
	if(VectorType==8)//BoneYELLOW
	{
		ArrowHeadUV.left   = 37.0/256.0;
		ArrowHeadUV.right  = 73.0/256.0;
		ArrowHeadUV.top    = 0.0;
		ArrowHeadUV.bottom = HeadLength;

		ArrowStemUV.left   = 37.0/256.0;
		ArrowStemUV.right  = 73.0/256.0;
		ArrowStemUV.top    = HeadLength;
		ArrowStemUV.bottom = 1.0;

		//go below zero to include o axle which is at zero
		ArrowStemXY.bottom = -(14.0/256.0);

		//make wide
		ArrowHeadXY.left   = hWidth*0.7;
		ArrowHeadXY.right  = - hWidth*0.7;

		ArrowStemXY.left   = hWidth*0.7;
		ArrowStemXY.right  = -hWidth*0.7;
	}
	if(VectorType==9)//BoneBLUE
	{
		ArrowHeadUV.left   = 73.0/256.0;
		ArrowHeadUV.right  = 109.0/256.0;
		ArrowHeadUV.top    = 0.0;
		ArrowHeadUV.bottom = HeadLength;

		ArrowStemUV.left   = 73.0/256.0;
		ArrowStemUV.right  = 109.0/256.0;
		ArrowStemUV.top    = HeadLength;
		ArrowStemUV.bottom = 1.0;

		//go below zero to include o axle which is at zero
		ArrowStemXY.bottom = -(14.0/256.0);

		//make wide
		ArrowHeadXY.left   = hWidth*0.7;
		ArrowHeadXY.right  = - hWidth*0.7;

		ArrowStemXY.left   = hWidth*0.7;
		ArrowStemXY.right  = -hWidth*0.7;
	}






	FillXYZNRGBUV(&pLocalVerts[0],ArrowHeadXY.left, ArrowHeadXY.top,   0.0,   0.0,0.0,-1.0,   Diffuse,ArrowHeadUV.left, ArrowHeadUV.top);
	FillXYZNRGBUV(&pLocalVerts[1],ArrowHeadXY.right,ArrowHeadXY.top,   0.0,   0.0,0.0,-1.0,   Diffuse,ArrowHeadUV.right,ArrowHeadUV.top);
	FillXYZNRGBUV(&pLocalVerts[2],ArrowHeadXY.left, ArrowHeadXY.bottom,0.0,   0.0,0.0,-1.0,   Diffuse,ArrowHeadUV.left, ArrowHeadUV.bottom);
	FillXYZNRGBUV(&pLocalVerts[3],ArrowHeadXY.right,ArrowHeadXY.bottom,0.0,   0.0,0.0,-1.0,   Diffuse,ArrowHeadUV.right,ArrowHeadUV.bottom);

	FillXYZNRGBUV(&pLocalVerts[4],ArrowStemXY.left, ArrowStemXY.top,   0.0,   0.0,0.0,-1.0,   Diffuse,ArrowStemUV.left, ArrowStemUV.top);
	FillXYZNRGBUV(&pLocalVerts[5],ArrowStemXY.right,ArrowStemXY.top,   0.0,   0.0,0.0,-1.0,   Diffuse,ArrowStemUV.right,ArrowStemUV.top);
	FillXYZNRGBUV(&pLocalVerts[6],ArrowStemXY.left, ArrowStemXY.bottom,0.0,   0.0,0.0,-1.0,   Diffuse,ArrowStemUV.left, ArrowStemUV.bottom);
	FillXYZNRGBUV(&pLocalVerts[7],ArrowStemXY.right,ArrowStemXY.bottom,0.0,   0.0,0.0,-1.0,   Diffuse,ArrowStemUV.right,ArrowStemUV.bottom);



	pIndices[0 ] = 0;
	pIndices[1 ] = 2;
	pIndices[2 ] = 1;

	pIndices[3 ] = 1;
	pIndices[4 ] = 2;
	pIndices[5 ] = 3;
	  
	pIndices[6 ] = 4;
	pIndices[7 ] = 6;
	pIndices[8 ] = 5;

	pIndices[9 ] = 5;
	pIndices[10] = 6;
	pIndices[11] = 7;

	pMesh->num_verts = 8;
	pMesh->num_tris = 4;
	if(VectorType<7)
	{
		LoadTexture("vector.bmp",NULL,pMesh->Texture,TEXTURE_FLAG_4444);
	}
	else
	{
		LoadTexture("vector2.bmp",NULL,pMesh->Texture,TEXTURE_FLAG_4444);
	}
}
























void CreateBoneGeometry(struct MeshLL* pMesh)
{
	long Diffuse = 0xffffffff;
	VERTEX_XYZNRGBUV* pLocalVerts = NULL;
	WORD* pIndices = NULL;

	pLocalVerts = pMesh->pLocalVerts;
	pIndices = pMesh->pIndices;

	float X_Scale  =  0.5;
	float Y_Scale  =  0.5;
	float Z_Scale0 =  0.0;
	float Z_Scale1 = 10.0;

	FillXYZNRGBUV(&pLocalVerts[0 ],-X_Scale, Y_Scale, Z_Scale0,   0.0,1.0,0.0,  Diffuse,   0.0/64.0,  0.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[1 ],-X_Scale, Y_Scale, Z_Scale1,   0.0,1.0,0.0,  Diffuse,  48.0/64.0,  0.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[2 ], X_Scale, Y_Scale, Z_Scale0,   0.0,1.0,0.0,  Diffuse,   0.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[3 ], X_Scale, Y_Scale, Z_Scale1,   0.0,1.0,0.0,  Diffuse,  48.0/64.0, 16.0/64.0);

	FillXYZNRGBUV(&pLocalVerts[4 ], X_Scale, Y_Scale, Z_Scale0,   1.0,0.0,0.0,  Diffuse,   0.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[5 ], X_Scale, Y_Scale, Z_Scale1,   1.0,0.0,0.0,  Diffuse,  48.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[6 ], X_Scale,-Y_Scale, Z_Scale0,   1.0,0.0,0.0,  Diffuse,   0.0/64.0, 32.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[7 ], X_Scale,-Y_Scale, Z_Scale1,   1.0,0.0,0.0,  Diffuse,  48.0/64.0, 32.0/64.0);

	FillXYZNRGBUV(&pLocalVerts[8 ], X_Scale,-Y_Scale, Z_Scale0,   0.0,-1.0,0.0, Diffuse,   0.0/64.0, 32.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[9 ], X_Scale,-Y_Scale, Z_Scale1,   0.0,-1.0,0.0, Diffuse,  48.0/64.0, 32.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[10],-X_Scale,-Y_Scale, Z_Scale0,   0.0,-1.0,0.0, Diffuse,   0.0/64.0, 48.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[11],-X_Scale,-Y_Scale, Z_Scale1,   0.0,-1.0,0.0, Diffuse,  48.0/64.0, 48.0/64.0);

	FillXYZNRGBUV(&pLocalVerts[12],-X_Scale,-Y_Scale, Z_Scale0,   -1.0,0.0,0.0, Diffuse,   0.0/64.0, 48.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[13],-X_Scale,-Y_Scale, Z_Scale1,   -1.0,0.0,0.0, Diffuse,  48.0/64.0, 48.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[14],-X_Scale, Y_Scale, Z_Scale0,   -1.0,0.0,0.0, Diffuse,   0.0/64.0, 64.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[15],-X_Scale, Y_Scale, Z_Scale1,   -1.0,0.0,0.0, Diffuse,  48.0/64.0, 64.0/64.0);

	FillXYZNRGBUV(&pLocalVerts[16],-X_Scale, Y_Scale, Z_Scale0,   0.0,0.0,-1.0, Diffuse,  48.0/64.0,  0.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[17], X_Scale, Y_Scale, Z_Scale0,   0.0,0.0,-1.0, Diffuse,  64.0/64.0,  0.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[18],-X_Scale,-Y_Scale, Z_Scale0,   0.0,0.0,-1.0, Diffuse,  48.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[19], X_Scale,-Y_Scale, Z_Scale0,   0.0,0.0,-1.0, Diffuse,  64.0/64.0, 16.0/64.0);

	FillXYZNRGBUV(&pLocalVerts[20], X_Scale, Y_Scale, Z_Scale1,   0.0,0.0,1.0,  Diffuse,  48.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[21],-X_Scale, Y_Scale, Z_Scale1,   0.0,0.0,1.0,  Diffuse,  64.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[22], X_Scale,-Y_Scale, Z_Scale1,   0.0,0.0,1.0,  Diffuse,  48.0/64.0, 32.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[23],-X_Scale,-Y_Scale, Z_Scale1,   0.0,0.0,1.0,  Diffuse,  64.0/64.0, 32.0/64.0);

	pIndices[0 ] = 0;
	pIndices[1 ] = 2;
	pIndices[2 ] = 1;

	pIndices[3 ] = 1;
	pIndices[4 ] = 2;
	pIndices[5 ] = 3;
	  
	pIndices[6 ] = 4;
	pIndices[7 ] = 6;
	pIndices[8 ] = 5;

	pIndices[9 ] = 5;
	pIndices[10] = 6;
	pIndices[11] = 7;

	pIndices[12] = 8;
	pIndices[13] = 10;
	pIndices[14] = 9;

	pIndices[15] = 9;
	pIndices[16] = 10;
	pIndices[17] = 11;

	pIndices[18] = 12;
	pIndices[19] = 14;
	pIndices[20] = 13;

	pIndices[21] = 13;
	pIndices[22] = 14;
	pIndices[23] = 15;

	//End Cap

	pIndices[24] = 16;
	pIndices[25] = 18;
	pIndices[26] = 17;

	pIndices[27] = 17;
	pIndices[28] = 18;
	pIndices[29] = 19;

	//Pointy Tip

	pIndices[30] = 20;
	pIndices[31] = 22;
	pIndices[32] = 21;

	pIndices[33] = 21;
	pIndices[34] = 22;
	pIndices[35] = 23;

	//change winding
/*
	for(int i=0;i<36;i+=3)
	{
		Temp = pIndices[1 ];
		pIndices[1] = pIndices[2];
		pIndices[2] = Temp;
	}
*/
	pMesh->num_verts = 24;
	pMesh->num_tris = 12;

	LoadTexture("bone.bmp",NULL,pMesh->Texture,TEXTURE_FLAG_4444);
}
















/*
	FillXYZNRGBUV(&pLocalVerts[0 ],-X_Scale, Y_Scale, Z_Scale0,   0.0,1.0,0.0, Diffuse,   0.0/64.0,  0.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[1 ],-X_Scale, Y_Scale, Z_Scale1,   0.0,1.0,0.0, Diffuse,  48.0/64.0,  0.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[2 ], X_Scale, Y_Scale, Z_Scale0,   0.0,1.0,0.0, Diffuse,   0.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[3 ], X_Scale, Y_Scale, Z_Scale1,   0.0,1.0,0.0, Diffuse,  48.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[4 ], X_Scale,-Y_Scale, Z_Scale0,   0.0,1.0,0.0, Diffuse,   0.0/64.0, 32.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[5 ], X_Scale,-Y_Scale, Z_Scale1,   0.0,1.0,0.0, Diffuse,  48.0/64.0, 32.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[6 ],-X_Scale,-Y_Scale, Z_Scale0,   0.0,1.0,0.0, Diffuse,   0.0/64.0, 48.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[7 ],-X_Scale,-Y_Scale, Z_Scale1,   0.0,1.0,0.0, Diffuse,  48.0/64.0, 48.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[8 ],-X_Scale, Y_Scale, Z_Scale0,   0.0,1.0,0.0, Diffuse,   0.0/64.0, 64.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[9 ],-X_Scale, Y_Scale, Z_Scale1,   0.0,1.0,0.0, Diffuse,  48.0/64.0, 64.0/64.0);


	FillXYZNRGBUV(&pLocalVerts[10],-X_Scale, Y_Scale, Z_Scale0,   0.0,1.0,0.0, Diffuse,  48.0/64.0,  0.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[11], X_Scale, Y_Scale, Z_Scale0,   0.0,1.0,0.0, Diffuse,  64.0/64.0,  0.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[12],-X_Scale,-Y_Scale, Z_Scale0,   0.0,1.0,0.0, Diffuse,  48.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[13], X_Scale,-Y_Scale, Z_Scale0,   0.0,1.0,0.0, Diffuse,  64.0/64.0, 16.0/64.0);

	FillXYZNRGBUV(&pLocalVerts[14], X_Scale, Y_Scale, Z_Scale1,   0.0,1.0,0.0, Diffuse,  48.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[15],-X_Scale, Y_Scale, Z_Scale1,   0.0,1.0,0.0, Diffuse,  64.0/64.0, 16.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[16], X_Scale,-Y_Scale, Z_Scale1,   0.0,1.0,0.0, Diffuse,  48.0/64.0, 32.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[17],-X_Scale,-Y_Scale, Z_Scale1,   0.0,1.0,0.0, Diffuse,  64.0/64.0, 32.0/64.0);
	FillXYZNRGBUV(&pLocalVerts[18], 0.0,     0.0,	  Z_Scale2,   0.0,1.0,0.0, Diffuse,  56.0/64.0, 24.0/64.0);
*/