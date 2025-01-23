#include "pch.h"
#include "CGameObjectEx.h"

#include <Engine/CComponent.h>

void CGameObjectEx::finaltick()
{
	// Component finaltick 호출
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != GetComponent(COMPONENT_TYPE(i)))
			GetComponent(COMPONENT_TYPE(i))->finaltick();
	}

	// 자식 오브젝트 finaltick 호출
	const vector<CGameObject*>& vecChild = GetChildren();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		vecChild[i]->finaltick();
	}
}