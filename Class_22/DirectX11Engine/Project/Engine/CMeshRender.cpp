#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
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

	// Animator2D Binding
	if (Animator2D())
	{
		Animator2D()->Binding();
	}
	else
	{
		CAnim2D::Clear();
	}

	// Animator3D Binding
	if (Animator3D())
	{
		Animator3D()->Binding();

		for (UINT i = 0; i < GetMtrlCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;

			GetMaterial(i)->SetAnim3D(true); // Animation Mesh 알리기
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	// 메쉬의 모든 부위를 렌더링 한다.
	UINT iMtrlCount = GetMaterialCount();
	for (int i = 0; i < iMtrlCount; ++i)
	{
		if (nullptr == GetMaterial(i))
			continue;

		// 사용할 쉐이더 바인딩
		GetMaterial(i)->Binding();

		// 메시 바인딩 및 렌더링
		GetMesh()->render(i);
	}


	if (Animator3D())
	{
		Animator3D()->ClearData();
	}	
}

void CMeshRender::render(UINT _Subset)
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(_Subset))
		return;

	// Transform 에 UpdateData 요청
	Transform()->Binding();

	// Animator2D 컴포넌트가 있다면
	if (Animator2D())
	{
		Animator2D()->Binding();
	}

	// Animator3D 업데이트
	if (Animator3D())
	{
		Animator3D()->Binding();
		GetMaterial(_Subset)->SetAnim3D(true); // Animation Mesh 알리기
		GetMaterial(_Subset)->SetBoneCount(Animator3D()->GetBoneCount());
	}

	// 사용할 재질 업데이트
	GetMaterial(_Subset)->Binding();

	// 사용할 메쉬 업데이트 및 렌더링
	GetMesh()->render(_Subset);

	// Animation 관련 정보 제거
	if (Animator2D())
		CAnim2D::Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}