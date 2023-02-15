#pragma once

struct FloorData {
	string dungeonKey = "";
	int floorNum;

	string file;

	string name;

	int postFloor;
	int nextFloor;

	//나오는 몬스터 데이터도 필요
	vector<pair<int, int>> monsters;
	int monsterNum;

	vector<string> items;
	int minItemNum;
	int maxItemNum;

	int minGoldNum;
	int maxGoldNum;

	vector<string> traps;
	int maxTrapNum;
};

class DungeonDataManager : public Singleton<DungeonDataManager>
{
private:
	friend class Singleton;
	DungeonDataManager();
	~DungeonDataManager() = default;

public:
	FloorData* GetData(string key, int floor);

private:
	void LoadData(string fileName);
private:
	map<pair<string, int>, FloorData> dungeonDatas;

	string curDungeon;
	int curFloor;
};

