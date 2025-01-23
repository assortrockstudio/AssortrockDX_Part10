#include "pch.h"
#include "TileMapUI.h"

#include "CImGuiMgr.h"
#include "TileMapEditor.h"


TileMapUI::TileMapUI()
	: ComponentUI("TileMap", "##TileMapUI", COMPONENT_TYPE::TILEMAP)
{
}

TileMapUI::~TileMapUI()
{
}


void TileMapUI::render_tick()
{
	render_title();

	ImVec2 vSize = ImGui::CalcTextSize("Open Editor");
	vSize.x += 6.f;
	vSize.y += 5.f;

	if (ImGui::Button("Open Editor##OpenTileMapEditor", vSize))
	{
		TileMapEditor* pEditor = CImGuiMgr::GetInst()->FindEditorUI<TileMapEditor>("TileMapEditor");
		pEditor->SetTileMap(GetTarget()->TileMap());
		pEditor->SetActive(true);		
	}
}