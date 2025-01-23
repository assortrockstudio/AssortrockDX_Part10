#pragma once
#include "EditorUI.h"


class MenuUI :
    public EditorUI
{
private:
    

public:
    void File();
    void GameObject();
    void Level();
    void Asset();
    void RenderTarget();

private:
    wstring GetAssetDefaultName(wstring _BaseName);
    void RenderTargetFilter(const string& _MenuItemName, const wstring& _TargetTexKey);

public:
    virtual void tick() override;
    virtual void render_tick() override;

public:
    MenuUI();
    ~MenuUI();
};

