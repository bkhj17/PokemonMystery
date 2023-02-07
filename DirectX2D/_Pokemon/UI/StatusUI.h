#pragma once

class StatusUI : public GameObject
{
public:
	StatusUI();
	~StatusUI();

private:


private:
	int floor;
	int playerLevel;
	int playerHP;
	ProgressBar* progressBar;
};

