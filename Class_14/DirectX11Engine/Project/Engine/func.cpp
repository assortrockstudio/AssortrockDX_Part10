#include "pch.h"

#include "CDbgRenderMgr.h"
#include "CTaskMgr.h"

#include "CGameObject.h"

void SpawnObject(int _LayerIdx, CGameObject* _Object)
{
	tTask task = {};
	task.Type = TASK_TYPE::SPAWN_OBJECT;
	task.dwParam_0 = _LayerIdx;
	task.dwParam_1 = (DWORD_PTR)_Object;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextLevelState)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.dwParam_0 = (DWORD_PTR)_NextLevel;
	task.dwParam_1 = (DWORD_PTR)_NextLevelState;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevelState(LEVEL_STATE _NextLevelState)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	task.dwParam_0 = (DWORD_PTR)_NextLevelState;;	

	CTaskMgr::GetInst()->AddTask(task);
}

void DeleteAsset(Ptr<CAsset> _Asset)
{
	tTask task = {};
	task.Type = TASK_TYPE::DEL_ASSET;
	task.dwParam_0 = (DWORD_PTR)_Asset.Get();

	CTaskMgr::GetInst()->AddTask(task);
}

bool IsValid(CGameObject*& _Object)
{
	if (nullptr == _Object)
	{		
		return false;
	}
	else if (_Object->IsDead())
	{
		_Object = nullptr;
		return false;
	}
	else
	{
		return true;
	}
}



void DrawDebugRect(Vec3 _WorldPos, Vec3 _WorldScale, Vec3 _WorldRotation, Vec4 _vColor, float _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::RECT;

	info.Position = _WorldPos;
	info.Scale = _WorldScale;
	info.Rotation = _WorldRotation;
	info.matWorld = XMMatrixIdentity();
	info.Color = _vColor;
	info.Duration = _Duration;
	info.Age = 0.f;
	info.DepthTest = _DepthTest;

	CDbgRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugRect(Matrix _matWorld, Vec4 _vColor, float _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};

	info.Shape = DEBUG_SHAPE::RECT;
	info.matWorld = _matWorld;
	info.Color = _vColor;
	info.Duration = _Duration;
	info.Age = 0.f;
	info.DepthTest = _DepthTest;

	CDbgRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(Vec3 _WorldPos, float _Radius, Vec4 _vColor, float _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::CIRCLE;

	info.Position = _WorldPos;
	info.Scale = Vec3(_Radius * 2.f, _Radius * 2.f, 1.f);	
	info.matWorld = XMMatrixIdentity();

	info.Color = _vColor;
	info.Duration = _Duration;
	info.Age = 0.f;
	info.DepthTest = _DepthTest;

	CDbgRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCube(Vec3 _WorldPos, Vec3 _WorldScale, Vec3 _WorldRotation, Vec4 _vColor, float _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::CUBE;

	info.Position = _WorldPos;
	info.Scale = _WorldScale;
	info.Rotation = _WorldRotation;
	info.matWorld = XMMatrixIdentity();
	info.Color = _vColor;
	info.Duration = _Duration;
	info.Age = 0.f;
	info.DepthTest = _DepthTest;

	CDbgRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCube(Matrix _matWorld, Vec4 _vColor, float _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};

	info.Shape = DEBUG_SHAPE::CUBE;
	info.matWorld = _matWorld;
	info.Color = _vColor;
	info.Duration = _Duration;
	info.Age = 0.f;
	info.DepthTest = _DepthTest;

	CDbgRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugSphere(Vec3 _WorldPos, float _Radius, Vec4 vColor, bool _DepthTest, float Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::SPHERE;

	info.Position = _WorldPos;
	info.Scale = Vec3(_Radius * 2.f, _Radius * 2.f, _Radius * 2.f);
	info.matWorld = XMMatrixIdentity();

	info.Color = vColor;
	info.Duration = Duration;
	info.Age = 0.f;

	info.DepthTest = _DepthTest;

	CDbgRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

string ToString(const wstring& _str)
{
	return string(_str.begin(), _str.end());
}

wstring ToWString(const string& _str)
{
	return wstring(_str.begin(), _str.end());
}

void SaveWString(const wstring& _Str, FILE* _File)
{
	size_t len = _Str.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_Str.c_str(), sizeof(wchar_t), len, _File);
}

void LoadWString(wstring& _str, FILE* _File)
{
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, _File);
	_str.resize(len);
	fread((wchar_t*)_str.c_str(), sizeof(wchar_t), len, _File);
}
