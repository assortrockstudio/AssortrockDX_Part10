#include "pch.h"
#include "MeshUI.h"

MeshUI::MeshUI()
	: AssetUI("MeshUI", "##MeshUI", ASSET_TYPE::MESH)
{
}

MeshUI::~MeshUI()
{
}

void MeshUI::render_tick()
{
	render_title();

	string strName = ToString(GetTarget()->GetKey());

	Ptr<CMesh> pMesh = dynamic_cast<CMesh*>(GetTarget().Get());
	assert(pMesh.Get());

	// 메시 이름
	ImGui::Text("Mesh Name");
	ImGui::SameLine(120);
	ImGui::InputText("##MeshNameMeshUI", (char*)strName.c_str(), strName.capacity(), ImGuiInputTextFlags_ReadOnly);

	// 정점 카운트
	int VtxCount = pMesh->GetVertexCount();
	ImGui::Text("Vertex Count");
	ImGui::SameLine(120);
	ImGui::InputInt("##VtxCountMeshUI", &VtxCount, 0, 100, ImGuiInputTextFlags_ReadOnly);
	
	// 삼각형 개수
	int IdxCount = pMesh->GetIndexCount() / 3;
	ImGui::Text("Triangle Count");
	ImGui::SameLine(120);
	ImGui::InputInt("##IdxCountMeshUI", &IdxCount, 0, 100, ImGuiInputTextFlags_ReadOnly);
}