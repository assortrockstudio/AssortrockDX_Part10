#pragma once


#include "CImGuiMgr.h"
#include "EditorUI.h"

class ParamUI
{
private:
	static UINT	g_ParmUIID;
	static int g_DescWidth;

	static EditorUI*		g_Inst;
	static UI_DELEGATE1		g_Delegate_1;

public:
	static void ResetParamID() { g_ParmUIID = 0; }
	static void SetDescWidth(int _DescWidth) { g_DescWidth = _DescWidth; }
	static void RegisterTexDelegate(EditorUI* _UI, UI_DELEGATE1 _Delegate) { g_Inst = _UI; g_Delegate_1 = _Delegate; }

public:
	static int Param_DragInt(const string& _strName, int* _pInOut, int _Speed = 1);
	static int Param_DragFloat(const string& _strName, float* _pInOut, float _Speed = 1.f);
	static int Param_DragVec2(const string& _strName, Vec2* _pInOut, float _Speed = 1.f);
	static int Param_DragVec4(const string& _strName, Vec4* _pInOut, float _Speed = 1.f);
	static int Param_DragMat(const string& _strName, Matrix* _pInOut, float _Speed = 1.f);

	static int Param_Tex(const string& _strName, Ptr<CTexture>& _Tex);
};

