#include "Framework.h"

DungeonDataManager::DungeonDataManager()
{
	LoadData("TextData/DungeonData.csv");
}

FloorData* DungeonDataManager::GetData(string key, int floor)
{
	if (dungeonDatas.find(make_pair(key, floor)) == dungeonDatas.end())
		return nullptr;

	return &dungeonDatas[make_pair(key, floor)];
}

void DungeonDataManager::LoadData(string fileName)
{
	ifstream ifs;
	ifs.open(fileName);
	if (ifs.fail())
		return;

	bool isFirstLine = true;
	while (!ifs.eof()) {
		string tmp;
		getline(ifs, tmp);

		if (isFirstLine) {
			isFirstLine = false;
			continue;
		}

		if (tmp.empty())
			break;

		vector<string> v = SplitString(tmp, ",");

		FloorData data;
		data.dungeonKey = v[0];
		data.floorNum = stoi(v[1]);

		data.name = v[2];
		data.name += " ";

		if (data.floorNum < 0)
			data.name += "B";
		data.name += to_string(abs(data.floorNum));
		data.name += "F";

		data.file = v[3];

		data.postFloor = stoi(v[4]);
		data.nextFloor = stoi(v[5]);

		data.items = SplitString(v[6], " ");
		data.minItemNum = stoi(v[7]);
		data.maxItemNum = stoi(v[8]);

		data.minGoldNum = stoi(v[9]);
		data.maxGoldNum = stoi(v[10]);

		auto vMonster = SplitString(v[11], " ");
		auto vLevel = SplitString(v[12], " ");

		for (int i = 0; i < vMonster.size(); i++)
			data.monsters.emplace_back(make_pair(stoi(vMonster[i]), stoi(vLevel[i])));
		data.monsterNum = stoi(v[13]);

		data.traps = SplitString(v[14], " ");
		data.maxTrapNum = stoi(v[15]);

		dungeonDatas[make_pair(data.dungeonKey, data.floorNum)] = data;
	}

}
