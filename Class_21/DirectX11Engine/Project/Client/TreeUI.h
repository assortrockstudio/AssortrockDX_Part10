#pragma once
#include "EditorUI.h"

class TreeUI;

class TreeNode
{
private:
    static  UINT    g_ID;

private:
    TreeUI*             m_Owner;
    TreeNode*           m_Parent;
    vector<TreeNode*>   m_vecChildNode;
    string              m_Name;
    string              m_ID;

    DWORD_PTR           m_Data;
    
    bool                m_bFrame;
    bool                m_bSelected;
    

public:
    void SetName(const string& _Name) { m_Name = _Name; }
    void SetData(DWORD_PTR _Data) { m_Data = _Data; }
    void SetFrame(bool _Frame) { m_bFrame = _Frame; }

    const string& GetName() { return m_Name; }
    DWORD_PTR GetData() { return m_Data; }

    void AddChildNode(TreeNode* _Child)
    {
        m_vecChildNode.push_back(_Child);
        _Child->m_Parent = this;
    }
    const vector<TreeNode*>& GetChildNode() { return m_vecChildNode; }



public:
    void render_tick();

public:
    TreeNode()
        : m_Owner(nullptr)
        , m_Parent(nullptr)
        , m_Data(0)
        , m_bFrame(false)
        , m_bSelected(false)
    {
        char buff[50] = {};
        _itoa_s(g_ID++, buff, 10);
        m_ID = buff;
    }

    TreeNode(const string& _Name, DWORD_PTR _Data)
        : m_Owner(nullptr)
        , m_Parent(nullptr)
        , m_Name(_Name)
        , m_Data(_Data)
        , m_bFrame(false)
        , m_bSelected(false)
    {
        char buff[50] = {};
        _itoa_s(g_ID++, buff, 10);
        m_ID = buff;
    }

    ~TreeNode();

    friend class TreeUI;
};



class TreeUI :
    public EditorUI
{
private:
    TreeNode*       m_RootNode;
    TreeNode*       m_SelectedNode;

    TreeNode*       m_DragNode;
    TreeNode*       m_DropNode;

    bool            m_bShowRoot;
    bool            m_bShowFileName;
    bool            m_bUseDrag;
    bool            m_bUseDrop;

    UI_CALLBACK     m_SelectedCallBack;

    EditorUI*       m_SelectedInst;
    UI_DELEGATE1    m_SelectedDelegate;

    EditorUI*       m_DragDropInst;
    UI_DELEGATE2    m_DragDropDelegate;



public:
    void ShowRoot(bool _Show) { m_bShowRoot = _Show; }
    void ShowFileNameOnly(bool _ShowFileName) { m_bShowFileName = _ShowFileName; }

    void UseDrag(bool _Drag) { m_bUseDrag = _Drag; }
    void UseDragDrop(bool _dragDrop) { m_bUseDrag = _dragDrop; m_bUseDrop = _dragDrop;}

    TreeNode* AddTreeNode(TreeNode* _Parent, const string& _NodeName, DWORD_PTR _dwData = 0);

    void RegisterSelectCallBack(UI_CALLBACK _CallBack) { m_SelectedCallBack = _CallBack; }
    void RegisterSelectDelegate(EditorUI* _Inst, UI_DELEGATE1 _MemFunc) { m_SelectedInst = _Inst; m_SelectedDelegate = _MemFunc;}
    void RegisterDragDropDelegate(EditorUI* _Inst, UI_DELEGATE2 _MemFunc) { m_DragDropInst = _Inst; m_DragDropDelegate = _MemFunc;};

    void Clear()
    {
        if (nullptr != m_RootNode)
        {
            delete m_RootNode;
            m_RootNode = nullptr;
        }         
    }

private:
    void SetSelectedNode(TreeNode* _Node);
    void SetDragNode(TreeNode* _Node) { m_DragNode = _Node; }
    void SetDropNode(TreeNode* _Node) { m_DropNode = _Node; }

public:
    virtual void render_tick() override;

public:
    TreeUI(const string& _Name);
    ~TreeUI();

    friend class TreeNode;
};

