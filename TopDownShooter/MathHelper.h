#pragma once

class MathHelper
{
public:
	static float Random(float min, float max);
private:
	static bool m_randomInit;
};