#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMeshRender.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"


MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRenderUI", "##MeshRenderUI", COMPONENT_TYPE::MESHRENDER)
{
	SetSize(ImVec2(0, 150));
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::render_tick()
{
	render_title();

	CMeshRender* pMeshRender = GetTarget()->MeshRender();

	Ptr<CMesh> pMesh = pMeshRender->GetMesh();
	Ptr<CMaterial> pMtrl = pMeshRender->GetMaterial();
	
	string strMeshKey;

	if (nullptr != pMesh)
	{
		strMeshKey = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
	}
	
	ImGui::Text("Mesh      ");
	ImGui::SameLine();
	ImGui::InputText("##MeshName", (char*)strMeshKey.c_str(), strMeshKey.capacity(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content");
		
		if (nullptr != payload)
		{
			DWORD_PTR dwData = 0;
			memcpy(&dwData, payload->Data, payload->DataSize);
			
			Ptr<CAsset> pAsset = (CAsset*)dwData;
			if (pAsset->GetAssetType() == ASSET_TYPE::MESH)
			{
				pMeshRender->SetMesh((CMesh*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}



	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", ImVec2(18, 18)))
	{
		// ListUI 를 활성화한다
		ListUI* pListUI = CImGuiMgr::GetInst()->FindEditorUI<ListUI>("List");

		if (nullptr != pListUI && !pListUI->IsActive())
		{			
			pListUI->SetModal(true);
			pListUI->SetActive(true);
			pListUI->SetFocus();
			
			vector<string> vecNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MESH, vecNames);
			pListUI->AddItem(vecNames);
			UI_DELEGATE1 d = (UI_DELEGATE1)&MeshRenderUI::SelectMesh;
			pListUI->RegisterDBClickDelegate(this, d);
		}		
	}


	string strMtrlKey;
	
	if (nullptr != pMtrl)
	{
		strMtrlKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	}
	
	ImGui::Text("Material  ");
	ImGui::SameLine();
	ImGui::InputText("##MaterialName", (char*)strMtrlKey.c_str(), strMtrlKey.capacity(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content");

		if (nullptr != payload)
		{
			DWORD_PTR dwData = 0;
			memcpy(&dwData, payload->Data, payload->DataSize);

			Ptr<CAsset> pAsset = (CAsset*)dwData;
			if (pAsset->GetAssetType() == ASSET_TYPE::MATERIAL)
			{
				pMeshRender->SetMaterial((CMaterial*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}




	ImGui::SameLine();
	if (ImGui::Button("##MaterialBtn", ImVec2(18, 18)))
	{
		// ListUI 를 활성화한다
		ListUI* pListUI = CImGuiMgr::GetInst()->FindEditorUI<ListUI>("List");

		if (nullptr != pListUI)
		{
			if (pListUI->SetActive(true))
			{
				pListUI->SetFocus();

				vector<string> vecNames;
				CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MATERIAL, vecNames);
				pListUI->AddItem(vecNames);
				pListUI->RegisterDBClickDelegate(this, (UI_DELEGATE1)&MeshRenderUI::SelectMaterial);
			}
		}
	}
}

UINT MeshRenderUI::SelectMesh(DWORD_PTR _Selected)
{
	string* pStr = (string*)_Selected;

	wstring strMeshKey = ToWString(*pStr);

	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(strMeshKey);

	assert(nullptr != GetTarget() && nullptr != pMesh);

	GetTarget()->GetRenderComponent()->SetMesh(pMesh);

	SetFocus();

	return 0;
}

UINT MeshRenderUI::SelectMaterial(DWORD_PTR _Selected)
{
	string* pStr = (string*)_Selected;

	wstring strMtrlKey = ToWString(*pStr);

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(strMtrlKey);

	assert(nullptr != GetTarget() && nullptr != pMtrl);

	GetTarget()->GetRenderComponent()->SetMaterial(pMtrl);

	SetFocus();

	return 0;
}
