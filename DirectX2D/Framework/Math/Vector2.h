#pragma once

struct Vector2 : public Float2
{
	Vector2() = default;
	Vector2(float x, float y) : Float2(x, y) {}
	Vector2(POINT point) : Float2(static_cast<float>(point.x), static_cast<float>(point.y)) {}
	Vector2(Float2 point) : Float2(point.x, point.y) {}

	bool operator==(const Vector2& value) const {
		return x == value.x && y == value.y;
	}

	Vector2 operator+(const Vector2& value) const {
		return Vector2(x + value.x, y + value.y);
	}

	Vector2 operator-(const Vector2& value) const {
		return Vector2(x - value.x, y - value.y);
	}

	Vector2 operator*(const Vector2& value) const
	{
		return Vector2(x * value.x, y * value.y);
	}
	Vector2 operator/(const Vector2& value) const
	{
		return Vector2(x / value.x, y / value.y);
	}

	Vector2 operator*(const float& value) const {
		return Vector2(x * value, y * value);
	}
	
	Vector2 operator/(const float& value) const {
		return Vector2(x / value, y / value);
	}

	void operator+=(const Vector2& value) {
		x += value.x;
		y += value.y;
	}

	void operator-=(const Vector2& value) {
		x -= value.x;
		y -= value.y;
	}

	void operator*=(const float& value) {
		x *= value;
		y *= value;
	}

	void operator/=(const float& value) {
		x /= value;
		y /= value;
	}

	Vector2 operator*(const Matrix& value) {
		Float2 coord(x, y);
		XMVECTOR temp = XMLoadFloat2(&coord);

		//XMVector2TransformCoord : w -> 1 => 위치 이동 반영
		//XMVector2TransformNormal : w -> 0 => 위치 이동 없음
		temp = XMVector2TransformCoord(temp, value);
		XMStoreFloat2(&coord, temp);
		return coord;
	}

	void operator*=(const Matrix& value) {
		Float2 coord(x, y);
		XMVECTOR temp = XMLoadFloat2(&coord);

		//XMVector2TransformCoord : w -> 1 => 위치 이동 반영
		//XMVector2TransformNormal : w -> 0 => 위치 이동 없음
		temp = XMVector2TransformCoord(temp, value);
		XMStoreFloat2(&coord, temp);

		x = coord.x;
		y = coord.y;

	}

	bool operator<(const Vector2& vec) {
		return Length() < vec.Length();
	}

	float Length() const {
		return sqrt(x * x + y * y);
	}

	float Cross(Vector2 value) {
		return x * value.y - y * value.x;
	}

	//정규화
	void Normalize() {
		float length = Length();
		x /= length;
		y /= length;
	}

	Vector2 GetNormalized() const {
		float length = Length();
		return Vector2(x / length, y / length);
	}

	float Angle() const {
		return atan2f(y, x);
	}
};

