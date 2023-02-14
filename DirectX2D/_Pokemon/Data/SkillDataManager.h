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
	Range range; //��ų ����
	int pokemonType = 0;	//���� �Ŵ����� �ִ� ���ϸ� Ÿ��
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
		//???	Ǯ	  ��		��	  ����	����	  ����	����	  ��		��	  ����	�븻	  ���� ������ ��Ʈ	�巡�� ��	��ö	  ��		   
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