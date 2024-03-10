#include "math.hpp"

float math_additional::Clamp01 ( float value )
{
	float result;
	if (value < 0.f)
	{
		result = 0.f;
	}
	else if (value > 1.f)
	{
		result = 1.f;
	}
	else
	{
		result = value;
	}
	return result;
}

float math_additional::Lerp ( Vector3 in )
{
	return in.x + (in.y - in.x) * Clamp01 ( in.z );
}