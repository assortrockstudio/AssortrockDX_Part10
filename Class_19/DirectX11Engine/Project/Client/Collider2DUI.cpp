#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>

Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2D", "##Collider2DUI", COMPONENT_TYPE::COLLIDER2D)
{
	SetSize(ImVec2(0, 120));
}

Collider2DUI::~Collider2DUI()
{
}


void Collider2DUI::render_tick()
{
	render_title();

	CCollider2D* pCollider = GetTarget()->Collider2D();
	assert(pCollider);

	Vec3 vOffsetPos = pCollider->GetOffset();
	Vec3 vScale = pCollider->GetScale();
	bool bAbsolute = pCollider->IsAbsolute();

	float ScaleDragSpeed = 1.f;
	if (!bAbsolute)
	{
		ScaleDragSpeed = 0.01f;
	}

	ImGui::Text("Offset Position");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Collider2DOffsetPos", vOffsetPos))
		pCollider->SetOffset(vOffsetPos);


	ImGui::Text("Scale          ");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Collider2DScale", vScale, ScaleDragSpeed))
		pCollider->SetScale(vScale);


	ImGui::Text("Absolute       ");
	ImGui::SameLine();
	if (ImGui::Checkbox("##Collider2DAbsolute", &bAbsolute))
	{
		pCollider->SetAbsolute(bAbsolute);
	}
}