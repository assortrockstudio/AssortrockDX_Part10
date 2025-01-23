#pragma once
#include "EditorUI.h"

class AssetUI :
    public EditorUI
{
private:
    Ptr<CAsset>             m_TargetAsset;
    const ASSET_TYPE        m_Type;

public:
    void SetTarget(Ptr<CAsset> _Target);
    Ptr<CAsset> GetTarget() { return m_TargetAsset; }
    ASSET_TYPE GetAssetType() { return m_Type; }

    // AssetUI 가 보여주는 대상이 변경된 경우
    virtual void TargetChanged() {};

public:
    virtual void render_tick() = 0;

protected:
    void render_title();

public:
    AssetUI(const string& _Name, const string& _ID, ASSET_TYPE _Type);
    ~AssetUI();
};

