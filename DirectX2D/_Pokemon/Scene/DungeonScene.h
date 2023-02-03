#pragma once

class DungeonScene : public Scene
{
public:
	DungeonScene();
	~DungeonScene();


	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;

	virtual void Render() override;

	virtual void PostRender() override;
private:
	class DungeonTileMap* tileMap;
	class Unit* testUnit;
};

