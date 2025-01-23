#include "pch.h"
#include "TMESub.h"

#include "AtlasView.h"

TMESub::TMESub(const string& _Name, const string& _ID)
	: EditorUI(_Name, _ID)
	, m_Owner(nullptr)
	, m_TileMap(nullptr)
{
}

TMESub::~TMESub()
{
}

AtlasView* TMESub::GetAtlasView()
{
	return (AtlasView*)m_Owner->GetSubUI(ATLASVIEW);
}

TileView* TMESub::GetTileView()
{
	return (TileView*)m_Owner->GetSubUI(TILEVIEW);
}

TileMapDetail* TMESub::GetDetailView()
{
	return (TileMapDetail*)m_Owner->GetSubUI(DETAIL);
}

void TMESub::SetTileMap(CTileMap* _TileMap)
{
	if (m_TileMap)
	{
		m_TileMap->SetCapture(false);
	}

	m_TileMap = _TileMap;

	if (m_TileMap)
	{
		m_TileMap->SetCapture(true);
	}
}
