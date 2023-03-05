#pragma once

#include <cstring> /* std::memcpy */
#include <assert.h> /* assert() */
#include <limits> /* std::numeric_limits */
#include <utility> /* std::move() */

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

#pragma region RuleOf5

	CustomString(const CustomString& other) noexcept;
	CustomString(CustomString&& other) noexcept;
	CustomString& operator=(const CustomString& other) noexcept;
	CustomString& operator=(CustomString&& other) noexcept;

#pragma endregion

#pragma region Adding_Chars

	CustomString& Assign(const T c, const size_t count);
	CustomString& Assign(const T* pStr);
	CustomString& Assign(const T* pStr, size_t count);
	CustomString& Append(const T c, const size_t count);
	CustomString& operator+=(const T* pStr);
	CustomString& operator+=(const CustomString<T>& other);

#pragma endregion

#pragma region String_Information

	NODISCARD size_t Size() const;
	NODISCARD size_t Capacity() const;
	NODISCARD size_t MaxSize() const;
	NODISCARD const T* Data() const;

#pragma endregion

#pragma region Comparison

	NODISCARD bool operator==(const CustomString& other) const;
	NODISCARD bool operator==(const T* pStr) const;

#pragma endregion

#pragma region String_Manipulation

	CustomString& ToUpper();
	CustomString& ToLower();

#pragma endregion

#pragma region Element_Access

	NODISCARD T& operator[](const size_t index);
	NODISCARD const T& operator[](const size_t index) const;

#pragma endregion

#pragma region Utility

	CustomString Substring(const size_t start, const size_t count = std::numeric_limits<size_t>::max());

#pragma endregion

private:
#pragma region Reallocation

	void Reallocate(const size_t min);
	NODISCARD constexpr size_t CalculateNewCapacity(const size_t min) const;
	constexpr void Release(T*& pData);
	constexpr void DeleteData(T* head, T* const tail) const;

#pragma endregion

#pragma region Helpers

	NODISCARD size_t CountRawString(const T* const pStr) const;

#pragma endregion

	T* m_pHead;
	T* m_pTail;
	T* m_pCurrentEnd; // Points past the end
	size_t m_Size;
};

#pragma region Ctors_Dtors

template<typename T>
CustomString<T>::CustomString(const T c, const size_t count)
	: m_pHead{}
	, m_pTail{}
	, m_pCurrentEnd{}
	, m_Size{}
{
	Assign(c, count);
}

template<typename T>
CustomString<T>::CustomString(const T* pStr)
	: m_pHead{}
	, m_pTail{}
	, m_pCurrentEnd{}
	, m_Size{}
{
	Assign(pStr);
}

template<typename T>
CustomString<T>::~CustomString()
{
	DeleteData(m_pHead, m_pCurrentEnd);
	Release(m_pHead);

	m_Size = 0;
}

#pragma endregion

#pragma region RuleOf5

template<typename T>
CustomString<T>::CustomString(const CustomString& other) noexcept
	: m_pHead{}
	, m_pTail{}
	, m_pCurrentEnd{}
	, m_Size{ other.m_Size }
{
	if (const uint64_t cap{ other.Capacity() }; cap > 0u)
	{
		m_pHead = new T[cap]{};
		m_pTail = m_pHead + cap;

		const uint64_t size{ other.Size() };
		std::memcpy(m_pHead, other.m_pHead, size);

		m_pCurrentEnd = m_pHead + size;
	}
}

template<typename T>
CustomString<T>::CustomString(CustomString&& other) noexcept
	: m_pHead{ std::move(other.m_pHead) }
	, m_pTail{ std::move(other.m_pTail) }
	, m_pCurrentEnd{ std::move(other.m_pCurrentEnd) }
	, m_Size{ std::move(other.m_Size) }
{
	other.m_pHead = nullptr;
	other.m_pTail = nullptr;
	other.m_pCurrentEnd = nullptr;
	other.m_Size = 0;
}

template<typename T>
CustomString<T>& CustomString<T>::operator=(const CustomString& other) noexcept
{
	if (m_pHead)
	{
		DeleteData(m_pHead, m_pCurrentEnd);
		Release(m_pHead);
	}

	m_pHead = nullptr;
	m_pTail = nullptr;
	m_pCurrentEnd = nullptr;
	m_Size = other.m_Size;

	if (const uint64_t cap{ other.Capacity() }; cap > 0u)
	{
		m_pHead = new T[cap]{};
		m_pTail = m_pHead + cap;

		const uint64_t size{ other.Size() };
		std::memcpy(m_pHead, other.m_pHead, size);

		m_pCurrentEnd = m_pHead + size;
	}

	return *this;
}

template<typename T>
CustomString<T>& CustomString<T>::operator=(CustomString&& other) noexcept
{
	if (m_pHead)
	{
		DeleteData(m_pHead, m_pCurrentEnd);
		Release(m_pHead);
	}

	m_pHead = std::move(other.m_pHead);
	m_pTail = std::move(other.m_pTail);
	m_pCurrentEnd = std::move(other.m_pCurrentEnd);
	m_Size = std::move(other.m_Size);

	other.m_pHead = nullptr;
	other.m_pTail = nullptr;
	other.m_pCurrentEnd = nullptr;
	other.m_Size = 0;

	return *this;
}

#pragma endregion

#pragma region Adding_Chars

template<typename T>
CustomString<T>& CustomString<T>::Assign(const T c, const size_t count)
{
	if (!m_pCurrentEnd || (m_pCurrentEnd + count) >= m_pTail)
		Reallocate(count + 1);

	if (m_pCurrentEnd > m_pHead)
		--m_pCurrentEnd; /* If there is a null-terminator make sure we overwrite it */

	for (size_t i{}; i < count; ++i)
		*m_pCurrentEnd++ = c;

	*m_pCurrentEnd++ = T();

	m_Size += count;

	return *this;
}

template<typename T>
CustomString<T>& CustomString<T>::Assign(const T* pStr)
{
	assert(pStr != nullptr);

	return Assign(pStr, CountRawString(pStr));
}

template<typename T>
CustomString<T>& CustomString<T>::Assign(const T* pStr, size_t count)
{
	if (pStr[count - 1] != '\0')
		++count;

	if (!m_pCurrentEnd || (m_pCurrentEnd + count) >= m_pTail)
		Reallocate(count);

	std::memcpy(m_pCurrentEnd, pStr, count);

	m_pCurrentEnd = m_pCurrentEnd + count;
	*(m_pCurrentEnd - 1) = T();

	m_Size += count - 1;

	return *this;
}

template<typename T>
CustomString<T>& CustomString<T>::Append(const T c, const size_t count)
{
	if (!m_pCurrentEnd || (m_pCurrentEnd + count + 1) >= m_pTail)
		Reallocate(m_Size + count + 1);

	if (m_pCurrentEnd > m_pHead)
		--m_pCurrentEnd;

	for (size_t i{}; i < count; ++i)
		*m_pCurrentEnd++ = c;

	*m_pCurrentEnd++ = T();

	m_Size += count;

	return *this;
}

template<typename T>
CustomString<T>& CustomString<T>::operator+=(const T* pStr)
{
	const size_t size{ CountRawString(pStr) };

	if (!m_pCurrentEnd || (m_pCurrentEnd + size) >= m_pTail)
		Reallocate(m_Size + size);

	if (m_pCurrentEnd > m_pHead)
		--m_pCurrentEnd;

	std::memcpy(m_pCurrentEnd, pStr, size);

	m_pCurrentEnd += size;

	m_Size += size - 1;

	return *this;
}

template<typename T>
CustomString<T>& CustomString<T>::operator+=(const CustomString<T>& other)
{
	const size_t size{ other.Size() };

	if (!m_pCurrentEnd || (m_pCurrentEnd + size + 1) >= m_pTail)
		Reallocate(m_Size + size + 1);

	if (m_pCurrentEnd > m_pHead)
		--m_pCurrentEnd;

	std::memcpy(m_pCurrentEnd, other.Data(), size);

	m_pCurrentEnd += size;
	*m_pCurrentEnd++ = T();

	m_Size += size;

	return *this;
}

#pragma endregion

#pragma region String_Information

template<typename T>
size_t CustomString<T>::Size() const
{
	return m_Size;
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
bool CustomString<T>::operator==(const CustomString& other) const
{
	if (Size() != other.Size())
		return false;

	const T* pThisStr{ m_pHead };
	const T* pOtherStr{ other.Data() };

	if (!pThisStr && !pOtherStr)
		return true;

	while ((*pThisStr != T()) && (*pOtherStr != T()))
	{
		if (*pThisStr != *pOtherStr)
			return false;

		++pThisStr;
		++pOtherStr;
	}

	if (*pThisStr != *pOtherStr)
		return false;

	return true;
}

template<typename T>
bool CustomString<T>::operator==(const T* pStr) const
{
	if (Size() + 1 != CountRawString(pStr))
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

#pragma region String_Manipulation

template<typename T>
CustomString<T>& CustomString<T>::ToUpper()
{
	T* pStr{ m_pHead };

	while (pStr != nullptr && *pStr != T())
	{
		if (*pStr >= 'a' && *pStr <= 'z')
			*pStr = *pStr - ('a' - 'A');

		++pStr;
	}

	return *this;
}

template<typename T>
CustomString<T>& CustomString<T>::ToLower()
{
	T* pStr{ m_pHead };

	while (pStr != nullptr && *pStr != T())
	{
		if (*pStr >= 'A' && *pStr <= 'Z')
			*pStr = *pStr + ('a' - 'A');

		++pStr;
	}

	return *this;
}

#pragma endregion

#pragma region Element_Access

template<typename T>
T& CustomString<T>::operator[](const size_t index)
{
	assert(index < Size());

	return *(m_pHead + index);
}

template<typename T>
const T& CustomString<T>::operator[](const size_t index) const
{
	assert(index < Size());

	return *(m_pHead + index);
}

#pragma endregion

#pragma region Utility

template<typename T>
CustomString<T> CustomString<T>::Substring(const size_t start, const size_t count)
{
	assert(start < m_Size);

	CustomString<T> string{};
	size_t substrCounter{};

	while ((substrCounter < count) &&
		(start + substrCounter < m_Size) &&
		(*(m_pHead + start + substrCounter) != T()))
	{
		++substrCounter;
	}

	string.Assign(m_pHead + start, substrCounter);

	return string;
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

	if (m_pCurrentEnd > m_pHead)
		*m_pCurrentEnd++ = T();

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