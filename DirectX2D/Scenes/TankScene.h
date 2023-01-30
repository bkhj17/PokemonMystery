#pragma once

class TankScene : public Scene
{
public:
	TankScene();
	~TankScene();

	virtual void Update() override;
	// Scene을(를) 통해 상속됨
	virtual void Render() override;
	virtual void PostRender() override;

private:
	GameTileMap* gameTileMap;
	AStar* astar;
	Tank* tank;
};

