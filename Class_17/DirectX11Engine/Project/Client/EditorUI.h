#pragma once

#include "ImGui/imgui.h"
#include "CImGuiMgr.h"

class EditorUI
{
private:
	string				m_Name;
	const string		m_ID;

	EditorUI*			m_ParentUI;		// Editor UI 의 부모UI 를 가리킴
	vector<EditorUI*>	m_vecChildUI;	// Editor UI 가 소유하고 있는 자식 UI 들

	ImVec2				m_vChildSize;

	bool				m_Menu;
	bool				m_Active;
	bool				m_Seperate;
	bool				m_Modal;
	bool				m_Move;

public:
	void SetName(const string& _Name) { m_Name = _Name; }
	const string& GetName() { return m_Name; }

	void SetSize(ImVec2 _Size) { m_vChildSize = _Size; }
	void SetSeperate(bool _Seperate) { m_Seperate = _Seperate; }
	bool SetActive(bool _Active);
	void SetModal(bool _Modal) { m_Modal = _Modal; }
	void SetMenu(bool _Menu) { m_Menu = _Menu; }
	void SetUIMove(bool _Move) { m_Move = _Move; }


	EditorUI* GetParentUI() { return m_ParentUI; }
	const vector<EditorUI*>& GetChildUI() { return m_vecChildUI; }

	void AddChildUI(EditorUI* _Child) 
	{ 
		_Child->m_ParentUI = this;
		m_vecChildUI.push_back(_Child); 
	}
	
	void SetFocus();

	bool IsActive() { return m_Active; }
	bool IsRootUI() { return !m_ParentUI; }

public:
	virtual void tick();
	virtual void render_tick() = 0;

	virtual void Activate() {}
	virtual void Deactivate() {}


public:	
	EditorUI(const string& _Name, const string& _ID);
	virtual ~EditorUI();
};

