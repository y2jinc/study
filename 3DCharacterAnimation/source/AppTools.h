//AppTools.h
#ifndef _AppTools_H_
#define _AppTools_H_

void CreateFontTexture(FontObject* pFont);
bool PickThisMatrix(float* pMatrix);
bool Mouse_Rotate_CurrentMatrix(float* pMatrix);
bool Mouse_Move_CurrentMatrix(float* pMatrix);
void RefreshPointer2(struct MeshLL* pMesh);
void CreateVectorGeometry(struct MeshLL* pMesh,int VectorType);
void CreateBoneGeometry(struct MeshLL* pMesh);

#endif//_AppTools_H_
