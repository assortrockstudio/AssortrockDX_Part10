#include "pch.h"
#include "CPathMgr.h"

CPathMgr::CPathMgr()
	: m_szContentPath{}
{

}

CPathMgr::~CPathMgr()
{

}

void CPathMgr::init()
{	
	GetCurrentDirectory(255, m_szContentPath);

	size_t len = wcslen(m_szContentPath);

	for (size_t i = len - 1; i > 0; --i)
	{
		if (m_szContentPath[i] == '\\')
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, L"\\content\\");
}

const wstring CPathMgr::GetRelativePath(const wstring& _AbsolutePath)
{
	size_t len = wcslen(m_szContentPath);
	wstring strRelativePath = _AbsolutePath.substr(len, _AbsolutePath.length() - len);
	return strRelativePath;
}
