#pragma once

class DungeonScene : public Scene
{
public:
	DungeonScene();
	~DungeonScene();


	// Scene을(를) 통해 상속됨
	virtual void Update() override;

	virtual void Render() override;

	virtual void PostRender() override;
private:
	class DungeonTileMap* tileMap;
	class Unit* testUnit;
};

