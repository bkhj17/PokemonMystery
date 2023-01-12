#pragma once

class Scene0112 : public Scene
{
private:
	const UINT MAX_BULLET = 3000;
public:
	Scene0112();
	~Scene0112();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void ShotBullet(Vector2, float);

private:
	MatrixBuffer* projectionBuffer;

	class Plane0112* plane;

	vector<class Bullet0112*> bullets;
};

