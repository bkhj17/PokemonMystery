#pragma once

struct SkillData {
	enum SkillType : int{
		PHYSICS,
		SPECIAL,
		TRANS
	};

	enum Range : int {
		FRONT,
		FRONT_THROUGH,
		SHOT,
		ALL_ROOM,
		SELF
	};

	enum ApplyTarget : int {
		ALL,
		ENEMY,
		FRIEND
	};

	int key;
	string name;

	SkillType type;
	Range range; //스킬 범위
	int pokemonType = 0;	//유닛 매니저에 있는 포켓몬 타입
	int applyTarget = 0;

	int power = 10;
	int hitRate = 100;

	int maxPP;
};

class SkillDataManager : public Singleton<SkillDataManager>
{
private:
	friend class Singleton;
	SkillDataManager();
	~SkillDataManager() = default;

public:
	SkillData* GetData(int i);

	float CalcAdvantage(int skillType, int u1);
private:
	void Load();

private:
	map<int, SkillData> totalDatas;

	vector<vector<float>> typeAdvantage = {
		//???	풀	  불		물	  전기	비행	  얼음	바위	  땅		독	  벌레	노말	  격투 에스퍼 고스트	드래곤 악	강철	  페어리		   
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{ 1.0f, 0.5f, 0.5f, 2.0f, 1.0f, 0.5f, 1.0f, 2.0f, 2.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 0.5f, 1.0f},
		{ 1.0f, 2.0f, 0.5f, 0.5f, 1.0f, 0.5f, 2.0f, 0.5f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 2.0f, 1.0f},
		{ 1.0f, 0.5f, 2.0f, 0.5f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f},
		{ 1.0f, 0.5f, 1.0f, 2.0f, 0.5f, 2.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f},
		{ 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f},
		{ 1.0f, 2.0f, 0.5f, 0.5f, 1.0f, 2.0f, 0.5f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 0.5f, 1.0f},
		{ 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 2.0f, 2.0f, 1.0f, 0.5f, 1.0f, 2.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f},
		{ 1.0f, 0.5f, 2.0f, 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f, 2.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f},
		{ 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 0.0f, 2.0f},
		{ 1.0f, 2.0f, 0.5f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 0.5f, 2.0f, 0.5f, 1.0f, 2.0f, 0.5f, 0.5f},
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f},
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 2.0f, 2.0f, 1.0f, 0.5f, 0.5f, 2.0f, 1.0f, 0.5f, 0.0f, 1.0f, 2.0f, 2.0f, 0.5f},
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 2.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.5f, 1.0f},
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 2.0f, 2.0f, 1.0f, 0.5f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 0.5f, 0.0f},
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 2.0f, 2.0f, 1.0f, 0.5f, 1.0f, 0.5f},
		{ 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 2.0f},
		{ 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 2.0f, 2.0f, 0.5f, 1.0f},
	};
};