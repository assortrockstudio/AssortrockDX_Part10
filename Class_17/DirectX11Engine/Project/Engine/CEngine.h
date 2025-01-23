#pragma once

class CEngine
	: public CSingleton<CEngine>
{
	SINGLE(CEngine)
private:
	HWND		m_hMainHwnd;
	Vec2		m_Resolution;

public:
	HWND GetMainWnd() { return m_hMainHwnd; }
	int init(HWND _hWnd, Vec2 _Resolution, PREFAB_SAVE_FUNC _Save, PREFAB_LOAD_FUNC _Load);
	void progress();
};

