#include "pch.h"
#include "MaterialUI.h"

#include <Engine/CMaterial.h>
#include <Engine/CGraphicShader.h>

#include "ParamUI.h"

MaterialUI::MaterialUI()
	: AssetUI("MaterialUI", "##MaterialUI", ASSET_TYPE::MATERIAL)
	, m_ParamDescMaxWidth(0)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::render_tick()
{
	render_title();

	string strName = ToString(GetTarget()->GetKey());

	Ptr<CMaterial> pMtrl = dynamic_cast<CMaterial*>(GetTarget().Get());
	assert(pMtrl.Get());

	// 메시 이름
	ImGui::Text("Material Name");
	ImGui::SameLine(110);
	ImGui::InputText("##MaterialNameMaterialUI", (char*)strName.c_str(), strName.capacity(), ImGuiInputTextFlags_ReadOnly);

	// 연결된 쉐이더 정보 출력
	string strShaderName = nullptr == pMtrl->GetShader() ? "" : ToString(pMtrl->GetShader()->GetKey());

	ImGui::Text("Shader Name");
	ImGui::SameLine(110);
	ImGui::InputText("##ShaderNameShaderUI", (char*)strShaderName.c_str(), strShaderName.capacity(), ImGuiInputTextFlags_ReadOnly);

	// DragDrop
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content");

		if (nullptr != payload)
		{
			DWORD_PTR dwData = 0;
			memcpy(&dwData, payload->Data, payload->DataSize);

			Ptr<CAsset> pAsset = (CAsset*)dwData;
			if (pAsset->GetAssetType() == ASSET_TYPE::GRAPHICS_SHADER)
			{
				Ptr<CGraphicShader> pShader = (CGraphicShader*)pAsset.Get();
				pMtrl->SetShader(pShader);
			}
		}

		ImGui::EndDragDropTarget();
	}








	ImGui::Separator();
	ImGui::Text("Shader Parameter");
	ImGui::Text("");

	// 재질이 참조하는 쉐이더가 있다면
	if (nullptr == pMtrl->GetShader())
		return;

	// 파라미터 UI 가 Desc 정보를 출력할때 최대 길이값 설정
	ParamUI::SetDescWidth(m_ParamDescMaxWidth);
	
	// 쉐이더가 요청한 파라미터 목록 확인
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();	
	vector<tShaderScalarParam> ScalarParam = pShader->GetScalarParam();
	
	// 쉐이더가 요청한 각 파라미터에 대응하는 정보를 MaterialUI 에 노출
	for (size_t i = 0; i < ScalarParam.size(); ++i)
	{
		// 파라미터 설명글의 최대 가로길이를 계산한다.
		ImVec2 vDescWidth = ImGui::CalcTextSize(ScalarParam[i].strDesc.c_str());
		if (m_ParamDescMaxWidth < vDescWidth.x)
			m_ParamDescMaxWidth = vDescWidth.x;
		

		// 파라미터 타입에 따른 정보를 노출시킨다.
		switch (ScalarParam[i].Param)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
			ParamUI::Param_DragInt(ScalarParam[i].strDesc, (int*)pMtrl->GetScalarParam(ScalarParam[i].Param));			
			break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
			ParamUI::Param_DragFloat(ScalarParam[i].strDesc, (float*)pMtrl->GetScalarParam(ScalarParam[i].Param), 0.01f);
			break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
			ParamUI::Param_DragVec2(ScalarParam[i].strDesc, (Vec2*)pMtrl->GetScalarParam(ScalarParam[i].Param));
			break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
			ParamUI::Param_DragVec4(ScalarParam[i].strDesc, (Vec4*)pMtrl->GetScalarParam(ScalarParam[i].Param));
			break;
		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
			ParamUI::Param_DragMat(ScalarParam[i].strDesc, (Matrix*)pMtrl->GetScalarParam(ScalarParam[i].Param));
			break;		
		}		
	}


	// 쉐이더가 요청한 텍스쳐 파라미터 목록 확인	
	vector<tShaderTexParam> TexParam = pShader->GetTexParam();
	for (size_t i = 0; i < TexParam.size(); ++i)
	{
		// 파라미터 설명글의 최대 가로길이를 계산한다.
		ImVec2 vDescWidth = ImGui::CalcTextSize(TexParam[i].strDesc.c_str());
		if (m_ParamDescMaxWidth < vDescWidth.x)
			m_ParamDescMaxWidth = vDescWidth.x;
				
		// ListUI 가 더블클릭됐을때 호출 될 Delegate 등록
		ParamUI::RegisterTexDelegate(this, (UI_DELEGATE1)&MaterialUI::SelectTexture);

		// 텍스쳐 파라미터 정보 출력
		if (ParamUI::Param_Tex(TexParam[i].strDesc, pMtrl->GetTexParam(TexParam[i].Param)))
		{
			m_TargetParam = TexParam[i].Param;
		}

		ImGui::Text("");
	}
}

void MaterialUI::TargetChanged()
{
	m_ParamDescMaxWidth = 0;
}

UINT MaterialUI::SelectTexture(DWORD_PTR _Selected)
{
	string* pStr = (string*)_Selected;

	wstring strTexKey = ToWString(*pStr);

	Ptr<CTexture> pTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(strTexKey);
		
	Ptr<CMaterial> pMtrl = dynamic_cast<CMaterial*>(GetTarget().Get());

	pMtrl->SetTexParam(m_TargetParam, pTexture);

	SetFocus();

	return 0;
}
