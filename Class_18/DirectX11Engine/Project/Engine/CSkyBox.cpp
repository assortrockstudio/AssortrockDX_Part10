#include "pch.h"
#include "CSkyBox.h"

#include "CTransform.h"

CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SKYBOX_TYPE::SPHERE)
{
	SetSkyBoxType(m_Type);

	SetFrustumCheck(false);
	SetDynamicShadow(false);
}

CSkyBox::~CSkyBox()
{

}

void CSkyBox::finaltick()
{
}

void CSkyBox::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
	{
		return;
	}

	// ������Ʈ�� ��ġ���� ������۸� ���ؼ� ���ε�
	Transform()->Binding();

	// ����� ���̴� ���ε�
	GetMaterial(0)->SetScalarParam(INT_0, m_Type);

	if (SKYBOX_TYPE::SPHERE == m_Type)
		GetMaterial(0)->SetTexParam(TEX_0, m_SkyBoxTex);
	else if(SKYBOX_TYPE::CUBE == m_Type)
		GetMaterial(0)->SetTexParam(TEX_CUBE_0, m_SkyBoxTex);
	
	GetMaterial(0)->Binding();

	// �޽� ���ε� �� ������
	GetMesh()->render(0);
}

void CSkyBox::SetSkyBoxType(SKYBOX_TYPE _Type)
{
	if (m_Type != _Type)
		m_SkyBoxTex = nullptr;

	m_Type = _Type;

	if (SKYBOX_TYPE::SPHERE == m_Type)
	{
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
	}	
	else if (SKYBOX_TYPE::CUBE == m_Type)
	{
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
	}

	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl"),0);
}