#pragma once

namespace GameMath {
	enum class Direction {
		UP, DOWN, LEFT, RIGHT, NONE
	};

	int Random(const int& min, const int& max);
	float Random(const float& min, const float& max);
	Vector2 Random(const Vector2& min, const Vector2& max);

	float Clamp(const float& min, const float& max, float value);

	float Lerp(const float& state, const float& end, float t);

	Vector2 Lerp(const Vector2& state, const Vector2& end, float t);
	Vector2 SLerp(const Vector2& state, const Vector2& end, float t);

	float Cross(const Vector2& vec1, const Vector2& vec2);
	float Dot(const Vector2& vec1, const Vector2& vec2);

	float Distance(const Vector2& vec1, const Vector2& vec2);
}
