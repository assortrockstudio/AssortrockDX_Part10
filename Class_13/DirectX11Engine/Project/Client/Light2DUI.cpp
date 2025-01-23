#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", "##Light2DUI", COMPONENT_TYPE::LIGHT2D)
{
	SetSize(ImVec2(0, 300));
}

Light2DUI::~Light2DUI()
{
}


void Light2DUI::render_tick()
{
	render_title();

	CLight2D* pLight2D = GetTarget()->Light2D();
	const tLightInfo& Info = pLight2D->GetLightInfo();

	// Light Type : LIGHT_TYPE 
	ImGui::Text("Light Type");
	ImGui::SameLine(120);
		
	const char* arrLightType[] = {"Directional", "Point", "Spot"};
	
	if (ImGui::BeginCombo("##Light2DType", arrLightType[Info.LightType], 0))
	{
		for (int i = 0; i < 3; ++i)
		{
			const bool is_selected = Info.LightType == i;
			if (ImGui::Selectable(arrLightType[i], is_selected))
			{
				pLight2D->SetLightType((LIGHT_TYPE)i);
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
		pLight2D->SetRange(fRange);
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
		pLight2D->SetRange(fRange);
	}

	ImGui::EndDisabled();
}