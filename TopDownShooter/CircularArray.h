#pragma once

#include <vector>

template <typename T>
class CircularArray
{
public:
	CircularArray(int capacity)
	{
		m_list.resize(capacity);
	}

	int start;
	int count;

	int GetCapacity() { return m_list.size(); }

	T& operator [](const int i)
	{
		return m_list[(start + i) % m_list.size()];
	}

	const T& operator [](const int i) const
	{
		return m_list[(start + i) % m_list.size()];
	}
private:
	std::vector<T> m_list;
};