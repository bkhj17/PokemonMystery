#pragma once

class EnemyTank0126 : public Tank
{
public:
	EnemyTank0126();
	~EnemyTank0126();

	virtual void Update() override;

	virtual void Control() override;
	virtual void HeadControl() override {}

	virtual void Fire() override;

	void Spawn(Vector2 pos);
	void SetTileMap(class TileMap0126* tileMap);
private:
	float shotRate = 2.0f;
	float shotTime = 0.0f;

	float moveDist = 0.0f;
	float moveRate = 50.0f;

	TileMap0126* tileMap = nullptr;
};

