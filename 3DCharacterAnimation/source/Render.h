
#if !defined(AFX_RENDER_H__CD358130_E293_4914_88C4_9C75B0C7CCBA__INCLUDED_)
#define AFX_RENDER_H__CD358130_E293_4914_88C4_9C75B0C7CCBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "library.h"

void Render_Init(char* pApplicationName);
bool Render_Create(	HINSTANCE hInstance,
					WNDPROC App_WndProc);
void Render_BeginScene();
void Render_EndScene();
void Render_Mesh(struct SceneLL* pScene,
				struct ObjectLL* pObject,
				struct MeshLL* pMesh);

void Render_ReSizeWindow();
void Render_Delete();

#endif // !defined(AFX_RENDER_H__CD358130_E293_4914_88C4_9C75B0C7CCBA__INCLUDED_)
