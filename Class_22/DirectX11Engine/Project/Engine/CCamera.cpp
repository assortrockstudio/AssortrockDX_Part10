#include "pch.h"
#include "CCamera.h"

#include "CKeyMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"
#include "CAnimator3D.h"

#include "CRenderMgr.h"
#include "CMRT.h"
#include "components.h"

#include "CDevice.h"
#include "CInstancingBuffer.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_Frustum(this)
	, m_ProjType(PROJ_TYPE::PERSPECTIVE)
	, m_CamPriority(-1)
	, m_FOV((XM_PI / 3.f))
	, m_Far(10000.f)
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_LayerCheck(0)
{
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();
	m_Width = vRenderResol.x;
	m_AspectRatio = vRenderResol.x / vRenderResol.y;
}

CCamera::CCamera(const CCamera& _Other)
	: CComponent(_Other)
	, m_Frustum(_Other.m_Frustum)
	, m_ProjType(_Other.m_ProjType)
	, m_CamPriority(-1)
	, m_FOV(_Other.m_FOV)
	, m_Far(_Other.m_Far)
	, m_Width(_Other.m_Width)
	, m_AspectRatio(_Other.m_AspectRatio)
	, m_Scale(_Other.m_Scale)
	, m_LayerCheck(_Other.m_LayerCheck)
{
	m_Frustum.SetOwner(this);
}

CCamera::~CCamera()
{
}

void CCamera::begin()
{
	// 레벨이 시작될 때 카메라를 RenderMgr 에 등록
	CRenderMgr::GetInst()->RegisterCamera(this, m_CamPriority);
}

void CCamera::finaltick()
{
	// View 행렬 계산
	Vec3 vCamWorldPos = Transform()->GetWorldPos();
	Matrix matViewTrans = XMMatrixTranslation(-vCamWorldPos.x, -vCamWorldPos.y, -vCamWorldPos.z);

	// 카메라가 바라보는 방향을 z 축을 처다보도록 하는 회전량을 회전행렬로 구해야한다.
	// 카메라의 우, 상, 전 벡터
	Vec3 vR = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Matrix matViewRot = XMMatrixIdentity();
	matViewRot._11 = vR.x;	matViewRot._12 = vU.x;	 matViewRot._13 = vF.x;
	matViewRot._21 = vR.y;	matViewRot._22 = vU.y;	 matViewRot._23 = vF.y;
	matViewRot._31 = vR.z;	matViewRot._32 = vU.z;	 matViewRot._33 = vF.z;

	m_matView = matViewTrans * matViewRot;
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);

	// Proj 행렬 계산
	if (PROJ_TYPE::PERSPECTIVE == m_ProjType)
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
	else
	{		
		m_matProj = XMMatrixOrthographicLH(m_Width * m_Scale, (m_Width / m_AspectRatio) * m_Scale, 1.f, m_Far);
	}

	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);

	// 마우스방향 Ray 계산
	CalcRay();

	// 절두체 계산
	m_Frustum.finaltick();
}

void CCamera::render()
{

}

void CCamera::CalcRay()
{
	// ViewPort 정보
	CMRT* pSwapChainMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	if (nullptr == pSwapChainMRT)
		return;
	const D3D11_VIEWPORT& VP = pSwapChainMRT->GetViewPort();

	// 현재 마우스 좌표
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	// 마우스를 향하는 직선은 카메라 위치를 지난다.
	m_Ray.vStart = Transform()->GetWorldPos();

	// View 공간 상에서 카메라에서 마우스 방향을 향하는 방향벡터를 구한다.
	//  - 마우스가 있는 좌표를 -1 ~ 1 사이의 정규화된 좌표로 바꾼다.
	//  - 투영행렬의 _11, _22 에 있는 값은 Near 평면상에서 Near 값을 가로 세로 길이로 나눈값
	//  - 실제 ViewSpace 상에서의 Near 평명상에서 마우스가 있는 지점을 향하는 위치를 구하기 위해서 비율을 나누어서 
	//  - 실제 Near 평면상에서 마우스가 향하는 위치를 좌표를 구함
	m_Ray.vDir.x = (((vMousePos.x - VP.TopLeftX) * 2.f / VP.Width) - 1.f) / m_matProj._11;
	m_Ray.vDir.y = -(((vMousePos.y - VP.TopLeftY) * 2.f / VP.Height) - 1.f) / m_matProj._22;
	m_Ray.vDir.z = 1.f;

	// 방향 벡터에 ViewMatInv 를 적용, 월드상에서의 방향을 알아낸다.
	m_Ray.vDir = XMVector3TransformNormal(m_Ray.vDir, m_matViewInv);
	m_Ray.vDir.Normalize();
}

void CCamera::SortClear()
{
	// 이전 프레임 분류정보 제거
	m_mapInstGroup_D.clear();
	m_mapInstGroup_F.clear();

	m_vecDecal.clear();
	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTransparent.clear();
	m_vecParticle.clear();
	m_vecPostProcess.clear();
}

void CCamera::render_deferred()
{
	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_D)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->Binding();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh 알리기
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->Binding_Inst();
		pMesh->render_instancing(pair.second[0].iMtrlIdx);
		CRenderMgr::GetInst()->AddDrawCall();

		// 정리
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh 알리기
			pMtrl->SetBoneCount(0);
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->Binding();

		for (auto& instObj : pair.second)
		{			
			instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
			CRenderMgr::GetInst()->AddDrawCall();
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::render_decal()
{
	for (size_t i = 0; i < m_vecDecal.size(); ++i)
	{
		m_vecDecal[i]->render();
		CRenderMgr::GetInst()->AddDrawCall();
	}	
}

void CCamera::render_opaque()
{
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->render();
		CRenderMgr::GetInst()->AddDrawCall();
	}
}

void CCamera::render_masked()
{
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->render();
		CRenderMgr::GetInst()->AddDrawCall();
	}
}

void CCamera::render_transparent()
{
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
		CRenderMgr::GetInst()->AddDrawCall();
	}
}

void CCamera::render_particle()
{
	for (size_t i = 0; i < m_vecParticle.size(); ++i)
	{
		m_vecParticle[i]->render();
		CRenderMgr::GetInst()->AddDrawCall();
	}	
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPostProcess[i]->render();
		CRenderMgr::GetInst()->AddDrawCall();
	}	
}

void CCamera::render_shadowmap()
{
	for (size_t i = 0; i < m_vecShadowMap.size(); ++i)
	{
		m_vecShadowMap[i]->GetRenderComponent()->render_shadowmap();
	}
}


void CCamera::SetCameraPriority(int _Priority)
{
	m_CamPriority = _Priority;	
}

void CCamera::LayerCheck(int _LayerIdx)
{
	if (m_LayerCheck & (1 << _LayerIdx))
	{
		m_LayerCheck &= ~(1 << _LayerIdx);
	}
	else
	{
		m_LayerCheck |= (1 << _LayerIdx);
	}
}

void CCamera::SortObject()
{
	SortClear();

	// 현재 레벨 가져와서 분류
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_LayerCheck & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);
			const vector<CGameObject*>& vecObjects = pLayer->GetObjects();

			for (size_t j = 0; j < vecObjects.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObjects[j]->GetRenderComponent();

				// 렌더링 기능이 없는 오브젝트는 제외
				if (nullptr == pRenderCom || nullptr == pRenderCom->GetMesh())
					continue;

				// FrustumCheck 기능을 사용하는지, 사용한다면 Frustum 내부에 들어오는지 체크
				if (vecObjects[j]->GetRenderComponent()->IsFrustumCheck())
				{
					// vecObjects[j] 의 BoundingBox 를 확인
					if (vecObjects[j]->BoundingBox())
					{
						Vec3 vWorldPos = vecObjects[j]->BoundingBox()->GetWorldPos();
						float Radius = vecObjects[j]->BoundingBox()->GetRadius();

						if (false == m_Frustum.FrustumSphereCheck(vWorldPos, Radius))
						{
							continue;
						}
					}

					else
					{
						Vec3 vWorldPos = vecObjects[j]->Transform()->GetWorldPos();
						Vec3 vWorldScale = vecObjects[j]->Transform()->GetWorldScale();

						float Radius = vWorldScale.x;
						if (Radius < vWorldScale.y) Radius = vWorldScale.y;
						if (Radius < vWorldScale.z) Radius = vWorldScale.z;

						if (false == m_Frustum.FrustumSphereCheck(vWorldPos, Radius))
						{
							continue;
						}
					}
				}


				// 메테리얼 개수만큼 반복
				UINT iMtrlCount = pRenderCom->GetMtrlCount();
				for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
				{
					// 재질이 없거나, 재질의 쉐이더가 설정이 안된 경우
					if (nullptr == pRenderCom->GetMaterial(iMtrl)
						|| nullptr == pRenderCom->GetMaterial(iMtrl)->GetShader())
					{
						continue;
					}

					// 쉐이더 도메인에 따른 분류
					SHADER_DOMAIN eDomain = pRenderCom->GetMaterial(iMtrl)->GetShader()->GetDomain();
					Ptr<CGraphicShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();

					switch (eDomain)
					{
					case SHADER_DOMAIN::DOMAIN_DEFERRED:
					case SHADER_DOMAIN::DOMAIN_OPAQUE:
					case SHADER_DOMAIN::DOMAIN_MASKED:
					{
						// Shader 의 DOMAIN 에 따라서 인스턴싱 그룹을 분류한다.
						map<ULONG64, vector<tInstObj>>* pMap = NULL;
						Ptr<CMaterial> pMtrl = pRenderCom->GetMaterial(iMtrl);

						if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED)
						{
							pMap = &m_mapInstGroup_D;
						}
						else if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_OPAQUE
							|| pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_MASKED)
						{
							pMap = &m_mapInstGroup_F;
						}
						else
						{
							assert(nullptr);
							continue;
						}

						uInstID uID = {};
						uID.llID = pRenderCom->GetInstID(iMtrl);

						// ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
						if (0 == uID.llID)
							continue;

						map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
						if (iter == pMap->end())
						{
							pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObjects[j], iMtrl }}));
						}
						else
						{
							iter->second.push_back(tInstObj{ vecObjects[j], iMtrl });
						}
					}
					break;
					case SHADER_DOMAIN::DOMAIN_DECAL:
						m_vecDecal.push_back(vecObjects[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
						m_vecTransparent.push_back(vecObjects[j]);
						break;
					case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
						m_vecPostProcess.push_back(vecObjects[j]);
						break;
					}
				}
			}
		}
	}
}

void CCamera::SortObject_ShadowMap()
{
	m_vecShadowMap.clear();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_LayerCheck & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);
			const vector<CGameObject*>& vecObjects = pLayer->GetObjects();

			for (size_t j = 0; j < vecObjects.size(); ++j)
			{
				if (!vecObjects[j]->GetRenderComponent()
					|| nullptr == vecObjects[j]->GetRenderComponent()->GetMesh()
					|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial(0)
					|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial(0)->GetShader()
					|| false == vecObjects[j]->GetRenderComponent()->IsDynamicShadow())
					continue;

				// FrustumCheck 기능을 사용하는지, 사용한다면 Frustum 내부에 들어오는지 체크
				//if (vecObjects[j]->GetRenderComponent()->IsFrustumCheck())
				//{
				//	// vecObjects[j] 의 BoundingBox 를 확인
				//	if (vecObjects[j]->BoundingBox())
				//	{
				//		Vec3 vWorldPos = vecObjects[j]->BoundingBox()->GetWorldPos();
				//		float Radius = vecObjects[j]->BoundingBox()->GetRadius();

				//		if (false == m_Frustum.FrustumSphereCheck(vWorldPos, Radius))
				//		{
				//			continue;
				//		}
				//	}

				//	else
				//	{
				//		Vec3 vWorldPos = vecObjects[j]->Transform()->GetWorldPos();
				//		Vec3 vWorldScale = vecObjects[j]->Transform()->GetWorldScale();

				//		float Radius = vWorldScale.x;
				//		if (Radius < vWorldScale.y) Radius = vWorldScale.y;
				//		if (Radius < vWorldScale.z) Radius = vWorldScale.z;

				//		if (false == m_Frustum.FrustumSphereCheck(vWorldPos, Radius))
				//		{
				//			continue;
				//		}
				//	}
				//}

				m_vecShadowMap.push_back(vecObjects[j]);				
			}
		}
	}
}




void CCamera::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_CamPriority, sizeof(int), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_Scale, sizeof(float), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
}

void CCamera::LoadFromLevelFile(FILE* _File)
{
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_CamPriority, sizeof(int), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_Scale, sizeof(float), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
}