#pragma once
#include "EditorUI.h"

class TreeUI;
class TreeNode;

class OutlinerUI :
    public EditorUI
{
private:
    TreeUI* m_Tree;

public:
    // Renew : 갱신, Reset : 초기 상태로 되돌림
    void RenewGameObject();


public:
    virtual void render_tick() override;

private:
    void AddGameObject(TreeNode* _Node, CGameObject* _Object);
    UINT SelectGameObject(DWORD_PTR _Data);

    UINT DragDropFunc(DWORD_PTR _DragNode, DWORD_PTR _DropNode);


public:
    OutlinerUI();
    ~OutlinerUI();
};

