#include "pch.h"
#include "SoundUI.h"


SoundUI::SoundUI()
	: AssetUI("SoundUI", "##SoundUI", ASSET_TYPE::SOUND)
{
}

SoundUI::~SoundUI()
{
}

void SoundUI::render_tick()
{
	render_title();

}