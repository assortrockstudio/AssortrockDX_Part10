#include "pch.h"
#include "Inspector.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "CameraUI.h"
#include "Light2DUI.h"
#include "Light3DUI.h"
#include "Animator2DUI.h"
#include "Collider2DUI.h"
#include "TileMapUI.h"
#include "ParticleSystemUI.h"
#include "SkyBoxUI.h"
#include "ScriptUI.h"

#include "MeshUI.h"
#include "MeshDataUI.h"
#include "MaterialUI.h"
#include "TextureUI.h"
#include "PrefabUI.h"
#include "SoundUI.h"
#include "GraphicShaderUI.h"
#include "ComputeShaderUI.h"

#include "TileMapEditor.h"

Inspector::Inspector()
	: EditorUI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)
	, m_arrComUI{}
{
	// ComponentUI 생성
	CreateComponentUI();

	// AssetUI 생성
	CreateAssetUI();
}

Inspector::~Inspector()
{

}

void Inspector::render_tick()
{

}

void Inspector::SetTargetObject(CGameObject* _Target)
{
	// 에셋 UI 비활성화
	if (nullptr != m_TargetAsset)
	{
		m_arrAssetUI[(UINT)m_TargetAsset->GetAssetType()]->SetActive(false);
		m_TargetAsset = nullptr;
	}

	m_TargetObject = _Target;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		// 해당 컴포넌트를 담당할 UI 가 생성되어있지 않기 때문에 nullptr 체크 해주어야 함
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTarget(m_TargetObject);
	}

	// Script 관련 처리
	if (nullptr == m_TargetObject)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetActive(false);
		}
	}
	else
	{
		// Object 가 Script 가 있는지 확인, 있으면 ScriptUI 를 활성화
		const vector<CScript*>& vecScripts = m_TargetObject->GetScripts();

		// 오브젝트의 스크립트 수 보다, 스크립트UI 가 더 개수가 작으면, ScriptUI 를 추가 생성한다.
		if (m_vecScriptUI.size() < vecScripts.size())
		{
			size_t iAddCount = vecScripts.size() - m_vecScriptUI.size();
			for (size_t i = 0; i < iAddCount; ++i)
			{
				ScriptUI* pScriptUI = nullptr;
				pScriptUI = new ScriptUI;
				pScriptUI->SetActive(false);
				pScriptUI->SetSeperate(true);
				m_vecScriptUI.push_back(pScriptUI);
				AddChildUI(pScriptUI);
			}
		}

		// 각 ScriptUI 에 각각의 CScript 를 배정한다.
		for (size_t i = 0; i < vecScripts.size(); ++i)
		{
			m_vecScriptUI[i]->SetScript(vecScripts[i]);
			m_vecScriptUI[i]->SetActive(true);
		}
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	// 모든 컴포넌트UI 들을 비활성화
	SetTargetObject(nullptr);

	// 타겟 에셋 지정
	m_TargetAsset = _Asset;

	// 모든 AssetUI 비활성화
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (nullptr == m_arrAssetUI[i])
			continue;
		m_arrAssetUI[i]->SetActive(false);
	}

	// 타겟 에셋이 nullptr 이면 함수 종료
	if (nullptr == m_TargetAsset)
		return;
	
	// 타겟 에셋에 해당하는 UI 를 활성화
	ASSET_TYPE AssetType = m_TargetAsset->GetAssetType();
	m_arrAssetUI[(UINT)AssetType]->SetActive(true);
	m_arrAssetUI[(UINT)AssetType]->SetTarget(m_TargetAsset);
}

void Inspector::CreateComponentUI()
{
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT3D] = new Light3DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new ParticleSystemUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::SKYBOX] = new SkyBoxUI;


	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetActive(false);
		m_arrComUI[i]->SetSeperate(true);
		AddChildUI(m_arrComUI[i]);
	}

	// ScriptUI
	ScriptUI* pScriptUI = nullptr;
	pScriptUI = new ScriptUI;
	pScriptUI->SetActive(false);
	pScriptUI->SetSeperate(true);

	m_vecScriptUI.push_back(pScriptUI);

	AddChildUI(pScriptUI);
}



void Inspector::CreateAssetUI()
{
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH] = new MeshUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH_DATA] = new MeshDataUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL] = new MaterialUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE] = new TextureUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB] = new PrefabUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::SOUND] = new SoundUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHICS_SHADER] = new GraphicShaderUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (nullptr == m_arrAssetUI[i])
			continue;

		m_arrAssetUI[i]->SetActive(false);
		m_arrAssetUI[i]->SetSeperate(true);
		AddChildUI(m_arrAssetUI[i]);
	}
}