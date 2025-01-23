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

	// ������Ʈ�� ��ġ���� ������۸� ���ؼ� ���ε�
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

			GetMaterial(i)->SetAnim3D(true); // Animation Mesh �˸���
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	// �޽��� ��� ������ ������ �Ѵ�.
	UINT iMtrlCount = GetMaterialCount();
	for (int i = 0; i < iMtrlCount; ++i)
	{
		if (nullptr == GetMaterial(i))
			continue;

		// ����� ���̴� ���ε�
		GetMaterial(i)->Binding();

		// �޽� ���ε� �� ������
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

	// Transform �� UpdateData ��û
	Transform()->Binding();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->Binding();
	}

	// Animator3D ������Ʈ
	if (Animator3D())
	{
		Animator3D()->Binding();
		GetMaterial(_Subset)->SetAnim3D(true); // Animation Mesh �˸���
		GetMaterial(_Subset)->SetBoneCount(Animator3D()->GetBoneCount());
	}

	// ����� ���� ������Ʈ
	GetMaterial(_Subset)->Binding();

	// ����� �޽� ������Ʈ �� ������
	GetMesh()->render(_Subset);

	// Animation ���� ���� ����
	if (Animator2D())
		CAnim2D::Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}