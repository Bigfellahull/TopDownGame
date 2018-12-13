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
	static DirectX::SimpleMath::Vector2 RotatePoint(DirectX::SimpleMath::Vector2 point, DirectX::SimpleMath::Vector2 centre, float angle)
	{
		float angleInRadians = angle * (3.142f / 180.0f);
		float cosTheta = std::cosf(angleInRadians);
		float sinTheta = std::sinf(angleInRadians);
		return DirectX::SimpleMath::Vector2(
			cosTheta * (point.x - centre.x) - sinTheta * (point.y - centre.y) + centre.x, 
			sinTheta * (point.x - centre.x) + cosTheta * (point.y - centre.y) + centre.y
		);
	}
	static DirectX::SimpleMath::Vector2 NextVector2(float min, float max)
	{
		float theta = MathHelper::Random(0.0f, 1.0f) * 2.0f * 3.142f;
		float length = MathHelper::Random(min, max);
		return DirectX::SimpleMath::Vector2(length * static_cast<float>(std::cosf(theta)), length * static_cast<float>(std::sinf(theta)));
	}
private:
	static bool m_randomInit;
};