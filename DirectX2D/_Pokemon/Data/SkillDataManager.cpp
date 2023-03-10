#include "Framework.h"

SkillDataManager::SkillDataManager()
{
	Load();
	/*
	//테스트 용
	SkillData data;
	data.key = 0;
	data.name = "기본공격";
	data.pokemonType = 0; //???
	data.type = SkillData::SkillType::PHYSICS;
	data.power = 10;
	data.hitRate = 100;
	data.maxPP = -1;
	data.range = SkillData::Range::FRONT;
	*/
}

SkillData* SkillDataManager::GetData(int i)
{
	if (totalDatas.find(i) == totalDatas.end())
		return nullptr;

	return &totalDatas[i];
}

void SkillDataManager::Load()
{
	ifstream ifs;
	ifs.open("TextData/SkillTable.csv");

	string s;
	bool isFirst = true;
	while (!ifs.eof()) {
		s = "";
		ifs >> s;
		if (isFirst) {
			isFirst = false;
			continue;
		}

		if (s.empty())
			break;

		vector<string> vs = SplitString(s, ",");

		SkillData data;
		data.key = stoi(vs[0]);
		data.name = vs[1];
		data.pokemonType = stoi(vs[2]);
		data.type = (SkillData::SkillType)stoi(vs[3]);
		data.power = stoi(vs[4]);
		data.hitRate = stoi(vs[5]);
		data.maxPP = stoi(vs[6]);
		data.range = (SkillData::Range)stoi(vs[7]);
		data.applyTarget = stoi(vs[8]);
		totalDatas[data.key] = data;

		if (vs[9] != "-")
			EffectManager::Get()->Add(data.name, 50, ChangeWString(vs[9]), stof(vs[10]), stof(vs[11]), 5.0f);
	}
}

float SkillDataManager::CalcAdvantage(int skill, int u)
{	
	return typeAdvantage[skill][u];
}
