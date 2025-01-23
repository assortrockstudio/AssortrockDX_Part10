#include "pch.h"
#include "TileMapDetail.h"

#include "ListUI.h"

TileMapDetail::TileMapDetail()
	: TMESub("Detail", "##TileMapDetail")
{
}

TileMapDetail::~TileMapDetail()
{
}

void TileMapDetail::render_tick()
{
	CTileMap* pTileMap = GetTileMap();

	if (nullptr == pTileMap)
		return;

	int row_col[2] = { pTileMap->GetRow(), pTileMap->GetColumn() };

	// Tile Row - Column
	ImGui::Text("Row");	   
	ImGui::SameLine(60); 
	ImGui::SetNextItemWidth(40);
	ImGui::InputInt("##TileMapRow", &row_col[0], 0);

	ImGui::Text("Column"); 
	ImGui::SameLine(60); 
	ImGui::SetNextItemWidth(40);
	ImGui::InputInt("##TileMapCol", &row_col[1], 0);

	// Tile Each Size
	Vec2 vTileSize = pTileMap->GetTileSize();
	float TileSize[2] = { vTileSize.x, vTileSize.y };

	ImGui::Text("Tile Size");
	ImGui::SameLine(100);	
	ImGui::InputFloat2("##TileEachSize", TileSize);
	
	// Atlas Texture
	ImGui::Separator();
	Ptr<CTexture> pAtlas = pTileMap->GetAtlas();
	string AtlasName;
	if (nullptr != pAtlas)
	{
		AtlasName = ToString(pAtlas->GetKey());
	}

	ImGui::Text("Atals");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##TileMapDetail_Atlas", (char*)AtlasName.c_str(), AtlasName.capacity(), ImGuiInputTextFlags_ReadOnly);

	// ListUI 버튼 추가	
	ImGui::SameLine();
	if (ImGui::Button("##TileMapDetail_AtlasBtn", ImVec2(18, 18)))
	{
		// ListUI 를 활성화한다
		ListUI* pListUI = CImGuiMgr::GetInst()->FindEditorUI<ListUI>("List");

		if (nullptr != pListUI && !pListUI->IsActive())
		{
			pListUI->SetModal(true);
			pListUI->SetActive(true);
			pListUI->SetFocus();

			vector<string> vecNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecNames);
			pListUI->AddItem(vecNames);

			pListUI->RegisterDBClickDelegate(this, (UI_DELEGATE1) & TileMapDetail::SelectTexture);
		}
	}

	// Image 출력
	ImTextureID TexID = (nullptr == pAtlas) ? nullptr : pAtlas->GetSRV().Get();
	ImVec4 BorderColor = (nullptr == pAtlas) ? ImVec4(1.f, 1.f, 1.f, 1.f) : ImVec4(1.f, 1.f, 1.f, 0.f);

	ImGui::Image(TexID
		, ImVec2(80.f, 80.f)
		, ImVec2(0.0f, 0.0f)
		, ImVec2(1.0f, 1.0f)
		, ImGui::GetStyleColorVec4(ImGuiCol_Text)
		, BorderColor);

	// Atlas Tile Size
	Vec2 vAtlasTileSize = pTileMap->GetAtlasTileSize();
	TileSize[0] = vAtlasTileSize.x; TileSize[1] = vAtlasTileSize.y;

	ImGui::Text("Atals TileSize");
	ImGui::SameLine(150);
	if (ImGui::InputFloat2("##AtlasTileEachSize", TileSize))
	{
		pTileMap->SetAtlasTileSize(Vec2(TileSize[0], TileSize[1]));
	}

	// Read-Olny Infomation
	// Tile Row - Column
	row_col[0] = pTileMap->GetAtlasMaxRow();
	row_col[1] = pTileMap->GetAtlasMaxColumn();

	ImGui::Text("Atlas Max Row");
	ImGui::SameLine(150);
	ImGui::SetNextItemWidth(40);
	ImGui::InputInt("##AtlasMaxRow", &row_col[0], 0, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Atlas Max Column");
	ImGui::SameLine(150);
	ImGui::SetNextItemWidth(40);
	ImGui::InputInt("##AtlasMaxCol", &row_col[1], 0, ImGuiInputTextFlags_ReadOnly);
	
	// Tile Each Size
	Vec2 vResolution = nullptr == pAtlas ? Vec2(0.f, 0.f) : Vec2(pAtlas->GetWidth(), pAtlas->GetHeight());
	float Resol[2] = { vResolution.x, vResolution.y };

	ImGui::Text("Atlas Resolution");
	ImGui::SameLine(150);
	ImGui::InputFloat2("##AtlasResolution", Resol, "%.f");
}

UINT TileMapDetail::SelectTexture(DWORD_PTR _Selected)
{
	string* pStr = (string*)_Selected;

	wstring Key = ToWString(*pStr);

	Ptr<CTexture> pAsset = CAssetMgr::GetInst()->FindAsset<CTexture>(Key);

	assert(nullptr != GetTileMap() && nullptr != pAsset);

	GetTileMap()->SetAtlasTexture(pAsset);

	SetFocus();

	return 0;
}
