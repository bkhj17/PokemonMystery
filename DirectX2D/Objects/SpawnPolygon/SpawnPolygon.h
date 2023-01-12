#pragma once
class SpawnPolygon : public GameObject
{
private:
	const int MIN_POINT = 3;
	const int MAX_POINT = 10;

	const float MIN_RADIUS = 0.1f;
	const float MAX_RADIUS = 0.3f;

	const float MIN_LIFETIME = 0.1f;
	const float MAX_LIFETIME = 0.2f;
public:
	SpawnPolygon();
	~SpawnPolygon();

	void Update() override;
	void Render() override;

	void Spawn();
public:
	bool isActive = false;
private:
	float lifeTime = 0.0f;
};

