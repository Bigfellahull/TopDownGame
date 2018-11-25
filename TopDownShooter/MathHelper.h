#pragma once

#include <ctime>

class MathHelper
{
public:
	template <typename T>
	static T Random(T min, T max)
	{
		if (!MathHelper::m_randomInit)
		{
			std::srand(static_cast<unsigned>(std::time(0)));
			MathHelper::m_randomInit = true;
		}

		return min + static_cast<T>(std::rand()) / (static_cast<T>(RAND_MAX / (max - min)));
	}
private:
	static bool m_randomInit;
};