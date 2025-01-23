#pragma once

class CGameObject;
void SpawnObject(int _LayerIdx, CGameObject* _Object);

class CLevel;
void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextLevelState);
void ChangeLevelState(LEVEL_STATE _NextLevelState);

class CAsset;
void DeleteAsset(Ptr<CAsset> _Asset);


bool IsValid(CGameObject*& _Object);


void DrawDebugRect(Vec3 _WorldPos, Vec3 _WorldScale, Vec3 _WorldRotation, Vec4 _vColor, float _DepthTest, float _Duration);
void DrawDebugRect(Matrix _matWorld, Vec4 _vColor, float _DepthTest, float _Duration);

void DrawDebugCircle(Vec3 _WorldPos, float _Radius, Vec4 _vColor, float _DepthTest, float _Duration);

void DrawDebugCube(Vec3 _WorldPos, Vec3 _WorldScale, Vec3 _WorldRotation, Vec4 _vColor, float _DepthTest, float _Duration);
void DrawDebugCube(Matrix _matWorld, Vec4 _vColor, float _DepthTest, float _Duration);

void DrawDebugLine();

void DrawDebugSphere(Vec3 _WorldPos, float _Radius, Vec4 vColor, bool DepthTest, float Duration);




string ToString(const wstring& _str);
wstring ToWString(const string& _str);

void SaveWString(const wstring& _Str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

template<typename T>
class Ptr;

template<typename T>
inline void SaveAssetRef(_In_ Ptr<T> _Asset, FILE* _File)
{
	bool bUse = _Asset.Get();
	fwrite(&bUse, sizeof(bool), 1, _File);

	if (bUse)
	{
		SaveWString(_Asset->GetKey(), _File);
		SaveWString(_Asset->GetRelativePath(), _File);
	}
}

#include "CAssetMgr.h"

template<typename T>
inline void LoadAssetRef(_Out_ Ptr<T>& _Asset, FILE* _File)
{
	bool bUse = 0;
	fread(&bUse, sizeof(bool), 1, _File);

	if (bUse)
	{
		wstring strKey, strRelativePath;

		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		_Asset = CAssetMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}





template<typename T, int _Size>
void Safe_Del_Array(T* (&Array)[_Size])
{
	for (int i = 0; i < _Size; ++i)
	{
		if (nullptr != Array[i])
		{
			delete Array[i];
			Array[i] = nullptr;
		}
	}
}

template<typename T>
void Safe_Del_Vector(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}

	_vec.clear();
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		if (nullptr != pair.second)
			delete pair.second;
	}

	_map.clear();
}