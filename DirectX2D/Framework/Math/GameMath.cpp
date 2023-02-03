#include "Framework.h"
#include "GameMath.h"

int GameMath::Random(const int& min, const int& max) {
	 return rand() % (max - min) + min;
}

float GameMath::Random(const float& min, const float& max)
{
	float normal = rand() / (float)RAND_MAX;	//������ 0.0~1.0������ �Ǽ��� ��ȯ
	return (max - min) * normal + min;			
}

Vector2 GameMath::Random(const Vector2& min, const Vector2& max)
{
	return Vector2(Random(min.x, max.x), Random(min.y, max.y));
}

float GameMath::Clamp(const float& min, const float& max, float value)
{
	if (value < min)
		value = min;

	if (value > max)
		value = max;

	return value;
}

float GameMath::Lerp(const float& state, const float& end, float t)
{
	t = Clamp(0.0f, 1.0f, t);

	return state + t * (end - state);
}

Vector2 GameMath::Lerp(const Vector2& state, const Vector2& end, float t)
{
	t = GameMath::Clamp(0.0f, 1.0f, t);
	return state + (end - state) * t;
}

Vector2 GameMath::SLerp(const Vector2& state, const Vector2& end, float t)
{
	t = GameMath::Clamp(0.0f, 1.0f, t);
	return state + (end - state) * t*t;
}

float GameMath::Cross(const Vector2& vec1, const Vector2& vec2)
{
	return vec1.x * vec2.y - vec2.x * vec1.y;
}

float GameMath::Dot(const Vector2& vec1, const Vector2& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

float GameMath::Distance(const Vector2& vec1, const Vector2& vec2)
{
	return (vec1 - vec2).Length();
}
