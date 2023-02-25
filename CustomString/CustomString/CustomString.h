#pragma once

#include <type_traits>
#include <cstring>
#include <assert.h>
#include <limits>

#ifdef max
#undef max
#endif

#define NODISCARD [[nodiscard]]

template<typename T>
class CustomString final
{
public:

#pragma region Ctors_Dtors

	CustomString() = default;
	CustomString(const T c, const size_t count);
	explicit CustomString(const T* pStr);

	~CustomString();

#pragma endregion

#pragma region Adding_Chars

	CustomString& Assign(const T c, const size_t count);
	CustomString& Assign(const T* pStr);
	CustomString& Assign(const T* pStr, const size_t count);

#pragma endregion

#pragma region String_Information

	NODISCARD size_t Size() const;
	NODISCARD size_t Capacity() const;
	NODISCARD size_t MaxSize() const;
	NODISCARD const T* Data() const;

#pragma endregion

#pragma region Comparison

	NODISCARD bool operator==(const T* pStr) const;

#pragma endregion


private:
#pragma region Reallocation

	void Reallocate(const size_t min);
	constexpr size_t CalculateNewCapacity(const size_t min) const;
	constexpr void Release(T*& pData);
	constexpr void DeleteData(T* head, T* const tail) const;


#pragma endregion

#pragma region Helpers

	size_t CountRawString(const T* const pStr) const;

#pragma endregion

	T* m_pHead;
	T* m_pTail;
	T* m_pCurrentEnd; // Points past the end
};

#pragma region Ctors_Dtors

template<typename T>
CustomString<T>::CustomString(const T c, const size_t count)
	: m_pHead{}
	, m_pTail{}
	, m_pCurrentEnd{}
{
	Assign(c, count);
}

template<typename T>
CustomString<T>::CustomString(const T* pStr)
	: m_pHead{}
	, m_pTail{}
	, m_pCurrentEnd{}
{
	Assign(pStr);
}

template<typename T>
CustomString<T>::~CustomString()
{
	DeleteData(m_pHead, m_pCurrentEnd);
	Release(m_pHead);
}

#pragma endregion

#pragma region Adding_Chars

template<typename T>
CustomString<T>& CustomString<T>::Assign(const T c, const size_t count)
{
	if (!m_pCurrentEnd || (m_pCurrentEnd + count) >= m_pTail)
		Reallocate(count + 1);

	// - 1 because final char needs to be null terminator
	for (size_t i{}; i < count; ++i)
		*m_pCurrentEnd++ = c;

	*m_pCurrentEnd++ = T();

	return *this;
}

template<typename T>
CustomString<T>& CustomString<T>::Assign(const T* pStr)
{
	return Assign(pStr, CountRawString(pStr));
}

template<typename T>
CustomString<T>& CustomString<T>::Assign(const T* pStr, const size_t count)
{
	if (!m_pCurrentEnd || (m_pCurrentEnd + count) >= m_pTail)
		Reallocate(count);

	std::memcpy(m_pCurrentEnd, pStr, count);

	m_pCurrentEnd = m_pCurrentEnd + count;

	return *this;
}

#pragma endregion

#pragma region String_Information

template<typename T>
size_t CustomString<T>::Size() const
{
	return m_pCurrentEnd - m_pHead;
}

template<typename T>
size_t CustomString<T>::Capacity() const
{
	return m_pTail - m_pHead;
}

template<typename T>
size_t CustomString<T>::MaxSize() const
{
	return std::numeric_limits<size_t>::max();
}

template<typename T>
const T* CustomString<T>::Data() const
{
	return m_pHead;
}

#pragma endregion

#pragma region Comparison

template<typename T>
bool CustomString<T>::operator==(const T* pStr) const
{
	if (Size() != CountRawString(pStr))
		return false;

	const T* pThisStr{ m_pHead };
	while ((*pThisStr != T()) && (*pStr != T()))
	{
		if (*pThisStr != *pStr)
			return false;

		++pThisStr;
		++pStr;
	}

	if (*pThisStr != *pStr)
		return false;

	return true;
}

#pragma endregion

#pragma region Reallocation

template<typename T>
void CustomString<T>::Reallocate(const size_t min)
{
	const uint64_t oldSize{ Size() };
	const uint64_t newCap{ CalculateNewCapacity(min) };

	T* pNewHead{ new T[newCap]{} };

	T* pOldHead{ m_pHead };
	T* pOldTail{ m_pTail };

	m_pHead = pNewHead;
	m_pTail = pNewHead + newCap;

	std::memcpy(m_pHead, pOldHead, oldSize);

	m_pCurrentEnd = m_pHead + oldSize;

	DeleteData(pOldHead, pOldTail);
	Release(pOldHead);
}

template<typename T>
constexpr size_t CustomString<T>::CalculateNewCapacity(const size_t min) const
{
	const size_t oldCap{ Capacity() };
	const size_t maxCap{ MaxSize() };

	if (oldCap > maxCap - oldCap / 2u)
	{
		return maxCap;
	}

	const size_t newCap{ oldCap + oldCap / 2u };

	// If our growth is insufficient, return just the bare minimum
	if (newCap < min)
	{
		return min;
	}

	return newCap;
}

template<typename T>
constexpr void CustomString<T>::Release(T*& pData)
{
	if (pData)
	{
		delete[] pData;
		pData = nullptr;
	}
}

template<typename T>
constexpr void CustomString<T>::DeleteData(T* head, T* const tail) const
{
	if constexpr (!std::is_trivially_destructible_v<T>)
	{
		while (head < tail)
		{
			head->~T();
			++head;
		}
	}
}

#pragma endregion

#pragma region Helpers

template<typename T>
size_t CustomString<T>::CountRawString(const T* pStr) const
{
	// Count length of null-terminated string
	assert(pStr != nullptr);

	size_t counter{};
	while (*pStr != T())
	{
		++counter;
		++pStr;
	}

	return ++counter;
}

#pragma endregion