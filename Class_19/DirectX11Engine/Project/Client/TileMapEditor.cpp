#include "pch.h"
#include "TileMapEditor.h"

#include "CImGuiMgr.h"
#include "TMESub.h"

#include "TileView.h"
#include "AtlasView.h"
#include "TileMapDetail.h"

TileMapEditor::TileMapEditor()
	: EditorUI("TileMapEditor", "##TileMapEditor")
	, m_Sub{}
	, m_PrevActive{}
	, m_TileMap(nullptr)
{
	m_Sub[TILEVIEW]  = CImGuiMgr::GetInst()->FindEditorUI<TileView>("TileView");
	m_Sub[ATLASVIEW] = CImGuiMgr::GetInst()->FindEditorUI<AtlasView>("AtlasView");
	m_Sub[DETAIL]	 = CImGuiMgr::GetInst()->FindEditorUI<TileMapDetail>("Detail");

	for (int i = 0; i < 3; ++i)
	{
		m_Sub[i]->SetActive(true);
		m_Sub[i]->m_Owner = this;
	}
}

TileMapEditor::~TileMapEditor()
{

}

void TileMapEditor::SetTileMap(CTileMap* _TileMapComponent)
{
	m_TileMap = _TileMapComponent;

	for (int i = 0; i < 3; ++i)
	{
		m_Sub[i]->SetTileMap(_TileMapComponent);
	}	
}

void TileMapEditor::render_tick()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File##TileMapEditor"))
		{


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View##TileMapEditor"))
		{
			bool IsActive[3] = { m_Sub[0]->IsActive() , m_Sub[1]->IsActive() , m_Sub[2]->IsActive()};		

			if (ImGui::MenuItem("TileView##TileMapEditor", nullptr, &IsActive[TILEVIEW]))
			{
				m_Sub[TILEVIEW]->SetActive(IsActive[TILEVIEW]);
			}

			if (ImGui::MenuItem("AtlasView##TileMapEditor", nullptr, &IsActive[ATLASVIEW]))
			{
				m_Sub[ATLASVIEW]->SetActive(IsActive[ATLASVIEW]);
			}

			if (ImGui::MenuItem("Detail##TileMapEditor", nullptr, &IsActive[DETAIL]))
			{
				m_Sub[DETAIL]->SetActive(IsActive[DETAIL]);
			}

			ImGui::EndMenu();
		}



		ImGui::EndMenuBar();
	}
}

void TileMapEditor::Activate()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_PrevActive[i])
			m_Sub[i]->SetActive(true);
	}
}

void TileMapEditor::Deactivate()
{
	for (int i = 0; i < 3; ++i)
	{
		m_PrevActive[i] = m_Sub[i]->IsActive();
		m_PrevActive[i] = m_Sub[i]->SetActive(false);
	}
}