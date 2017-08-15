
//Common.cpp written by david paull.
//This should be called Tools.cpp!!!
// Copyright (c) 2001 Tanzanite Software. All rights reserved.
// The names, "Tanzanite", and "TechNature" are registered trademarks of Tanzanite Software, and David Paull
// 		support@tanzanite.to
// 		etinaznat@hotmail.com
// work in progress : For latest version goto www.tanzanite.to

#include "stdafx.h"//this all needs to go into render.cpp
#include "Library.h"
#include "FileAccess.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>//tolower

#include <time.h>//clock()
#include <assert.h>

/////////////////////
//// Extern Data ////
/////////////////////

extern float Render_WindowWidth;
extern float Render_WindowHeight;
extern float pRender_ViewMatrix[];
extern float pRender_ProjMatrix[];
extern char* gp_TextBuffer3;

////////////////////
//// Local Data ////
////////////////////

clock_t StartTime;

////////////////////////////////////
//// Extern Function Prototypes ////
////////////////////////////////////
extern void AppendDebugMsg(char *pChar);

///////////////////////////////////
//// Local Function Prototypes ////
///////////////////////////////////
/*
void MatrixMul(float* pDst,float* pA,float* pB);
void CreateAxisAngleFromMatrix(	aVECTOR& Axis,
								float& Angle,
								float* pM);
void OrthonormalizeOrientation(float* pMat);
*/
/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""aVECTOR""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/


void CreateRandomVector(aVECTOR* pMat)
{
	float Length;
	pMat->x = RandNegOneToOne;
	pMat->y = RandNegOneToOne;
	pMat->z = RandNegOneToOne;
	MacroNormalize(Length,pMat->x,pMat->y,pMat->z);	
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
void VertexMatrixMul(	VERTEX_XYZNRGBUV* pDstV,
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









float Min_Distance_PointToBone(	aVECTOR Point,
								aVECTOR RayOrigin,
								aVECTOR	RayDir)//not normalized!!
{
	float RayLength;
	float Length;
	aVECTOR Dir;
	aVECTOR ToPoint;
	aVECTOR ProjectedPoint;
	aVECTOR P1toP2;
	
	Dir.x = RayDir.x;
	Dir.y = RayDir.y;
	Dir.z = RayDir.z;
	MacroMagnitude(RayLength,Dir.x,Dir.y,Dir.z);
	MacroNormalize(Length,Dir.x,Dir.y,Dir.z);

	if(RayLength==1.0){assert(0 && "RayDir shouldnt be normalized. can ignore");}

	ToPoint.x = Point.x-RayOrigin.x;
	ToPoint.y = Point.y-RayOrigin.y;
	ToPoint.z = Point.z-RayOrigin.z;

	MacroDot(Length,ToPoint.x,
					ToPoint.y,
					ToPoint.z,
					Dir.x,
					Dir.y,
					Dir.z);

	if(Length<=0)
	{
		P1toP2.x = RayOrigin.x-Point.x;
		P1toP2.y = RayOrigin.y-Point.y;
		P1toP2.z = RayOrigin.z-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
	}
	else if(Length>0 && Length<RayLength)
	{
		ProjectedPoint.x = RayOrigin.x+Dir.x*Length;
		ProjectedPoint.y = RayOrigin.y+Dir.y*Length;
		ProjectedPoint.z = RayOrigin.z+Dir.z*Length;
	
		P1toP2.x = ProjectedPoint.x-Point.x;
		P1toP2.y = ProjectedPoint.y-Point.y;
		P1toP2.z = ProjectedPoint.z-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
	}
	else//Length > RayLength
	{//ONLY RETURN DISTANCE TO START POINT>,THIS BONES ENDPOINT WILL BE TESTED FOR NEXT BONE
		//P1toP2.x = (RayOrigin.x+RayDir.x)-Point.x;
		//P1toP2.y = (RayOrigin.y+RayDir.y)-Point.y;
		//P1toP2.z = (RayOrigin.z+RayDir.z)-Point.z;
		P1toP2.x = RayOrigin.x-Point.x;
		P1toP2.y = RayOrigin.y-Point.y;
		P1toP2.z = RayOrigin.z-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
	}
	return Length;
}


// calcs 3 distances, and adds them together
// add dist to line, and dist to both endpoints. 
// This gives an overall feel for the influence this bone might have over a point.
// This is used to determine which bone has the greatest influence. InitWeights()

float Tri_Distance_PointToBone(	aVECTOR Point,
								aVECTOR RayOrigin,
								aVECTOR	RayDir)
{
	float RayLength;
	float Length;
	float TriDistance = 0;
	aVECTOR Dir;
	aVECTOR ToPoint;
	aVECTOR ProjectedPoint;
	aVECTOR P1toP2;
	
	Dir.x = RayDir.x;
	Dir.y = RayDir.y;
	Dir.z = RayDir.z;
	MacroMagnitude(RayLength,Dir.x,Dir.y,Dir.z);
	MacroNormalize(Length,Dir.x,Dir.y,Dir.z);

	if(RayLength==1.0){assert(0 && "RayDir shouldnt be normalized. can ignore");}

	ToPoint.x = Point.x-RayOrigin.x;
	ToPoint.y = Point.y-RayOrigin.y;
	ToPoint.z = Point.z-RayOrigin.z;

	MacroDot(Length,ToPoint.x,
					ToPoint.y,
					ToPoint.z,
					Dir.x,
					Dir.y,
					Dir.z);

	if(Length<=0)
	{// point is past bone endpoint. calc distance to that endpoint twice + distance to other endpoint

		P1toP2.x = RayOrigin.x-Point.x;
		P1toP2.y = RayOrigin.y-Point.y;
		P1toP2.z = RayOrigin.z-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
		TriDistance = Length*2;
		//////////////////////
		//now other endpoint
		P1toP2.x = (RayOrigin.x+RayDir.x)-Point.x;
		P1toP2.y = (RayOrigin.y+RayDir.y)-Point.y;
		P1toP2.z = (RayOrigin.z+RayDir.z)-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
		TriDistance += Length;

	}
	else if(Length>0 && Length<RayLength)
	{
		//////////////////////
		P1toP2.x = RayOrigin.x-Point.x;
		P1toP2.y = RayOrigin.y-Point.y;
		P1toP2.z = RayOrigin.z-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
		TriDistance = Length;
		//////////////////////
		ProjectedPoint.x = RayOrigin.x+Dir.x*Length;
		ProjectedPoint.y = RayOrigin.y+Dir.y*Length;
		ProjectedPoint.z = RayOrigin.z+Dir.z*Length;
	
		P1toP2.x = ProjectedPoint.x-Point.x;
		P1toP2.y = ProjectedPoint.y-Point.y;
		P1toP2.z = ProjectedPoint.z-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
		TriDistance += Length;
		//////////////////////
		P1toP2.x = (RayOrigin.x+RayDir.x)-Point.x;
		P1toP2.y = (RayOrigin.y+RayDir.y)-Point.y;
		P1toP2.z = (RayOrigin.z+RayDir.z)-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
		TriDistance += Length;
	}
	else//Length > RayLength
	{
		P1toP2.x = RayOrigin.x-Point.x;
		P1toP2.y = RayOrigin.y-Point.y;
		P1toP2.z = RayOrigin.z-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
		TriDistance = Length;
		//////////////////////
		P1toP2.x = (RayOrigin.x+RayDir.x)-Point.x;
		P1toP2.y = (RayOrigin.y+RayDir.y)-Point.y;
		P1toP2.z = (RayOrigin.z+RayDir.z)-Point.z;

		MacroMagnitude(Length,P1toP2.x,P1toP2.y,P1toP2.z);
		TriDistance += Length*2;
	}
	return TriDistance;
}



/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""VERTEX_XYZNRGBUV""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/




//matTemp = matWorld * matView;
//pMat    = matTemp * matProj;
//point is inview if 	pDst->x [0-640] &&
//				    	pDst->y [0-480]	&&
//						pDst->z [0-1]
//3d to 2d projection
bool TransformVertex(	VERTEX_XYZNRGBUV* pDstV,
						VERTEX_XYZNRGBUV* pSrcV,
						float* pMat,
						float ScreenWidth,
						float ScreenHeight)
{
	float x,y,z,w;
	// Transform it through the current matrix set
	//xp = matSet._11*x + matSet._21*y + matSet._31*z + matSet._41;
	//yp = matSet._12*x + matSet._22*y + matSet._32*z + matSet._42;
	//zp = matSet._13*x + matSet._23*y + matSet._33*z + matSet._43;
	//wp = matSet._14*x + matSet._24*y + matSet._34*z + matSet._44;

	x = pSrcV->x*pMat[0] + 
		pSrcV->y*pMat[4] + 
		pSrcV->z*pMat[8] +
		pMat[12];
	y = pSrcV->x*pMat[1] + 
		pSrcV->y*pMat[5] + 
		pSrcV->z*pMat[9] +
		pMat[13];
	z = pSrcV->x*pMat[2] + 
		pSrcV->y*pMat[6] + 
		pSrcV->z*pMat[10]+
		pMat[14];
	w = pSrcV->x*pMat[3] + 
		pSrcV->y*pMat[7] + 
		pSrcV->z*pMat[11]+
		pMat[15];

	//Finally, scale the vertices to screen coordinates. In the following code, the first step flattens the coordinates from 3-D space to 2-D device coordinates by dividing each coordinate by the wp value. Then, the x- and y-components are transformed from device coordinates to screen coordinates.
	if(w!=0.0)
	{
    	pDstV->x  = ( 1.0f + (x/w) ) * (ScreenWidth *0.5);//[0-2]*(SW/2)
    	pDstV->y  = ( 1.0f - (y/w) ) * (ScreenHeight*0.5);//[0-2]*(SH/2)
    	pDstV->z  = z / w;
    	//pDstV[i].rhw = w;
	}
	else//avoid div by 0
	{
    	pDstV->x  = 0.0;
    	pDstV->y  = 0.0;
    	pDstV->z  = -1.0;//flag as not valid
	}

    if(pDstV->z >= 0.0 && pDstV->z <= 1.0)
	{
    	if(pDstV->x >= 0.0 && pDstV->x <= ScreenWidth)
		{
    		if(pDstV->y >= 0.0 && pDstV->y <= ScreenHeight)
			{
				return true;//in view
			}
		}
	}
	return false;
}




void FillXYZNRGBUV(VERTEX_XYZNRGBUV* pV,
					float x,
					float y,
					float z,
					float nx,
					float ny,
					float nz,
					long diffuse,
					float u,
					float v)
{
	pV->x   = x;
	pV->y   = y;
	pV->z   = z;
	pV->nx  = nx;
	pV->ny  = ny;
	pV->nz  = nz;
	pV->diffuse  = diffuse;
	pV->tu  = u;
	pV->tv  = v;
}








/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""TRIANGLE""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/


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



void CalculatePlaneNormal(aVECTOR& vR,aVECTOR pA,aVECTOR pB,aVECTOR pC)
{
	float Length,Ax,Ay,Az,Bx,By,Bz;

	Ax = pC.x - pB.x;
	Ay = pC.y - pB.y;
	Az = pC.z - pB.z;

	Bx = pA.x - pB.x;
	By = pA.y - pB.y;
	Bz = pA.z - pB.z;

	MacroCross(vR.x,vR.y,vR.z,Ax,Ay,Az,Bx,By,Bz);
	MacroNormalize(Length,vR.x,vR.y,vR.z);
}

/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""MATRIX""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/
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




void PositionMatrix(float *pMat,float Ax,float Ay,float Az)
{
    pMat[12] = Ax;
    pMat[13] = Ay;
    pMat[14] = Az;
    pMat[15] = 1.0;
} 







void LookAt(float* pMat,aVECTOR* pPos,aVECTOR* pLookAt)
{
	float Length;
	aVECTOR Left,Up,Dir;


	Up.x = 0.0;
	Up.y = 1.0;
	Up.z = 0.0;

    Dir.x = pLookAt->x - pPos->x;
    Dir.y = pLookAt->y - pPos->y;
    Dir.z = pLookAt->z - pPos->z;

	MacroNormalize(Length,Dir.x,Dir.y,Dir.z);

	MacroCross(	Left.x,Left.y,Left.z,
				Up.x,Up.y,Up.z,
				Dir.x,Dir.y,Dir.z);
	MacroNormalize(Length,Left.x,Left.y,Left.z);

	MacroCross(	Up.x,Up.y,Up.z,
				Dir.x,Dir.y,Dir.z,
				Left.x,Left.y,Left.z);
	MacroNormalize(Length,Up.x,Up.y,Up.z);

 	pMat[0]  = Left.x;
 	pMat[1]  = Up.x;
 	pMat[2]  = Dir.x;
 	pMat[3]  = 0.0;

	pMat[4]  = Left.y;
	pMat[5]  = Up.y;
	pMat[6]  = Dir.y;
 	pMat[7]  = 0.0;

	pMat[8]  = Left.z;
	pMat[9]  = Up.z;
	pMat[10] = Dir.z;
 	pMat[11] = 0.0;

	MacroDot(pMat[12],pMat[0],pMat[4],pMat[8 ],pPos->x,pPos->y,pPos->z);
	MacroDot(pMat[13],pMat[1],pMat[5],pMat[9 ],pPos->x,pPos->y,pPos->z);
	MacroDot(pMat[14],pMat[2],pMat[6],pMat[10],pPos->x,pPos->y,pPos->z);

	pMat[12] = -pMat[12];
	pMat[13] = -pMat[13];
	pMat[14] = -pMat[14];
	pMat[15] = 1.0;
}
void FrustumMatrix( float* pMat,
					float l,float r,
					float t,float b,
					float n,float f)
{
	float aa,bb,w,h;


	w  =   2*n/(r-l);
	h  = -(2*n/(t-b));
	aa = (f/(f-n));
	bb = (-f*n)/(f-n);

 	pMat[0]  = w;
 	pMat[1]  = 0.0;
 	pMat[2]  = 0.0;
 	pMat[3]  = 0.0;
	pMat[4]  = 0.0;
	pMat[5]  = h;
	pMat[6]  = 0.0;
 	pMat[7]  = 0.0;
	pMat[8]  = 0.0;
	pMat[9]  = 0.0;
	pMat[10] = aa;
 	pMat[11] = 1.0;
	pMat[12] = 0.0;
	pMat[13] = 0.0;
	pMat[14] = bb;
	pMat[15] = 0.0;
}			 

void CreateOrthoMatrix(float* pMat)
{
 	pMat[0]  =  2.0/Render_WindowWidth;
 	pMat[1]  =  0.0;
 	pMat[2]  =  0.0;
 	pMat[3]  =  0.0;
	pMat[4]  =  0.0;
 	pMat[5]  = -2.0/Render_WindowHeight;
	pMat[6]  =  0.0;
 	pMat[7]  =  0.0;
	pMat[8]  =  0.0;
	pMat[9]  =  0.0;
	pMat[10] =  2.0/(1000);
 	pMat[11] =  0.0;
	pMat[12] = -1.0;
	pMat[13] =  1.0;
	pMat[14] =  0.0;
	pMat[15] =  1.0;
}
void CreateProjectionMatrix(float* pMat,float fov,float nearplane,float farplane)
{
    float fDesiredAspect;
    float fWindowAspect;
    float fAspectlr;
    float fAspecttb;
	float near_width = tan(fov*0.5)*nearplane;



	fDesiredAspect = 0.75;
	fWindowAspect  = Render_WindowWidth/Render_WindowHeight;

	if(fWindowAspect>fDesiredAspect)
	{
    	fAspectlr = fWindowAspect*fDesiredAspect;
    	fAspecttb = fDesiredAspect;
	}
	else
	{
    	fAspectlr  = 1.0;
    	fAspecttb  = (Render_WindowHeight/Render_WindowWidth)*1.0;
	}
	FrustumMatrix(	 pMat,
					-near_width * fAspectlr, 
					 near_width * fAspectlr, 
					-near_width * fAspecttb,
					 near_width * fAspecttb,
					 nearplane,farplane);
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
//Y,X,Z
float* CreateMatrixYawPitchRoll(float* pOut,float yaw, float pitch, float roll)
{
	float pMat1[16];
	float pMat2[16];
	float pMat3[16];
	float pMat4[16];

	CreateMatrixRotationX(pMat1,pitch);
	CreateMatrixRotationY(pMat2,yaw);
	CreateMatrixRotationZ(pMat3,roll);

//	MatrixMul(pMat4,pMat1,pMat2);//D3D
//	MatrixMul( pOut,pMat3,pMat4);//D3D

	MatrixMul(pMat4,pMat1,pMat2);//MS3D
	MatrixMul( pOut,pMat4,pMat3);//MS3D

//	MatrixMul(pMat4,pMat2,pMat1);
//	MatrixMul( pOut,pMat4,pMat3);

	return pOut;
}
//rotation based matrix mul
void MatrixMul(float* pDst,float* pA,float* pB)
{
	if(pDst==NULL)return;
	if(pA==NULL)return;
	if(pB==NULL)return;
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
//full 4x4 for use with projection matrix, and physics
void MatrixMul4x4(float* pDst,float* pA,float* pB)
{
	assert(pDst!=pA);//src, and dst cant be same. no a=a*b
	assert(pDst!=pB);

	pDst[0]  =  (pA[0]  * pB[0])  +  (pA[1]  * pB[4])  +  (pA[2]  * pB[8] )  +  (pA[3]  * pB[12]);
	pDst[1]  =  (pA[0]  * pB[1])  +  (pA[1]  * pB[5])  +  (pA[2]  * pB[9] )  +  (pA[3]  * pB[13]);
	pDst[2]  =  (pA[0]  * pB[2])  +  (pA[1]  * pB[6])  +  (pA[2]  * pB[10])  +  (pA[3]  * pB[14]);
	pDst[3]  =  (pA[0]  * pB[3])  +  (pA[1]  * pB[7])  +  (pA[2]  * pB[11])  +  (pA[3]  * pB[15]);
	pDst[4]  =  (pA[4]  * pB[0])  +  (pA[5]  * pB[4])  +  (pA[6]  * pB[8] )  +  (pA[7]  * pB[12]);
	pDst[5]  =  (pA[4]  * pB[1])  +  (pA[5]  * pB[5])  +  (pA[6]  * pB[9] )  +  (pA[7]  * pB[13]);
	pDst[6]  =  (pA[4]  * pB[2])  +  (pA[5]  * pB[6])  +  (pA[6]  * pB[10])  +  (pA[7]  * pB[14]);
	pDst[7]  =  (pA[4]  * pB[3])  +  (pA[5]  * pB[7])  +  (pA[6]  * pB[11])  +  (pA[7]  * pB[15]);
	pDst[8]  =  (pA[8]  * pB[0])  +  (pA[9]  * pB[4])  +  (pA[10] * pB[8] )  +  (pA[11] * pB[12]);
	pDst[9]  =  (pA[8]  * pB[1])  +  (pA[9]  * pB[5])  +  (pA[10] * pB[9] )  +  (pA[11] * pB[13]);
	pDst[10] =  (pA[8]  * pB[2])  +  (pA[9]  * pB[6])  +  (pA[10] * pB[10])  +  (pA[11] * pB[14]);
	pDst[11] =  (pA[8]  * pB[3])  +  (pA[9]  * pB[7])  +  (pA[10] * pB[11])  +  (pA[11] * pB[15]);
	pDst[12] =  (pA[12] * pB[0])  +  (pA[13] * pB[4])  +  (pA[14] * pB[8] )  +  (pA[15] * pB[12]);
	pDst[13] =  (pA[12] * pB[1])  +  (pA[13] * pB[5])  +  (pA[14] * pB[9] )  +  (pA[15] * pB[13]);
	pDst[14] =  (pA[12] * pB[2])  +  (pA[13] * pB[6])  +  (pA[14] * pB[10])  +  (pA[15] * pB[14]);
	pDst[15] =  (pA[12] * pB[3])  +  (pA[13] * pB[7])  +  (pA[14] * pB[11])  +  (pA[15] * pB[15]);
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
	MacroNormalize(Length,vU.x,vU.y,vU.z);

	MacroCross(	vV.x,vV.y,vV.z,
				vN.x,vN.y,vN.z,
				vU.x,vU.y,vU.z);
	MacroNormalize(Length,vV.x,vV.y,vV.z);

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

void CreateLookAtMatrixY(aVECTOR& Pos,
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

	pDst[4]  = vN.x;
	pDst[5]  = vN.y;
	pDst[6]  = vN.z;
 	pDst[7]  = 0.0; 

	pDst[8]  = -vV.x;
	pDst[9]  = -vV.y;
	pDst[10] = -vV.z;
 	pDst[11] = 0.0; 

	pDst[12] = Pos.x;
	pDst[13] = Pos.y;
	pDst[14] = Pos.z;
 	pDst[15] = 1.0; 
}





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

	pM[3]  = 0.0;
	pM[7]  = 0.0;
	pM[11] = 0.0;

	pM[12] = 0.0;
	pM[13] = 0.0;
	pM[14] = 0.0;
	pM[15] = 1.0;
}


bool IsMatrixIdentity(float* pM)
{
	return((pM[0]+pM[5]+pM[10]+pM[15])==4.0);
}




/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""aQUATERNION"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
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
    pC.x = pB.w*pA.x + pB.x*pA.w + pB.y*pA.z + pB.z*pA.y;
    pC.y = pB.w*pA.y + pB.y*pA.w + pB.z*pA.x + pB.x*pA.z;
    pC.z = pB.w*pA.z + pB.z*pA.w + pB.x*pA.y + pB.y*pA.x;
    pC.w = pB.w*pA.w + pB.x*pA.x + pB.y*pA.y + pB.z*pA.z;


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

void RotationQuaternionIdentity(aQUATERNION& Q)
{
	Q.x = 0.0;
	Q.y = 0.0;
	Q.z = 0.0;
	Q.w = 1.0;
}

void PositionQuaternionIdentity(aQUATERNION& Q)
{
	Q.x = 0.0;
	Q.y = 0.0;
	Q.z = 0.0;
	Q.w = 0.0;
}

//this works. no angle > pi.
//very intuit
void CreateRotationQuaternionFromMatrix(aQUATERNION& Q,float* pM)
{
	int next[3]={1,2,0};
	float Trace;
	float sqrtTrace;
	float oo2sqrtTrace;

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
void CreatePositionQuaternionFromMatrix(aQUATERNION& Q,float* pM)
{
	Q.x = pM[12];
	Q.y = pM[13];
	Q.z = pM[14];
	Q.w = 0.0;
}

void PositionQuaternion(aQUATERNION& Q,float PosX,float PosY,float PosZ)
{
	Q.x = PosX;
	Q.y = PosY;
	Q.z = PosZ;
	Q.w = 0.0;
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

void CreateAxisAngleFromMatrix(	aVECTOR& Axis,
								float& Angle,
								float* pM)
{
	float Length;
	float Trace;
	float sqrtTrace;

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



/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""aTEXTURE""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/

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


	//DiskFile=fopen("log.log","a");
	//fprintf(DiskFile,"LoadTexture %s \n",pFileName);
	//fclose(DiskFile);						


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
		//DiskFile=fopen("log.log","a");
		//fprintf(DiskFile,"Original File Not Found hbm = %d \n",hbm);
		//fclose(DiskFile);						

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


	//DiskFile=fopen("log.log","a");
	//fprintf(DiskFile,"LoadTexture %s \n",pFileName);
	//fclose(DiskFile);						


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
		//DiskFile=fopen("log.log","a");
		//fprintf(DiskFile,"Original File Not Found hbm = %d \n",hbm);
		//fclose(DiskFile);						


		sprintf(FullName,"default.bmp");
		AddPathToFileName(FullName);


		//DiskFile=fopen("log.log","a");
		//fprintf(DiskFile,"try to Load %s \n",FullName);
		//fclose(DiskFile);						

		hbm = (HBITMAP)LoadImage(NULL,FullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);


		//DiskFile=fopen("log.log","a");
		//fprintf(DiskFile,"hbm = %d \n",hbm);
		//fclose(DiskFile);						
		


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
			}
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
	else//normal load bitmap
	{
		GetObject(hbm,sizeof(BITMAP),&bmData);
		pSrc   = (unsigned char *)bmData.bmBits;

		//DiskFile=fopen("log.log","a");
		//fprintf(DiskFile,"Normal 1 \n");
		//fclose(DiskFile);						

		strcpy(pTexture.pName,FullName);
		pTexture.Width  = bmData.bmWidth;
		pTexture.Height = bmData.bmHeight;
		pTexture.pChar  = (unsigned char *)calloc(pTexture.Width*pTexture.Height*3,sizeof(unsigned char));
		pTexture.flags  = flags;

		//DiskFile=fopen("log.log","a");
		//fprintf(DiskFile,"Normal 2 \n");
		//fclose(DiskFile);						

		//local copies for readability
		width  = pTexture.Width; 
		height = pTexture.Height;
		pDst   = pTexture.pChar;
		for(j=0;j<height;j++)
		{
			memcpy(&pDst[j*width*3],&pSrc[(height-j-1)*bmData.bmWidthBytes],width*3*sizeof(char));
		}
		//DiskFile=fopen("log.log","a");
		//fprintf(DiskFile,"Normal 3 \n");
		//fclose(DiskFile);						

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
	SAFE_FREE(pTexture->pChar);
	SAFE_FREE(pTexture->pAlpha);
	pTexture->pAPITEXTURE = NULL;//<- just a pointer, no alloced mem.
	SAFE_FREE(pTexture->pWord);
	//memset(pTexture,0,sizeof(aTEXTURE));
}




/*
void ()//___________________________________________________________________________________________________________________________
void ()//"""""""""EXTRA TOOLS""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void ()//___________________________________________________________________________________________________________________________
*/












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
		flags-=(flags&flag);
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


void ClearBit(unsigned char& flags,unsigned char flag)
{
	if(flags&flag)
	{
		flags-=(flags&flag);
	}
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





//untested
unsigned long GetSecondsLong(bool reset)
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



bool IntesectSphere(	aVECTOR& 	RayPos,
						aVECTOR& 	RayDir,
						aVECTOR& 	Center,
						double		Radius,
						double		*rin,
						double		*rout)
{
	double x,y,z,a,b,c,root;

	x = RayPos.x-Center.x;
	y = RayPos.y-Center.y;
	z = RayPos.z-Center.z;
	a = (x*RayDir.x)+(y*RayDir.y)+(z*RayDir.z); 
	b = (x*x)+(y*y)+(z*z)-(Radius*Radius);
	c = (a*a)-b;
  
	if(c >= 0.0)
	{
	    root  =  sqrt(c);
	    *rin  = -a - root;
	    *rout = -a + root;
	    return true;
	}
	else
	{
    	return false;
	}  
}



/*
desc : mouse selection of objects, and meshes
work in progress.
bool Pick()
{
	struct SceneLL*  pScene  = NULL;
	struct ObjectLL* pObject = NULL;
	struct MeshLL* 	 pMesh   = NULL;
	VERTEX_XYZNRGBUV* pLocalVerts;
	VERTEX_XYZNRGBUV* pWorldVerts;
	WORD* pIndices;
	aVECTOR v0;
	aVECTOR v1;
	aVECTOR v2;
	aVECTOR Pos;
	aVECTOR Dir;
	float m_fPickT,m_fPickU,m_fPickV;
	long i;
	bool bIntersection = false;

	pScene = pSceneHead;//<-BUGBUG only renders first Scene
	if(pScene)
	{
		pObject = pScene->pObjectHead;
		while(pObject)
		{
			pMesh = pObject->pMeshHead;
			while(pMesh)	 
			{
//!!!!!!!!!!!!!
see PaintWeights(), in app.cpp 
for required upgrade to CreatePickRay()
//
				CreatePickRay(pObject,pMesh,Pos,Dir);
				if(pMesh->RenderFlags & RENDER_SELECTABLE)
				{
					pLocalVerts = pMesh->pLocalVerts;
					pWorldVerts = pMesh->pWorldVerts;
					pIndices    = pMesh->pIndices;
		        	for(i=0;i<pMesh->num_tris;i++)
		        	{

						//a=Oltow*Mltow
						//a-1*c-1

		        	    v0.x = pLocalVerts[pIndices[3*i+0]].x;
		        	    v0.y = pLocalVerts[pIndices[3*i+0]].y;
		        	    v0.z = pLocalVerts[pIndices[3*i+0]].z;

		        	    v1.x = pLocalVerts[pIndices[3*i+2]].x;
		        	    v1.y = pLocalVerts[pIndices[3*i+2]].y;
		        	    v1.z = pLocalVerts[pIndices[3*i+2]].z;

		        	    v2.x = pLocalVerts[pIndices[3*i+1]].x;
		        	    v2.y = pLocalVerts[pIndices[3*i+1]].y;
		        	    v2.z = pLocalVerts[pIndices[3*i+1]].z;
		        	    if( IntersectTriangle( Pos,Dir, v0, v1, v2,
		        	                           &m_fPickT, &m_fPickU, &m_fPickV ) )
		        	    {
							char text[512];
							sprintf(text,"Pick %s\n",pMesh->pName);
							OutputDebugString(text);	

//							pCurrentlyPickedScene  = pScene;
//							pCurrentlyPickedObject = pObject;
//							pCurrentlyPickedMesh   = pMesh;

							//pSelectedObject = pObject;
							//pSelectedMesh   = pMesh;
						}
					}//End for i<num_tris
				}//End if not NOPICK
				pMesh = pMesh->pNext;
			}//End while(pMesh)
			pObject = pObject->pNext;
		}//End while(pObject)
	}//End while(pScene)
	return(bIntersection);
}
*/





//returns cam position, and ray direction based on curso pos
//Dir IS NOT NORMALIZED!!!!!!!!!!!!!
void CreatePickRay(	struct ObjectLL* pParentObject,
					struct MeshLL* pMesh,
					POINT ptCursor,
					aVECTOR& Pos,aVECTOR& Dir)
{
	aVECTOR v;
//	POINT ptCursor;
	float pInverseView[16];
//	float pModelTemp[16];
	float pInverseModel[16];
	float pModelFinal[16];
	float pInverseFinal[16];
//	float pLtoWFinal[16];


//    GetCursorPos(&ptCursor);
//    ScreenToClient(System_SavehWnd,&ptCursor);

	v.x =  (((2.0*ptCursor.x)/Render_WindowWidth )-1.0)/pRender_ProjMatrix[0];
	v.y = -(((2.0*ptCursor.y)/Render_WindowHeight)-1.0)/pRender_ProjMatrix[5];
	v.z =  1.0;

	//ToDo!!!! allow picking obj/mesh between keyframes.

	//Inverse view matrix to get world matrix defining camera
	InverseMatrix(pInverseView,pRender_ViewMatrix);

	//concat obj, and mesh matrices. pModelFinal defines local to world
	MatrixMul(pModelFinal,pParentObject->pMatrix,pMesh->pMatrix);

	//pInverseModel defines world to local
	InverseMatrix(pInverseModel,pModelFinal);
	
	//pInverseFinal is the camera in local coordinates. The relationship between camera, and mesh are the same.
	//The camera has been inverse transformed into local space so that local vertices can be processed, avoiding LtoW transform for each vertex.
	//The resulting local coord camera will be different for each mesh given their obj, and msh matrices.
	MatrixMul(pInverseFinal,pInverseView,pInverseModel);

	Dir.x  = v.x*pInverseFinal[0] + v.y*pInverseFinal[4] + v.z*pInverseFinal[8 ];
	Dir.y  = v.x*pInverseFinal[1] + v.y*pInverseFinal[5] + v.z*pInverseFinal[9 ];
	Dir.z  = v.x*pInverseFinal[2] + v.y*pInverseFinal[6] + v.z*pInverseFinal[10];

	Pos.x = pInverseFinal[12];
	Pos.y = pInverseFinal[13];
	Pos.z = pInverseFinal[14];
}













