#include "pch.h"
#include "Light3DUI.h"

#include <Engine/CLight3D.h>

Light3DUI::Light3DUI()
	: ComponentUI("Light3D", "##Light3DUI", COMPONENT_TYPE::LIGHT3D)
{
	SetSize(ImVec2(0, 300));
}

Light3DUI::~Light3DUI()
{
}

void Light3DUI::render_tick()
{
	render_title();

	CLight3D* pLight3D = GetTarget()->Light3D();
	const tLightInfo& Info = pLight3D->GetLightInfo();

	// Light Type : LIGHT_TYPE 
	ImGui::Text("Light Type");
	ImGui::SameLine(120);

	const char* arrLightType[] = { "Directional", "Point", "Spot" };

	if (ImGui::BeginCombo("##Light2DType", arrLightType[Info.LightType], 0))
	{
		for (int i = 0; i < 3; ++i)
		{
			const bool is_selected = Info.LightType == i;
			if (ImGui::Selectable(arrLightType[i], is_selected))
			{
				pLight3D->SetLightType((LIGHT_TYPE)i);
			}
		}
		ImGui::EndCombo();
	}

	// Light Color
	ImGui::Text("Light Color");
	ImGui::SameLine(120);
	ImGui::ColorEdit3("##Light2DColor", (float*)&Info.Light.vDiffuse, 0);

	// Light Ambient
	ImGui::Text("Light Ambient");
	ImGui::SameLine(120);
	ImGui::ColorEdit3("##Light2DAmbient", (float*)&Info.Light.vAmbient, 0);


	// Range : DirLight X
	bool IsDirLight = Info.LightType == (UINT)LIGHT_TYPE::DIRECTIONAL;
	float fRange = Info.Range;
	ImGui::BeginDisabled(IsDirLight);

	ImGui::Text("Light Range");
	ImGui::SameLine(120);
	if (ImGui::DragFloat("##Light Range", &fRange))
	{
		pLight3D->SetRange(fRange);
	}

	ImGui::EndDisabled();

	// Angle : Only Specular
	bool IsSpot = Info.LightType == (UINT)LIGHT_TYPE::SPOT;
	float Angle = Info.Angle;
	ImGui::BeginDisabled(!IsSpot);

	ImGui::Text("SpotLight Angle");
	ImGui::SameLine(120);
	if (ImGui::DragFloat("##SpotLightAngle", &Angle))
	{
		pLight3D->SetRange(fRange);
	}

	ImGui::EndDisabled();
}