#include "pch.h"
#include "Animator2DUI.h"


Animator2DUI::Animator2DUI()
	: ComponentUI("Animator2D", "##Animator2DUI", COMPONENT_TYPE::ANIMATOR2D)
{
	SetSize(ImVec2(0, 200));
}

Animator2DUI::~Animator2DUI()
{
}


void Animator2DUI::render_tick()
{
	render_title();


}