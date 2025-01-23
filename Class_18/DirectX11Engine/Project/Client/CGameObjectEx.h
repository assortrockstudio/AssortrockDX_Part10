#pragma once

#include <Engine/CGameObject.h>

class CGameObjectEx
	: public CGameObject
{
public:
	virtual void finaltick() override;
};

