#pragma once

template<typename T>
class Ptr
{
private:
	T*	m_Asset;

public:
	T* Get() const { return m_Asset; }
	T** GetAdressOf() const { return &m_Asset; }


public:
	bool operator == (T* _Asset) const
	{
		if (m_Asset == _Asset)
			return true;
		
		return false;
	}

	bool operator != (T* _Asset) const
	{
		return !((*this) == _Asset);
	}

	bool operator == (const Ptr<T>& _Asset) const
	{
		if (m_Asset == _Asset.m_Asset)
			return true;

		return false;
	}

	bool operator != (const Ptr<T>& _Asset) const
	{
		return !((*this) == _Asset);
	}






	void operator = (T* _Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->Release();

		m_Asset = _Asset;

		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	Ptr<T>& operator = (const Ptr<T>& _Other)
	{
		if (nullptr != m_Asset)
			m_Asset->Release();

		m_Asset = _Other.m_Asset;

		if (nullptr != m_Asset)
			m_Asset->AddRef();

		return *this;
	}

	T* operator->() const
	{
		return m_Asset;
	}

public:
	Ptr()
		: m_Asset(nullptr)
	{
	}

	Ptr(T* _Asset)
		: m_Asset(_Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	Ptr(const Ptr<T>& _Other)
		: m_Asset(_Other.m_Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	~Ptr()
	{
		if (nullptr != m_Asset)
			m_Asset->Release();
	}
};


template<typename T>
bool operator == (void* _Asset, const Ptr<T>& _Ptr)
{
	if (_Asset == _Ptr.Get())
		return true;
	return false;
}

template<typename T>
bool operator != (void* _Asset, const Ptr<T>& _Ptr)
{
	return !(_Asset == _Ptr);
}