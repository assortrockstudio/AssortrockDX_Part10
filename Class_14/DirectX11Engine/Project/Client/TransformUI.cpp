#include "pch.h"
#include "TransformUI.h"


#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: ComponentUI("Transform", "##TransformUI", COMPONENT_TYPE::TRANSFORM)
{
	SetSize(ImVec2(0, 150));
}

TransformUI::~TransformUI()
{
}

void TransformUI::render_tick()
{	
	// 타이틀 출력
	render_title();

	Vec3 vPos = GetTarget()->Transform()->GetRelativePos();
	Vec3 vScale = GetTarget()->Transform()->GetRelativeScale();
	Vec3 vRotation = GetTarget()->Transform()->GetRelativeRotation();

	ImGui::Text("Position  "); 
	ImGui::SameLine();

	if (ImGui::DragFloat3("##Position", vPos))
		GetTarget()->Transform()->SetRelativePos(vPos);

	ImGui::Text("Scale     ");
	ImGui::SameLine();

	if (ImGui::DragFloat3("##Scale", vScale))
		GetTarget()->Transform()->SetRelativeScale(vScale);

	ImGui::Text("Rotation  ");
	ImGui::SameLine();

	vRotation = (vRotation / XM_PI) * 180.f;
	if (ImGui::DragFloat3("##Rotation", vRotation))
	{
		vRotation = (vRotation / 180.f) * XM_PI;
		GetTarget()->Transform()->SetRelativeRotation(vRotation);
	}

	bool Absolute = GetTarget()->Transform()->IsAbsolute();

	ImGui::Text("Absolute  ");
	ImGui::SameLine();
	if (ImGui::Checkbox("##Absolute", &Absolute))
	{
		GetTarget()->Transform()->SetAbsolute(Absolute);
	}	
}
