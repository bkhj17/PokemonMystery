#pragma once

class StatusUI : public GameObject
{
public:
	StatusUI();
	~StatusUI();

	void UpdateInfo();

	void PostRender();

	void UpdateWorld() override;
private:
	void UpdateFloorInfo();
	void UpdatePlayerInfo();
private:
	int floor = 0;
	int playerLevel = 1;
	int playerCurHP = 1;
	int playerMaxHP = 1;

	class HPProgressBar* progressBar;
	ImageFont* curHpFont;
	ImageFont* maxHpFont;
	ImageFont* floorFont;
	ImageFont* levelFont;

	Quad* qB;
	Quad* qF;
	Quad* qLv;
	Quad* qHP;
	Quad* qSlash;
};

