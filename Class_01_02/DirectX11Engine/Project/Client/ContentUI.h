#pragma once
#include "EditorUI.h"

class TreeUI;

class ContentUI :
    public EditorUI
{
private:
    TreeUI*             m_Tree;
    vector<wstring>     m_vecContentName;   // Content 폴더에 있는 에셋들의 상대경로

public:
    // Renew : 갱신, Reset : 초기 상태로 되돌림
    void RenewContent();
    void ReloadContent();

private:
    void FindFileName(const wstring& _strFolderPath);
    UINT SelectAsset(DWORD_PTR _Data);
    ASSET_TYPE GetAssetTypeFromExt(const path& _AssetName);


public:    
    virtual void render_tick() override;

public:
    ContentUI();
    ~ContentUI();
};

