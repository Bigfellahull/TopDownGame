#include "stdafx.h"

#include "MathHelper.h"
#include <ctime>

using namespace DirectX::SimpleMath;

bool MathHelper::m_randomInit = false;

float MathHelper::Random(float min, float max)
{
	if (!MathHelper::m_randomInit)
	{
		std::srand(static_cast<unsigned>(std::time(0)));
		MathHelper::m_randomInit = true;
	}

	return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}
