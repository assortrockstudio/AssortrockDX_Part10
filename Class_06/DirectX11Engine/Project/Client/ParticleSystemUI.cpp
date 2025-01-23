#include "pch.h"
#include "ParticleSystemUI.h"


ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystem", "##ParticleSystemUI", COMPONENT_TYPE::PARTICLESYSTEM)
{
}

ParticleSystemUI::~ParticleSystemUI()
{
}


void ParticleSystemUI::render_tick()
{
	render_title();


}