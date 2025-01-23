#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_CurAnim(nullptr)
	, m_Repeat(false)
{

}

CAnimator2D::CAnimator2D(const CAnimator2D& _Other)
	: CComponent(_Other)
	, m_CurAnim(nullptr)
	, m_Repeat(_Other.m_Repeat)
{
	for (const auto& pair : _Other.m_mapAnim)
	{
		CAnim2D* pCloneAnim = pair.second->Clone();

		pCloneAnim->m_Animator = this;
		m_mapAnim.insert(make_pair(pair.first, pCloneAnim));
	}

	if (nullptr != _Other.m_CurAnim)
	{		
		m_CurAnim = FindAnimation(_Other.m_CurAnim->GetName());
	}
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (nullptr == m_CurAnim)
		return;

	if (m_Repeat && m_CurAnim->IsFinish())
	{
		m_CurAnim->Reset();
	}

	m_CurAnim->finaltick();
}

void CAnimator2D::Binding()
{
	if(nullptr != m_CurAnim)
		m_CurAnim->Binding();
}



void CAnimator2D::CreateAnimation(const wstring& _AnimName, Ptr<CTexture> _AltasTex
							, Vec2 _LeftTopPixelPos, Vec2 _vSlicePixelSize, Vec2 _BackgroundSize
						    , int _FrameCount, UINT _FPS)
{
	assert(!FindAnimation(_AnimName));

	CAnim2D* pAnim = new CAnim2D;
	pAnim->SetName(_AnimName);
	pAnim->Create(_AltasTex, _LeftTopPixelPos, _vSlicePixelSize, _BackgroundSize, _FrameCount, _FPS);

	pAnim->m_Animator = this;
	m_mapAnim.insert(make_pair(_AnimName, pAnim));
}

CAnim2D* CAnimator2D::FindAnimation(const wstring& _AnimName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_AnimName);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator2D::LoadAnimation(const wstring& _strRelativePath)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Load(_strRelativePath);

	pAnim->m_Animator = this;
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}

void CAnimator2D::Play(const wstring& _strAnimName, bool _Repeat)
{
	m_CurAnim = FindAnimation(_strAnimName);
	m_Repeat = _Repeat;
}



void CAnimator2D::SaveToLevelFile(FILE* _File)
{
	// 애니메이션 개수
	size_t AnimCount = m_mapAnim.size();
	fwrite(&AnimCount, sizeof(size_t), 1, _File);

	// 재생중이던 애니메이션 이름
	wstring strCurAnimName;
	if (m_CurAnim)
	{
		strCurAnimName = m_CurAnim->GetName();
	}
	SaveWString(strCurAnimName, _File);

	// 반복 상태
	fwrite(&m_Repeat, 1, 1, _File);

	// 모든 애니메이션 정보를 저장
	for (const auto& pair : m_mapAnim)
	{
		pair.second->SaveToLevelFile(_File);
	}	
}

void CAnimator2D::LoadFromLevelFile(FILE* _File)
{
	// 애니메이션 개수
	size_t AnimCount = 0;
	fread(&AnimCount, sizeof(size_t), 1, _File);

	// 재생중이던 애니메이션 이름
	wstring strCurAnimName;	
	LoadWString(strCurAnimName, _File);

	// 반복 상태
	fread(&m_Repeat, 1, 1, _File);

	// 모든 애니메이션 정보를 저장
	for (size_t i = 0; i < AnimCount; ++i)
	{
		CAnim2D* pAnim = new CAnim2D;
		pAnim->LoadFromLevelFile(_File);

		m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
		pAnim->m_Animator = this;
	}	

	// 저장당시 지정해두었던 애니메이션을 다시 재지정한다.
	m_CurAnim = FindAnimation(strCurAnimName);
}