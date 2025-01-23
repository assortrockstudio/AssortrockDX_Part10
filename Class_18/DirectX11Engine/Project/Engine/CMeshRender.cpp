#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CAnimator2D.h"
#include "CAnim2D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
}

void CMeshRender::render()
{
	if (nullptr == GetMesh())
	{
		return;
	}

	// 오브젝트의 위치값을 상수버퍼를 통해서 바인딩
	Transform()->Binding();

	// 메쉬의 모든 부위를 렌더링 한다.
	UINT iMtrlCount = GetMaterialCount();
	for (int i = 0; i < iMtrlCount; ++i)
	{
		if (nullptr == GetMaterial(i))
			continue;

		// 오브젝트가 Animator 컴포넌트가 있다면..
		if (Animator2D())
		{
			Animator2D()->Binding();
		}
		else
		{
			CAnim2D::Clear();
		}

		// 사용할 쉐이더 바인딩
		GetMaterial(i)->Binding();

		// 메시 바인딩 및 렌더링
		GetMesh()->render(i);
	}
}