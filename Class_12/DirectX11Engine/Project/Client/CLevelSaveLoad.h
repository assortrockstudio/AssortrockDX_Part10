#pragma once


class CLevelSaveLoad
{
public:
	static void SaveLevel(CLevel* _Level, const wstring& _FilePath);
	static void SaveGameObject(CGameObject* _Object, FILE* _File);

	static CLevel* LoadLevel(const wstring& _FilePath);
	static CGameObject* LoadGameObject(FILE* _File);
};

